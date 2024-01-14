/*
 * motion.c
 *
 *  Created on: 12 dic 2023
 *      Author: davide
 */

#include "app_threadx.h"
#include "main.h"
#include <stdbool.h>
//--------------Filter Cionstant Time baase samples--------------------
#define NUM_STAGES 1
#define BLOCK_SIZE 1


encoder_instance enc_instance;

int32_t EncoderCount = 0;
uint32_t OldEncoderPosition = 0;
uint8_t rot_new_state = 0;
uint8_t rot_old_state = 0;
uint16_t EncoderPulse = 2048;
float RevoluctionFactor = 1.0f; //2.758620689f
float RevoluctionFactorSet = 0.0f;
float KinematicPositionUnit = 0.0f;
float EncoderSpeedRPS = 0.0f;
int16_t EncoderRevCount = 0;
float EncoderSpeedRPM = 0.0;
float EncoderSpeedRPSold = 0.0;
float EncoderSpeedUnit = 0.0;
float DiffTickClockMotion;
float OldTickClockMotion;
int32_t EncoderPosition = 0;
float EncoderPositionFloat = 0.0;
float PositionMotor;
float EncoderSpeedRPSFloat = 0.0f;
//---------------Decleare variables for filter Speed----------------
uint8_t FilterSpeedEnable = 0;
float RPSSpeedFilter = 0;
float RPSSpeedFilterPrev = 0;
float EncoderSpeedRPSToFiler = 0.0;
uint8_t FrequencySpeedFilter = 20;
uint8_t FrequencyCase = 1;
float b_i;
float a_i;
float TickClockMotion;
float ActualPosition = 0;
float ActualSpeedRPM = 0;
float ActualSpeed = 0;
float OldEncoderSpeedRPM = 0;
float KinematicSpeedRPSToFiler = 0.0;

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
	  //OldEncoderSpeedRPM = EncoderSpeedRPM;


}
void DiagnosticMotor(void)
{
	  CounterDiag++;
}

void EncoderFeeBack(TIM_HandleTypeDef *htim)
    {
	    //--------------------UPDATE STATE ENCODER---------------------------
		TM2_Currentvalue = __HAL_TIM_GET_COUNTER(htim); // Get current time (microseconds)
		Update_Encoder(&enc_instance, htim);
		EncoderPosition = enc_instance.position;
		EncoderRevCount = enc_instance.speed;
		Calculate_Rotation(EncoderPulse, EncoderPosition,EncoderRevCount,FilterSpeedEnable,FrequencySpeedFilter);

		//Calculate_Rotation(EncoderPulse,RevoluctionFactor,EncoderCount);
	}
	// ---------------------------------END EXTERNAL INTERRUPT FOR ENCODER MOTOR--------------------------------------

// ----------------------------------------CALCULATE REV TO FACTOR --------------------------------------
/* Calculate Revolution to Factor
 *
 */
void Update_Encoder(encoder_instance *encoder_value, TIM_HandleTypeDef *htim)
 {
uint32_t temp_counter = __HAL_TIM_GET_COUNTER(htim);
static uint8_t first_time = 0;
if(!first_time)
{
   encoder_value ->speed = 0;
   first_time = 1;
}
else
{
  if(temp_counter == encoder_value ->LastCounterValue)
  {
    encoder_value ->speed = 0;
  }
  else if(temp_counter > encoder_value ->LastCounterValue)
  {
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
    {
      encoder_value ->speed = -encoder_value ->LastCounterValue -
	(__HAL_TIM_GET_AUTORELOAD(htim)-temp_counter);
    }
    else
    {
      encoder_value ->speed = temp_counter -
           encoder_value ->LastCounterValue;
    }
  }
  else
  {
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
    {
	encoder_value ->speed = temp_counter -
            encoder_value ->LastCounterValue;
    }
    else
    {
	encoder_value ->speed = temp_counter +
	(__HAL_TIM_GET_AUTORELOAD(htim) -
              encoder_value ->LastCounterValue);
    }
   }
}
encoder_value ->position += encoder_value ->speed;
encoder_value ->LastCounterValue = temp_counter;
 }

void Calculate_Rotation(uint16_t EncoderPulseSet,int32_t EncoderCountSet,int16_t EncoderSpeedSet,uint8_t FilterSpeedEnableSet,uint8_t FrequencySpeedFilterSet)
{
	EncoderPosition = EncoderCountSet;   // Single Event Encoder 1*4 in Single Counter
	EncoderPositionFloat = EncoderPosition; // Single Counter Encoder
	PositionMotor = EncoderPositionFloat/(EncoderPulseSet*4);
	KinematicPositionUnit = PositionMotor*RevoluctionFactor;  //Unit Position Factor

	if (FilterSpeedEnableSet == 1)  //  CutOff Low-Pass Filter
	{
		GetConstantFilter();
		EncoderSpeedRPSFloat = EncoderSpeedSet;
		KinematicSpeedRPSToFiler = ((EncoderSpeedRPSFloat)/(EncoderPulseSet*4)*1000); //Calculate RPS speed From microsecond to second
		EncoderSpeedRPS = ((b_i*RPSSpeedFilter) + (a_i*KinematicSpeedRPSToFiler) + (a_i*RPSSpeedFilterPrev));
		EncoderSpeedRPM = (EncoderSpeedRPS*60); //Calculate RPM Speed
		EncoderSpeedUnit = (1 / RevoluctionFactor);
		EncoderSpeedUnit = EncoderSpeedRPM * EncoderSpeedUnit;  // Calculate Speed Unit Value
		RPSSpeedFilterPrev = KinematicSpeedRPSToFiler;
		RPSSpeedFilter = EncoderSpeedRPS;
	}
	else
	{
		EncoderSpeedRPSFloat = EncoderSpeedSet;
		EncoderSpeedRPS = ((EncoderSpeedRPSFloat)/(EncoderPulseSet*4)*1000); //Calculate RPS speed From microsecond to second
		EncoderSpeedRPM = (EncoderSpeedRPS*60); //Calculate RPM Speed
		EncoderSpeedUnit = (1.0 / RevoluctionFactor);
		EncoderSpeedUnit = EncoderSpeedRPM * EncoderSpeedUnit;  // Calculate Speed Unit Value
	}
}
// -------------------------------------END CALCULATE REV TO FACTOR --------------------------------------

//----------------------------START FILTER--------------------------
void GetConstantFilter()
{
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
//----------------------------END SELECT FILTER--------------------------
