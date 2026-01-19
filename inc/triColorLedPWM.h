/* ------------------------------------------
       ECS642/ECS714 Lab Work

   Header for configuration and control of red, green, blue LEDs
   on the FRDM KL28Z development board
   
   Red     PTE29   ALT3 TPMO_CH2
   Green   PTC4    ALT4 TPM0_CH3
   Blue    PTE31   ALT3 TPM0_CH4
     ---------------------------------------- */

#ifndef TRICOLORLEDPWM_H
#define TRICOLORLEDPWM_H

// red
#define TPM0_REDCHAN (2)
#define RED_PORT (PORTE)
#define RED_PIN (29)
#define RED_ALT (3)

// green: pin 19, port B. FTM2_CH1
#define TPM0_GREENCHAN (3)
#define GREEN_PORT (PORTC)
#define GREEN_PIN (4)
#define GREEN_ALT (4)

// blue: pin 1, port D. FTM0_CH1
#define TPM0_BLUECHAN (4)
#define BLUE_PORT (PORTE)
#define BLUE_PIN (31)
#define BLUE_ALT (3)

// functions
void configureLEDforPWM() ; // configure PWM alternatives for LEDs

#endif
