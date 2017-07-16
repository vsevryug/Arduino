#ifndef _DIGISENSOR_H_
#define _DIGISENSOR_H_

//#include <String.h>
#include <WString.h>
#include "Arduino.h"

class DigiSensor {
  public:
    DigiSensor(int pin, String message0, String message1);
    void SetUp();
    String GetTextState();
    int GetAlarmState();
  private:
    int _pin;
    int sensorState;
    String _message0;
    String _message1;
};

#endif
