#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
// Motor A Connections
#define enA 3
#define in1 2
#define in2 4
 
// Motor B Connections
#define enB 5
#define in3 6
#define in4 7

//Lights Connections
#define LEDs 9

bool LEDstate = false;
int oldbutton = 1;

#define posDIR 150 //maybe change to 140 or 145
#define negDIR 110

RF24 radio(8, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

struct Data_Package {
  byte joy1X;
  byte joy1Y;
  byte joy2X;
  byte joy2Y;
  byte button1;
  byte button2;
};

Data_Package data;
 
void setup(){
  // Setup Serial Monitor
  Serial.begin(9600);
  
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(LEDs, OUTPUT);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();

} 

void loop() {
   if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
   currentTime = millis();
   if ( currentTime - lastReceiveTime > 1500 ) { //resets data incase of disconnection
    resetData(); 
  }

  Serial.print("j1PotX: ");
  Serial.print(data.joy1X);
  Serial.print("; j1PotY: ");
  Serial.println(data.joy1Y);

  //controls motors
  motorControl(data.joy1X, data.joy1Y);

  //if button is pressed switch the state of the LEDs
  if (data.button1 == 0 && oldbutton == 1){
    LEDstate = !LEDstate;
  }

  //LEDS turn on if the state is true and off if not
  if (LEDstate == true){
    digitalWrite(LEDs,HIGH);
  }
  else {
    digitalWrite(LEDs,LOW);
  }
  oldbutton = data.button1;

}

void motorControl(int x, int y){
int Mspeed=0;
  if (x>negDIR && x<posDIR){
      if (y<negDIR){
      //Moves Tank Forwards
        Mspeed = map(y, posDIR, 0, 95, 255);
        analogWrite(enA, Mspeed);
        analogWrite(enB, Mspeed);
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
      }
      if (y>posDIR){
      //Moves Tank Backwards
        Mspeed = map(y, posDIR, 255, 95, 255);
        analogWrite(enA, Mspeed);
        analogWrite(enB, Mspeed);
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
      }
     if (y>negDIR && y<posDIR) { 
     //No movement if not moving joystick
        analogWrite(enA, 0);
        analogWrite(enB, 0);
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
     }
 }
 if (x>posDIR){
 //Rotates Tank clockwise
     Mspeed = map(x, posDIR, 255, 95, 255);
     analogWrite(enA, Mspeed);
     analogWrite(enB, Mspeed);
     digitalWrite(in1, HIGH);
     digitalWrite(in2, LOW);
     digitalWrite(in3, HIGH);
     digitalWrite(in4, LOW);
 }
 if (x<negDIR){
//Rotates Tank counter-clockwise
     Mspeed = map(x, negDIR, 0, 95, 255);
     analogWrite(enA, Mspeed);
     analogWrite(enB, Mspeed);
     digitalWrite(in1, LOW);
     digitalWrite(in2, HIGH);
     digitalWrite(in3, LOW);
     digitalWrite(in4, HIGH);
 }

  
}

void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.joy1X = 130;
  data.joy1Y = 130;
  data.joy2X = 130;
  data.joy2Y = 130;
  data.button1 = 1;
  data.button2 = 1;
  LEDstate = false;
}
