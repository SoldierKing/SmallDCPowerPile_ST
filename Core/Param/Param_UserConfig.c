#include "Param_UserConfig.h"




//LED 快闪(亮250MS，灭250MS),一直循环(运行灯配置信息)
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

//LED 快闪(亮250MS，灭250MS),一直循环(电池电压指示灯效果)
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


//蓝色灯 快闪(亮5000MS，灭500MS),一直循环(运行灯配置信息)
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

//定义启动按键长按短按信息
StructKeyPressCfg KeyPressCfg = 
{

  .IsDown = GPIO_PIN_SET,
  .IsUp = GPIO_PIN_RESET,
  .KeyShortPressTime = 100,
  .KeyLongPressTime = 3000,
  .BtnTick = 0,
  
};

//定义一建停机、解锁按键长按短按信息
StructKeyPressCfg StopBtnPressCfg = 
{

  .IsDown = GPIO_PIN_RESET,
  .IsUp = GPIO_PIN_SET,
  .KeyShortPressTime = 100,
  .KeyLongPressTime = 3000,
  .BtnTick = 0,
  
};

//工作模式切换配置
StructWorkModeCfg  WorkModeCfg = 
{

  //模式状态锁,锁住的时候无法切换状态
  .StateLock = DISABLE,
  //模式切换使能
  .SwitchModeEN = ENABLE,
  //工作模式
  .WorkMode = Enum_ChargeWork,
  
};


//充电过程中掉电故障配置
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



//电子锁故障配置数据结构
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


//高压继电器故障配置数据结构
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


//车端电压反接故障配置数据结构
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


//漏电流故障配置数据结构
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


//开关机按键故障配置数据结构
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


//一建关机按键故障配置数据结构
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


//检测是否有DC模块配置数据结构
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
  
  
//CAN1故障检测配置数据结构
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
  
   
//CAN2故障检测配置数据结构
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

   
//SPIFLASH故障检测配置数据结构
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
  
  .PillarNum = {0x07,0x55,},  //桩号
  .AssetNum = "1234567890123456",//资产编码

  .MacAddr = {0, 0, 0, 0, 0, 0},//Mac地址
  //.NetType = 0,//网络模式
  .DcMaxVolt = 5000,//模块最大电压 XXX.xV
  .DcMinVolt = 2000,//模块最小电压 XXX.xV
  .DcMaxCurrt = 170,//模块最大电流 XX.xA
  .DcMinCurrt = 00,//模块最小电流 XX.xA
  .DcMaxPower = 36,//模块最大功率 XX KW
  .DcDisMinVolt = 3000,
  .ChgVer = eChgVer_2015,
  .ChgFun.Mult = 0x00000FEB,
  //.TempDiff = 400,
  .AlarmTemp =80,
  .FaultTemp = 100,
  .VoltHigh = 2600,
  .VoltLow = 1600,
  .VoltRatio = 1003,     //精度0.001倍数
  .CurrtRatio = 1010,    //精度0.001倍数

  //.Module = {eDCModType_Incre,1},
  .SingleMaxCurrt = 160,
  .SOCMAX = 100,
};
