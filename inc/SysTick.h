/* ------------------------------------------
   Header file for ECS642U/714P SysTick Timer

   Functions
      Init_SysTick(t) initialse timer for t ticks (i.e. interrupts) per second
      waitSysTickCounter(int t)
          initialse counter to t ticks
          wait until it reaches zero
          call at start to initialise counter
  -------------------------------------------- */

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

/* ECS642 Lab 1 header - provided version */

/* Function prototypes for cycle timing using SysTick */

void Init_SysTick(uint32_t ticksPerSec) ;
void waitSysTickCounter(int ticks) ;


#endif