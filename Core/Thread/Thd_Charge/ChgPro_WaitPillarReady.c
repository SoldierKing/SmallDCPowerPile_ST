

#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"
#include "ADrv_AdcVolt.h"
#include "ADrv_Comm.h"

/**************************************************************************************************
** 函数名: GetBatVoltWaitPillarReady
** 输　入: 无
** 输　出: 返回直流口当前电压
** 描  述: 获取直流电压
**************************************************************************************************/
s32 GetBatVoltWaitPillarReady(void)
{
  if(GetBoostENSwitchSta() != GPIO_PIN_SET)
  {
    SwitcBoostENOnOff(eSwSta_On);
    osDelay(30);
  }

  return AdcVoltBuf[EnumVddc_AD3];
}

//根据IMD电压判断车端电池是否反接
s32 IsBatPnCorrect(void)
{
  
  if(GPIO_PIN_RESET == GetVbatReverseSta())
    return -1;
  return 1;
}

void ChgProWaitPillarReady(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  CRO_SendTick = 0;
  u8  CroStarus = 0x00;
  u32 CcStatusChkTick = 0;
  u32 VoltReadyTick = 0;

  BemMsg.Valid = 0;

  ChargeTimerSet(eSwSta_Off);
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);

  DcOutSet(eSwSta_Off);

  CroStarus = 0x00;
  CRO_Send(CroStarus);
  CRO_SendTick = GetSystemTick();

  if((BcpMsg.StartVolt >= DcModuleAbility.MinVolt) && (BcpMsg.StartVolt <= DcModuleAbility.MaxVolt))
    DcModuleSet(eSwSta_On, BcpMsg.StartVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
  else if(BcpMsg.StartVolt < DcModuleAbility.MinVolt)
    DcModuleSet(eSwSta_On, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
  else
  {
    CstMsg.StopReason.Value.Truble = 1;
    CstMsg.StopTruble.Value.Other = 1;
    CstMsg.StopFault.Value.Volt = 1;
    DcOutSet(eSwSta_Off);
    DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
    ChargeStopType = eChgStop_PillarReady_BatVoltOverPillarMax;
    pChargeCtrl->CurProcess = eChgPro_ChgStop;
    pChargeCtrl->Param = 0;
    return;
  }
  VoltReadyTick = GetSystemTick();

  while(1)
  {
    
    if((GetSystemTick() - CRO_SendTick) > 250)
    {
      CRO_Send(CroStarus);
      CRO_SendTick = GetSystemTick();
    }
    if( 0x00 == BroMsg.Status)
    {//接收到Brm
      CemMsg.Cem.Value.Other = 1;
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      pChargeCtrl->CurProcess = eChgPro_CommError;
      pChargeCtrl->Param = 0;
    }

    ChgPgnRecv(&PgnRecv);

    if((eChgVer_2011 != UserParam.ChgVer) && (eChgVer_2011 != ChgVer))
    {

      if((0 != VoltReadyTick) && ((GetSystemTick() - VoltReadyTick) > (MsgTimeout_5S/2)))
      {
        if(-1 == IsBatPnCorrect())
        {
          CstMsg.StopReason.Value.Truble = 1;
          CstMsg.StopTruble.Value.Other = 1;
          CstMsg.StopFault.Value.Volt = 1;
          DcOutSet(eSwSta_Off);
          DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
          ChargeStopType = eChgStop_PillarReady_BatVoltError;
          pChargeCtrl->CurProcess = eChgPro_ChgStop;
          pChargeCtrl->Param = 0;
        }

        else if((eChgVer_2011 != UserParam.ChgVer)
                && ((abs(GetBatVoltWaitPillarReady() - BcpMsg.StartVolt)) > (BcpMsg.StartVolt*5/100)))
        {
          CstMsg.StopReason.Value.Truble = 1;
          CstMsg.StopTruble.Value.Other = 1;
          CstMsg.StopFault.Value.Volt = 1;
          DcOutSet(eSwSta_Off);
          DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
          ChargeStopType = eChgStop_PillarReady_BatVoltMismatchBcpVolt;
          pChargeCtrl->CurProcess = eChgPro_ChgStop;
          pChargeCtrl->Param = 0;
        }

        else if(abs(GetDcModuleVolt()- GetBatVoltWaitPillarReady()) <= 100)
        {
          VoltReadyTick = 0;
          pChargeCtrl->CurProcess = eChgPro_ChgStart;
          pChargeCtrl->Param = 0;
        }
        else
        {
          if((GetBatVoltWaitPillarReady() >= DcModuleAbility.MinVolt) && (GetBatVoltWaitPillarReady() <= DcModuleAbility.MaxVolt))
            DcModuleSet(eSwSta_On, GetBatVoltWaitPillarReady(), DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
          else if(GetBatVoltWaitPillarReady() < DcModuleAbility.MinVolt)
            DcModuleSet(eSwSta_On, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        }
      }
      if((0 != VoltReadyTick) && ((GetSystemTick() - VoltReadyTick) > (MsgTimeout_60S)))
      {
         /*VoltReadyTick = 0;
        pChargeCtrl->CurProcess = eChgPro_ChgStart;
        pChargeCtrl->Param = 0;*/
        CstMsg.StopReason.Value.Truble = 1;
        CstMsg.StopTruble.Value.Other = 1;
        CstMsg.StopFault.Value.Volt = 1;
        DcOutSet(eSwSta_Off);
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        ChargeStopType = eChgStop_Conf_ChgMaxVoltLessPillarMinVolt;
        pChargeCtrl->CurProcess = eChgPro_ChgStop;
        pChargeCtrl->Param = 0;
      }
    }
/*
    else if(eChgVer_2011 != ChgVer)
    {
      if((0 != VoltReadyTick) && ((GetSystemTick() - VoltReadyTick) > (MsgTimeout_10S)))
      {
        VoltReadyTick = 0;
        pChargeCtrl->CurProcess = eChgPro_ChgStart;
        pChargeCtrl->Param = 0;
      }
      else if(abs(GetDcModuleVolt()- GetBatVoltWaitPillarReady()) <= 100)
      {
        VoltReadyTick = 0;
        pChargeCtrl->CurProcess = eChgPro_ChgStart;
        pChargeCtrl->Param = 0;
      }
      else
      {
        if((GetBatVoltWaitPillarReady() >= DcModuleAbility.MinVolt) && (GetBatVoltWaitPillarReady() <= DcModuleAbility.MaxVolt))
          DcModuleSet_B(eSwSta_On, GetBatVoltWaitPillarReady(), DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        else if(GetBatVoltWaitPillarReady() < DcModuleAbility.MinVolt)
          DcModuleSet(eSwSta_On, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      }
    }
*/
    else
    {
      VoltReadyTick = 0;
      pChargeCtrl->CurProcess = eChgPro_ChgStart;
      pChargeCtrl->Param = 0;
    }

    if((GetSystemTick() - BroMsg.RecvTick) > MsgTimeout_5S)
    {
      CemMsg.Cem.Value.Bro = 1;
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      pChargeCtrl->CurProcess = eChgPro_CommBreak;
      pChargeCtrl->Param = 0;
    }
    else
      CemMsg.Cem.Value.Bro = 0;

    if(0 != BemMsg.Valid)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      pChargeCtrl->CurProcess = eChgPro_CommBreak;
      pChargeCtrl->Param = 0;
    }

    if(0 == ChargeEnable)
    {//外部控制结束充电
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      ChargeStopType = eChgStop_PillarReady_UserCancel;
      pChargeCtrl->CurProcess = eChgPro_ChgStop;
      pChargeCtrl->Param = 0;
    }

    if(eSwSta_On != CcStatusRead())
    {
      if(0 == CcStatusChkTick)
        CcStatusChkTick = GetSystemTick();
    }
    else
      CcStatusChkTick = 0;
    if((0 != CcStatusChkTick) && ((GetSystemTick() - CcStatusChkTick) > CC_CheckTick * 2))
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      CcStatusChkTick = 0;
      ChargeStopType = eChgStop_PillarReady_CcOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Emergency)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      ChargeStopType = eChgStop_PillarReady_EmerClose;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Door)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      ChargeStopType = eChgStop_PillarReady_DoorOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Contactor)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      ChargeStopType = eChgStop_PillarReady_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.UniContactor)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      ChargeStopType = eChgStop_PillarReady_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.InVolt)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      ChargeStopType = eChgStop_PillarReady_InVolt;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.NoDcmod)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      ChargeStopType = eChgStop_PillarReady_NoDcmod;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Meter)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      ChargeStopType = eChgStop_PillarReady_Meter;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    if(0 != PillarError.Value.ACInputOut)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
      CstMsg.StopFault.Value.Volt = 1;
      ChargeStopType = eChgStop_Fault_ACInputOut;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    if (ResMsg.Valid != 0 && ResMsg.V2LDischargeReq.Value.V2lDisChargeSta != 4)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      CstMsg.StopTruble.Value.Other = 1;
	  ChargeStopType = eChgStop_VEH_RES_WorkModleErr;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    
    if(eChgPro_WaitPillarReady != pChargeCtrl->CurProcess)
      return;
    osDelay(5);
  }
}



