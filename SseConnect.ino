void handleOnConnect(AsyncEventSourceClient *client)
{
  unsigned long id = millis();

  Serial.print(client->client()->remoteIP());
  Serial.print(" - ");

  if (client->lastId())
  {
    Serial.print("EventSource Reconnected ");
    Serial.println(client->lastId());
  }
  else
  {
    Serial.print("EventSource Connected ");
    Serial.println(id);
  }

  client->send("hello!", NULL, id, 10000);
}
