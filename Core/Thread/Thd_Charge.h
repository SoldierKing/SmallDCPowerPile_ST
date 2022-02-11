
#ifndef __THD_CHARGE_H__
#define __THD_CHARGE_H__

#include "Global.h"

#define CC_CheckTick    (100)
#define MsgTimeout_1S   (1000+50)
#define MsgTimeout_5S   (5000+100)//原100
#define MsgTimeout_10S  (10*1000+1000)
#define MsgTimeout_60S  (60*1000+3000) //yuan3000
#define MsgTimeout_10M  (10*60*1000+3000)

#define TimeOut   (u32)(60*1000*5)

#define SleepTimeOut  (u32)(15*1000)

typedef struct
{
  u8 DontSendA1_Flag;
  u8 SendA1LenFail_Flag;
  u8 SendA1VerNoV1_1_Flag;
  u8 SendA1VerV1_1_0_Flag;
  u8 SendA1Ver3_0_0AndFail_Flag;
}NegoConfigFlag;

typedef enum
{
    Normal,
    DontSendA1,
    SendA1LenFail,
    SendA1VerNoV1_1_0,
    SendA1VerVer1_0_0,
    SendA1Ver3_0_0Fail,
    
}NegoVersionTypeEnum;

typedef enum
{
  
  //充放电桩状态
  eChgPro_Sleep = 0,
  eChgPro_TurnOn,
  eChgPro_TurnOff,
  
  //放电线程
  eChgPro_DisChgIdle,
  eChgPro_DisV2L,
  eChgPro_DisShakeHand,

  eChgPro_DisChgRecognizeEx,

  eChgPro_DisChgRecognize,
  eChgPro_DisChgConfigure,
  eChgPro_DisWaitBmsReady,
  eChgPro_DisWaitPillarReady,
  eChgPro_DisChgStart,
  eChgPro_DisChgRun,
  //eChgPro_DisWaitVoltChgRun,
  eChgPro_DisChgStop,
  eChgPro_DisCommBreak,
  eChgPro_DisCommError,
  eChgPro_DisChgBreak,
  
  //充电线程
  eChgPro_ChgIdle,
  eChgPro_ShakeHand,

  eChgPro_ImdCheck,
  eChgPro_ChgRecognizeEx,

  eChgPro_ChgRecognize,
  eChgPro_ChgConfigure,
  eChgPro_WaitBmsReady,
  eChgPro_WaitPillarReady,
  eChgPro_ChgStart,
  eChgPro_ChgRun,
  //eChgPro_WaitVoltChgRun,
  eChgPro_ChgStop,
  eChgPro_CommBreak,
  eChgPro_CommError,
  eChgPro_ChgBreak,

  eChgPro_Max,
}EnumChargeProcess;

typedef struct
{
  EnumChargeProcess CurProcess;
  //EnumChargeProcess AimStep;
  void *Param;
}StructChargeCtrl;

typedef void (*pChargeCtrlHandle)(StructChargeCtrl *pChgCtrl);


extern u8 ChargeEnable;
extern u8 DisChargeEnable;
extern u8 ChargePauseFlag;
extern u8 UniChgEnable;
extern StructChargeCtrl ChargeCtrl;

extern CarTypeEnum CarTypeVar;

extern s32 StartChgEnergy;
extern s32 StopChgEnergy;

extern EnumChgStopType ChargeStopType;
extern EnumChgStopType DisChargeStopType;

extern u8 CommErrorCount;
extern u32 CommErrFrame[4];

extern u32 ChargeTimeCount;//秒
extern void ChargeTimerSet(EnumSwitchStatus OnOff);
u16 GetChgMin(void);
extern u32 GetChgSec(void);
extern void CleanChgTimeCnt(void);
extern u32 CHM_SendTick ;//CHM发送时间统计
extern u32 CRM_SendTick ;//crm 发送时间统计
extern u32 CRO_SendTick ;//a枪cro时间统计
extern u32 NTM_SendTick ; 
extern u32 V2L_SendTick;

extern void Thd_Charge(void const *parameter);


#endif

