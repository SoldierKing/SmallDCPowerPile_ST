
#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"
#include "ADrv_ImdCheck.h"
#include "ADrv_AdcVolt.h"


#define ImdCheckFreq          (200)//ms
#define DcModOutTimeOut       (11000)//ms
#define VoltWaitTimeOut       (1000)//ms
#define ChkWaitTimeOut        (1200)//(8500)//ms
#define ChkTimeOut            (1500)//ms
#define ImdCheckCount         (3)
#define BleederCheckTimeOut   (3500)//ms

typedef enum
{
  eImdStep_Start= 0,
  eImdStep_DcModOut,
  eImdStep_VoltWait,
  eImdStep_ChkWait,
  eImdStep_Check,
  eImdStep_Bleeder,
  eImdStep_Stop,
  eImdStep_Finish,
}EnumImdChkStep;

/**************************************************************************************************
** 函数名: GetImdCheck
** 输　入: 无
** 输　出: 返回0 未有检测结果    返回1 检测通过    返回-1 绝缘电阻低警告   返回-2 绝缘故障
** 描  述: 获取绝缘检测结果
**************************************************************************************************/
s32 GetImdCheck(void)
{
  u32 PositResist;
  u32 NegatResist;
  u32 Volt;
  u32 Resist;

  if(0 == ImdValue.Vaild)
    return 0;

  PositResist = ImdValue.PositResist;
  NegatResist = ImdValue.NegatResist;
  Volt = GetDcModuleVolt();

  if(PositResist < NegatResist)
    Resist = (PositResist*10000);
  else
    Resist = (NegatResist*10000);

  if(Resist >= (505*Volt))
    return 1;
  else if(Resist >= (108*Volt))
    return -1;
  else
    return -2;
}

/**************************************************************************************************
** 函数名: GetBleederResult
** 输　入: 无
** 输　出: 返回0 未有检测通过    返回1 检测通过
** 描  述: 获取泄放电路结果
**************************************************************************************************/
u8 GetBleederResult(void)
{
  if(AdcVoltBuf[EnumVddc_AD3] < 600)
    return TRUE;
  return FALSE;
}

void ImdCheckSet(EnumSwitchStatus Set)
{
  
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Set) return;
  if (Set  == eSwSta_On)
  {
    GetIMDCheckResult(&ImdValue);
  }
  else if (Set == eSwSta_Off)
    ;
  LastSta = Set;
}

void ImdProcess(EnumImdChkStep *pImdChckStep)
{
  static u32 ImdCheckTick = 0;
  static u32 ImdTimeout = 0;
  static u8 ImdNoErrCnt = 0;
  static u8 ImdWarnCnt = 0;
  static u8 ImdErrCnt = 0;

  s32 Tmep;

  if((GetSystemTick() - ImdCheckTick) < ImdCheckFreq)
    return;

  ImdCheckTick = GetSystemTick();

  switch(*pImdChckStep)
  {
    case eImdStep_Start:
      DcOutSet(eSwSta_On);
      ImdCheckSet(eSwSta_Off);
      if(0 != BhmMsg.Valid)//最大允许电压有效
      {
        if(BhmMsg.ImdMaxVolt <= DcModuleAbility.MaxVolt)
          Tmep = BhmMsg.ImdMaxVolt;
        else
          Tmep = DcModuleAbility.MaxVolt;
      }
      else
        Tmep = DcModuleAbility.MinVolt;

      DcModuleSet(eSwSta_On, Tmep, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      *pImdChckStep = eImdStep_DcModOut;
      ImdTimeout = 0;
      break;
    case eImdStep_DcModOut:
      if(0 == ImdTimeout)
        ImdTimeout = GetSystemTick();
      else if((GetSystemTick() - ImdTimeout) > DcModOutTimeOut)
      {
        *pImdChckStep = eImdStep_VoltWait;
        ImdTimeout = 0;
      }
      if(GetDcModuleVolt() >= ((GetDcModuleVoltSet()>>3)*7.8))
      {
        *pImdChckStep = eImdStep_VoltWait;
        ImdTimeout = 0;
      }
      break;
    case eImdStep_VoltWait:
      if(0 == ImdTimeout)
        ImdTimeout = GetSystemTick();
      else if((GetSystemTick() - ImdTimeout) > VoltWaitTimeOut)
      {
        ImdCheckSet(eSwSta_On);
        *pImdChckStep = eImdStep_ChkWait;
        ImdTimeout = 0;
      }
      if(AdcVoltBuf[EnumVddc_AD3] >= ((GetDcModuleVolt()>>3)*7.8))
      {
        ImdCheckSet(eSwSta_On);
        *pImdChckStep = eImdStep_ChkWait;
        ImdTimeout = 0;
      }
      break;
    case eImdStep_ChkWait:
      if(0 == ImdTimeout)
        ImdTimeout = GetSystemTick();
      else if((GetSystemTick() - ImdTimeout) > ChkWaitTimeOut)
      {
        *pImdChckStep = eImdStep_Check;
        ImdTimeout = 0;
        ImdNoErrCnt = 0;
        ImdWarnCnt = 0;
        ImdErrCnt = 0;
      }
      break;
    case eImdStep_Check:
      if(0 == ImdTimeout)
        ImdTimeout = GetSystemTick();
      else if((0 != ImdTimeout) && ((GetSystemTick() - ImdTimeout) > ChkTimeOut))
      {//超时
        ChargeError.Value.IsolateWarn = 1;//绝缘警告
        ChargeError.Value.IsolateError = 0;

        DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);//关直流模块输出
        //osDelay(100);
        //DrainResistSet(eSwSta_On);
        ImdCheckSet(eSwSta_Off);
        *pImdChckStep = eImdStep_Bleeder;
        ImdTimeout = 0;
      }
      Tmep = GetImdCheck();
      if(0 != Tmep)
      {
        if(1 == Tmep)
          ImdNoErrCnt++;
        else if(-1 == Tmep)
          ImdWarnCnt++;
        else
          ImdErrCnt++;

        if(ImdNoErrCnt >= ImdCheckCount)
        {
          ChargeError.Value.IsolateWarn = 0;//绝缘警告
          ChargeError.Value.IsolateError = 0;//绝缘故障
          DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);//关直流模块输出
          //osDelay(100);
          //DrainResistSet(eSwSta_On);
          ImdCheckSet(eSwSta_Off);
          *pImdChckStep = eImdStep_Bleeder;
          ImdTimeout = 0;
        }
        else if(ImdWarnCnt >= ImdCheckCount)
        {
          ChargeError.Value.IsolateWarn = 1;//绝缘警告
          ChargeError.Value.IsolateError = 0;//绝缘故障
          DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);//关直流模块输出
          //osDelay(100);
          //DrainResistSet(eSwSta_On);
          ImdCheckSet(eSwSta_Off);
          *pImdChckStep = eImdStep_Bleeder;
          ImdTimeout = 0;
        }
        else if(ImdErrCnt >= ImdCheckCount)
        {
          ChargeError.Value.IsolateWarn = 0;//绝缘警告
          ChargeError.Value.IsolateError = 1;//绝缘故障
          DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);//关直流模块输出
          //osDelay(100);
          //DrainResistSet(eSwSta_On);
          ImdCheckSet(eSwSta_Off);
          *pImdChckStep = eImdStep_Bleeder;
          ImdTimeout = 0;
        }
      }
      break;
    case eImdStep_Bleeder:
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);//关直流模块输出
      DcOutSet(eSwSta_Off);
      //DrainResistSet(eSwSta_On);
      //ImdWorkSet(eSwSta_Off);
      ImdCheckSet(eSwSta_Off);

      if(0 == ImdTimeout)
        ImdTimeout = GetSystemTick();
      if((GetSystemTick() - ImdTimeout) > BleederCheckTimeOut)
      {//超时
        ChargeError.Value.DrainResist = 1;//泄放电阻警告
        //DrainResistSet(eSwSta_Off);
        *pImdChckStep = eImdStep_Stop;
      }

      if(FALSE != GetBleederResult())
      {
        ChargeError.Value.DrainResist = 0;//泄放电阻警告
        //DrainResistSet(eSwSta_Off);
        *pImdChckStep = eImdStep_Stop;
      }
      break;
    case eImdStep_Stop:
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);//关直流模块输出
      DcOutSet(eSwSta_Off);
      ImdCheckSet(eSwSta_Off);
      //DrainResistSet(eSwSta_Off);
      *pImdChckStep = eImdStep_Finish;
      break;
    case eImdStep_Finish:
      break;
    default:
      *pImdChckStep = eImdStep_Start;
      break;
  }
}

void ChgProImdCheck(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  u32 CcStatusChkTick = 0;

  EnumImdChkStep ImdChckStep;

  PillarError.Value.Insulation = 0;
  PillarError.Value.ImdDev = 0;
  PillarError.Value.DrainResist = 0;

  ChargeError.Value.DrainResist = 0;
  ChargeError.Value.IsolateWarn = 0;//绝缘警告
  ChargeError.Value.IsolateError = 0;
  ChargeError.Value.VoltOutError = 0;
  ChargeError.Value.CurrtOutError = 0;
  ChargeError.Mult = 0;

 // BmsCanInit();
 // osDelay(20);

  ChargeTimerSet(eSwSta_Off);
  CleanChgTimeCnt();
  //BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);
  DcOutSet(eSwSta_Off);
  
  BhmMsg.RecvTick = GetSystemTick();

  ImdChckStep = eImdStep_Start;

  while(1)
  {
    
    ChgPgnRecv(&PgnRecv);
    if((GetSystemTick() - CHM_SendTick) > 250)
    {
      if(eChgVer_2011 != ChgVer)
        CHM_Send();
      CHM_SendTick = GetSystemTick();
    }


    ImdProcess(&ImdChckStep);

    if(eImdStep_Finish == ImdChckStep)
    {
      if(1 == ChargeError.Value.IsolateError)
      {
        ChargeStopType = eChgStop_Imd_IsolateError;
        pChargeCtrl->CurProcess = eChgPro_ChgStop;
        pChargeCtrl->Param = 0;

      }
      else
      {
        pChargeCtrl->CurProcess = eChgPro_ChgRecognize;
        pChargeCtrl->Param = 0;
	CHM_SendTick = 0;

      }
    }

    if((eChgVer_2011 != ChgVer) && ((GetSystemTick() - BhmMsg.RecvTick) > MsgTimeout_5S))
    {//通信故障
      ChargeStopType = eChgStop_Imd_BhmTimeOut;
      pChargeCtrl->CurProcess = eChgPro_ChgStop;
      pChargeCtrl->Param = 0;

    }

    if(0 == ChargeEnable)
    {//外部控制结束充电
      if ( ChargeStopType != eChgStop_Stop_ByKeyTurnOff)
        ChargeStopType = eChgStop_Imd_UserCancel;
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
      ChargeStopType = eChgStop_Imd_CcOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Emergency)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Imd_EmerClose;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Door)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Imd_DoorOpen;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Contactor)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Imd_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.UniContactor)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Imd_Contactor;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.InVolt)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Imd_InVolt;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.NoDcmod)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Imd_NoDcmod;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    if(0 != PillarError.Value.Meter)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      ChargeStopType = eChgStop_Imd_Meter;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    if(0 != PillarError.Value.ACInputOut)
    {
      CstMsg.StopReason.Value.Truble = 1;
      CstMsg.StopTruble.Value.Other = 1;
      CstMsg.StopFault.Value.Volt = 1;
      ChargeStopType = eChgStop_Fault_ACInputOut;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    if (ResMsg.Valid != 0 && ResMsg.V2LDischargeReq.Value.V2lDisChargeSta != 4)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      CstMsg.StopTruble.Value.Other = 1;
	  ChargeStopType = eChgStop_VEH_RES_WorkModleErr;
      pChargeCtrl->CurProcess = eChgPro_ChgBreak;
      pChargeCtrl->Param = 0;
    }
    
    if(eChgPro_ImdCheck != pChargeCtrl->CurProcess)
      return;
    
    osDelay(5);
  }
}


