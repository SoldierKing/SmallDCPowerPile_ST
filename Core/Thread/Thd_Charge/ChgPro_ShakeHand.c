
#include "Thd_Charge.h"
#include "ADrv_AdcVolt.h"
#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "ADrv_ImdCheck.h"



s32 GetDCVoltBeforInsulation(void)
{
  
  if(GetBoostENSwitchSta() != GPIO_PIN_SET)
  {
    SwitcBoostENOnOff(eSwSta_On);
    osDelay(15);
  }
  
  return (s32)AdcVoltBuf[EnumVddc_AD3];
    
}

void ChgProShakeHand(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  CHM_SendTick = 0;
  u32 CcStatusChkTick = 0;
  u32 BHM_FirstRecv = 0;

  
  ChargeError.Value.DrainResist = 0;
  ChargeError.Value.IsolateWarn = 0;   //��Ե����
  ChargeError.Value.IsolateError = 0;
  ChargeError.Value.VoltOutError = 0;
  ChargeError.Value.CurrtOutError = 0;

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
  

  ChargeStopType = eChgStop_None;

  BhmMsg.Valid = 0;
  BhmMsg.RecvTick = GetSystemTick();
  BHM_FirstRecv = 0;

  ChargeTimerSet(eSwSta_Off);
  CleanChgTimeCnt();
  //��BMS��Դ����
  BmsPowerSet(eSwSta_On);
  //�򿪵���ǹ������
  LockSet(eSwSta_On);
  //�رո�ѹ�̵������
  DcOutSet(eSwSta_Off);
  //�ر�ģ�����
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
 
  //��BOOSTEN ������⳵�˵�ѹADC�ṩ+12V  -12V
  //SwitcBoostENOnOff(eSwSta_On);
  //osDelay(50);
  
  //BmsCanInit();
  while(pdTRUE == xQueueReceive(BmsMsg, &PgnRecv, 0));
  
  CHM_Send();
  CHM_SendTick = GetSystemTick();

  while(1)
  {
    
    ChgPgnRecv(&PgnRecv);
    
    if((0 != CHM_SendTick) && ((GetSystemTick() - CHM_SendTick) > 250))
    {
      CHM_Send();
      CHM_SendTick = GetSystemTick();
    }

    
    if(eChgVer_2011 != UserParam.ChgVer)
    {
      
      if(0 != BhmMsg.Valid)//�¹���        
      {//�Ѿ��յ�BHM���ģ�
        ChgVer = eChgVer_2015;
        if(0 == BHM_FirstRecv)
          BHM_FirstRecv = GetSystemTick();
        
        if(0 != ImdValue.Vaild)
        {
          if( GetDCVoltBeforInsulation() > 100)
          {
            ChargeStopType = eChgStop_Imd_IsolateError;
            pChargeCtrl->CurProcess = eChgPro_ChgStop;
            pChargeCtrl->Param = 0;
          }
          else
          {
 
            pChargeCtrl->CurProcess = eChgPro_ImdCheck;
            pChargeCtrl->Param = 0;
          }
        }
        else
        {
          if((GetSystemTick() - BHM_FirstRecv) > MsgTimeout_5S)
          {
            
            ChargeError.Value.IsolateWarn = 1;  //��Ե����
            pChargeCtrl->CurProcess = eChgPro_ChgRecognize;
            pChargeCtrl->Param = 0;
          }
        }
      }
      else if((GetSystemTick() - BhmMsg.RecvTick) > MsgTimeout_5S)
      {
        //�ɹ��꣬�ȶϿ�Bms�����پ�Ե���
        ChgVer = eChgVer_2011;
        BmsPowerSet(eSwSta_Off);
        CHM_SendTick = 0;
        if(0 != ImdValue.Vaild)
        {
          pChargeCtrl->CurProcess = eChgPro_ImdCheck;
          pChargeCtrl->Param = 0;
        }
        else if((GetSystemTick() - BhmMsg.RecvTick) > (MsgTimeout_5S*3/2))
        {
          BmsPowerSet(eSwSta_On);
          pChargeCtrl->CurProcess = eChgPro_ChgRecognize;
          pChargeCtrl->Param = 0;
        }
       }
    }
    else
    {
      if(0 != BhmMsg.Valid)//�¹���
      {
        
        ChgVer = eChgVer_2015;
        if(0 == BHM_FirstRecv)
          BHM_FirstRecv = GetSystemTick();
        if((GetSystemTick() - BHM_FirstRecv) > MsgTimeout_5S)
        {
          pChargeCtrl->CurProcess = eChgPro_ChgRecognize;
          pChargeCtrl->Param = 0;
        }
      }
      else if((GetSystemTick() - BhmMsg.RecvTick) > (MsgTimeout_5S/2))
      {
        //�ɹ���
        ChgVer = eChgVer_2011;
        pChargeCtrl->CurProcess = eChgPro_ChgRecognizeEx;
        pChargeCtrl->Param = 0;
      }
    }
    
    if(0 == ChargeEnable)
    {//�ⲿ���ƽ������
      if (ChargeStopType != eChgStop_Stop_ByKeyTurnOff)
        ChargeStopType = eChgStop_Shake_UserCancel;
      pChargeCtrl->CurProcess = eChgPro_ChgStop;
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
      ChargeStopType = eChgStop_Shake_CcOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Emergency)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Shake_EmerClose;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Door)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Shake_DoorOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Contactor)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Shake_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.UniContactor)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Shake_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.InVolt)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Shake_InVolt;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    if(0 != PillarError.Value.NoDcmod)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Shake_NoDcmod;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Meter)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Shake_Meter;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    
    if(eChgPro_ShakeHand != pChargeCtrl->CurProcess)
      return;

    osDelay(5);
  }
}

