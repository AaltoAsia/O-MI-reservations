/*
SoftwareSerial.h

SoftwareSerial.cpp - Implementation of the Arduino software serial for ESP8266.
Copyright (c) 2015-2016 Peter Lerup. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <inttypes.h>
#include <Stream.h>

#define DATA_SIZE 7//How many data do you want to get from serial?
#define START_BYTE 0XAA
#define STOP_BYTE  0XAA
// This class is compatible with the corresponding AVR one,
// the constructor however has an optional rx buffer size.
// Speed up to 115200 can be used.

class SoftwareSerial : public Stream
{
public:
   SoftwareSerial(int receivePin, int transmitPin, bool inverse_logic = false, unsigned int buffSize = 64);
   ~SoftwareSerial();

   void begin(long speed);
   void setTransmitEnablePin(int transmitEnablePin);

   int peek();

   virtual size_t write(uint8_t byte);
   virtual int read();
   virtual int available();
   virtual void flush();
   operator bool() {return m_rxValid || m_txValid;}

   // Disable or enable interrupts on the rx pin
   void enableRx(bool on);
 
    void rxRead();
   
   using Print::write;
 
 bool data_control(void){
 	
 	if(serInString[0]==START_BYTE && serInString[6]==STOP_BYTE){
 		return true;
	 }
	 else{
	 	return false;
	 }
 }
/*****************GET DATA FUNCTION********************
*
*  
*
*get_data[0]--->Start byte
*get_data[1]--->POWER_HIGH BYTE
*get_data[2]--->POWER_LOW BYTE
*get_data[3]--->CURRENT_HIGH BYTE
*get_data[4]--->CURRENT_LOW BYTE
*get_data[5]--->RELAY_STATUS
*get_data[6]--->Stop byte
*
********************************************************/
byte get_data(int i){
	
 	if(i<DATA_SIZE&&data_control()){
 			return	serInString[i];//success
	 }
 
	 
 	
 }
private:
	
	 
   bool isValidGPIOpin(int pin);
 
   // Member variables
    byte serInString[DATA_SIZE];
   int  serInIndx  = 0;
   int m_rxPin, m_txPin, m_txEnablePin;
   bool m_rxValid, m_txValid, m_txEnableValid;
   bool m_invert;
   unsigned long m_bitTime;
   unsigned int m_inPos, m_outPos;
   int m_buffSize;
   uint8_t *m_buffer;

};


// If only one tx or rx wanted then use this as parameter for the unused pin
#define SW_SERIAL_UNUSED_PIN -1


#endif
