void flash()
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
}

void stop(int error)
{
  pinMode(LED_BUILTIN, OUTPUT);
  while (1)
  {
    for (int i = 0; i < error; i++) flash();
    delay(1000);
  }
}
