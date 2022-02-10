

#include "Thd_Charge.h"
#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"


void ChgProTurnOn(StructChargeCtrl *pChargeCtrl)
{
  

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
  
  //���ѱ�������߳�
  
  while(1)
  {
    
    if (GetWorkMode() == Enum_ChargeWork)
    {
    
      //������ʹ�ܹر�
      //SwitcStartOnOnOff(eSwSta_Off);
      pChargeCtrl->CurProcess = eChgPro_ChgIdle;
      pChargeCtrl->Param = 0;
    }
    else if (GetWorkMode() == Enum_DischargeWork)
    {
    
      SwitcStartOnOnOff(eSwSta_On);
      pChargeCtrl->CurProcess = eChgPro_DisChgIdle;
      pChargeCtrl->Param = 0;
    }
    
    if(eChgPro_TurnOn != pChargeCtrl->CurProcess)
      return;

    osDelay(100);
	
  }
}

