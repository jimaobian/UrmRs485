///
/// @file		LocalLibrary.h
/// @brief		Library header
///
/// @details	<#details#>
/// @n
/// @n @b		Project UrmRs485
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Angelo qiao
/// @author		DFRobot
///
/// @date		8/14/14 8:34 PM
/// @version	<#version#>
///
/// @copyright	(c) Angelo qiao, 2014
/// @copyright	GNU General Public License
///
/// @see		ReadMe.txt for references
///

#include "Arduino.h"

#ifndef UrmRs485Library_h
#define UrmRs485Library_h

#define ReceivedCommandStackSize 10
#define SendingCommandStackSize 10

#define DefaultTimeOutDuration 1000

#define Header0 0x55
#define Header1 0xaa

#define Header0Index 0
#define Header1Index 1
#define AddressIndex 2
#define LengthIndex 3
#define CommandIndex 4
#define Content0Index 5
#define Content1Index 6

#define RequestDistanceCommand 0x02            //distance command
#define RequestTemperatureCommand 0x03           //temperature command
#define RequestMaxDistanceCommand 0x05         //distance threshold

#define SetMaxDistanceCommand 0x04
#define SetBaudrateCommand 0x08
#define SetAddressCommand 0x55

#define BroadcastAddress 0xab

#define OperationSuccess 0xcc
#define OperationFailure 0xee

#define Baudrate1200BPS 0x00
#define Baudrate2400BPS 0x01
#define Baudrate4800BPS 0x02
#define Baudrate9600BPS 0x03
#define Baudrate14400BPS 0x04
#define Baudrate19200BPS 0x05
#define Baudrate28800BPS 0x06
#define Baudrate38400BPS 0x07
#define Baudrate57600BPS 0x08
#define Baudrate115200BPS 0x09
#define Baudrate128000BPS 0x0A
#define Baudrate256000BPS 0x0B

#define ReceivedNull 0
#define ReceivedTimeOut 1
#define ReceivedDistance 2
#define ReceivedTemperature 3
#define ReceivedMaxDistance 4
#define ReceivedSetMaxDistanceReply 5
#define ReceivedSetBaudrateReply 6
#define ReceivedSetAddressReply 7
#define ReceivedWrongStack 8

#define SendingRingSize 8

class UrmRs485 {
  
private:
  HardwareSerial& SerialTransceiver;      //serial port to communicate with
  unsigned long timeOutTimer;
  unsigned long timeOutDuration;
  byte receivedCommandStack[ReceivedCommandStackSize];
  byte sendingCommandStack[SendingCommandStackSize];
  
  byte sendingRingAddressBuffer[SendingRingSize];
  byte sendingRingCommandBuffer[SendingRingSize];
  int sendingRingContentBuffer[SendingRingSize];
  
  byte sendingRingWriteIndex;
  byte sendingRingReadIndex;
  byte sendingRingLength;
  
  void executeRingBuffer();
  
  
  byte receivedCommandStackIndex;
  
  int rs485TriggerPin;
  
  void sendStack();
  byte parseStack();
  boolean validateStack();
  void finishStack();
  
  byte receivedAddress;
  int receivedContent;
  
  
  
  
public:
  UrmRs485(HardwareSerial& theSerial, int theRs485TriggerPin);
  
  byte available();
  
  boolean begin(unsigned long theBaudrate);
  
  boolean requestDistance(byte theAddress);
  boolean requestTemperature(byte theAddress);
  boolean requestMaxDistance(byte theAddress);
  
  boolean setMaxDistance(byte theAddress,int theMaxDistance);
  boolean setBaudrate(byte theAddress,unsigned long theBaudrate);
  boolean setAddress(byte theAddress);
  
  void setTimeout(unsigned long theTimeOutDuration);
  
  int readAddress();
  int read();
  
};


///
/// @brief	Blink a LED
/// @details	LED attached to pin is light on then light off
/// @n		Total cycle duration = ms
/// @param	pin pin to which the LED is attached
/// @param	times number of times
/// @param	ms cycle duration in ms
///

#endif
