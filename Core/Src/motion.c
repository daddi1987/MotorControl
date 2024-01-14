/*
 * motion.c
 *
 *  Created on: 12 dic 2023
 *      Author: davide
 */

#include "app_threadx.h"
#include "main.h"
#include <stdbool.h>
#include <math.h>

//--------------Filter Cionstant Time baase samples--------------------
#define RC 1.0f
#define SAMPLE_RATE 1000.0f

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
uint8_t FrequencySpeedFilter = 80;
uint8_t FrequencyCase = 1;
float b_i;
float a_i;
float TickClockMotion;
float ActualPosition = 0;
float ActualSpeedRPM = 0;
float ActualSpeed = 0;
float OldEncoderSpeedRPM = 0;
float dt = 0.0;
uint8_t cutoffFrequency = 100;  //Max Filter is 255 Hz
float alpha = 0.0;
float filteredValue = 0.0;

/////////--------------------LOW PASS FILTER------------------////////////////////////
float lowPassFilter(float input, float dt, float alpha) {
    static float filteredValue = 0.0f;

    // Calcolare la costante di tempo del filtro in base alla frequenza di taglio
    float tau = 1.0f / (2.0f * M_PI * alpha);

    // Calcolare il peso del nuovo valore rispetto al valore precedente
    float alphaCalc = dt / (tau + dt);

    // Applicare il filtro
    filteredValue = alphaCalc * input + (1.0f - alphaCalc) * filteredValue;

    return filteredValue;
}



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
		Calculate_Rotation(EncoderPulse, EncoderPosition,EncoderRevCount,FilterSpeedEnable,cutoffFrequency);

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

void Calculate_Rotation(uint16_t EncoderPulseSet,int32_t EncoderCountSet,int16_t EncoderSpeedSet,uint8_t FilterSpeedEnableSet,uint8_t cutoffFrequencySet)
{
	EncoderPosition = EncoderCountSet;   // Single Event Encoder 1*4 in Single Counter
	EncoderPositionFloat = EncoderPosition; // Single Counter Encoder
	PositionMotor = EncoderPositionFloat/(EncoderPulseSet*4);
	KinematicPositionUnit = PositionMotor*RevoluctionFactor;  //Unit Position Factor

	if (FilterSpeedEnableSet == 1)  //  CutOff Low-Pass Filter
	{
		//GetConstantFilter();        DA INSERIRE //////////////////////////////////////////////////////////
		//EncoderSpeedRPSToFiler = ((20000.0/DiffTickClockMotion)/(EncoderPulseSet*4)); //Calculate RPS speed From microsecond to second
		//EncoderSpeedRPS = ((b_i*RPSSpeedFilter) + (a_i*EncoderSpeedRPSToFiler) + (a_i*RPSSpeedFilterPrev));
		//EncoderSpeedRPM = (EncoderSpeedRPS * 60.0); //Calculate RPM Speed
		//EncoderSpeedUnit = (EncoderSpeedRPM * RevoluctionFactor);
		//OldTickClockMotion = TickClockMotion; // Save to old value
		//HAL_GPIO_TogglePin (GPIOA, LD2_Green_Led_Pin);
		//RPSSpeedFilterPrev = EncoderSpeedRPSToFiler;
		//RPSSpeedFilter = EncoderSpeedRPS;
		//HAL_Delay(1);

		EncoderSpeedRPSFloat = EncoderSpeedSet;
		EncoderSpeedRPS = ((EncoderSpeedRPSFloat)/(EncoderPulseSet*4)*1000); //Calculate RPS speed From microsecond to second
		EncoderSpeedRPM = (EncoderSpeedRPS*60); //Calculate RPM Speed
		EncoderSpeedUnit = (1 / RevoluctionFactor);
		EncoderSpeedUnit = EncoderSpeedRPM * EncoderSpeedUnit;  // Calculate Speed Unit Value
		//---------------------Filter Cuttoff-------------------------------------------------

	    dt = 1.0f / SAMPLE_RATE; // Sample Rate
	    alpha = dt / (1.0f / (2.0f * M_PI * cutoffFrequencySet) + dt);
	    filteredValue = lowPassFilter(EncoderSpeedUnit, dt, alpha);    // Apply Filter
	    EncoderSpeedUnit = filteredValue;
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

