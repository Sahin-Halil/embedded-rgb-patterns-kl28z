# RGB LED Patterns Using LPIT and PWM (FRDM-KL28Z)

This project implements two RGB LED colour animations on the NXP FRDM-KL28Z development board using interrupt driven timing and PWM brightness control.

The lab framework provided low level peripheral configuration including SysTick, TPM, and LPIT setup. My contribution focused on application level logic, specifically state machine design, timing calculations, and LPIT interrupt behaviour used to drive the colour transitions.

## Overview

Two RGB colour patterns are implemented as finite state machines.

• Cube Edge Traversal  
  This pattern transitions along the edges of the RGB colour cube and consists of six states with a total of 192 colour transitions.

• Cube Face Traversal  
  This pattern transitions across the faces of the RGB colour cube and consists of four states with a total of 128 colour transitions.

Each LPIT interrupt advances the currently active state machine by one transition and updates the PWM duty cycles for the red, green, and blue LEDs.

## Timing Model

The LPIT is clocked at 8 MHz. Rather than using a fixed interrupt period, the LPIT load value is calculated so that each full colour pattern completes in a fixed total time.

• FAST mode completes a full colour cycle in approximately 2 seconds  
• MEDIUM mode completes a full colour cycle in approximately 5 seconds  
• SLOW mode completes a full colour cycle in approximately 9 seconds  

Patterns with fewer internal transitions use a longer interrupt period. This ensures that the perceived animation speed remains consistent when switching between patterns.

## User Controls

• Button B1 cycles through the FAST, MEDIUM, and SLOW animation rates by updating the LPIT load value.

• Button B2 switches between the two colour patterns. When a pattern change occurs, the active state machine is reinitialised and the LPIT timing is recalculated while preserving the currently selected speed.

## System Architecture

• A 10 ms cyclic executive in the main loop handles button polling and control logic.

• The LPIT interrupt service routine advances the active colour state machine and updates LED brightness values.

• TPM PWM hardware continuously controls LED brightness using the most recent duty cycle values.

This separation ensures deterministic timing and responsive user interaction.

## Code Structure

• main.c  
  Contains the application logic written for this lab, including colour pattern state machines, timing calculations, LPIT interrupt behaviour, and pattern and rate control.

• Support modules such as SysTick, TPM, LPIT, and clock configuration  
  Provided as part of the lab framework and used to supply timing, PWM output, and peripheral configuration.

## Hardware

• NXP FRDM-KL28Z development board  
• On board RGB LED  
• Shield buttons B1 and B2  

## Notes

This project was completed as part of an embedded systems laboratory exercise. The focus of the work was on designing correct timing behaviour and state based control logic on top of provided peripheral infrastructure.
