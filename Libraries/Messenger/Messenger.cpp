

//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}

#include "WProgram.h"
#include "Messenger.h"




Messenger::Messenger()
{
	init(' ');
}

Messenger::Messenger(char separator)
{
	if (separator == 10 || separator == 13 || separator == 0)  separator = 32;
	init(separator);
}

void Messenger::init(char separator) 
{
	callback = NULL;
	token[0] = separator; 
	token[1] = 0;
	bufferLength = MESSENGERBUFFERSIZE;
    bufferLastIndex = MESSENGERBUFFERSIZE -1;
    reset();
}

void Messenger::attach(messengerCallbackFunction newFunction) {
	callback = newFunction;
}


void Messenger::reset() {
        bufferIndex = 0;
        messageState = 0;
        current = NULL;
        last = NULL;
        dumped = 1;
}

int Messenger::readInt() {

    if (next()) {
    	dumped = 1;
    	return atoi(current);
     }
  return 0;

}

char Messenger::readChar() {

  if (next()) {
  	 dumped = 1;
  	 return current[0];
 }
  return 0;

}

uint8_t Messenger::next() {

  char * temppointer= NULL;
  switch (messageState) {
  case 0:
    return 0;
  case 1:
    temppointer = buffer;
    messageState = 2;
  default:
    if (dumped) current = strtok_r(temppointer,token,&last);
    if (current != NULL) {
    	dumped = 0;
    	return 1; 
	}
  }
  
  return 0;
}

uint8_t Messenger::available() {
	
  return next();
  
}


uint8_t Messenger::process(int serialByte) {
     messageState = 0;
    if (serialByte > 0) {

      switch (serialByte) {
      case 0: 
      	break;
      case 10: // LF
        break;
      case 13: // CR
        buffer[bufferIndex]=0;
        reset();
        messageState = 1;
        current = buffer;
        break;
      default:
          buffer[bufferIndex]=serialByte;
          bufferIndex++;
           if (bufferIndex >= bufferLastIndex) reset();
         }
      } 
      if ( messageState == 1 && callback != NULL) (*callback)();
      return messageState;
 }



