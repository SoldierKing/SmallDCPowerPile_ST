#ifndef __DRV_ADC_H__
#define __DRV_ADC_H__
#include "Global.h"
#include "Param_PileVar.h"


typedef enum
{
  //������ѹ(�Եأ�
  EnumVpdc_AD0 = 0,
  //������ѹ���ԵΣ�
  EnumVndc_AD1,
  //HDC��������ѹ�������Ը�����
  EnumVhdc_AD2,
  //���˵�ѹ
  EnumVddc_AD3,
  
  EnumVt1_ADC,
  EnumVt2_ADC,

  //12V�����ѹ(ģ�鹩��:12V;��ػ򳬼����ݹ���:5V)
  EnumVin_VOL,
  //��ػ򳬼����ݵ�ѹ
  EnumVbat_VOL,
  
  EnumVadc_MAX,
}EnumADCType;


extern void ADCInit(void);
extern void ADCStart(u32 *AdcBuff);
extern uint32_t ADC_Get_Average(EnumADCType ch,uint8_t times);
extern void GetAdcBuffValue(u32 *AdcBuff);

#endif