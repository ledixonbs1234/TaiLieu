#include <Arduino.h>

const int trig = D5;
const int echo = D6;
void setup()
{
  //Thuc hien lenh trong nay
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}
int count = 0;
bool isOn =false;
void loop(){
  unsigned long duration;
  int distance;
  
  //digitalWrite(trig,0);
  //delayMicroseconds(2);
  digitalWrite(trig,1);
  delayMicroseconds(5);
  digitalWrite(trig,0);

  duration = pulseIn(echo,1);
  distance = int((duration/2)/29.412);
  if(distance<10 && isOn ==false){
    count ++;
    isOn = true;
  }else{
    isOn = false;
  }


  Serial.println(count);
  delay(50);
}
