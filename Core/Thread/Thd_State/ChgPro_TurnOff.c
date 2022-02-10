

#include "Thd_Charge.h"
#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"
#include "ADrv_AdcVolt.h"


void ChgProTurnOff(StructChargeCtrl *pChargeCtrl)
{
  
  //������ʹ�ܹر�
  //SwitcStartOnOnOff(eSwSta_Off);
  //�ر�IMD��Ե��⼰��ѹ�̵�����Դ
  SwitcBoostENOnOff(eSwSta_Off);
  //���ʱ�����
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
  
  KeyPressCfg.OverLongPressTime = 0;
  
  while(1)
  {
	/*
	if (AdcVoltBuf[EnumVin_VOL] < VinInVolt_Vaild && GetWorkMode() == Enum_DischargeWork)
	{
      pChargeCtrl->CurProcess = eChgPro_Sleep;
      pChargeCtrl->Param = 0;
      
	}
	else if (GetWorkMode() == Enum_ChargeWork)
	{

	  pChargeCtrl->CurProcess = eChgPro_Sleep;
      pChargeCtrl->Param = 0;
	}
       */
	pChargeCtrl->CurProcess = eChgPro_Sleep;
    pChargeCtrl->Param = 0;
    SwitcStartOnOnOff(eSwSta_Off);
	
    if(eChgPro_TurnOff != pChargeCtrl->CurProcess)
      return;

    osDelay(100);
  }
}

