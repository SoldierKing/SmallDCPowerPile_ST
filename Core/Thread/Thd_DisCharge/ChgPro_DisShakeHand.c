
#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"
#include "ADrv_AdcVolt.h"


s32 GetDisDCVoltBeforInsulation(void)
{
  
  //if(GetBoostENSwitchSta() != GPIO_PIN_SET)
  //{
    //SwitcBoostENOnOff(eSwSta_On);
    //osDelay(15);
  //}
  
  return (s32)AdcVoltBuf[EnumVddc_AD3];
    
}


void DisChgProShakeHand(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  CHM_SendTick = 0;
  u32 CcStatusChkTick = 0;
  u32 BHM_FirstRecv = 0;


  memset(&BemMsg, 0, sizeof(BemMsg));
  memset(&CemMsg, 0, sizeof(CemMsg));
  memset(&BstMsg, 0, sizeof(BstMsg));
  memset(&CstMsg, 0, sizeof(CstMsg));
  memset(&BsdMsg, 0, sizeof(BsdMsg));
  memset(&CsdMsg, 0, sizeof(CsdMsg));

  CommErrorCount = 0;
  CommErrFrame[0] = 0;
  CommErrFrame[1] = 0;
  CommErrFrame[2] = 0;
  CommErrFrame[3] = 0;
  

  DisChargeStopType = eChgStop_None;

  BhmMsg.Valid = 0;
  BhmMsg.RecvTick = GetSystemTick();
  BHM_FirstRecv = 0;

  ChargeTimerSet(eSwSta_Off);
  CleanChgTimeCnt();
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);
  DcOutSet(eSwSta_Off);
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  //BmsCanInit();
  while(pdTRUE == xQueueReceive(BmsMsg, &PgnRecv, 0));
  //osDelay(20);

  CHM_Send();
  CHM_SendTick = GetSystemTick();

  while(1)
  {
    
    if((0 != CHM_SendTick) && ((GetSystemTick() - CHM_SendTick) > 250))
    {
    
      CHM_Send();
      CHM_SendTick = GetSystemTick();
    }

	if((0 != V2L_SendTick) && ((GetSystemTick() - V2L_SendTick) > 100)
		&& (CarTypeVar == NioCar_Type))
    {
    
      V2L_Send();
      V2L_SendTick = GetSystemTick();
    }

    ChgPgnRecv(&PgnRecv);


    if(eChgVer_2011 != UserParam.ChgVer)
    {
      if(0 != BhmMsg.Valid)//新国标
      {//已经收到BHM报文，
        ChgVer = eChgVer_2015;
        if(0 == BHM_FirstRecv)
          BHM_FirstRecv = GetSystemTick();
        
        if( GetDisDCVoltBeforInsulation() >1000)
        {
          ChargeStopType = eChgStop_Imd_IsolateError;
          pChargeCtrl->CurProcess = eChgPro_DisChgStop;
          pChargeCtrl->Param = 0;
        }
        else
        {
          pChargeCtrl->CurProcess = eChgPro_DisChgRecognize;
          pChargeCtrl->Param = 0;
        }
        
        if((GetSystemTick() - BHM_FirstRecv) > MsgTimeout_5S)
        {
          
          pChargeCtrl->CurProcess = eChgPro_DisChgRecognize;
          pChargeCtrl->Param = 0;
        }
      }
      else if((GetSystemTick() - BhmMsg.RecvTick) > MsgTimeout_5S*3/2)
      {
      
        ChgVer = eChgVer_2011;
        //BmsPowerSet(eSwSta_Off);
        CHM_SendTick = 0;
        pChargeCtrl->CurProcess = eChgPro_DisChgRecognize;
        pChargeCtrl->Param = 0;
       }
    }
    else
    {
      if(0 != BhmMsg.Valid)//新国标
      {
        ChgVer = eChgVer_2015;
        if(0 == BHM_FirstRecv)
          BHM_FirstRecv = GetSystemTick();
        if((GetSystemTick() - BHM_FirstRecv) > MsgTimeout_5S)
        {
          pChargeCtrl->CurProcess = eChgPro_DisChgRecognize;
          pChargeCtrl->Param = 0;
        }
      }
      else if((GetSystemTick() - BhmMsg.RecvTick) > (MsgTimeout_5S/2))
      {
        //旧国标
        ChgVer = eChgVer_2011;
        pChargeCtrl->CurProcess = eChgPro_DisChgRecognizeEx;
        pChargeCtrl->Param = 0;
      }
    }
    
    
    if(0 == DisChargeEnable)
    {
      //外部控制结束充电
      if( DisChargeStopType != eChgStop_Stop_ByKeyTurnOff)
        DisChargeStopType = eChgStop_Shake_UserCancel;
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
      DcOutSet(eSwSta_Off);
      CcStatusChkTick = 0;
      DisChargeStopType = eChgStop_Shake_CcOpen;
      pChargeCtrl->CurProcess = eChgPro_DisChgBreak;
      pChargeCtrl->Param = 0;
    }

	/*
    if(0 != PillarError.Value.NoDcmod)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      DisChargeStopType = eChgStop_Shake_NoDcmod;
      pChargeCtrl->CurProcess = eChgPro_DisChgBreak;
      pChargeCtrl->Param = 0;
    }
    */
    if(eChgPro_DisShakeHand != pChargeCtrl->CurProcess)
      return;

    osDelay(5);
	
  }
}

