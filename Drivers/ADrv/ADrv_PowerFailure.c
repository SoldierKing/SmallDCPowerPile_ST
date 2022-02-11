#include "ADrv_PowerFailure.h"
#include "Thd_Charge.h"
#include "ADrv_AdcVolt.h"
#include "ADrv_Comm.h"
#include "ADrv_CompatibleInterface.h"
#include "ADrv_DcModel.h"
#include "ADrv_OneKeyStart.h"


#define   PowerOffVoltLimit   (11.00*100)
#define   PowerOffACVoltLimit (150.0*10)
#define   PowerOffDCVoltLimit (200.0*10)

void PowerFailureCheck(void)
{

  static u32 PowerFailureTick = 0;
  static u32 DisChgRunTick = 0;
  if (PowerFailureTick == 0) PowerFailureTick =  GetSystemTick();
  if (GetSystemTick() - PowerFailureTick < 5000) return;
  
  if (DcModuleManage.OnlineFlag == 0) return;

  if (GetWorkMode() == Enum_ChargeWork)
  {
      if (DcModuleCtrl.ActualVoltA < PowerOffACVoltLimit && GetSwitchEleLockSta() == eSwSta_On)
      {
    
         ChargeEnable = 0;
         DisChargeEnable = 0;
     	//电子锁关闭
        //LockSet(eSwSta_Off);
         LockSet(eSwSta_Off);
      }
  }
  else if (GetWorkMode() == Enum_DischargeWork && ChargeCtrl.CurProcess == eChgPro_DisChgRun)
  {
    if (DisChgRunTick == 0) DisChgRunTick = GetSystemTick();
	if (DisChgRunTick != 0 && GetSystemTick() - DisChgRunTick > 5000)
	{
	   //DisChgRunTick = 0;
       if (AdcVoltBuf[EnumVddc_AD3] < PowerOffDCVoltLimit && GetSwitchEleLockSta() == eSwSta_On)
         {
   
   	     //SwitcBoostENOnOff(eSwSta_On);
            ChargeEnable = 0;
            DisChargeEnable = 0;
        	//电子锁关闭
           //LockSet(eSwSta_Off);
             LockSet(eSwSta_Off);
   		//SwitcBoostENOnOff(eSwSta_Off);
         }
	}
  }
 
}


