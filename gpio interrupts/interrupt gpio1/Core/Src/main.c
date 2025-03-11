#include "stm32f4xx_hal.h"   // Adjust include for your MCU family

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
  /* Initialize the HAL Library */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* Infinite loop - LED control is handled in the interrupt callback */
  while (1)
  {
    /* Optionally, you can add low power code here */
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable GPIO Ports Clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configure PA8 as output for the LED */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure PC9 as input with interrupt on both rising and falling edges.
     Assuming the switch is active low, we use a pull-up resistor. */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Enable and set EXTI line (PC9 falls under EXTI9_5_IRQn) Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_9)
  {
    /* Read the state of PC9. If the switch is pressed, PC9 will be LOW */
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_RESET)
    {
      /* Button pressed: turn LED on */
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    }
    else
    {
      /* Button released: turn LED off */
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    }
  }
}

/**
  * @brief  This function handles EXTI line[9:5] interrupts.
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

/**
  * @brief  System Clock Configuration.
  *         This is a placeholder. Configure the clock according to your system.
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Insert your clock configuration code here */
}
