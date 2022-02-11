
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include "Stm32f1xx.h"
#include "cmsis_os.h"

#define BakStartAddr        ((u32)0x08080000)
#define AppCodeSize         ((u32)480*1024)
#define McnRomSectionSize   ((u32)128*1024)
#define UpdateFlagAddr      ((u32)0x08004000)

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef TRUE
#define TRUE  (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef UNKNOWN
#define UNKNOWN (0xFF)
#endif


/** @addtogroup Exported_types
  * @{
  */  
/*!< STM32F10x Standard Peripheral Library old types (maintained for legacy purpose) */
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

/**
  * @}
  */

typedef enum
{
  NoInit,
  Inited,
}EnumInitState;

typedef enum
{

  NioCar_Type = 0,
  OtherCar_Type,

}CarTypeEnum;



typedef union
{
  u16 Mult16;
  struct
  {
    u8 L16 : 8;
    u8 H16 : 8;
  }Disp16;
}UnionLittleEnd16;

typedef union
{
  u32 Mult32;
  struct
  {
    u32 LL32 : 8;
    u32 ML32 : 8;
    u32 MH32 : 8;
    u32 HH32 : 8;
  }Disp32;
}UnionLittleEnd32;

typedef struct
{
  u16 Year;
  u8  Mon;
  u8  Day;
  u8  Hour;
  u8  Min;
  u8  Sec;
}StructTime;

typedef enum
{
  eChgType_Auto         = 0x00,
  eChgType_Time         = 0x01,
  eChgType_Energy       = 0x02,
  eChgType_Money        = 0x03,
  eChgType_Delay        = 0x04,
}EnumChargeType;

typedef struct
{
  u32 Energy;
  u32 Serve;
  u32 All;
}StructChgCost;

typedef struct
{
  u8  RecordValid;      //充电信息标志      0 充电信息无效 1充电信息有效
  u8  CardValidFlag;    //卡信息标志      0 卡信息无效(APP账号) 1卡信息有效
  u8  CardSureCtrl;     //充电账户认证状态  0不需要发送 1等待发送 2已发送 3已回复 4回复超时  5网络异常 6扣款后重发
  u8  ChgServerUpCtrl;  //充电服务请求状态  0不需要发送 1等待发送 2已发送 3已回复
  u8  ChgRecUpCtrl;    //充电记录信息上传状态 0不需要发送 1等待发送 2已发送 3已回复 4需要保存 5已保存
  u8  ChgStartNoteCtrl;   //充电开始状态上传  0不需要发送 1等待发送 2已发送 3已回复
  u8  ChgStopNotetrl;   //充电开始状态上传  0不需要发送 1等待发送 2已发送 3已回复

  u8  CardNum[16];      //卡号
  u8  OrderId[8];       //订单号
  u8  ServiceNum[8];    //序列号   
  u8  CardPwd[6];       //密码
  u8  CardResp;         //卡号认证结果
  s32 CardRespMoney;     //卡号认证余额
  u8  CardRespCtrl;         //卡号认证结果

  u16 ChgEnergySeg[48];         //时段电能 0.001kwh/bit

  u8  MoneyDecFlag;   //0xFF未扣款 0x00已线下扣款 0x5F二次扣款   //对应Flash特性可以扣费后覆盖
  u8  ChgStopType;      //充电停止类型   //1=正常结束 2=异常结束 具体参见附录三《充电结束编码定义》
  u16  ChgStopReason;//停止原因用于事件上传
  u8  ChgType;       //充电满策略 0=自然充电  1=自定义时间充电  2=自定义金额  3=自定义电度数充电；
  u32 ChgTypeParam; //充电满策略参数0=自然充电（0）1=自定义时间（1s/bit）；2=自定义电度数（0.001Kwh/bit）3=自定义金额（1分/bit）

  u32 ChgEnergy;     //累积充电电能 0.001kWh/bit
  u32 ChgTime;       //累积充电时间 1s/bit

  u32 RespMaxChgEnergy; //认证可充电电量 0.001kWh/bit
  u32 RespMaxChgTime;   //认证可充电时间 1s/bit

  u32 WipeStartTime;    //刷卡开始充电时间 0未刷卡 非0 刷卡时间标准时间戳
  u32 WipeStopTime;     //刷卡结束充电时间 0未刷卡 非0 刷卡时间标准时间戳
  u32 ChgStartTime;     //充电开始时间 0未开始 非0充电开始时间时间戳
  u32 ChgStopTime;      //充电结束时间 0未开始 非0充电结束时间时间戳

  u32 StartMeterEnergy; //开始充电能表读数 0.001kWh/bit
  u32 StopMeterEnergy;  //结束充电能表读数 0.001kWh/bit

  s32 StartCardMoney;    //开始充电卡余额  (1分/bit）
  s32 StopCardMoney;    //结束充电卡余额  (1分/bit）
  s32 EnergyMoney;  //电费金额
  s32 ServeMoney;   //服务费金额
  s32 EmployMoney;   //桩占用金额
  s32 TotalMoney;   //总消费金额
}StructChgRecd;

/*
//
//ChgStopType 充电停止类型表
//

停止状态	停止原因	字节编码
用户停止	用户通过刷卡停止，或者手动选择停止充电	1
远程停止	通过平台系统下达停机指令，充电停止	2
正常停止	按照充电策略，充电结束，充电停止	3
拔枪停止	"交流桩CC/CP信号断开，直流桩CC1连接确认断开"	4
余额不足停止	充电费用达到账户金额，停止充电	5
充满停止	充电充满，完成停止	6
远程重启停止	用远程重启充电桩，停止充电	7
掉电停止	充电过程中，充电桩断电	8
管理员停止	管理员停止充电，充电结束	9

系统故障停止	系统故障无法充电，充电停止	20
急停停止	紧急停机按钮按下，充电停止	21
读卡器故障停止	读卡器通讯故障，停止充电	22
电表故障停止	电表通讯故障，停止充电	23
显示故障停止	显示屏通讯故障，停止充电	24
绝缘检测仪通讯故障停止	绝缘检测仪通讯故障，停止充电	25
接触器故障停止	接触器故障，停止充电	26

输入过压停止	输入电压大于额定电压，停止充电	40
输入欠压停止	输入电压小于额定电压，停止充电	41
输入缺相停止	输入电压缺相，停止充电	42
输出过流停止	充电电流大于保护电流，停止充电	43
输出短路停止	充电电流大于短路保护电流，停止充电	44
输出过压停止	输出电压大于保护电压，停止充电	45
输出欠压停止	输出电压小于保护电压，停止充电	46

BMS主动停止	BMS主动结束充电，充电停止	80
充电桩绝缘故障停止	充电桩绝缘检测故障，停止充电	81
BMS通讯异常停止	BMS通讯错误，充电机停止	82
电池电压故障停止	单体电池电压过高，充电停止	83
电池温度故障停止	单体电池温度过高，充电停止	84
电池充电电流故障停止	单体电池充电电流异常，充电停止	85
整车SOC故障停止	整车电池SOC过高或者过高，停止充电	86
电池绝缘故障停止	动力蓄电池绝缘状态故障，停止充电	87

握手异常停止	握手通讯异常，停止充电	120
开始SOC满停止	开始充电BMS提供SOC大于98，停止充电	121
充电电池电压异常停止	开始充电BMS电池电压大于允许充电电压，停止充电	122
充电桩电压不符合停止	开始充电BMS电池电压大于充电桩最大电压，停止充电	123
电池实测电压异常停止	开始充电电池实测电压大于充电桩最大电压，停止充电	124
充电SOC满停止	充电中SOC满持续10分钟，充电停止	125
电流异常停止	充电电流下降异常，停止充电	126
*/


typedef enum
{
  eChgStop_None = 0,
  eChgStop_Shake_UserCancel = 1,
  eChgStop_Imd_UserCancel = 2,
  eChgStop_Reco_UserCancle = 3,
  eChgStop_Conf_UserCancel = 4,
  eChgStop_BmsReady_UserCancel = 5,
  eChgStop_PillarReady_UserCancel = 6,
  eChgStop_Start_UserCancel = 7,
  eChgStop_Run_UserCancel = 8,
  eChgStop_ComBreak_UserCancel = 9,
  eChgStop_Shake_CcOpen = 10,
  eChgStop_Imd_CcOpen = 11,
  eChgStop_Reco_CcOpen = 12,
  eChgStop_Conf_CcOpen = 13,
  eChgStop_BmsReady_CcOpen = 14,
  eChgStop_PillarReady_CcOpen = 15,
  eChgStop_Start_CcOpen = 16,
  eChgStop_Run_CcOpen = 17,
  eChgStop_ComBreak_CcOpen = 18,
  eChgStop_Shake_EmerClose = 19,
  eChgStop_Imd_EmerClose = 20,
  eChgStop_Reco_EmerClose = 21,
  eChgStop_Conf_EmerClose = 22,
  eChgStop_BmsReady_EmerClose = 23,
  eChgStop_PillarReady_EmerClose = 24,
  eChgStop_Start_EmerClose = 25,
  eChgStop_Run_EmerClose = 26,
  eChgStop_ComBreak_EmerClose = 27,
  eChgStop_Imd_BhmTimeOut = 28,
  eChgStop_Conf_StartSoc98 = 29,
  eChgStop_Conf_BatVoltOverPermitMax = 30,
  eChgStop_Conf_StartVoltOverPillarMax = 31,
  eChgStop_PillarReady_BatVoltOverPillarMax = 32,
  eChgStop_ComBreak_CurtError = 33,
  eChgStop_ComBreak_BmsStop = 34,
  eChgStop_ComBreak_CommError = 35,
  eChgStop_Start_BmsStop = 36,
  eChgStop_Run_BmsStop = 37,
  eChgStop_Imd_IsolateError = 38,
  eChgStop_Run_Soc98 = 39,
  eChgStop_Run_SinglBatVolt = 40,
  eChgStop_Run_SocOver = 41,
  eChgStop_Run_CurrtOver = 42,
  eChgStop_Run_BatTempOver = 43,
  eChgStop_Run_BatIsolate = 44,
  eChgStop_Run_Cc2Error = 45,
  eChgStop_Run_BmsPause = 46,
  eChgStop_Conf_ChgMaxVoltLessPillarMinVolt = 47,
  eChgStop_PillarReady_BatVoltError = 48,
  eChgStop_PillarReady_BatVoltMismatchBcpVolt = 49,
  eChgStop_Run_VoltOutHighErr = 50,
  eChgStop_Run_CurrtOutHighErr = 51,
  eChgStop_Conf_DoorOpen = 52,
  eChgStop_Reco_DoorOpen = 53,
  eChgStop_Run_DoorOpen = 54,
  eChgStop_Start_DoorOpen = 55,
  eChgStop_ComBreak_DoorOpen = 56,
  eChgStop_Imd_DoorOpen = 57,
  eChgStop_Shake_DoorOpen = 58,
  eChgStop_BmsReady_DoorOpen = 59,
  eChgStop_PillarReady_DoorOpen = 60,
  eChgStop_Run_SinglBatVoltOverPermitMax = 61,
  eChgStop_Conf_Contactor = 62,
  eChgStop_Reco_Contactor = 63,
  eChgStop_Run_Contactor = 64,
  eChgStop_Start_Contactor = 65,
  eChgStop_ComBreak_Contactor = 66,
  eChgStop_Imd_Contactor = 67,
  eChgStop_Shake_Contactor = 68,
  eChgStop_BmsReady_Contactor = 69,
  eChgStop_PillarReady_Contactor = 70,
  eChgStop_Conf_InVolt = 71,
  eChgStop_Reco_InVolt = 72,
  eChgStop_Run_InVolt = 73,
  eChgStop_Start_InVolt = 74,
  eChgStop_ComBreak_InVolt = 75,
  eChgStop_Imd_InVolt = 76,
  eChgStop_Shake_InVolt = 77,
  eChgStop_BmsReady_InVolt = 78,
  eChgStop_PillarReady_InVolt = 79,
  eChgStop_Conf_NoDcmod = 80,
  eChgStop_Reco_NoDcmod = 81,
  eChgStop_Run_NoDcmod = 82,
  eChgStop_Start_NoDcmod = 83,
  eChgStop_ComBreak_NoDcmod = 84,
  eChgStop_Imd_NoDcmod = 85,
  eChgStop_Shake_NoDcmod = 86,
  eChgStop_BmsReady_NoDcmod = 87,
  eChgStop_PillarReady_NoDcmod = 88,
  eChgStop_Conf_Meter = 89,
  eChgStop_Reco_Meter = 90,
  eChgStop_Run_Meter = 91,
  eChgStop_Start_Meter = 92,
  eChgStop_ComBreak_Meter = 93,
  eChgStop_Imd_Meter = 94,
  eChgStop_Shake_Meter = 95,
  eChgStop_BmsReady_Meter = 96,
  eChgStop_PillarReady_Meter = 97,
  eChgStop_Imd_Short = 98,
  eChgStop_WaitDisChg_UserCancel = 99,
  eChgStop_WaitDisChg_CcOpen = 100,
  eChgStop_WaitDisChg_StopByCar = 101,
  eChgStop_DisCharge_StopByCar = 102,
  eChgStop_DisCharge_TimeOut = 103,
  eChgStop_Stop_ByKeyTurnOff = 104,
  eChgStop_Stop_ByOneKeyBtn = 105,
  eChgStop_PillarReady_BatCurrentErr = 106,
  eChgStop_Run_VoltInputErr = 107,
  eChgStop_Run_DisVoltOutErr = 108,
  eChgStop_V2L_DisCarTypeErr = 109,
  eChgStop_BRM_DisCarTypeErr = 110,
  eChgStop_V2L_DisPileModeErr = 111,
  eChgStop_VEH_RES_WorkModleErr = 112,


}EnumChgStopType;

typedef struct
{
  u8  ValidFlag; //0无效 1有效
  u8  ReserveNoteCtrl;
  u8  CardNum[16];//预约卡号 16
  u32 StartTime;//预约开始时间
  u32 StopTime;//预约结束时间
}StructReserveParam;

typedef struct
{
  u8  ConnectFlag;//0 未连接 其他已连接
  u8  SignInFlag; //0 未注册 其他已注册
  u8  FrameSendNum;
  u8  FrameRecvNum;
  u8  HeartBeatErrCnt;
  u8  HeartBeatErrCnt_B;
  u8  HeartBeatErrCnt_AB;
  u16 HeartBeatNumCnt;
  u16 HeartBeatLoseCnt;
  u32 HeartBeatSendTime;
  u32 HeartBeatSendTime_B;
   u32 HeartBeatSendTime_AB;
  u32 HeartBeatRecvTime;
  u32 HeartBeatRecvTime_B;
  u32 HeartBeatRecvTime_AB;
  u32 SignInSendTime;
  u32 SignInRecvTime;
  u32 StatusSendTime;
  u32 StatusRecvTime;
}StructCommuStatus;

typedef enum
{
  eSwSta_Off = 0,
  eSwSta_On,
  eSwSta_Keep1_5,
}EnumSwitchStatus;

typedef union
{
  u32 Mult;
  struct
  {
    u32 Emergency   : 1;
    u32 NoDcmod     : 1;
    u32 Insulation  : 1;
    u32 ImdDev      : 1;
    u32 DrainResist : 1;
    u32 Reader      : 1;
    u32 Meter       : 1;
    u32 Memory      : 1;
	
    u32 Lcd         : 1;
    u32 Net         : 1;
    u32 Door        : 1;
    u32 SPD         : 1;
    u32 Humiture    : 1;
    u32 ACMeter     : 1;
    u32 Contactor   : 1;
    u32 BMS         : 1;
	
    u32 InVolt      : 1;
    u32 EleLock     : 1;
    u32 OutVolt     : 1;
    u32 OutCurrt    : 1;
    u32 UniContactor: 1;
    u32 InterTempOver:1;
    u32 PosGunTempOver:1;
    u32 NeGunTempOver:1;
    u32 ACCContactor :1;
    u32 ACBreaker    :1;
  }Value;
}UnionPillarError;

typedef union
{
  u32 Mult;
  struct
  {
    u32 DrainResist   : 1;
    u32 IsolateWarn   : 1;
    u32 IsolateError  : 1;
    u32 VoltOutError  : 1;
    u32 CurrtOutError : 1;
  }Value;
}UnionChgError;

/*
typedef enum
{
  eChgErr_NoneError = 0,
  eChgErr_StopErr   = (1<<0),
  eChgErr_RelayErr  = (1<<1),
  eChgErr_CpErr     = (1<<2),
  eChgErr_ConnectErr= (1<<3),
  eChgErr_CurrtLeak = (1<<4),
  eChgErr_CurrtOver = (1<<5),
  eChgErr_CurrtProt = (1<<6),
  eChgErr_VoltHigh  = (1<<7),
  eChgErr_VoltLow   = (1<<8),
  eChgErr_TempOver  = (1<<9),
  eChgErr_NoneGnd   = (1<<10),
  eChgErr_PhaseErr  = (1<<11),
  eChgErr_Reader    = (1<<12),
  eChgErr_Meter     = (1<<13),
  eChgErr_Memory    = (1<<14),
  eChgErr_Display   = (1<<15),
  eChgErr_NetConnect= (1<<16),
  eChgErr_Max,
}EnumChargeError;//充电盒故障类型
*/

extern UnionPillarError PillarError;
extern UnionChgError ChargeError;

extern UnionPillarError PillarError_B;
extern UnionChgError ChargeError_B;

extern u32 GetSystemTick(void);
extern u8 BcdToHex(u8 Bcd);
extern u8 HexToBcd(u8 Hex);

extern int CopyCode(u32 Des, u32 Scr, u32 Len);
extern int UpdateFlagSet(u32 Status);

#define __SoftVer__ (0x0101)  //BCD码 如:V1.02   SoftVer = 0x0102

#endif

