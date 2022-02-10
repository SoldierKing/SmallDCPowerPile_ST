#ifndef  __DRV_CAN_H__
#define  __DRV_CAN_H__

#include "Global.h"

typedef void (*pCanRecvHandler)(CAN_RxHeaderTypeDef RxMessage,u8 *Buff);


typedef struct
{
  
  CAN_HandleTypeDef *hcan1;
  CAN_HandleTypeDef *hcan2;
  pCanRecvHandler   CanRecvHandler1;
  pCanRecvHandler   CanRecvHandler2;
  
}StructHcanDrv;

extern StructHcanDrv HcanDrv;

extern void MX_CAN1_Init(void);
extern void MX_CAN2_Init(void);

#endif
