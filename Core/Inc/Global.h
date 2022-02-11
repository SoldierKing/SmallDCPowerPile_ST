
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
  u8  RecordValid;      //�����Ϣ��־      0 �����Ϣ��Ч 1�����Ϣ��Ч
  u8  CardValidFlag;    //����Ϣ��־      0 ����Ϣ��Ч(APP�˺�) 1����Ϣ��Ч
  u8  CardSureCtrl;     //����˻���֤״̬  0����Ҫ���� 1�ȴ����� 2�ѷ��� 3�ѻظ� 4�ظ���ʱ  5�����쳣 6�ۿ���ط�
  u8  ChgServerUpCtrl;  //����������״̬  0����Ҫ���� 1�ȴ����� 2�ѷ��� 3�ѻظ�
  u8  ChgRecUpCtrl;    //����¼��Ϣ�ϴ�״̬ 0����Ҫ���� 1�ȴ����� 2�ѷ��� 3�ѻظ� 4��Ҫ���� 5�ѱ���
  u8  ChgStartNoteCtrl;   //��翪ʼ״̬�ϴ�  0����Ҫ���� 1�ȴ����� 2�ѷ��� 3�ѻظ�
  u8  ChgStopNotetrl;   //��翪ʼ״̬�ϴ�  0����Ҫ���� 1�ȴ����� 2�ѷ��� 3�ѻظ�

  u8  CardNum[16];      //����
  u8  OrderId[8];       //������
  u8  ServiceNum[8];    //���к�   
  u8  CardPwd[6];       //����
  u8  CardResp;         //������֤���
  s32 CardRespMoney;     //������֤���
  u8  CardRespCtrl;         //������֤���

  u16 ChgEnergySeg[48];         //ʱ�ε��� 0.001kwh/bit

  u8  MoneyDecFlag;   //0xFFδ�ۿ� 0x00�����¿ۿ� 0x5F���οۿ�   //��ӦFlash���Կ��Կ۷Ѻ󸲸�
  u8  ChgStopType;      //���ֹͣ����   //1=�������� 2=�쳣���� ����μ���¼�������������붨�塷
  u16  ChgStopReason;//ֹͣԭ�������¼��ϴ�
  u8  ChgType;       //��������� 0=��Ȼ���  1=�Զ���ʱ����  2=�Զ�����  3=�Զ���������磻
  u32 ChgTypeParam; //��������Բ���0=��Ȼ��磨0��1=�Զ���ʱ�䣨1s/bit����2=�Զ���������0.001Kwh/bit��3=�Զ����1��/bit��

  u32 ChgEnergy;     //�ۻ������� 0.001kWh/bit
  u32 ChgTime;       //�ۻ����ʱ�� 1s/bit

  u32 RespMaxChgEnergy; //��֤�ɳ����� 0.001kWh/bit
  u32 RespMaxChgTime;   //��֤�ɳ��ʱ�� 1s/bit

  u32 WipeStartTime;    //ˢ����ʼ���ʱ�� 0δˢ�� ��0 ˢ��ʱ���׼ʱ���
  u32 WipeStopTime;     //ˢ���������ʱ�� 0δˢ�� ��0 ˢ��ʱ���׼ʱ���
  u32 ChgStartTime;     //��翪ʼʱ�� 0δ��ʼ ��0��翪ʼʱ��ʱ���
  u32 ChgStopTime;      //������ʱ�� 0δ��ʼ ��0������ʱ��ʱ���

  u32 StartMeterEnergy; //��ʼ����ܱ���� 0.001kWh/bit
  u32 StopMeterEnergy;  //��������ܱ���� 0.001kWh/bit

  s32 StartCardMoney;    //��ʼ��翨���  (1��/bit��
  s32 StopCardMoney;    //������翨���  (1��/bit��
  s32 EnergyMoney;  //��ѽ��
  s32 ServeMoney;   //����ѽ��
  s32 EmployMoney;   //׮ռ�ý��
  s32 TotalMoney;   //�����ѽ��
}StructChgRecd;

/*
//
//ChgStopType ���ֹͣ���ͱ�
//

ֹͣ״̬	ֹͣԭ��	�ֽڱ���
�û�ֹͣ	�û�ͨ��ˢ��ֹͣ�������ֶ�ѡ��ֹͣ���	1
Զ��ֹͣ	ͨ��ƽ̨ϵͳ�´�ͣ��ָ����ֹͣ	2
����ֹͣ	���ճ����ԣ������������ֹͣ	3
��ǹֹͣ	"����׮CC/CP�źŶϿ���ֱ��׮CC1����ȷ�϶Ͽ�"	4
����ֹͣ	�����ôﵽ�˻���ֹͣ���	5
����ֹͣ	�����������ֹͣ	6
Զ������ֹͣ	��Զ���������׮��ֹͣ���	7
����ֹͣ	�������У����׮�ϵ�	8
����Աֹͣ	����Աֹͣ��磬������	9

ϵͳ����ֹͣ	ϵͳ�����޷���磬���ֹͣ	20
��ֹͣͣ	����ͣ����ť���£����ֹͣ	21
����������ֹͣ	������ͨѶ���ϣ�ֹͣ���	22
������ֹͣ	���ͨѶ���ϣ�ֹͣ���	23
��ʾ����ֹͣ	��ʾ��ͨѶ���ϣ�ֹͣ���	24
��Ե�����ͨѶ����ֹͣ	��Ե�����ͨѶ���ϣ�ֹͣ���	25
�Ӵ�������ֹͣ	�Ӵ������ϣ�ֹͣ���	26

�����ѹֹͣ	�����ѹ���ڶ��ѹ��ֹͣ���	40
����Ƿѹֹͣ	�����ѹС�ڶ��ѹ��ֹͣ���	41
����ȱ��ֹͣ	�����ѹȱ�ֹ࣬ͣ���	42
�������ֹͣ	���������ڱ���������ֹͣ���	43
�����·ֹͣ	���������ڶ�·����������ֹͣ���	44
�����ѹֹͣ	�����ѹ���ڱ�����ѹ��ֹͣ���	45
���Ƿѹֹͣ	�����ѹС�ڱ�����ѹ��ֹͣ���	46

BMS����ֹͣ	BMS����������磬���ֹͣ	80
���׮��Ե����ֹͣ	���׮��Ե�����ϣ�ֹͣ���	81
BMSͨѶ�쳣ֹͣ	BMSͨѶ���󣬳���ֹͣ	82
��ص�ѹ����ֹͣ	�����ص�ѹ���ߣ����ֹͣ	83
����¶ȹ���ֹͣ	�������¶ȹ��ߣ����ֹͣ	84
��س���������ֹͣ	�����س������쳣�����ֹͣ	85
����SOC����ֹͣ	�������SOC���߻��߹��ߣ�ֹͣ���	86
��ؾ�Ե����ֹͣ	�������ؾ�Ե״̬���ϣ�ֹͣ���	87

�����쳣ֹͣ	����ͨѶ�쳣��ֹͣ���	120
��ʼSOC��ֹͣ	��ʼ���BMS�ṩSOC����98��ֹͣ���	121
����ص�ѹ�쳣ֹͣ	��ʼ���BMS��ص�ѹ�����������ѹ��ֹͣ���	122
���׮��ѹ������ֹͣ	��ʼ���BMS��ص�ѹ���ڳ��׮����ѹ��ֹͣ���	123
���ʵ���ѹ�쳣ֹͣ	��ʼ�����ʵ���ѹ���ڳ��׮����ѹ��ֹͣ���	124
���SOC��ֹͣ	�����SOC������10���ӣ����ֹͣ	125
�����쳣ֹͣ	�������½��쳣��ֹͣ���	126
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
  u8  ValidFlag; //0��Ч 1��Ч
  u8  ReserveNoteCtrl;
  u8  CardNum[16];//ԤԼ���� 16
  u32 StartTime;//ԤԼ��ʼʱ��
  u32 StopTime;//ԤԼ����ʱ��
}StructReserveParam;

typedef struct
{
  u8  ConnectFlag;//0 δ���� ����������
  u8  SignInFlag; //0 δע�� ������ע��
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
}EnumChargeError;//���й�������
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

#define __SoftVer__ (0x0101)  //BCD�� ��:V1.02   SoftVer = 0x0102

#endif

