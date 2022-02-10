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
  
  //IO �ڳ�ʼ��
  GPIOInit();
  //ADC1 ��ʼ��
  ADCInit();
  //LED�Ƴ�ʼ��  
  WS2812B_Init();  
  //e2ROM��ʼ��
  //E2romInit();
     
  //E2ROM  ��д����
  //I2CTest();

  //�ⲿFLASH��ʼ��
  //sFLASH_Init();
  
  //sFLASHTest();
  //FlashTest();
  osDelay(20);
   
  while(1)
  {
    
    //����ָʾ��
    LedTwinkleAlways(SwitcRunLedOnOff,&TwinkQuick);    
    
    //һ��ͣ��������
    OneKeyStop();
	
    //�����������ػ�
    //StartKeyTurnOff();
    
    //��ȡADC��ѹֵ
    GetAdcVoltHandler();
    
    //LedEffectSwitch(Breathing,Red);
    
    //�����߳�
    Thd_LampSignals();
    
    //��Դ�������������
    PowerFailureCheck();
    
    //ADCֵ����
    GetAdcBuffValue(AdcBuffer);

	//��������AC�е������
	ChargeACOffFault();
             
    osDelay(5);
  }
}
