#include "Global.h"

UnionPillarError PillarError = {0};
UnionChgError ChargeError = {0};

/******************************************************************************
����  : GetSystemTick
����  : ��ȡϵͳTick�����ⷵ��0����ʹĳЩ���ܷ���0Ӧ�ó���
����  : ��
����  : ϵͳTick
******************************************************************************/
u32 GetSystemTick(void)
{
  u32 Tick = osKernelSysTick();

  if(0 == Tick)
    return 1;
  return Tick;
}

//*����:BcDToHex
//*����: BCD��ת16����
//*����: bcd_data  Ҫת����BCD������(0-100)
//*����: ת�����16��������
u8 BcdToHex(u8 Bcd)
{
  return ((Bcd>>4)*10 + (Bcd&0x0F));
}

//*���ƣ� HexToBcd
//*���ܣ�  16����תBCD��
//*������  hex_data  Ҫת����16��������(0-100)
//*���أ�  ת�����BCD������
u8 HexToBcd(u8 Hex)
{
  u8 Temp;
  u8 Bcd;

  Temp = Hex%100;
  Bcd = (Temp/10<<4) | (Temp%10);

  return Bcd;
}
