
#include "Thd_Charge.h"
#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "ADrv_ImdCheck.h"


void ChgProChgIdle(StructChargeCtrl *pChargeCtrl)
{
  
  StructPgnRecv PgnRecv;
  u32 CcStatusChkTick = 0;
  
  //���弴��ģʽ
  ChargeEnable = 1;

  ResMsg.Valid = 0;
 
  //���ʱ�����
  ChargeTimerSet(eSwSta_Off);
  //���ʱ�����
  CleanChgTimeCnt();
  //BMS��Դʹ�� �ر�
  BmsPowerSet(eSwSta_Off);
  //�ر�BOOSTEN ������⳵�˵�ѹDC�ṩ+12V  -12V
  SwitcBoostENOnOff(eSwSta_On);
  //�������ر�
  LockSet(eSwSta_Off);
  //���Ƹ�ѹ�̵���
  DcOutSet(eSwSta_Off);
  //ģ������
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
  
  CHM_SendTick = 0;   //CHM����ʱ��ͳ��
  CRM_SendTick = 0;   //crm ����ʱ��ͳ��
  CRO_SendTick = 0;   //aǹcroʱ��ͳ��
  
  BmsCanInit();
  osDelay(20);
  
  while(1)
  {
    
    ChgPgnRecv(&PgnRecv);
    
    if(eSwSta_On == CcStatusRead())
    {
      if(0 == CcStatusChkTick)
        CcStatusChkTick = GetSystemTick();
    }
    else
      CcStatusChkTick = 0;

    if((0 != CcStatusChkTick) && ((GetSystemTick() - CcStatusChkTick) > 1500))
    {
      CcStatusChkTick = 0;

      if(0 != ChargeEnable && GetDcModuleVoltA() > 1800)
      {
        pChargeCtrl->CurProcess = eChgPro_ShakeHand;
        pChargeCtrl->Param = 0;
      }
    }
    
    if(
        (0 != PillarError.Value.Emergency)
        || (0 != PillarError.Value.NoDcmod)
        || (0 != PillarError.Value.Meter)
        || (0 != PillarError.Value.Memory)
        || (0 != PillarError.Value.Lcd)
        || (0 != PillarError.Value.Door)
        || (0 != PillarError.Value.Contactor)
        || (0 != PillarError.Value.UniContactor)
        || (0 != PillarError.Value.InVolt)
        || (0 != PillarError.Value.ACInputOut)
        || (ResMsg.Valid != 0 && ResMsg.V2LDischargeReq.Value.V2lDisChargeSta != 4)
      )
    {
      pChargeCtrl->CurProcess = eChgPro_ChgIdle;
      pChargeCtrl->Param = 0;
    }
    
    /*  �����ػ����ܹرգ�
    if (ChargeEnable == 0 && ChargeStopType == eChgStop_Stop_ByKeyTurnOff)
    {
    
      pChargeCtrl->CurProcess = eChgPro_TurnOff;
      pChargeCtrl->Param = 0;
    }
    */
    
    if(eChgPro_ChgIdle != pChargeCtrl->CurProcess)
      return;

    osDelay(100);
  }
}

