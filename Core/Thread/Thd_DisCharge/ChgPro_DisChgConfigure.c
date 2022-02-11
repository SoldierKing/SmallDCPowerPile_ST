

#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"

void DisChgProChgConfigure(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  //CRM_SendTick = 0;
  u8  CrmRecog;
  u32 CcStatusChkTick = 0;
  u8  CrmFirstSendFlag = 0;

  BemMsg.Valid = 0;
  BcpMsg.Valid = 0;
  ChargeTimerSet(eSwSta_Off);
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);
  DcOutSet(eSwSta_Off);
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  CrmRecog = 0xAA;
  BcpMsg.RecvTick = GetSystemTick();

  while(1)
  {
    
    if((GetSystemTick() - CRM_SendTick) > 250)
    {
      CRM_Send(CrmRecog);
      CRM_SendTick = GetSystemTick();
      if(CrmFirstSendFlag == 0)
      {
         BcpMsg.RecvTick= GetSystemTick();
         CrmFirstSendFlag = 1;
      }
    }

	if((0 != V2L_SendTick) && ((GetSystemTick() - V2L_SendTick) > 100)
		&& (CarTypeVar == NioCar_Type))
    {
    
      V2L_Send();
      V2L_SendTick = GetSystemTick();
    }

    ChgPgnRecv(&PgnRecv);

    if((GetSystemTick() - BcpMsg.RecvTick) > MsgTimeout_5S)
    {
      CemMsg.Cem.Value.Bcp = 1;
      pChargeCtrl->CurProcess = eChgPro_DisCommBreak;
      pChargeCtrl->Param = 0;
    }
    else if (((GetSystemTick() -BcpMsg.RecvTick) > (2000)&&(eChgVer_2011 == UserParam.ChgVer)))
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
    else
      CemMsg.Cem.Value.Bcp = 0;

    if(0 != BcpMsg.Valid)
    {//接收到Brm

      //允许 放电输入（车端电压)最小电压大于整车电池包总电压
      if(0 != BhmMsg.Valid && UserParam.DcDisMinVolt > BhmMsg.ImdMaxVolt)
      {
        CstMsg.StopReason.Value.Truble = 1;
        CstMsg.StopTruble.Value.Other = 1;
        CstMsg.StopFault.Value.Volt = 1;
        DisChargeStopType = eChgStop_Conf_ChgMaxVoltLessPillarMinVolt;
        pChargeCtrl->CurProcess = eChgPro_DisChgStop;
        pChargeCtrl->Param = 0;
      }
        
	  /*
      else if((BcpMsg.ChgMaxCurrt - 4000) < 0)
      {
        
        CstMsg.StopReason.Value.Truble = 1;
        CstMsg.StopTruble.Value.Other = 1;
        CstMsg.StopFault.Value.Volt = 1;
        DcOutSet(eSwSta_Off);
        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
        DisChargeStopType = eChgStop_PillarReady_BatCurrentErr;
        pChargeCtrl->CurProcess = eChgPro_DisChgStop;
        pChargeCtrl->Param = 0;
      }
      */
      else 
      {
        pChargeCtrl->CurProcess = eChgPro_DisWaitBmsReady;
        pChargeCtrl->Param = 0;
        CRM_SendTick= 0;
      }
    }

    if(0 != BemMsg.Valid)
    {
      pChargeCtrl->CurProcess = eChgPro_DisCommBreak;
      pChargeCtrl->Param = 0;
    }

    if(0 == DisChargeEnable)
    {
      //外部控制结束充电
      if (DisChargeStopType != eChgStop_Stop_ByKeyTurnOff)
        DisChargeStopType = eChgStop_Conf_UserCancel;
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
      DisChargeStopType = eChgStop_Conf_CcOpen;
      pChargeCtrl->CurProcess = eChgPro_DisChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    if(eChgPro_DisChgConfigure != pChargeCtrl->CurProcess)
      return;
    osDelay(5);
  }
}

