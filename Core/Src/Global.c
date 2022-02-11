#include "Global.h"

UnionPillarError PillarError = {0};
UnionChgError ChargeError = {0};

/******************************************************************************
函数  : GetSystemTick
描述  : 获取系统Tick，避免返回0，致使某些不能返回0应用出错
参数  : 无
返回  : 系统Tick
******************************************************************************/
u32 GetSystemTick(void)
{
  u32 Tick = osKernelSysTick();

  if(0 == Tick)
    return 1;
  return Tick;
}

//*名称:BcDToHex
//*功能: BCD码转16进制
//*参数: bcd_data  要转换的BCD码数据(0-100)
//*返回: 转换后的16进制数据
u8 BcdToHex(u8 Bcd)
{
  return ((Bcd>>4)*10 + (Bcd&0x0F));
}

//*名称： HexToBcd
//*功能：  16进制转BCD码
//*参数：  hex_data  要转换的16进制数据(0-100)
//*返回：  转换后的BCD码数据
u8 HexToBcd(u8 Hex)
{
  u8 Temp;
  u8 Bcd;

  Temp = Hex%100;
  Bcd = (Temp/10<<4) | (Temp%10);

  return Bcd;
}
