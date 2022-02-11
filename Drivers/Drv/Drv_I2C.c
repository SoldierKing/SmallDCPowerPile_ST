#include "Drv_I2C.h"

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

#define  ADDR_AT24C512BN_Write   (0xA0)
#define  ADDR_AT24C512BN_Read    (0xA1)


/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    //Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    //Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}


int16_t AT24C512_Write_nBytes(uint8_t *data, uint16_t addr, uint16_t length)

{

    if( HAL_I2C_Mem_Write( &hi2c1, ADDR_AT24C512BN_Write, addr, 
                          I2C_MEMADD_SIZE_16BIT, data, length, 1000 ) == HAL_OK )

        return 0;

    else

        return -1;

}

 

int16_t AT24C512_Read_nBytes(uint8_t *data, uint16_t addr, uint16_t length)

{

    if( HAL_I2C_Mem_Read( &hi2c1, ADDR_AT24C512BN_Write, addr, 
                         I2C_MEMADD_SIZE_16BIT, data, length, 1000 ) == HAL_OK )

        return 0;

    else

        return -1;

}

void I2CInit(void)
{

  MX_I2C1_Init();
  MX_I2C2_Init();
}

void I2CTest(void)
{

    uint8_t write_buff[18] = "read & write test";

    uint8_t read_buff[18] = {0};

    
    AT24C512_Write_nBytes( &write_buff[0], 0, 10 );

    HAL_Delay( 10 ); //这里的延时是必要的

    AT24C512_Write_nBytes( &write_buff[10], 10, 8 );

    HAL_Delay( 10 );

    AT24C512_Read_nBytes( read_buff, 0, 18 );
    
}


