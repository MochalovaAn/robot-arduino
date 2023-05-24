#ifndef STEPPER_H
#define STEPPER_H

#include <AccelStepper.h>

// ULN2003 motor driver pins

#define IN1 D1
#define IN2 D2
#define IN3 D5
#define IN4 D6

// #define INTERFACE AccelStepper::HALF4WIRE // полушаговый, 4 проводной
// #define STEPS_PER_REVOLUTION 4096

#define INTERFACE AccelStepper::FULL4WIRE // полношаговый, 4 проводной
#define STEPS_PER_REVOLUTION 2048

class Stepper : virtual public AccelStepper
{
public:
  Stepper() : AccelStepper(INTERFACE, IN1, IN3, IN2, IN4, false){};

  uint16_t stepsPerRevolution() { return STEPS_PER_REVOLUTION; }
};

#endif // STEPPER_H
