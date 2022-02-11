#include "ADrv_ImdCheck.h"
#include "ADrv_AdcVolt.h"
#include "ADrv_Comm.h"
#include <math.h>

//函数声明
int GetIMDCheckResult(StructImdValue *pImdValue);

typedef struct
{
  
  u16 Vx1;
  u16 Vx2;
  u16 Vy1;
  u16 Vy2;
}StructImdVolt;


//基准电压值
#define  RefVolt                      (float)(3.3)
#define  R1                           (float)(270*5)
#define  R3                           (float)(22*5)
//Rx分母
#define  RxDenom(Vx1,Vx2,Vy1,Vy2)     (float)(R3*Vx2*(Vx2*Vy1-Vx1*Vy2));
//Rx分子
#define  RxMolecule(Vx1,Vx2,Vy1,Vy2)  (float)(Vy2*(Vx2*Vy1+Vx1*Vx2));
//Ry分母
#define  RyDenom(Vx1,Vx2,Vy1,Vy2)     (float)(R3*(Vx2*Vy1-Vx1*Vy2));
//Ry分子
#define  RyMolecule(Vx1,Vx2,Vy1,Vy2)  (float)(Vx1*Vy2+Vx1*Vx2);


//IMD检测使能
static FunctionalState ImdCheckEN = DISABLE;
//IMD启动使能
static FunctionalState ImdStartEN = DISABLE;
//Vx Vy电压数据结构
static StructImdVolt   ImdVxVolt;



/// <summary>
///  Imd绝缘检测线程
/// </summary>
/// <param name="StructImdValue *pImdValue">输入IMD数据结构指针</param>  
void ImdCheckHandler(StructImdValue *pImdValue)
{
  static EnumInitState ImdCheckOnece = NoInit;  

  if (!ImdStartEN)
  {
    pImdValue->Vaild = 0;
    //SwitcBoostENOnOff(eSwSta_Off);
    ImdCheckOnece = NoInit;
    return;
  }
  pImdValue->Vaild = 1;
  //开启imd绝缘检测辅源
  SwitcBoostENOnOff(eSwSta_On);
  if (GetBoostENSwitchSta() == GPIO_PIN_SET && !ImdCheckEN)
  {
    
    pImdValue->PnVolt = AdcVoltBuf[EnumVhdc_AD2];
    ImdCheckOnece = NoInit;
    
  }
  else if (ImdCheckEN && GetBoostENSwitchSta() == GPIO_PIN_SET)
  {
    
    if (ImdCheckOnece == NoInit)
    {
      osDelay(50);
      
      GetIMDCheckResult(pImdValue);
      ImdCheckOnece = Inited;
      
    }
    
  }   
}


int GetIMDCheckResult(StructImdValue *pImdValue)
{
  
  if (GetBoostENSwitchSta() == GPIO_PIN_SET)
  {
    SwitcIMDPRlyOnOff(eSwSta_On);
    SwitcIMDPERlyOnOff(eSwSta_On);
    SwitcIMDNRlyOnOff(eSwSta_Off);
    osDelay(500);
    //获取Vx1 Vy1电压值
    if (GetIMDHPSta() == GPIO_PIN_SET && GetIMDHPESta() == GPIO_PIN_SET
        && GetIMDHNSta() == GPIO_PIN_RESET)
    {       

      ImdVxVolt.Vx1 = AdcVoltBuf[EnumVpdc_AD0];
      ImdVxVolt.Vy1 = AdcVoltBuf[EnumVndc_AD1];
    }
    else return -1;
    
    SwitcIMDPRlyOnOff(eSwSta_Off);
    SwitcIMDNRlyOnOff(eSwSta_On);
    osDelay(500);
    //获取Vx2 Vy2电压值
    if (GetIMDHPSta() == GPIO_PIN_RESET && GetIMDHPESta() == GPIO_PIN_SET
        && GetIMDHNSta() == GPIO_PIN_SET)
    {
    
      ImdVxVolt.Vx2 = AdcVoltBuf[EnumVpdc_AD0];
      ImdVxVolt.Vy2 = AdcVoltBuf[EnumVndc_AD1];
    }
    else return -1;
    
    SwitcIMDPRlyOnOff(eSwSta_Off);
    SwitcIMDPERlyOnOff(eSwSta_Off);
    SwitcIMDNRlyOnOff(eSwSta_Off);
    //计算正极绝缘电阻
    float Temp = 0;
    Temp = RxMolecule((float)(ImdVxVolt.Vx1/10),(float)(ImdVxVolt.Vx2/10),
                      (float)(ImdVxVolt.Vy1/10),(float)(ImdVxVolt.Vy2/10));
    Temp = Temp / RxDenom((float)(ImdVxVolt.Vx1/10),(float)(ImdVxVolt.Vx2/10),
                      (float)(ImdVxVolt.Vy1/10),(float)(ImdVxVolt.Vy2/10));
    
    pImdValue->PositResist = (u16)(1/(Temp - (1/R1)));
    
    //计算负极绝缘电阻
    Temp = 0;
    Temp = RyMolecule((float)(ImdVxVolt.Vx1/10),(float)(ImdVxVolt.Vx2/10),
                      (float)(ImdVxVolt.Vy1/10),(float)(ImdVxVolt.Vy2/10));
    Temp = Temp / RyDenom((float)(ImdVxVolt.Vx1/10),(float)(ImdVxVolt.Vx2/10),
                      (float)(ImdVxVolt.Vy1/10),(float)(ImdVxVolt.Vy2/10));
    
    pImdValue->NegatResist = (u16)(1/(Temp - (1/R1)));
    
  }
  else return -1;
  
  return 1;
}

/// <summary>
///  开关IMD检测辅源启动使能
/// </summary>
/// <param name="EnumSwitchStatus">输入想要改变的状态</param>  
void SwitchIMDStartENOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    ImdStartEN = ENABLE;
  }
  else if (Sta == eSwSta_Off)
  {
    ImdStartEN = DISABLE;
  }
  LastSta = Sta;
}

/// <summary>
///  开关IMD检测使能
/// </summary>
/// <param name="EnumSwitchStatus">输入想要改变的状态</param>  
void SwitchIMDCheckENOnOff(EnumSwitchStatus Sta)
{
  static EnumSwitchStatus LastSta = eSwSta_Off;
  if (LastSta == Sta) return;
  if (Sta  == eSwSta_On)
  {
    ImdCheckEN = ENABLE;
  }
  else if (Sta == eSwSta_Off)
  {
    ImdCheckEN = DISABLE;
  }
  LastSta = Sta;
}





