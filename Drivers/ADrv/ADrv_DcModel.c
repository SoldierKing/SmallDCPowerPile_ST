#include "ADrv_DcModel.h"
#include "ADrv_BmsFrame.h"
#include "Drv_CAN.h"
#include "Param_UserConfig.h"
#include "Drv_CAN.h"
#include "ADrv_OneKeyStart.h"
#include "Thd_Charge.h"




#define TemCtrlSwith 0  //温控开关 1:打开，0:关闭

#if 1
#define DcModuleMaxVolt   (5000)//750V
#define DcModuleMinVolt   (2000)//200V
#define DcModuleMaxCurrt  (100) //10A
#define DcModuleMinCurrt  (5)  //1A
#define DcModuleSinMaxCurrt    (2000)

#else

#define DcModuleMaxVolt   (5000)//750V
#define DcModuleMinVolt   (2000)//250V
#define DcModuleMaxCurrt  (150) //15A
#define DcModuleMinCurrt  (10)  //1A
#endif


#define YINGKERUI

StructDcModuleAbility DcModuleAbility =
{
  .MaxVolt = DcModuleMaxVolt,
  .MinVolt = DcModuleMinVolt,
  .MaxCurrt = DcModuleMaxCurrt,
  .MinCurrt = DcModuleMinCurrt,
  .SingleMaxCurrt = DcModuleSinMaxCurrt,
};

StructDcModuleManage DcModuleManage;

StructDcModuleStatus DcModuleStatus[64];

StructDcModuleCtrl DcModuleCtrl;

void DcModuleRxIrqHandle(CAN_RxHeaderTypeDef RxMsg,u8 *Buff)
{

  StructDcModuleStatus Data;
  UnionINFYHeadId ID = {.Mult = 0,};
  ID.Mult =RxMsg.ExtId;
  
  if(ID.Disp.Cmd == 0x23)
  {
    
    Data.Id = ID.Disp.SourAddr & 0x0000003F;
    DcModuleManage.OnlineFlag |= ((long long)1<<Data.Id);
    DcModuleStatus[Data.Id].RecvTime= GetSystemTick();
    //模块直流侧电压
    if((Buff[0] ==0x11)&&(Buff[1] ==0x01 ))
    {
       DcModuleStatus[Data.Id].Volt = (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                      ((u32)Buff[6]<<8) + (u32)Buff[7]
                                       )/100;
    }
    //模块直流侧电流
    else if((Buff[0] ==0x11)&&(Buff[1] ==0x02 ) )
    {
       DcModuleStatus[Data.Id].Currt= (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                      ((u32)Buff[6]<<8) + (u32)Buff[7]
                                       )/100;
    }
    //模块状态0.1.2
    else if((Buff[0] ==0x11)&&(Buff[1] ==0x10 ) )
   {
      DcModuleStatus[Data.Id].State = (DcModuleStatus[Data.Id].State|0xFFFFFF)&
                                      ((((u32)Buff[5]<<16)+((u32)Buff[6]<<8) 
                                      + (u32)Buff[7])|0xFF000000);
    }
    //模块逆变状态0.1
    else if((Buff[0] ==0x11)&&(Buff[1] ==0x11 ) )
   {
      DcModuleStatus[Data.Id].State = (DcModuleStatus[Data.Id].State|0xFF000000)&
                                      (((u32)Buff[7]<<24)|0xFFFFFF);
      if(Buff[7]&0x01 ==0x01)
        DcModuleStatus[Data.Id].ModStatus = eSwSta_High;//逆变模式
      else
        DcModuleStatus[Data.Id].ModStatus = eSwSta_Low;//整流模式
   }
   //交流A相电压
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x01))
   {
     DcModuleStatus[Data.Id].VoltA = (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                     ((u32)Buff[6]<<8) + (u32)Buff[7]
                                      )/100;
   }
   //交流B相电压
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x02 ))
   {
     DcModuleStatus[Data.Id].VoltB = (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                     ((u32)Buff[6]<<8) + (u32)Buff[7]
                                      )/100;
   }
   //交流C相电压
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x03 ) )
   {
     DcModuleStatus[Data.Id].VoltC = (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                     ((u32)Buff[6]<<8) + (u32)Buff[7]
                                      )/100;
   }
   //交流A相电流
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x04 ) )
   {
     DcModuleStatus[Data.Id].CurA= (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                   ((u32)Buff[6]<<8) + (u32)Buff[7]
                                    )/100;
   }
   //交流B相电流
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x05 ) )
   {
     DcModuleStatus[Data.Id].CurB= (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                   ((u32)Buff[6]<<8) + (u32)Buff[7]
                                    )/100;
   }
   //交流C相电流
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x06 ) )
   {
     DcModuleStatus[Data.Id].CurC= (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                   ((u32)Buff[6]<<8) + (u32)Buff[7]
                                    )/100;
   }
   //交流总功率
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x08 ) )
   {
     DcModuleStatus[Data.Id].TotalPower= (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                         ((u32)Buff[6]<<8) + (u32)Buff[7]
                                          )/1000/100;
   }
  }

}

/**************************************************************************************************
** 函数名: CAN1_SendMessage
** 输　入: ulCanId----发送ID， ulCanDataA----发送数据的底4字节，  ulCanDataB----发送数据的高4字节
** 输　出: 参数读取状态
** 描  述: 通过CAN通道发送数据
** 日　期: 2010/08/19
**************************************************************************************************/
u8 DcModuleSendMsg(StructDcModuleSend *pMsg)
{
  CAN_TxHeaderTypeDef TxMsg;
  u32 pTxMailbox = 0;

  TxMsg.ExtId = pMsg->Id;
  TxMsg.IDE = CAN_ID_EXT;
  TxMsg.RTR = CAN_RTR_DATA;
  TxMsg.DLC = pMsg->Len;

  if (HAL_CAN_AddTxMessage(HcanDrv.hcan2,&TxMsg,pMsg->Data,&pTxMailbox) != HAL_OK)
    return FALSE;				//未发送成功，返回FALSE

  return TRUE;
}


void DcModuleManageProcess_INFY(void)
{
  
  static u32 CheckTick = 0;
  static u32 CheckTickStatus = 0;//模块状态
  static u32 CheckTickStatus1 = 0;//逆变状态
  static u32 CheckTickVol= 0;//模块直流测电压
  static u32 CheckTickCur= 0;//模块直流电流
  static u32 CheckTickACVol= 0;//模块输出交流电压
  static u32 CheckTickACur= 0;//模块输出交流电压
  static u8 StatusGetCnt = 0;
  UnionINFYHeadId ID;
  u8 i;
  long long Flag;
  u8 Count;
  u8 CountFail;
  u16 Currt;
  u16 Volt;  
  u16 CurrtA;
  u16 VoltA;
  u16 CurrtB;
  u16 VoltB;
  u16 CurrtC;
  u16 VoltC;
  u16 TotalPower;
  StructDcModuleSend DataSend;
  
	
  if((GetSystemTick() - CheckTickStatus) > 250)
  {
    for(i = 0; i<0x01; i++)
    {
      ID.Mult = 0 ;
      ID.Disp.SourAddr =0xF0;
      ID.Disp.DestAddr = i;
      ID.Disp.Cmd = 0x23;       //读取
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
	  
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x10;//模块状态0.1.2.
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);
      osDelay(2);
    }
    CheckTickStatus = GetSystemTick();
  }
  if((GetSystemTick() - CheckTickStatus1) > 500)
  {
    for(i = 0; i<1; i++)
    {
      ID.Mult = 0 ;
      ID.Disp.SourAddr =0xF0 ;
      ID.Disp.DestAddr = i;
      ID.Disp.Cmd = 0x23;  //读取//逆变状态
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
	  
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x11;
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);
      osDelay(2);
    }
    CheckTickStatus1 = GetSystemTick();
  }
  if((GetSystemTick() - CheckTickVol) > 1250)
  {
    for(i = 0; i<1; i++)
    {
      ID.Mult = 0 ;
      ID.Disp.SourAddr =0xF0 ;
      ID.Disp.DestAddr = i;
      ID.Disp.Cmd = 0x23;//读取
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
	  
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x01;//读取直流测电压
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);
      osDelay(2);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x02;//读取直流测电流
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);
      osDelay(2);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x08;//读取交流测总功率
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);
      osDelay(2);
    }
    CheckTickVol= GetSystemTick();
  }
  
  if((GetSystemTick() - CheckTickACVol) > 750)
  {
     for(i = 0; i<1; i++)
    {
      ID.Mult = 0 ;
      ID.Disp.SourAddr =0xF0 ;
      ID.Disp.DestAddr = i;
      ID.Disp.Cmd = 0x23;//读取
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
	  
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x01;//读取交流测A相电压
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//逆变状态
      osDelay(2);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x02;//读取交流测b相电压
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//逆变状态
      osDelay(5);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x03;//读取交流测c相电压
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//逆变状态
      osDelay(5);
    }
    CheckTickACVol= GetSystemTick();
  }

  if((GetSystemTick() - CheckTickACur) > 1000)
  {
     for(i = 0; i<1; i++)
    {
      ID.Mult = 0 ;
      ID.Disp.SourAddr =0xF0 ;
      ID.Disp.DestAddr = i;
      ID.Disp.Cmd = 0x23;//读取
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
	  
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x04;//读取交流测A相电流
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//逆变状态
      osDelay(5);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x05;//读取交流测b相电流
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//逆变状态
      osDelay(5);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x06;//读取交流测c相电流
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//逆变状态
      osDelay(5);
    }
    CheckTickACur= GetSystemTick();
  }
  if((GetSystemTick() - CheckTick) < 200)
    return;

  CheckTick = GetSystemTick();
  Flag = 1;
  Count = 0;
  CountFail = 0;
  for(i=0; i<64; i++)
  {
    if(Flag & DcModuleManage.OnlineFlag)
    {
      if((GetSystemTick() - DcModuleStatus[i].RecvTime) > 8000)
      {
        DcModuleManage.OnlineFlag &= ~Flag;
        DcModuleManage.ValidFlag &= ~Flag;
      }
      else
      {
        if(0x0 == (DcModuleStatus[i].State & 0xF07EAA21))//0x03FE))
        {
          DcModuleManage.ValidFlag |= Flag;
          Count++;
        }
	else
          CountFail++;
		  
      }
    }
    Flag = Flag << 1;
  }
  DcModuleManage.ValidNum = Count;
  Flag = 1;
  Currt = 0;
  Volt = 0;
  CurrtA = 0;
  VoltA = 0;
  CurrtB = 0;
  VoltB = 0;
  CurrtC = 0;
  VoltC = 0;
  TotalPower = 0;
  for(i=0; i<0x0d; i++)
  {
    if(Flag & DcModuleManage.ValidFlag)
    {
      Currt += DcModuleStatus[i].Currt;
      Volt += DcModuleStatus[i].Volt;
      CurrtA += DcModuleStatus[i].CurA;
      VoltA += DcModuleStatus[i].VoltA;
      CurrtB += DcModuleStatus[i].CurB;
      VoltB += DcModuleStatus[i].VoltB;
      CurrtC += DcModuleStatus[i].CurC;
      VoltC += DcModuleStatus[i].VoltC;
      TotalPower +=DcModuleStatus[i].TotalPower;
    }
    Flag = Flag << 1;
  }
  if(Currt<0x4E20)//任一项都不超过10000，超过则为异常
  DcModuleCtrl.ActualCurrt = Currt;
  else
  DcModuleCtrl.ActualCurrt = 0;	
  if(Volt<0x4E20)//任一项都不超过10000，超过则为异常
  DcModuleCtrl.ActualVolt = Volt/Count;
  else
  DcModuleCtrl.ActualVolt = 0;	
  
  if(CurrtA<0x4E20)//任一项都不超过10000，超过则为异常
    DcModuleCtrl.ActualCurrtA = CurrtA;
  else
    DcModuleCtrl.ActualCurrtA = 0;
  
  if(CurrtB<0x4E20)//任一项都不超过10000，超过则为异常
    DcModuleCtrl.ActualCurrtB = CurrtB;
  else
    DcModuleCtrl.ActualCurrtB = 0;
  if(CurrtC<0x4E20)//任一项都不超过10000，超过则为异常
    DcModuleCtrl.ActualCurrtC = CurrtC;
  else
    DcModuleCtrl.ActualCurrtC = 0;
  
  if(VoltA<0x4E20)//任一项都不超过10000，超过则为异常
    DcModuleCtrl.ActualVoltA= VoltA/Count;
  else
    DcModuleCtrl.ActualVoltA = 0;
  if(VoltB<0x4E20)//任一项都不超过10000，超过则为异常
    DcModuleCtrl.ActualVoltB= VoltB/Count;
  else
    DcModuleCtrl.ActualVoltB= 0;
  if(VoltC<0x4E20)//任一项都不超过10000，超过则为异常
    DcModuleCtrl.ActualVoltC= VoltC/Count;
  else
    DcModuleCtrl.ActualVoltC= 0;
  if(TotalPower <0x4E20)
    DcModuleCtrl.ActualPower = TotalPower;
  else
    DcModuleCtrl.ActualPower = 0;
  //if(0 == DcModuleManage.OnlineFlag)
    //MX_CAN2_Init();
}



void DcModuleCtrlProcess_INFY (void)
{
  static u32 CtrlTick = 0;
  static u8 LastFlag = 0;
  UnionINFYHeadId ID;

  StructDcModuleSend DataSend;
  u32 Temp;
  Temp = 0;
  if(LastFlag != DcModuleCtrl.CtrlFlag)
  {
    LastFlag = DcModuleCtrl.CtrlFlag;
    Temp++;
  }
  if((GetSystemTick() - CtrlTick) > 100)
  {
    CtrlTick = GetSystemTick();
    Temp++;
  }
  if(0 == Temp)
    return;

  if(DcModuleCtrl.CtrlFlag)
  {

    if(GetWorkMode() == Enum_ChargeWork)
    {
      
    if(DcModuleCtrl.NeedVolt > DcModuleAbility.MaxVolt)
      DcModuleManage.SetVolt = (DcModuleAbility.MaxVolt*100);
    else if(DcModuleCtrl.NeedVolt > DcModuleAbility.MinVolt)
      DcModuleManage.SetVolt = (DcModuleCtrl.NeedVolt*100);
    else
      DcModuleManage.SetVolt = (DcModuleAbility.MinVolt*100);

    if(DcModuleManage.ValidNum)
    {
      Temp = (DcModuleCtrl.NeedCurrt*100)/DcModuleManage.ValidNum;
      if((DcModuleCtrl.NeedCurrt*100) % DcModuleManage.ValidNum)
        Temp++;
    }
    else
      Temp = DcModuleManage.SetCurrt;
    if(Temp > (DcModuleAbility.MaxCurrt*100))
      Temp = (DcModuleAbility.MaxCurrt*100);
    else if(Temp < (DcModuleAbility.MinCurrt*100))
      Temp = (DcModuleAbility.MinCurrt*100);
    //限单个模块功率
    u32 temp1 = GetDcModuleVolt();//DcModuleCtrl.NeedVolt/10;
    if( temp1 == 0) 
      temp1 = 1;
    temp1 = (u32)UserParam.DcMaxPower*100*1000*10/temp1;
    if(Temp > temp1)
     Temp = temp1;
    ////////////////
    if(Temp <= DcModuleManage.SetCurrt)
      DcModuleManage.SetCurrt = Temp;
    else
    {
      DcModuleManage.SetCurrt += 500;
      if(DcModuleManage.SetCurrt >= Temp)
        DcModuleManage.SetCurrt = Temp;
    }
    if(DcModuleManage.SetCurrt > (DcModuleAbility.MaxCurrt*100))
      DcModuleManage.SetCurrt = (DcModuleAbility.MaxCurrt*100);
    else if(DcModuleManage.SetCurrt < (DcModuleAbility.MinCurrt*100))
      DcModuleManage.SetCurrt = (DcModuleAbility.MinCurrt*100);
	
    }
      ID.Mult = 0 ;
      ID.Disp.SourAddr =0xF0;
      ID.Disp.DestAddr = 0x3F;
      ID.Disp.Cmd = 0x24;//设置
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
      //判断是否为放电模式
      if(GetWorkMode() == Enum_DischargeWork)
      {
        
        DcModuleManage.SetVolt = DcModuleCtrl.ActualVolt*100;
        DcModuleManage.SetCurrt = UserParam.DcMaxPower*100*1000*10/DcModuleManage.SetVolt*100;//逆变模式
        
		if(DcModuleManage.SetCurrt >= DcModuleCtrl.NeedCurrt * 100 && CarTypeVar == NioCar_Type)
          DcModuleManage.SetCurrt = DcModuleCtrl.NeedCurrt * 100;

		if(DcModuleManage.SetCurrt > 16000)
		  DcModuleManage.SetCurrt = 16000;
		  
      }
		
      Temp = DcModuleManage.SetVolt ;
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x01;    //设置电压电流
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = (u8)(Temp>>24);
      DataSend.Data[5] = (u8)(Temp>>16);
      DataSend.Data[6] = (u8)(Temp>>8);;
      DataSend.Data[7] = (u8)(Temp>>0);;
      DcModuleSendMsg(&DataSend);  //逆变状态
      osDelay(2);
      Temp = DcModuleManage.SetCurrt;
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x02;//设置电压电流
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = (u8)(Temp>>24);;
      DataSend.Data[5] = (u8)(Temp>>16);;
      DataSend.Data[6] = (u8)(Temp>>8);;
      DataSend.Data[7] = (u8)(Temp>>0);;
      DcModuleSendMsg(&DataSend);//逆变状态
      osDelay(2);
      if(
	 ((DcModuleStatus[0].ModStatus == eSwSta_High)&&(GetWorkMode() == Enum_DischargeWork))
	  ||((DcModuleStatus[0].ModStatus == eSwSta_Low)&&(GetWorkMode() == Enum_ChargeWork))
	 )
     {
       DataSend.Id = ID.Mult;
       DataSend.Len = 8;
       DataSend.Data[0] = 0x11;
       DataSend.Data[1] = 0x10;
       DataSend.Data[2] = 0x00;
       DataSend.Data[3] = 0x00;
       DataSend.Data[4] = 0x00;
       DataSend.Data[5] = 0x00;
       DataSend.Data[6] = 0x00;
       DataSend.Data[7] = 0xA0;//开机
       DcModuleSendMsg(&DataSend);//逆变状态
       osDelay(5);
     }
     else 
     {
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x10;
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      if(GetWorkMode() == Enum_DischargeWork)
        DataSend.Data[7] = 0xA2;//更改为离网逆变状态
      else
	DataSend.Data[7] = 0xA0;//更改为整流模式
      DcModuleSendMsg(&DataSend);
      osDelay(5);
     }     
  }
  else
  {
      
      ID.Mult = 0 ;
      ID.Disp.SourAddr =0xF0;
      ID.Disp.DestAddr = 0x3F;
      ID.Disp.Cmd = 0x24;//设置
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x01;//设置电压电流
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//逆变状态
      osDelay(5);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x02;//设置电压电流
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//逆变状态
      osDelay(5);

      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x10;
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0xA1;//关机
      DcModuleSendMsg(&DataSend);//逆变状态
      osDelay(5);
      if((DcModuleStatus[0].ModStatus == eSwSta_Low)&&(GetWorkMode() == Enum_DischargeWork))
      {
	 DataSend.Id = ID.Mult;
         DataSend.Len = 8;
         DataSend.Data[0] = 0x21;
         DataSend.Data[1] = 0x10;
         DataSend.Data[2] = 0x00;
         DataSend.Data[3] = 0x00;
         DataSend.Data[4] = 0x00;
         DataSend.Data[5] = 0x00;
         DataSend.Data[6] = 0x00;
         DataSend.Data[7] = 0xA2;//更改为逆变状态
         DcModuleSendMsg(&DataSend);//逆变状态
         osDelay(5);
      }
      if((DcModuleStatus[0].ModStatus == eSwSta_High)&&(GetWorkMode() == Enum_ChargeWork))
      {
	 DataSend.Id = ID.Mult;
         DataSend.Len = 8;
         DataSend.Data[0] = 0x21;
         DataSend.Data[1] = 0x10;
         DataSend.Data[2] = 0x00;
         DataSend.Data[3] = 0x00;
         DataSend.Data[4] = 0x00;
         DataSend.Data[5] = 0x00;
         DataSend.Data[6] = 0x00;
         DataSend.Data[7] = 0xA0;   //更改为充电模式
         DcModuleSendMsg(&DataSend);
         osDelay(5);
   }
  }
}


void DcModuleSet(EnumSwitchStatus Set, u16 Volt, u16 Currt)
{
  
  if (Currt> DcModuleAbility.SingleMaxCurrt)
    Currt = DcModuleAbility.SingleMaxCurrt;
  
  if(eSwSta_Off == Set)
  {
    DcModuleCtrl.CtrlFlag = 0;
    DcModuleCtrl.NeedVolt = Volt;
    DcModuleCtrl.NeedCurrt = Currt;
  }
  else
  {
    DcModuleCtrl.CtrlFlag = 1;
    DcModuleCtrl.NeedVolt = Volt;
    DcModuleCtrl.NeedCurrt = Currt;
  }
}

//获取直流侧电压
u16 GetDcModuleVolt(void)
{
  //return DcModuleCtrl.ActualVolt*201/200;
  return (u16)((u32)DcModuleCtrl.ActualVolt*UserParam.VoltRatio/1000);
}

//获取直流侧电流
u16 GetDcModuleCurrt(void)
{
  //return DcModuleCtrl.ActualCurrt*202/200;
  return (u16)((u32)DcModuleCtrl.ActualCurrt*UserParam.CurrtRatio/1000);
}

//获取模块设置电压
u16 GetDcModuleVoltSet(void)
{
  //return DcModuleManage.SetVolt/100;
  return DcModuleCtrl.NeedVolt;
}

//获取模块设置电流
u16 GetDcModuleCurrtSet(void)
{
  //return DcModuleManage.SetVolt*DcModuleManage.ValidNum/100;
  return DcModuleCtrl.NeedCurrt;
}

//获取交流测电压
u16 GetDcModuleVoltA(void)
{
  return DcModuleCtrl.ActualVoltA;
}

//获取交流侧电流
u16 GetDcModuleCurrtA(void)
{
  return DcModuleCtrl.ActualCurrtA;
}

u16 GetDcModuleVoltB(void)
{
  return DcModuleCtrl.ActualVoltB;
}

u16 GetDcModuleCurrtB(void)
{
  return DcModuleCtrl.ActualCurrtB;
} 

u16 GetDcModuleCurrtC(void)
{
  return DcModuleCtrl.ActualCurrtC;
}

u16 GetDcModuleVoltC(void)
{
  return DcModuleCtrl.ActualVoltC;
}


/* the system main thread */
void DcModule(void const *parameter)
{
  osDelay(200);

  MX_CAN2_Init();

  DcModuleAbility.MaxVolt = UserParam.DcMaxVolt;
  DcModuleAbility.MinVolt = UserParam.DcMinVolt;
  DcModuleAbility.MaxCurrt = UserParam.DcMaxCurrt;
  DcModuleAbility.MinCurrt = UserParam.DcMinCurrt;
  DcModuleAbility.SingleMaxCurrt= UserParam.SingleMaxCurrt;
  
  HcanDrv.CanRecvHandler2 = DcModuleRxIrqHandle;
  while(1)
  {
    
    DcModuleManageProcess_INFY();
    osDelay(15);
    DcModuleCtrlProcess_INFY();
    osDelay(15);
  }
}


u8 DcModuleInit(void)
{

  osThreadDef(DcModule, DcModule, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE);
  if(NULL == osThreadCreate(osThread(DcModule), NULL))
    return FALSE;

  return TRUE;
}

