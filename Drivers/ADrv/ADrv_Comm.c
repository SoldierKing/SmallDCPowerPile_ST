#include "ADrv_Comm.h"
#include "Thd_Charge.h"

#define RunLed_ON()          HAL_GPIO_WritePin(RunLed_GPIO_Port, RunLed_Pin, GPIO_PIN_RESET);
#define RunLed_OFF()         HAL_GPIO_WritePin(RunLed_GPIO_Port, RunLed_Pin, GPIO_PIN_SET);
#define EleLock_ON()         HAL_GPIO_WritePin(EleLock_GPIO_Port, EleLock_Pin, GPIO_PIN_SET);
#define EleLock_OFF()        HAL_GPIO_WritePin(EleLock_GPIO_Port, EleLock_Pin, GPIO_PIN_RESET);
#define EleUnLock_ON()       HAL_GPIO_WritePin(EleUnlock_GPIO_Port, EleUnlock_Pin, GPIO_PIN_SET);
#define EleUnLock_OFF()      HAL_GPIO_WritePin(EleUnlock_GPIO_Port, EleUnlock_Pin, GPIO_PIN_RESET);
#define EleLock_Open()       EleLock_ON();EleUnLock_OFF();
#define EleLock_Close()      EleLock_OFF();EleUnLock_ON();
#define BoostEN_ON()         HAL_GPIO_WritePin(BoostEN_GPIO_Port, BoostEN_Pin, GPIO_PIN_SET);
#define BoostEN_OFF()        HAL_GPIO_WritePin(BoostEN_GPIO_Port, BoostEN_Pin, GPIO_PIN_RESET);
#define StartOn_ON()         HAL_GPIO_WritePin(StartON_GPIO_Port, StartON_Pin, GPIO_PIN_SET);
#define StartOn_OFF()        HAL_GPIO_WritePin(StartON_GPIO_Port, StartON_Pin, GPIO_PIN_RESET);
#define HRly_ON()            HAL_GPIO_WritePin(Hrly_GPIO_Port, Hrly_Pin, GPIO_PIN_SET);
#define HRly_OFF()           HAL_GPIO_WritePin(Hrly_GPIO_Port, Hrly_Pin, GPIO_PIN_RESET);
#define LRly_ON()            HAL_GPIO_WritePin(Lrly_GPIO_Port, Lrly_Pin, GPIO_PIN_SET);
#define LRly_OFF()           HAL_GPIO_WritePin(Lrly_GPIO_Port, Lrly_Pin, GPIO_PIN_RESET);
#define PERly_ON()           HAL_GPIO_WritePin(PeRly_GPIO_Port, PeRly_Pin, GPIO_PIN_SET);
#define PERly_OFF()          HAL_GPIO_WritePin(PeRly_GPIO_Port, PeRly_Pin, GPIO_PIN_RESET);
#define IMDHRly_ON()         HAL_GPIO_WritePin(IMDHrly_GPIO_Port, IMDHrly_Pin, GPIO_PIN_SET);
#define IMDHRly_OFF()        HAL_GPIO_WritePin(IMDHrly_GPIO_Port, IMDHrly_Pin, GPIO_PIN_RESET);
#define IMDLRly_ON()         HAL_GPIO_WritePin(IMDLrly_GPIO_Port, IMDLrly_Pin, GPIO_PIN_SET);
#define IMDLRly_OFF()        HAL_GPIO_WritePin(IMDLrly_GPIO_Port, IMDLrly_Pin, GPIO_PIN_RESET);

#define BMSPower_ON()        HAL_GPIO_WritePin(BmsON_GPIO_Port, BmsON_Pin, GPIO_PIN_SET);
#define BMSPower_OFF()       HAL_GPIO_WritePin(BmsON_GPIO_Port, BmsON_Pin, GPIO_PIN_RESET);


/// <summary>
///  ��������ָʾ��
/// </summary>
/// <param name="Sta">������Ҫ�ı��״̬</param>  
void SwitcRunLedOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus RunLastSta = eSwSta_Off;
  if (RunLastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    RunLed_ON();
  }
  else if (Sta == eSwSta_Off)
    RunLed_OFF();
  RunLastSta = Sta;
}

/// <summary>
///  ��ȡCC1��⿪�����źţ��ߵ�ƽ==��ǹ���͵�ƽ==δ��ǹ��
/// </summary>
/// <param name=""><param>  
GPIO_PinState GetCC1Sta(void)
{
  return  HAL_GPIO_ReadPin(CC1In_GPIO_Port,CC1In_Pin);
}

/// <summary>
///  �򿪳��ǹ������
/// </summary>
/// <param name="keepOnMs">������Ҫ���ִ򿪵�ʱ��</param>  
static void EleLickOnHandle(u16 KeepOnMs)
{
  while (KeepOnMs--)
  {
    EleLock_Open();
    osDelay(1);
  }
  EleLock_OFF();
  EleUnLock_OFF();
}

/// <summary>
///  �رճ��ǹ������
/// </summary>
/// <param name="keepOffMs">������Ҫ���ֹرյ�ʱ��</param>  
static void EleLickCloseHandle(u16 keepOffMs)
{
  while (keepOffMs--)
  {
    EleLock_Close();
    osDelay(1);
  }
  EleLock_OFF();
  EleUnLock_OFF();
}

/// <summary>
///  ��ȡ������״̬���������߹رգ�
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetEleLockSta(void)
{
  return  HAL_GPIO_ReadPin(LockSW_GPIO_Port,LockSW_Pin);
}



/// <summary>
///  �򿪹رյ���������
/// </summary>
/// <param name="Sta">������Ҫ�ı��״̬</param>  
/// <param name="RecTimes">���򿪹ر�ʧ��ʱ�ظ�����</param> 
void SwitchELeOnOffHandler(EnumSwitchStatus Sta,u8 RecTimes)
{

  if (RecTimes <= 0) return;
  if (Sta == eSwSta_Off)
  {
    EleLickCloseHandle(150);
    while(GetEleLockSta() == GPIO_PIN_RESET && RecTimes > 0)
    {

       EleLickCloseHandle(150);
       RecTimes--;
    }
  }
  else if (Sta == eSwSta_On)
  {
  
    EleLickOnHandle(150);
    while(GetEleLockSta() == GPIO_PIN_SET && RecTimes > 0)
    {

       EleLickOnHandle(150);
       RecTimes--;
    }
  }
}

/// <summary>
///  �򿪵���������
/// </summary>
/// <param name="Sta">������Ҫ�ı��״̬</param>  
static EnumSwitchStatus EleLockLastSta = eSwSta_Off;
void SwitchEleLockOnOff(EnumSwitchStatus Sta)
{
  /*
  static u8 EleHandlerTimes = 5;
  if (EleLockLastSta == Sta && PillarError.Value.EleLock == 0)
    return;
  if (PillarError.Value.EleLock != 0) 
  {
    if (EleHandlerTimes)
      EleHandlerTimes--;
  }
  else EleHandlerTimes = 5;
  if (EleHandlerTimes == 0) return;
  */
  
  if (GetEleLockSta() == GPIO_PIN_SET && 
      EleLockLastSta == eSwSta_Off &&
      Sta == eSwSta_Off)  return;
  
  if (GetEleLockSta() == GPIO_PIN_RESET && 
      EleLockLastSta == eSwSta_On &&
      Sta == eSwSta_On)  return;

  if (Sta  == eSwSta_On)
  {
    SwitchELeOnOffHandler(eSwSta_On,5);
  }
  else if (Sta == eSwSta_Off)
    SwitchELeOnOffHandler(eSwSta_Off,5);
  
  EleLockLastSta = Sta;
  
}


EnumSwitchStatus GetSwitchEleLockSta(void)
{
   return EleLockLastSta;
}

/// <summary>
///  ��ȡ���ǹ�ϰ�ť״̬
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetOBSBtnSta(void)
{
  return  HAL_GPIO_ReadPin(OBSBtn_GPIO_Port,OBSBtn_Pin);
}

/// <summary>
///  ���ؾ�Ե��⸨Դʹ��
/// </summary>
/// <param name="Sta">������Ҫ�ı��״̬</param>  
void SwitcBoostENOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    BoostEN_ON();
  }
  else if (Sta == eSwSta_Off)
    BoostEN_OFF();
  LastSta = Sta;
}




/// <summary>
///  ��ȡIMD��Ե��Դ����״̬״̬
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetBoostENSwitchSta(void)
{
  return  HAL_GPIO_ReadPin(BoostEN_GPIO_Port,BoostEN_Pin);
}


/// <summary>
///  ��������������ع��籣��ʹ��
/// </summary>
/// <param name="Sta">������Ҫ�ı��״̬</param>  
void SwitcStartOnOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    StartOn_ON();
  }
  else if (Sta == eSwSta_Off)
    StartOn_OFF();
  LastSta = Sta;
}

/// <summary>
///  ��ȡ����������ع��籣�����״̬
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetStartOnBtnSta(void)
{
  return  HAL_GPIO_ReadPin(StartON_GPIO_Port,StartON_Pin);
}


/// <summary>
///  ��ȡ����״̬
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetKeyBtnSta(void)
{
  return  HAL_GPIO_ReadPin(KeySta_GPIO_Port,KeySta_Pin);
}

/// <summary>
///  ��ȡ���������µ�ʱ��
/// </summary>
/// <param name="u32">���ذ�������ʱ��MS</param>  
u32 GetKeyKeepTime(void)
{
  static u32 BtnTick = 0;
  if (GetKeyBtnSta() == GPIO_PIN_SET)
  {
    if (BtnTick == 0)
      BtnTick = GetSystemTick();
  }
  else if (GetKeyBtnSta() == GPIO_PIN_RESET)
    BtnTick = 0;
  
  return (BtnTick == 0) ? BtnTick : (GetSystemTick() - BtnTick);
}

/// <summary>
///  ��ȡ����Ƿ��ڳ��״̬
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetVbatChargeSta(void)
{
  return  HAL_GPIO_ReadPin(VbatChg_GPIO_Port,VbatChg_Pin);
}

/// <summary>
///  ��ȡ�Ƿ���©����
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetLeakSta(void)
{
  return  HAL_GPIO_ReadPin(LeakIn_GPIO_Port,LeakIn_Pin);
}

/// <summary>
///  ���������ѹ�̵���
/// </summary>
/// <param name="EnumSwitchStatus">������Ҫ�ı��״̬</param>  
void SwitcHighRlyOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    HRly_ON();
    LRly_ON();
  }
  else if (Sta == eSwSta_Off)
  {
    HRly_OFF();
    LRly_OFF();
  }
  LastSta = Sta;
}

/// <summary>
///  ��ȡ��ѹ�̵�������״̬
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetHRlySta(void)
{
  return  (GPIO_PinState)(HAL_GPIO_ReadPin(Hrly_GPIO_Port,Hrly_Pin) & 
           HAL_GPIO_ReadPin(Lrly_GPIO_Port,Lrly_Pin));
}


/// <summary>
///  ��ȡ����Ƿ񷴽�״̬
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetVbatReverseSta(void)
{
  return  HAL_GPIO_ReadPin(VbatNeg_GPIO_Port,VbatNeg_Pin);
}

/// <summary>
///  ����������Ե���̵���
/// </summary>
/// <param name="EnumSwitchStatus">������Ҫ�ı��״̬</param>  
void SwitcIMDPosRlyOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    IMDHRly_ON();
    PERly_ON();
  }
  else if (Sta == eSwSta_Off)
  {
    IMDHRly_OFF();
    PERly_OFF();
  }
  LastSta = Sta;
}

/// <summary>
///  ���ظ�������Ե���̵���
/// </summary>
/// <param name="EnumSwitchStatus">������Ҫ�ı��״̬</param>  
void SwitcIMDNegRlyOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    IMDLRly_ON();
    PERly_ON();
  }
  else if (Sta == eSwSta_Off)
  {
    IMDLRly_OFF();
    PERly_OFF();
  }
  LastSta = Sta;
}

/// <summary>
///  ����������������Ե���̵���
/// </summary>
/// <param name="EnumSwitchStatus">������Ҫ�ı��״̬</param>  
void SwitcIMDPnRlyOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    IMDLRly_ON();
    IMDHRly_ON();
  }
  else if (Sta == eSwSta_Off)
  {
    IMDLRly_OFF();
    IMDHRly_OFF();
  }
  LastSta = Sta;
}

/// <summary>
///  ����������Ե���̵���
/// </summary>
/// <param name="EnumSwitchStatus">������Ҫ�ı��״̬</param>  
void SwitcIMDPRlyOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    IMDHRly_ON();
  }
  else if (Sta == eSwSta_Off)
  {
    IMDHRly_OFF();
  }
  LastSta = Sta;
}

/// <summary>
///  ����PE��Ե���̵���
/// </summary>
/// <param name="EnumSwitchStatus">������Ҫ�ı��״̬</param>  
void SwitcIMDPERlyOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
     PERly_ON();
  }
  else if (Sta == eSwSta_Off)
  {
     PERly_ON();
  }
  LastSta = Sta;
}

/// <summary>
///  ���ظ�����Ե���̵���
/// </summary>
/// <param name="EnumSwitchStatus">������Ҫ�ı��״̬</param>  
void SwitcIMDNRlyOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    IMDLRly_ON();

  }
  else if (Sta == eSwSta_Off)
  {
    IMDLRly_OFF();
  }
  LastSta = Sta;
}

/// <summary>
///  ��ȡIMD ��Ե��������̵���״̬
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetIMDHPSta(void)
{
  return  HAL_GPIO_ReadPin(IMDHrly_GPIO_Port,IMDHrly_Pin);
}

/// <summary>
///  ��ȡIMD ��Ե��⸺���̵���״̬
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetIMDHNSta(void)
{
  return  HAL_GPIO_ReadPin(IMDLrly_GPIO_Port,IMDLrly_Pin);
}

/// <summary>
///  ��ȡIMD ��Ե���PE�̵���״̬
/// </summary>
/// <param name="void"></param>  
GPIO_PinState GetIMDHPESta(void)
{
  return  HAL_GPIO_ReadPin(PeRly_GPIO_Port,PeRly_Pin);
}

/// <summary>
///  ����BMS��Դʹ��
/// </summary>
/// <param name="EnumSwitchStatus">������Ҫ�ı��״̬</param>  
void SwitcBMSPowerENOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    BMSPower_ON();
  }
  else if (Sta == eSwSta_Off)
  {
    BMSPower_OFF();
  }
  LastSta = Sta;
}



