
#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"
#include "ADrv_AdcVolt.h"


void DisChgProChgBreak(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  u32 CST_SendTick = 0;
  u32 DrainResistTick = 0;
  u8 ChgEndFlag = 0;
  u32 DtcSendTick = 0;
  u32 DtcSendCnt = 0;
  u32 Dtc = 0;
  u32 CstSendCnt = 0;
  u32 CcStatusChkTick = 0;
  u8 GunOutFlag = 0;
  u32 SleepTick = 0;


  ChargeTimerSet(eSwSta_Off);
  BmsPowerSet(eSwSta_On);
  //BmsPowerSet(eSwSta_Off);
  LockSet(eSwSta_On);
  DcOutSet(eSwSta_Off);
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  CST_Send(&CstMsg);
  CST_SendTick = GetSystemTick();
  CstSendCnt = 0;

  DtcSendCnt = 0;
  DtcSendTick = GetSystemTick();

  DrainResistTick = GetSystemTick();

  Dtc = 0;
  if((eChgStop_Shake_CcOpen == DisChargeStopType)
      || (eChgStop_Imd_CcOpen == DisChargeStopType)
      || (eChgStop_Reco_CcOpen == DisChargeStopType)
      || (eChgStop_Conf_CcOpen == DisChargeStopType)
      || (eChgStop_BmsReady_CcOpen == DisChargeStopType)
      || (eChgStop_PillarReady_CcOpen == DisChargeStopType)
      || (eChgStop_Start_CcOpen == DisChargeStopType)
      || (eChgStop_Run_CcOpen == DisChargeStopType)
      || (eChgStop_ComBreak_CcOpen == DisChargeStopType))
  {
    Dtc = 0x01380DC2;
  }

  if((eChgStop_Shake_EmerClose == DisChargeStopType)
      || (eChgStop_Imd_EmerClose == DisChargeStopType)
      || (eChgStop_Reco_EmerClose == DisChargeStopType)
      || (eChgStop_Conf_EmerClose == DisChargeStopType)
      || (eChgStop_BmsReady_EmerClose == DisChargeStopType)
      || (eChgStop_PillarReady_EmerClose == DisChargeStopType)
      || (eChgStop_Start_EmerClose == DisChargeStopType)
      || (eChgStop_Run_EmerClose == DisChargeStopType)
      || (eChgStop_ComBreak_EmerClose == DisChargeStopType))
  {
    Dtc = 0x01480DC2;//9
  }

  if((eChgStop_Shake_DoorOpen== DisChargeStopType)
      || (eChgStop_Imd_DoorOpen == DisChargeStopType)
      || (eChgStop_Reco_DoorOpen == DisChargeStopType)
      || (eChgStop_Conf_DoorOpen == DisChargeStopType)
      || (eChgStop_BmsReady_DoorOpen == DisChargeStopType)
      || (eChgStop_PillarReady_DoorOpen == DisChargeStopType)
      || (eChgStop_Start_DoorOpen == DisChargeStopType)
      || (eChgStop_Run_DoorOpen == DisChargeStopType)
      || (eChgStop_ComBreak_DoorOpen == DisChargeStopType))
  {
    Dtc = 0x01500DC2;//10
  }

  if((eChgStop_Shake_Contactor == DisChargeStopType)
      || (eChgStop_Imd_Contactor == DisChargeStopType)
      || (eChgStop_Reco_Contactor == DisChargeStopType)
      || (eChgStop_Conf_Contactor == DisChargeStopType)
      || (eChgStop_BmsReady_Contactor == DisChargeStopType)
      || (eChgStop_PillarReady_Contactor == DisChargeStopType)
      || (eChgStop_Start_Contactor == DisChargeStopType)
      || (eChgStop_Run_Contactor == DisChargeStopType)
      || (eChgStop_ComBreak_Contactor == DisChargeStopType))
  {
    Dtc = 0x01580DC2;//11
  }

  if((eChgStop_Shake_InVolt == DisChargeStopType)
      || (eChgStop_Imd_InVolt == DisChargeStopType)
      || (eChgStop_Reco_InVolt == DisChargeStopType)
      || (eChgStop_Conf_InVolt == DisChargeStopType)
      || (eChgStop_BmsReady_InVolt == DisChargeStopType)
      || (eChgStop_PillarReady_InVolt == DisChargeStopType)
      || (eChgStop_Start_InVolt == DisChargeStopType)
      || (eChgStop_Run_InVolt == DisChargeStopType)
      || (eChgStop_ComBreak_InVolt== DisChargeStopType))
  {
    Dtc = 0x01600DC2;//12
  }

  if((eChgStop_Shake_NoDcmod == DisChargeStopType)
      || (eChgStop_Imd_NoDcmod== DisChargeStopType)
      || (eChgStop_Reco_NoDcmod== DisChargeStopType)
      || (eChgStop_Conf_NoDcmod== DisChargeStopType)
      || (eChgStop_BmsReady_NoDcmod== DisChargeStopType)
      || (eChgStop_PillarReady_NoDcmod== DisChargeStopType)
      || (eChgStop_Start_NoDcmod== DisChargeStopType)
      || (eChgStop_Run_NoDcmod== DisChargeStopType)
      || (eChgStop_ComBreak_NoDcmod== DisChargeStopType))
  {
    Dtc = 0x01680DC2;//13
  }

  if((eChgStop_Shake_Meter== DisChargeStopType)
      || (eChgStop_Imd_Meter== DisChargeStopType)
      || (eChgStop_Reco_Meter == DisChargeStopType)
      || (eChgStop_Conf_Meter== DisChargeStopType)
      || (eChgStop_BmsReady_Meter == DisChargeStopType)
      || (eChgStop_PillarReady_Meter == DisChargeStopType)
      || (eChgStop_Start_Meter == DisChargeStopType)
      || (eChgStop_Run_Meter == DisChargeStopType)
      || (eChgStop_ComBreak_Meter == DisChargeStopType))
  {
    Dtc = 0x01700DC2;//14
  }

  while(1)
  {
    if((0 != CST_SendTick) && ((GetSystemTick() - CST_SendTick) > 10))
    {
      CST_Send(&CstMsg);
      CST_SendTick = GetSystemTick();
      CstSendCnt++;
      if(CstSendCnt > 50)
      {
        CST_SendTick = 0;
        V2L_SendTick = 0;
      }
    }

	if((0 != V2L_SendTick) && ((GetSystemTick() - V2L_SendTick) > 100)
		&& (CarTypeVar == NioCar_Type))
    {
    
      V2L_Send();
      V2L_SendTick = GetSystemTick();
    }

    if((0 != DtcSendTick) && ((GetSystemTick() - DtcSendTick) > 500))
    {
      DtcSendTick = GetSystemTick();
      DM1_Send(Dtc);
      DM3_Send(1);
      DtcSendCnt++;
      if(DtcSendCnt > 5)
      {
        BmsPowerSet(eSwSta_Off);
        DtcSendTick = 0;
      }
    }

    ChgPgnRecv(&PgnRecv);

    if(0 != DrainResistTick)
    {
      if((GetDcModuleVolt() < 600) || ((GetSystemTick() - DrainResistTick) > 4000))//小于60V
      {
       
        ChgEndFlag = 1;
        DrainResistTick = 0;
      }
    }

    if((0 != ChgEndFlag) && (DtcSendCnt > 5))
    {
      LockSet(eSwSta_Off);
      
    }
    
    if(eSwSta_On != CcStatusRead())
    {
      if(0 == CcStatusChkTick)
        CcStatusChkTick = GetSystemTick();
    }
    else
      CcStatusChkTick = 0;
    
    if((0 != CcStatusChkTick) && ((GetSystemTick() - CcStatusChkTick) > CC_CheckTick))
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      GunOutFlag = 1;
      CcStatusChkTick = 0;
    }

    if((0 != ChgEndFlag) && (DtcSendCnt > 5))
    {//外部控制结束充电

	  /*
      if (DisChargeEnable == 0 && DisChargeStopType == eChgStop_Stop_ByKeyTurnOff)
      {
    
        pChargeCtrl->CurProcess = eChgPro_TurnOff;
        pChargeCtrl->Param = 0;
      }
      else
      {
        if (GunOutFlag)
        {
          
          //pChargeCtrl->CurProcess = eChgPro_DisChgIdle;
          //pChargeCtrl->Param = 0;
        }
      }
      */
      if (GunOutFlag && AdcVoltBuf[EnumVin_VOL] < VinInVolt_Vaild )
      {
        
        pChargeCtrl->CurProcess = eChgPro_TurnOff;
        pChargeCtrl->Param = 0;
      }
      if (SleepTick == 0)
        SleepTick = GetSystemTick();
	  else if (GetSystemTick() - SleepTick > SleepTimeOut)
	  {
        //SleepTick = 0;
		pChargeCtrl->CurProcess = eChgPro_TurnOff;
        pChargeCtrl->Param = 0;
	  }
      
    }    


    if(eChgPro_DisChgBreak != pChargeCtrl->CurProcess)
      return;

    osDelay(5);
  }
}

