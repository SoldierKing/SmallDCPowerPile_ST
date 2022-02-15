
#include "Thd_Mult.h"
#include "ADrv_FDC.h"
#include "ADrv_Comm.h"
#include "Param_UserConfig.h"
#include "ADrv_OneKeyStart.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_AdcVolt.h"
#include "Thd_Charge.h"


typedef BitAction  (*pGetFaultByBit)(void);
typedef u32        (*pGetFaultByValue)(void);

/*
/// <summary>
///  获取电子锁是否异常
/// </summary>
/// <param name="BitAction">返回Bit_SET---有故障发生，Bit_RESET----无故障</param>  
BitAction  GetEleLockFault(void)
{

  BitAction Ret = Bit_RESET;
  
  if (GetEleLockOnOffSta() == eSwSta_Off)
  {
  
    if (GetEleLockSta() == GPIO_PIN_RESET)
      Ret =  Bit_SET;
    else
      Ret = Bit_RESET;
  }
  else
  {
  
    if (GetEleLockSta() == GPIO_PIN_SET)
      Ret = Bit_SET;
    else
      Ret = Bit_RESET;
  
  }
  return Ret;
}

/// <summary>
///  获取继电器是否异常(粘连异常)
/// </summary>
/// <param name="BitAction">返回Bit_SET---有故障发生，Bit_RESET----无故障</param>  
BitAction GetHRlyFault(void)
{

  BitAction Ret = Bit_RESET;
  
  
  if (GetHRlyOnOffSta() == GPIO_PIN_RESET && GetLRlyOnOffSta() == GPIO_PIN_SET)
  {
  
    if (GetWorkMode() == Enum_ChargeWork)
    {
      if (ChargeCtrl.CurProcess == eChgPro_ImdCheck)
      if (AdcVoltBuf[EnumVddc_AD3] >= AdcVoltBuf[EnumVhdc_AD2] *0.7)
        Ret =  Bit_SET;
    }
    else if (GetWorkMode() == Enum_DischargeWork)
    {
    
      if (AdcVoltBuf[EnumVddc_AD3]*0.7 <= AdcVoltBuf[EnumVhdc_AD2] )
        Ret =  Bit_SET;
    }
  }
  else if (GetHRlyOnOffSta() == GPIO_PIN_SET && GetLRlyOnOffSta() == GPIO_PIN_RESET)
  {
  
    if (GetWorkMode() == Enum_ChargeWork)
    {
      if (ChargeCtrl.CurProcess == eChgPro_ImdCheck)
      if (AdcVoltBuf[EnumVddc_AD3] >= AdcVoltBuf[EnumVhdc_AD2] *0.7)
        Ret =  Bit_SET;
    }
    else if (GetWorkMode() == Enum_DischargeWork)
    {
    
      if (AdcVoltBuf[EnumVddc_AD3]*0.7 <= AdcVoltBuf[EnumVhdc_AD2])
        Ret =  Bit_SET;
    }
  }
  
  return Ret;
}


/// <summary>
///  获取车端电池反接是否异常
/// </summary>
/// <param name="BitAction">返回Bit_SET---有故障发生，Bit_RESET----无故障</param>  
BitAction  GetBatReverseFault(void)
{

  return (BitAction)GetVbatReverseSta();
}

/// <summary>
///  获取漏电流故障
/// </summary>
/// <param name="BitAction">返回Bit_SET---有故障发生，Bit_RESET----无故障</param>  
BitAction  GetLeakFault(void)
{

  return (BitAction)GetLeakSta();
}

/// <summary>
///  开关机按键故障检测
/// </summary>
/// <param name="BitAction">返回Bit_SET---有故障发生，Bit_RESET----无故障</param>  
BitAction  GetTurnOnOffKeyFault(void)
{

  return (KeyPressCfg.KeyDownTime > 5*60*1000 ? Bit_SET:Bit_RESET);
}

/// <summary>
///  充电枪上一建停机按键故障检测
/// </summary>
/// <param name="BitAction">返回Bit_SET---有故障发生，Bit_RESET----无故障</param>  
BitAction  GetStopKeyFault(void)
{

  return (StopBtnPressCfg.KeyDownTime > 5*60*1000 ? Bit_SET:Bit_RESET);
}

/// <summary>
///  是否有DC模块检测
/// </summary>
/// <param name="BitAction">返回Bit_SET---有故障发生，Bit_RESET----无故障</param>  
BitAction  GetDCModuleFault(void)
{

  return (0 == DcModuleManage.OnlineFlag ? Bit_SET:Bit_RESET);
}

/// <summary>
///  检测CAN1是否有故障
/// </summary>
/// <param name="BitAction">返回Bit_SET---有故障发生，Bit_RESET----无故障</param>  
BitAction  GetCan1Fault(void)
{

  return (0 != HcanDrv.Can1ErrCount ? Bit_SET:Bit_RESET);
}

/// <summary>
///  检测CAN2是否有故障
/// </summary>
/// <param name="BitAction">返回Bit_SET---有故障发生，Bit_RESET----无故障</param>  
BitAction  GetCan2Fault(void)
{

  return (0 != HcanDrv.Can2ErrCount ? Bit_SET:Bit_RESET);
}

*/
/// <summary>
///  故障检测函数
/// </summary>
/// <param name="StructFaultDetCfg* TempComDetCfg">故障检测配置信息</param>  
/// <param name="pGetFaultByBit pFBB">位触发故障函数指针</param>  
/// <param name="pGetFaultByValue pFBV">检测值触发故障函数指针</param> 
int FDC(StructFaultDetCfg* TempComDetCfg,
              pGetFaultByBit pFBB,pGetFaultByValue pFBV)
{

  if (TempComDetCfg == NULL) return -1;
  if (pFBB == NULL && pFBV == NULL) return -2;
  if (pFBB != NULL && pFBV!= NULL) return -3;
  
  if (pFBB != NULL && pFBV == NULL)
    TempComDetCfg->BitVaule = pFBB();
  else 
    TempComDetCfg->FaultVaule = pFBV();
  FaultDetct(TempComDetCfg);
  

  return TempComDetCfg->Erro;
}

/*
/// <summary>
///  检测SPIFLASH是否有故障
/// </summary>
/// <param name="BitAction">返回Bit_SET---有故障发生，Bit_RESET----无故障</param>  
BitAction  GetSpiFlashFault(void)
{

  return (0 != SpiFlashError ? Bit_SET:Bit_RESET);
}

*/

u32 GetACInputVolt(void)
{
  return (u32)GetDcModuleVoltA();
}

void Thd_FDCMonitor(void const *argument)
{
  
  while (TRUE)
  {
   
    int Ret = -1; 
    
    //充电中AC侧输入电压掉电故障检测
    Ret = FDC(&FltACVoltDownErro,NULL,GetACInputVolt);
    if (Ret == 1 && FltACVoltDownErro.ErrCount >= 1)
      PillarError.Value.ACInputOut = 1;
    else if (Ret == 0)
    {
      FltACVoltDownErro.ErrCount = 0;
      PillarError.Value.ACInputOut = 0;
    }
    /*
    //电子锁故障检测
    Ret = FDC(&FltEleLockErro,GetEleLockFault,NULL);
    if (Ret == 1 && FltEleLockErro.ErrCount >= 1)
      PillarError.Value.EleLock = 1;
    else if (Ret == 0)
    {
      FltEleLockErro.ErrCount = 0;
      PillarError.Value.EleLock = 0;
    }
    */
    
    /*
    //高压继电器粘连故障检测
    Ret = FDC(&FltRlyAdhesionErro,GetHRlyFault,NULL);
    if (Ret == 1 && FltRlyAdhesionErro.ErrCount >= 1)
      PillarError.Value.RlyAdhesion = 1;
    else if (Ret == 0)
    {
      FltRlyAdhesionErro.ErrCount = 0;
      PillarError.Value.RlyAdhesion = 0;
    }
    */
    
    /*
    //车端电池反接故障检测
    Ret = FDC(&FltBatReverseErro,GetBatReverseFault,NULL);
    if (Ret == 1 && FltBatReverseErro.ErrCount >= 1)
      PillarError.Value.BatReverse = 1;
    else if (Ret == 0)
    {
      FltBatReverseErro.ErrCount = 0;
      PillarError.Value.BatReverse = 0;
    }
    */
    
    /*
    //漏电流故障检测
    Ret = FDC(&FltLeakErro,GetLeakFault,NULL);
    if (Ret == 1 && FltLeakErro.ErrCount >= 1)
      PillarError.Value.Leak = 1;
    else if (Ret == 0)
    {
      FltLeakErro.ErrCount = 0;
      PillarError.Value.Leak = 0;
    }
    
    //开机按键故障检测
    Ret = FDC(&FltTurnOnOffKeyErro,GetTurnOnOffKeyFault,NULL);
    if (Ret == 1 && FltTurnOnOffKeyErro.ErrCount >= 1)
      PillarError.Value.TurnOnOffKey = 1;
    else if (Ret == 0)
    {
      FltTurnOnOffKeyErro.ErrCount = 0;
      PillarError.Value.TurnOnOffKey = 0;
    }
    
    //一建停机按键故障检测
    Ret = FDC(&FltStopKeyErro,GetStopKeyFault,NULL);
    if (Ret == 1 && FltStopKeyErro.ErrCount >= 1)
      PillarError.Value.StopKey = 1;
    else if (Ret == 0)
    {
      FltStopKeyErro.ErrCount = 0;
      PillarError.Value.StopKey = 0;
    }
    
    //检测是否有DC模块故障检测
    if ((GetWorkMode() == Enum_DischargeWork && ChargeCtrl.CurProcess == eChgPro_DisChgRun)
        || GetWorkMode() == Enum_ChargeWork)
    {
      Ret = FDC(&FltDcModuleErro,GetDCModuleFault,NULL);
      if (Ret == 1 && FltDcModuleErro.ErrCount >= 1)
        PillarError.Value.NoDcmod = 1;
      else if (Ret == 0)
      {
        FltDcModuleErro.ErrCount = 0;
        PillarError.Value.NoDcmod = 0;
      }
    }
    */
    
    /*
    //Can1故障检测
    Ret = FDC(&FltCan1Erro,GetCan1Fault,NULL);
    if (Ret == 1 && FltCan1Erro.ErrCount >= 1)
      PillarError.Value.Can1Err = 1;
    else if (Ret == 0)
    {
      FltCan1Erro.ErrCount = 0;
      PillarError.Value.Can1Err = 0;
    }
    
    //Can2故障检测
    Ret = FDC(&FltCan2Erro,GetCan2Fault,NULL);
    if (Ret == 1 && FltCan2Erro.ErrCount >= 1)
      PillarError.Value.Can2Err = 1;
    else if (Ret == 0)
    {
      FltCan2Erro.ErrCount = 0;
      PillarError.Value.Can2Err = 0;
    }
    */
    
    /*
    //SPIFlash故障检测
    Ret = FDC(&FltSpiFlashErro,GetSpiFlashFault,NULL);
    if (Ret == 1 && FltSpiFlashErro.ErrCount >= 1)
      PillarError.Value.SpiFlash = 1;
    else if (Ret == 0)
    {
      FltSpiFlashErro.ErrCount = 0;
      PillarError.Value.SpiFlash = 0;
    }
    */     
    osDelay(5);
  }
  
}






