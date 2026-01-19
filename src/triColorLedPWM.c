/* ------------------------------------------
       ECS642/ECS714 Lab Work

   Configuration of PWM control of red, green, blue LEDs on 
   FRDM KL25Z development board
   ---------------------------------------- */
#include <MKL28Z7.h>
#include "../inc/triColorLedPwm.h"

// Create a bit mask (32 bits) with only bit x set
#define MASK(x) (1UL << (x))

/*----------------------------------------------------------------------------
  Configuration
  The pin control registers (PCR) are set to enable PWM control of LEDs. 
*----------------------------------------------------------------------------*/

void configureLEDforPWM(void) {
    // Enable clock to ports C and E
    PCC_PORTC |= PCC_CLKCFG_CGC(1) ; // green LED
    PCC_PORTE |= PCC_CLKCFG_CGC(1) ; // red, blue
    
    // Set the Pin Control Register 
    // Set Pin on each port as TPM o/p
    RED_PORT->PCR[RED_PIN] &= ~PORT_PCR_MUX_MASK;          
    RED_PORT->PCR[RED_PIN] |= PORT_PCR_MUX(RED_ALT);

    GREEN_PORT->PCR[GREEN_PIN] &= ~PORT_PCR_MUX_MASK;          
    GREEN_PORT->PCR[GREEN_PIN] |= PORT_PCR_MUX(GREEN_ALT);

    BLUE_PORT->PCR[BLUE_PIN] &= ~PORT_PCR_MUX_MASK;          
    BLUE_PORT->PCR[BLUE_PIN] |= PORT_PCR_MUX(BLUE_ALT);  
}
