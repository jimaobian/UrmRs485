#include "Arduino.h"

// Include application, user and local libraries
#include "UrmRs485.h"

#define Rs485TriggerPin 2               //the pin to select whether the RS485 is output or input
#define DefaultBaudrate 19200UL         //the Default Baudrate for the Urm06_485
#define DefaultAddress 0x11             //the Default Address for the Urm06_485
#define DefaultMaxDistance (-1)         //the Default Max Distance for the Urm06_485. "-1" means there is no Max Distance limitation. Set the Max Distance can limit the Distance range and speed up the detection.

#define CustomizedTimeOutDuration 500   //Time Out Duration can be Customized in "ms" unit

// Define variables and constants
UrmRs485 urm(Serial,Rs485TriggerPin);  //select the Serial port for communication with Urm_485 sensor

// Add setup code
void setup()
{
  urm.begin(DefaultBaudrate);
  
//  urm.setTimeout(CustomizedTimeOutDuration);    //Time Out Duration can be Customized Here.
}

// Add loop code
void loop()
{
  static unsigned long sendingTimer=millis();
  if (millis()-sendingTimer>=1000) {
    sendingTimer=millis();
    
    if (!urm.requestDistance(DefaultAddress)) {
      Serial.println(F("Buffer Full, Discard!"));
    }
    
    if(!urm.requestTemperature(DefaultAddress)) {
      Serial.println(F("Buffer Full, Discard!"));
    }
    
    if(!urm.requestMaxDistance(DefaultAddress)) {
      Serial.println(F("Buffer Full, Discard!"));
    }
    
    //    if(!urm.setAddress(DefaultAddress)) {
    //      Serial.println(F("Buffer Full, Discard!"));
    //    }
    //
    //    if(!urm.setBaudrate(DefaultAddress, DefaultBaudrate)) {
    //      Serial.println(F("Buffer Full, Discard!"));
    //    }
    //
    //    if(!urm.setMaxDistance(DefaultAddress, DefaultMaxDistance)) {
    //      Serial.println(F("Buffer Full, Discard!"));
    //    }
  }
  
  
  
  switch (urm.available()) {
    case ReceivedNull:
      break;

    case ReceivedDistance:
      Serial.print(F("Address:"));
      Serial.println(urm.readAddress());
      Serial.print(F("Distance:"));
      Serial.print(urm.read());
      Serial.println(F("mm"));
      break;

    case ReceivedTemperature:
      Serial.print(F("Address:"));
      Serial.println(urm.readAddress());
      Serial.print(F("Temperature:"));
      Serial.print(urm.read()/10.0);
      Serial.println(F(" C"));
      break;

    case ReceivedMaxDistance:
      Serial.print(F("Address:"));
      Serial.println(urm.readAddress());
      Serial.print(F("MaxDistance:"));
      Serial.print(urm.read());
      Serial.println(F("mm"));
      break;

    case ReceivedSetMaxDistanceReply:
      Serial.print(F("Address:"));
      Serial.println(urm.readAddress());
      Serial.print(F("SetMaxDistance:"));
      if (urm.read()) {
        Serial.println(F("Success"));
      }
      else{
        Serial.println(F("Failure"));
      }
      break;

    case ReceivedSetBaudrateReply:
      Serial.print(F("Address:"));
      Serial.println(urm.readAddress());
      Serial.print(F("SetBaudrate:"));
      if (urm.read()) {
        Serial.println(F("Success"));
      }
      else{
        Serial.println(F("Failure"));
      }
      break;

    case ReceivedSetAddressReply:
      Serial.print(F("Address:"));
      Serial.println(urm.readAddress());
      Serial.print(F("SetAddress:"));
      if (urm.read()) {
        Serial.println(F("Success"));
      }
      else{
        Serial.println(F("Failure"));
      }
      break;

    case ReceivedTimeOut:
      Serial.println(F("Time Out"));
      break;

    case ReceivedWrongStack:
      Serial.println(F("Wrong Stack"));
      break;

    default:
      Serial.println(F("Unknown"));
      break;
  }
}

