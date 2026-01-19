/* ------------------------------------------
       ECS642/ECS714 Lab4

   The green LED is displayed at different brightness levels using PWM
   The PIT is used to time the transition between the brightness levels
   A button press switches between two rates (by changing the PIT load value): 
       * a fast one cycles through all the brightness levels in 2 s
       * a medium one cycles in 5 s
			 * a slow one takes 9 s
  -------------------------------------------- */

#include <MKL28Z7.h>
#include <stdbool.h>
#include "../inc/SysTick.h"
#include "../inc/button.h"
#include "../inc/clock.h"
#include "../inc/lpit.h"
#include "../inc/TPMPWM.h"
#include "../inc/triColorLedPWM.h"

/* --------------------------------------
     Documentation
     =============
     This is a cyclic system with a cycle time of 10ms

     The file has a main function, four tasks:
       1. pollB1Task: polls shield button B1 (rate control)
       2. pollB2Task: polls shield button B2 (pattern switching)
       3. toggleRateTask: toggles between fast, medium and slow rates
       4. toggleTaskSelection: switches between colour patterns
     and the PIT interrupt service routine which changes the brightness
 -------------------------------------- */
 
/* --------------------------------------------
  Variables for communication between tasks
*----------------------------------------------------------------------------*/
bool pressedB1_ev;  // set by pollB1Task, cleared by toggleRateTask
bool pressedB2_ev;  // set by pollB2Task, cleared by toggleTaskSelection

/*----------------------------------------------------------------------------
  Button B1 Polling Task (Rate Control)
*----------------------------------------------------------------------------*/
int b1State;
int b1BounceCount;

void initPollB1Task() {
    b1State = BOPEN;
    pressedB1_ev = false;
    b1BounceCount = 0;
}

void pollB1Task() {
    if (b1BounceCount > 0) b1BounceCount--;
    
    switch (b1State) {
        case BOPEN:
            if (isPressed(B1)) {
                b1State = BCLOSED;
                pressedB1_ev = true;
            }
            break;

        case BCLOSED:
            if (!isPressed(B1)) {
                b1State = BBOUNCE;
                b1BounceCount = BOUNCEDELAY;
            }
            break;

        case BBOUNCE:
            if (isPressed(B1)) {
                b1State = BCLOSED;
            }
            else if (b1BounceCount == 0) {
                b1State = BOPEN;
            }
            break;
    }
}

/*----------------------------------------------------------------------------
  Button B2 Polling Task (Pattern Switching)
*----------------------------------------------------------------------------*/
int b2State;
int b2BounceCount;

void initPollB2Task() {
    b2State = BOPEN;
    pressedB2_ev = false;
    b2BounceCount = 0;
}

void pollB2Task() {
    if (b2BounceCount > 0) b2BounceCount--;
    
    switch (b2State) {
        case BOPEN:
            if (isPressed(B2)) {
                b2State = BCLOSED;
                pressedB2_ev = true;
            }
            break;

        case BCLOSED:
            if (!isPressed(B2)) {
                b2State = BBOUNCE;
                b2BounceCount = BOUNCEDELAY;
            }
            break;

        case BBOUNCE:
            if (isPressed(B2)) {
                b2State = BCLOSED;
            }
            else if (b2BounceCount == 0) {
                b2State = BOPEN;
            }
            break;
    }
}

/*----------------------------------------------------------------------------
  Pattern 1: Colour Sequence (Cube Edges)
  States traverse the edges of the RGB cube
*----------------------------------------------------------------------------*/
#define BlueInc 0
#define RedDec 1
#define GreenInc 2
#define BlueDec 3
#define RedInc 4
#define GreenDec 5

int stateSelection;
int maxB;
int redB;
int blueB;
int greenB;

void updateColourSequenceInit() {
    stateSelection = BlueInc;
    maxB = 31;
    redB = 31;  // Start at red corner
    blueB = 0;
    greenB = 0;
}

void updateColourSequence() {
    switch (stateSelection) {
        case BlueInc:
            if (blueB < maxB) {
                blueB++;
            }
            else {
                stateSelection = RedDec;
            }
            break;
            
        case RedDec:
            if (redB > 0) {
                redB--;
            }
            else {
                stateSelection = GreenInc;
            }
            break;
            
        case GreenInc:
            if (greenB < maxB) {
                greenB++;
            }
            else {
                stateSelection = BlueDec;
            }
            break;
            
        case BlueDec:
            if (blueB > 0) {
                blueB--;
            }
            else {
                stateSelection = RedInc;
            }
            break;
            
        case RedInc:
            if (redB < maxB) {
                redB++;
            }
            else {
                stateSelection = GreenDec;
            }
            break;
            
        case GreenDec:
            if (greenB > 0) {
                greenB--;
            }
            else {
                stateSelection = BlueInc;
            }
            break;
    }
}

/*----------------------------------------------------------------------------
  Pattern 2: Colour Combinations (Cube Faces)
  States traverse the faces of the RGB cube
*----------------------------------------------------------------------------*/
#define AllOff 0
#define RedBlue 1
#define RedGreen 2
#define BlueGreen 3

void updateColourComboInit() {
    stateSelection = AllOff;
    maxB = 31;
    redB = 0;   // Start with all off
    blueB = 0;
    greenB = 0;
}

void updateColourCombo() {
    switch (stateSelection) {
        case AllOff:
            if (redB < maxB && blueB < maxB) {
                redB++;
                blueB++;
            }
            else {
                stateSelection = RedBlue;
            }
            break;
            
        case RedBlue:
            if (blueB > 0 && greenB < maxB) {
                blueB--;
                greenB++;
            }
            else {	
                stateSelection = RedGreen;
            }
            break;
            
        case RedGreen:
            if (redB > 0 && blueB < maxB) {
                redB--;
                blueB++;
            }
            else {
                stateSelection = BlueGreen;
            }
            break;
            
        case BlueGreen:
            if (blueB > 0 && greenB > 0) {
                blueB--;
                greenB--;
            }
            else {
                stateSelection = AllOff;
            }
            break;
    }
}

/*----------------------------------------------------------------------------
  PIT Timer Configuration
  
  Pattern 1: 192 transitions (6 edges × 32 steps)
  Pattern 2: 128 transitions (4 faces × 32 steps)
*----------------------------------------------------------------------------*/
#define numOfTransitionsTask1 192
#define numOfTransitionsTask2 128

// LPIT load values (8 MHz clock)
uint32_t pitSlowCount = ((9 * 8000000) / numOfTransitionsTask1) - 1;
uint32_t pitMediumCount = ((5 * 8000000) / numOfTransitionsTask1) - 1;
uint32_t pitFastCount = ((2 * 8000000) / numOfTransitionsTask1) - 1;

/*----------------------------------------------------------------------------
  Task: Rate Control
  Cycles through Slow ? Medium ? Fast ? SLOW on B1 press
*----------------------------------------------------------------------------*/
#define FAST 0
#define MEDIUM 1
#define SLOW 2

int rateState;

void initToggleRateTask() {
    setTimer(0, pitSlowCount);
    rateState = SLOW;
}

void toggleRateTask() {
    switch (rateState) {
        case FAST:
            if (pressedB1_ev) {
                pressedB1_ev = false;
                setTimer(0, pitSlowCount);
                rateState = SLOW;
            }
            break;
            
        case MEDIUM:
            if (pressedB1_ev) {
                pressedB1_ev = false;
                setTimer(0, pitFastCount);
                rateState = FAST;
            }
            break;
            
        case SLOW:
            if (pressedB1_ev) {
                pressedB1_ev = false;
                setTimer(0, pitMediumCount);
                rateState = MEDIUM;
            }
            break;
    }
}

/*----------------------------------------------------------------------------
  Task: Pattern Selection
  Switches between the two colour patterns on B2 press
*----------------------------------------------------------------------------*/
#define taskOne 0
#define taskTwo 1

int taskSelection = taskOne;

void toggleTaskSelection() {
    switch (taskSelection) {
        case taskOne:
            if (pressedB2_ev) {
                pressedB2_ev = false;
                taskSelection = taskTwo;
                updateColourComboInit();
                // Recalculate timing for pattern 2
                pitSlowCount = ((9 * 8000000) / numOfTransitionsTask2) - 1;
                pitMediumCount = ((5 * 8000000) / numOfTransitionsTask2) - 1;
                pitFastCount = ((2 * 8000000) / numOfTransitionsTask2) - 1;
							
							  if (rateState == FAST) setTimer(0, pitFastCount);
                else if (rateState == MEDIUM) setTimer(0, pitMediumCount);
                else setTimer(0, pitSlowCount);
            }
            break;
            
        case taskTwo:
            if (pressedB2_ev) {
                pressedB2_ev = false;
                taskSelection = taskOne;
                updateColourSequenceInit();
                // Recalculate timing for pattern 1
                pitSlowCount = ((9 * 8000000) / numOfTransitionsTask1) - 1;
                pitMediumCount = ((5 * 8000000) / numOfTransitionsTask1) - 1;
                pitFastCount = ((2 * 8000000) / numOfTransitionsTask1) - 1;
            
								if (rateState == FAST) setTimer(0, pitFastCount);
                else if (rateState == MEDIUM) setTimer(0, pitMediumCount);
                else setTimer(0, pitSlowCount);
						}
            break;
    }
}

/*----------------------------------------------------------------------------
  LPIT Interrupt Service Routine
  Updates LED brightness based on current pattern
*----------------------------------------------------------------------------*/
void LPIT0_IRQHandler() {
    NVIC_ClearPendingIRQ(LPIT0_IRQn);

    if (LPIT0->MSR & LPIT_MSR_TIF0_MASK) {
        // Execute current pattern
        if (taskSelection == taskOne) {
            updateColourSequence();
        }
        else {
            updateColourCombo();
        }
        
        // Apply brightness values to LEDs
        setLEDBrightness(Red, redB);
        setLEDBrightness(Blue, blueB);
        setLEDBrightness(Green, greenB);
    }

    // Clear interrupt flags
    LPIT0->MSR = LPIT_MSR_TIF0(1) | LPIT_MSR_TIF1(1) | 
                 LPIT_MSR_TIF2(1) | LPIT_MSR_TIF3(1);
}

/*----------------------------------------------------------------------------
  Main Function
*----------------------------------------------------------------------------*/
int main(void) {
    // Initialize peripherals
    enablePeripheralClock();
    configureLEDforPWM();
    configureButtons(B1, false);
    configureButtons(B2, false);
    configureLPIT_interrupt(0);
    configureTPMClock();
    configureTPM0forPWM();
    Init_SysTick(1000);

    // Initialize tasks
    updateColourSequenceInit();
    initPollB1Task();
    initPollB2Task();
    initToggleRateTask();

    // Start timer and main loop
    startTimer(0);
    waitSysTickCounter(10);
    
    while (1) {
        pollB1Task();
        pollB2Task();
				toggleTaskSelection();
			  toggleRateTask();
        waitSysTickCounter(10);  // 10ms cycle time
    }
}