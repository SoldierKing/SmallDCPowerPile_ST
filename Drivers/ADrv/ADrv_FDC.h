#ifndef  __ADRV_FDC_H__
#define  __ADRV_FDC_H__

#include "Global.h"


/*--------------------------------���������õ�����----------------------------*/
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


//������������ȫ�ֱ�������
typedef struct
{
  //���ϼ��ʹ��
  EnumEN    FaultEn;
  //���ϻָ���
  EnumLockState RecLock;
  //���ϻָ�ʹ��
  EnumEN    FaultRecEn;
  //���ϼ��ֵ(�趯̬��ֵ)
  u32   FaultVaule;
  //��ֵ���ߵ͵�ƽ(�趯̬��ֵ)
  BitAction BitVaule;
  //���ϼ���жϷ���(ͨ��IO �ߵ͵�ƽ,ͨ����������ֵ)
  EnumDetectType  DetType;
  //�����ж�����(ͨ��ʱ�䣬ͨ���ȶ�����)
  EnumDetectBasis DetBasisType;
  //���ϳ���ʱ��
  StructTimer  Timer;
  //�����ȶ�����
  StructCount  Count;
  //�жϵøߵ͵�ƽ
  StructBit    Bit;
  //�жϵù��ϼ��ֵ���жϷ���
  StructAdcVaule AdcValue;
  //�������ϱ�־λ0--δ��������,1--��������
  u8     Erro;
  //���ϴ���
  u8     ErrCount;
  
}StructFaultDetCfg;


extern int FaultDetct(StructFaultDetCfg *DetCfg);

#endif