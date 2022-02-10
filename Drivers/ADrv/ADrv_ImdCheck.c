#include "ADrv_ImdCheck.h"
#include "ADrv_AdcVolt.h"
#include "ADrv_Comm.h"
#include <math.h>

//��������
int GetIMDCheckResult(StructImdValue *pImdValue);

typedef struct
{
  
  u16 Vx1;
  u16 Vx2;
  u16 Vy1;
  u16 Vy2;
}StructImdVolt;


//��׼��ѹֵ
#define  RefVolt                      (float)(3.3)
#define  R1                           (float)(270*5)
#define  R3                           (float)(22*5)
//Rx��ĸ
#define  RxDenom(Vx1,Vx2,Vy1,Vy2)     (float)(R3*Vx2*(Vx2*Vy1-Vx1*Vy2));
//Rx����
#define  RxMolecule(Vx1,Vx2,Vy1,Vy2)  (float)(Vy2*(Vx2*Vy1+Vx1*Vx2));
//Ry��ĸ
#define  RyDenom(Vx1,Vx2,Vy1,Vy2)     (float)(R3*(Vx2*Vy1-Vx1*Vy2));
//Ry����
#define  RyMolecule(Vx1,Vx2,Vy1,Vy2)  (float)(Vx1*Vy2+Vx1*Vx2);


//IMD���ʹ��
static FunctionalState ImdCheckEN = DISABLE;
//IMD����ʹ��
static FunctionalState ImdStartEN = DISABLE;
//Vx Vy��ѹ���ݽṹ
static StructImdVolt   ImdVxVolt;



/// <summary>
///  Imd��Ե����߳�
/// </summary>
/// <param name="StructImdValue *pImdValue">����IMD���ݽṹָ��</param>  
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
  //����imd��Ե��⸨Դ
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
    //��ȡVx1 Vy1��ѹֵ
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
    //��ȡVx2 Vy2��ѹֵ
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
    //����������Ե����
    float Temp = 0;
    Temp = RxMolecule((float)(ImdVxVolt.Vx1/10),(float)(ImdVxVolt.Vx2/10),
                      (float)(ImdVxVolt.Vy1/10),(float)(ImdVxVolt.Vy2/10));
    Temp = Temp / RxDenom((float)(ImdVxVolt.Vx1/10),(float)(ImdVxVolt.Vx2/10),
                      (float)(ImdVxVolt.Vy1/10),(float)(ImdVxVolt.Vy2/10));
    
    pImdValue->PositResist = (u16)(1/(Temp - (1/R1)));
    
    //���㸺����Ե����
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
///  ����IMD��⸨Դ����ʹ��
/// </summary>
/// <param name="EnumSwitchStatus">������Ҫ�ı��״̬</param>  
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
///  ����IMD���ʹ��
/// </summary>
/// <param name="EnumSwitchStatus">������Ҫ�ı��״̬</param>  
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





