#ifndef __PARAM_PILEVAR_H__
#define __PARAM_PILEVAR_H__
#include "Global.h"
//#include "ADrv_BmsFrame.h"


typedef struct
{
  u8  Vaild;
  u8  PositWarn;
  u8  NegatWarn;
  u16 PositVolt;
  u16 NegatVolt;
  u16 PnVolt;
  u16 PositResist;
  u16 NegatResist;
}StructImdValue;


typedef enum
{
  eMeterSta_Unavail = 0,
  eMeterSta_Avail
}EnumMeterStatus;

typedef struct
{
  EnumMeterStatus Status;
  s32 Energy;//0.01Kwh/bit
  s32 Sharp;//0.01Kwh/bit
  s32 Peak;//0.01Kwh/bit
  s32 Flat;//0.01Kwh/bit
  s32 Valley;//0.01Kwh/bit
  u32 Voltage;//0.1V/bit
  s32 Current;//0.001A/bit
  u32 VoltageA;// A µçÑ¹
  u32 VoltageB;
  u32 VoltageC;
  //s32 CurrentA;// A µçÁ÷
  //s32 CurrentB;
  //s32 CurrentC;
  s32 Power;//0.0001Kw/bit
  s32 PowerFac;//0.001/bit
  s32 Freq;//0.01Hz/bit
}StructMeterValue;


typedef struct
{
  u32 *pAdcBuff;
  StructImdValue *pImdValue;
  StructMeterValue *pMeterValue;
  u16 *pAdcVoltBuf;
  
}StructPileParam;



extern StructImdValue ImdValue;
extern StructMeterValue MeterValue;


#endif