#include <Arduino.h>

extern float sensor1;
int intrvl = 0;
long currentSecond;

//Publish data to mqtt server.
bool doPublish(int i_on)
{
  bool publish;
  i_on = i_on * 10; //konversi ke second
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