#include "Param_UserConfig.h"




//LED ����(��250MS����250MS),һֱѭ��(���е�������Ϣ)
StructFLECfg TwinkQuick =
{
  
  .Count = 0,
  .Shans = 0,
  .KeepOnMs = 250,
  .KeepOffMs = 250,
  .GapTimeMs = 0,
  .TimeTick = 0,
  .Tick = 0,
  .Sta = 0,
  .OnMs = 0,
  .OffMs = 0,
};

//LED ����(��250MS����250MS),һֱѭ��(��ص�ѹָʾ��Ч��)
StructFLECfg TwinkQuickBatLighting =
{
  
  .Count = 0,
  .Shans = 0,
  .KeepOnMs = 250,
  .KeepOffMs = 250,
  .GapTimeMs = 0,
  .TimeTick = 0,
  .Tick = 0,
  .Sta = 0,
  .OnMs = 0,
  .OffMs = 0,
};


//��ɫ�� ����(��5000MS����500MS),һֱѭ��(���е�������Ϣ)
StructFLECfg TwinkQuickBlue =
{
  
  .Count = 0,
  .Shans = 0,
  .KeepOnMs = 500,
  .KeepOffMs = 500,
  .GapTimeMs = 0,
  .TimeTick = 0,
  .Tick = 0,
  .Sta = 0,
  .OnMs = 0,
  .OffMs = 0,
};

//�����������������̰���Ϣ
StructKeyPressCfg KeyPressCfg = 
{

  .IsDown = GPIO_PIN_SET,
  .IsUp = GPIO_PIN_RESET,
  .KeyShortPressTime = 100,
  .KeyLongPressTime = 3000,
  .BtnTick = 0,
  
};

//����һ��ͣ�����������������̰���Ϣ
StructKeyPressCfg StopBtnPressCfg = 
{

  .IsDown = GPIO_PIN_RESET,
  .IsUp = GPIO_PIN_SET,
  .KeyShortPressTime = 100,
  .KeyLongPressTime = 3000,
  .BtnTick = 0,
  
};

//����ģʽ�л�����
StructWorkModeCfg  WorkModeCfg = 
{

  //ģʽ״̬��,��ס��ʱ���޷��л�״̬
  .StateLock = DISABLE,
  //ģʽ�л�ʹ��
  .SwitchModeEN = ENABLE,
  //����ģʽ
  .WorkMode = Enum_ChargeWork,
  
};


//�������е����������
StructFaultDetCfg FltACVoltDownErro =
{
  .FaultEn = Enable,
  .FaultRecEn = Enable,
  .RecLock = UnLock,
  .DetType = DetByAdcValue,
  .DetBasisType = ByTimer,
  .Timer.FaultContinueTime = 1500,
  .Timer.FaultRecTime = 1500,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 10,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_SET,
  .Bit.RecBit = Bit_RESET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 1700,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 2000,
  .AdcValue.DetType = LowLimit,
  .Erro = 0,
  .ErrCount = 0,
};



//�����������������ݽṹ
StructFaultDetCfg FltEleLockErro =
{
  .FaultEn = Enable,
  .FaultRecEn = Enable,
  .RecLock = UnLock,
  .DetType = DetByBitAction,
  .DetBasisType = ByTimer,
  .Timer.FaultContinueTime = 5000,
  .Timer.FaultRecTime = 1500,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 10,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_SET,
  .Bit.RecBit = Bit_RESET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 0,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 0,
  .AdcValue.DetType = OverLimit,
  .Erro = 0,
  .ErrCount = 0,
};


//��ѹ�̵��������������ݽṹ
StructFaultDetCfg FltRlyAdhesionErro =
{
  .FaultEn = Enable,
  .FaultRecEn = Disable,
  .RecLock = Lock,
  .DetType = DetByBitAction,
  .DetBasisType = ByTimer,
  .Timer.FaultContinueTime = 80,
  .Timer.FaultRecTime = 80,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 10,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_SET,
  .Bit.RecBit = Bit_RESET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 0,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 0,
  .AdcValue.DetType = OverLimit,
  .Erro = 0,
  .ErrCount = 0,
};


//���˵�ѹ���ӹ����������ݽṹ
StructFaultDetCfg FltBatReverseErro =
{
  .FaultEn = Enable,
  .FaultRecEn = Enable,
  .RecLock = UnLock,
  .DetType = DetByBitAction,
  .DetBasisType = ByTimer,
  .Timer.FaultContinueTime = 100,
  .Timer.FaultRecTime = 100,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 10,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_RESET,
  .Bit.RecBit = Bit_SET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 0,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 0,
  .AdcValue.DetType = OverLimit,
  .Erro = 0,
  .ErrCount = 0,
  
};


//©���������������ݽṹ
StructFaultDetCfg FltLeakErro =
{
  .FaultEn = Enable,
  .FaultRecEn = Disable,
  .RecLock = Lock,
  .DetType = DetByBitAction,
  .DetBasisType = ByTimer,
  .Timer.FaultContinueTime = 150,
  .Timer.FaultRecTime = 150,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 10,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_RESET,
  .Bit.RecBit = Bit_SET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 0,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 0,
  .AdcValue.DetType = OverLimit,
  .Erro = 0,
  .ErrCount = 0,
  
};


//���ػ����������������ݽṹ
StructFaultDetCfg FltTurnOnOffKeyErro =
{
  .FaultEn = Enable,
  .FaultRecEn = Enable,
  .RecLock = UnLock,
  .DetType = DetByBitAction,
  .DetBasisType = ByTimer,
  .Timer.FaultContinueTime = 150,
  .Timer.FaultRecTime = 150,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 10,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_SET,
  .Bit.RecBit = Bit_RESET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 0,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 0,
  .AdcValue.DetType = OverLimit,
  .Erro = 0,
  .ErrCount = 0,
  
};


//һ���ػ����������������ݽṹ
StructFaultDetCfg FltStopKeyErro =
{
  .FaultEn = Enable,
  .FaultRecEn = Enable,
  .RecLock = UnLock,
  .DetType = DetByBitAction,
  .DetBasisType = ByTimer,
  .Timer.FaultContinueTime = 150,
  .Timer.FaultRecTime = 150,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 10,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_SET,
  .Bit.RecBit = Bit_RESET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 0,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 0,
  .AdcValue.DetType = OverLimit,
  .Erro = 0,
  .ErrCount = 0,
  
};


//����Ƿ���DCģ���������ݽṹ
StructFaultDetCfg FltDcModuleErro =
{
  .FaultEn = Enable,
  .FaultRecEn = Enable,
  .RecLock = UnLock,
  .DetType = DetByBitAction,
  .DetBasisType = ByTimer,
  .Timer.FaultContinueTime = 3000,
  .Timer.FaultRecTime = 1000,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 10,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_SET,
  .Bit.RecBit = Bit_RESET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 0,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 0,
  .AdcValue.DetType = OverLimit,
  .Erro = 0,
  .ErrCount = 0,
  
};
  
  
//CAN1���ϼ���������ݽṹ
StructFaultDetCfg FltCan1Erro =
{
  .FaultEn = Enable,
  .FaultRecEn = Enable,
  .RecLock = UnLock,
  .DetType = DetByBitAction,
  .DetBasisType = ByTimer,
  .Timer.FaultContinueTime = 1000,
  .Timer.FaultRecTime = 1000,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 10,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_SET,
  .Bit.RecBit = Bit_RESET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 0,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 0,
  .AdcValue.DetType = OverLimit,
  .Erro = 0,
  .ErrCount = 0,
  
};
  
   
//CAN2���ϼ���������ݽṹ
StructFaultDetCfg FltCan2Erro =
{
  .FaultEn = Enable,
  .FaultRecEn = Enable,
  .RecLock = UnLock,
  .DetType = DetByBitAction,
  .DetBasisType = ByTimer,
  .Timer.FaultContinueTime = 1000,
  .Timer.FaultRecTime = 1000,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 10,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_SET,
  .Bit.RecBit = Bit_RESET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 0,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 0,
  .AdcValue.DetType = OverLimit,
  .Erro = 0,
  .ErrCount = 0,
  
};

   
//SPIFLASH���ϼ���������ݽṹ
StructFaultDetCfg FltSpiFlashErro =
{
  .FaultEn = Enable,
  .FaultRecEn = Enable,
  .RecLock = UnLock,
  .DetType = DetByBitAction,
  .DetBasisType = ByCount,
  .Timer.FaultContinueTime = 1000,
  .Timer.FaultRecTime = 1000,
  .Timer.Tick = 0,
  .Timer.RecTick = 0,
  .Count.FaultCounts = 3,
  .Count.FaultRecCount = 0,
  .Count.Count = 0,
  .Bit.TiggerBit = Bit_SET,
  .Bit.RecBit = Bit_RESET,
  .AdcValue.UpLimitValue = 0,
  .AdcValue.LowLimitValue = 0,
  .AdcValue.UpLimitRecValue = 0,
  .AdcValue.LowLimitRecValue = 0,
  .AdcValue.DetType = OverLimit,
  .Erro = 0,
  .ErrCount = 0,
  
};


WS2812BCfgStruct BreatheBlueCfg = 
{
  .RGB.B = 178,
  .RGB.R = 26,
  .RGB.G = 178,
  .RGBTemp.B = 0,
  .RGBTemp.R = 0,
  .RGBTemp.G = 0,
  .UpTime = 1000,
  .KeepOnTime = 500,
  .DownTime = 2000,
  .KeepOffTime = 0,
  .UpTickB = 0,
  .UpTickR = 0,
  .UpTickG = 0,
  .KeepOnTick = 0,
  .DownTickB = 0,
  .DownTickR = 0,
  .DownTickG = 0,
  .KeepOffTick = 0,
  .BreatheStep = BreatheUp,

};

WS2812BCfgStruct BreatheWhiteCfg = 
{
  .RGB.B = 255,
  .RGB.R = 255,
  .RGB.G = 255,
  .RGBTemp.B = 0,
  .RGBTemp.R = 0,
  .RGBTemp.G = 0,
  .UpTime = 1000,
  .KeepOnTime = 500,
  .DownTime = 2000,
  .KeepOffTime = 0,
  .UpTickB = 0,
  .UpTickR = 0,
  .UpTickG = 0,
  .KeepOnTick = 0,
  .DownTickB = 0,
  .DownTickR = 0,
  .DownTickG = 0,
  .KeepOffTick = 0,
  .BreatheStep = BreatheUp,

};


StructUserConfig UserParam =
{
  
  .PillarNum = {0x07,0x55,},  //׮��
  .AssetNum = "1234567890123456",//�ʲ�����

  .MacAddr = {0, 0, 0, 0, 0, 0},//Mac��ַ
  //.NetType = 0,//����ģʽ
  .DcMaxVolt = 5000,//ģ������ѹ XXX.xV
  .DcMinVolt = 2000,//ģ����С��ѹ XXX.xV
  .DcMaxCurrt = 170,//ģ�������� XX.xA
  .DcMinCurrt = 00,//ģ����С���� XX.xA
  .DcMaxPower = 36,//ģ������� XX KW
  .DcDisMinVolt = 3000,
  .ChgVer = eChgVer_2015,
  .ChgFun.Mult = 0x00000FEB,
  //.TempDiff = 400,
  .AlarmTemp =80,
  .FaultTemp = 100,
  .VoltHigh = 2600,
  .VoltLow = 1600,
  .VoltRatio = 1003,     //����0.001����
  .CurrtRatio = 1010,    //����0.001����

  //.Module = {eDCModType_Incre,1},
  .SingleMaxCurrt = 160,
  .SOCMAX = 100,
};
