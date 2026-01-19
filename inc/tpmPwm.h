/*----------------------------------------------------------------------------
       ECS642/ECS714 Lab Work

    Header File for TPM
    
    Functions:
        configureTPMClock - configure clock for TPM
        configureTPM0forPWM - configure TPM0
        configureTPM2forPWM - configure TPM2
        setLEDBrightness - set brightness of the RGB LEDs
 *---------------------------------------------------------------------------*/

#ifndef TPM_PWM_H
#define TPM_PWM_H

#define PWM_DUTY_MAX (255)

// prototypes
void configureTPMClock(void) ;
void configureTPM0forPWM(void) ;
void configureTPM2forPWM(void) ;

extern const unsigned int MAXBRIGHTNESS ;
enum LED {Red, Green, Blue} ;
void setLEDBrightness(enum LED led, unsigned int brightness) ;

#endif
