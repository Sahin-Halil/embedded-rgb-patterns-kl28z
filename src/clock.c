#include <MKL28Z7.H>
#include "../inc/clock.h"

/* ------------------------------------------
       ECS642U/714P Control Clock

   Enable the DIV3 peripheral function clock using
   the SIRClock
  -------------------------------------------- */

void enablePeripheralClock() {
  SCG->SIRCDIV |= SCG_SIRCDIV_SIRCDIV3(1) ; // enable DIV3 using SIRC, divide by 1
}


