/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

#include <stdbool.h>
extern uint8_t HEADER2[14];
extern uint8_t HEADER3[16];
extern bool TickMotion;
extern bool TickSerial;
extern bool TickDiag;
extern uint32_t SerialTX;
extern uint32_t Counter;
extern uint32_t CouterSerial;
extern uint32_t CouterSerialOld;
extern uint32_t CounterDiag;
extern int16_t EncoderCount;
extern uint8_t rot_new_state;
extern uint8_t rot_old_state;
extern uint16_t EncoderPulse;
extern uint16_t RevoluctionFactor;
extern float KinematicPositionUnit;
extern float EncoderSpeed;
extern float EncoderSpeedRPM;
extern float EncoderSpeedRPSold;
extern float EncoderSpeedUnit;
extern float DiffTickClockMotion;
extern uint32_t TM1_OldValue;
extern int16_t EncoderPosition;
extern float EncoderPositionFloat;
extern float PositionMotor;
extern uint8_t FilterSpeedEnable;
extern float RPSSpeedFilter;
extern float RPSSpeedFilterPrev;
extern float EncoderSpeedRPSToFiler;
extern uint8_t FrequencySpeedFilter;
extern uint8_t FrequencyCase;
extern float b_i;
extern float a_i;
extern uint32_t TickClockMotion;
extern float ActualPosition;
extern float ActualSpeedRPM;
extern float ActualSpeed;
extern uint32_t timer_counter;
extern int16_t EncoderCounter;
extern uint32_t CounterSpeed;
extern uint32_t index2;

typedef struct{
	int16_t velocity;
	int64_t position;
	uint32_t last_Encoder_counter_value;
}encoder_instance;


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Green_Led_Pin GPIO_PIN_5
#define LD2_Green_Led_GPIO_Port GPIOA
#define Encoder_Index_Pin GPIO_PIN_10
#define Encoder_Index_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
