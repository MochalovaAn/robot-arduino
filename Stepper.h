#ifndef STEPPER_H
#define STEPPER_H

#include <AccelStepper.h>

// ULN2003 motor driver pins

#define STEPPER_IN1 0  // D3
#define STEPPER_IN2 2  // D4
#define STEPPER_IN3 14 // D5
#define STEPPER_IN4 12 // D6

#define STEPPER_INTERFACE AccelStepper::HALF4WIRE // полушаговый, 4 проводной
#define STEPS_PER_REVOLUTION 4096

// #define STEPPER_INTERFACE AccelStepper::FULL4WIRE // поношаговый, 4 проводной
// #define STEPS_PER_REVOLUTION 2048

class Stepper : virtual public AccelStepper
{
public:
  Stepper() : AccelStepper(STEPPER_INTERFACE, STEPPER_IN1, STEPPER_IN2, STEPPER_IN3, STEPPER_IN4, false){};

  uint16_t stepsPerRevolution() { return STEPS_PER_REVOLUTION; }
};

#endif // STEPPER_H
