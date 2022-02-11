

#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "ADrv_AdcVolt.h"

/**************************************************************************************************
** 函数名: GetBatVoltWaitPillarReady
** 输　入: 无
** 输　出: 返回直流口当前电压
** 描  述: 获取直流电压
**************************************************************************************************/
s32 GetDisBatVoltWaitPillarReady(void)
{
  //if(GetBoostENSwitchSta() != GPIO_PIN_SET)
  //{
    //SwitcBoostENOnOff(eSwSta_On);
    //osDelay(30);
  //}

  return AdcVoltBuf[EnumVddc_AD3];
}


//根据IMD电压判断车端电池是否反接
s32 IsDisBatPnCorrect(void)
{
  
  if(GPIO_PIN_RESET == GetVbatReverseSta())
    return -1;
  return 1;
}

void DisChgProWaitPillarReady(StructChargeCtrl *pChargeCtrl)
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

  //BRO准备OK后直接打开高压继电器
  DcOutSet(eSwSta_On);

  CroStarus = 0x00;
  CRO_Send(CroStarus);
  CRO_SendTick = GetSystemTick();
  
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

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
      pChargeCtrl->CurProcess = eChgPro_DisCommError;
      pChargeCtrl->Param = 0;
    }

	if((0 != V2L_SendTick) && ((GetSystemTick() - V2L_SendTick) > 100)
		&& (CarTypeVar == NioCar_Type))
    {
    
      V2L_Send();
      V2L_SendTick = GetSystemTick();
    }

    ChgPgnRecv(&PgnRecv);

	
    if(1)//(eChgVer_2011 != UserParam.ChgVer) && (eChgVer_2011 != ChgVer))
    {

      if((0 != VoltReadyTick) && ((GetSystemTick() - VoltReadyTick) > (MsgTimeout_5S/2)))
      {
        //放电过程中，检测电池反接；
        /*
        if(-1 == IsDisBatPnCorrect())
        {
          CstMsg.StopReason.Value.Truble = 1;
          CstMsg.StopTruble.Value.Other = 1;
          CstMsg.StopFault.Value.Volt = 1;
          DcOutSet(eSwSta_Off);
          DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
          DisChargeStopType = eChgStop_PillarReady_BatVoltError;
          pChargeCtrl->CurProcess = eChgPro_DisChgStop;
          pChargeCtrl->Param = 0;
        }

        else */if(abs(GetDcModuleVolt()- GetDisBatVoltWaitPillarReady()) <= 100)
        {
          VoltReadyTick = 0;
          pChargeCtrl->CurProcess = eChgPro_DisChgStart;
          pChargeCtrl->Param = 0;
        }
        else
        {
          
          if((GetDisBatVoltWaitPillarReady() >= DcModuleAbility.MinVolt) && (GetDisBatVoltWaitPillarReady() <= DcModuleAbility.MaxVolt))
            DcModuleSet(eSwSta_On, GetDisBatVoltWaitPillarReady(), DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
          else if(GetDisBatVoltWaitPillarReady() < DcModuleAbility.MinVolt)
            DcModuleSet(eSwSta_On, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        }
      }
      if((0 != VoltReadyTick) && ((GetSystemTick() - VoltReadyTick) > (MsgTimeout_60S)))
      {

        CstMsg.StopReason.Value.Truble = 1;
        CstMsg.StopTruble.Value.Other = 1;
        CstMsg.StopFault.Value.Volt = 1;
        DcOutSet(eSwSta_Off);
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        DisChargeStopType = eChgStop_Conf_ChgMaxVoltLessPillarMinVolt;
        pChargeCtrl->CurProcess = eChgPro_DisChgStop;
        pChargeCtrl->Param = 0;
      }	  
    }

	/*
    else
    {
      VoltReadyTick = 0;
      pChargeCtrl->CurProcess = eChgPro_DisChgStart;
      pChargeCtrl->Param = 0;
    }
    */
    if((GetSystemTick() - BroMsg.RecvTick) > MsgTimeout_5S)
    {
      CemMsg.Cem.Value.Bro = 1;
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      pChargeCtrl->CurProcess = eChgPro_DisCommBreak;
      pChargeCtrl->Param = 0;
    }
    else
      CemMsg.Cem.Value.Bro = 0;

    if(0 != BemMsg.Valid)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      pChargeCtrl->CurProcess = eChgPro_DisCommBreak;
      pChargeCtrl->Param = 0;
    }

    if(0 == DisChargeEnable)
    {//外部控制结束充电
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      if (DisChargeStopType != eChgStop_Stop_ByKeyTurnOff)
        DisChargeStopType = eChgStop_PillarReady_UserCancel;
      pChargeCtrl->CurProcess = eChgPro_DisChgStop;
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
      DisChargeStopType = eChgStop_PillarReady_CcOpen;
      pChargeCtrl->CurProcess = eChgPro_DisChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    if(0 != PillarError.Value.NoDcmod)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DisChargeStopType = eChgStop_PillarReady_NoDcmod;
      pChargeCtrl->CurProcess = eChgPro_DisChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    if(eChgPro_DisWaitPillarReady != pChargeCtrl->CurProcess)
      return;
    osDelay(5);
  }
}



