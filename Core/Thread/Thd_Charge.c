
#include "Thd_Charge.h"
#include "Param_UserConfig.h"
#include "ADrv_Comm.h"
#include "ADrv_BmsFrame.h"
#include "ADrv_DcModel.h"


u8 ChargeEnable = 0;
u8 DisChargeEnable = 0;

u8 ChargePauseFlag = 0;
u8 UniChgEnable = FALSE;

CarTypeEnum CarTypeVar = OtherCar_Type;


EnumChgStopType ChargeStopType = eChgStop_None;
EnumChgStopType DisChargeStopType = eChgStop_None;

u8  CommErrorCount = 0;
u32 CommErrFrame[4] = {0};

u32 CHM_SendTick = 0;//CHM发送时间统计
u32 CRM_SendTick = 0;//crm 发送时间统计
u32 CRO_SendTick = 0;//a枪cro时间统计
u32 NTM_SendTick = 0; //NTM 协商版本发送时间统计
u32 V2L_SendTick = 0; //DisCharge 放电请求时间统计



extern void ChgProChgIdle(StructChargeCtrl *Param);

extern void ChgProShakeHand(StructChargeCtrl *Param);

extern void ChgProImdCheck(StructChargeCtrl *pChargeCtrl);
extern void ChgProChgRecognizeEx(StructChargeCtrl *pChargeCtrl);

extern void ChgProChgRecognize(StructChargeCtrl *Param);
extern void ChgProChgConfigure(StructChargeCtrl *pChargeCtrl);
extern void ChgProWaitBmsReady(StructChargeCtrl *pChargeCtrl);
extern void ChgProWaitPillarReady(StructChargeCtrl *pChargeCtrl);
extern void ChgProChgStart(StructChargeCtrl *pChargeCtrl);
extern void ChgProChgRun(StructChargeCtrl *pChargeCtrl);
//extern void ChgProWaitVoltChgRun(StructChargeCtrl *pChargeCtrl);
extern void ChgProChgStop(StructChargeCtrl *pChargeCtrl);
extern void ChgProCommBeak(StructChargeCtrl *pChargeCtrl);
extern void ChgProCommError(StructChargeCtrl *pChargeCtrl);
extern void ChgProChgBreak(StructChargeCtrl *pChargeCtrl);

extern void ChgProSleep(StructChargeCtrl *pChargeCtrl);
extern void ChgProTurnOn(StructChargeCtrl *pChargeCtrl);
extern void ChgProTurnOff(StructChargeCtrl *pChargeCtrl);

//放电线程
extern void DisChgProChgIdle(StructChargeCtrl *pChargeCtrl);
extern void DisChgProV2L(StructChargeCtrl *pChargeCtrl);
extern void DisChgProShakeHand(StructChargeCtrl *Param);


extern void DisChgProChgRecognizeEx(StructChargeCtrl *pChargeCtrl);

extern void DisChgProChgRecognize(StructChargeCtrl *Param);
extern void DisChgProChgConfigure(StructChargeCtrl *pChargeCtrl);
extern void DisChgProWaitBmsReady(StructChargeCtrl *pChargeCtrl);
extern void DisChgProWaitPillarReady(StructChargeCtrl *pChargeCtrl);
extern void DisChgProChgStart(StructChargeCtrl *pChargeCtrl);
extern void DisChgProChgRun(StructChargeCtrl *pChargeCtrl);
//extern void DisChgProWaitVoltChgRun(StructChargeCtrl *pChargeCtrl);
extern void DisChgProChgStop(StructChargeCtrl *pChargeCtrl);
extern void DisChgProCommBeak(StructChargeCtrl *pChargeCtrl);
extern void DisChgProCommError(StructChargeCtrl *pChargeCtrl);
extern void DisChgProChgBreak(StructChargeCtrl *pChargeCtrl);


const pChargeCtrlHandle ChargeCtrlHandle[] =
{
  
  ChgProSleep,
  ChgProTurnOn,
  ChgProTurnOff,
  
  //放电线程函数
  DisChgProChgIdle,
  DisChgProV2L,
  DisChgProShakeHand,
  
  DisChgProChgRecognizeEx,

  DisChgProChgRecognize,
  DisChgProChgConfigure,
  DisChgProWaitBmsReady,
  DisChgProWaitPillarReady,
  DisChgProChgStart,
  DisChgProChgRun,
  //DisChgProWaitVoltChgRun,
  DisChgProChgStop,
  DisChgProCommBeak,
  DisChgProCommError,
  DisChgProChgBreak,

  //充电线程函数
  ChgProChgIdle,
  
  ChgProShakeHand,

  ChgProImdCheck,
  ChgProChgRecognizeEx,

  ChgProChgRecognize,
  ChgProChgConfigure,
  ChgProWaitBmsReady,
  ChgProWaitPillarReady,
  ChgProChgStart,
  ChgProChgRun,
  //ChgProWaitVoltChgRun,
  ChgProChgStop,
  ChgProCommBeak,
  ChgProCommError,
  ChgProChgBreak,
    
  ChgProChgIdle
};

StructChargeCtrl ChargeCtrl;

osTimerId ChargeTimer;
u32 ChargeTimeCount;//秒
u8 ChargeTimeCountEn = 0;

void ChargeTimerSet(EnumSwitchStatus OnOff)
{
  if(eSwSta_Off == OnOff)
    ChargeTimeCountEn = 0;
  else
    ChargeTimeCountEn = 1;
}

u16 GetChgMin(void)
{
  u32 Temp;

  Temp = ChargeTimeCount/60;
  if(Temp > 600)
    Temp = 600;
  return Temp;
}

u32 GetChgSec(void)
{
  return ChargeTimeCount;
}


void CleanChgTimeCnt(void)
{
  ChargeTimeCount = 0;
}

void ChargeTimeCountCtrl(void const *Param)
{
  if(ChargeTimeCountEn)
    ChargeTimeCount++;
}



/* the system main thread */
void Thd_Charge(void const *parameter)
{
  
  //BMS 通信参数初始化
  BmsCommInit();
  //模块通信can初始化
  DcModuleInit();
  
  /* Create a Timer */
  osTimerDef(ChargeTimer, ChargeTimeCountCtrl);
  ChargeTimer = osTimerCreate(osTimer(ChargeTimer), osTimerPeriodic, (void *)0);
  osTimerStart(ChargeTimer, 1000);//一秒

  ChargeCtrl.CurProcess = eChgPro_Sleep;
  ChargeCtrl.Param = 0;

  osDelay(100);
  while(1)
  {

    if(ChargeCtrl.CurProcess < eChgPro_Max)
      ChargeCtrlHandle[ChargeCtrl.CurProcess](&ChargeCtrl);
    else
    {
      ChargeCtrl.CurProcess = eChgPro_Sleep;
      ChargeCtrl.Param = 0;
    }

    osDelay(5);
  }
}

