/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
//#include "utility/Adafruit_PWMServoDriver.h"


// OPTO
int rotateSensorPin = 2;
int baseSensorPin  = 3;
int rotateSensor = 0;
int baseSensor = 0;
int count = 0;


// Test Parameters

int rotateMove = 500;
int rotateSpeed = 75;
int baseMove = 500;
int baseSpeed = 75;

int delayTime = 2000;


// Create the motor shield object with the default I2C address
//Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

Adafruit_MotorShield AFMSbot(0x61);                   // Rightmost jumper closed
Adafruit_MotorShield AFMStop(0x60);                   // Default address, no jumpers


// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *rotateMotor = AFMStop.getMotor(2);
Adafruit_DCMotor *baseMotor = AFMStop.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  //AFMS.begin();  // create with the default frequency 1.6KHz
  AFMSbot.begin(1000);  // OR with a different frequency, say 1KHz
  AFMStop.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  //myMotor->setSpeed(100);
  //myMotor->run(FORWARD);
  // turn on motor
  //myMotor->run(RELEASE);
}

void loop() {
  uint8_t i;

  Serial.println("Moving Arm - " + String(count));
  
  // Forwards
  rotateMotor->run(FORWARD);
  baseMotor->run(FORWARD);
  //
  // Rotate
  //
  rotateSensor = 0;
  while (rotateSensor != rotateMove ) {
    rotateMotor->setSpeed(rotateSpeed);
    if  (digitalRead(rotateSensorPin) == 1) {
        rotateSensor ++; 
    }
  }
  rotateMotor->setSpeed(0);
  Serial.println("Rotate Motor moved forward " + String(rotateSensor));
  delay(delayTime);
  //
  // Base
  //
  baseSensor = 0;
  while (baseSensor != baseMove ) {
    baseMotor->setSpeed(baseSpeed);
    if  (digitalRead(baseSensorPin) == 1) {
        baseSensor ++; 
    }
  }
  baseMotor->setSpeed(0);
  Serial.println("Base Motor moved forward " + String(baseSensor));
  delay(delayTime);
  
  
  
  
  // Forwards
  rotateMotor->run(BACKWARD);
  baseMotor->run(BACKWARD);
  //
  // Rotate
  //
  rotateSensor = 0;
  while (rotateSensor != rotateMove ) {
    rotateMotor->setSpeed(rotateSpeed);
    if  (digitalRead(rotateSensorPin) == 1) {
        rotateSensor ++; 
    }
  }
  rotateMotor->setSpeed(0);
  Serial.println("Rotate Motor moved backward " + String(rotateSensor));
  delay(delayTime);
  //
  // Base
  //
  baseSensor = 0;
  while (baseSensor != baseMove ) {
    baseMotor->setSpeed(baseSpeed);
    if  (digitalRead(baseSensorPin) == 1) {
        baseSensor ++; 
    }
  }
  baseMotor->setSpeed(0);
  Serial.println("Base Motor moved backward " + String(baseSensor));
  delay(delayTime);
  
  rotateMotor->run(RELEASE);
  baseMotor->run(RELEASE);
  Serial.println("Move Complete");
  delay(delayTime);
  count++;
}
  

