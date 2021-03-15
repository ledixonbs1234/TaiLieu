#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "bqE84StNq9oGMl961KtH_lgWSb8iNeeT";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Huong -2.4G";
char pass[] = "0348759205";
BlynkTimer timer;
float  voltage = 0;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, voltage);
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth,ssid,pass);
  timer.setInterval(500L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
  // Đọc tín hiệu analog từ chân A0:
  int sensorValue = analogRead(A0);
  // Chuyển đổi tín hiệu đọc được:
  voltage = sensorValue * (34.202 / 1023.0)*100;
  // Xuất ra ngoài màn hình:
  Serial.println(voltage);
}