#include "UrmRs485.h"

UrmRs485::UrmRs485(HardwareSerial& theSerial, int theRs485TriggerPin)
:SerialTransceiver(theSerial)
{
  
  sendingCommandStack[Header0Index]=Header0;
  sendingCommandStack[Header1Index]=Header1;
  receivedCommandStack[Header0Index]=Header0;
  receivedCommandStack[Header1Index]=Header1;
  
  sendingRingLength=0;
  sendingRingReadIndex=0;
  sendingRingWriteIndex=0;
  
  timeOutDuration=DefaultTimeOutDuration;
  rs485TriggerPin=theRs485TriggerPin;
  receivedCommandStackIndex=0;
}

boolean UrmRs485::begin(unsigned long theBaudrate)
{
  pinMode(rs485TriggerPin, OUTPUT);
  digitalWrite(rs485TriggerPin, LOW);
  SerialTransceiver.begin(theBaudrate);
  delay(100);
}

void UrmRs485::sendStack()
{
  byte stackLength=sendingCommandStack[LengthIndex]+5;
  
  if (stackLength>=SendingCommandStackSize) {
    return;
  }
  sendingCommandStack[stackLength]=0;
  
  //calculate the checksum
  for (int i=0; i< stackLength; i++) {
    sendingCommandStack[stackLength]+=sendingCommandStack[i];
  }
  digitalWrite(rs485TriggerPin, HIGH);
  SerialTransceiver.write((byte *)sendingCommandStack, stackLength+1);
  SerialTransceiver.flush();
  digitalWrite(rs485TriggerPin, LOW);
}

void UrmRs485::executeRingBuffer()
{
  sendingCommandStack[AddressIndex]=sendingRingAddressBuffer[sendingRingReadIndex];
  sendingCommandStack[CommandIndex]=sendingRingCommandBuffer[sendingRingReadIndex];
  
  switch (sendingCommandStack[CommandIndex]) {
    case RequestDistanceCommand:
      sendingCommandStack[LengthIndex]=0;
      break;
    case RequestTemperatureCommand:
      sendingCommandStack[LengthIndex]=0;
      break;
    case RequestMaxDistanceCommand:
      sendingCommandStack[LengthIndex]=0;
      break;
    case SetMaxDistanceCommand:
      sendingCommandStack[LengthIndex]=2;
      sendingCommandStack[Content0Index]=(byte)((sendingRingContentBuffer[sendingRingReadIndex]>>8)&0x00ff);
      sendingCommandStack[Content1Index]=(byte)(sendingRingContentBuffer[sendingRingReadIndex]&0x00ff);
      break;
    case SetBaudrateCommand:
      sendingCommandStack[LengthIndex]=1;
      sendingCommandStack[Content0Index]=(byte)(sendingRingContentBuffer[sendingRingReadIndex]);
      break;
    case SetAddressCommand:
      sendingCommandStack[LengthIndex]=1;
      sendingCommandStack[Content0Index]=(byte)(sendingRingContentBuffer[sendingRingReadIndex]);
      break;
      
    default:
      break;
  }
  
  sendStack();
  timeOutTimer=millis();
}


boolean UrmRs485::requestDistance(byte theAddress)
{
  if (sendingRingLength==SendingRingSize) {
    return false;
  }
  
  sendingRingAddressBuffer[sendingRingWriteIndex]=theAddress;
  sendingRingCommandBuffer[sendingRingWriteIndex]=RequestDistanceCommand;
  sendingRingWriteIndex=(sendingRingWriteIndex+1)%SendingRingSize;
  
  if (sendingRingLength==0){
    executeRingBuffer();
  }
  sendingRingLength++;
  return true;
}

boolean UrmRs485::requestTemperature(byte theAddress)
{
  if (sendingRingLength==SendingRingSize) {
    return false;
  }
  
  sendingRingAddressBuffer[sendingRingWriteIndex]=theAddress;
  sendingRingCommandBuffer[sendingRingWriteIndex]=RequestTemperatureCommand;
  sendingRingWriteIndex=(sendingRingWriteIndex+1)%SendingRingSize;
  
  if (sendingRingLength==0){
    executeRingBuffer();
  }
  sendingRingLength++;
  return true;
}



boolean UrmRs485::requestMaxDistance(byte theAddress)
{
  if (sendingRingLength==SendingRingSize) {
    return false;
  }
  
  sendingRingAddressBuffer[sendingRingWriteIndex]=theAddress;
  sendingRingCommandBuffer[sendingRingWriteIndex]=RequestMaxDistanceCommand;
  sendingRingWriteIndex=(sendingRingWriteIndex+1)%SendingRingSize;
  
  if (sendingRingLength==0){
    executeRingBuffer();
  }
  sendingRingLength++;
  return true;
}


boolean UrmRs485::setMaxDistance(byte theAddress,int theMaxDistance)
{
  if (sendingRingLength==SendingRingSize) {
    return false;
  }
  
  sendingRingAddressBuffer[sendingRingWriteIndex]=theAddress;
  sendingRingCommandBuffer[sendingRingWriteIndex]=SetMaxDistanceCommand;
  sendingRingContentBuffer[sendingRingWriteIndex]=theMaxDistance;
  sendingRingWriteIndex=(sendingRingWriteIndex+1)%SendingRingSize;
  
  if (sendingRingLength==0){
    executeRingBuffer();
  }
  sendingRingLength++;
  return true;
}


boolean UrmRs485::setBaudrate(byte theAddress,unsigned long theBaudrate)
{
  if (sendingRingLength==SendingRingSize) {
    return false;
  }
  
  sendingRingAddressBuffer[sendingRingWriteIndex]=theAddress;
  sendingRingCommandBuffer[sendingRingWriteIndex]=SetBaudrateCommand;
  
  switch (theBaudrate) {
    case 1200UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate1200BPS;
      break;
    case 2400UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate2400BPS;
      break;
    case 4800UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate4800BPS;
      break;
    case 9600UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate9600BPS;
      break;
    case 14400UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate14400BPS;
      break;
    case 19200UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate19200BPS;
      break;
    case 28800UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate28800BPS;
      break;
    case 38400UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate38400BPS;
      break;
    case 57600UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate57600BPS;
      break;
    case 115200UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate115200BPS;
      break;
    case 128000UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate128000BPS;
      break;
    case 256000UL:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate256000BPS;
      break;
    default:
      sendingRingContentBuffer[sendingRingWriteIndex]=Baudrate19200BPS;
      break;
  }
  
  sendingRingWriteIndex=(sendingRingWriteIndex+1)%SendingRingSize;
  
  if (sendingRingLength==0){
    executeRingBuffer();
  }
  sendingRingLength++;
  return true;
  
}
boolean UrmRs485::setAddress(byte theAddress)
{
  if (sendingRingLength==SendingRingSize) {
    return false;
  }
  
  sendingRingAddressBuffer[sendingRingWriteIndex]=BroadcastAddress;
  sendingRingCommandBuffer[sendingRingWriteIndex]=SetAddressCommand;
  sendingRingContentBuffer[sendingRingWriteIndex]=theAddress;
  sendingRingWriteIndex=(sendingRingWriteIndex+1)%SendingRingSize;
  
  if (sendingRingLength==0){
    executeRingBuffer();
  }
  sendingRingLength++;
  return true;
}

byte UrmRs485::parseStack()
{
  switch (receivedCommandStack[CommandIndex]) {
    case RequestDistanceCommand:
      receivedContent=receivedCommandStack[Content0Index];
      receivedContent<<=8;
      receivedContent&=0xff00;
      receivedContent|=receivedCommandStack[Content1Index];
      receivedAddress=receivedCommandStack[AddressIndex];
      return ReceivedDistance;
      
    case RequestTemperatureCommand:
      receivedContent=receivedCommandStack[Content0Index];
      receivedContent<<=8;
      receivedContent&=0xff00;
      receivedContent|=receivedCommandStack[Content1Index];
      receivedAddress=receivedCommandStack[AddressIndex];
      return ReceivedTemperature;
      
    case RequestMaxDistanceCommand:
      receivedContent=receivedCommandStack[Content0Index];
      receivedContent<<=8;
      receivedContent&=0xff00;
      receivedContent|=receivedCommandStack[Content1Index];
      receivedAddress=receivedCommandStack[AddressIndex];
      return ReceivedMaxDistance;
      
    case SetAddressCommand:
      if (receivedCommandStack[Content0Index]==OperationSuccess) {
        receivedContent=true;
      }
      else if (receivedCommandStack[Content0Index]==OperationFailure){
        receivedContent=false;
      }
      else{
        return ReceivedWrongStack;
      }
      receivedAddress=receivedCommandStack[AddressIndex];
      return ReceivedSetAddressReply;
      
    case SetBaudrateCommand:
      if (receivedCommandStack[Content0Index]==OperationSuccess) {
        receivedContent=true;
      }
      else if (receivedCommandStack[Content0Index]==OperationFailure){
        receivedContent=false;
      }
      else{
        return ReceivedWrongStack;
      }
      receivedAddress=receivedCommandStack[AddressIndex];
      return ReceivedSetBaudrateReply;
      
    case SetMaxDistanceCommand:
      if (receivedCommandStack[Content0Index]==OperationSuccess) {
        receivedContent=true;
      }
      else if (receivedCommandStack[Content0Index]==OperationFailure){
        receivedContent=false;
      }
      else{
        return ReceivedWrongStack;
      }
      receivedAddress=receivedCommandStack[AddressIndex];
      return ReceivedSetMaxDistanceReply;
      
    default:
      return ReceivedWrongStack;
  }
}
;
boolean UrmRs485::validateStack()
{
  byte checkSum=0x00;
  byte checkLength=receivedCommandStack[LengthIndex]+5;
  for (int i=0; i<checkLength; i++) {
    checkSum+=receivedCommandStack[i];
  }
  return (receivedCommandStack[checkLength]==checkSum);
}

void UrmRs485::finishStack()
{
  receivedCommandStackIndex=0;
  sendingRingLength--;
  sendingRingReadIndex=(sendingRingReadIndex+1)%SendingRingSize;
  
  if (sendingRingLength) {
    executeRingBuffer();
  }
}

int UrmRs485::read()
{
  return receivedContent;
}

int UrmRs485::readAddress()
{
  return receivedAddress;
}

byte UrmRs485::available()
{
  if (sendingRingLength) {
    if (millis()-timeOutTimer>=timeOutDuration) {
      finishStack();
      return ReceivedTimeOut;
    }
    
    while (SerialTransceiver.available()) {
      if (receivedCommandStackIndex) {
        //is receiving
        if (receivedCommandStackIndex==Header1Index) {
          if (SerialTransceiver.peek()!=Header1) {
            receivedCommandStackIndex=0;
            return ReceivedNull;
          }
        }
        
        if (receivedCommandStackIndex<=LengthIndex) {
          receivedCommandStack[receivedCommandStackIndex++]=SerialTransceiver.read();
        }
        else{
          if (receivedCommandStack[LengthIndex]<=(ReceivedCommandStackSize-5)) {
            receivedCommandStack[receivedCommandStackIndex++]=SerialTransceiver.read();
            if (receivedCommandStackIndex==receivedCommandStack[LengthIndex]+5+1) {
              if (validateStack()) {
                finishStack();
                return parseStack();
              }
              else{
                receivedCommandStackIndex=0;
                finishStack();
                return ReceivedWrongStack;
              }
            }
          }
          else{
            receivedCommandStackIndex=0;
            finishStack();
            return ReceivedWrongStack;
          }
        }
      }
      else{
        //is not receiving
        if (SerialTransceiver.read()==Header0) {
          receivedCommandStackIndex++;
        }
      }
    }
  }
  
  return ReceivedNull;
}


void UrmRs485::setTimeout(unsigned long theTimeOutDuration)
{
  timeOutDuration=theTimeOutDuration;
}

















