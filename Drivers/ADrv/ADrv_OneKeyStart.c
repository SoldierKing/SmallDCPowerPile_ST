#include "ADrv_OneKeyStart.h"
#include "Param_UserConfig.h"
#include "Thd_Charge.h"
#include "ADrv_Comm.h"
#include "Thd_Charge.h"
#include "ADrv_AdcVolt.h"
#include "Drv_WS2812B.h"
#include "ADrv_LET.h"
#include "ADrv_LampSignals.h"
#include "Param_UserConfig.h"
#include "ADrv_Comm.h"




/// <summary>
///  工作模式切换
/// </summary>
/// <param name="EnumWorkMode Mode">输入想要的工作模式</param>  
void SwitchWorkMode(EnumWorkMode Mode)
{

  if (WorkModeCfg.StateLock == ENABLE || WorkModeCfg.SwitchModeEN == DISABLE) return;
  if (WorkModeCfg.WorkMode == Mode) return;
  if (Mode  == Enum_ChargeWork)
  {
    WorkModeCfg.WorkMode = Enum_ChargeWork;
  }
  else if (Mode == Enum_DischargeWork)
  {
    WorkModeCfg.WorkMode = Enum_DischargeWork;
  }
}

/// <summary>
///  获取当前的工作模式
/// </summary>
/// <param name="void"></param>  
EnumWorkMode GetWorkMode(void)
{
   
  return WorkModeCfg.WorkMode;
}


/// <summary>
///  一建停机,按键检测
/// </summary>
/// <param name="void"></param>  
void OneKeyStop(void)
{

  static u32 StopTick = 0;
  StopBtnPressCfg.IOSta = GetOBSBtnSta();
  
  if (GetKeyPressType(&StopBtnPressCfg) == Enum_KeyShortPress
       || GetKeyPressType(&StopBtnPressCfg)  == Enum_KeyLongPress)
  {
    
     ChargeEnable = 0;
     DisChargeEnable = 0;
     //ChargeStopType = eChgStop_Stop_ByOneKeyBtn;
     //DisChargeStopType = eChgStop_Stop_ByOneKeyBtn;
     if (StopTick == 0)
       StopTick = GetSystemTick();
  }
  
  if (StopTick != 0 && GetSystemTick() - StopTick > 3500)
  {
    
    StopTick = 0;
    if (GetEleLockSta() == GPIO_PIN_RESET)
      SwitchELeOnOffHandler(eSwSta_Off,5);
  }

}

#define   VbatLimit    (3.7*10)

/// <summary>
///  超级电容电量显示，通过LED灯指示
/// </summary>
/// <param name="void"></param>  
void BatPowerShowByLed(void)
{

  //KeyPressCfg.IOSta = GetKeyBtnSta();  
  //GetKeyPressType(&KeyPressCfg);  
  if (KeyPressCfg.KeyDownTime != 0)
  {

	//电池电压低于最低开机值
    if (AdcVoltBuf[EnumVbat_VOL] < VbatLimit)
		LedTwinkleAlways(SwitchGreenLED,&TwinkQuickBatLighting);  
	else
		SwitchGreenLED(eSwSta_On);
     
  }
  else
  	WS2812B_Black();
}


/// <summary>
///  开关机按键，短按启动充放电
/// </summary>
/// <param name="void"></param>  
void StartDisOrChargeKey(void)
{

  //KeyPressCfg.IOSta = GetKeyBtnSta();
  if (GetKeyPressType(&KeyPressCfg) == Enum_KeyShortPress)
  {
    
    if (GetWorkMode() == Enum_ChargeWork
        && ChargeCtrl.CurProcess == eChgPro_ChgIdle)
      ChargeEnable = 1;
    else if (GetWorkMode() == Enum_DischargeWork 
             && ChargeCtrl.CurProcess == eChgPro_DisChgIdle)
      DisChargeEnable = 1;
     
  }
}


/// <summary>
///  长按启动键关机
/// </summary>
/// <param name="void"></param>  
void StartKeyTurnOff(void)
{
  
  if (ChargeCtrl.CurProcess != eChgPro_Sleep
      && ChargeCtrl.CurProcess != eChgPro_TurnOn
      && ChargeCtrl.CurProcess != eChgPro_TurnOff)
  {
     //KeyPressCfg.IOSta = GetKeyBtnSta();
     GetKeyKeepDownTime(&KeyPressCfg);
  
    if (KeyPressCfg.KeyDownTime >= KeyPressCfg.KeyLongPressTime)
    {
    
      //关机灯效
      WS2812B_Black();
      //准备进入关机流程标志位
      KeyPressCfg.OverLongPressTime = 1;
      
      //按键释放后进入关机流程
      if (KeyPressCfg.IsUp == KeyPressCfg.IOSta)
      {
        ChargeEnable = 0;
        DisChargeEnable = 0;
        ChargeStopType = eChgStop_Stop_ByKeyTurnOff;
        DisChargeStopType = eChgStop_Stop_ByKeyTurnOff;
      }
     
    }
  }
}

/// <summary>
///  在放电流程中接入市电；在充电中断开市电；这应属于故障；
/// </summary>
/// <param name="void"></param>  
void ChargeACOffFault(void)
{
   static u32 VinLowTick = 0;
   if (ChargeCtrl.CurProcess == eChgPro_Sleep) return;
   
   if (GetWorkMode() == Enum_ChargeWork)
   {

	 if (GetVinValue() < VinInVolt_Vaild)
	 {

	   if (VinLowTick == 0) VinLowTick = GetSystemTick();
	 }
	 else VinLowTick = 0;
	 
     if ((VinLowTick != 0 && (GetSystemTick() - VinLowTick > SleepTimeOut)) || GetVinValue() < 600)
     {
        //VinLowTick = 0;
  	   ChargeCtrl.CurProcess = eChgPro_TurnOff;
       ChargeCtrl.Param = 0;
	   //NVIC_SystemReset();
     }
   }


}



/// <summary>
///  模式自动切换
/// </summary>
/// <param name="void"></param>  

void SwitchModeAuto(void)
{

  if (ChargeCtrl.CurProcess != eChgPro_Sleep
      && ChargeCtrl.CurProcess != eChgPro_TurnOn
      && ChargeCtrl.CurProcess != eChgPro_TurnOff)
  {
  
    if (GetWorkMode() == Enum_ChargeWork)
    {
    
       if (GetHRlySta() == GPIO_PIN_RESET && GetBoostENSwitchSta() == GPIO_PIN_RESET)
       {
         if (ChargeCtrl.CurProcess == eChgPro_ChgIdle && GetVinValue() < VinInVolt_Vaild)
         {
           ChargeCtrl.CurProcess = eChgPro_DisChgIdle;
           ChargeCtrl.Param = 0;
           SwitchWorkMode(Enum_DischargeWork);
         }
       }
    }
    else if (GetWorkMode() == Enum_DischargeWork)
    {
    
      if (GetHRlySta() == GPIO_PIN_RESET && GetBoostENSwitchSta() == GPIO_PIN_RESET)
       {
         if (ChargeCtrl.CurProcess == eChgPro_DisChgIdle && GetVinValue() >= VinInVolt_Vaild)
         {
           
           ChargeCtrl.CurProcess = eChgPro_ChgIdle;
           ChargeCtrl.Param = 0;
           SwitchWorkMode(Enum_ChargeWork);
         }
       }
    }
  }
}





