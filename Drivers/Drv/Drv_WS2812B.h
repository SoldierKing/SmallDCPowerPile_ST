
#ifndef __DRV_WS2812B_H_
#define	__DRV_WS2812B_H_

#include "Global.h"


//WS2812B IO 定义  注意初始化APB时钟也要修改
#define WS2812B_PORT		GPIOB
#define WS2812B_PIN		GPIO_PIN_0

#if 1
#define WS2812B_Hi()		WS2812B_PORT->BSRR=WS2812B_PIN	
#define WS2812B_Low()		WS2812B_PORT->BRR =WS2812B_PIN 

#else
#define WS2812B_Low()		WS2812B_PORT->BSRR=WS2812B_PIN	
#define WS2812B_Hi()		WS2812B_PORT->BRR =WS2812B_PIN 

#endif

//颜色
typedef struct color{
	u8 G;
	u8 R;
	u8 B;
}Color_TypeDef;


typedef enum
{
  Lighting,
  Blinking,
  Breathing,
}EnumLedSwitchStatus;

typedef enum
{
  Blue,
  Green,
  Red,
}EnumDisplay;


typedef enum
{

  BreatheUp = 0,
  BreatheKeepOn,
  BreatheDown,
  BreatheKeepOff,

}LedBreatheStepEnem;


typedef struct
{

  Color_TypeDef RGB;
  Color_TypeDef RGBTemp;
  u32 UpTime;
  u32 KeepOnTime;
  u32 DownTime;
  u32 KeepOffTime;
  u32 UpTickB;
  u32 UpTickR;
  u32 UpTickG;
  u32 KeepOnTick;
  u32 DownTickB;
  u32 DownTickR;
  u32 DownTickG;
  u32 KeepOffTick;
  LedBreatheStepEnem BreatheStep;
  

}WS2812BCfgStruct;

typedef void (*pWS2812B_Lighting)(Color_TypeDef *pColor,u8 num);


//extern void LedEffectSwitch(EnumLedSwitchStatus Sta,EnumDisplay Color);
extern void WS2812B_Init(void);
extern void WS2812B_WriteColor(Color_TypeDef *pColor);
extern void WS2812B_Blue(void);
extern void WS2812B_Green(void);
extern void WS2812B_Red(void);
extern void WS2812B_Black(void);
extern void WS2812B_White(void);
extern void WS2812B_Lighting(Color_TypeDef *pColor,u8 num);
extern void LedWs2812Breathe(pWS2812B_Lighting WS2812B_Lighting,WS2812BCfgStruct *pWS2812BCfgStruct);


#endif /* __WS2812B_H */



