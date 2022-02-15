
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
///  ��ȡ�������Ƿ��쳣
/// </summary>
/// <param name="BitAction">����Bit_SET---�й��Ϸ�����Bit_RESET----�޹���</param>  
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
///  ��ȡ�̵����Ƿ��쳣(ճ���쳣)
/// </summary>
/// <param name="BitAction">����Bit_SET---�й��Ϸ�����Bit_RESET----�޹���</param>  
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
///  ��ȡ���˵�ط����Ƿ��쳣
/// </summary>
/// <param name="BitAction">����Bit_SET---�й��Ϸ�����Bit_RESET----�޹���</param>  
BitAction  GetBatReverseFault(void)
{

  return (BitAction)GetVbatReverseSta();
}

/// <summary>
///  ��ȡ©��������
/// </summary>
/// <param name="BitAction">����Bit_SET---�й��Ϸ�����Bit_RESET----�޹���</param>  
BitAction  GetLeakFault(void)
{

  return (BitAction)GetLeakSta();
}

/// <summary>
///  ���ػ��������ϼ��
/// </summary>
/// <param name="BitAction">����Bit_SET---�й��Ϸ�����Bit_RESET----�޹���</param>  
BitAction  GetTurnOnOffKeyFault(void)
{

  return (KeyPressCfg.KeyDownTime > 5*60*1000 ? Bit_SET:Bit_RESET);
}

/// <summary>
///  ���ǹ��һ��ͣ���������ϼ��
/// </summary>
/// <param name="BitAction">����Bit_SET---�й��Ϸ�����Bit_RESET----�޹���</param>  
BitAction  GetStopKeyFault(void)
{

  return (StopBtnPressCfg.KeyDownTime > 5*60*1000 ? Bit_SET:Bit_RESET);
}

/// <summary>
///  �Ƿ���DCģ����
/// </summary>
/// <param name="BitAction">����Bit_SET---�й��Ϸ�����Bit_RESET----�޹���</param>  
BitAction  GetDCModuleFault(void)
{

  return (0 == DcModuleManage.OnlineFlag ? Bit_SET:Bit_RESET);
}

/// <summary>
///  ���CAN1�Ƿ��й���
/// </summary>
/// <param name="BitAction">����Bit_SET---�й��Ϸ�����Bit_RESET----�޹���</param>  
BitAction  GetCan1Fault(void)
{

  return (0 != HcanDrv.Can1ErrCount ? Bit_SET:Bit_RESET);
}

/// <summary>
///  ���CAN2�Ƿ��й���
/// </summary>
/// <param name="BitAction">����Bit_SET---�й��Ϸ�����Bit_RESET----�޹���</param>  
BitAction  GetCan2Fault(void)
{

  return (0 != HcanDrv.Can2ErrCount ? Bit_SET:Bit_RESET);
}

*/
/// <summary>
///  ���ϼ�⺯��
/// </summary>
/// <param name="StructFaultDetCfg* TempComDetCfg">���ϼ��������Ϣ</param>  
/// <param name="pGetFaultByBit pFBB">λ�������Ϻ���ָ��</param>  
/// <param name="pGetFaultByValue pFBV">���ֵ�������Ϻ���ָ��</param> 
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
///  ���SPIFLASH�Ƿ��й���
/// </summary>
/// <param name="BitAction">����Bit_SET---�й��Ϸ�����Bit_RESET----�޹���</param>  
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
    
    //�����AC�������ѹ������ϼ��
    Ret = FDC(&FltACVoltDownErro,NULL,GetACInputVolt);
    if (Ret == 1 && FltACVoltDownErro.ErrCount >= 1)
      PillarError.Value.ACInputOut = 1;
    else if (Ret == 0)
    {
      FltACVoltDownErro.ErrCount = 0;
      PillarError.Value.ACInputOut = 0;
    }
    /*
    //���������ϼ��
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
    //��ѹ�̵���ճ�����ϼ��
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
    //���˵�ط��ӹ��ϼ��
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
    //©�������ϼ��
    Ret = FDC(&FltLeakErro,GetLeakFault,NULL);
    if (Ret == 1 && FltLeakErro.ErrCount >= 1)
      PillarError.Value.Leak = 1;
    else if (Ret == 0)
    {
      FltLeakErro.ErrCount = 0;
      PillarError.Value.Leak = 0;
    }
    
    //�����������ϼ��
    Ret = FDC(&FltTurnOnOffKeyErro,GetTurnOnOffKeyFault,NULL);
    if (Ret == 1 && FltTurnOnOffKeyErro.ErrCount >= 1)
      PillarError.Value.TurnOnOffKey = 1;
    else if (Ret == 0)
    {
      FltTurnOnOffKeyErro.ErrCount = 0;
      PillarError.Value.TurnOnOffKey = 0;
    }
    
    //һ��ͣ���������ϼ��
    Ret = FDC(&FltStopKeyErro,GetStopKeyFault,NULL);
    if (Ret == 1 && FltStopKeyErro.ErrCount >= 1)
      PillarError.Value.StopKey = 1;
    else if (Ret == 0)
    {
      FltStopKeyErro.ErrCount = 0;
      PillarError.Value.StopKey = 0;
    }
    
    //����Ƿ���DCģ����ϼ��
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
    //Can1���ϼ��
    Ret = FDC(&FltCan1Erro,GetCan1Fault,NULL);
    if (Ret == 1 && FltCan1Erro.ErrCount >= 1)
      PillarError.Value.Can1Err = 1;
    else if (Ret == 0)
    {
      FltCan1Erro.ErrCount = 0;
      PillarError.Value.Can1Err = 0;
    }
    
    //Can2���ϼ��
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
    //SPIFlash���ϼ��
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






