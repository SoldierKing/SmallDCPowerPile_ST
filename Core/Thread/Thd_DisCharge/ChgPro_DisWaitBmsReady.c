

#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"


void DisChgProWaitBmsReady(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  u32 CML_SendTick = 0;
  u32 CTS_SendTick = 0;
  u32 CcStatusChkTick = 0;
  u32 BRO_AA_RecvTick = 0;

  BemMsg.Valid = 0;
  BroMsg.Valid = 0;
  ChargeTimerSet(eSwSta_Off);
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);
  
  DcOutSet(eSwSta_Off);
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  CTS_Send();
  CTS_SendTick = GetSystemTick();

  //1.整车电池包总电压+10V；（随车充--车端）
  //2.最大允许放电电流 -400A；
  //CML_Send((BhmMsg.ImdMaxVolt + 100), DcModuleAbility.MinVolt, (4000 - 4000), 4000 - DcModuleAbility.MinCurrt);
  CML_Send((BcpMsg.StartVolt + 100), DcModuleAbility.MinVolt, (4000 - 4000), 4000 - DcModuleAbility.MinCurrt);

  CML_SendTick = GetSystemTick();

  BroMsg.RecvTick = GetSystemTick();
  BRO_AA_RecvTick = GetSystemTick();

  while(1)
  {
    
    if(((GetSystemTick() - CML_SendTick) > 250)&&((0 == BroMsg.Valid) && (0xAA != BroMsg.Status)))
    {
      //CML_Send((BhmMsg.ImdMaxVolt + 100), DcModuleAbility.MinVolt, (4000-4000), 4000 - DcModuleAbility.MinCurrt);
      CML_Send((BcpMsg.StartVolt + 100), DcModuleAbility.MinVolt, (4000-4000), 4000 - DcModuleAbility.MinCurrt);
      CML_SendTick = GetSystemTick();
    }
    if((GetSystemTick() - CTS_SendTick) > 500)
    {
      CTS_Send();
      CTS_SendTick = GetSystemTick();
    }

	if((0 != V2L_SendTick) && ((GetSystemTick() - V2L_SendTick) > 100)
		&& (CarTypeVar == NioCar_Type))
    {
    
      V2L_Send();
      V2L_SendTick = GetSystemTick();
    }

    ChgPgnRecv(&PgnRecv);

    if((GetSystemTick() - BroMsg.RecvTick) > MsgTimeout_5S)
    {
      CemMsg.Cem.Value.Bro = 1;//置位
      pChargeCtrl->CurProcess = eChgPro_DisCommBreak;
      pChargeCtrl->Param = 0;
    }
    else if (((GetSystemTick() -BroMsg.RecvTick) > MsgTimeout_5S / 2)&&(eChgVer_2011 == UserParam.ChgVer))
    {
      
        StructChargeSend FrameSend;
        FrameSend.Id.Mult	= 0;
        FrameSend.Id.Disp.Prio  = 7;
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
    else
      CemMsg.Cem.Value.Bro = 0;

    if((0 != BRO_AA_RecvTick) && ((GetSystemTick() - BRO_AA_RecvTick) > MsgTimeout_60S-300))
    {
      CemMsg.Cem.Value.Bro = 1;
      pChargeCtrl->CurProcess = eChgPro_DisCommBreak;
      pChargeCtrl->Param = 0;
    }

    if((0 != BroMsg.Valid) && (0xAA == BroMsg.Status))
    {
      //接收到Brm
      BRO_AA_RecvTick = 0;
      pChargeCtrl->CurProcess = eChgPro_DisWaitPillarReady;
      pChargeCtrl->Param = 0;
    }

    if(0 != BemMsg.Valid)
    {
      pChargeCtrl->CurProcess = eChgPro_DisCommBreak;
      pChargeCtrl->Param = 0;
    }

    if(0 == DisChargeEnable)
    {//外部控制结束充电
      if (DisChargeStopType != eChgStop_Stop_ByKeyTurnOff)
        DisChargeStopType = eChgStop_BmsReady_UserCancel;
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
      CcStatusChkTick = 0;
      DisChargeStopType = eChgStop_BmsReady_CcOpen;
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
    
    if(eChgPro_DisWaitBmsReady != pChargeCtrl->CurProcess)
      return;
    osDelay(5);
  }
}


