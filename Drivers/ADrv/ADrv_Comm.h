#ifndef __ADRV_COMM_H__
#define __ADRV_COMM_H__
#include "Global.h"
#include "Drv_GPIO.h"

extern GPIO_PinState GetCC1Sta(void);

extern void SwitcBMSPowerENOnOff(EnumSwitchStatus Sta);
extern void SwitchEleLockOnOff(EnumSwitchStatus Sta);
extern void SwitcHighRlyOnOff(EnumSwitchStatus Sta);
extern void SwitcRunLedOnOff(EnumSwitchStatus Sta);
extern void SwitcBoostENOnOff(EnumSwitchStatus Sta);
extern GPIO_PinState GetBoostENSwitchSta(void);
extern void SwitcIMDPnRlyOnOff(EnumSwitchStatus Sta);
extern void SwitcIMDPosRlyOnOff(EnumSwitchStatus Sta);
extern void SwitcIMDNegRlyOnOff(EnumSwitchStatus Sta);
extern GPIO_PinState GetKeyBtnSta(void);
extern GPIO_PinState GetHRlySta(void);
extern void SwitcStartOnOnOff(EnumSwitchStatus Sta);
extern GPIO_PinState GetOBSBtnSta(void);
extern GPIO_PinState GetStartOnBtnSta(void);
extern GPIO_PinState GetIMDHPSta(void);
extern GPIO_PinState GetIMDHNSta(void);
extern GPIO_PinState GetIMDHPESta(void);
extern void SwitcIMDPRlyOnOff(EnumSwitchStatus Sta);
extern void SwitcIMDPERlyOnOff(EnumSwitchStatus Sta);
extern void SwitcIMDNRlyOnOff(EnumSwitchStatus Sta);
extern GPIO_PinState GetVbatReverseSta(void);
extern void SwitchELeOnOffHandler(EnumSwitchStatus Sta,u8 RecTimes);
extern GPIO_PinState GetEleLockSta(void);
extern EnumSwitchStatus GetSwitchEleLockSta(void);


#endif