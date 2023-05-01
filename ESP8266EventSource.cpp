#include "ESP8266EventSource.h"

static String generateEventMessage(const char *data, const char *event, uint32_t id, uint32_t reconnect)
{
  String message = "";

  if (reconnect)
  {
    message += "retry: ";
    message += String(reconnect);
    message += "\r\n";
  }
  if (id)
  {
    message += "id: ";
    message += String(id);
    message += "\r\n";
  }
  if (event != NULL)
  {
    message += "event: ";
    message += event;
    message += "\r\n";
  }
  if (data != NULL)
  {
    message += "data: ";
    message += data;
    message += "\r\n";
  }
  message += "\r\n";

  return message;
}

// ESP8266EventSourceClient

uint32_t _counter = 0;

ESP8266EventSourceClient::ESP8266EventSourceClient(ESP8266WebServer *server)
{
  _client = server->client();
  _lastId = server->header("Last-Event-ID").toInt();
  _clientId = ++_counter;
  _remoteIP = _client.remoteIP();

  _client.setNoDelay(true);
  _client.setSync(true);
}

ESP8266EventSourceClient::~ESP8266EventSourceClient()
{
  close();
}

void ESP8266EventSourceClient::close()
{
  _client.flush();
  _client.stop();
  _client = WiFiClient();
}

void ESP8266EventSourceClient::send(const char *data, const char *event, uint32_t id, uint32_t reconnect)
{
  if (connected())
  {
    String message = generateEventMessage(data, event, id, reconnect);
    write(message.c_str(), message.length());
  }
}

void ESP8266EventSourceClient::write(const char *message, size_t len)
{
  if (_client.write(message, len) != len)
    close();
}

// ESP8266EventSource

ESP8266EventSource::ESP8266EventSource(const String &url)
    : _url(url), _count(0), _connectcb(NULL), _disconnectcb(NULL), _timer(0)
{
  for (size_t i = 0; i < EVENT_SOURCE_CLIENT_MAX; i++)
    _clients[i] = NULL;
}

ESP8266EventSource::~ESP8266EventSource()
{
  close();
}

void ESP8266EventSource::begin(ESP8266WebServer *server)
{
  _server = server;
}

bool ESP8266EventSource::handleRequest()
{
  if (_url != "" && _server->uri() != _url)
    return false;

  if (_count == EVENT_SOURCE_CLIENT_MAX)
  {
    // Browser connects to the server, and if the connection is broken – reconnects.
    // There will be no reconnection if the response has an incorrect Content-Type
    // header or its HTTP status differs from 301, 307, 200 and 204. So closing the
    // connection without any response would be better than a 503 status response.

    return false;
  }

  // Serial.println(_server->header("Last-Event-ID"));

  for (size_t i = 0; i < EVENT_SOURCE_CLIENT_MAX; i++)
  {
    if (_clients[i] != NULL)
      continue;

    _clients[i] = new ESP8266EventSourceClient(_server);
    _count++;

    _server->sendContent_P(PSTR("HTTP/1.1 200 OK\nContent-Type: text/event-stream;\nConnection: keep-alive\nCache-Control: no-cache\n\n"));

    if (_connectcb)
      _connectcb(_clients[i]);

    break;
  }

  return true;
}

void ESP8266EventSource::keepAlive()
{
  unsigned long now;

  if (_count == 0)
    return;

  now = millis();
  if (now - _timer < EVENT_SOURCE_KEEP_ALIVE)
    return;

  _timer = now;

  for (size_t i = 0; i < EVENT_SOURCE_CLIENT_MAX; i++)
    if (_clients[i] != NULL && !_clients[i]->connected())
      _handleDisconnect(i);
}

void ESP8266EventSource::close()
{
  for (size_t i = 0; i < EVENT_SOURCE_CLIENT_MAX; i++)
    if (_clients[i] != NULL)
      _handleDisconnect(i);
}

void ESP8266EventSource::send(const char *data, const char *event, uint32_t id, uint32_t reconnect)
{
  if (_count == 0)
    return;

  String message = generateEventMessage(data, event, id, reconnect);

  for (size_t i = 0; i < EVENT_SOURCE_CLIENT_MAX; i++)
    if (_clients[i] != NULL && _clients[i]->connected())
      _clients[i]->write(message.c_str(), message.length());
}

void ESP8266EventSource::onConnect(ESP8266EventSource::TClientHandlerFunction cb)
{
  _connectcb = cb;
}

void ESP8266EventSource::onDisconnect(ESP8266EventSource::TClientHandlerFunction cb)
{
  _disconnectcb = cb;
}

void ESP8266EventSource::_handleDisconnect(size_t i)
{
  if (_disconnectcb)
    _disconnectcb(_clients[i]);

  delete _clients[i];

  _clients[i] = NULL;
  _count--;
}
