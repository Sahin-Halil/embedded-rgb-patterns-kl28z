/* ------------------------------------------
       ECS642/ECS714 Lab Work

   Configuration of TPM modules
   ---------------------------------------- */

#include <MKL28Z7.H>
#include "../inc/TPMPWM.h"
#include "../inc/triColorLedPWM.h"


/*----------------------------------------------------------------------------
  Configure TPM0 for PWM on TMP0

Three channels used to control LEDs:
  Red   channel 2
  Green channel 3
  Blue  channel 4
  
  
  Controlled by the following macro definitions:
       TPM0_REDCHAN, TPM0_GREENCHAN, TPM0_BLUECHAN the channels, determined by the pins
    These values mst be consistent with the KL25Z pinout
    
    The following values are fixed (i.e. no macros)
       Clock frequency - DIV3 peripheral function clock 8 MHz
       Pre-scaling - divide by 16
       Count modulo - 1023
         
    Note that there are many other useful configurations of the TPM
    not covered here.
 *----------------------------------------------------------------------------*/ 
void configureTPMClock(){
    // Enable peripheral and function clocks in PCC
    PCC_TPM0 &= ~PCC_CLKCFG_PCS(0x7) ;
    PCC_TPM0 |= PCC_CLKCFG_CGC(1) | PCC_CLKCFG_PCS(0x2) ;
    // DIV3 clock must be enable first
}

void configureTPM0forPWM() {
    // -----------------------
    // Module configuration
    // ------------------------
    
    // Disable the TPM0
    TPM0->SC = TPM_SC_CMOD(0) ;
    while ((TPM0->SC & TPM_SC_CMOD(3))) ;
    
    // Prescale by 16 with 100
    TPM0->SC |= TPM_SC_PS(0x4) ;

    // Count modulo 2^10 - 1 = 1023. Note: 1024 is used for 100% duty cycle
    TPM0->MOD = TPM_MOD_MOD(1023) ;

    // Set counter to continue when debugging
    TPM0->CONF |= TPM_CONF_DBGMODE(0x3) ;

    // -----------------------
    // Channel configuration
    // ------------------------

    // Configure channel TPM_CHAN
    //   Edge pulse: MSB:A = 1:0
    //   True pulse high: ELSB:A = 1:0
    TPM0->CONTROLS[TPM0_REDCHAN].CnSC = 0x28 ; 
    TPM0->CONTROLS[TPM0_GREENCHAN].CnSC = 0x28 ; 
    TPM0->CONTROLS[TPM0_BLUECHAN].CnSC = 0x28 ; 
    
    // Set the channel variable - off
    TPM0->CONTROLS[TPM0_REDCHAN].CnV = TPM_CnV_VAL(0) ;
    TPM0->CONTROLS[TPM0_GREENCHAN].CnV = TPM_CnV_VAL(0) ;
    TPM0->CONTROLS[TPM0_BLUECHAN].CnV = TPM_CnV_VAL(0) ;
       
    // -----------------------
    // Enable using internal clock 
    // -----------------------

    TPM0->SC |= TPM_SC_CMOD(0x01) ;
    while (!(TPM0->SC & TPM_SC_CMOD(3))) ;
}


/*----------------------------------------------------------------------------
  Set PWM duty cycle to change the LED brightness. There are two complications
    1. The LEDs are wired active low so we need a low duty cycle for a bright LED
    2. The response is not at all linear: we use a look-up table for duty cycle values
    
 *----------------------------------------------------------------------------*/ 


const uint16_t duty[] = {
    1024, 1021, 1016, 1010, 1002, 991, 977, 958,
    935, 904, 866, 819, 764, 699, 628, 551,
    473, 397, 325, 261, 205, 159, 121, 90,
    66, 48, 33, 23, 15, 8, 4, 0} ;

const unsigned int MAXBRIGHTNESS = 31 ; // range is 0 to 31
                         
void setLEDBrightness(enum LED led, unsigned int brightness) {
    if (brightness > MAXBRIGHTNESS) brightness = MAXBRIGHTNESS ;
    uint16_t d = duty[brightness] ; 
    
    // Set the channel variable
    switch (led) {
        case Red:
            TPM0->CONTROLS[TPM0_REDCHAN].CnV = TPM_CnV_VAL(d) ;
            break ;
        case Green:
            TPM0->CONTROLS[TPM0_GREENCHAN].CnV = TPM_CnV_VAL(d) ;
            break ;
        case Blue:
            TPM0->CONTROLS[TPM0_BLUECHAN].CnV = TPM_CnV_VAL(d) ;
            break ;
    }
}
