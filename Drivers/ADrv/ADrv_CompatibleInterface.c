#include "ADrv_CompatibleInterface.h"
#include "ADrv_Comm.h"
#include "Drv_CAN.h"



EnumSwitchStatus CcStatusRead(void)
{
  return (EnumSwitchStatus)GetCC1Sta();
}

void BmsPowerSet(EnumSwitchStatus Set)
{
  SwitcBMSPowerENOnOff(Set);
}

void LockSet(EnumSwitchStatus Set)
{
  SwitchEleLockOnOff(Set);
}

void DcOutSet(EnumSwitchStatus Set)
{

  SwitcHighRlyOnOff(Set);

}

void BmsCanInit(void)
{
  MX_CAN1_Init();
}


