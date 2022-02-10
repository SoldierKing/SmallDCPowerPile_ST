#ifndef  __ADRV_KEYDOWN_H__
#define  __ADRV_KEYDOWN_H__

#include "Global.h"


typedef enum
{

  Enum_KeyShortPress,
  Enum_KeyLongPress,
  Enum_KeyDown,
  Enum_KeyUp,
  
}EnumKeyType;


typedef struct
{

  //��Ҫ��̬��ֵIO״̬
  GPIO_PinState IOSta;
  GPIO_PinState IsDown;
  GPIO_PinState IsUp;
  u32 BtnTick;
  u32 KeyDownTime;
  u32 KeyShortPressTime;
  u32 KeyLongPressTime;
  u8  OverLongPressTime;
  
}StructKeyPressCfg;


extern EnumKeyType GetKeyPressType(StructKeyPressCfg *pKeyPressCfg);
extern void GetKeyKeepDownTime(StructKeyPressCfg *pKeyPressCfg);


#endif