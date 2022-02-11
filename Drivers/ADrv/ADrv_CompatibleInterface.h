#ifndef  __ADRV_COMPATIBLEINTERFACE_H__
#define  __ADRV_COMPATIBLEINTERFACE_H__

#include "Global.h"

extern EnumSwitchStatus CcStatusRead(void);
extern void BmsPowerSet(EnumSwitchStatus Set);
extern void LockSet(EnumSwitchStatus Set);
extern void DcOutSet(EnumSwitchStatus Set);
extern void BmsCanInit(void);

#endif