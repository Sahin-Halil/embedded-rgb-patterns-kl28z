/*----------------------------------------------------------------------------
       ECS642/ECS714 Lab Work

    Header File for LPIT - Low-power Interrupt Timer
    
    Functions:
      configurePIT(int channel) - configure channel 0 to 4
      startTimer(int channel)   - start PIT running
      stopTimer(int channel)    - stop it
      setTimer(int channel, uint32_t timeout) - set channel count (no need to stop)
*---------------------------------------------------------------------------*/

#ifndef LPIT_H
#define LPIT_H
#include <MKL28Z7.h>

// The PIT functional clock is DIV3, which is 8MHz (SIRClock)
#define PITCLOCK (8000000)

// prototypes
void configureLPIT_interrupt(int channel) ;
void startTimer(int channel) ;
void stopTimer(int channel) ;
void setTimer(int channel, uint32_t timeout) ;

#endif
