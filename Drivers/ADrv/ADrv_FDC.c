#include "ADrv_FDC.h"



/// <summary>
///  通过IO 高低电平判断是否发生故障
/// </summary>
/// <param name="DetCfg">输入故障配置信息</param>  
int FaultDetByBit(StructFaultDetCfg *DetCfg)
{
  if (DetCfg->BitVaule  == DetCfg->Bit.TiggerBit)
      return 1;
  else if (DetCfg->BitVaule == DetCfg->Bit.RecBit)
      return 0;
  return -1;
}

/// <summary>
///  通过ADC 检测值判断是否发生故障
/// </summary>
/// <param name="DetCfg">输入故障配置信息</param>  
int FaultDetByAdcValue(StructFaultDetCfg *DetCfg)
{
  //超出上限模式
  if (DetCfg->AdcValue.DetType == OverLimit)
  {
    if (DetCfg->AdcValue.UpLimitValue <= DetCfg->FaultVaule)
	return 1;
    if (DetCfg->AdcValue.UpLimitRecValue > DetCfg->FaultVaule)
        return 0;
  }
  //超出下限模式
  else if (DetCfg->AdcValue.DetType == LowLimit)
  {
    if (DetCfg->AdcValue.LowLimitValue >= DetCfg->FaultVaule)
	return 1;
    if (DetCfg->AdcValue.LowLimitRecValue < DetCfg->FaultVaule)
        return 0;
  }
  //在上限与下限之间
  else if (DetCfg->AdcValue.DetType == MiddleLimit)
  {
    if (DetCfg->AdcValue.LowLimitValue <= DetCfg->FaultVaule
        && DetCfg->AdcValue.UpLimitValue >= DetCfg->FaultVaule)
        return 1;
    if (DetCfg->AdcValue.LowLimitRecValue > DetCfg->FaultVaule
        || DetCfg->AdcValue.UpLimitRecValue < DetCfg->FaultVaule)
        return 0;
  }
  return -1;
}

/// <summary>
///  判断是否发生故障
/// </summary>
/// <param name="DetCfg">输入故障配置信息</param>  
int FaultDetct(StructFaultDetCfg *DetCfg)
{
  u8 Err = 0;
  int Rtn = 0;
  if (DetCfg->DetType == DetByBitAction)
  	Err = FaultDetByBit(DetCfg);
  else if (DetCfg->DetType == DetByAdcValue)
  	Err = FaultDetByAdcValue(DetCfg);
  else
  	return -1;
  //通过时间判断
  if (DetCfg->DetBasisType == ByTimer)
  {
	//故障检测	
	if (Err == 1 && DetCfg->Erro == 0 && DetCfg->FaultEn == Enable)
	{
      if (DetCfg->Timer.Tick == 0) DetCfg->Timer.Tick = GetSystemTick();
	}else DetCfg->Timer.Tick = 0;
	if (0 != DetCfg->Timer.Tick)
	{
      if ((GetSystemTick() - DetCfg->Timer.Tick) > DetCfg->Timer.FaultContinueTime)
      {
        DetCfg->Erro = 1;
		DetCfg->Timer.Tick = 0;
		Rtn = 1;
                if (DetCfg->ErrCount < 255)
                  DetCfg->ErrCount++;
	  }
	}
	//故障恢复检测
	if (DetCfg->Erro == 1 && DetCfg->FaultRecEn ==  Enable && DetCfg->RecLock == UnLock)
	{
      if(Err == 0)
      {
        if(DetCfg->Timer.RecTick == 0) DetCfg->Timer.RecTick = GetSystemTick();
      }else DetCfg->Timer.RecTick = 0;

      if (DetCfg->Timer.RecTick != 0)
	  {
        if ((GetSystemTick() - DetCfg->Timer.RecTick) > DetCfg->Timer.FaultRecTime)
        {
          DetCfg->Erro = 0;
	      DetCfg->Timer.RecTick = 0;
		  Rtn = 2;
                  //DetCfg->ErrCount = 0;
		}
      }
	}else DetCfg->Timer.RecTick = 0;
  }
  //通过计数判断
  else if (DetCfg->DetBasisType == ByCount)
  {
    if (Err != 1)
    {
      if (DetCfg->Count.Count) DetCfg->Count.Count--;
	}
	else 
	{
      if (DetCfg->Count.Count < DetCfg->Count.FaultCounts) 
	  	DetCfg->Count.Count++;
	}
	if (DetCfg->Count.Count >= DetCfg->Count.FaultCounts && DetCfg->FaultEn==  Enable) 
	{
		DetCfg->Erro = 1;
		Rtn = 1;
                if (DetCfg->ErrCount < 255)
                DetCfg->ErrCount++;
    }
	else if (DetCfg->Count.Count <= DetCfg->Count.FaultRecCount &&
		DetCfg->FaultRecEn ==  Enable && DetCfg->RecLock == UnLock)
	{
		DetCfg->Erro = 0;
		Rtn = 2;
	}
  }
  else
  	 return -1;
  
  return Rtn;
}
