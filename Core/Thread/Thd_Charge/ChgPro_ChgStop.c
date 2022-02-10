
#include "Thd_Charge.h"

#include "ADrv_BmsFrame.h"
#include "ADrv_Comm.h"
#include "ADrv_DcModel.h"
#include "Drv_CAN.h"
#include "ADrv_CompatibleInterface.h"
#include "Param_PileVar.h"


void ChgProChgStop(StructChargeCtrl *pChargeCtrl)
{
  StructPgnRecv PgnRecv;
  u32 CST_SendTick = 0;
  u32 CSD_SendTick = 0;
  u32 CEM_SendTick = 0;
  u32 CurrtCheckTick = 0;
  u32 DrainResistTick = 0;
  u32 CcStatusChkTick = 0;
  u8 ChgEndFlag = 0;
  u8 GunOutFlag = 0;

  u32 Bst_RecvTick= 0;
  u32 Bsd_RecvTick= 0;
  u32 CsdCemSendCnt = 0;

  BsdMsg.Valid = 0;
  //BmsCanInit();
  //osDelay(5);

  ChargeTimerSet(eSwSta_Off);
  BmsPowerSet(eSwSta_On);
  LockSet(eSwSta_On);
  //DcOutSet(eSwSta_On);//有可能是充通信失败等待过来，保存原来状态
  DcModuleSet(eSwSta_Off, DcModuleAbility.MinVolt, DcModuleAbility.MinCurrt*DcModuleManage.ValidNum);

  CST_Send(&CstMsg);
  CST_SendTick = GetSystemTick();

  CemMsg.Cem.Value.Bst = 0;
  CemMsg.Cem.Value.Bsd = 0;
  CEM_SendTick = 0;

  CsdCemSendCnt = 0;
  CurrtCheckTick = GetSystemTick();
  DrainResistTick = 0;

  CSD_SendTick = 0;

  if(BstMsg.Valid)
    Bst_RecvTick = 0;
  else
    Bst_RecvTick = GetSystemTick();
  Bsd_RecvTick = GetSystemTick();

  while(1)
  {
    
    if((0 != CST_SendTick) && ((GetSystemTick() - CST_SendTick) > 8))
    {
      CST_Send(&CstMsg);
      CST_SendTick = GetSystemTick();
    }

    if((0 != CSD_SendTick) && ((GetSystemTick() - CSD_SendTick) > 250))//时间到
    {
      CsdMsg.ChgMin = GetChgMin();
      //StopChgEnergy  = MeterValue.Energy;
      //CsdMsg.ChgEnergy = (StopChgEnergy - StartChgEnergy)/10;
      CSD_Send(&CsdMsg);
      CSD_SendTick = GetSystemTick();
      CsdCemSendCnt++;
      if(CsdCemSendCnt > 5)
      {
        BmsPowerSet(eSwSta_Off);
        CSD_SendTick = 0;
      }
    }

    if((0 != CEM_SendTick) && ((GetSystemTick() - CEM_SendTick) > 250))
    {
    
      CEM_SendTick = GetSystemTick();
      if(0 != CemMsg.Cem.Mult) //有通信故障
      {
        CEM_Send(&CemMsg);
        CsdCemSendCnt++;
        if(CsdCemSendCnt > 5)
        {
          BmsPowerSet(eSwSta_Off);
          CEM_SendTick = 0;
        }
      }
    }

    ChgPgnRecv(&PgnRecv);

    if(0 != Bst_RecvTick)
    {
      if(0 != BstMsg.Valid)
      {
        CemMsg.Cem.Value.Bst = 0;
        Bst_RecvTick = 0;
      }
      else if((GetSystemTick() - Bst_RecvTick) > MsgTimeout_5S)
      {
        CemMsg.Cem.Value.Bst = 1;
        CEM_SendTick = GetSystemTick();
        Bst_RecvTick = 0;
      }
    }

    if(0 != Bsd_RecvTick)
    {
      if(0 != BsdMsg.Valid)
      {
        CemMsg.Cem.Value.Bsd = 0;
        CSD_SendTick = GetSystemTick();
        Bsd_RecvTick = 0;
        CST_SendTick = 0;
      }
      else if((GetSystemTick() - Bsd_RecvTick) > MsgTimeout_10S)
      {
        if(0 == CemMsg.Cem.Value.Bst)
        {
          CemMsg.Cem.Value.Bsd = 1;
          CEM_SendTick = GetSystemTick();
        }
        Bsd_RecvTick = 0;
        CST_SendTick = 0;
      }
    }

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
      if((GetDcModuleVolt() < 600) || ((GetSystemTick() - DrainResistTick) > 4000))//小于60V
      {
        ChgEndFlag = 1;
        DrainResistTick = 0;
      }
    }

    if((0 != ChgEndFlag) && (CsdCemSendCnt > 5))
    {
      LockSet(eSwSta_Off);
      //外部控制结束充电

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

    if(eChgPro_ChgStop != pChargeCtrl->CurProcess)
      return;

    osDelay(5);
  }
}





