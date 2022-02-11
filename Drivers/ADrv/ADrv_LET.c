#include "ADrv_LET.h"


//��������
void SwitchLEDSta(pSwitchOnOffHandler pSwitchOnOff, EnumSwitchStatus Sta);
void LedTwinkle(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg);
void LedTwinkleAlways(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg);
void LedTwinkleCycle(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg);
void LedChargeBreathe(pSetDutyHandler LedSetDuty,StructFLECfg *FleCfg);


/// <summary>
///  ���ص�
/// </summary>
/// <param name="Sta">������Ҫ�ı��״̬</param>  
/// <param name="pSwitchOnOff">������Ҫ���ƵĵƵĶ���ָ��</param>  
void SwitchLEDSta(pSwitchOnOffHandler pSwitchOnOff, EnumSwitchStatus Sta)
{
  if (Sta  == eSwSta_On)
    pSwitchOnOff(eSwSta_On);
  else if (Sta == eSwSta_Off)
    pSwitchOnOff(eSwSta_Off);
}

/// <summary>
///  ����˸һ������(�ƹ�Ч��)
/// </summary>
/// <param name="pSwitchOnOff">������Ҫ���ƵĵƵĶ���ָ��</param>  
/// <param name=" *FleCfg">�ƹ�Ч��ָ��</param>  
void LedTwinkle(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg)
{
  if (FleCfg->Count < 1) return;
  if (FleCfg->Sta== 0)
  {
    if (FleCfg->TimeTick== 0)
    {
      pSwitchOnOff(eSwSta_On);
      FleCfg->TimeTick = GetSystemTick();
    }
    else if ((GetSystemTick() - FleCfg->TimeTick) > FleCfg->KeepOnMs) 
    {
      FleCfg->TimeTick = 0;
	  FleCfg->Sta = 1;
    }
  }
  else if (FleCfg->Sta == 1)
  {
    if (FleCfg->TimeTick == 0)
    {
      pSwitchOnOff(eSwSta_Off);
      FleCfg->TimeTick = GetSystemTick();
    }
	else if ((GetSystemTick() - FleCfg->TimeTick) > FleCfg->KeepOffMs) 
    {
      FleCfg->TimeTick = 0;
	  FleCfg->Sta = 0;
	  if (FleCfg->Count > 0)
	    FleCfg->Count --;
    }
  }
}

/// <summary>
///  �ƹ�һֱ��˸(�ƹ�Ч��)
/// </summary>
/// <param name="pSwitchOnOff">������Ҫ���ƵĵƵĶ���ָ��</param>  
/// <param name=" *FleCfg">�ƹ�Ч��ָ��</param>  
void LedTwinkleAlways(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg)
{

  if (FleCfg->Sta== 0)
  {
    if (FleCfg->TimeTick== 0)
    {
      pSwitchOnOff(eSwSta_On);
      FleCfg->TimeTick = GetSystemTick();
    }
    else if ((GetSystemTick() - FleCfg->TimeTick) > FleCfg->KeepOnMs) 
    {
      FleCfg->TimeTick = 0;
	  FleCfg->Sta = 1;
    }
  }
  else if (FleCfg->Sta == 1)
  {
    if (FleCfg->TimeTick == 0)
    {
      pSwitchOnOff(eSwSta_Off);
      FleCfg->TimeTick = GetSystemTick();
    }
	else if ((GetSystemTick() - FleCfg->TimeTick) > FleCfg->KeepOffMs) 
    {
      FleCfg->TimeTick = 0;
	  FleCfg->Sta = 0;
    }
  }
}


/// <summary>
///  �ƹ���˸N�κ�һ��ʱ����ظ���˸N�Σ�һֱѭ��(�ƹ�Ч��)
/// </summary>
/// <param name="pSwitchOnOff">������Ҫ���ƵĵƵĶ���ָ��</param>  
/// <param name=" *FleCfg">�ƹ�Ч��ָ��</param>  
void LedTwinkleCycle(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg)
{
  
   if (FleCfg->Tick== 0 )
   {
	 LedTwinkle(pSwitchOnOff,FleCfg);
	 if (FleCfg->Count== 0)
	 {
	   FleCfg->Tick = GetSystemTick();
	   FleCfg->Count = FleCfg->Shans;
	 }
   }
   else
   {
     if ((GetSystemTick() - FleCfg->Tick) > FleCfg->GapTimeMs)
     {
        FleCfg->Tick = 0;
     }
   }
}


/// <summary>
///  ������(�ƹ�Ч��)
/// </summary>
/// <param name="LedSetDuty">������Ҫ���ƵĵƵĶ���ָ��</param>  
/// <param name=" *FleCfg">�ƹ�Ч��ָ��</param>  
void LedChargeBreathe(pSetDutyHandler LedSetDuty,StructFLECfg *FleCfg)
{
  u32 TmepTickCnt;
  
  TmepTickCnt = GetSystemTick() - FleCfg->TimeTick;

  if(TmepTickCnt < FleCfg->OnMs)
  {
    if (FleCfg->Count < Pulse)
      FleCfg->Count++;
    LedSetDuty(1*FleCfg->Count);
  }
  else if(TmepTickCnt < (FleCfg->OnMs + FleCfg->KeepOnMs))
  {
    FleCfg->Count = Pulse;
    LedSetDuty(Pulse);
  }
  else if(TmepTickCnt < FleCfg->OffMs)
  {
    FleCfg->Count--;
    LedSetDuty(1*FleCfg->Count);
  }
   else if(TmepTickCnt < (FleCfg->OffMs + FleCfg->KeepOffMs))
     LedSetDuty(0);
  else
  {
    FleCfg->TimeTick = GetSystemTick();
	FleCfg->Count = 0;
  }
}


