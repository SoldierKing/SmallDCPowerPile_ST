#ifndef  __ADRV_IMDCHECK_H__
#define  __ADRV_IMDCHECK_H__

#include "Global.h"
#include "Param_PileVar.h"

extern void ImdCheckHandler(StructImdValue *pImdValue);
extern void SwitchIMDStartENOnOff(EnumSwitchStatus Sta);
extern void SwitchIMDCheckENOnOff(EnumSwitchStatus Sta);

extern int GetIMDCheckResult(StructImdValue *pImdValue);

#endif