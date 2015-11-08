#include <SPI.h>
#include "RF24.h"

RF24 radio(9,10);
byte addresses[][6] = {"camPi","robot"};

int M1A = 5;
int M1B = 6;
int M2A = 3;
int M2B = 4;

void setMotors(short leftSpeed, short rightSpeed)
{
  if (leftSpeed > 0)
  {
    analogWrite(M1A, leftSpeed);
    analogWrite(M1B, 0);
  }
  else
  {
    analogWrite(M1A, 0);
    analogWrite(M1B, -leftSpeed);
  }
  
  if (rightSpeed > 0)
  {
    analogWrite(M2A, rightSpeed);
    analogWrite(M2B, 0);
  }
  else
  {
    analogWrite(M2A, 0);
    analogWrite(M2B, -rightSpeed);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
    uint8_t gotChar;
    
    if( radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &gotChar, sizeof(gotChar) );             // Get the payload
      }
      
      if (gotChar == 'l')
      {
        setMotors(128, -128);
      }
      else if (gotChar == 'r')
      {
        setMotors(-128,128);
      }
      else if (gotChar == 'f')
      {
        setMotors(128,128);
      }
      else if (gotChar == 'b')
      {
        setMotors(-128,-128);
      }
      else if (gotChar == 's')
      {
        setMotors(0,0);
      }
      /*
      radio.stopListening();                                        // First, stop listening so we can talk   
      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
      Serial.print(F("Sent response "));
      Serial.println(got_time);  */
   }
}

