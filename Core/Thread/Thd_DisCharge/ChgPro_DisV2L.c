
#include "Thd_Charge.h"
#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "ADrv_AdcVolt.h"


void DisChgProV2L(StructChargeCtrl *pChargeCtrl)
{
  
  StructPgnRecv PgnRecv;
  
  u32 CcStatusChkTick = 0;
  u32 V2l_FirstSend = 0;

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

  ResMsg.Valid = 0;
  ResMsg.RecvTick = GetSystemTick();

  ChargeTimerSet(eSwSta_Off);
  CleanChgTimeCnt();
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);
  DcOutSet(eSwSta_Off);
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  //BmsCanInit();
  while(pdTRUE == xQueueReceive(BmsMsg, &PgnRecv, 0));
  //osDelay(20);

  V2L_Send();
  
  V2l_FirstSend = GetSystemTick();
  V2L_SendTick = GetSystemTick();

  CarTypeVar = OtherCar_Type;

  while(1)
  {
    
    if((0 != V2L_SendTick) && ((GetSystemTick() - V2L_SendTick) > 100))
    {
    
      V2L_Send();
      V2L_SendTick = GetSystemTick();
    }
    
    ChgPgnRecv(&PgnRecv);
    
    if (ResMsg.Valid != 0)
    {
      //0表示pending，设备继续等待
      //1表示VCU允许放电
      //4表示VCU允许充电
      //其他值认为无效，设备继续等待
      //该信号一直周期性发送
      if (ResMsg.V2LDischargeReq.Value.V2lDisChargeSta == 1)
      {
        
        pChargeCtrl->CurProcess = eChgPro_DisShakeHand;
        pChargeCtrl->Param = 0;
      }
	  else if (ResMsg.V2LDischargeReq.Value.V2lDisChargeSta == 4)
	  {

		//充放电模式不匹配
		
	  }

	  //检测到为蔚来汽车类型；
	  CarTypeVar = NioCar_Type;

	  
    }

    //超时要怎么处理
    if ((GetSystemTick() - V2l_FirstSend) > 3000)
    {
       if (CarTypeVar == NioCar_Type)
       {

		 CstMsg.StopTruble.Value.Other = 1;
		 DisChargeStopType = eChgStop_V2L_DisCarTypeErr;
         pChargeCtrl->CurProcess = eChgPro_DisChgBreak;
         pChargeCtrl->Param = 0;
	   }
	   else
	   {
         pChargeCtrl->CurProcess = eChgPro_DisShakeHand;
         pChargeCtrl->Param = 0;
	   }
	   //检测到是其他车辆类型；
	   //CarTypeVar = OtherCar_Type;
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
    if(eChgPro_DisV2L != pChargeCtrl->CurProcess)
      return;

    osDelay(5);
	
  }
}

