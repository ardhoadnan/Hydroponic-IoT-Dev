#include <Arduino.h>
#include <Wire.h>
//#include <RTClib.h>
#include <ArduinoJson.h>

//extern RTC_DS3231 rtc;
extern float sensor1;
int intrvl = 0;
long currentSecond;

//Publish data to mqtt server.
bool doPublish(int i_on)
{
  bool publish;
  //DateTime now = rtc.now();
  i_on = i_on * 1000; //konversi ke second
  
  //increment untuk interval
  /*if (currentSecond != now.secondstime())
  {
    intrvl++;
    currentSecond = now.secondstime();
    //Serial.println(intrvl);
  }*/
  if (currentSecond != millis())
  {
    intrvl++;
    currentSecond = millis();
  }
  
  if (i_on == intrvl)
  {
    publish = true;
    intrvl = 0;
  }
  else
  {
    publish = false;
    //intrvl = 0;
  }
  return publish;
}