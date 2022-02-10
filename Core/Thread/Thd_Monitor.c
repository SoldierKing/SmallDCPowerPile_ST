#include "Thd_Monitor.h"
#include "Param_UserConfig.h"
#include "ADrv_Comm.h"
#include "Drv_ADC.h"
#include "ADrv_BmsFrame.h"
#include "Drv_CAN.h"
#include "ADrv_DcModel.h"
#include "Drv_WS2812B.h"

//#include "Drv_IWDG.h"

//#include "ADrv_CompatibleInterface.h"
//#include "ADrv_ImdCheck.h"
#include "ADrv_AdcVolt.h"
#include "ADrv_LampSignals.h"

//#include "ADrv_E2rom.h"
//#include "ADrv_SpiFlash.h"
#include "Param_Memory.h"
#include "ADrv_PowerFailure.h"




/* the system main thread */
void Thd_Monitor(void const *parameter)
{
  
  //IO 口初始化
  GPIOInit();
  //ADC1 初始化
  ADCInit();
  //LED灯初始化  
  WS2812B_Init();  
  //e2ROM初始化
  //E2romInit();
     
  //E2ROM  读写测试
  //I2CTest();

  //外部FLASH初始化
  //sFLASH_Init();
  
  //sFLASHTest();
  //FlashTest();
  osDelay(20);
   
  while(1)
  {
    
    //运行指示灯
    LedTwinkleAlways(SwitcRunLedOnOff,&TwinkQuick);    
    
    //一建停机、解锁
    OneKeyStop();
	
    //长按启动键关机
    //StartKeyTurnOff();
    
    //获取ADC电压值
    GetAdcVoltHandler();
    
    //LedEffectSwitch(Breathing,Red);
    
    //灯语线程
    Thd_LampSignals();
    
    //电源掉电电子锁解锁
    PowerFailureCheck();
    
    //ADC值更新
    GetAdcBuffValue(AdcBuffer);

	//充电过程中AC市电掉电检测
	ChargeACOffFault();
             
    osDelay(5);
  }
}
