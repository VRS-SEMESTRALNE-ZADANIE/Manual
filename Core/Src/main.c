/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "tim1.h"
#include "../MD25/md25.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

uint16_t adc_buffer[ADC_BUFFER_SIZE];
uint16_t STLAC_buffer[ADC_BUFFER_SIZE];
uint8_t a=0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */
  uint8_t a=0;
  uint8_t b=0;
  uint8_t aloha[2];
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    /* USER CODE END 2 */

    while(!LL_USART_IsEnabled(USART1)) {
        // USART1 is enabled
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
       MD25_init();
        LL_mDelay(10);
      a=get_mode();
        get_speed_Both(aloha);
       set_mode(2);
      // set_speed_Both(10,10);
    Start_ADC_Conversion();
  while (1)
  {

    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */


	 if(Prepinanie()) {
		 Check_Input_Pins();

	 }
	 else {
	adc_buffer[0]=adcConvertedDataBuffer[0]/4;
	adc_buffer[1]=adcConvertedDataBuffer[1]/4;
	set_speed_Both(adc_buffer[0],adc_buffer[1]);
	get_speed_Both(aloha);
	 }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_4);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SetSystemCoreClock(8000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
  LL_RCC_SetTIMClockSource(LL_RCC_TIM1_CLKSOURCE_PCLK2);
  LL_RCC_SetADCClockSource(LL_RCC_ADC12_CLKSRC_PLL_DIV_1);
}

/* USER CODE BEGIN 4 */
void Check_Input_Pins(void)
{
	STLAC_buffer[0]=128;
	STLAC_buffer[1]=128;

	//DPLAVA
    if (!LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_0))
    {
    	STLAC_buffer[1]=STLAC_buffer[1]-40;


    }


    if (!LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_6))
    {
    	STLAC_buffer[1]=STLAC_buffer[1]+40;
    }


    if (!LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_7))
    {
    	STLAC_buffer[0]=STLAC_buffer[0]-50;
    }

    // Kontrola stavu pinu na GPIOA
    if (!LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_12))
    {
    	STLAC_buffer[0]=STLAC_buffer[0]+50;
    }
    set_speed_Both(STLAC_buffer[0],STLAC_buffer[1]);
}

uint8_t Prepinanie(void)
{
	 if (!LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_1) && a==0)
	    {
	    	a=1;
	    	while(!LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_1)){ }


	    }
	 else if(!LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_1)&& a ==1)
	    {
	    	a=0;
	    	while(!LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_1)){  }

	    }

	 if (a==0){
		 return 1;

	 }
	 else{
		 return 0;
	 }

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
