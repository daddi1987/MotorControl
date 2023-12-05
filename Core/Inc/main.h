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
#include "stdint.h"
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define Encoder1_Count_Pin GPIO_PIN_0
#define Encoder1_Count_GPIO_Port GPIOA
#define Encoder1_Count_EXTI_IRQn EXTI0_IRQn
#define Encoder1_Direct_Pin GPIO_PIN_1
#define Encoder1_Direct_GPIO_Port GPIOA
#define Encoder1_Direct_EXTI_IRQn EXTI1_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define IN2_PhaseA_Pin GPIO_PIN_10
#define IN2_PhaseA_GPIO_Port GPIOB
#define IN1_PhaseB_Pin GPIO_PIN_7
#define IN1_PhaseB_GPIO_Port GPIOC
#define Enable_A_PhaseStepper_Pin GPIO_PIN_8
#define Enable_A_PhaseStepper_GPIO_Port GPIOA
#define Enable_B_PhaseStepper_Pin GPIO_PIN_9
#define Enable_B_PhaseStepper_GPIO_Port GPIOA
#define Encoder1_Index_Pin GPIO_PIN_10
#define Encoder1_Index_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define IN1_PhaseA_Pin GPIO_PIN_4
#define IN1_PhaseA_GPIO_Port GPIOB
#define IN2_PhaseB_Pin GPIO_PIN_6
#define IN2_PhaseB_GPIO_Port GPIOB

extern uint8_t TickSerial;
extern float EncoderSpeedRPSold;
extern uint8_t IncrementSpeedCheckDouble;
extern float EncoderSpeedRPS;
extern float EncoderSpeedRPM;
extern float EncoderSpeedRPSold;
extern float EncoderSpeedUnit;
extern uint8_t MSG[200];
extern int32_t EncoderCount;
extern int32_t EncoderPosition;
extern float PositionMotor;
extern float KinematicPositionUnit;
extern UART_HandleTypeDef huart2;
extern uint8_t CR;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
