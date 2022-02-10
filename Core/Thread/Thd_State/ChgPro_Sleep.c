

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
  
  //������ʹ�ܹر�
  SwitcStartOnOnOff(eSwSta_Off);
  //�ر�IMD��Ե��⼰��ѹ�̵�����Դ
  SwitcBoostENOnOff(eSwSta_Off);
  //��ŵ��ʱ�����
  ChargeTimerSet(eSwSta_Off);
  //���ʱ�����
  CleanChgTimeCnt();
  //BMS��Դʹ�� �ر�
  BmsPowerSet(eSwSta_Off);
  //�������ر�
  LockSet(eSwSta_Off);
  //���Ƹ�ѹ�̵��� �ر�
  DcOutSet(eSwSta_Off);
  //ģ������
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  
  DisChargeEnable = 0;
  ChargeEnable = 0;
  ChargeStopType = eChgStop_None;
  DisChargeStopType = eChgStop_None;
  ChargeError.Mult = 0;
  
  
  //�ȴ�ADC��ʼ����ɣ�
  osDelay(100);
    
  while(1)
  {
    
     
      //������ͷ״̬�ж�;�ȴ�Ӳ������˹��ܣ�
      //������ģʽ������:
      //1.�����е磻
      //2.����ͷʶ��
      if (AdcVoltBuf[EnumVin_VOL] >= VinInVolt_Vaild && GetDcModuleVoltA() > 1800)
      {
  
        SwitchWorkMode(Enum_ChargeWork);   
  	    pChargeCtrl->CurProcess = eChgPro_TurnOn;
        pChargeCtrl->Param = 0;
  	    WS2812B_Black();
      }
  	
  	  //����ŵ�ģʽ������
  	  //1.���ǲ�ͷ����״̬��
  	  //2.ģ����޸�Դ(û�е�)��
  	  //3.�Ѳ�ǹ��
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
    	  //��ص�����ʾ       
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

