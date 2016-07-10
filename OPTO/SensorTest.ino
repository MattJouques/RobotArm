
#include <Wire.h>                                       //
#include <Adafruit_MotorShield.h>                       // Adafruit Motor Shield Library         

Adafruit_MotorShield AFMSbot(0x61);                     // Bottom board - (Rightmost jumper closed)
Adafruit_MotorShield AFMStop(0x60);                     // Top Board - (Default address, no jumpers)

const int testSensorPin = A8;                          // Pin for the Opto sensor
Adafruit_DCMotor *testMotor = AFMStop.getMotor(1);    // Base rotation Motor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);                                              // Initialize serial communications at 9600 bps:
  Serial.println("Tests Started");
  
  AFMSbot.begin();                                              // Initialise bottom Motor shield and set frequency
  AFMStop.begin();                                              // Initialise top Motor shield and set frequency
  delay(5000);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  testMotor->run(FORWARD);                                                 // Set Motor direction
  int i = 0;
  int Sensor = 0;
  
  while (i < 2000) {
    testMotor->setSpeed(100); 
    if  (analogRead(testSensorPin) == 1) {                                   // Capture sensor triggers
      Sensor ++;                                                             // Increment Sensor Register
    }
    i ++;
  }
  testMotor->setSpeed(0);                                                           // Stop the Motor
  Serial.println("Sensor registers = " + String(Sensor));
  
  testMotor->run(RELEASE);
  delay(2000);
  
  testMotor->run(BACKWARD);                                                 // Set Motor direction
  i = 0;
  Sensor = 0;
  
  
  while (i < 2000) {
    testMotor->setSpeed(100); 
    if  (analogRead(testSensorPin) == 1) {                                   // Capture sensor triggers
      Sensor --;                                                             // Increment Sensor Register
    }
    i ++;
  }
  testMotor->setSpeed(0);  
  Serial.println("Sensor registers = " + String(Sensor));
  

  testMotor->run(RELEASE);
  delay(2000);


}
