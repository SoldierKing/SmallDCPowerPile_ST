

#include "Thd_Charge.h"
#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"


void ChgProTurnOn(StructChargeCtrl *pChargeCtrl)
{
  

  //关闭IMD绝缘检测及高压继电器辅源
  SwitcBoostENOnOff(eSwSta_Off);
  //充电时间控制
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
  
  //唤醒被挂起的线程
  
  while(1)
  {
    
    if (GetWorkMode() == Enum_ChargeWork)
    {
    
      //电池输出使能关闭
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

