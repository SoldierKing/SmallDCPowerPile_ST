

#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "ADrv_AdcVolt.h"



s32 GetChgVoltChgRun(void)
{
  
  if(GetBoostENSwitchSta() != GPIO_PIN_SET)
  {
    SwitcBoostENOnOff(eSwSta_On);
    osDelay(10);
  }
  return AdcVoltBuf[EnumVddc_AD3];
  
  //if(BcsMsg.Valid)
    //return BcsMsg.ChgVolt;
  //return -1;
}

s32 GetChgCurrtChgRun(void)
{
  
  if(PillarError.Value.NoDcmod == 0)
    return GetDcModuleCurrt();
  if(BcsMsg.Valid)
    return abs((s32)BcsMsg.ChgCurrt - 4000);
  return -1;
}

void ChgProChgRun(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  u32 CCS_SendTick = 0;
  u32 CcStatusChkTick = 0;
  u32 SocStopTick = 0;

  u16 MaxCurrt = 0;
  u16 NeedCurrt = 0;

  u16 MaxVolt = 0;
  u16 NeedVolt = 0;

  u16 CcsChgEnable = 0;

  u32 BmsPauseTick = 0;

  s32 Temp = 0;
  u32 CurrtOutChkTick = 0;


  memset(&BemMsg, 0, sizeof(BemMsg));
  memset(&CemMsg, 0, sizeof(CemMsg));
  memset(&BstMsg, 0, sizeof(BstMsg));
  memset(&CstMsg, 0, sizeof(CstMsg));
  memset(&BsdMsg, 0, sizeof(BsdMsg));
  memset(&CsdMsg, 0, sizeof(CsdMsg));

  memset(&BmvMsg, 0xFF, sizeof(BmvMsg));
  memset(&BmtMsg, 0xFF, sizeof(BmtMsg));
  BmvMsg.Valid = 0;
  BmtMsg.Valid = 0;

  BemMsg.Valid = 0;
  BstMsg.Valid = 0;
  BmsCanInit();
  osDelay(20);

  ChargeTimerSet(eSwSta_On);
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);
 
  DcOutSet(eSwSta_On);

  CcsChgEnable = 1;
  CCS_Send(AdcVoltBuf[EnumVddc_AD3], (4000-GetDcModuleCurrt()), GetChgMin(), CcsChgEnable);
  CCS_SendTick = GetSystemTick();
  BclMsg.RecvTick = GetSystemTick();
  BcsMsg.RecvTick = GetSystemTick();

  BmsPauseTick = 0;

  CurrtOutChkTick = 0;

  UniChgEnable = FALSE;


  while(1)
  {
    if((GetSystemTick() - CCS_SendTick) > 45)
    {
      
      CCS_Send( AdcVoltBuf[EnumVddc_AD3], (4000- GetDcModuleCurrt()), GetChgMin(), CcsChgEnable);      
      CCS_SendTick = GetSystemTick();
    }

    ChgPgnRecv(&PgnRecv);
    
    /*非联充模式*/
    if(FALSE == UniChgEnable)
    {
      if((0 != ChargePauseFlag) || (0 != BmsPauseTick))
      {
        CcsChgEnable = 0;
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        ChargeTimerSet(eSwSta_Off);
      }
      else if(0 != BclMsg.Valid)
      {
        if(eChgVer_2011 != UserParam.ChgVer)
          MaxCurrt = abs((s32)BcpMsg.ChgMaxCurrt - 4000);
        else
          MaxCurrt = abs((s32)BclMsg.NeedCurrt - 4000);

        NeedCurrt = abs((s32)BclMsg.NeedCurrt - 4000);
        {
          if(NeedCurrt > MaxCurrt)
            NeedCurrt = MaxCurrt;
          if(NeedCurrt > (DcModuleAbility.MaxCurrt*DcModuleManage.ValidNum))
            NeedCurrt = DcModuleAbility.MaxCurrt*DcModuleManage.ValidNum;

          MaxVolt = BcpMsg.ChgMaxVolt;
          NeedVolt = BclMsg.NeedVolt;
          if(NeedVolt > MaxVolt)
            NeedVolt = MaxVolt;
          if(NeedVolt > DcModuleAbility.MaxVolt)
            NeedVolt = DcModuleAbility.MaxVolt;

          CcsChgEnable = 1;
          DcModuleSet(eSwSta_On, NeedVolt, NeedCurrt);
          ChargeTimerSet(eSwSta_On);
        }
      }
    /*电压异常判断*/
    if((eChgVer_2011 != UserParam.ChgVer) && (eChgVer_2011 != ChgVer))
    {
      Temp = GetChgVoltChgRun();
      if((0 != CcsChgEnable) && (Temp >= DcModuleAbility.MinVolt) && (Temp > GetDcModuleVoltSet()))
      {
        if((Temp - GetDcModuleVoltSet()) > (GetDcModuleVoltSet()*5/100))
        {
          CstMsg.StopReason.Value.Truble = 1;
          CstMsg.StopTruble.Value.Other = 1;
          CstMsg.StopFault.Value.Volt = 1;
          ChargeError.Value.VoltOutError = 1;
          DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
          DcOutSet(eSwSta_Off);
          ChargeStopType = eChgStop_Run_VoltOutHighErr;
          pChargeCtrl->CurProcess = eChgPro_ChgStop;
          //pChargeCtrl->CurProcess = eChgPro_ChgBreak;
          pChargeCtrl->Param = 0;
        }
      }
    }
      /*电流异常判断*/
      if((eChgVer_2011 != UserParam.ChgVer) && (eChgVer_2011 != ChgVer))
      {
        Temp = GetChgCurrtChgRun();
        if((0 != CcsChgEnable) && (Temp > (DcModuleAbility.MinCurrt*DcModuleManage.ValidNum)) && (Temp > GetDcModuleCurrtSet()))
        {
          if((Temp - GetDcModuleCurrtSet()) > (GetDcModuleCurrtSet()*5/100))
          {
            if(0 == CurrtOutChkTick)
              CurrtOutChkTick = GetSystemTick();
          }
          else
            CurrtOutChkTick = 0;
        }
        else
          CurrtOutChkTick = 0;
        if((0 != CurrtOutChkTick) && ((GetSystemTick() - CurrtOutChkTick) > 10000))
        {
          CurrtOutChkTick = 0;
          CstMsg.StopReason.Value.Truble = 1;
          CstMsg.StopTruble.Value.Other = 1;
          CstMsg.StopFault.Value.Currt = 1;
          ChargeError.Value.CurrtOutError = 1;
          DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
          DcOutSet(eSwSta_Off);
          ChargeStopType = eChgStop_Run_CurrtOutHighErr;
          pChargeCtrl->CurProcess = eChgPro_ChgStop;
          pChargeCtrl->Param = 0;
        }
      }
    } 
    
    
    if(0 != BsmMsg.Valid)
    {
      if(0x00 != BsmMsg.BatSta.Value.Volt)
      {
        CstMsg.StopReason.Value.Truble = 1;
        CstMsg.StopFault.Value.Volt = 1;
       // CST_Send(&CstMsg);
        
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        UniChgEnable = FALSE;
        ChargeTimerSet(eSwSta_Off);
        ChargeStopType = eChgStop_Run_SinglBatVolt;
        pChargeCtrl->CurProcess = eChgPro_ChgStop;
        pChargeCtrl->Param = 0;
      }
      else if(0x00 != BsmMsg.BatSta.Value.Soc)
      {
        CstMsg.StopReason.Value.Truble = 1;
        CstMsg.StopTruble.Value.Other = 1;
        //CST_Send(&CstMsg);
        
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

        UniChgEnable = FALSE;
        ChargeTimerSet(eSwSta_Off);
        ChargeStopType = eChgStop_Run_SocOver;
        pChargeCtrl->CurProcess = eChgPro_ChgStop;
        pChargeCtrl->Param = 0;
      }
      else if(0x01 == BsmMsg.BatSta.Value.Currt)
      {
        CstMsg.StopReason.Value.Truble = 1;
        CstMsg.StopFault.Value.Currt = 1;
        //CST_Send(&CstMsg);

        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

        UniChgEnable = FALSE;
        ChargeTimerSet(eSwSta_Off);
        ChargeStopType = eChgStop_Run_CurrtOver;
        pChargeCtrl->CurProcess = eChgPro_ChgStop;
        pChargeCtrl->Param = 0;
      }
      else if(0x01 == BsmMsg.BatSta.Value.Temp)
      {
        CstMsg.StopReason.Value.Truble = 1;
        CstMsg.StopTruble.Value.Other = 1;
       // CST_Send(&CstMsg);

        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

        UniChgEnable = FALSE;
        ChargeTimerSet(eSwSta_Off);
        ChargeStopType = eChgStop_Run_BatTempOver;
        pChargeCtrl->CurProcess = eChgPro_ChgStop;
        pChargeCtrl->Param = 0;
      }
      if(0x01 == BsmMsg.ChgSta.Value.Imd)
      {
        CstMsg.StopReason.Value.Truble = 1;
        CstMsg.StopTruble.Value.Other = 1;
        //CST_Send(&CstMsg);

        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        
        UniChgEnable = FALSE;
        ChargeTimerSet(eSwSta_Off);
        ChargeStopType = eChgStop_Run_BatIsolate;
        pChargeCtrl->CurProcess = eChgPro_ChgStop;
        pChargeCtrl->Param = 0;
      }
      if(0x01 == BsmMsg.ChgSta.Value.Connect)
      {
        CstMsg.StopReason.Value.Truble = 1;
        CstMsg.StopTruble.Value.Other = 1;
        //CST_Send(&CstMsg);
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

        UniChgEnable = FALSE;
        ChargeTimerSet(eSwSta_Off);
        ChargeStopType = eChgStop_Run_Cc2Error;
        pChargeCtrl->CurProcess = eChgPro_ChgStop;
        pChargeCtrl->Param = 0;
      }
      else if(0x00 == BsmMsg.ChgSta.Value.ChgEnable)
      {
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

        UniChgEnable = FALSE;
        ChargeTimerSet(eSwSta_Off);
        if(0 == BmsPauseTick)
          BmsPauseTick = GetSystemTick();
        if((0 != BmsPauseTick) && ((GetSystemTick() - BmsPauseTick) > MsgTimeout_10M))
        {
          CstMsg.StopReason.Value.Truble = 1;
          CstMsg.StopTruble.Value.Other = 1;
         // CST_Send(&CstMsg);

          DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
          
          UniChgEnable = FALSE;
          ChargeTimerSet(eSwSta_Off);
          ChargeStopType = eChgStop_Run_BmsPause;
          pChargeCtrl->CurProcess = eChgPro_ChgStop;
          pChargeCtrl->Param = 0;
        }
      }
      else if(0x01 == BsmMsg.ChgSta.Value.ChgEnable)
        BmsPauseTick = 0;
    }

    if((GetSystemTick() - BclMsg.RecvTick) > MsgTimeout_1S)
    {
      CemMsg.Cem.Value.Bcl = 1;
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      UniChgEnable = FALSE;
      ChargeTimerSet(eSwSta_Off);
      pChargeCtrl->CurProcess = eChgPro_CommBreak;
      pChargeCtrl->Param = 0;
    }
    else
      CemMsg.Cem.Value.Bcl = 0;
	
	if(((GetSystemTick() - BcsMsg.RecvTick) >1000+500)&&(eChgVer_2011 == UserParam.ChgVer))
	{
          StructChargeSend FrameSend;

          FrameSend.Id.Mult = 0;
          FrameSend.Id.Disp.Prio = 7;
          FrameSend.Id.Disp.PF = TPCM>>8;
          FrameSend.Id.Disp.DA_PS = Card1939Addr;
          FrameSend.Id.Disp.SA = Charger1939Addr;
          FrameSend.Data[0] = 0x13;
          FrameSend.Data[1] = 0x09;
          FrameSend.Data[2] = 0x00;
          FrameSend.Data[3] = 0x02;
          FrameSend.Data[4] = 0xFe;
          FrameSend.Data[5] = 0x00;
          FrameSend.Data[6] = 0x11;
          FrameSend.Data[7] = 0x00;
          FrameSend.Len = 8;
    
          BmsCommSendMsg(&FrameSend);
	  BcsMsg.RecvTick= GetSystemTick(); 
	}
	else
	  CemMsg.Cem.Value.Bcs = 0;//开沃丢帧补偿

    if((GetSystemTick() - BcsMsg.RecvTick) > MsgTimeout_5S)
    {
      CemMsg.Cem.Value.Bcs = 1;

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      
      UniChgEnable = FALSE;
      ChargeTimerSet(eSwSta_Off);
      pChargeCtrl->CurProcess = eChgPro_CommBreak;
      pChargeCtrl->Param = 0;
    }
    else
      CemMsg.Cem.Value.Bcs = 0;

    if(0 != BemMsg.Valid)
    {

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      UniChgEnable = FALSE;
      ChargeTimerSet(eSwSta_Off);
      pChargeCtrl->CurProcess = eChgPro_CommBreak;
      pChargeCtrl->Param = 0;
    }

    if(0 != BstMsg.Valid)
    {
      CstMsg.StopReason.Value.BmsStop = 1;

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      
      UniChgEnable = FALSE;
      ChargeTimerSet(eSwSta_Off);
      ChargeStopType = eChgStop_Run_BmsStop;
      pChargeCtrl->CurProcess = eChgPro_ChgStop;
      pChargeCtrl->Param = 0;
    }
    if(0 == ChargeEnable)
    {//外部控制结束充电
      CstMsg.StopReason.Value.Manual = 1;
      //CST_Send(&CstMsg);

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      UniChgEnable = FALSE;
      ChargeTimerSet(eSwSta_Off);
      ChargeStopType = eChgStop_Run_UserCancel;
      pChargeCtrl->CurProcess = eChgPro_ChgStop;
      pChargeCtrl->Param = 0;
    }

    if((BcsMsg.Valid) && (BcsMsg.CurSoc >= 100))
    {
      if(0 == SocStopTick)
        SocStopTick = GetSystemTick();
    }
    else
      SocStopTick = 0;
    
    if((0 != SocStopTick) && ((GetSystemTick() - SocStopTick) > (MsgTimeout_60S*2)))
    {//外部控制结束充电
      CstMsg.StopReason.Value.AutoSet = 1;
     // CST_Send(&CstMsg);
      
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      UniChgEnable = FALSE;
      ChargeTimerSet(eSwSta_Off);
      ChargeStopType = eChgStop_Run_Soc98;
      pChargeCtrl->CurProcess = eChgPro_ChgStop;
      pChargeCtrl->Param = 0;
    }

    if((BcsMsg.Valid) && (BcsMsg.BatVoltAndGroup.Value.Volt > BcpMsg.PerBatMaxVolt) && (1 == UserParam.ChgFun.Value.BatCellJud))
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopFault.Value.Volt = 1;
     // CST_Send(&CstMsg);

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      
      UniChgEnable = FALSE;
      ChargeTimerSet(eSwSta_Off);
      ChargeStopType = eChgStop_Run_SinglBatVoltOverPermitMax;
      pChargeCtrl->CurProcess = eChgPro_ChgStop;
      pChargeCtrl->Param = 0;
    }

   // if((eSwSta_On != CcStatusRead())&&( ChargeStopType != eChgStop_None))
    if(eSwSta_On != CcStatusRead())
    {
      if(0 == CcStatusChkTick)
        CcStatusChkTick = GetSystemTick();
    }
    else
      CcStatusChkTick = 0;
    if((0 != CcStatusChkTick) && ((GetSystemTick() - CcStatusChkTick) > CC_CheckTick))
    {
      //CstMsg.StopReason.Value.Truble = 1;
      //CstMsg.StopTruble.Value.ChgGun = 1;
      CstMsg.StopReason.Value.Manual = 1;
     // CST_Send(&CstMsg);

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      
      DcOutSet(eSwSta_Off);
      UniChgEnable = FALSE;
      CcStatusChkTick = 0;
      ChargeStopType = eChgStop_Run_CcOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Emergency)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Emergency = 1;
      //CST_Send(&CstMsg);

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      DcOutSet(eSwSta_Off);
      UniChgEnable = FALSE;
      ChargeStopType = eChgStop_Run_EmerClose;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Door)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
      //CST_Send(&CstMsg);

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      DcOutSet(eSwSta_Off);
      UniChgEnable = FALSE;
      ChargeStopType = eChgStop_Run_DoorOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Contactor)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
     // CST_Send(&CstMsg);

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      DcOutSet(eSwSta_Off);
      UniChgEnable = FALSE;
      ChargeStopType = eChgStop_Run_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.UniContactor)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
      //CST_Send(&CstMsg);

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      DcOutSet(eSwSta_Off);
      UniChgEnable = FALSE;
      ChargeStopType = eChgStop_Run_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.InVolt)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
      //CST_Send(&CstMsg);

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      DcOutSet(eSwSta_Off);
      UniChgEnable = FALSE;
      ChargeStopType = eChgStop_Run_InVolt;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.NoDcmod)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
     // CST_Send(&CstMsg);

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      DcOutSet(eSwSta_Off);
      UniChgEnable = FALSE;
      ChargeStopType = eChgStop_Run_NoDcmod;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Meter)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
     // CST_Send(&CstMsg);

      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

      DcOutSet(eSwSta_Off);
      UniChgEnable = FALSE;
      ChargeStopType = eChgStop_Run_Meter;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    if (ResMsg.Valid != 0 && ResMsg.V2LDischargeReq.Value.V2lDisChargeSta != 4)
    {
      CstMsg.StopTruble.Value.Other = 1;
	  ChargeStopType = eChgStop_VEH_RES_WorkModleErr;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    
    if(eChgPro_ChgRun != pChargeCtrl->CurProcess)
      return;
    osDelay(5);
  }
}




