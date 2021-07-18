#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
//Joystick & button Pins
#define JOYSTICKone_Y    A6 
#define JOYSTICKone_X    A7
#define JOYSTICKtwo_Y    A1 
#define JOYSTICKtwo_X    A0
#define BUTTONONE 3
#define BUTTONTWO 4
 
RF24 radio(8, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Address
// Max size of this struct is 32 bytes - NRF24L01 buffer limit

struct Data_Package {
  byte joy1X;
  byte joy1Y;
  byte joy2X;
  byte joy2Y;
  byte button1;
  byte button2;
};

Data_Package data;
 
void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600);

  pinMode(JOYSTICKone_Y,INPUT);
  pinMode(JOYSTICKone_X,INPUT);
  pinMode(JOYSTICKtwo_Y,INPUT);
  pinMode(JOYSTICKtwo_X,INPUT);
  
  pinMode(BUTTONONE, INPUT_PULLUP);
  pinMode(BUTTONTWO, INPUT_PULLUP);
 
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  //radio.stopListening();
 
}

void loop() {


  ValueCollection();
  radio.write(&data, sizeof(Data_Package));
  //delay(100);


}

// RR-S Tank will only require one joystick and one button as of now...
void ValueCollection(){
  data.joy1Y = map(analogRead(JOYSTICKone_Y),0,1023,0,255); 
  data.joy1X = map(analogRead(JOYSTICKone_X),0,1023,0,255);
  data.joy2Y = map(analogRead(JOYSTICKtwo_Y),0,1023,0,255); 
  data.joy2X = map(analogRead(JOYSTICKtwo_X),0,1023,0,255);

  //Serial.print(data.joy1X);
  //Serial.print(" , ");
  //Serial.print(data.joy1Y);
  //Serial.print(" , ");
  //Serial.print(data.joy2X);
  //Serial.print(" , ");
  //Serial.print(data.joy2Y);
  //Serial.print(" , ");
  //Serial.print(data.button1);
  //Serial.print(" , ");
  //Serial.println(data.button2);

  data.button1 = digitalRead(BUTTONONE);
  data.button2 = digitalRead(BUTTONTWO);
  
}
