#ifndef STEPPER_H
#define STEPPER_H

#include <AccelStepper.h>

// ULN2003 motor driver pins
//
// ULN2003 NodeMCU       Description
// ======= ==========    ====================================================
// INT1    D3 (GPIO00)
// INT2    D4 (GPIO02)
// INT3    D5 (GPIO14)
// INT4    D6 (GPIO12)

#define IN1 D3
#define IN2 D4
#define IN3 D5
#define IN4 D6

#define INTERFACE AccelStepper::FULL4WIRE // полношаговый, 4 проводной
#define STEPS_PER_REVOLUTION 2048

// #define INTERFACE AccelStepper::HALF4WIRE // полушаговый, 4 проводной
// #define STEPS_PER_REVOLUTION 4096

extern AccelStepper Stepper;

#endif // STEPPER_H
