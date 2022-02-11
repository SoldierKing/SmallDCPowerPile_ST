#include "ADrv_FDC.h"



/// <summary>
///  ͨ��IO �ߵ͵�ƽ�ж��Ƿ�������
/// </summary>
/// <param name="DetCfg">�������������Ϣ</param>  
int FaultDetByBit(StructFaultDetCfg *DetCfg)
{
  if (DetCfg->BitVaule  == DetCfg->Bit.TiggerBit)
      return 1;
  else if (DetCfg->BitVaule == DetCfg->Bit.RecBit)
      return 0;
  return -1;
}

/// <summary>
///  ͨ��ADC ���ֵ�ж��Ƿ�������
/// </summary>
/// <param name="DetCfg">�������������Ϣ</param>  
int FaultDetByAdcValue(StructFaultDetCfg *DetCfg)
{
  //��������ģʽ
  if (DetCfg->AdcValue.DetType == OverLimit)
  {
    if (DetCfg->AdcValue.UpLimitValue <= DetCfg->FaultVaule)
	return 1;
    if (DetCfg->AdcValue.UpLimitRecValue > DetCfg->FaultVaule)
        return 0;
  }
  //��������ģʽ
  else if (DetCfg->AdcValue.DetType == LowLimit)
  {
    if (DetCfg->AdcValue.LowLimitValue >= DetCfg->FaultVaule)
	return 1;
    if (DetCfg->AdcValue.LowLimitRecValue < DetCfg->FaultVaule)
        return 0;
  }
  //������������֮��
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
///  �ж��Ƿ�������
/// </summary>
/// <param name="DetCfg">�������������Ϣ</param>  
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
  //ͨ��ʱ���ж�
  if (DetCfg->DetBasisType == ByTimer)
  {
	//���ϼ��	
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
	//���ϻָ����
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
  //ͨ�������ж�
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
