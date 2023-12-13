/*
 * motion.c
 *
 *  Created on: 12 dic 2023
 *      Author: davide
 */

#include "app_threadx.h"
#include "main.h"
#include <stdbool.h>

int32_t EncoderCount = 0;
uint8_t rot_new_state = 0;
uint8_t rot_old_state = 0;
uint16_t EncoderPulse = 2048;
uint16_t RevoluctionFactor = 2;
float KinematicPositionUnit = 0.0;
float EncoderSpeedRPS = 0.0;
float EncoderSpeedRPM = 0.0;
float EncoderSpeedRPSold = 0.0;
float EncoderSpeedUnit = 0.0;
uint32_t TM1_DiffCaunter;
uint32_t TM1_OldValue;
int32_t EncoderPosition = 0;
float EncoderPositionFloat = 0.0;
float PositionMotor;
//---------------Decleare variables for filter Speed----------------
uint8_t FilterSpeedEnable = 0;
float RPSSpeedFilter = 0;
float RPSSpeedFilterPrev = 0;
float EncoderSpeedRPSToFiler = 0.0;
uint8_t FrequencySpeedFilter = 80;
uint8_t FrequencyCase = 1;
float b_i;
float a_i;
int16_t TM1_Currentvalue;
float ActualPosition = 0;
float ActualSpeedRPM = 0;
float ActualSpeed = 0;


void Motion(void)      // THIS VOID RUN AT 20Khz
{
	  Counter++;
	  if(TickSerial == true)
	  {
		  CouterSerial = Counter;
	  }

	  //--------------------GET SENSOR VALUES------------------------------

	  ActualPosition = KinematicPositionUnit;
	  ActualSpeedRPM = EncoderSpeedRPM;
	  ActualSpeed = EncoderSpeedUnit;


}
void DiagnosticMotor(void)
{
	  CounterDiag++;
}

void EncoderFeadBack (rot_old_state, rot_new_state)
{
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
		Calculate_Rotation(EncoderPulse,RevoluctionFactor,EncoderCount);
	}
	// ---------------------------------END EXTERNAL INTERRUPT FOR ENCODER MOTOR--------------------------------------

// ----------------------------------------CALCULATE REV TO FACTOR --------------------------------------
/* Calculate Revolution to Factor
 *
 */
void Calculate_Rotation(uint16_t EncoderPulseSet,uint16_t RevoluctionFactorSet,uint16_t EncoderCountSet)
{
	EncoderPosition = EncoderCountSet/4.0;   // Single Event Encoder 1*4 in Single Counter
	EncoderPositionFloat = EncoderPosition; // Single Counter Encoder
	PositionMotor = EncoderPositionFloat/EncoderPulseSet;
	KinematicPositionUnit = RevoluctionFactorSet * PositionMotor;

	TM1_Currentvalue = __HAL_TIM_GET_COUNTER(&htim1); // Get current time (microseconds)

	if(TM1_Currentvalue >= TM1_OldValue)
		{
		TM1_DiffCaunter = (TM1_Currentvalue - TM1_OldValue); // Calculate time from count to count
	if (FilterSpeedEnable == 1)  //  CutOff Low-Pass Filter
	{
		//GetConstantFilter();        DA INSERIRE //////////////////////////////////////////////////////////
		EncoderSpeedRPSToFiler = ((1000000.0/TM1_DiffCaunter)/(EncoderPulseSet*4)); //Calculate RPS speed From microsecond to second
		EncoderSpeedRPS = ((b_i*RPSSpeedFilter) + (a_i*EncoderSpeedRPSToFiler) + (a_i*RPSSpeedFilterPrev));
		EncoderSpeedRPM = (EncoderSpeedRPS * 60.0); //Calculate RPM Speed
		EncoderSpeedUnit = (EncoderSpeedRPM * RevoluctionFactorSet);
		TM1_OldValue = TM1_Currentvalue; // Save to old value
		HAL_GPIO_TogglePin (GPIOA, LD2_Green_Led_Pin);
		RPSSpeedFilterPrev = EncoderSpeedRPSToFiler;
		RPSSpeedFilter = EncoderSpeedRPS;
		//HAL_Delay(1);
	}
	else
	{
		EncoderSpeedRPS = ((1000000.0/TM1_DiffCaunter)/(EncoderPulseSet*4)); //Calculate RPS speed From microsecond to second
		EncoderSpeedRPM = (EncoderSpeedRPS * 60.0); //Calculate RPM Speed
		EncoderSpeedUnit = (EncoderSpeedRPM * RevoluctionFactorSet);
		TM1_OldValue = TM1_Currentvalue; // Save to old value
		//IncrementSpeedCheckOld = IncrementSpeedCheck;
		//IncrementSpeedCheck++;
		//TM6_Currentvalue = 0; //Reset Current Value Counter
		HAL_GPIO_TogglePin (GPIOA, LD2_Green_Led_Pin);
	}
}
else
{
	TM1_OldValue = TM1_Currentvalue;
}

}
// -------------------------------------END CALCULATE REV TO FACTOR --------------------------------------

