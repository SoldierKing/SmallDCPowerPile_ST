

#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"


void ChgProChgStart(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  u8  CroStarus = 0x00;
  u32 CcStatusChkTick = 0;
  u8  CroRecvFirstFlag = 0;

  BemMsg.Valid = 0;
  BclMsg.Valid = 0;
  BcsMsg.Valid = 0;
  BstMsg.Valid = 0;

  ChargeTimerSet(eSwSta_Off);
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);
  if((eChgVer_2011 != UserParam.ChgVer) && (eChgVer_2011 != ChgVer))
    DcOutSet(eSwSta_On);
  else
    DcOutSet(eSwSta_Off);

  CroStarus = 0xAA;
  BclMsg.RecvTick = GetSystemTick();
  BcsMsg.RecvTick = GetSystemTick();

  while(1)
  {
    if((GetSystemTick() - CRO_SendTick) > 250)
    {
      
      CRO_Send(CroStarus);
      CRO_SendTick = GetSystemTick();
      if (CroRecvFirstFlag ==0)
      {
      
        BclMsg.RecvTick = GetSystemTick();
        BcsMsg.RecvTick = GetSystemTick();
		CroRecvFirstFlag = 1;
	  }
	  
    }

    ChgPgnRecv(&PgnRecv);

    if((eChgVer_2011 != UserParam.ChgVer) && (eChgVer_2011 != ChgVer))
    {
      if((GetSystemTick() - BclMsg.RecvTick) > MsgTimeout_1S)
      {
        CemMsg.Cem.Value.Bcl = 1;
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        pChargeCtrl->CurProcess = eChgPro_CommBreak;
        pChargeCtrl->Param = 0;
      }

      if((GetSystemTick() - BcsMsg.RecvTick) > MsgTimeout_5S)
      {
        CemMsg.Cem.Value.Bcs = 1;
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        pChargeCtrl->CurProcess = eChgPro_CommBreak;
        pChargeCtrl->Param = 0;
      }
    }
    else
    {
      if((GetSystemTick() - BclMsg.RecvTick) > (MsgTimeout_5S*4))
      {
        CemMsg.Cem.Value.Bcl = 1;
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        pChargeCtrl->CurProcess = eChgPro_CommBreak;
        pChargeCtrl->Param = 0;
      }

      if((GetSystemTick() - BcsMsg.RecvTick) > (MsgTimeout_5S*4))
      {
        CemMsg.Cem.Value.Bcs = 1;
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        pChargeCtrl->CurProcess = eChgPro_CommBreak;
        pChargeCtrl->Param = 0;
      }
      else if ((GetSystemTick() -BcsMsg.RecvTick) > (1000 + 500))
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
        FrameSend.Len		= 8;

        BmsCommSendMsg(&FrameSend);
      }
    }

    if((0 != BclMsg.Valid) && (0 != BcsMsg.Valid))
    {//接收到Bcl Bcs
      pChargeCtrl->CurProcess = eChgPro_ChgRun;
      CRO_SendTick = 0;
      //pChargeCtrl->CurProcess = eChgPro_WaitVoltChgRun;
      pChargeCtrl->Param = 0;
    }

    if(0 != BemMsg.Valid)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      pChargeCtrl->CurProcess = eChgPro_CommBreak;
      pChargeCtrl->Param = 0;
    }

    if(0 != BstMsg.Valid)
    {
      CstMsg.StopReason.Value.BmsStop = 1;
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      ChargeStopType = eChgStop_Start_BmsStop;
      pChargeCtrl->CurProcess = eChgPro_ChgStop;
      pChargeCtrl->Param = 0;
    }

    if(0 == ChargeEnable)
    {//外部控制结束充电
      CstMsg.StopReason.Value.Manual = 1;
      //CST_Send(&CstMsg);
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      ChargeStopType = eChgStop_Start_UserCancel;
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
    if((0 != CcStatusChkTick) && ((GetSystemTick() - CcStatusChkTick) > CC_CheckTick))
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.ChgGun = 1;
     // CST_Send(&CstMsg);
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      CcStatusChkTick = 0;
      ChargeStopType = eChgStop_Start_CcOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Emergency)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Emergency = 1;
     // CST_Send(&CstMsg);
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Start_EmerClose;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Door)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
     // CST_Send(&CstMsg);
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Start_DoorOpen;
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
      ChargeStopType = eChgStop_Start_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.UniContactor)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
     // CST_Send(&CstMsg);
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Start_Contactor;
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
      ChargeStopType = eChgStop_Start_InVolt;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.NoDcmod)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
      //CST_Send(&CstMsg);
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Start_NoDcmod;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Meter)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
      //CST_Send(&CstMsg);
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Start_Meter;
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
      CstMsg.StopTruble.Value.Other = 1;
	  ChargeStopType = eChgStop_VEH_RES_WorkModleErr;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    
    if(eChgPro_ChgStart != pChargeCtrl->CurProcess)
      return;
    osDelay(5);
  }
}



