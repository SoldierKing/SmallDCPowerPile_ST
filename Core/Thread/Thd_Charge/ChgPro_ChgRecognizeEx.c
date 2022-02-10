

#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"


void ChgProChgRecognizeEx(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
   CRM_SendTick = 0;
  u8  CrmRecog;
  u32 CcStatusChkTick = 0;

  memset(&BemMsg, 0, sizeof(BemMsg));
  memset(&CemMsg, 0, sizeof(CemMsg));
  memset(&BstMsg, 0, sizeof(BstMsg));
  memset(&CstMsg, 0, sizeof(CstMsg));

  BrmMsg.Valid = 0;
  BemMsg.Valid = 0;

  ChargeTimerSet(eSwSta_Off);
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);
  DcOutSet(eSwSta_Off);
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  while(pdTRUE == xQueueReceive(BmsMsg, &PgnRecv, 0));
  CrmRecog = 0x00;
  CRM_Send(CrmRecog);
  CRM_SendTick = GetSystemTick();
  BrmMsg.RecvTick = GetSystemTick();
  CemMsg.Cem.Value.Brm = 0;

  while(1)
  {
    
    ChgPgnRecv(&PgnRecv);
    if((0 != CRM_SendTick) && ((GetSystemTick() - CRM_SendTick) > 250))
    {
      CRM_Send(CrmRecog);
      CRM_SendTick = GetSystemTick();
    }

    if(0 != BrmMsg.Valid)
    {//接收到Brm
      pChargeCtrl->CurProcess = eChgPro_ChgConfigure;
      pChargeCtrl->Param = 0;
    }
    else if((GetSystemTick() - BrmMsg.RecvTick) < (MsgTimeout_5S/2))
    {

    }
    else if((GetSystemTick() - BrmMsg.RecvTick) < (MsgTimeout_10S))
    {
     // BmsPowerSet(eSwSta_Off);
      CRM_SendTick = 0;
    }
    else
    {
      pChargeCtrl->CurProcess = eChgPro_ChgRecognize;
      pChargeCtrl->Param = 0;
    }

    if(0 == ChargeEnable )
    {//外部控制结束充电
      if (ChargeStopType != eChgStop_Stop_ByKeyTurnOff)
        ChargeStopType = eChgStop_Reco_UserCancle;
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
      CcStatusChkTick = 0;
      ChargeStopType = eChgStop_Reco_CcOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Emergency)
    {
      ChargeStopType = eChgStop_Reco_EmerClose;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Door)
    {
      ChargeStopType = eChgStop_Reco_DoorOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Contactor)
    {
      ChargeStopType = eChgStop_Reco_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.UniContactor)
    {
      ChargeStopType = eChgStop_Reco_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.InVolt)
    {
      ChargeStopType = eChgStop_Reco_InVolt;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.NoDcmod)
    {
      ChargeStopType = eChgStop_Reco_NoDcmod;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Meter)
    {
      ChargeStopType = eChgStop_Reco_Meter;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    
    if(eChgPro_ChgRecognizeEx != pChargeCtrl->CurProcess)
      return;

    osDelay(5);
  }
}


