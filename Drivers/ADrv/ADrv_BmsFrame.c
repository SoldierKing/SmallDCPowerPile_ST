#include "ADrv_BmsFrame.h"

#include "Drv_CAN.h"

EnumChgVersion ChgVer = eChgVer_2011;

StructRES ResMsg = {0};
StructBHM BhmMsg = {0};
StructBRM BrmMsg = {0};
StructBCP BcpMsg = {0};
StructBRO BroMsg = {0};
StructBCL BclMsg = {0};
StructBCS BcsMsg = {0};
StructBSM BsmMsg = {0};
StructBMV BmvMsg = {0};
StructBMT BmtMsg = {0};
StructBSP BspMsg = {0};
StructBST BstMsg = {0};
StructBSD BsdMsg = {0};
StructBEM BemMsg = {0};
StructNTM NtmMsg = {0};

StructCST CstMsg = {0};
StructCSD CsdMsg = {0};
StructCEM CemMsg = {0};

osMessageQId BmsMsg = 0;

u32 SupportedVerList[] = 
{
  (u32)0x00010100, //对应Ver1_1_0 版本
  (u32)0x00020000, //对应MaxVer2_0_0 版本
};



void BmsComm_CanRecvHandler(CAN_RxHeaderTypeDef RxMessage,u8 *Buff)
{
  Union1939Id Id1939 = {.Mult = 0,};
  StructPgnRecv Frame;

  if((CAN_ID_STD == RxMessage.IDE) || (CAN_RTR_REMOTE == RxMessage.RTR))
    return;
  Id1939.Mult = RxMessage.ExtId;
  if(1 == Id1939.Disp.R)
    return;

  if(1 == Id1939.Disp.DP)
    return;
  if((Id1939.Disp.DA_PS != Charger1939Addr) || (Id1939.Disp.SA != Card1939Addr))
    return;

  if(Id1939.Disp.PF < 240)
  {
    Frame.DA = Id1939.Disp.DA_PS;

    Frame.Pgn = Id1939.Disp.PF << 8;
    if(Id1939.Disp.DP)
      Frame.Pgn += 0x010000;
  }
  else
  {
    Frame.DA = 0xFF;
    Frame.Pgn = (Id1939.Disp.PF << 8) + Id1939.Disp.DA_PS;
    if(Id1939.Disp.DP)
      Frame.Pgn += 0x010000;
  }

  switch(Frame.Pgn)
  {
    case EVM:
    case RES:
    case BHM:
    case BRM:

    //case BCP:
    case BRO:

    case BCL:
    //case BCS:
    case BSM:
    case BMV:
    case BMT:
    case BSP:
    case BST:

    case BSD:

    case BEM:

    //case DM1:
    //case DM2:
    //case DM3:
    //case DM4:
    //case DM5:
    //case DM6:

    case TPCM:
    case TPDT:
    case REQ:
      Frame.Prio = Id1939.Disp.Prio;
      Frame.DP = Id1939.Disp.DP;
      Frame.PF = Id1939.Disp.PF;
      Frame.SA = Id1939.Disp.SA;
      Frame.Len = RxMessage.DLC;
      if(Frame.Len > 8)
        Frame.Len = 8;
      Frame.Data[0] = Buff[0];
      Frame.Data[1] = Buff[1];
      Frame.Data[2] = Buff[2];
      Frame.Data[3] = Buff[3];
      Frame.Data[4] = Buff[4];
      Frame.Data[5] = Buff[5];
      Frame.Data[6] = Buff[6];
      Frame.Data[7] = Buff[7];

      //osMessagePut(BmsMsg, &Frame, 0);
      {
        portBASE_TYPE taskWoken = pdFALSE;
        if(pdTRUE == xQueueSendFromISR(BmsMsg, &Frame, &taskWoken))
          portEND_SWITCHING_ISR(taskWoken);
      }
      break;

    case ACK:
    default:
      break;
  }
}

u8 BmsCommInit(void)
{
  /* Create Storage Message Queue */
  BmsMsg = NULL;
  osMessageQDef(BmsMsg, 64, StructPgnRecv);
  BmsMsg = osMessageCreate(osMessageQ(BmsMsg), NULL);

  HcanDrv.CanRecvHandler1 = BmsComm_CanRecvHandler;

  osDelay(20);
  MX_CAN1_Init();
  return TRUE;
}


void ResMsgMake(u8 *pBuff)
{
  u8 Temp;

  Temp = *(pBuff+2);
  ResMsg.V2LDischargeReq.Mult = Temp;

  ResMsg.RecvTick = GetSystemTick();
  ResMsg.Valid = 1;
  
}

void BhmMsgMake(u8 *pBuff)
{
  u16 Temp;

  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BhmMsg.ImdMaxVolt = Temp;

  ChgVer = eChgVer_2015;
  BhmMsg.RecvTick = GetSystemTick();
  BhmMsg.Valid = 1;
}

void BrmMsgMake(u8 *pBuff, u16 Len)
{
  u8 i;
  u16 Temp;
  u32 u32Temp;

  BrmMsg.CommVer.L = *pBuff++;
  BrmMsg.CommVer.M = *pBuff++;
  BrmMsg.CommVer.H = *pBuff++;
  BrmMsg.BatType = *pBuff++;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BrmMsg.BatRateCap = Temp;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BrmMsg.BatRateVolt = Temp;

  if(Len > 8)
  {
    BrmMsg.Manufacturer[0] = *pBuff++;
    BrmMsg.Manufacturer[1] = *pBuff++;
    BrmMsg.Manufacturer[2] = *pBuff++;
    BrmMsg.Manufacturer[3] = *pBuff++;
    BrmMsg.ProduceNum[0] = *pBuff++;
    BrmMsg.ProduceNum[1] = *pBuff++;
    BrmMsg.ProduceNum[2] = *pBuff++;
    BrmMsg.ProduceNum[3] = *pBuff++;
    BrmMsg.ProduceYear = *pBuff++;
    BrmMsg.ProduceMon = *pBuff++;
    BrmMsg.ProduceDay = *pBuff++;
    u32Temp = *pBuff++;
    u32Temp |= ((u32)*pBuff++) << 8;
    u32Temp |= ((u32)*pBuff++) << 16;
    BrmMsg.ChgCount = u32Temp;
    BrmMsg.BatOwn = *pBuff++;
    BrmMsg.Reserve = *pBuff++;
    for(i=0; i<17; i++)
      BrmMsg.Vin[i] = *pBuff++;
  }

  if(49 == Len)
  {
    for(i=0; i<8; i++)
      BrmMsg.BmsVer[i] = *pBuff++;
  }
  else
  {
    for(i=0; i<8; i++)
      BrmMsg.BmsVer[i] = 0xFF;
  }
  
  
  if(0 != BhmMsg.Valid)
    ChgVer = eChgVer_2015;
  else if((0x01 == BrmMsg.CommVer.L) && (0x01 == BrmMsg.CommVer.M) && (49 == Len))
    ChgVer = eChgVer_2015;
  else
    ChgVer = eChgVer_2011;
  BrmMsg.RecvTick = GetSystemTick();
  BrmMsg.Valid = 1;
}

void BcpMsgMake(u8 *pBuff)
{
  u16 Temp;

  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BcpMsg.PerBatMaxVolt = Temp;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BcpMsg.ChgMaxCurrt = Temp;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BcpMsg.BatEnergy = Temp;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BcpMsg.ChgMaxVolt = Temp;
  BcpMsg.MaxTemp = *pBuff++;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BcpMsg.StartSoc = Temp;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BcpMsg.StartVolt = Temp;

  BcpMsg.RecvTick = GetSystemTick();
  BcpMsg.Valid = 1;
}

void BroMsgMake(u8 *pBuff)
{
  BroMsg.Status = *pBuff++;

  BroMsg.RecvTick = GetSystemTick();
  BroMsg.Valid = 1;
}

void BclMsgMake(u8 *pBuff)
{
  u16 Temp;

  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BclMsg.NeedVolt = Temp;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BclMsg.NeedCurrt = Temp;
  BclMsg.ChgType = *pBuff++;

  BclMsg.RecvTick = GetSystemTick();
  BclMsg.Valid = 1;
}

void BcsMsgMake(u8 *pBuff)
{
  u16 Temp;

  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BcsMsg.ChgVolt = Temp;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BcsMsg.ChgCurrt = Temp;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BcsMsg.BatVoltAndGroup.Mult = Temp;
  BcsMsg.CurSoc = *pBuff++;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BcsMsg.RemainMin = Temp;

  BcsMsg.RecvTick = GetSystemTick();
  BcsMsg.Valid = 1;
}

void BsmMsgMake(u8 *pBuff)
{
  BsmMsg.HighTempBatNum = *pBuff++;
  BsmMsg.HighTemp = *pBuff++;
  BsmMsg.HighTempPointNum = *pBuff++;
  BsmMsg.LowTemp = *pBuff++;
  BsmMsg.LowTempPointNum = *pBuff++;
  BsmMsg.BatSta.Mult = *pBuff++;
  BsmMsg.ChgSta.Mult = *pBuff++;

  BsmMsg.RecvTick = GetSystemTick();
  BsmMsg.Valid = 1;
}

void BmvMsgMake(u8 *pBuff, u16 Len)
{
  u16 i;
  u16 Temp;

  if(Len > 512)
    Len = 512;

  for(i=0; i<(Len/2); i++)
  {
    Temp = *pBuff++;
    Temp |= ((u32)*pBuff++) << 8;
    BmvMsg.BatVoltAndGroup[i].Mult = Temp;
  }
  for( ; i<256; i++)
    BmvMsg.BatVoltAndGroup[i].Mult = 0xFFFF;

  BmvMsg.RecvTick = GetSystemTick();
  BmvMsg.Valid = 1;
}

void BmtMsgMake(u8 *pBuff, u16 Len)
{
  u16 i;

  if(Len > 128)
    Len = 128;

  for(i=0; i<Len; i++)
    BmtMsg.Temp[i] = *pBuff++;

  for( ; i<128; i++)
    BmtMsg.Temp[i] = 0xFF;

  BmtMsg.RecvTick = GetSystemTick();
  BmtMsg.Valid = 1;
}

void BspMsgMake(u8 *pBuff, u16 Len)
{
  u16 i;

  if(Len > 16)
    Len = 16;

  for(i=0; i<Len; i++)
    BspMsg.Bsp[i] = *pBuff++;

  for( ; i<16; i++)
    BspMsg.Bsp[i] = 0xFF;

  BspMsg.RecvTick = GetSystemTick();
  BspMsg.Valid = 1;
}

void BstMsgMake(u8 *pBuff)
{
  u16 Temp;

  BstMsg.StopReason.Mult = *pBuff++;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BstMsg.StopTruble.Mult = Temp;
  BstMsg.StopFault.Mult = *pBuff++;

//  BstMsg.RecvTick = GetSystemTick();
  BstMsg.Valid = 1;
}

void BsdMsgMake(u8 *pBuff)
{
  u16 Temp;

  BsdMsg.StopSOC= *pBuff++;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BsdMsg.PerMinVolt = Temp;
  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  BsdMsg.PerMaxVolt = Temp;
  BsdMsg.MinTemp = *pBuff++;
  BsdMsg.MaxTemp = *pBuff++;

//  BsdMsg.RecvTick = GetSystemTick();
  BsdMsg.Valid = 1;
}

void BemMsgMake(u8 *pBuff)
{
  u32 Temp;

  Temp = *pBuff++;
  Temp |= ((u32)*pBuff++) << 8;
  Temp |= ((u32)*pBuff++) << 16;
  Temp |= ((u32)*pBuff++) << 24;
  BemMsg.Bem.Mult = Temp;

  BemMsg.Valid = 1;
}

StructTransProt TransProt = {0};

/**************************************************************************************************
** 函数名: CAN1_SendMessage
** 输　入: ulCanId----发送ID， ulCanDataA----发送数据的底4字节，  ulCanDataB----发送数据的高4字节
** 输　出: 参数读取状态
** 描  述: 通过CAN通道发送数据
** 日　期: 2010/08/19
**************************************************************************************************/
u8 BmsCommSendMsg(StructChargeSend *pMsg)
{
  CAN_TxHeaderTypeDef TxMessage;
  u32 pTxMailbox = 0;

  TxMessage.ExtId = pMsg->Id.Mult;// 设定扩展标识符
  TxMessage.IDE = CAN_ID_EXT;// 设定消息标识符的类型
  TxMessage.RTR = CAN_RTR_DATA;// 设定待传输消息的帧类型
  TxMessage.DLC = pMsg->Len; //设定待传输消息的帧长度
  
  if (HAL_CAN_AddTxMessage(HcanDrv.hcan1,&TxMessage,pMsg->Data,&pTxMailbox) != HAL_OK)
    return FALSE;				//未发送成功，返回FALSE

  return TRUE;
}

void NACK_Send(StructPgnRecv *pFrameRecv)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 6;
  FrameSend.Id.Disp.PF = ACK>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = 0x01;
  FrameSend.Data[1] = 0xFF;
  FrameSend.Data[2] = 0xFF;
  FrameSend.Data[3] = 0xFF;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = pFrameRecv->Data[0];
  FrameSend.Data[6] = pFrameRecv->Data[1];
  FrameSend.Data[7] = pFrameRecv->Data[2];
  FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
}

void TPCM_AbortSendBegin(StructPgnRecv *pFrameRecv)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 7;
  FrameSend.Id.Disp.PF = TPCM>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = 0xFF;
  FrameSend.Data[1] = 0xFF;
  FrameSend.Data[2] = 0xFF;
  FrameSend.Data[3] = 0xFF;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = pFrameRecv->Data[5];
  FrameSend.Data[6] = pFrameRecv->Data[6];
  FrameSend.Data[7] = pFrameRecv->Data[7];
  FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
}

void TPCM_Cts(StructPgnRecv *pFrameRecv)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 7;
  FrameSend.Id.Disp.PF = TPCM>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = 0x11;
  FrameSend.Data[1] = pFrameRecv->Data[3];
  FrameSend.Data[2] = 0x01;
  FrameSend.Data[3] = 0xFF;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = pFrameRecv->Data[5];
  FrameSend.Data[6] = pFrameRecv->Data[6];
  FrameSend.Data[7] = pFrameRecv->Data[7];
  FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
}

void TPCM_AbortSending(StructTransProt *pTransProt)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 7;
  FrameSend.Id.Disp.PF = TPCM>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = 0x13;
  FrameSend.Data[1] = pTransProt->RecvDataCnt;
  FrameSend.Data[2] =  pTransProt->RecvDataCnt>> 8;
  FrameSend.Data[3] = pTransProt->RecvFrameCnt;
  FrameSend.Data[4] = 0xFe;
  FrameSend.Data[5] = (u8)(pTransProt->Pgn>>0);
  FrameSend.Data[6] = (u8)(pTransProt->Pgn>>8);
  FrameSend.Data[7] = (u8)(pTransProt->Pgn>>16);
  FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
}
void TPCM_EndOfMsgAck(StructTransProt *pTransProt)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 7;
  FrameSend.Id.Disp.PF = TPCM>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = 0x13;
  FrameSend.Data[1] = pTransProt->RecvDataCnt;
  FrameSend.Data[2] = pTransProt->RecvDataCnt>> 8;
  FrameSend.Data[3] = pTransProt->RecvFrameCnt;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = (u8)(pTransProt->Pgn>>0);
  FrameSend.Data[6] = (u8)(pTransProt->Pgn>>8);
  FrameSend.Data[7] = (u8)(pTransProt->Pgn>>16);
  FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
}
void NTM_SendLenErro(u8  isOK,u32 verNum)
{
  StructChargeSend FrameSend;
  u32 Temp = verNum;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 0x03;
  FrameSend.Id.Disp.PF = NTM>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = 0x00;
  switch (isOK)
  {
  case 1:
    FrameSend.Data[1] = 0x01;
	break;
  case 0:
  	FrameSend.Data[1] = 0x00;
	break;
  case 2:	
	FrameSend.Data[1] = 0x02;
	break;
  }
  FrameSend.Data[2] = (u8)((Temp & 0x00FF0000) >> 16); //版本号
  FrameSend.Data[3] = (u8)((Temp & 0x0000FF00) >> 8);
  FrameSend.Data[4] = (u8)((Temp & 0x000000FF));
  //FrameSend.Data[5] = 0xFF;
  //FrameSend.Data[6] = 0xFF;
  //FrameSend.Data[7] = 0x02;
  FrameSend.Len = 5;

  BmsCommSendMsg(&FrameSend);
}

void NTM_Send(u8  isOK,u32 verNum)
{
  StructChargeSend FrameSend;
  u32 Temp = verNum;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 0x03;
  FrameSend.Id.Disp.PF = NTM>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = 0x00;
  switch (isOK)
  {
  case 1:
    FrameSend.Data[1] = 0x01;
	break;
  case 0:
  	FrameSend.Data[1] = 0x00;
	break;
  case 2:	
	FrameSend.Data[1] = 0x02;
	break;
  }
  FrameSend.Data[2] = (u8)((Temp & 0x00FF0000) >> 16); //版本号
  FrameSend.Data[3] = (u8)((Temp & 0x0000FF00) >> 8);
  FrameSend.Data[4] = (u8)((Temp & 0x000000FF));
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = (u8)((FrameSend.Data[0]+ FrameSend.Data[1]+FrameSend.Data[2]+FrameSend.Data[3]+FrameSend.Data[4]+
  	                  FrameSend.Data[5]+FrameSend.Data[6])&0x00FF);
  FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
}


void V2L_Send(void)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 3;
  FrameSend.Id.Disp.PF = V2L>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  //VCU收到1认为是放电，其他值都认为是充电
  //该信号周期性发送，VCU未收到该信号也认为是充电
  FrameSend.Data[0] = 0x01;
  FrameSend.Data[1] = 0xFF;
  FrameSend.Data[2] = 0xFF;
  
  FrameSend.Data[3] = 0xFF;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = 0xFF;
  FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
}


void CHM_Send(void)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 6;
  FrameSend.Id.Disp.PF = CHM>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  if (NtmMsg.Result == 0x01 && NtmMsg.VerisOK == 1)
  {
    FrameSend.Data[0] =(u8)((SupportedVerList[NtmMsg.svEnum]& 0x00FF0000)>>16);
  
    FrameSend.Data[1] =(u8)((SupportedVerList[NtmMsg.svEnum]& 0x0000FF00)>>8);
  
    FrameSend.Data[2] =(u8)((SupportedVerList[NtmMsg.svEnum]& 0x000000FF));
  }
  else
  {
    FrameSend.Data[0] =(u8)((SupportedVerList[Ver1_1_0]& 0x00FF0000)>>16);
  
    FrameSend.Data[1] =(u8)((SupportedVerList[Ver1_1_0]& 0x0000FF00)>>8);
  
    FrameSend.Data[2] =(u8)((SupportedVerList[Ver1_1_0]& 0x000000FF));
  }
  /*
  FrameSend.Data[0] = 0x01;
  FrameSend.Data[1] = 0x01;
  FrameSend.Data[2] = 0x00;
  */
  FrameSend.Data[3] = 0xFF;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = 0xFF;
  FrameSend.Len = 3;

  BmsCommSendMsg(&FrameSend);
}

void CRM_Send(u8 Recog)
{

  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 6;
  FrameSend.Id.Disp.PF = CRM>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = Recog;//0x00,0xAA
  FrameSend.Data[1] = 0xFF;
  FrameSend.Data[2] = 0xFF;
  FrameSend.Data[3] = 0xFF;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = 0xFF;
  FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
  
}

void CTS_Send(void)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 6;
  FrameSend.Id.Disp.PF = CTS>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = 0xFF;
  FrameSend.Data[1] = 0xFF;
  FrameSend.Data[2] = 0xFF;
  FrameSend.Data[3] = 0xFF;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = 0xFF;
  FrameSend.Len = 7;

  BmsCommSendMsg(&FrameSend);
}

void CML_Send(u16 MaxVolt, u16 MinVolt, u16 MaxCurrt, u16 MinCurrt)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 6;
  FrameSend.Id.Disp.PF = CML>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = (u8)(MaxVolt);//最高输出电压（V）
  FrameSend.Data[1] = (u8)((u32)MaxVolt>>8);//数据分辨率： 0.1 V /位，0 V偏移量；数据范围：0~+750 V；
  FrameSend.Data[2] = (u8)(MinVolt);//最低输出电压（V）
  FrameSend.Data[3] = (u8)((u32)MinVolt>>8);//数据分辨率： 0.1 V /位，0 V偏移量；数据范围：0~+750 V；
  FrameSend.Data[4] = (u8)(MaxCurrt);//最大输出电流（A）
  FrameSend.Data[5] = (u8)((u32)(MaxCurrt)>>8);//数据分辨率：0.1 A/位，-400 A偏移量
  FrameSend.Data[6] = (u8)(MinCurrt);////最小输出电流（A）
  FrameSend.Data[7] = (u8)((u32)(MinCurrt)>>8);////数据分辨率：0.1 A/位，-400 A偏移量
  if(eChgVer_2011 == ChgVer)
    FrameSend.Len = 6;
  else
    FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
}

void CRO_Send(u8 Status)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 4;
  FrameSend.Id.Disp.PF = CRO>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = Status;//0x00,0xAA
  FrameSend.Data[1] = 0xFF;
  FrameSend.Data[2] = 0xFF;
  FrameSend.Data[3] = 0xFF;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = 0xFF;
  FrameSend.Len = 1;

  BmsCommSendMsg(&FrameSend);
}

void CCS_Send(u16 Volt, u16 Currt, u16 Min, u16 ChgEnable)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 6;
  FrameSend.Id.Disp.PF = CCS>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = Volt;//0x00,0xAA
  FrameSend.Data[1] = Volt>>8;
  FrameSend.Data[2] = Currt;
  FrameSend.Data[3] = Currt>>8;
  FrameSend.Data[4] = Min;
  FrameSend.Data[5] = Min>>8;;
  FrameSend.Data[6] = 0xFC | (0x03 & ChgEnable);
  FrameSend.Data[7] = 0xFF;

  if(eChgVer_2011 == ChgVer)
    FrameSend.Len = 6;
  else
    FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
}

void CST_Send(StructCST *pCst)
{
  StructChargeSend FrameSend;
  StructCST TempCst;

  TempCst = *pCst;
  TempCst.StopTruble.Value.Reserve = 0x0F;
  TempCst.StopFault.Value.Reserve = 0x0F;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 4;
  FrameSend.Id.Disp.PF = CST>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = TempCst.StopReason.Mult;
  FrameSend.Data[1] = TempCst.StopTruble.Mult;
  FrameSend.Data[2] = TempCst.StopTruble.Mult>>8;
  FrameSend.Data[3] = TempCst.StopFault.Mult;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = 0xFF;
  FrameSend.Len = 4;

  BmsCommSendMsg(&FrameSend);
}

void CSD_Send(StructCSD *pCsd)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 6;
  FrameSend.Id.Disp.PF = CSD>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = pCsd->ChgMin;
  FrameSend.Data[1] = pCsd->ChgMin>>8;
  FrameSend.Data[2] = pCsd->ChgEnergy;
  FrameSend.Data[3] = pCsd->ChgEnergy>>8;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = 0xFF;
  if(eChgVer_2011 == ChgVer)
    FrameSend.Len = 5;
  else
    FrameSend.Len = 8;

  BmsCommSendMsg(&FrameSend);
}

void CEM_Send(StructCEM *pCem)
{
  StructChargeSend FrameSend;
  StructCEM TempCem;

  TempCem = *pCem;
  TempCem.Cem.Value.Reserve1 = 0x3F;
  TempCem.Cem.Value.Reserve2 = 0x0F;
  TempCem.Cem.Value.Reserve3 = 0x03;
  TempCem.Cem.Value.Other = 0x3F;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 2;
  FrameSend.Id.Disp.PF = CEM>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = TempCem.Cem.Mult;
  FrameSend.Data[1] = TempCem.Cem.Mult>>8;
  FrameSend.Data[2] = TempCem.Cem.Mult>>16;
  FrameSend.Data[3] = TempCem.Cem.Mult>>24;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = 0xFF;
  FrameSend.Len = 4;

  BmsCommSendMsg(&FrameSend);
}

void DM1_Send(u32 Dtc)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 6;
  FrameSend.Id.Disp.PF = DM1>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = Dtc;
  FrameSend.Data[1] = Dtc>>8;
  FrameSend.Data[2] = Dtc>>16;
  FrameSend.Data[3] = Dtc>>24;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = 0xFF;
  FrameSend.Len = 4;

  BmsCommSendMsg(&FrameSend);
}

void DM3_Send(u16 Num)
{
  StructChargeSend FrameSend;

  FrameSend.Id.Mult = 0;
  FrameSend.Id.Disp.Prio = 6;
  FrameSend.Id.Disp.PF = DM3>>8;
  FrameSend.Id.Disp.DA_PS = Card1939Addr;
  FrameSend.Id.Disp.SA = Charger1939Addr;
  FrameSend.Data[0] = Num;
  FrameSend.Data[1] = Num>>8;
  FrameSend.Data[2] = 0xFF;
  FrameSend.Data[3] = 0xFF;
  FrameSend.Data[4] = 0xFF;
  FrameSend.Data[5] = 0xFF;
  FrameSend.Data[6] = 0xFF;
  FrameSend.Data[7] = 0xFF;
  FrameSend.Len = 2;

  BmsCommSendMsg(&FrameSend);
}

s32 ChgPgnRecv(StructPgnRecv *pPgnRecv)
{
  u32 TP_Pgn = 0;
  s32 RetPgn = -1;

  if(pdTRUE != xQueueReceive(BmsMsg, pPgnRecv, 0))
    return RetPgn;

  switch(pPgnRecv->Pgn)
  {
    case EVM:
	  //NtmMsgMake(pPgnRecv->Data,pPgnRecv->Len);
	  //RetPgn = pPgnRecv->Pgn;
      break;
    case RES:
      ResMsgMake(pPgnRecv->Data);
      RetPgn = pPgnRecv->Pgn;
      break;
    case BHM:
      BhmMsgMake(pPgnRecv->Data);
      RetPgn = pPgnRecv->Pgn;
      break;
    case BRO:
      BroMsgMake(pPgnRecv->Data);
      RetPgn = pPgnRecv->Pgn;
      break;
    case BCL:
      BclMsgMake(pPgnRecv->Data);
      RetPgn = pPgnRecv->Pgn;
      break;
    case BSM:
      BsmMsgMake(pPgnRecv->Data);
      RetPgn = pPgnRecv->Pgn;
      break;
    case BMV:
      BmvMsgMake(pPgnRecv->Data, pPgnRecv->Len);
      RetPgn = pPgnRecv->Pgn;
      break;
    case BMT:
      BmtMsgMake(pPgnRecv->Data, pPgnRecv->Len);
      RetPgn = pPgnRecv->Pgn;
      break;
    case BSP:
      BspMsgMake(pPgnRecv->Data, pPgnRecv->Len);
      RetPgn = pPgnRecv->Pgn;
      break;
    case BST:
      BstMsgMake(pPgnRecv->Data);
      RetPgn = pPgnRecv->Pgn;
      break;
    case BSD:
      BsdMsgMake(pPgnRecv->Data);
      RetPgn = pPgnRecv->Pgn;
      break;
    case BEM:
      BemMsgMake(pPgnRecv->Data);
      RetPgn = pPgnRecv->Pgn;
      break;
    case REQ:
      NACK_Send(pPgnRecv);
      RetPgn = pPgnRecv->Pgn;
      break;
	case BRM:
      BrmMsgMake(pPgnRecv->Data,8);
      RetPgn = pPgnRecv->Pgn;
      break;
    case TPCM:
      switch(pPgnRecv->Data[0])
      {
        case 0x10:
          TP_Pgn = (pPgnRecv->Data[7]<<16) + (pPgnRecv->Data[6]<<8) + pPgnRecv->Data[5];
          switch(TP_Pgn)
          {
            case BRM:
            case BCP:
            case BCS:
            case BMV:
            case BMT:
            case BSP:
              TransProt.FrameDataLen = ((u32)pPgnRecv->Data[2]<<8) + pPgnRecv->Data[1];
              TransProt.FrameMaxNum = pPgnRecv->Data[3];
              TransProt.Pgn = TP_Pgn;
              TransProt.RecvFrameCnt = 0;
              TransProt.RecvDataCnt = 0;

              if((TransProt.FrameDataLen < 9)|| (TransProt.FrameDataLen > 1785))
              {
                TransProt.Flag = 0x00;
                TPCM_AbortSendBegin(pPgnRecv);
              }
              else if(0 == TransProt.FrameMaxNum)
              {
                TransProt.Flag = 0x00;
                TPCM_AbortSendBegin(pPgnRecv);
              }
              else if(TransProt.FrameDataLen > MaxDataSize1939)
              {
                TransProt.Flag = 0x00;
                TPCM_AbortSendBegin(pPgnRecv);
              }
              else
              {
                TransProt.Flag = 0x10;
                TPCM_Cts(pPgnRecv);
              }
              break;

            default:
              TPCM_AbortSendBegin(pPgnRecv);
              TransProt.Flag = 0x00;
              break;
          }
          break;
        case 0xFF:
          TP_Pgn = (pPgnRecv->Data[7]<<16) + (pPgnRecv->Data[6]<<8) + pPgnRecv->Data[5];
          if(TransProt.Pgn == TP_Pgn)
            TransProt.Flag = 0x00;
          break;
        default:
          break;
      }
      break;
    case TPDT:
     // if(0x10 == TransProt.Flag)//开沃大巴
      {
         if(pPgnRecv->Data[0] == 1)
         {
		  TransProt.RecvFrameCnt = 0;
          TransProt.RecvDataCnt = 0;
		  
         }
        if((TransProt.RecvFrameCnt+1) == pPgnRecv->Data[0])
        {
          u8 i;

          for(i=1; i<8; i++)
          {
            TransProt.RecvBuff[TransProt.RecvDataCnt] = pPgnRecv->Data[i];
            TransProt.RecvDataCnt++;
            if(TransProt.RecvDataCnt >= TransProt.FrameDataLen)
              break;
          }
          TransProt.RecvFrameCnt++;

          if((TransProt.RecvFrameCnt >= TransProt.FrameMaxNum)
            || (TransProt.RecvDataCnt >= TransProt.FrameDataLen))
          {
            TransProt.Flag = 0x13;
            TPCM_EndOfMsgAck(&TransProt);

            switch(TransProt.Pgn)
            {
              case BRM:
                BrmMsgMake(TransProt.RecvBuff, TransProt.FrameDataLen);
                RetPgn = pPgnRecv->Pgn;
                break;
              case BCP:
                BcpMsgMake(TransProt.RecvBuff);
                RetPgn = pPgnRecv->Pgn;
                break;
              case BCS:
                BcsMsgMake(TransProt.RecvBuff);
                RetPgn = pPgnRecv->Pgn;
                break;
              case BMV:
                BmvMsgMake(TransProt.RecvBuff, TransProt.FrameDataLen);
                RetPgn = pPgnRecv->Pgn;
                break;
              case BMT:
                BmtMsgMake(TransProt.RecvBuff, TransProt.FrameDataLen);
                RetPgn = pPgnRecv->Pgn;
                break;
              case BSP:
                BspMsgMake(TransProt.RecvBuff, TransProt.FrameDataLen);
                RetPgn = pPgnRecv->Pgn;
                break;
              default:
                break;
            }
          }
        }
        else
        {
          TransProt.RecvFrameCnt =0;
          TransProt.Flag = 0x00;
          TPCM_AbortSending(&TransProt);
        }
      }
      break;
    default:
      break;
  }
  return RetPgn;
}

