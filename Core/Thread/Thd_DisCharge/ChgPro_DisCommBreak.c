
#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"


void DisChgProCommBeak(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  u32 CEM_SendTick = 0;
  u32 CurrtCheckTick = 0;
  u32 ChgReStartTick = 0;
  u32 CcStatusChkTick = 0;

  if(0 == CommErrorCount)
  {
    CommErrFrame[0] = BemMsg.Bem.Mult;
    CommErrFrame[1] = CemMsg.Cem.Mult;
  }
  else if(1 == CommErrorCount)
  {
    CommErrFrame[2] = BemMsg.Bem.Mult;
    CommErrFrame[3] = CemMsg.Cem.Mult;
  }

  CommErrorCount++;
  if(CommErrorCount > 2)
  {
    CstMsg.StopReason.Value.Truble = 1;//故障停止
    CstMsg.StopTruble.Value.Other = 1;//其他故障  通信超时超过三次
    DisChargeStopType = eChgStop_ComBreak_CommError;
    pChargeCtrl->CurProcess = eChgPro_DisCommError;
    pChargeCtrl->Param = 0;
    return;
  }

  BstMsg.Valid = 0;

  ChargeTimerSet(eSwSta_Off);
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);

  //DcOutSet(eSwSta_On);
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  if(0 != CemMsg.Cem.Mult)
    CEM_Send(&CemMsg);
  CEM_SendTick = GetSystemTick();

  CurrtCheckTick = GetSystemTick();
  ChgReStartTick = 0;

  while(1)
  {
    if((0 != CEM_SendTick) && ((GetSystemTick() - CEM_SendTick) > 250))
    {
      if(0 != CemMsg.Cem.Mult)//有通信故障
        CEM_Send(&CemMsg);
      CEM_SendTick = GetSystemTick();
    }

	if((0 != V2L_SendTick) && ((GetSystemTick() - V2L_SendTick) > 100)
		&& (CarTypeVar == NioCar_Type))
    {
    
      V2L_Send();
      V2L_SendTick = GetSystemTick();
    }

    ChgPgnRecv(&PgnRecv);

    if(GetDcModuleCurrt() < 50)//小于5A
    {
      CurrtCheckTick = 0;
      if(0 == ChgReStartTick)
      {
        DcOutSet(eSwSta_Off);//k1 k2
        ChgReStartTick = GetSystemTick();
      }
    }
    else
    {
      if((0 != CurrtCheckTick) && ((GetSystemTick() - CurrtCheckTick) > 5000))
      {
        DcOutSet(eSwSta_Off);//k1 k2
        CurrtCheckTick = 0;
        CstMsg.StopReason.Value.Truble = 1;//故障停止
        CstMsg.StopTruble.Value.Other = 1;//其他故障  电流故障
        CstMsg.StopFault.Value.Currt = 1;//电流不匹配
        DisChargeStopType = eChgStop_ComBreak_CurtError;
        pChargeCtrl->CurProcess = eChgPro_DisChgBreak;
        pChargeCtrl->Param = 0;
      }
    }

    if((0 != ChgReStartTick) && ((GetSystemTick() - ChgReStartTick) > 3500))
    {
      pChargeCtrl->CurProcess = eChgPro_DisChgRecognize;
      pChargeCtrl->Param = 0;
    }

    if(0 != BstMsg.Valid)
    {
      CstMsg.StopReason.Value.BmsStop = 1;
      DisChargeStopType = eChgStop_ComBreak_BmsStop;
      pChargeCtrl->CurProcess = eChgPro_DisChgStop;
      pChargeCtrl->Param = 0;
    }
    if(0 == DisChargeEnable)
    {//外部控制结束充电
      CstMsg.StopReason.Value.Manual = 1;
      if ( DisChargeStopType != eChgStop_Stop_ByKeyTurnOff)
        DisChargeStopType = eChgStop_ComBreak_UserCancel;
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
    if((0 != CcStatusChkTick) && ((GetSystemTick() - CcStatusChkTick) > CC_CheckTick))
    {
      DcOutSet(eSwSta_Off);
      CcStatusChkTick = 0;
      DisChargeStopType = eChgStop_ComBreak_CcOpen;
      pChargeCtrl->CurProcess = eChgPro_DisChgBreak;
      pChargeCtrl->Param = 0;
    }

	/*
    if(0 != PillarError.Value.NoDcmod)
    {
      DcOutSet(eSwSta_Off);
      DisChargeStopType = eChgStop_ComBreak_NoDcmod;
      pChargeCtrl->CurProcess = eChgPro_DisChgBreak;
      pChargeCtrl->Param = 0;
    }
    */
    
    if(eChgPro_DisCommBreak != pChargeCtrl->CurProcess)
      return;
    osDelay(5);
  }
}


