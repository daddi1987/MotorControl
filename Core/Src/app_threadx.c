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
#include "main.h"
//#include "stdbool.h"
//#include "stdio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */
#define THREAD_STACK_SIZE  1024

uint8_t thread_stack1[THREAD_STACK_SIZE];
TX_THREAD thread_ptr1;
uint8_t thread_stack2[THREAD_STACK_SIZE];
TX_THREAD thread_ptr2;
uint8_t thread_stack3[THREAD_STACK_SIZE];
TX_THREAD thread_ptr3;


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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */
void my_Thread_entry_1(ULONG initial_input);  // Trade Principale per controllo motore 20KH
void my_Thread_entry_2(ULONG initial_input);  // trade senìcondario per gestione della diagnostica a 1KH
void my_Thread_entry_3(ULONG initial_input);  // trade senìcondario per gestione della seriale a 1KH
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
  (void)byte_pool;
  /* USER CODE END App_ThreadX_Init */
  tx_thread_create(&thread_ptr1,"my_First_trade",my_Thread_entry_1,0x1234,thread_stack1,THREAD_STACK_SIZE,
		  15,15,1,TX_AUTO_START);  //RICHIAMARE IL PRIMO TRADE

  tx_thread_create(&thread_ptr2,"my_Second_trade",my_Thread_entry_2,0x1234,thread_stack2,THREAD_STACK_SIZE,
		  14,14,1,TX_AUTO_START);  //RICHIAMARE IL SECONDO TRADE

  tx_thread_create(&thread_ptr3,"my_Third_trade",my_Thread_entry_3,0x1234,thread_stack3,THREAD_STACK_SIZE,
		  13,13,1,TX_AUTO_START);  //RICHIAMARE IL SECONDO TRADE

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

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

void my_Thread_entry_1(ULONG initial_input)   // TRADE CONTROLLO MOTORE
{
	while(1)
	{
		  //HAL_GPIO_TogglePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin);
		  HAL_GPIO_WritePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin, 1);
		  HAL_Delay(1000);
		  //HAL_GPIO_TogglePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin);
		  HAL_GPIO_WritePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin, 0);
		  HAL_Delay(1000);
	}
}


void my_Thread_entry_2(ULONG initial_input)    //TRADE DIAGNOSTICA
{
	while(1)
	{
		  //HAL_GPIO_TogglePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin);
		  HAL_GPIO_WritePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin, 0);
		  HAL_Delay(250);
		  //HAL_GPIO_TogglePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin);
		  HAL_GPIO_WritePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin, 1);
		  HAL_Delay(250);
	}
}

void my_Thread_entry_3(ULONG initial_input)  // TRADE SERIALE
{
	while(1)
	{
		  //HAL_GPIO_TogglePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin);
		  HAL_GPIO_WritePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin, 0);
		  HAL_Delay(250);
		  //HAL_GPIO_TogglePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin);
		  HAL_GPIO_WritePin(LD2_Green_Led_GPIO_Port, LD2_Green_Led_Pin, 1);
		  HAL_Delay(250);
	}
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
