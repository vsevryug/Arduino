#include "flood.h"

const short pinBathroomSensor = 4;
const short pinKitchenSensor = 2;

// 220 or 1k resistor connected to LEDs pins
const int pinGreenLed = 8;   //1k    Ok i.e. No Flood
const int pinYellowLed = 9;  //220   Attention i.e. Line is broken
const int pinRedLed = 10;    //1k    Alarm or Flood was detected
const int pinGuardLed = A1;

//
const short pinMotorLeft = 12;
const short pinMotorRight = 11;

const int pinButtons = 0;

short ledState = sensorUnknowState;
boolean isInGuardMode = false; //isValveOpened

void setup()                    // run once, when the sketch starts
{
  setupOutputLowPin(pinGreenLed);
  setupOutputLowPin(pinYellowLed);
  setupOutputLowPin(pinRedLed);
  setupOutputLowPin(pinGuardLed);
  
  setupOutputLowPin(pinMotorLeft);
  setupOutputLowPin(pinMotorRight);
  
  stopMotor();
  Serial.begin(9600);
  Serial.println("start");      // a personal quirk
  delay(2000); 
}

void setupOutputLowPin(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void loop()                     // run over and over again
{
  readButtons();
  
  int floodState = sensorUnknowState;
  
//  SerialPrintln("--- Bathroom ----");
  int stateBathroom = getSensorState(pinBathroomSensor);
  printWaterSensorInfo(stateBathroom, "=== Bathroom:");
  floodState = stateBathroom;
  
//  SerialPrintln("--- Kitchen ----");
  int stateKitchen = getSensorState(pinKitchenSensor);
  printWaterSensorInfo(stateKitchen, "=== Kitchen:");
  floodState |= stateKitchen;
  
  lightLed(floodState);
  motorReact(floodState);

  delay(50);
}

void readButtons() {
  uint32_t ms = millis();
  static unsigned long prevButtonTime = 0;
  static boolean isButtonPressed = false;
  
  if(ms - prevButtonTime > 70) {
    int sensorValue = analogRead(pinButtons);
    if(sensorValue < 50) {
      // No button pressed
      isButtonPressed = false;
    } else if(!isButtonPressed)  {
      isButtonPressed = true;
      if(abs(sensorValue - 342) < 50) { // Open button pressed
        Serial.println(">>>Open<<<");
        turnMotor(1);
        isInGuardMode = false;
      } else if (sensorValue - 513 < 50) { // Close button pressed
        Serial.println(">>>Close<<<");
        turnMotor(0);
        isInGuardMode = false;
      } else if (sensorValue - 1018 < 50) { // Reset button pressed
        Serial.println(">>>Reset<<<");
        isInGuardMode = !isInGuardMode;
        
      }
      Serial.println(ms);
      Serial.println(prevButtonTime);
      Serial.println(ms - prevButtonTime);
      Serial.print("isInGuardMode=");Serial.println(isInGuardMode);
      prevButtonTime = ms;
    }
  }
  
//  Serial.print("Button value = ");
//  Serial.println(sensorValue);
}

void motorReact(int floodState) {
  if( (floodState & sensorAlarmState) && isInGuardMode) {
    SerialPrintln("motorReact floodState = ", floodState);
    SerialPrintln("motorReact sensorAlarmState = ",sensorAlarmState);
    SerialPrintln("motorReact isInGuardMode = ",isInGuardMode);
    turnMotor (1); 
    isInGuardMode = false;
  }
}

void lightLed(int floodState) {
  SerialPrintln("floodState=",floodState);
  
  int maskOffLeds = ledState^floodState;
  
  if(maskOffLeds & sensorOkState) {
    digitalWrite(pinGreenLed, LOW);
  }
  if(maskOffLeds & sensorBrokenState) {
    digitalWrite(pinYellowLed, LOW);
  }
  if(maskOffLeds & sensorAlarmState) {
    digitalWrite(pinRedLed, LOW);
  }
  
  if(floodState & sensorOkState) {
    digitalWrite(pinGreenLed, HIGH);
  }
  if(floodState & sensorBrokenState) {
    digitalWrite(pinYellowLed, HIGH);
  }
  if(floodState & sensorAlarmState) {
    digitalWrite(pinRedLed, HIGH);
  }
  
  if(isInGuardMode){
//    Serial.println("In Guard Mode");
    digitalWrite(pinGuardLed, HIGH);
  } else {
//    Serial.println("!In Guard Mode");    
    digitalWrite(pinGuardLed, LOW);
  }
  
  ledState = floodState;
}

void printWaterSensorInfo(int state, char info[]) {
  switch(state) {
    case sensorOkState:
      SerialPrintln(info, "OK");
        break;
    case sensorAlarmState:
      SerialPrintln(info, "FLOOD");
        break;
    case sensorBrokenState:
      SerialPrintln(info, "BROKEN");
        break;
    default:
      SerialPrintln(info, "UNKNOWN");
        break;
  }
}

short getSensorState(int sensorPin) {
   uint32_t ms = millis();
   static unsigned long prevLogTime = 0;
  
   long result = 0;
   pinMode(sensorPin, OUTPUT);       // make pin OUTPUT
   digitalWrite(sensorPin, HIGH);    // make pin HIGH to discharge capacitor - study the schematic
   delay(1);                       // wait a  ms to make sure cap is discharged

   pinMode(sensorPin, INPUT);        // turn pin into an input and time till pin goes low
   digitalWrite(sensorPin, LOW);     // turn pullups off - or it won't work
   while(digitalRead(sensorPin)){    // wait for pin to go low
      result++;
      if(result>1000) break;
   }
   
   if(ms - prevLogTime > 1000) {
     SerialPrintln("Flood pin#  :",sensorPin);
     SerialPrintln("Flood sensor:",result);
     prevLogTime = ms;
   }
   
   if(result==0) return sensorBrokenState;
   if(result>900) return sensorAlarmState;
   //if(result<700) 
     return sensorOkState;
}

long RCtime(int sensPin){
   long result = 0;
   pinMode(sensPin, OUTPUT);       // make pin OUTPUT
   digitalWrite(sensPin, HIGH);    // make pin HIGH to discharge capacitor - study the schematic
   delay(1);                       // wait a  ms to make sure cap is discharged

   pinMode(sensPin, INPUT);        // turn pin into an input and time till pin goes low
   digitalWrite(sensPin, LOW);     // turn pullups off - or it won't work
   while(digitalRead(sensPin)){    // wait for pin to go low
      result++;
   }
   return result;                   // report results   
}

void turnMotor(int direction)
{
  SerialPrintln("turnMotor", direction);
  boolean inPin1 = (direction ==1)? HIGH : LOW;
  boolean inPin2 = (direction ==1)? LOW  : HIGH;

  digitalWrite(pinMotorLeft, inPin1);
  digitalWrite(pinMotorRight , inPin2);
  
  delay(MotorTurnTimeMillisec);
  stopMotor();
}

void stopMotor(){
  digitalWrite(pinMotorLeft, LOW);
  digitalWrite(pinMotorRight , LOW);
}

void SerialPrint(char info[]) {
//  Serial.print(info);
}

void SerialPrintln(char info[]) {
//  Serial.println(info);
}

void SerialPrintln(int info) {
//  Serial.println(info);
}

void SerialPrintln(char info[], int state) {
  SerialPrint(info);
  SerialPrintln(state);
}

void SerialPrintln(char info[], char state[]) {
  SerialPrint(info);
  SerialPrintln(state);
}
