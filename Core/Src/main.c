/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Ext_IO.h"

#include "Const.h"
#include "G_Value.h"
#include "Config.h"
#include "Parameter.h"

#include "Uart.h"
#include "Fan.h"

#include "Process.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
volatile uint16_t Adc_Temp[16];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t FlagFirmwareWROn = OFF;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == FAN_RPM_Pin)
	{
		tFan.RpmCurrCnt++;
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART1)
	{
/*
		tUartMsg.uOldPos = tUartMsg.uNewPos;

		if (tUartMsg.uOldPos+Size > MAX_RX_RING_BUFF_SIZE)
		{
			uint8_t DataToCopy = MAX_RX_BUFF_SIZE-tUartMsg.uOldPos;
			memcpy ((uint8_t *)tUartMsg.RingRxTemp + tUartMsg.uOldPos, tUartMsg.RxTemp, DataToCopy);

			tUartMsg.uOldPos  = 0;
			memcpy ((uint8_t *)tUartMsg.RingRxTemp, (uint8_t *)tUartMsg.RxTemp + DataToCopy, (Size - DataToCopy));
			tUartMsg.uNewPos = (Size - DataToCopy);
		}

		else
		{
			memcpy ((uint8_t *)tUartMsg.RingRxTemp + tUartMsg.uOldPos, tUartMsg.RxTemp, Size);
			tUartMsg.uNewPos = Size + tUartMsg.uOldPos;
		}
*/
		//memset(tUartMsg.RingRxTemp, 0, sizeof(tUartMsg.RingRxTemp));
		//tUartMsg.uOldPos = 0;
		//tUartMsg.uNewPos = 0;
		/* start the DMA again */
//		HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)tUartMsg.RxTemp, MAX_RX_BUFF_SIZE);
//		__HAL_DMA_ENABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

		memcpy ((uint8_t *)tUartMsg.RxTemp, (uint8_t *)tUartMsg.RingRxTemp, Size);

		tUartMsg.FlagMsgRcvOn = ON;
		tUartMsg.RxTempCnt = Size;

		memset(tUartMsg.RingRxTemp, 0, sizeof(tUartMsg.RingRxTemp));

		HAL_UART_DMAStop(&huart1);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *) tUartMsg.RingRxTemp, MAX_RX_BUFF_SIZE);
       __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{

	if(ON == tSysState.FlagGetAdcOn)
	{
		tSiPmCtrl.CurrAdcVal 	= (uint16_t)(Adc_Temp[0]);
		tPdCtrl.CurrAdcVal  	= (uint16_t)(Adc_Temp[1]);
		tLdCtrl.CurrAdcVal 		= (uint16_t)(Adc_Temp[2]);
		tHvCtrl.CurrAdcVal 		= (uint16_t)(Adc_Temp[3]);


		tPdCtrl.ChkCount++;

		//HAL_ADC_Start_DMA(&hadc1, Adc_Temp, 5);
	}

	tSysState.Temp			= (uint16_t)(Adc_Temp[4]);

}


void UART_Transmit_Proc(UART_HandleTypeDef* hUart)
{
	if(ON == tMsgPkt.FlagTxPktOn)
	{
		HAL_UART_Transmit(hUart, tUartMsg.RingTxTemp, tUartMsg.TxTempCnt,100);
		UART_TxInit();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		tSysTime.FlagSysTickOn = ON;
	}
}

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
  HAL_Init();

  /* USER CODE BEGIN Init */
  memset(Adc_Temp, 0, sizeof(Adc_Temp));
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  PROC_Init();
  EXT_Init();
  FAN_Init();
  UART_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  PARAM_Init(&hi2c1);
  PARAM_Process();

  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)tUartMsg.RingRxTemp, MAX_RX_BUFF_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  HAL_TIM_Base_Start_IT(&htim2);

/*
  uint8_t temp[32] = "Hello World!";
  memcpy(tUartMsg.RingTxTemp, temp, 13);
  tUartMsg.TxTempCnt = 13;
  tMsgPkt.FlagTxPktOn = ON;
*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 SYSTEM_Process();
	 UART_Transmit_Proc(&huart1);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
