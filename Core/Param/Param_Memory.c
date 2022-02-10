#include "Param_Memory.h"
#include "ADrv_SpiFlash.h"
#include "Param_UserConfig.h"


u8 sFlash_SaveUserParam(u8* pData, u16 Len)
{
  u16 i;
  u8 ChkSum;

  ChkSum = 0;
  for(i=0; i<Len; i++)
    ChkSum += pData[i];
  ChkSum = ~ChkSum;

  sFLASH_EraseSector(UserParamAddr);

  sFLASH_WriteBuffer(pData, UserParamAddr, Len);
  sFLASH_WriteBuffer(&ChkSum, UserParamAddr+4095, 1);

  sFLASH_EraseSector(UserParamBakAddr);

  sFLASH_WriteBuffer(pData, UserParamBakAddr, Len);
  sFLASH_WriteBuffer(&ChkSum, UserParamBakAddr+4095, 1);

  return TRUE;
}

u8 GetUserParam(u8* pData, u16 Len)
{
  u8 Temp;
  u8 ChkSum;
  u16 i;

  sFLASH_ReadBuffer(pData, UserParamBakAddr, Len);
  ChkSum = 0;
  for(i=0; i<Len; i++)
    ChkSum += pData[i];
  ChkSum = ~ChkSum;

  sFLASH_ReadBuffer(&Temp, UserParamBakAddr+4095, 1);
  if(ChkSum == Temp)
    return TRUE;

  sFLASH_ReadBuffer(pData, UserParamAddr, Len);
  ChkSum = 0;
  for(i=0; i<Len; i++)
    ChkSum += pData[i];
  ChkSum = ~ChkSum;

  sFLASH_ReadBuffer(&Temp, UserParamAddr+4095, 1);
  if(ChkSum == Temp)
    return TRUE;

  return FALSE;
}


StructUserConfig Temp;
void FlashTest(void)
{

  sFlash_SaveUserParam((u8*)&UserParam,sizeof(UserParam));
  osDelay(50);
  GetUserParam((u8*)&Temp,sizeof(Temp));
  
}

