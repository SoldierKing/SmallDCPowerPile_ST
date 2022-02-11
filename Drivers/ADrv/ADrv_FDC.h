#ifndef  __ADRV_FDC_H__
#define  __ADRV_FDC_H__

#include "Global.h"


/*--------------------------------检测故障所用的类型----------------------------*/
typedef enum
{
  DetByBitAction,
  DetByAdcValue,
}EnumDetectType;

typedef enum
{
  Lock,
  UnLock,
}EnumLockState;

typedef enum
{
  Enable,
  Disable,
}EnumEN;

typedef enum
{
  ByTimer,
  ByCount,
}EnumDetectBasis;

typedef struct
{
  u32 FaultContinueTime;
  u32 FaultRecTime;
  u32 Tick;
  u32 RecTick;
}StructTimer;

typedef struct
{
  u32 FaultCounts;
  u32 FaultRecCount;
  u32 Count;
}StructCount;

typedef enum
{ Bit_RESET = 0,
  Bit_SET
}BitAction;

typedef struct
{
  BitAction TiggerBit;
  BitAction RecBit;
}StructBit;

typedef enum
{
  OverLimit,
  MiddleLimit,
  LowLimit,
}EnumVaultDetType;

typedef struct
{
  u32 UpLimitValue;
  u32 LowLimitValue;
  u32 UpLimitRecValue;
  u32 LowLimitRecValue;
  EnumVaultDetType DetType;
}StructAdcVaule;


//故障配置数据全局变量类型
typedef struct
{
  //故障检测使能
  EnumEN    FaultEn;
  //故障恢复锁
  EnumLockState RecLock;
  //故障恢复使能
  EnumEN    FaultRecEn;
  //故障检测值(需动态赋值)
  u32   FaultVaule;
  //估值检测高低电平(需动态赋值)
  BitAction BitVaule;
  //故障检测判断方法(通过IO 高低电平,通过上下限量值)
  EnumDetectType  DetType;
  //故障判断依据(通过时间，通过稳定次数)
  EnumDetectBasis DetBasisType;
  //故障持续时间
  StructTimer  Timer;
  //故障稳定次数
  StructCount  Count;
  //判断得高低电平
  StructBit    Bit;
  //判断得故障检测值及判断方法
  StructAdcVaule AdcValue;
  //发生故障标志位0--未发生故障,1--发生故障
  u8     Erro;
  //故障次数
  u8     ErrCount;
  
}StructFaultDetCfg;


extern int FaultDetct(StructFaultDetCfg *DetCfg);

#endif