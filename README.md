# MotorControl
Motor Control Firmware for Stepper motor in ClosedLoop, implemented on Microcontrol STM32 F4 Cortex.

![](https://img.shields.io/github/stars/daddi7987/editor.md.svg) ![](https://img.shields.io/github/v/tag/daddi7987/MotorControl) ![](https://img.shields.io/github/release/MotorControl/editor.md.svg) ![](https://img.shields.io/github/issues/MotorControl/editor.md.svg)

**Table of Contents**

[TOC]


## Requirments
 
 
>1. The Motor must move from point A at point B.
>2. The motor must follow the position and speed detected on the encoder installed on the motor
>3. Expose the variables available on the serial port at 1000 Hz.

## Specification

### Encoder 
Use the incremental encoder to determine the motor position.
From the encoder we calculate the speed using the interrupts without exploiting the      Encoder port of the device to understand its operation.
Encoder Transition:
Four Interrupts transitions are generated for one Encoder pulse.
![Alt text](https://github.com/daddi1987/MotorControl/blob/Devel/Image/EncoderState.png?raw=true "**RobotArm With Gripper**")

### Filter speed
It is possible to filter the speed in Hz, this functionality is implemented using a table and a switch case that selects the previous calculation of the correct coefficient. Non-realtime computation filter maybe.

| Hz  | bi | ai |
| :------------ |:---------------:| -----:|
| 5 | 0,96906992| 0,01546504 |
| 10 | 0,93908194 | 0,03045903 |
| 15 | 0,90999367 | 0,04500317 |
| 25 | 0,85435899 | 0,07282051 |
| 35 | 0,80187364 | 0,09906318 |
| 45 | 0,75227759 | 0,1238612 |
| 55 | 0,70533864 | 0,14733068 |
| 65 | 0,66084882 | 0,16957559 |
| 75 | 0,61862133 | 0,19068933 |
| 85 | 0,57848789 | 0,21075605|
| 95 | 0,5402965  | 0,22985175 |
| 105|0,50390953|   0,24804523 |


#### Speed Raw
![Alt text](https://github.com/daddi1987/MotorControl/blob/main/Image/Chart%20Speed%20RPM%20Raw.jpeg?raw=true "**KinematicSpeedRAW**")

#### Speed Filter 85 Hz
![Alt text](https://github.com/daddi1987/MotorControl/blob/main/Image/Chart%20Speed%20RPM%2085Hz.jpeg?raw=true "**KinematicSpeed85Hz**")

#### Speed Filter 35 Hz
![Alt text](https://github.com/daddi1987/MotorControl/blob/main/Image/Chart%20Speed%20RPM%2035Hz.jpeg?raw=true "**KinematicSpeed35Hz**")

#### Speed Filter 5 Hz
![Alt text](https://github.com/daddi1987/MotorControl/blob/main/Image/Chart%20Speed%20RPM%205Hz.jpeg?raw=true "**KinematicSpeed5Hz**")

### Driver Motor
Use the L289n for driver motor, this driver have a 2 Ampere with max current.
For the pilot motor driver use interface trigger signal modulation. Implementig Enable phase and frequency for motor speed. 

### Pilot Motor Sequence
|Step| A+ | B+| A- |B- |
| :------------ |:---------------:| -----:|:---------------:| -----:|
|1 | 1 | 0 |0 | 0 |
| 2 |1 | 1 |0 | 0 |
| 3 | 0 | 1 |0 | 0 |
| 4 | 0 | 1 |1 | 0 |
| 5 | 0 | 0 |1 | 0 |
| 6 | 0 | 0 |1 | 1 |
| 7 | 0 | 0 |0 | 1 |
| 8 | 1 | 0 |0 | 1 |


![Alt text](https://content.instructables.com/F7D/J0WE/I36LH0ER/F7DJ0WEI36LH0ER.png?auto=webp?raw=true "**KinematicSpeed5Hz**")

## Features 
- [x] Filter Speed in Hz for eliminate noise implemented, missing convert from Hz to coefficent.

## Bug
- [x] Max 4/5 Values for to send at serial comunication (To be check tomorrow)
- [x] Motor Stopped send last value expost to callback Encoder and print value different to 0.0.
- [x] Used Variable uint32 per value position incremental encoder for limiting wrapping for the position. 
- [ ] Remove value 0 in the list get value.
- [ ] For future inser Wrapping Position Parameter.
- [ ] Calculate Low pass Filter in RealTime.

## Hardware
The Test Bench includes the Gearbox and two motors with Nucleus 64 and Ln298n for pilot motor.

![Alt text](https://github.com/daddi1987/MotorControl/blob/Devel/Image/Test%20Banch.JPG?raw=true "**RobotArm With Gripper**")
