#include <Arduino.h>
#include <ct1642.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

const char *ssid = "Huong -2.4G";
const char *password = "0348759205";
const char *currentVersion = "1.0";
const char *serverUrl = "https://raw.githubusercontent.com/ledixonbs1234/otaesp8266/main/firmware.bin?raw=true";
const char *address_firmware = "https://raw.githack.com/ledixonbs1234/otaesp8266/main/firmware.bin?raw=true";

#define CLK D5;
#define DATA D6;
String text = "";
void FirmwareUpdate()
{
  t_httpUpdate_return ret = ESPhttpUpdate.update(serverUrl, currentVersion);

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
    break;
  }
}
void setup()
{
  //thuc hien cac lenh khoi tao trong nay
  Serial.begin(9600);
  installCT1642(D5, D6);
  text = ESP.getFreeSketchSpace();
  Serial.print("ESP 8266 http update, current version: ");
  Serial.println(currentVersion);
  WiFi.begin(ssid, password);
  Serial.println("Dang mo wifi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Da ket noi thanh cong");
  FirmwareUpdate();
}


void loop()
{
  animationText("so3");
}