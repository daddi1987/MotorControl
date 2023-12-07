/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
//#include "stdbool.h"
//#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define THREAD_STACK_SIZE  1024
uint8_t thread_stack1[THREAD_STACK_SIZE];
TX_THREAD thread_ptr1;
uint8_t thread_stack2[THREAD_STACK_SIZE];
TX_THREAD thread_ptr2;
uint8_t HEADER2[14] = {'\0'};
uint8_t HEADER3[16] = {'\0'};
//uint8_t HEADER4[16] = {'\0'};
uint8_t MSG[10] = {'\0'};
uint8_t CR[4] = {'\0'};
uint32_t Counter = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
void my_Thread_entry_1(ULONG initial_input);  // INIZIALIZZARE IL PRIMO TRADE
void my_Thread_entry_2(ULONG initial_input);  // INIZIALIZZARE IL SECONDO TRADE

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  tx_thread_create(&thread_ptr1,"my_First_trade",my_Thread_entry_1,0x1234,thread_stack1,THREAD_STACK_SIZE,
   		  15,15,1,TX_AUTO_START);  //RICHIAMARE IL PRIMO TRADE
  tx_thread_create(&thread_ptr2,"my_Second_trade",my_Thread_entry_2,0x1234,thread_stack2,THREAD_STACK_SIZE,
   		  15,15,1,TX_AUTO_START);  //RICHIAMARE IL SECONDO TRADE
  (void)byte_pool;
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */
  sprintf(HEADER2, "Init ThreadX");
  HAL_UART_Transmit(&huart2, HEADER2, sizeof(HEADER2), 100);
  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
void my_Thread_entry_1(ULONG initial_input)
{
	while(1)
	{
		if(TickMotion == true)
		{
		  Counter = Counter+1;
		  TickMotion = false;
		  /*
		  //HAL_GPIO_TogglePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin);
		  HAL_GPIO_WritePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin, 1);
		  HAL_Delay(1);
		  //HAL_GPIO_TogglePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin);
		  HAL_GPIO_WritePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin, 0);
		  //HAL_Delay(100);
		  sprintf(HEADER4, "Inside Trade 1\r\n");
		  HAL_UART_Transmit(&huart2, HEADER4, sizeof(HEADER4), 100);
		  */
		  //Indispensable for Send Value without error to row empty
		  //HAL_Delay(1);

	}
}
}

void my_Thread_entry_2(ULONG initial_input)
{
	while(1)
	{
		  HAL_GPIO_TogglePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin);
		  //HAL_GPIO_WritePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin, 0);
		  //HAL_Delay(1000);
		  //HAL_GPIO_TogglePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin);
		  //HAL_GPIO_WritePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin, 1);
		  //HAL_Delay(1000);
		  sprintf(MSG, "%d;Sx",Counter);
		  HAL_UART_Transmit(&huart2, MSG, sizeof(MSG), 0xFFFF);
		  sprintf(CR,"\r\n");   											//Indispensable for Send Value without error to row empty
		  HAL_UART_Transmit(&huart2, CR, sizeof(CR), 0xFFFF);
		  HAL_Delay(1000);
	}
}
/* USER CODE END 1 */
