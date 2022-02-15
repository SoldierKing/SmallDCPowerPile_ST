#ifndef __PARAM_USERCONFIG_H__
#define __PARAM_USERCONFIG_H__

#include "Global.h"
#include "ADrv_LET.h"
#include "ADrv_KeyDown.h"
#include "ADrv_OneKeyStart.h"
#include "ADrv_FDC.h"
#include "Drv_WS2812B.h"


typedef union
{
  u32 Mult;
  struct
  {
    u8 Num_A        : 1;
    u8 Num_B        : 1;
    u8 Num_AB       : 1;
    u8 IdleLed      : 1;
    u8 Dehumi       : 1;
    u8 Door         : 1;
    u8 BatCellJud   : 1;
    u8 TopFan       : 1;
    u8 ACMeter      : 1;
    u8 NegContactor : 1;
    u8 EleLock      : 1;
    u8 SigUniChg    : 1;
    u8 V24Swinth12  : 1;
  }Value;
}UnionChgFunctional;

typedef enum
{
  eDCModType_No            = 0x00,
  eDCModType_Incre         = 0x01,
  eDCModType_ZhongXin      = 0x02,
  eDCModType_YouYou        = 0x03,
  eDCModType_YouYouNormal  = 0x04,
  eDCModType_yongLian      = 0x05,
  eDCModType_ZhongXingNew  = 0x06,
  eDCModType_MAX      = eDCModType_ZhongXingNew,
}EnumDCModType;

typedef struct
{
  EnumDCModType DCModType;
  u8 Auto_ID;
}StructModul;

typedef enum
{
  eChgVer_2011,
  eChgVer_2015,
}EnumChgVersion;


typedef struct
{
  
  u8 PillarNum[8];  //桩号
  //u8 TelNum_1[16];//热线电话1
  //u8 TelNum_2[16];//热线电话2
  u8 AssetNum[16];//资产编码

  u8 MacAddr[6];//Mac地址

  //u16 NetType;//网络模式

  u16 DcMaxVolt;     //模块最大电压 XXX.xV
  u16 DcMinVolt;     //模块最小电压 XXX.xV
  u16 DcMaxCurrt;    //模块最大电流 XX.xA
  u16 DcMinCurrt;    //模块最小电流 XX.xA
  u8  DcMaxPower;    //模块最大功率 XX KW
  u16 DcDisMinVolt;  //模块放电允许最小电压 xxx.xV
  u8  ChgVer;
  UnionChgFunctional ChgFun;
  u16 TempDiff;
  u16 SuitTempDiff;
  u32 AlarmTemp;
  u16 FaultTemp;

  u16 VoltHigh;
  u16 VoltLow;

  u16 EnergyRatio;//精度0.001倍数
  u16 VoltRatio;//精度0.001倍数
  u16 CurrtRatio;//精度0.001倍数
  //StructModul Module;
  u16 SingleMaxCurrt;
  u16 SOCMAX;
  
}StructUserConfig;




extern StructUserConfig UserParam ;
extern StructKeyPressCfg KeyPressCfg;
extern StructWorkModeCfg  WorkModeCfg;
extern StructKeyPressCfg StopBtnPressCfg;

extern StructFLECfg TwinkQuick;
extern StructFLECfg TwinkQuickBlue;
extern StructFaultDetCfg FltEleLockErro;
extern StructFaultDetCfg FltRlyAdhesionErro;
extern StructFaultDetCfg FltBatReverseErro;
extern StructFaultDetCfg FltLeakErro;
extern StructFaultDetCfg FltTurnOnOffKeyErro;
extern StructFaultDetCfg FltStopKeyErro;
extern StructFaultDetCfg FltDcModuleErro;
extern StructFaultDetCfg FltCan1Erro;
extern StructFaultDetCfg FltCan2Erro;
extern StructFaultDetCfg FltSpiFlashErro;
extern StructFaultDetCfg FltACVoltDownErro;

extern StructFLECfg TwinkQuickBatLighting;
extern WS2812BCfgStruct BreatheBlueCfg;
extern WS2812BCfgStruct BreatheWhiteCfg;


#endif 