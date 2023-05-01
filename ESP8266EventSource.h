#ifndef ESP8266_EVENT_SOURCE
#define ESP8266_EVENT_SOURCE

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define EVENT_SOURCE_CLIENT_MAX 10
#define EVENT_SOURCE_KEEP_ALIVE 10000

class ESP8266EventSourceClient
{
public:
  ESP8266EventSourceClient(ESP8266WebServer *server);
  ~ESP8266EventSourceClient();

  WiFiClient &client() { return _client; }
  void close();
  void send(const char *message, const char *event = NULL, uint32_t id = 0, uint32_t reconnect = 0);
  bool connected() { return _client.connected(); }
  uint32_t lastId() const { return _lastId; }
  uint32_t clientId() const { return _clientId; }
  IPAddress remoteIP() const { return _remoteIP; }

  // system routines (do not call)
  void write(const char *message, size_t len);

private:
  WiFiClient _client;
  uint32_t _lastId;
  uint32_t _clientId;
  IPAddress _remoteIP;
};

class ESP8266EventSource
{
public:
  ESP8266EventSource(const String &url);
  ~ESP8266EventSource();

  typedef std::function<void(ESP8266EventSourceClient *client)> TClientHandlerFunction;

  void begin(ESP8266WebServer *server);
  bool handleRequest();
  void keepAlive();

  const char *url() const { return _url.c_str(); }
  void close();
  void onConnect(TClientHandlerFunction cb);
  void onDisconnect(TClientHandlerFunction cb);
  void send(const char *data, const char *event = NULL, uint32_t id = 0, uint32_t reconnect = 0);
  size_t count() const { return _count; }

  // system routines (do not call)
  void _handleDisconnect(size_t i);

private:
  String _url;
  ESP8266WebServer *_server;
  ESP8266EventSourceClient *_clients[EVENT_SOURCE_CLIENT_MAX];
  size_t _count;
  TClientHandlerFunction _connectcb;
  TClientHandlerFunction _disconnectcb;
  unsigned long _timer;
};

#endif
