

#include "Thd_Charge.h"
#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "ADrv_CompatibleInterface.h"
#include "ADrv_ImdCheck.h"


void DisChgProChgIdle(StructChargeCtrl *pChargeCtrl)
{

  //u32 CcStatusChkTick = 0;
  //u32 TimeOutTick = 0;
  u32 CcStatusChkTick = 0;
  u32 SleepTick = 0;
  
  //即插即放电模式
  DisChargeEnable = 1;  
  
  //关闭IMD绝缘检测及高压继电器辅源
  SwitcBoostENOnOff(eSwSta_On);
  //充电时间控制
  ChargeTimerSet(eSwSta_Off);
  //充电时间清除
  CleanChgTimeCnt();
  //BMS辅源使能 关闭
  BmsPowerSet(eSwSta_Off);
  //电子锁关闭
  LockSet(eSwSta_Off);
  //控制高压继电器
  DcOutSet(eSwSta_Off);
  //模块设置
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  CHM_SendTick = 0;       //CHM发送时间统计
  CRM_SendTick = 0;	      //crm 发送时间统计
  CRO_SendTick = 0;	      //a枪cro时间统计
  V2L_SendTick = 0;

  BmsCanInit();
  osDelay(20);
  
  while(1)
  {

    /*
    if(eSwSta_On == CcStatusRead())
    {
      if(0 == CcStatusChkTick)
        CcStatusChkTick = GetSystemTick();
    }
    else
      CcStatusChkTick = 0;
    */
	/*
    if (TimeOutTick == 0)
      TimeOutTick = GetSystemTick();
    else if (GetSystemTick() - TimeOutTick > TimeOut)
    {
      
       //超时，先停机再进入停机线程，最后进入TurnOFF
       pChargeCtrl->CurProcess = eChgPro_TurnOff;
       pChargeCtrl->Param = 0;
       DisChargeStopType = eChgStop_DisCharge_TimeOut;
    }
    */
    /*
    if((0 != CcStatusChkTick) && ((GetSystemTick() - CcStatusChkTick) > 1500))
    {
      CcStatusChkTick = 0;

      if(0 != DisChargeEnable)
      {
        pChargeCtrl->CurProcess = eChgPro_DisV2L;
        pChargeCtrl->Param = 0;
      }
    }    
    */
    
    //KeyPressCfg.IOSta = GetKeyBtnSta();  
    //GetKeyPressType(&KeyPressCfg);      
    if(eSwSta_On == CcStatusRead())
    {
      if(0 == CcStatusChkTick)
        CcStatusChkTick = GetSystemTick();
    }
    else
    {
      CcStatusChkTick = 0;
	   
    }
	  	
   if((0 != CcStatusChkTick) && ((GetSystemTick() - CcStatusChkTick) > 200))
    {
      CcStatusChkTick = 0;
	  if(0 != DisChargeEnable)
      {
        pChargeCtrl->CurProcess = eChgPro_DisV2L;
        pChargeCtrl->Param = 0;
      }
      
    }

    if (SleepTick == 0)
      SleepTick = GetSystemTick();
	
    else if (GetSystemTick() - SleepTick > SleepTimeOut)
    {
      //SleepTick = 0;
  	  pChargeCtrl->CurProcess = eChgPro_TurnOff;
      pChargeCtrl->Param = 0;
    }
	
    if(
        (0 != PillarError.Value.Emergency)
        || (0 != PillarError.Value.Meter)
        || (0 != PillarError.Value.Memory)
        || (0 != PillarError.Value.Lcd)
        || (0 != PillarError.Value.Door)
        || (0 != PillarError.Value.Contactor)
        || (0 != PillarError.Value.UniContactor)
        || (0 != PillarError.Value.InVolt)
      )
    {
      pChargeCtrl->CurProcess = eChgPro_DisChgIdle;
      pChargeCtrl->Param = 0;
    }
    

    if(eChgPro_DisChgIdle != pChargeCtrl->CurProcess)
      return;

    osDelay(100);
  }
}

