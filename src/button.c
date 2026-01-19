
/*----------------------------------------------------------------------------
    Shield Buttons
    
    Functions:
        configureButtons - configure buttons, selected using a bit mask
        isPressed - get button position
 *---------------------------------------------------------------------------*/
 
#include <MKL28Z7.h>
#include <stdbool.h>
#include <stdint.h>
#include "..\inc\button.h"
#include "..\inc\mask.h"

// Button Header/PIN  GPIO
//   B1		J1/10     PTB10
//   B2		J1/12     PTB11
//   B3		J4/2      PTB0
//   B4		J4/4      PTB1
//   B5		J2/9      PTE6   <----- port E

/*----------------------------------------------------------------------------
  Button Input Configuration

  Initialse selected buttons on ports B or E pins as an input, with
    - a pullup resistor 
    - no interrupt (B1 to B4)
  The mask has a bit set for each button in use.
  The interrupt parameter is used if B5 is configured: a falling edge can be set.
*----------------------------------------------------------------------------*/
void configureButtons(uint32_t mask, bool interrupt) {
   
    // Check which ports are in use and enable clocks
    if (mask & (B1 | B2 | B3 | B4)) {
        PCC_PORTB |= PCC_CLKCFG_CGC(1) ;
    }
    if (mask & B5) {
        PCC_PORTE |= PCC_CLKCFG_CGC(1) ;
    }
    
    // Evaluate these bit patterns once only
    const uint32_t gpioPullUp = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK ;
    const uint32_t clearMUX_IRQ = ~(PORT_PCR_MUX_MASK | PORT_PCR_IRQC_MASK)  ;     
    
    // Configure each of B1 to B4 as required
    if (mask & B1) { // on port B
        /* Select GPIO and enable pull-up resistors and no interrupt */
        PORTB->PCR[B1pin] &= clearMUX_IRQ ;          
        PORTB->PCR[B1pin] |= gpioPullUp ;
        // set as input
        PTB->PDDR &= ~B1 ;
    }        
    if (mask & B2) { // on port B
        /* Select GPIO and enable pull-up resistors and no interrupt */
        PORTB->PCR[B2pin] &= clearMUX_IRQ ;         
        PORTB->PCR[B2pin] |= gpioPullUp ;
        // set as input
        PTB->PDDR &= ~B2 ;
    }        
    if (mask & B3) {  // on port B
        /* Select GPIO and enable pull-up resistors and no interrupt */
        PORTB->PCR[B3pin] &= clearMUX_IRQ ;          
        PORTB->PCR[B3pin] |= gpioPullUp ;
        // set as input
        PTB->PDDR &= ~B3 ;
    }        
    if (mask & B4) {  // on port B
        /* Select GPIO and enable pull-up resistors and no interrupt */
        PORTB->PCR[B4pin] &= clearMUX_IRQ ;         
        PORTB->PCR[B4pin] |= gpioPullUp ;
        // set as input
        PTB->PDDR &= ~B4 ;
    }        

    // Button B5 may be used with an interrupt
    if (mask & B5) {  // on port E
        /* Select GPIO and enable pull-up resistors and no interrupt */
        PORTE->PCR[B5pin] &= clearMUX_IRQ ;           
        PORTE->PCR[B5pin] |=  gpioPullUp ;
        // set as input
        PTE->PDDR &= ~B5 ;
        // check if interrupts required
        if (interrupt) {
            PORTE->PCR[B5pin] |= PORT_PCR_IRQC(0x0A) ; // falling edge
 
            /* Enable Interrupts for port A*/
            NVIC_SetPriority(PORTE_IRQn, 128); // 0, 64, 128 or 192
            NVIC_ClearPendingIRQ(PORTE_IRQn);  // clear any pending interrupts
            NVIC_EnableIRQ(PORTE_IRQn);  
        }        
    }        
}

/*----------------------------------------------------------------------------
  isPressed: test the button position

  Operating the switch connects the input to ground. A non-zero value
  shows the switch is not pressed.
 *----------------------------------------------------------------------------*/

bool isPressed(enum BUTTON_t b) { // only one bit set
    // check for the port
    if (b & (B1 | B2 | B3 | B4)) {
        if (PTB->PDIR & b) {
            return false ;
        } 
        return true ;
    } else { // must be B5
        if (PTE->PDIR & b) {
            return false ;
        }
        return true;
    }
}
