

#include "Thd_Charge.h"
#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "ADrv_AdcVolt.h"
#include "Drv_WS2812B.h"





void ChgProSleep(StructChargeCtrl *pChargeCtrl)
{
  
  
  u32 CcStatusChkTick = 0;
  
  //电池输出使能关闭
  SwitcStartOnOnOff(eSwSta_Off);
  //关闭IMD绝缘检测及高压继电器辅源
  SwitcBoostENOnOff(eSwSta_Off);
  //充放电电时间控制
  ChargeTimerSet(eSwSta_Off);
  //充电时间清除
  CleanChgTimeCnt();
  //BMS辅源使能 关闭
  BmsPowerSet(eSwSta_Off);
  //电子锁关闭
  LockSet(eSwSta_Off);
  //控制高压继电器 关闭
  DcOutSet(eSwSta_Off);
  //模块设置
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  
  DisChargeEnable = 0;
  ChargeEnable = 0;
  ChargeStopType = eChgStop_None;
  DisChargeStopType = eChgStop_None;
  ChargeError.Mult = 0;
  
  
  //等待ADC初始化完成；
  osDelay(100);
    
  while(1)
  {
    
     
      //需加入插头状态判断;等待硬件加入此功能；
      //进入充电模式需满足:
      //1.接入市电；
      //2.充电接头识别；
      if (AdcVoltBuf[EnumVin_VOL] >= VinInVolt_Vaild && GetDcModuleVoltA() > 1800)
      {
  
        SwitchWorkMode(Enum_ChargeWork);   
  	    pChargeCtrl->CurProcess = eChgPro_TurnOn;
        pChargeCtrl->Param = 0;
  	    WS2812B_Black();
      }
  	
  	  //进入放电模式需满足
  	  //1.三角插头缩回状态；
  	  //2.模块端无辅源(没市电)；
  	  //3.已插枪；
      else
      {
        KeyPressCfg.IOSta = GetKeyBtnSta();  
        GetKeyPressType(&KeyPressCfg); 

		if(eSwSta_On == CcStatusRead())
        {
          if(0 == CcStatusChkTick)
            CcStatusChkTick = GetSystemTick();
        }
        else
        {
          CcStatusChkTick = 0;
    	  //电池电量显示       
    	  BatPowerShowByLed();
    	   
        }
	  	
       if((0 != CcStatusChkTick) && ((GetSystemTick() - CcStatusChkTick) > 800)
       	&& (KeyPressCfg.KeyDownTime != 0))
        {
          CcStatusChkTick = 0;
    
          SwitchWorkMode(Enum_DischargeWork);
          pChargeCtrl->CurProcess = eChgPro_TurnOn;
          pChargeCtrl->Param = 0;
          
        }       
       
      }

	  if (KeyPressCfg.KeyDownTime == 0) WS2812B_Black();
        
      
    if(eChgPro_Sleep != pChargeCtrl->CurProcess)
      return;

    osDelay(100);
  }
}

