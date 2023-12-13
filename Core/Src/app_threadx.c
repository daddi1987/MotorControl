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
uint8_t thread_stack3[THREAD_STACK_SIZE];
TX_THREAD thread_ptr3;
uint8_t HEADER2[14] = {'\0'};
uint8_t HEADER3[16] = {'\0'};
//uint8_t HEADER4[16] = {'\0'};
uint8_t MSG[86] = {'\0'};
uint8_t CR[4] = {'\0'};
uint32_t Counter = 0;
uint32_t CouterSerial = 0;
uint32_t CounterDiag = 0;
uint32_t CounterDiagSerial = 0;
uint32_t SerialTX = 0;
uint32_t ThransholdSerialTX = 10000;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
void my_Thread_entry_1(ULONG initial_input);  // INIZIALIZZARE IL PRIMO TRADE
//void my_Thread_entry_2(ULONG initial_input);  // INIZIALIZZARE IL SECONDO TRADE
//void my_Thread_entry_3(ULONG initial_input);  // INIZIALIZZARE IL TERZO TRADE
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
   		  3,3,1,TX_AUTO_START);  //RICHIAMARE IL PRIMO TRADE
  /*tx_thread_create(&thread_ptr2,"my_Second_trade",my_Thread_entry_2,0x1234,thread_stack2,THREAD_STACK_SIZE,
   		  3,3,1,TX_AUTO_START);  //RICHIAMARE IL SECONDO TRADE
  tx_thread_create(&thread_ptr3,"my_Third_trade",my_Thread_entry_3,0x1234,thread_stack3,THREAD_STACK_SIZE,
     	  3,3,1,TX_AUTO_START);  //RICHIAMARE IL SECONDO TRADE
     	  */
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
		if(TickSerial == true)
					{
					  TickSerial = false;
					  if (SerialTX >= ThransholdSerialTX)
					  {
						  SerialTX = 0;
						  SerialTX++;
					  }
					  else
					  {
						  SerialTX++;
					  }
						  sprintf(MSG,"Px,%d;%d;%d;%.3f;%.3f;%.3f;Sx",SerialTX,Counter,EncoderCount,
								  ActualPosition,ActualSpeedRPM,ActualSpeed);
						  HAL_UART_Transmit(&huart2, MSG, sizeof(MSG), 0xFFFF);
						  sprintf(CR,"\r\n");   // sprintf(CR,"\r\n"); 	//Ritorno a capo e a destra
						  HAL_UART_Transmit(&huart2, CR, sizeof(CR), 0xFFFF);
					 }

	}
}
/*
void my_Thread_entry_2(ULONG initial_input)
{
	while(1)
	{
		if(TickDiag == true)
		{
		  CounterDiag++;
		  if((TickSerial == true)&&(TickDiag == true))
		  {
			  CounterDiagSerial = CounterDiag;
			  CounterDiag = 0;
		  }
		 TickDiag = false;
		}
	}
}
*/

/* USER CODE END 1 */
