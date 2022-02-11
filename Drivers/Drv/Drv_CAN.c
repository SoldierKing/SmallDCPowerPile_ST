#include "Drv_CAN.h"


CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

StructHcanDrv HcanDrv = 
{
  .hcan1 = &hcan1,
  .hcan2 = &hcan2,
  .CanRecvHandler1 = NULL,
  .CanRecvHandler2 = NULL, 
  
};

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */
  CAN_FilterTypeDef CAN1_FilerConf;
  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 9;   //波特率250kbit/s
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_7TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    //Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */
  
  /* USER CODE BEGIN CAN2_Init 2 */
  CAN1_FilerConf.FilterIdHigh = 0X0000;
  CAN1_FilerConf.FilterIdLow = 0X0000;
  CAN1_FilerConf.FilterMaskIdHigh = 0X0000;
  CAN1_FilerConf.FilterMaskIdLow = 0X0000;
  CAN1_FilerConf.FilterFIFOAssignment = CAN_FILTER_FIFO0;//此处选择fifo号要与其他地方一致
  CAN1_FilerConf.FilterBank = 0;
  CAN1_FilerConf.FilterMode = CAN_FILTERMODE_IDMASK;
  CAN1_FilerConf.FilterScale = CAN_FILTERSCALE_32BIT;
  CAN1_FilerConf.FilterActivation = ENABLE;
  CAN1_FilerConf.SlaveStartFilterBank = 0;
  if (HAL_CAN_ConfigFilter(&hcan1, &CAN1_FilerConf) != HAL_OK)
  {
    /* Filter configuration Error */
    //Error_Handler();
  }

  /* Start the CAN peripheral */
  if (HAL_CAN_Start(&hcan1) != HAL_OK)//这个函数和下面的函数是cubemx没有给出的，需要手动添加
  {
    /* Start Error */
    //Error_Handler();
  }

  /* Activate CAN RX notification */
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)//选择fifo0中断
  {
    /* Notification Error */
    //Error_Handler();
  }
  

}
  

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */
  CAN_FilterTypeDef CAN2_FilerConf;
  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 18;//9;   //波特率125Kbit/s
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_7TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    //Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */
  CAN2_FilerConf.FilterIdHigh = 0X0000;
  CAN2_FilerConf.FilterIdLow = 0X0000;
  CAN2_FilerConf.FilterMaskIdHigh = 0X0000;
  CAN2_FilerConf.FilterMaskIdLow = 0X0000;
  CAN2_FilerConf.FilterFIFOAssignment = CAN_FILTER_FIFO0;//此处选择fifo号要与其他地方一致
  CAN2_FilerConf.FilterBank = 0;
  CAN2_FilerConf.FilterMode = CAN_FILTERMODE_IDMASK;
  CAN2_FilerConf.FilterScale = CAN_FILTERSCALE_32BIT;
  CAN2_FilerConf.FilterActivation = ENABLE;
  CAN2_FilerConf.SlaveStartFilterBank = 0;
  if (HAL_CAN_ConfigFilter(&hcan2, &CAN2_FilerConf) != HAL_OK)
  {
    /* Filter configuration Error */
    //Error_Handler();
  }

  /* Start the CAN peripheral */
  if (HAL_CAN_Start(&hcan2) != HAL_OK)//这个函数和下面的函数是cubemx没有给出的，需要手动添加
  {
    /* Start Error */
    //Error_Handler();
  }

  /* Activate CAN RX notification */
  if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)//选择fifo1中断
  {
    /* Notification Error */
    //Error_Handler();
  }

}


/**
*@brief  CAN1回调函数
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef Rx1Msg;

  uint8_t Rx1_BUF[8];
  Rx1Msg.DLC=8;
  Rx1Msg.IDE=CAN_ID_STD;
  Rx1Msg.RTR=CAN_RTR_DATA;
  
  CAN_RxHeaderTypeDef Rx2Msg;

  uint8_t Rx2_BUF[8];
  Rx2Msg.DLC=8;
  Rx2Msg.IDE=CAN_ID_STD;
  Rx2Msg.RTR=CAN_RTR_DATA;

  if(hcan->Instance == CAN1)
  {
      HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &Rx1Msg, Rx1_BUF);//此处的fifo0也要注意
      if (HcanDrv.CanRecvHandler1 != NULL)
        HcanDrv.CanRecvHandler1(Rx1Msg, Rx1_BUF); 
  }
  
  if(hcan->Instance == CAN2)
  {
      HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &Rx2Msg, Rx2_BUF);//此处的fifo0也要注意
      if (HcanDrv.CanRecvHandler2 != NULL)
        HcanDrv.CanRecvHandler2(Rx2Msg, Rx2_BUF); 
  }
}





