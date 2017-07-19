#include "SIM900.h"
#include <SoftwareSerial.h>
//If not used, is better to exclude the HTTP library,for RAM saving.
//If your sketch reboots itself proprably you have finished, your memory available.

#include "sms.h"

#include "DigiSensor.h"

#define DEBUG_DACHA

//To change pins for Software Serial, use the two lines in GSM.cpp.
//const char smsNumber[]="+79040568161";
//const int reedPin = 4;

//int numdata;

SMSGSM sms;
char number[]="+79040568161";
char message[180];
char pos;


DigiSensor reed(4, "Metal Door Open", "Metal Door Closed");
DigiSensor pir(2, "NO Movement", "Movement");

void setup() 
{
  //Serial connection.
  Serial.begin(19200);
  Serial.println("Dacha Monitor");

  if (gsm.begin(19200)){
    Serial.println("\nstatus= SIM900: READY and STARTED");
  }
  else Serial.println("\nstatus=IDLE");
  
  //Очистка от старых смс
  for(pos=1;pos<=21;pos++) {
    sms.DeleteSMS(pos);
  }
  
  reed.SetUp();
  sendSMS("INFO","POWER ON. "+reed.GetTextState()+"\n"+pir.GetTextState());
  
//  parseMegafonBalanceString();
};


void loop() {
 pos=sms.IsSMSPresent(SMS_UNREAD);
 
 if((int)pos>0&&(int)pos<=20){
   Serial.print("New Message, POS=");
   Serial.println((int)pos);
   
   message[0]='\0';
   sms.GetSMS((int)pos,number,message,180);
   
   Serial.print("SMS text:");
   Serial.println(message);
   Serial.print("from number:");
   Serial.println(number);
   
   if(!strcmp("KAK?",message)) {
//     sendSMS("INFO",getReedStatus());
//     checkMegafonBalance();
   } else if(!strcmp("1 etaz?", message)){
   } else if(!strcmp("2 etaz?", message)){
   }
   
   sms.DeleteSMS((int)pos);
 }
 
 checkReedStatus();
 checkPirStatus();
 
 delay(5000);
};

char sendSMS(String smsStatus, String message) {
  char smsText[180];
  String prepareString;
  char result = 'V';
  
#ifdef DEBUG_DACHA    
  prepareString = "DEBUG SMS:"+smsStatus+'.'+message;
  Serial.println(prepareString);
#else
  prepareString = smsStatus+'.'+message;
  prepareString.toCharArray(smsText, 180);
  result = sms.SendSMS("+79040568161", smsText);
#endif

  return result;
}

void parseMegafonBalanceString() {
//  //SMS text:003100330036002E003500300440002E0414043B044F0020044204350445002C0020043A0442043E0020043D043500200441043F043804420020002D00200437043D0430043A043
//  //SMS text:
//  // 0031 0033 0036 002E 0035 0030 0440 002E
//  // 1    3    6    .    5    0    p    .
//  //from number:Balance
//  //Например
//  //136.50р.Жми *243# и выиграй до 5000 руб!Проверь свои знания! (12р/д)
  int startPos=0;
  char sms2Parse[] = "003100330036002E003500300440002E0414043B044F0020044204350445002C0020043A0442043E0020043D043500200441043F043804420020002D00200437043D0430043A043\0";
  
  Serial.print("sms2Parse=");
  Serial.println(sms2Parse);
  String strSms(sms2Parse);
  Serial.print("strSms=");
  Serial.println(strSms);

 // int smsLength=sms2Parse.length();
//   int posRURDot = StringContains(strSms,"0440002E");
   Serial.print("p. position=");
//   Serial.println(posRURDot);
  
//  while(startPos+4 < sizeof(sms2Parse)) {
//    
//    
//    
//    String split = sms2Parse.substring(startPos, startPos+4);
//    Serial.print("parsed part = ");
//    Serial.println(split);
//    startPos=startPos+4;
//    //if(!strcmp("
//  }
  Serial.println("END of parsed sms __________________________");
}

String checkMegafonBalance() {
  sms.SendSMS("000100","B");
  // send sms to number 000100
  // read reply
  // send reply text to admin
}

void checkReedStatus() {
  if(reed.GetAlarmState()) {
    sendSMS("ALARM", reed.GetTextState());
  }
}

void checkPirStatus() {
  if(pir.GetAlarmState()) {
    sendSMS("ALARM", pir.GetTextState());
  }
}

