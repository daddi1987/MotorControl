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
- Used Variable uint32 per value position incremental encoder for limiting wrapping for the position. 
  For future inser Wrapping Position Parameter.
- Max 4/5 Values for to send at serial comunication (To be check tomorrow) BUG
- Motor Stopped send last value expost to callback Encoder and print value different to 0.0. (To be check tomorrow) BUG

## Features 

## Hardware
