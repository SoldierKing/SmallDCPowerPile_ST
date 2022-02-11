#ifndef __DRV_LET_H__
#define __DRV_LET_H__
#include "Global.h"

//�ƹ�Ч���������ݽṹ
typedef struct 
{
  u8 Count;
  u8 Shans;
  u16 KeepOnMs;
  u16 KeepOffMs;
  u16 GapTimeMs;
  u32 TimeTick;
  u32 Tick;
  u8  Sta;
  u32 OnMs;
  u32 OffMs;
}StructFLECfg;

//TIM PWM����
#define Pulse          (100)

typedef void (*pSwitchOnOffHandler)(EnumSwitchStatus Sta);
typedef void (*pSetDutyHandler)(u16 Duty);

extern void SwitchLEDSta(pSwitchOnOffHandler pSwitchOnOff, EnumSwitchStatus Sta);
extern void LedTwinkle(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg);
extern void LedTwinkleAlways(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg);
extern void LedTwinkleCycle(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg);
extern void LedChargeBreathe(pSetDutyHandler LedSetDuty,StructFLECfg *FleCfg);



#endif
