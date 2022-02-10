#ifndef  __ADRV_DCMODEL_H__
#define  __ADRV_DCMODEL_H__

#include "Global.h"



typedef struct
{
  u16 MaxVolt;//0.1V/Bit;
  u16 MinVolt;//0.1V/Bit;
  u16 MaxCurrt;//0.1A/Bit;
  u16 MinCurrt;//0.1A/Bit;
  u16 SingleMaxCurrt;//0.1A/Bit
  
}StructDcModuleAbility;


typedef union
{
  u32 Mult;
  struct
  {
    u32 SeNunLo : 9;
    u32 ProduDay: 5;
    u32 ModAd   : 7;//module adress
    u32 MonAd   : 4;//monitor adress
    u32 Protocol: 4;
    u32 Reserve : 3;
  }Disp;
}UnionYouExtId;

typedef struct
{
  u8 Data[8];
  u8 Len; //Ö¡Êý¾Ý³¤
  u32 Id;//Ö¡ID
}StructDcModuleSend;

typedef union
{
  u8 Mult;
  struct
  {
    u8 cmd       : 4;
    u8 Rang      : 1;
    u8 Re1       : 1;
    u8 Sc        : 1;
    u8 Re2       : 1;
  }Disp;
}UnionCtlNum;



typedef struct
{
  u8  Cmd;
  u8  ValidNum;
  u32 SetVolt;//0.001V/Bit
  u32 SetCurrt;//0.001A/Bit
  long long OnlineFlag;
  long long ValidFlag;
  u8  ModuleMalfunctionCount;
}StructDcModuleManage;

typedef struct
{
  u8 CtrlFlag;
  u16 NeedVolt;//0.1V/Bit;
  u16 NeedCurrt;//0.1A/Bit;
  u16 ActualVolt;//0.1V/Bit;
  u16 ActualCurrt;//0.1A/Bit;
  
  u16 ActualVoltA;//0.1V/Bit;
  u16 ActualCurrtA;//0.1A/Bit;
  u16 ActualVoltB;//0.1V/Bit;
  u16 ActualCurrtB;//0.1A/Bit;
  u16 ActualVoltC;//0.1V/Bit;
  u16 ActualCurrtC;//0.1A/Bit;
  u16 ActualPower;//0.1KW/Bit;
  
  u32 NeedPower;//1W/Bit;
  u32 SetPower;//1W/Bit;
}StructDcModuleCtrl;

typedef union
{
  u32 Mult;
  struct
  {
    u32 SourAddr  : 8;
    u32 DestAddr  : 8;
    u32 Cmd       : 6;
    u32 DeviceNum : 4;
    u32 ErrNum    : 3;
  }Disp;
}UnionINFYHeadId;

typedef enum
{
  eSwSta_Low= 0,
  eSwSta_High,
}EnumModuStatus;

typedef struct
{
  u8  Id;
  u8  IDH;
  u8  Temp;
  u16 Volt;//0.1V/Bit;
  u16 Currt;//0.1A/Bit;
  u32 MaxPower;//1W/Bit;
  u16 TotalPower;//0.1KW/bit
  u32 State;
  u8  GroupNum;
  u8  ErroFlag; 
  u32 RecvTime;
  EnumModuStatus ModStatus;
  u16 VoltA;
  u16 VoltB;
  u16 VoltC;
  u16 CurA;
  u16 CurB;
  u16 CurC;
}StructDcModuleStatus;

extern StructDcModuleAbility DcModuleAbility;

extern StructDcModuleManage DcModuleManage;

extern StructDcModuleCtrl DcModuleCtrl;

extern u8 DcModuleInit(void);
extern u8 DcModuleSendMsg(StructDcModuleSend *pMsg);

extern void DcModuleSet(EnumSwitchStatus Set, u16 Volt, u16 Currt);
extern u16 GetDcModuleVolt(void);
extern u16 GetDcModuleCurrt(void);
extern u16 GetDcModuleVoltSet(void);
extern u16 GetDcModuleCurrtSet(void);
extern u16 GetDcModuleVoltA(void);


extern u32 TemDecreaseCurrentValue(void);

#endif