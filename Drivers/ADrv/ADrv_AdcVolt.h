#ifndef  __ADRV_ADCVOLT_H__
#define  __ADRV_ADCVOLT_H__

#include "Global.h"
#include "Drv_ADC.h"


#define GetAdcVolt(AdcVaule)   (float)(AdcVaule*3.256/4096);

//VIN 输入有效值（市电供电）
#define VinInVolt_Vaild        (11.65*100)
//VIN 输入无效值（没有市电供电）
#define VinOutVolt_Invaild     (5.0*100)

//车辆放电电压，依其判断车辆是否开始放电
#define DisChargeVolt_Vaild    (220*100)
#define DisChargeVolt_Invaild  (20*100)

extern u16 GetVinValue(void);
extern u16 GetDCVolt(void);
extern void GetAdcVoltHandler(void);

extern u16 AdcVoltBuf[EnumVadc_MAX];
extern u32 AdcBuffer[EnumVadc_MAX];


#endif