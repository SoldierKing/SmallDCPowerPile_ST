#include "ADrv_LET.h"


//函数声明
void SwitchLEDSta(pSwitchOnOffHandler pSwitchOnOff, EnumSwitchStatus Sta);
void LedTwinkle(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg);
void LedTwinkleAlways(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg);
void LedTwinkleCycle(pSwitchOnOffHandler pSwitchOnOff,StructFLECfg *FleCfg);
void LedChargeBreathe(pSetDutyHandler LedSetDuty,StructFLECfg *FleCfg);


/// <summary>
///  开关灯
/// </summary>
/// <param name="Sta">输入想要改变的状态</param>  
/// <param name="pSwitchOnOff">输入想要控制的灯的动作指针</param>  
void SwitchLEDSta(pSwitchOnOffHandler pSwitchOnOff, EnumSwitchStatus Sta)
{
  if (Sta  == eSwSta_On)
    pSwitchOnOff(eSwSta_On);
  else if (Sta == eSwSta_Off)
    pSwitchOnOff(eSwSta_Off);
}

/// <summary>
///  灯闪烁一定次数(灯光效果)
/// </summary>
/// <param name="pSwitchOnOff">输入想要控制的灯的动作指针</param>  
/// <param name=" *FleCfg">灯光效果指针</param>  
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
///  灯光一直闪烁(灯光效果)
/// </summary>
/// <param name="pSwitchOnOff">输入想要控制的灯的动作指针</param>  
/// <param name=" *FleCfg">灯光效果指针</param>  
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
///  灯光闪烁N次后，一定时间后重复闪烁N次，一直循环(灯光效果)
/// </summary>
/// <param name="pSwitchOnOff">输入想要控制的灯的动作指针</param>  
/// <param name=" *FleCfg">灯光效果指针</param>  
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
///  呼吸灯(灯光效果)
/// </summary>
/// <param name="LedSetDuty">输入想要控制的灯的动作指针</param>  
/// <param name=" *FleCfg">灯光效果指针</param>  
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


