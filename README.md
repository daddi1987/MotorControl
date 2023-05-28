# MotorControl
Motor Control Firmware for Stepper motor in ClosedLoop, implemented on Microcontrol STM32 F4 Cortex.

![](https://img.shields.io/github/stars/pandao/editor.md.svg) ![](https://img.shields.io/github/forks/pandao/editor.md.svg) ![](https://img.shields.io/github/tag/pandao/editor.md.svg) ![](https://img.shields.io/github/release/pandao/editor.md.svg) ![](https://img.shields.io/github/issues/pandao/editor.md.svg) ![](https://img.shields.io/bower/v/editor.md.svg)

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

Below de Chart to RPM calculate


## Features 
- [ ] Filter Speed in Hz for eliminate noise implemented, missing convert from Hz to coefficent.

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

