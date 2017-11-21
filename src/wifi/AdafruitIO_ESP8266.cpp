//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Copyright (c) 2015-2016 Adafruit Industries
// Authors: Tony DiCola, Todd Treece
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.
//
#ifdef ESP8266

#include "AdafruitIO_ESP8266.h"

AdafruitIO_ESP8266::AdafruitIO_ESP8266(const char *user, const char *key, const char *ssid, const char *pass):AdafruitIO(user, key)
{
  _ssid = ssid;
  _pass = pass;
  _client = new WiFiClientSecure;
  _mqtt = new Adafruit_MQTT_Client(_client, _host, _mqtt_port);
  _http = new HttpClient(*_client, _host, _http_port);
}

AdafruitIO_ESP8266::~AdafruitIO_ESP8266()
{
  if(_client)
    delete _client;
  if(_mqtt)
    delete _mqtt;
}

void AdafruitIO_ESP8266::_connect()
{

  delay(100);
  //WIFI_STA = 1, WIFI_AP_STA = 3
  if (_wifi_mode == WIFI_OFF || _wifi_mode == WIFI_AP) {
    return;
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  //delay(10);
  WiFi.mode(_wifi_mode);
  WiFi.begin(_ssid, _pass);
  _status = AIO_NET_DISCONNECTED;
  delay(100);
  if (_wifi_mode == WIFI_AP_STA) {
      if (! _ap_ssid) {
        return;
      }
      if (! _ap_pass) {
        WiFi.softAP(_ap_ssid);
      } else {
        WiFi.softAP(_ap_ssid, _ap_pass);
      }
      //_status = AIO_DISCONNECTED;
      //return;
  }
  delay(100);
}

bool AdafruitIO_ESP8266::setAP(const char *ap_ssid, const char *ap_pass)
{
  if (! ap_ssid) {
    return 0;
  } else {
    _ap_ssid = ap_ssid;
  }
  _ap_pass = ap_pass;
  return 1;
}

aio_status_t AdafruitIO_ESP8266::networkStatus()
{

  switch(WiFi.status()) {
    case WL_CONNECTED:
      return AIO_NET_CONNECTED;
    case WL_CONNECT_FAILED:
      return AIO_NET_CONNECT_FAILED;
    case WL_IDLE_STATUS:
      return AIO_IDLE;
    default:
      return AIO_NET_DISCONNECTED;
  }

}

const char* AdafruitIO_ESP8266::connectionType()
{
  return "wifi";
}

#endif // ESP8266
