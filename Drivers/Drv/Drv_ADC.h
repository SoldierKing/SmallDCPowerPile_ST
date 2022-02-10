#ifndef __DRV_ADC_H__
#define __DRV_ADC_H__
#include "Global.h"
#include "Param_PileVar.h"


typedef enum
{
  //正极电压(对地）
  EnumVpdc_AD0 = 0,
  //负极电压（对滴）
  EnumVndc_AD1,
  //HDC正负极电压（正极对负极）
  EnumVhdc_AD2,
  //车端电压
  EnumVddc_AD3,
  
  EnumVt1_ADC,
  EnumVt2_ADC,

  //12V供电电压(模块供电:12V;电池或超级电容供电:5V)
  EnumVin_VOL,
  //电池或超级电容电压
  EnumVbat_VOL,
  
  EnumVadc_MAX,
}EnumADCType;


extern void ADCInit(void);
extern void ADCStart(u32 *AdcBuff);
extern uint32_t ADC_Get_Average(EnumADCType ch,uint8_t times);
extern void GetAdcBuffValue(u32 *AdcBuff);

#endif