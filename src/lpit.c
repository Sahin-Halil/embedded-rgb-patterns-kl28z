/*----------------------------------------------------------------------------
       ECS642/ECS714 Lab Work

    LPIT - Low-power Interrupt Timer
    
    Functions:
      configurePIT(int channel) - configure channel 0 to 4
      startTimer(int channel)   - start PIT running
      stopTimer(int channel)    - stop it
      setTimer(int channel, uint32_t timeout) - set channel count (no need to stop)
*---------------------------------------------------------------------------*/

#include <MKL28Z7.h>
#include "../inc/lpit.h"

/* ---------------------------------------------------
    Configure a timer channel for periodic interrupts
   --------------------------------------------------- */
void configureLPIT_interrupt(int channel) {
    // Enable peripheral and function clocks in PCC
    PCC_LPIT0 &= ~PCC_CLKCFG_PCS(0x7) ;
    PCC_LPIT0 |= PCC_CLKCFG_CGC(1) | PCC_CLKCFG_PCS(0x2) ;
    // DIV3 clock must be enable first
    
    // Enable the LPIT Module clock
    LPIT0->MCR = LPIT_MCR_M_CEN_MASK  ; 

    // Set Module control register
    LPIT0->MCR |= LPIT_MCR_DBG_EN(1) ; // run in debug mode
    
    // No need to change defaults in LPIT0->CHANNEL[channel].TCTRL

    // Clear any outstanding interrupts
    // Set TIEx = 1 to enable interupts
    // Do not start timer
    switch (channel) {
        case 0:
            LPIT0->MSR = LPIT_MSR_TIF0(1) ; // write to clear
            LPIT0->MIER |= LPIT_MIER_TIE0(1) ;  
            break;
        case 1:
            LPIT0->MSR = LPIT_MSR_TIF0(1) ; // write to clear
            LPIT0->MIER |= LPIT_MIER_TIE1(1) ;  
            break;
        case 2:
            LPIT0->MSR = LPIT_MSR_TIF0(1) ; // write to clear
            LPIT0->MIER |= LPIT_MIER_TIE2(1) ;  
            break;
        case 3:
            LPIT0->MSR = LPIT_MSR_TIF0(1) ; // write to clear
            LPIT0->MIER |= LPIT_MIER_TIE3(1) ;  
            break;
    }
 
    // Enable Interrupts LPIT0_IRQn
    NVIC_SetPriority(LPIT0_IRQn, 128); // 0, 64, 128 or 192
    NVIC_ClearPendingIRQ(LPIT0_IRQn);  // clear any pending interrupts
    NVIC_EnableIRQ(LPIT0_IRQn);
}

/* -------------------------------------
    Start the timer on the given channel
   ------------------------------------- */
void startTimer(int channel) {
    LPIT0->CHANNEL[channel].TCTRL |= LPIT_TCTRL_T_EN_MASK ;    
}

/* -------------------------------------
    Stop the timer on the given channel
   ------------------------------------- */
void stopTimer(int channel) {
    LPIT0->CHANNEL[channel].TCTRL &= ~LPIT_TCTRL_T_EN_MASK ;    
}

/* -------------------------------------
   Set the timer value
      If the timer is running, the new value is used
      after the next timeout

   Unit are number of cycle of bus clock
   ------------------------------------- */
void setTimer(int channel, uint32_t timeout) {
    LPIT0->CHANNEL[channel].TVAL = timeout ;
}

