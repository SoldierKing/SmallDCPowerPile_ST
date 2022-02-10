#include "ADrv_LampSignals.h"
#include "Thd_Charge.h"
#include "Drv_WS2812B.h"
#include "Param_UserConfig.h"


/// <summary>
///  开关蓝灯
/// </summary>
/// <param name="Sta">输入想要改变的状态</param>  
void SwitchBlueLED(EnumSwitchStatus Sta)
{
  
  if (Sta  == eSwSta_On)
    WS2812B_Blue();
  else if (Sta == eSwSta_Off)
    WS2812B_Black();
}

/// <summary>
///  开关白灯
/// </summary>
/// <param name="Sta">输入想要改变的状态</param>  
void SwitchWitheLED(EnumSwitchStatus Sta)
{
  
  if (Sta  == eSwSta_On)
    WS2812B_White();
  else if (Sta == eSwSta_Off)
    WS2812B_Black();
}

/// <summary>
///  开关绿灯
/// </summary>
/// <param name="Sta">输入想要改变的状态</param>  
void SwitchGreenLED(EnumSwitchStatus Sta)
{
  
  if (Sta  == eSwSta_On)
    WS2812B_Green();
  else if (Sta == eSwSta_Off)
    WS2812B_Black();
}



void Thd_LampSignals(void)
{

  if (KeyPressCfg.OverLongPressTime == 0)
  {
  
  switch (ChargeCtrl.CurProcess)
  {
    
    //放电线程
    case eChgPro_DisChgIdle:
      if (PillarError.Mult == 0 && ChargeError.Mult == 0)
        //WS2812B_White();
        LedWs2812Breathe(WS2812B_Lighting,&BreatheWhiteCfg);
      else
        WS2812B_Red();
      break;
    case eChgPro_DisShakeHand:

    case eChgPro_DisChgRecognizeEx:

    case eChgPro_DisChgRecognize:
    case eChgPro_DisChgConfigure:
    case eChgPro_DisWaitBmsReady:
    case eChgPro_DisWaitPillarReady:
    case eChgPro_DisChgStart:
    //case eChgPro_DisWaitVoltChgRun:   
    case eChgPro_DisCommBreak:
      if (PillarError.Mult == 0 && ChargeError.Mult == 0)
        //LedTwinkleAlways(SwitchWitheLED,&TwinkQuickBlue);
        LedWs2812Breathe(WS2812B_Lighting,&BreatheWhiteCfg);
      else
        WS2812B_Red();
      break;
    case eChgPro_DisChgRun:
      if (PillarError.Mult == 0 && ChargeError.Mult == 0)
        //LedTwinkleAlways(SwitchWitheLED,&TwinkQuickBlue);
        LedWs2812Breathe(WS2812B_Lighting,&BreatheWhiteCfg);
      else
        WS2812B_Red();
      break;

    case eChgPro_DisChgStop:
      if (PillarError.Mult == 0 && ChargeError.Mult == 0)
      {
        
        //WS2812B_White();
        LedTwinkleAlways(SwitchWitheLED,&TwinkQuickBlue);
      }
      else
        WS2812B_Red();
      break;
      
    case eChgPro_DisChgBreak:
    case eChgPro_DisCommError:
      WS2812B_Red();
      break;
  
    //充电线程
    case eChgPro_ChgIdle:
      if (PillarError.Mult == 0 && ChargeError.Mult == 0)
        WS2812B_Blue();
      else
        WS2812B_Red();
      break;
    case eChgPro_ShakeHand:

    case eChgPro_ImdCheck:
    case eChgPro_ChgRecognizeEx:

    case eChgPro_ChgRecognize:
    case eChgPro_ChgConfigure:
    case eChgPro_WaitBmsReady:
    case eChgPro_WaitPillarReady:
    case eChgPro_ChgStart:
    //case eChgPro_WaitVoltChgRun:
    case eChgPro_CommBreak:
      if (PillarError.Mult == 0 && ChargeError.Mult == 0)
        LedTwinkleAlways(SwitchBlueLED,&TwinkQuickBlue);
      else
        WS2812B_Red();
      break;
      
    case eChgPro_ChgRun:
      if (PillarError.Mult == 0 && ChargeError.Mult == 0)
        //LedEffectSwitch(Breathing,Blue);
        LedWs2812Breathe(WS2812B_Lighting,&BreatheBlueCfg);
      else
        WS2812B_Red();
      break;

    case eChgPro_ChgStop:
      if (PillarError.Mult == 0 && ChargeError.Mult == 0)
      {
        
        if (ChargeStopType != eChgStop_Run_Soc98)
          WS2812B_Blue();
		else
		  WS2812B_Green();
      }
      else
        WS2812B_Red();
      break;
    case eChgPro_ChgBreak:
    case eChgPro_CommError:
      WS2812B_Red();
     break;
      
  }
  }
  else 
      WS2812B_Black();
}

