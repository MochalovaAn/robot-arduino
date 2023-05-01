void handleEvents()
{
  if (handleCaptivePortal())
    return;

  if (webServer.method() != HTTP_GET)
  {
    methodNotAllowed("GET");
    return;
  }

  if (!events.handleRequest())
  {
    Serial.println("503");
    return;
  }
}

void eventsOnConnect(ESP8266EventSourceClient *client)
{
  unsigned long id = millis();

  Serial.println(200);

  Serial.print("EventSource: Connected ");
  Serial.print(client->remoteIP());
  Serial.print(", client ");
  Serial.print(client->clientId());
  Serial.print(", last ");
  Serial.println(client->lastId());

  client->send("hello!", NULL, id, 10000);
}

void eventsOnDisconnect(ESP8266EventSourceClient *client)
{
  Serial.print("EventSource: Disconnected ");
  Serial.print(client->remoteIP());
  Serial.print(", client ");
  Serial.print(client->clientId());
  Serial.println();
}
