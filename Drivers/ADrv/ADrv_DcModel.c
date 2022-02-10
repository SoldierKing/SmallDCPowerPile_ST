#include "ADrv_DcModel.h"
#include "ADrv_BmsFrame.h"
#include "Drv_CAN.h"
#include "Param_UserConfig.h"
#include "Drv_CAN.h"
#include "ADrv_OneKeyStart.h"
#include "Thd_Charge.h"




#define TemCtrlSwith 0  //�¿ؿ��� 1:�򿪣�0:�ر�

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
    //ģ��ֱ�����ѹ
    if((Buff[0] ==0x11)&&(Buff[1] ==0x01 ))
    {
       DcModuleStatus[Data.Id].Volt = (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                      ((u32)Buff[6]<<8) + (u32)Buff[7]
                                       )/100;
    }
    //ģ��ֱ�������
    else if((Buff[0] ==0x11)&&(Buff[1] ==0x02 ) )
    {
       DcModuleStatus[Data.Id].Currt= (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                      ((u32)Buff[6]<<8) + (u32)Buff[7]
                                       )/100;
    }
    //ģ��״̬0.1.2
    else if((Buff[0] ==0x11)&&(Buff[1] ==0x10 ) )
   {
      DcModuleStatus[Data.Id].State = (DcModuleStatus[Data.Id].State|0xFFFFFF)&
                                      ((((u32)Buff[5]<<16)+((u32)Buff[6]<<8) 
                                      + (u32)Buff[7])|0xFF000000);
    }
    //ģ�����״̬0.1
    else if((Buff[0] ==0x11)&&(Buff[1] ==0x11 ) )
   {
      DcModuleStatus[Data.Id].State = (DcModuleStatus[Data.Id].State|0xFF000000)&
                                      (((u32)Buff[7]<<24)|0xFFFFFF);
      if(Buff[7]&0x01 ==0x01)
        DcModuleStatus[Data.Id].ModStatus = eSwSta_High;//���ģʽ
      else
        DcModuleStatus[Data.Id].ModStatus = eSwSta_Low;//����ģʽ
   }
   //����A���ѹ
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x01))
   {
     DcModuleStatus[Data.Id].VoltA = (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                     ((u32)Buff[6]<<8) + (u32)Buff[7]
                                      )/100;
   }
   //����B���ѹ
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x02 ))
   {
     DcModuleStatus[Data.Id].VoltB = (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                     ((u32)Buff[6]<<8) + (u32)Buff[7]
                                      )/100;
   }
   //����C���ѹ
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x03 ) )
   {
     DcModuleStatus[Data.Id].VoltC = (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                     ((u32)Buff[6]<<8) + (u32)Buff[7]
                                      )/100;
   }
   //����A�����
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x04 ) )
   {
     DcModuleStatus[Data.Id].CurA= (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                   ((u32)Buff[6]<<8) + (u32)Buff[7]
                                    )/100;
   }
   //����B�����
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x05 ) )
   {
     DcModuleStatus[Data.Id].CurB= (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                   ((u32)Buff[6]<<8) + (u32)Buff[7]
                                    )/100;
   }
   //����C�����
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x06 ) )
   {
     DcModuleStatus[Data.Id].CurC= (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                   ((u32)Buff[6]<<8) + (u32)Buff[7]
                                    )/100;
   }
   //�����ܹ���
   else if((Buff[0] ==0x21)&&(Buff[1] ==0x08 ) )
   {
     DcModuleStatus[Data.Id].TotalPower= (((u32)Buff[4]<<24) + ((u32)Buff[5]<<16)+
                                         ((u32)Buff[6]<<8) + (u32)Buff[7]
                                          )/1000/100;
   }
  }

}

/**************************************************************************************************
** ������: CAN1_SendMessage
** �䡡��: ulCanId----����ID�� ulCanDataA----�������ݵĵ�4�ֽڣ�  ulCanDataB----�������ݵĸ�4�ֽ�
** �䡡��: ������ȡ״̬
** ��  ��: ͨ��CANͨ����������
** �ա���: 2010/08/19
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
    return FALSE;				//δ���ͳɹ�������FALSE

  return TRUE;
}


void DcModuleManageProcess_INFY(void)
{
  
  static u32 CheckTick = 0;
  static u32 CheckTickStatus = 0;//ģ��״̬
  static u32 CheckTickStatus1 = 0;//���״̬
  static u32 CheckTickVol= 0;//ģ��ֱ�����ѹ
  static u32 CheckTickCur= 0;//ģ��ֱ������
  static u32 CheckTickACVol= 0;//ģ�����������ѹ
  static u32 CheckTickACur= 0;//ģ�����������ѹ
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
      ID.Disp.Cmd = 0x23;       //��ȡ
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
	  
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x10;//ģ��״̬0.1.2.
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
      ID.Disp.Cmd = 0x23;  //��ȡ//���״̬
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
      ID.Disp.Cmd = 0x23;//��ȡ
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
	  
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x01;//��ȡֱ�����ѹ
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
      DataSend.Data[1] = 0x02;//��ȡֱ�������
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
      DataSend.Data[1] = 0x08;//��ȡ�������ܹ���
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
      ID.Disp.Cmd = 0x23;//��ȡ
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
	  
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x01;//��ȡ������A���ѹ
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//���״̬
      osDelay(2);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x02;//��ȡ������b���ѹ
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//���״̬
      osDelay(5);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x03;//��ȡ������c���ѹ
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//���״̬
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
      ID.Disp.Cmd = 0x23;//��ȡ
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
	  
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x04;//��ȡ������A�����
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//���״̬
      osDelay(5);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x05;//��ȡ������b�����
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//���״̬
      osDelay(5);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x21;
      DataSend.Data[1] = 0x06;//��ȡ������c�����
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//���״̬
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
  if(Currt<0x4E20)//��һ�������10000��������Ϊ�쳣
  DcModuleCtrl.ActualCurrt = Currt;
  else
  DcModuleCtrl.ActualCurrt = 0;	
  if(Volt<0x4E20)//��һ�������10000��������Ϊ�쳣
  DcModuleCtrl.ActualVolt = Volt/Count;
  else
  DcModuleCtrl.ActualVolt = 0;	
  
  if(CurrtA<0x4E20)//��һ�������10000��������Ϊ�쳣
    DcModuleCtrl.ActualCurrtA = CurrtA;
  else
    DcModuleCtrl.ActualCurrtA = 0;
  
  if(CurrtB<0x4E20)//��һ�������10000��������Ϊ�쳣
    DcModuleCtrl.ActualCurrtB = CurrtB;
  else
    DcModuleCtrl.ActualCurrtB = 0;
  if(CurrtC<0x4E20)//��һ�������10000��������Ϊ�쳣
    DcModuleCtrl.ActualCurrtC = CurrtC;
  else
    DcModuleCtrl.ActualCurrtC = 0;
  
  if(VoltA<0x4E20)//��һ�������10000��������Ϊ�쳣
    DcModuleCtrl.ActualVoltA= VoltA/Count;
  else
    DcModuleCtrl.ActualVoltA = 0;
  if(VoltB<0x4E20)//��һ�������10000��������Ϊ�쳣
    DcModuleCtrl.ActualVoltB= VoltB/Count;
  else
    DcModuleCtrl.ActualVoltB= 0;
  if(VoltC<0x4E20)//��һ�������10000��������Ϊ�쳣
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
    //�޵���ģ�鹦��
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
      ID.Disp.Cmd = 0x24;//����
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
      //�ж��Ƿ�Ϊ�ŵ�ģʽ
      if(GetWorkMode() == Enum_DischargeWork)
      {
        
        DcModuleManage.SetVolt = DcModuleCtrl.ActualVolt*100;
        DcModuleManage.SetCurrt = UserParam.DcMaxPower*100*1000*10/DcModuleManage.SetVolt*100;//���ģʽ
        
		if(DcModuleManage.SetCurrt >= DcModuleCtrl.NeedCurrt * 100 && CarTypeVar == NioCar_Type)
          DcModuleManage.SetCurrt = DcModuleCtrl.NeedCurrt * 100;

		if(DcModuleManage.SetCurrt > 16000)
		  DcModuleManage.SetCurrt = 16000;
		  
      }
		
      Temp = DcModuleManage.SetVolt ;
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x01;    //���õ�ѹ����
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = (u8)(Temp>>24);
      DataSend.Data[5] = (u8)(Temp>>16);
      DataSend.Data[6] = (u8)(Temp>>8);;
      DataSend.Data[7] = (u8)(Temp>>0);;
      DcModuleSendMsg(&DataSend);  //���״̬
      osDelay(2);
      Temp = DcModuleManage.SetCurrt;
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x02;//���õ�ѹ����
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = (u8)(Temp>>24);;
      DataSend.Data[5] = (u8)(Temp>>16);;
      DataSend.Data[6] = (u8)(Temp>>8);;
      DataSend.Data[7] = (u8)(Temp>>0);;
      DcModuleSendMsg(&DataSend);//���״̬
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
       DataSend.Data[7] = 0xA0;//����
       DcModuleSendMsg(&DataSend);//���״̬
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
        DataSend.Data[7] = 0xA2;//����Ϊ�������״̬
      else
	DataSend.Data[7] = 0xA0;//����Ϊ����ģʽ
      DcModuleSendMsg(&DataSend);
      osDelay(5);
     }     
  }
  else
  {
      
      ID.Mult = 0 ;
      ID.Disp.SourAddr =0xF0;
      ID.Disp.DestAddr = 0x3F;
      ID.Disp.Cmd = 0x24;//����
      ID.Disp.DeviceNum = 0x0A;
      ID.Disp.ErrNum = 0x00;
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x01;//���õ�ѹ����
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//���״̬
      osDelay(5);
      DataSend.Id = ID.Mult;
      DataSend.Len = 8;
      DataSend.Data[0] = 0x11;
      DataSend.Data[1] = 0x02;//���õ�ѹ����
      DataSend.Data[2] = 0x00;
      DataSend.Data[3] = 0x00;
      DataSend.Data[4] = 0x00;
      DataSend.Data[5] = 0x00;
      DataSend.Data[6] = 0x00;
      DataSend.Data[7] = 0x00;
      DcModuleSendMsg(&DataSend);//���״̬
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
      DataSend.Data[7] = 0xA1;//�ػ�
      DcModuleSendMsg(&DataSend);//���״̬
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
         DataSend.Data[7] = 0xA2;//����Ϊ���״̬
         DcModuleSendMsg(&DataSend);//���״̬
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
         DataSend.Data[7] = 0xA0;   //����Ϊ���ģʽ
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

//��ȡֱ�����ѹ
u16 GetDcModuleVolt(void)
{
  //return DcModuleCtrl.ActualVolt*201/200;
  return (u16)((u32)DcModuleCtrl.ActualVolt*UserParam.VoltRatio/1000);
}

//��ȡֱ�������
u16 GetDcModuleCurrt(void)
{
  //return DcModuleCtrl.ActualCurrt*202/200;
  return (u16)((u32)DcModuleCtrl.ActualCurrt*UserParam.CurrtRatio/1000);
}

//��ȡģ�����õ�ѹ
u16 GetDcModuleVoltSet(void)
{
  //return DcModuleManage.SetVolt/100;
  return DcModuleCtrl.NeedVolt;
}

//��ȡģ�����õ���
u16 GetDcModuleCurrtSet(void)
{
  //return DcModuleManage.SetVolt*DcModuleManage.ValidNum/100;
  return DcModuleCtrl.NeedCurrt;
}

//��ȡ�������ѹ
u16 GetDcModuleVoltA(void)
{
  return DcModuleCtrl.ActualVoltA;
}

//��ȡ���������
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

