
#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"

void ChgProChgBreak(StructChargeCtrl *pChargeCtrl)
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
  if((eChgStop_Shake_CcOpen == ChargeStopType)
      || (eChgStop_Imd_CcOpen == ChargeStopType)
      || (eChgStop_Reco_CcOpen == ChargeStopType)
      || (eChgStop_Conf_CcOpen == ChargeStopType)
      || (eChgStop_BmsReady_CcOpen == ChargeStopType)
      || (eChgStop_PillarReady_CcOpen == ChargeStopType)
      || (eChgStop_Start_CcOpen == ChargeStopType)
      || (eChgStop_Run_CcOpen == ChargeStopType)
      || (eChgStop_ComBreak_CcOpen == ChargeStopType))
  {
    Dtc = 0x01380DC2;
  }

  if((eChgStop_Shake_EmerClose == ChargeStopType)
      || (eChgStop_Imd_EmerClose == ChargeStopType)
      || (eChgStop_Reco_EmerClose == ChargeStopType)
      || (eChgStop_Conf_EmerClose == ChargeStopType)
      || (eChgStop_BmsReady_EmerClose == ChargeStopType)
      || (eChgStop_PillarReady_EmerClose == ChargeStopType)
      || (eChgStop_Start_EmerClose == ChargeStopType)
      || (eChgStop_Run_EmerClose == ChargeStopType)
      || (eChgStop_ComBreak_EmerClose == ChargeStopType))
  {
    Dtc = 0x01480DC2;//9
  }

  if((eChgStop_Shake_DoorOpen== ChargeStopType)
      || (eChgStop_Imd_DoorOpen == ChargeStopType)
      || (eChgStop_Reco_DoorOpen == ChargeStopType)
      || (eChgStop_Conf_DoorOpen == ChargeStopType)
      || (eChgStop_BmsReady_DoorOpen == ChargeStopType)
      || (eChgStop_PillarReady_DoorOpen == ChargeStopType)
      || (eChgStop_Start_DoorOpen == ChargeStopType)
      || (eChgStop_Run_DoorOpen == ChargeStopType)
      || (eChgStop_ComBreak_DoorOpen == ChargeStopType))
  {
    Dtc = 0x01500DC2;//10
  }

  if((eChgStop_Shake_Contactor == ChargeStopType)
      || (eChgStop_Imd_Contactor == ChargeStopType)
      || (eChgStop_Reco_Contactor == ChargeStopType)
      || (eChgStop_Conf_Contactor == ChargeStopType)
      || (eChgStop_BmsReady_Contactor == ChargeStopType)
      || (eChgStop_PillarReady_Contactor == ChargeStopType)
      || (eChgStop_Start_Contactor == ChargeStopType)
      || (eChgStop_Run_Contactor == ChargeStopType)
      || (eChgStop_ComBreak_Contactor == ChargeStopType))
  {
    Dtc = 0x01580DC2;//11
  }

  if((eChgStop_Shake_InVolt == ChargeStopType)
      || (eChgStop_Imd_InVolt == ChargeStopType)
      || (eChgStop_Reco_InVolt == ChargeStopType)
      || (eChgStop_Conf_InVolt == ChargeStopType)
      || (eChgStop_BmsReady_InVolt == ChargeStopType)
      || (eChgStop_PillarReady_InVolt == ChargeStopType)
      || (eChgStop_Start_InVolt == ChargeStopType)
      || (eChgStop_Run_InVolt == ChargeStopType)
      || (eChgStop_ComBreak_InVolt== ChargeStopType))
  {
    Dtc = 0x01600DC2;//12
  }

  if((eChgStop_Shake_NoDcmod == ChargeStopType)
      || (eChgStop_Imd_NoDcmod== ChargeStopType)
      || (eChgStop_Reco_NoDcmod== ChargeStopType)
      || (eChgStop_Conf_NoDcmod== ChargeStopType)
      || (eChgStop_BmsReady_NoDcmod== ChargeStopType)
      || (eChgStop_PillarReady_NoDcmod== ChargeStopType)
      || (eChgStop_Start_NoDcmod== ChargeStopType)
      || (eChgStop_Run_NoDcmod== ChargeStopType)
      || (eChgStop_ComBreak_NoDcmod== ChargeStopType))
  {
    Dtc = 0x01680DC2;//13
  }

  if((eChgStop_Shake_Meter== ChargeStopType)
      || (eChgStop_Imd_Meter== ChargeStopType)
      || (eChgStop_Reco_Meter == ChargeStopType)
      || (eChgStop_Conf_Meter== ChargeStopType)
      || (eChgStop_BmsReady_Meter == ChargeStopType)
      || (eChgStop_PillarReady_Meter == ChargeStopType)
      || (eChgStop_Start_Meter == ChargeStopType)
      || (eChgStop_Run_Meter == ChargeStopType)
      || (eChgStop_ComBreak_Meter == ChargeStopType))
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
        CST_SendTick = 0;
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

    if((GetDcModuleVolt() < 600) || ((GetSystemTick() - DrainResistTick) > 4000))//小于60V
    {
     
      ChgEndFlag = 1;
      DrainResistTick = 0;
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
      LockSet(eSwSta_Off);

      if (GunOutFlag)
      {
        
        pChargeCtrl->CurProcess = eChgPro_ChgIdle;
        pChargeCtrl->Param = 0;
      }
    }    


    if(eChgPro_ChgBreak != pChargeCtrl->CurProcess)
      return;

    osDelay(5);
  }
}

