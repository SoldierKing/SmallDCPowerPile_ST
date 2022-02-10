#include "Drv_ADC.h"



DMA_HandleTypeDef hdma_adc1;
ADC_HandleTypeDef hadc1;


static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  //ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.NbrOfDiscConversion = 1;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Configure Regular Channel
  
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Configure Regular Channel
  
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Configure Regular Channel
  
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Configure Regular Channel
  
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Configure Regular Channel
  
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Configure Regular Channel
  
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Configure Regular Channel
  
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Configure Regular Channel
  
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}


/**
  * Enable DMA controller clock
  */

static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

void ADCInit(void)
{
  MX_ADC1_Init();
  //MX_DMA_Init();
  //ADC 校准
  HAL_ADCEx_Calibration_Start(&hadc1);
}

void ADCStart(u32 *AdcBuff)
{
  HAL_ADC_Start(&hadc1);    
  HAL_ADC_Start_DMA(&hadc1, (u32*)AdcBuff, EnumVadc_MAX);
}

//ch(1-6Chnnal),times(读取次数)
uint32_t ADC_Get_Average(EnumADCType ch,uint8_t times)
{
  ADC_ChannelConfTypeDef sConfig;		//通道初始化
  uint32_t value_sum=0;	
  uint8_t i;
  switch(ch)	//选择ADC通道
  {
    case EnumVpdc_AD0:sConfig.Channel = ADC_CHANNEL_10;break;
    case EnumVndc_AD1:sConfig.Channel = ADC_CHANNEL_11;break;	
    case EnumVhdc_AD2:sConfig.Channel = ADC_CHANNEL_12;break;
    case EnumVddc_AD3:sConfig.Channel = ADC_CHANNEL_13;break;
    case EnumVt1_ADC:sConfig.Channel = ADC_CHANNEL_0;break;
    case EnumVt2_ADC:sConfig.Channel = ADC_CHANNEL_1;break;
    case EnumVin_VOL:sConfig.Channel = ADC_CHANNEL_5;break;
    case EnumVbat_VOL:sConfig.Channel = ADC_CHANNEL_6;break;
  }
  sConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;		//采用周期239.5周期
  sConfig.Rank = 1;
  HAL_ADC_ConfigChannel(&hadc1,&sConfig);											
  for(i=0;i<times;i++)
  {
    HAL_ADC_Start(&hadc1);	//启动转换
    HAL_ADC_PollForConversion(&hadc1,30); //等待转化结束
    value_sum += HAL_ADC_GetValue(&hadc1);//求和					
    HAL_ADC_Stop(&hadc1);	//停止转换
  }
  return value_sum/times;	//返回平均值
}

void GetAdcBuffValue(u32 *AdcBuff)
{
  for (u8 i=EnumVpdc_AD0;i < EnumVadc_MAX;i++)
    AdcBuff[i] = ADC_Get_Average(i,10);
 
}
