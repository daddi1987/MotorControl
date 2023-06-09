/* USER CODE BEGIN Header */
// The Developper for this code is Davide Zuanon contact on d.zuanon87@gmail.com
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim6;
UART_HandleTypeDef huart2;

int32_t EncoderCount = 0;

uint8_t MSG[100] = {'\0'};
uint8_t CR[2] = {'\0'};
uint8_t Prefix[3] = {'\0'};
uint8_t Sufix[3] = {'\0'};
uint8_t HEADER1[35] = {'\0'};
uint8_t HEADER2[35] = {'\0'};
uint8_t HEADER3[35] = {'\0'};
uint8_t HEADER4[35] = {'\0'};
uint8_t HEADER5[35] = {'\0'};
uint16_t PositionSend[7];
uint16_t KinematicPositionSend[7];
uint16_t SpeedSend[7];
uint16_t SpeedUnitSend[7];

//---------------Decleare variables for filter Speed----------------
uint8_t FilterSpeedEnable = 0;
float RPSSpeedFilter = 0;
float RPSSpeedFilterPrev = 0;
float KinematicSpeedRPSToFiler = 0.0;
uint8_t FrequencySpeedFilter = 45;
uint8_t FrequencyCase = 1;
float b_i;
float a_i;


uint8_t rot_new_state = 0;
uint8_t rot_old_state = 0;

//Declare variables for Calculate Position Encoder
float PositionMotor;
int32_t EncoderPosition = 0;
float EncoderPositionFloat = 0.0;
uint16_t EncoderPulse = 2048;
uint16_t RevoluctionFactor = 1;
float KinematicPositionUnit = 0.0;
float KinematicSpeedRPS = 0.0;
float KinematicSpeedRPM = 0.0;
float KinematicSpeedRPSold = 0.0;
float KinematicSpeedUnit = 0.0;
uint32_t TM6_DiffCaunter;
uint32_t TM6_OldValue;
uint8_t IncrementSpeedCheckDouble = 0;

char uart_buf[50];
int uart_buf_len;
int16_t TM6_Currentvalue;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM6_Init(void);
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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

  sprintf(Prefix, "Px;");
  HAL_UART_Transmit(&huart2, Prefix, sizeof(Prefix), 100);
  sprintf(HEADER1, "PulseEncoder;");
  HAL_UART_Transmit(&huart2, HEADER1, sizeof(HEADER1), 100);
  sprintf(HEADER2, "PositionMotor;");
  HAL_UART_Transmit(&huart2, HEADER2, sizeof(HEADER2), 100);
  sprintf(HEADER3, "RevolutionMotor;");
  HAL_UART_Transmit(&huart2, HEADER3, sizeof(HEADER3), 100);
  sprintf(HEADER4, "KinematicPositionUnit;");
  HAL_UART_Transmit(&huart2, HEADER4, sizeof(HEADER4), 100);
  sprintf(HEADER5, "KinematicSpeed[Rpm];");
  HAL_UART_Transmit(&huart2, HEADER5, sizeof(HEADER5), 100);
  sprintf(Sufix, "Sx;\n");
  HAL_UART_Transmit(&huart2, Sufix, sizeof(Sufix), 100);
  HAL_Delay(1000);

  GetConstantFilter();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	    /* USER CODE END WHILE */

	  	 if((KinematicSpeedRPSold == KinematicSpeedRPS) && (IncrementSpeedCheckDouble >=10))
	  	 {
	  		KinematicSpeedRPS = 0.0;
	  		KinematicSpeedRPM = 0.0;
	  		KinematicSpeedUnit = 0.0;
	  		KinematicSpeedRPSold = KinematicSpeedRPS;
	  	 }
	  	 else
	  	 {
	  		KinematicSpeedRPSold = KinematicSpeedRPS;
	  		IncrementSpeedCheckDouble++;
	  	 }

	     //TM6_Currentvalue = __HAL_TIM_GET_COUNTER(&htim6);

	     sprintf(MSG, "Px;%d;%d;%.3f;%.3f;%.3f;%.3f;%.3f;Sx\r",
				 EncoderCount,
				 EncoderPosition,
				 PositionMotor,
				 KinematicPositionUnit,
				 KinematicSpeedRPS,
				 KinematicSpeedRPM,
				 KinematicSpeedUnit);
	 		 HAL_UART_Transmit(&huart2, MSG, sizeof(MSG), 0xFFFF);
	 	 sprintf(CR,"\n");   											//Indispensable for Send Value without error to row empty
	 		 HAL_UART_Transmit(&huart2, CR, sizeof(CR), 0xFFFF);        //Indispensable for Send Value without error to row empty

		 HAL_Delay(1);
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 41;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */
  HAL_TIM_Base_Start_IT(&htim6); // Start Timer
  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200; //115200 Baudrate
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|Enable_A_PhaseStepper_Pin|Enable_B_PhaseStepper_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, IN2_PhaseA_Pin|IN1_PhaseA_Pin|IN2_PhaseB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(IN1_PhaseB_GPIO_Port, IN1_PhaseB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Encoder1_Count_Pin Encoder1_Direct_Pin */
  GPIO_InitStruct.Pin = Encoder1_Count_Pin|Encoder1_Direct_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin Enable_A_PhaseStepper_Pin Enable_B_PhaseStepper_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|Enable_A_PhaseStepper_Pin|Enable_B_PhaseStepper_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : IN2_PhaseA_Pin IN1_PhaseA_Pin IN2_PhaseB_Pin */
  GPIO_InitStruct.Pin = IN2_PhaseA_Pin|IN1_PhaseA_Pin|IN2_PhaseB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : IN1_PhaseB_Pin */
  GPIO_InitStruct.Pin = IN1_PhaseB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IN1_PhaseB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Encoder1_Index_Pin */
  GPIO_InitStruct.Pin = Encoder1_Index_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Encoder1_Index_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

uint8_t rot_get_state() {
	return (uint8_t)((HAL_GPIO_ReadPin(GPIOA, Encoder1_Direct_Pin) << 1)
                | (HAL_GPIO_ReadPin(GPIOA, Encoder1_Count_Pin)));
}

// ---------------------------------EXTERNAL INTERRUPT FOR ENCODER MOTOR--------------------------------------
/* Use Interrupt callback for determinate the count encoder and direction
 * The Encoder Have a 2048 pulse/rot, in this implementation do it increment counter POSITION four time at impulse
 * Example 1rev/8192 pulse
 * PIN ENCODER A0 COUNT AND A1 DIRECTION
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == Encoder1_Count_Pin || GPIO_Pin == Encoder1_Direct_Pin) {

		rot_new_state = rot_get_state();

		//DBG("%d:%d", rot_old_state, rot_new_state);

		// Check transition
		if (rot_old_state == 3 && rot_new_state == 2) {        // 3 -> 2 transition
			EncoderCount++;
		} else if (rot_old_state == 2 && rot_new_state == 0) { // 2 -> 0 transition
			EncoderCount++;
		} else if (rot_old_state == 0 && rot_new_state == 1) { // 0 -> 1 transition
			EncoderCount++;
		} else if (rot_old_state == 1 && rot_new_state == 3) { // 1 -> 3 transition
			EncoderCount++;
		} else if (rot_old_state == 3 && rot_new_state == 1) { // 3 -> 1 transition
			EncoderCount--;
		} else if (rot_old_state == 1 && rot_new_state == 0) { // 1 -> 0 transition
			EncoderCount--;
		} else if (rot_old_state == 0 && rot_new_state == 2) { // 0 -> 2 transition
			EncoderCount--;
		} else if (rot_old_state == 2 && rot_new_state == 3) { // 2 -> 3 transition
			EncoderCount--;
		}

		rot_old_state = rot_new_state;
		Calculate_Rotation(EncoderPulse,RevoluctionFactor);
	}
}
// ---------------------------------END EXTERNAL INTERRUPT FOR ENCODER MOTOR--------------------------------------
/*
 *
 *
 *
 */
// ----------------------------------------CALCULATE REV TO FACTOR --------------------------------------
/* Calculate Revolution to Factor
 *
 */
void Calculate_Rotation(uint16_t EncoderPulseSet,uint16_t RevoluctionFactorSet)
{
EncoderPosition = EncoderCount/4.0;   // Single Event Encoder 1*4 in Single Counter
EncoderPositionFloat = EncoderPosition; // Single Counter Encoder
PositionMotor = EncoderPositionFloat/EncoderPulseSet;
KinematicPositionUnit = RevoluctionFactorSet * PositionMotor;

TM6_Currentvalue = __HAL_TIM_GET_COUNTER(&htim6); // Get current time (microseconds)

if(TM6_Currentvalue >= TM6_OldValue)
{
	TM6_DiffCaunter = (TM6_Currentvalue - TM6_OldValue); // Calculate time from count to count
	if (FilterSpeedEnable == 1)  // 25Hz CutOff Low-Pass Filter
	{
		GetConstantFilter();
		KinematicSpeedRPSToFiler = ((1000000.0/TM6_DiffCaunter)/(EncoderPulseSet*4)); //Calculate RPS speed From microsecond to second
		//KinematicSpeedRPS = ((0.854*RPSSpeedFilter) + (0.0728*KinematicSpeedRPSToFiler) + (0.0728*RPSSpeedFilterPrev));
		KinematicSpeedRPS = ((b_i*RPSSpeedFilter) + (a_i*KinematicSpeedRPSToFiler) + (a_i*RPSSpeedFilterPrev));
		KinematicSpeedRPM = (KinematicSpeedRPS * 60.0); //Calculate RPM Speed
		KinematicSpeedUnit = (KinematicSpeedRPM * RevoluctionFactorSet);
		TM6_OldValue = TM6_Currentvalue; // Save to old value
		HAL_GPIO_TogglePin (GPIOA, LD2_Pin);
		RPSSpeedFilterPrev = KinematicSpeedRPSToFiler;
		RPSSpeedFilter = KinematicSpeedRPS;
		//HAL_Delay(1);
	}
	else
	{
		KinematicSpeedRPS = ((1000000.0/TM6_DiffCaunter)/(EncoderPulseSet*4)); //Calculate RPS speed From microsecond to second
		KinematicSpeedRPM = (KinematicSpeedRPS * 60.0); //Calculate RPM Speed
		KinematicSpeedUnit = (KinematicSpeedRPM * RevoluctionFactorSet);
		TM6_OldValue = TM6_Currentvalue; // Save to old value
		//IncrementSpeedCheckOld = IncrementSpeedCheck;
		//IncrementSpeedCheck++;
		//TM6_Currentvalue = 0; //Reset Current Value Counter
		HAL_GPIO_TogglePin (GPIOA, LD2_Pin);
	}
}
else
{
	TM6_OldValue = TM6_Currentvalue;
}

}
// -------------------------------------END CALCULATE REV TO FACTOR --------------------------------------
/* Calculate Revolution to Factor
 *
 */
//----------------------------DA CONTROLLARE NON FUNZIONANTE--------------------------
void GetConstantFilter()
{
	/*float b0 = 0.0;
	float b1 = 0.0;
	float b2 = 0.0;
	float a1 = 0.0;
	float a2 = 0.0;
    const double ita =1.0/ tan(42*0.25);  //Sampling Freq: 10kHz   Cut-off Freq: 1kHz
    const double q=sqrt(2.0);
    b0 = 1.0 / (1.0 + q*ita + ita*ita);
    b1= 2*b0;
    b2= b0;
    a1 = 2.0 * (ita*ita - 1.0) * b0;
    a2 = -(1.0 - q*ita + ita*ita) * b0;
    */

	switch (FrequencyCase) {

	  case 1:
		if(FrequencySpeedFilter <= 5)
		{
			b_i = 0.96906992;
			a_i = 0.01546504;
			//FrequencySpeedFilter = 1;
			break;
		}
		else
		{
			FrequencyCase = 2;
		}


	  case 2:
		if ((FrequencySpeedFilter >= 6)&&(FrequencySpeedFilter <= 10))
		{
		    b_i = 0.93908194;
		    a_i = 0.03045903;
		    //FrequencySpeedFilter = 2;
		    break;
		}
		else
		{
			FrequencyCase = 3;
		}


	  case 3:
		if ((FrequencySpeedFilter >= 11)&&(FrequencySpeedFilter <= 15))
		{
		    b_i = 0.90999367;
		    a_i = 0.04500317;
		    //FrequencySpeedFilter = 3;
		    break;
		}
		else
		{
			FrequencyCase = 4;
		}


	  case 4:
		if ((FrequencySpeedFilter >= 16)&&(FrequencySpeedFilter <= 25))
		{
		    b_i = 0.85435899;
		    a_i = 0.07282051;
		    //FrequencySpeedFilter = 4;
		    break;
		}
		else
		{
			FrequencyCase = 5;
		}


	  case 5:
		if ((FrequencySpeedFilter >= 26)&&(FrequencySpeedFilter <= 35))
		{
		    b_i = 0.80187364;
		    a_i = 0.09906318;
		    //FrequencySpeedFilter = 5;
		    break;
		}
		else
		{
			FrequencyCase = 6;
		}


	  case 6:
		if ((FrequencySpeedFilter >= 36)&&(FrequencySpeedFilter <= 45))
		{
		    b_i = 0.75227759;
		    a_i = 0.1238612;
		    //FrequencySpeedFilter = 6;
		    break;
		}
		else
		{
			FrequencyCase = 7;
		}


	  case 7:
		if ((FrequencySpeedFilter >= 46)&&(FrequencySpeedFilter <= 55))
		{
		    b_i = 0.70533864;
		    a_i = 0.14733068;
		    //FrequencySpeedFilter = 7;
		    break;
		}
		else
		{
			FrequencyCase = 8;
		}


	  case 8:
		if ((FrequencySpeedFilter >= 56)&&(FrequencySpeedFilter <= 65))
		{
		    b_i = 0.66084882;
		    a_i = 0.16957559;
		    //FrequencySpeedFilter = 8;
		    break;
		}
		else
		{
			FrequencyCase = 9;
		}


	  case 9:
		if ((FrequencySpeedFilter >= 66)&&(FrequencySpeedFilter <= 75))
		{
		    b_i = 0.61862133;
		    a_i = 0.19068933;
		    //FrequencySpeedFilter = 9;
		    break;
		}
		else
		{
			FrequencyCase = 10;
		}


	  case 10:
		if ((FrequencySpeedFilter >= 76)&&(FrequencySpeedFilter <= 85))
		{
		    b_i = 0.57848789;
		    a_i = 0.21075605;
		    //FrequencySpeedFilter = 10;
		    break;
		}
		else
		{
			FrequencyCase = 11;
		}


	  case 11:
		if ((FrequencySpeedFilter >= 86)&&(FrequencySpeedFilter <= 95))
		{
		    b_i = 0.5402965;
		    a_i = 0.22985175;
		    //FrequencySpeedFilter = 11;
		    break;
		}
		else
		{
			FrequencyCase = 12;
		}


	  case 12:
	  		if (FrequencySpeedFilter >= 96)
	  		{
	  		    b_i = 0.50390953;
	  		    a_i = 0.24804523;
	  		    //FrequencySpeedFilter = 10;
	  		    break;
	  		}
	  		else
	  		{
	  			FrequencyCase = 1;
	  		}

}
}
//----------------------------FINE  CONTROLLARE NON FUNZIONANTE--------------------------

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
