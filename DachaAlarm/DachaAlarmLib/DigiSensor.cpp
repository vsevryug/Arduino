#include "DigiSensor.h"


DigiSensor::DigiSensor(int pin, String message0, String message1) {
  _pin = pin;
  _message0 = message0;
  _message1 = message1;
}

void DigiSensor::SetUp() {
  pinMode(_pin, INPUT);
  digitalWrite(_pin, HIGH);
  
  delay(500);
  sensorState = digitalRead(_pin);
}

String DigiSensor::GetTextState() {
  int currentDoorState = digitalRead(_pin);
  
  if(currentDoorState == 0) {
      return _message0; // "Door Opened";
  } else
  
  return _message1; // "Door Closed";
}

int DigiSensor::GetAlarmState() {
  int currentSensorState = digitalRead(_pin);
  
  if(currentSensorState != sensorState) {
    sensorState = currentSensorState;
    return 1;
  }
  return 0;
}
