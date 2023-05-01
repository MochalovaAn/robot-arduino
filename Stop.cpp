#include <Arduino.h>
#include "Stop.h"

void flash()
{
  digitalWrite(LED_STOP, LOW);
  delay(100);
  digitalWrite(LED_STOP, HIGH);
  delay(100);
}

void stop(int error)
{
  pinMode(LED_STOP, OUTPUT);
  while (1)
  {
    for (int i = 0; i < error; i++) flash();
    delay(2000);
  }
}
