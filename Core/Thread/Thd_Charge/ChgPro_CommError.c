
#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"


void ChgProCommError(StructChargeCtrl *pChargeCtrl)
{
  
  StructPgnRecv PgnRecv;
  u32 CurrtCheckTick = 0;
  u32 DrainResistTick = 0;
  u32 CcStatusChkTick = 0;
  u8 ChgEndFlag = 0;
  u8 GunOutFlag = 0;

  u32 CST_SendTick = 0;
  u32 CEM_SendTick = 0;
  u32 CemSendCnt = 0;
  u32 CstSendCnt = 0;

  BsdMsg.Valid = 0;
  ChargeTimerSet(eSwSta_Off);
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);

  //DcOutSet(eSwSta_On);
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  CstSendCnt = 0;
  CST_Send(&CstMsg);
  CST_SendTick = GetSystemTick();

  CemSendCnt = 0;
  CEM_Send(&CemMsg);
  CEM_SendTick = GetSystemTick();

  CurrtCheckTick = GetSystemTick();
  DrainResistTick = 0;

  while(1)
  {
    if((0 != CST_SendTick) && ((GetSystemTick() - CST_SendTick) > 10))
    {
      CST_Send(&CstMsg);
      CST_SendTick = GetSystemTick();
      CstSendCnt++;
      if(CstSendCnt > 50)
        CST_SendTick = 0;
    }

    if((0 != CEM_SendTick) && ((GetSystemTick() - CEM_SendTick) > 250))
    {
      if(0 != CemMsg.Cem.Mult)//有通信故障
        CEM_Send(&CemMsg);
      CEM_SendTick = GetSystemTick();
      CemSendCnt++;
      if(CemSendCnt > 5)
      {
        BmsPowerSet(eSwSta_Off);
        CEM_SendTick = 0;
      }
    }

    ChgPgnRecv(&PgnRecv);

    if(0 != CurrtCheckTick)
    {
      if((GetDcModuleCurrt() < 50) || ((GetSystemTick() - CurrtCheckTick) > 120))
      {//小于5A
        DcOutSet(eSwSta_Off);//k1 k2
        DrainResistTick = GetSystemTick();
        CurrtCheckTick = 0;
      }
    }

    if(0 != DrainResistTick)
    {
      if((GetDcModuleVolt() < 600) || ((GetSystemTick() - DrainResistTick) > 4000))
      {//小于60V
        ChgEndFlag = 1;
        DrainResistTick = 0;
      }
    }

    if((0 != ChgEndFlag) && (CemSendCnt > 5))
    {
      LockSet(eSwSta_Off);
    }

    if((0 != ChgEndFlag) && (CemSendCnt > 5))
    {//外部控制结束充电
    
      if (GunOutFlag)
      {
    
        pChargeCtrl->CurProcess = eChgPro_ChgIdle;
        pChargeCtrl->Param = 0;
      }
    }

    if(eSwSta_On != CcStatusRead())
    {
      if(0 == CcStatusChkTick)
        CcStatusChkTick = GetSystemTick();
    }
    else
      CcStatusChkTick = 0;
	
    if((0 != CcStatusChkTick) && ((GetSystemTick() - CcStatusChkTick) > CC_CheckTick))
    {
      
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
      GunOutFlag = 1;
      CcStatusChkTick = 0;
    }

    if(0 != PillarError.Value.Emergency)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
    }
    if(0 != PillarError.Value.Door)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
    }
    if(0 != PillarError.Value.Contactor)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
    }
    if(0 != PillarError.Value.UniContactor)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
    }
    if(0 != PillarError.Value.InVolt)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
    }
    if(0 != PillarError.Value.NoDcmod)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
    }
    if(0 != PillarError.Value.Meter)
    {
      DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);
      DcOutSet(eSwSta_Off);
    }
    

    if(eChgPro_CommError != pChargeCtrl->CurProcess)
      return;

    osDelay(5);
  }
}

