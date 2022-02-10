#ifndef  __ADRV_ONEKEYSTART_H__
#define  __ADRV_ONEKEYSTART_H__

#include "Global.h"

typedef enum
{

  Enum_ChargeWork,
  Enum_DischargeWork,
  
}EnumWorkMode;


typedef struct
{

  FunctionalState StateLock;
  FunctionalState SwitchModeEN;
  EnumWorkMode  WorkMode;
  u32  KeepKeyDownTime;
  
}StructWorkModeCfg;

extern void SwitchWorkMode(EnumWorkMode Mode);
extern EnumWorkMode GetWorkMode(void);
extern void OneKeyStop(void);
extern void StartKeyTurnOff(void);
extern void SwitchModeAuto(void);
extern void StartDisOrChargeKey(void);
extern void BatPowerShowByLed(void);
extern void ChargeACOffFault(void);

#endif


