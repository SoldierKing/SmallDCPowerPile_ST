

#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"

void ChgProChgRecognize(StructChargeCtrl *pChargeCtrl)
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
    

    if((GetSystemTick() - CRM_SendTick) > 250)
    {
      CRM_Send(CrmRecog);
      CRM_SendTick = GetSystemTick();
    }
    
    ChgPgnRecv(&PgnRecv);

    if(0 != BrmMsg.Valid)
    {
      //接收到Brm
      pChargeCtrl->CurProcess = eChgPro_ChgConfigure;
      pChargeCtrl->Param = 0;
    }
    else if((GetSystemTick() - BrmMsg.RecvTick) > (MsgTimeout_5S))
    {
      CemMsg.Cem.Value.Brm = 1;
      pChargeCtrl->CurProcess = eChgPro_CommBreak;
      pChargeCtrl->Param = 0;
    }
    else if (((GetSystemTick() -BrmMsg.RecvTick) > (2000)&&(eChgVer_2011 == UserParam.ChgVer)))
    {
      StructChargeSend FrameSend;

      FrameSend.Id.Mult	= 0;
      FrameSend.Id.Disp.Prio = 7;
      FrameSend.Id.Disp.PF = TPCM >> 8;
      FrameSend.Id.Disp.DA_PS = Card1939Addr;
      FrameSend.Id.Disp.SA = Charger1939Addr;
      FrameSend.Data[0]	= 0x13;
      FrameSend.Data[1]	= 0x09;
      FrameSend.Data[2]	= 0x00;
      FrameSend.Data[3]	= 0x02;
      FrameSend.Data[4]	= 0xFe;
      FrameSend.Data[5]	= 0x00;
      FrameSend.Data[6]	= 0x11;
      FrameSend.Data[7]	= 0x00;
      FrameSend.Len	= 8;

      BmsCommSendMsg(&FrameSend);
    }

    if(0 == ChargeEnable)
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
    
    if(eChgPro_ChgRecognize != pChargeCtrl->CurProcess)
      return;

    osDelay(5);
  }
}

