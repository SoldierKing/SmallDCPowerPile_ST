#include "ADrv_AdcVolt.h"


u16 AdcVoltBuf[EnumVadc_MAX] = {0};
u32 AdcBuffer[EnumVadc_MAX] = {0};

/// <summary>
///  获取车端实际电压
/// </summary>
/// <param name="float AdcVolt">输入ADC电压值</param>  
static u16 GetHDCVolt(float AdcVolt)
{
  return (u16)((float)(((AdcVolt*(15+1) - (3*1))/15)*2310/7.5)*10);
}

/// <summary>
///  获取模块正负极对地电压
/// </summary>
/// <param name="float AdcVolt">输入ADC电压值</param>  
static float GetHPEDcVolt(float AdcVolt)
{
  return (float)((AdcVolt*7*330)/7.5);
}

/// <summary>
///  获取电池电压值
/// </summary>
/// <param name="u16">返回VIN电压值</param>  
u16 GetVbatVolt(void)
{
  
  float AdcVolt = 0;
  AdcVolt = GetAdcVolt(AdcBuffer[EnumVbat_VOL]);
  AdcVolt = AdcVolt*133/100;
  return (u16)(AdcVolt*100);
  
}

/// <summary>
///  获取Vin 模块辅源输入电压
/// </summary>
/// <param name="u16">返回VIN电压值</param>  
u16 GetVinValue(void)
{
  
  float Temp = 0;
  Temp = GetAdcVolt(AdcBuffer[EnumVin_VOL]);
  return (u16)(Temp*6*100);
  
}

/// <summary>
///  获取车端DC电压
/// </summary>
/// <param name="u16">返回车端DC电压值</param>  
u16 GetDCVolt(void)
{
  
  float Temp = 0;
  Temp = GetAdcVolt(AdcBuffer[EnumVddc_AD3]);
  if (Temp < 0.2 ) return 1;

  return (u16)GetHDCVolt(Temp);
}

/// <summary>
///  获取正极对地电压
/// </summary>
/// <param name="u16">返回正极对地电压/param>  
u16 GetHPpeVolt(void)
{
  
  float Temp = 0;
  Temp = GetAdcVolt(AdcBuffer[EnumVpdc_AD0]);

  return (u16)(GetHPEDcVolt(Temp)*10);
}

/// <summary>
///  获取负极对地电压
/// </summary>
/// <param name="u16">返回负极对地电压/param>  
u16 GetHNpeVolt(void)
{
  
  float Temp = 0;
  Temp = GetAdcVolt(AdcBuffer[EnumVndc_AD1]);

  return (u16)(GetHPEDcVolt(Temp)*10);
}

/// <summary>
///  获取正负极电压
/// </summary>
/// <param name="u16">返回负极对地电压/param>  
u16 GetHPNpeVolt(void)
{
  
  float Temp = 0;
  Temp = GetAdcVolt(AdcBuffer[EnumVhdc_AD2]);
  if (Temp < 0.2 ) return 1;

  return (u16)GetHDCVolt(Temp);
}

/// <summary>
///  获取ADC各通道实际电压值 0.1精度
/// </summary>
/// <param name="void"></param>  
void GetAdcVoltHandler(void)
{

  AdcVoltBuf[EnumVpdc_AD0] = (u16)GetHPpeVolt();
  AdcVoltBuf[EnumVndc_AD1] = (u16)GetHNpeVolt();
  AdcVoltBuf[EnumVhdc_AD2] = GetHPNpeVolt();
  AdcVoltBuf[EnumVddc_AD3] = GetDCVolt();
  AdcVoltBuf[EnumVin_VOL] = GetVinValue();
  AdcVoltBuf[EnumVbat_VOL] = GetVbatVolt();
  
}


