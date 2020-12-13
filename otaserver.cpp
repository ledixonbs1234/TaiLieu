#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

const char *ssid = "TP-LINK_CDD650";
const char *password = "2d4ux6yu";
const char *currentVersion = "1.0";
const char *serverUrl = "https://node-esp-t.herokuapp.com/firmware";

void setup()
{
  Serial.begin(9600);
  Serial.println();
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

  //thuc hien cap nhat firmware tu web
  t_httpUpdate_return ref = ESPhttpUpdate.update();

  switch (ref)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("Http update faild Error (%d) : %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    break;
  case HTTP_UPDATE_NO_UPDATES:
    Serial.printf("Http no update");
    break;
  case HTTP_UPDATE_OK:
    Serial.println("Http update thanh cong");
    break;

  default:
    break;
  }
}

  void loop(){

  }

