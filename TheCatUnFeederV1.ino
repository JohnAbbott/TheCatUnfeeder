/*
A Cat Un-Feeder
by Joseph Grey

*/

#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 8

Servo doorServo;
MFRC522 rfid522(SS_PIN, RST_PIN); // Instance of the class

int doorServoPin = 9;
int doorOpen = 90;
int doorClosed = 2;
int ledPin = 6;                // LED 
int motionPin = 4;                 // PIR Out pin 
int pirStat = 0; // PIR status

void setup() {
 pinMode(ledPin, OUTPUT);     
 pinMode(motionPin, INPUT);     
 Serial.begin(9600);
 SPI.begin();
 rfid522.PCD_Init();
 
 doorServo.attach(doorServoPin);
 doorServo.write(doorOpen);
 
}

void loop(){
 int slowDoor;
 pirStat = digitalRead(motionPin); 
 //Serial.println(pirStat);
 if (pirStat == HIGH) {            // if motion detected
   digitalWrite(ledPin, HIGH);  // turn LED ON
   if ( ! rfid522.PICC_IsNewCardPresent() )
   {
    return;
   }
   if ( ! rfid522.PICC_ReadCardSerial())
   { 
    return;
   }
   String content = "";
   byte letter;
   int i;
   for (byte i = 0; i <rfid522.uid.size; i++)
   {
    Serial.print(rfid522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid522.uid.uidByte[i], HEX);
     content.concat(String(rfid522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfid522.uid.uidByte[i], HEX));
    }

  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "90 1B 1F 83") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Success, looping");
    for (int slowDoor = 90; slowDoor >= 2; slowDoor--)
    {
      Serial.println(slowDoor); 
      delay(10);
      doorServo.write(slowDoor);
    }
    delay(5000);
    for (int slowDoor = 2; slowDoor <= 90; slowDoor++)
    {
      Serial.println(slowDoor); 
      delay(10);
      doorServo.write(slowDoor);
    }
    //doorServo.write(doorOpen);
  }
 } 
 else {
   digitalWrite(ledPin, LOW); // turn LED OFF if we have no motion
 }
} 
