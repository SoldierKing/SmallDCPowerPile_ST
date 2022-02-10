#include "Drv_GPIO.h"

static void RunLedInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  //运行指示灯初始化
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RunLed_GPIO_Port, RunLed_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : RunLed_Pin */
  GPIO_InitStruct.Pin = RunLed_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(RunLed_GPIO_Port, &GPIO_InitStruct);
}

static void CC1InInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
    
  /*Configure GPIO pins : CC1In_Pin VbatNeg_Pin */
  GPIO_InitStruct.Pin = CC1In_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CC1In_GPIO_Port, &GPIO_InitStruct);
}

static void EleLockInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  HAL_GPIO_WritePin(EleLock_GPIO_Port, EleLock_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EleUnlock_GPIO_Port, EleUnlock_Pin, GPIO_PIN_RESET);
  //LOCK 口定义
  /*Configure GPIO pins : CC1In_Pin VbatNeg_Pin */
  GPIO_InitStruct.Pin = EleLock_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(EleLock_GPIO_Port, &GPIO_InitStruct);
  //Unlock
  GPIO_InitStruct.Pin = EleUnlock_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(EleUnlock_GPIO_Port, &GPIO_InitStruct);
  //电子锁反馈
  GPIO_InitStruct.Pin = LockSW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(LockSW_GPIO_Port, &GPIO_InitStruct);
  
}

static void IMDPowerENInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  HAL_GPIO_WritePin(BoostEN_GPIO_Port, BoostEN_Pin, GPIO_PIN_RESET);
   
  GPIO_InitStruct.Pin = BoostEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BoostEN_GPIO_Port, &GPIO_InitStruct);
}

static void OneKeyInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  HAL_GPIO_WritePin(StartON_GPIO_Port, StartON_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = StartON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(StartON_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = KeySta_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KeySta_GPIO_Port, &GPIO_InitStruct);
}

static void VbatIsChargInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  GPIO_InitStruct.Pin = VbatChg_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VbatChg_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = VbatNeg_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VbatNeg_GPIO_Port, &GPIO_InitStruct);
}

static void BMSPowerENInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  HAL_GPIO_WritePin(BmsON_GPIO_Port, BmsON_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = BmsON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BmsON_GPIO_Port, &GPIO_InitStruct);
}

static void IMDCheckCtlInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  HAL_GPIO_WritePin(IMDLrly_GPIO_Port, IMDLrly_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(IMDHrly_GPIO_Port, IMDHrly_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PeRly_GPIO_Port, PeRly_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = IMDLrly_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IMDLrly_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = IMDHrly_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IMDHrly_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = PeRly_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PeRly_GPIO_Port, &GPIO_InitStruct);
  
}

static void LeakCheckInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  GPIO_InitStruct.Pin = LeakIn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(LeakIn_GPIO_Port, &GPIO_InitStruct);
}

static void HiRlyInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  HAL_GPIO_WritePin(Lrly_GPIO_Port, Lrly_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Hrly_GPIO_Port, Hrly_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = Lrly_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Lrly_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = Hrly_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Hrly_GPIO_Port, &GPIO_InitStruct);
}

static void ObsBtnInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  GPIO_InitStruct.Pin = OBSBtn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(OBSBtn_GPIO_Port, &GPIO_InitStruct);
}

void GPIOInit(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  RunLedInit();
  CC1InInit();
  EleLockInit();
  IMDPowerENInit();
  OneKeyInit();
  VbatIsChargInit();
  BMSPowerENInit();
  IMDCheckCtlInit();
  LeakCheckInit();
  HiRlyInit();
  ObsBtnInit();
}

