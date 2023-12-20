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
float EncoderSpeedRPM = 0.0;
float EncoderSpeedUnit = 0.0;
float DiffTickClockMotion;
float OldTickClockMotion;
uint32_t last_counter_value;
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
float TickClockMotion;
float ActualPosition = 0;
float ActualSpeedRPM = 0;
float ActualSpeed = 0;
float OldEncoderSpeedRPM = 0;


void Motion(void)      // THIS VOID RUN AT 20Khz
{

	  Counter++;
	  if(TickSerial == true)
	  {
		  CouterSerial = Counter;
	  }

	  //--------------------GET SENSOR VALUES------------------------------

	  ActualPosition = KinematicPositionUnit;
	  ActualSpeedRPM = EncoderPosition;
	  ActualSpeed = EncoderSpeed;
	  //OldEncoderSpeedRPM = EncoderSpeedRPM;


}
void DiagnosticMotor(void)
{
	  CounterDiag++;
}

void UpdateEncoderFeeBack(encoder_instance *encoder_value, TIM_HandleTypeDef *htim)
    {
	uint32_t temp_counter = __HAL_TIM_GET_COUNTER(htim);
	static uint8_t first_time = 0;
	if(!first_time)
	{
	   encoder_value ->velocity = 0;
	   first_time = 1;
	}
	else
	{
	  if(temp_counter == encoder_value ->last_Encoder_counter_value)
	  {
	    encoder_value ->velocity = 0;
	  }
	  else if(temp_counter > encoder_value ->last_Encoder_counter_value)
	  {
	    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
	    {
	      encoder_value ->velocity = -encoder_value ->last_Encoder_counter_value -
		(__HAL_TIM_GET_AUTORELOAD(htim)-temp_counter);
	    }
	    else
	    {
	      encoder_value ->velocity = temp_counter -
	           encoder_value ->last_Encoder_counter_value;
	    }
	  }
	  else
	  {
	    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
	    {
		encoder_value ->velocity = temp_counter -
	            encoder_value ->last_Encoder_counter_value;
	    }
	    else
	    {
		encoder_value ->velocity = temp_counter +
		(__HAL_TIM_GET_AUTORELOAD(htim) -
	              encoder_value ->last_Encoder_counter_value);
	    }
	   }
	}
	encoder_value ->position += encoder_value ->velocity;
	encoder_value ->last_Encoder_counter_value = temp_counter;
}
	// ---------------------------------END EXTERNAL INTERRUPT FOR ENCODER MOTOR--------------------------------------

// ----------------------------------------CALCULATE REV TO FACTOR --------------------------------------
/* Calculate Revolution to Factor
 *
 */
void Calculate_Rotation(uint16_t EncoderPulseSet,uint16_t RevoluctionFactorSet,int32_t EncoderCountSet)
{
	EncoderPosition = EncoderCountSet/4.0;   // Single Event Encoder 1*4 in Single Counter
	EncoderPositionFloat = EncoderPosition; // Single Counter Encoder
	PositionMotor = EncoderPositionFloat/EncoderPulseSet;
	KinematicPositionUnit = RevoluctionFactorSet * PositionMotor;

	TickClockMotion = Counter*0.00005; // Get current time (seconds)

	DiffTickClockMotion = (TickClockMotion - OldTickClockMotion); // Calculate time from count to count


	if (FilterSpeedEnable == 1)  //  CutOff Low-Pass Filter
	{
		//GetConstantFilter();        DA INSERIRE //////////////////////////////////////////////////////////
		EncoderSpeedRPSToFiler = ((20000.0/DiffTickClockMotion)/(EncoderPulseSet*4)); //Calculate RPS speed From microsecond to second
		EncoderSpeed = ((b_i*RPSSpeedFilter) + (a_i*EncoderSpeedRPSToFiler) + (a_i*RPSSpeedFilterPrev));
		EncoderSpeedRPM = (EncoderSpeed* 60.0); //Calculate RPM Speed
		EncoderSpeedUnit = (EncoderSpeedRPM * RevoluctionFactorSet);
		OldTickClockMotion = TickClockMotion; // Save to old value
		HAL_GPIO_TogglePin (GPIOA, LD2_Green_Led_Pin);
		RPSSpeedFilterPrev = EncoderSpeedRPSToFiler;
		RPSSpeedFilter = EncoderSpeed;
		//HAL_Delay(1);
	}
	else
	{
		EncoderSpeed= (1/(DiffTickClockMotion*EncoderPulseSet)); //Calculate RPS speed From microsecond to second
		EncoderSpeedRPM = (EncoderSpeed* 60.0); //Calculate RPM Speed
		EncoderSpeedUnit = (EncoderSpeedRPM * RevoluctionFactorSet);
		OldTickClockMotion = TickClockMotion; // Save to old value
		//IncrementSpeedCheckOld = IncrementSpeedCheck;
		//IncrementSpeedCheck++;
		//TM6_Currentvalue = 0; //Reset Current Value Counter
		HAL_GPIO_TogglePin (GPIOA, LD2_Green_Led_Pin);
	}

}
// -------------------------------------END CALCULATE REV TO FACTOR --------------------------------------

