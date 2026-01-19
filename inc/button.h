/*----------------------------------------------------------------------------
    Header File for Shield Buttons
    
    Functions:
        configureButtons - configure buttons, selected using a bit mask
        isPressed - get button position
 *---------------------------------------------------------------------------*/
 
// Button Header/PIN  GPIO
//   B1		J1/10     PTB10
//   B2		J1/12     PTB11
//   B3		J4/2      PTB0
//   B4		J4/4      PTB1
//   B5		J2/9      PTE6   <----- port E

#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>
#include "mask.h"

// States of a button
#define BOPEN (0)      // also called UP or NOTPRESSED
#define BCLOSED (1)    // also called DOWN or PRESSED
#define BBOUNCE (2)    // state for handling bouncing

// Delay value for 
#define BOUNCEDELAY (2) // number of cycles to wait in bounce state


// ===============
// Buttons 1 - 5
// ===============
#define B1pin (10)   // on port B      0000 0400
#define B2pin (11)   // on port B      0000 0800
#define B3pin (0)    // on port B      0000 0001
#define B4pin (1)    // on port B      0000 0002
#define B5pin (6)    // on port EEEEE  0000 0040

//#define B1MASK MASK(B1pin)
//#define B2MASK MASK(B2pin)
//#define B3MASK MASK(B3pin)
//#define B4MASK MASK(B4pin)
//#define B5MASK MASK(B5pin)

// Create an enum using bit masks for the corresponding pin
// this depends on the pin numbers differing
enum BUTTON_t {  
    B1  = 0x00000400,
    B2  = 0x00000800,
    B3  = 0x00000001,
    B4  = 0x00000002,
    B5  = 0x00000040
} ;

// API
bool isPressed(enum BUTTON_t b) ;                            // test if  button is pressed
void configureButtons(uint32_t bmask, bool interrupt) ; // configure buttons for polling

#endif
