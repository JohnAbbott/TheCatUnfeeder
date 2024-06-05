/*
A Cat Un-Feeder
by Joseph Grey

Version one used RFID but that proved to be unusable because the tag had to be so 
    close to the reader
Version two uses the color temperature sensor to detect cat color.

*/

#include <Servo.h>
#include <Adafruit_TCS34725.h>

#define SS_PIN 10
#define RST_PIN 8
#define ColorLED 7   // The arduino pin that controls the onboard LED of the color temp sensor

Servo doorServo;
Servo doorLockServo;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


int doorServoPin = 9;
int doorServoLockPin = 3;
int doorOpen = 90;
int doorClosed = 2;
int doorUnlocked = 90;
int doorLocked = 0;
int ledPin = 6;                // LED 
int motionPin = 4;                 // PIR Out pin 
int pirStat = 0; // PIR status

void setup() {
   pinMode(ledPin, OUTPUT);     
   pinMode(motionPin, INPUT);     
   Serial.begin(9600);
   // Color temp sensor
   tcs.begin();
   pinMode(ColorLED,OUTPUT);
   digitalWrite(ColorLED, LOW);    // Turns off the LED
  
   doorServo.attach(doorServoPin);
   doorServo.write(doorOpen);
   doorLockServo.attach(doorServoLockPin);
   doorLockServo.write(doorUnlocked);
}

void loop(){
 int slowDoor;
 pirStat = digitalRead(motionPin); 
 //Serial.println(pirStat);
 if (pirStat == HIGH) {            // if motion detected
   digitalWrite(ledPin, HIGH);  // turn LED ON
   digitalWrite(ColorLED, HIGH);  // turn on the LED on the color temp sensor
   uint16_t r, g, b, c, colorTemp, lux;
   getRawData_noDelay(&r, &g, &b, &c);
   colorTemp = tcs.calculateColorTemperature(r, g, b);
   Serial.println(colorTemp);
   delay(2000);
   if (colorTemp < 5000){
      for (int slowDoor = 90; slowDoor >= 2; slowDoor--)
      {
        //Serial.println(slowDoor); 
        delay(10);  //Increase this time even more to slow the door down more.
        doorServo.write(slowDoor);
      }
      delay(1000);
      Serial.println("Locking Door");
      doorLockServo.write(doorLocked);
      delay(5000);
      Serial.println("Un-locking Door");
      doorLockServo.write(doorUnlocked);
      delay(1000);
      for (int slowDoor = 2; slowDoor <= 90; slowDoor++)
      {
        //Serial.println(slowDoor); 
        delay(10);  //Increase this time even more to slow the door down more.
        doorServo.write(slowDoor);
      }
   }
  } 
 else {
   digitalWrite(ledPin, LOW); // turn LED OFF if we have no motion
   digitalWrite(ColorLED, LOW);  // turn off the LED on the color sensor
 }
}

void getRawData_noDelay(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
  *c = tcs.read16(TCS34725_CDATAL);
  *r = tcs.read16(TCS34725_RDATAL);
  *g = tcs.read16(TCS34725_GDATAL);
  *b = tcs.read16(TCS34725_BDATAL);
}
