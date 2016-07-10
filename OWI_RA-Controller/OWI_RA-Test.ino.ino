#include <Wire.h>
#include <Adafruit_MotorShield.h> 
//
// Initialise Boards
//
Adafruit_MotorShield AFMSbot(0x61);                     // Bottom board - (Rightmost jumper closed)
Adafruit_MotorShield AFMStop(0x60);                     // Top Board - (Default address, no jumpers)
//
// Initialise Motors
//
Adafruit_DCMotor *rotateMotor = AFMStop.getMotor(1);
Adafruit_DCMotor *baseMotor = AFMStop.getMotor(2);
Adafruit_DCMotor *elbowMotor = AFMSbot.getMotor(3);
Adafruit_DCMotor *wristMotor = AFMSbot.getMotor(4);
Adafruit_DCMotor *fingerMotor = AFMStop.getMotor(3);
//
// Initialise Sensors
//
String sensorInput = "Analog";

// Digital
/*
    int rotateSensorPin = 23;
    int baseSensorPin = 25;
    int elbowSensorPin = 27;
    int wristSensorPin = 29;
*/
 
 // Analog
 
    int rotateSensorPin = A8;
    int baseSensorPin = A9;
    int elbowSensorPin = A10;
    int wristSensorPin = A11;

//
// Initialise Joystick
//
int joystickPinX = A0;
int joystickPinY = A1;
int joystickValueX;                                     // x-Axis
int joystickValueY;                                     // y-Axis
int xLowTrigger = 590;
int xHighTrigger = 700;
int yLowTrigger = 600;
int yHighTrigger = 700;
//
// LED
// 
int fingerLEDPin = 8;                                   // Head LED
int testDuration = 7500;




void setup() {
  Serial.begin(9600);
  Serial.println("Serial Active");
  Serial.println("Waiting 10 Seconds for any code change to be sent");
  delay(10000);
  Serial.println("Continuing.....");
  Serial.println("Sensor input type set as " + String(sensorInput));
  //
  // LED
  //
  pinMode(fingerLEDPin, OUTPUT);
  Serial.println("LED Pin set");
  //
  // Motors
  //
  AFMSbot.begin();                                              // Initialise bottom Motor shield and set frequency
  Serial.println("Bottom Motor board Initialised");  
  AFMStop.begin();                                              // Initialise top Motor shield and set frequency
  Serial.println("Top Motor board Initialised");  
}

void loop() {
  Serial.println("Starting Test Cycle");
  delay(1000);
  //
  // Test LED
  //
  Serial.println("Testing LED - Check Operation");
  digitalWrite(fingerLEDPin,HIGH);
  delay(5000);
  digitalWrite(fingerLEDPin,LOW);
  Serial.println("LED Test Complete");
  //
  // Test Joystick input
  //
  Serial.println("Testing Joytstick");
  Serial.println("Press & hold Joystick Button and wait");
  delay(3000);
  joystickValueX = analogRead(joystickPinX);
  joystickValueY = analogRead(joystickPinY);
  if (joystickValueY > 1020) {
    Serial.println("Joytstick button on Y axis - Correct");
  } else if (joystickValueX > 1020) {
    Serial.println("Joytstick button on X axis - INCORRECT!! Switch Pins");
  } else {
    Serial.println("Joytstick button input not detected - Check connections");
  }
  Serial.println("Release Joytstick button");
  delay(2000);
  //
  // Test Joystick X axis Range
  //
  Serial.println("Joystick Full Left and hold for lowest x values");
  delay(1000);
  int i = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX);                      // Capture Joystick input x-axis
    Serial.println(joystickValueX);
    i ++;
  }
  Serial.println("Release Joystick to get mid point x values");
  i = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX);                      // Capture Joystick input x-axis
    Serial.println(joystickValueX);
    i ++;
  }
  Serial.println("Joystick Full Right and hold for highest x values");
  i = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX);                      // Capture Joystick input x-axis
    Serial.println(joystickValueX);
    i ++;
  }
  Serial.println("Joystick Release - X axis values complete");
  delay(1000);
  Serial.println("Joystick Full Up and hold for Highest y values");
  delay(1000);
  i = 0;
  while (i < testDuration) {
    joystickValueY = analogRead(joystickPinY);                      // Capture Joystick input x-axis
    Serial.println(joystickValueY);
    i ++;
  }
  Serial.println("Release Joystick to get mid point values");
  i = 0;
  while (i < testDuration) {
    joystickValueY = analogRead(joystickPinY);                      // Capture Joystick input x-axis
    Serial.println(joystickValueY);
    i ++;
  }
  Serial.println("Joystick Full Down and hold for Lowest y values");
  i = 0;
  while (i < testDuration) {
    joystickValueY = analogRead(joystickPinY);                      // Capture Joystick input x-axis
    Serial.println(joystickValueY);
    i ++;
  }
  Serial.println("Joystick Release - Y axis values complete");
  delay(3000);
  //
  // Motors & Sensors
  //
  int sensorValue = 0;
  Serial.println("Use Joystick X axis to control each Motor in turn");
  delay(1000);
  //
  // Rotate
  //
  Serial.println("Rotate Motor - Backwards");
  i = 0;
  sensorValue = 0;
  while (i < testDuration) {
    if (joystickValueX < xLowTrigger) {
      joystickValueX = analogRead(joystickPinX);
      rotateMotor->run(BACKWARD);
      rotateMotor->setSpeed(75);
      if (sensorInput == "Digital") {
        if  (digitalRead(rotateSensorPin) == 1) {                                   // Capture sensor triggers
          sensorValue -- ;                                                          // change the sensor Register
        }
      } else {
        if  (analogRead(rotateSensorPin) == 1) {                                    // Capture sensor triggers
          sensorValue -- ;                                                          // change the sensor Register
        }
      }
    } else {
      rotateMotor->setSpeed(0);                                                         // Stop the Motor
      rotateMotor->run(RELEASE); 
    }
    i ++;
  }
  Serial.println("Rotate Motor Test Backwards Complete - Registers = " + String(sensorValue));
  delay(1000);
  Serial.println("Rotate Motor - Forwards");
  i = 0;
  sensorValue = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX);
    if (joystickValueX > xHighTrigger) {
      rotateMotor->run(FORWARD);
      rotateMotor->setSpeed(75);
      if (sensorInput == "Digital") {
        if  (digitalRead(rotateSensorPin) == 1) {                                   // Capture sensor triggers
          sensorValue ++ ;                                                          // change the sensor Register
        }
      } else {
        if  (analogRead(rotateSensorPin) == 1) {                                    // Capture sensor triggers
          sensorValue ++ ;                                                          // change the sensor Register
        }
      }
    } else {
      rotateMotor->setSpeed(0);                                                         // Stop the Motor
      rotateMotor->run(RELEASE); 
    }
    i ++;
  }
  Serial.println("Rotate Motor Test FORWARDS Complete - Registers = " + String(sensorValue));
  delay(1000);
  Serial.println("Rotate Motor Test Complete");
  delay(2000);
  //
  // Base
  //
  Serial.println("Base Motor - Backwards");
  i = 0;
  sensorValue = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX);
    if (joystickValueX < xLowTrigger) {
      baseMotor->run(BACKWARD);
      baseMotor->setSpeed(75);
      if (sensorInput == "Digital") {
        if  (digitalRead(baseSensorPin) == 1) {                                   // Capture sensor triggers
          sensorValue -- ;                                                          // change the sensor Register
        }
      } else {
        if  (analogRead(baseSensorPin) == 1) {                                    // Capture sensor triggers
          sensorValue -- ;                                                          // change the sensor Register
        }
      }
    } else {
      baseMotor->setSpeed(0);                                                         // Stop the Motor
      baseMotor->run(RELEASE); 
    }
    i ++;
  }
  Serial.println("Base Motor Test Backwards Complete - Registers = " + String(sensorValue));
  delay(1000);
  Serial.println("Base Motor - Forwards");
  i = 0;
  sensorValue = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX);
    if (joystickValueX > xHighTrigger) {
      baseMotor->run(FORWARD);
      baseMotor->setSpeed(75);
      if (sensorInput == "Digital") {
        if  (digitalRead(baseSensorPin) == 1) {                                   // Capture sensor triggers
          sensorValue ++ ;                                                          // change the sensor Register
        }
      } else {
        if  (analogRead(baseSensorPin) == 1) {                                    // Capture sensor triggers
          sensorValue ++ ;                                                          // change the sensor Register
        }
      }
    } else {
      baseMotor->setSpeed(0);                                                         // Stop the Motor
      baseMotor->run(RELEASE); 
    }
    i ++;
  }
  Serial.println("Base Motor Test FORWARDS Complete - Registers = " + String(sensorValue));
  delay(1000);
  Serial.println("Base Motor Test Complete");
  delay(2000);
  //
  // Elbow
  //
  Serial.println("Elbow Motor - Backwards");
  i = 0;
  sensorValue = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX);
    if (joystickValueX < xLowTrigger) {
      elbowMotor->run(BACKWARD);
      elbowMotor->setSpeed(75);
      if (sensorInput == "Digital") {
        if  (digitalRead(elbowSensorPin) == 1) {                                   // Capture sensor triggers
          sensorValue -- ;                                                          // change the sensor Register
        }
      } else {
        if  (analogRead(elbowSensorPin) == 1) {                                    // Capture sensor triggers
          sensorValue -- ;                                                          // change the sensor Register
        }
      }
    } else {
      elbowMotor->setSpeed(0);                                                         // Stop the Motor
      elbowMotor->run(RELEASE); 
    }
    i ++;
  }
  Serial.println("Elbow Motor Test Backwards Complete - Registers = " + String(sensorValue));
  delay(1000);
  Serial.println("Elbow Motor - Forwards");
  i = 0;
  sensorValue = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX); 
    if (joystickValueX > xHighTrigger) {
      elbowMotor->run(FORWARD);
      elbowMotor->setSpeed(75);
      if (sensorInput == "Digital") {
        if  (digitalRead(elbowSensorPin) == 1) {                                   // Capture sensor triggers
          sensorValue ++ ;                                                          // change the sensor Register
        }
      } else {
        if  (analogRead(elbowSensorPin) == 1) {                                    // Capture sensor triggers
          sensorValue ++ ;                                                          // change the sensor Register
        }
      }
    } else {
      elbowMotor->setSpeed(0);                                                         // Stop the Motor
      elbowMotor->run(RELEASE); 
    }
    i ++;
  }
  Serial.println("Elbow Motor Test FORWARDS Complete - Registers = " + String(sensorValue));
  delay(1000);
  Serial.println("Elbow Motor Test Complete");
  delay(2000);  
  //
  // Wrist
  //
  Serial.println("Wrist Motor - Backwards");
  i = 0;
  sensorValue = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX); 
    if (joystickValueX < xLowTrigger) {
      wristMotor->run(BACKWARD);
      wristMotor->setSpeed(75);
      if (sensorInput == "Digital") {
        if  (digitalRead(wristSensorPin) == 1) {                                   // Capture sensor triggers
          sensorValue -- ;                                                          // change the sensor Register
        }
      } else {
        if  (analogRead(wristSensorPin) == 1) {                                    // Capture sensor triggers
          sensorValue -- ;                                                          // change the sensor Register
        }
      }
    } else {
      wristMotor->setSpeed(0);                                                         // Stop the Motor
      wristMotor->run(RELEASE); 
    }
    i ++;
  }
  Serial.println("Wrist Motor Test Backwards Complete - Registers = " + String(sensorValue));
  delay(1000);
  Serial.println("Wrist Motor - Forwards");
  i = 0;
  sensorValue = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX); 
    if (joystickValueX > xHighTrigger) {
      wristMotor->run(FORWARD);
      wristMotor->setSpeed(75);
      if (sensorInput == "Digital") {
        if  (digitalRead(wristSensorPin) == 1) {                                   // Capture sensor triggers
          sensorValue ++ ;                                                          // change the sensor Register
        }
      } else {
        if  (analogRead(wristSensorPin) == 1) {                                    // Capture sensor triggers
          sensorValue ++ ;                                                          // change the sensor Register
        }
      }
    } else {
      wristMotor->setSpeed(0);                                                         // Stop the Motor
      wristMotor->run(RELEASE); 
    }
    i ++;
  }
  Serial.println("Wrist Motor Test FORWARDS Complete - Registers = " + String(sensorValue));
  delay(1000);
  Serial.println("Wrist Motor Test Complete");
  delay(2000);
  //
  // Fingers
  //
  Serial.println("Grabber Motor - Backwards (left)");
  i = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX); 
    if (joystickValueX < xLowTrigger) {
      fingerMotor->run(BACKWARD);
      fingerMotor->setSpeed(75);
    } else {
      fingerMotor->setSpeed(0);                                                         // Stop the Motor
      fingerMotor->run(RELEASE); 
    }
    i ++;
  }
  delay(1000);
  Serial.println("Grabber Motor - Forwards (right)");
  i = 0;
  while (i < testDuration) {
    joystickValueX = analogRead(joystickPinX); 
    if (joystickValueX > xHighTrigger) {
      fingerMotor->run(FORWARD);
      fingerMotor->setSpeed(75);
    } else {
      fingerMotor->setSpeed(0);                                                         // Stop the Motor
      fingerMotor->run(RELEASE); 
    }
    i ++;
  }
  Serial.println("Grabber Motor - Complete");
  delay(1000);
  Serial.println("-------------------------");
  Serial.println("Motor and Sensor Tests are complete");
  Serial.println("Waiting 10 seconds before restarting Test Cycle");
  delay(10000);     
      
      
 
  

}
