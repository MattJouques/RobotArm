/*
Author:     BlitzResearch 
Version:    0.1
Platform:   Arduino
Model:      Mega 2560
Title:      OWI-RA-Controller
About:      Automated control of OWI Robot Arm modified to: 
                1) Add OpTo sensors on each of the primary joints
                2) Connection of motors to Adafruit Motor shields (2x stacked)
                3) Addition of an ACS712 current sensor for the fingers
                4) Addition of a TFT touch screen shield for control
                5) Addition of Maplin Linker joystick for Manual control
*/
//
// ----- Libraries -----
//
#include <Wire.h>                                       //
#include <Adafruit_MotorShield.h>                       // Adafruit Motor Shield Library           
//#include <Adafruit_GFX.h>                             // Adafruit Core graphics library
//#include <Adafruit_TFTLCD.h>                          // Adafruit Hardware-specific library
//#include <stdint.h>                                   // Standard Integer Types Library
//#include "TouchScreen.h"                              // Adafruit Touchscreen Library 

//
// ----- TFT & Touch Screen configuration -----
//
//#define LCD_CS A3                                     // Chip Select
//#define LCD_CD A2                                     // Command/Data
//#define LCD_WR A1                                     // LCD Write
//#define LCD_RD A0                                     // LCD Read
//#define LCD_RESET A4 
//#define YP A2                                         // must be an analog pin, use "An" notation!
//#define XM A3                                         // must be an analog pin, use "An" notation!
//#define YM 8                                          // can be a digital pin
//#define XP 9                                          // can be a digital pin

//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);    // Set to 300 ohms

//
// Joystick Configuration
//
const int joystickPinX = A0;
const int joystickPinY = A1;
int joystickMode = 1;                                   // Default Mode for Joystick control
int joystickValueX;                                     // x-Axis
int joystickValueY;                                     // y-Axis
int xLowTrigger = 500;
int xHighTrigger = 600;
int yLowTrigger = 600;
int yHighTrigger = 650;
//
// ----- Motor configuration -----
//
// Motor Shield
//
Adafruit_MotorShield AFMSbot(0x61);                     // Bottom board - (Rightmost jumper closed)
Adafruit_MotorShield AFMStop(0x60);                     // Top Board - (Default address, no jumpers)
//
// Rotation
//
const int rotateSensorPin = A8;                         // Pin for the Opto sensor
Adafruit_DCMotor *rotateMotor = AFMStop.getMotor(1);    // Base rotation Motor
const int rotateRange = 270;                            // Range of the joint in degrees
const int rotateSpeed = 75;                             // Speed to run the Motor 1-255  
int rotateRangeRegisters;                               // Number of sensor registers in Range
int rotateLowerLimit = 0000;                            // Set Lower Sensor Limit     
int rotateUpperLimit = 9999;                            // Set Upper Sensor Limit
int rotatePos;
int rotateTarget;
//
// Base
//
const int baseSensorPin = A9;                            // Pin for the Opto sensor
Adafruit_DCMotor *baseMotor = AFMStop.getMotor(3);      // Base Motor
const int baseRange = 180;                              // Range of the joint in degrees
const int baseSpeed = 75;                               // Speed to run the Motor 1-255     
int baseRangeRegisters;                                 // Number of sensor registers in Range
int baseLowerLimit = 0000;                              // Set Lower Sensor Limit     
int baseUpperLimit = 9999;                              // Set Upper Sensor Limit
int basePos;
int baseTarget;
//
// Elbow
//
const int elbowSensorPin = A10;                           // Pin for the Opto sensor
Adafruit_DCMotor *elbowMotor = AFMSbot.getMotor(3);     // Elbow Motor
const int elbowRange = 300;                             // Range of the joint in degrees
const int elbowSpeed = 150;                             // Speed to run the Motor 1-255  
int elbowRangeRegisters;                                // Number of sensor registers in Range
int elbowLowerLimit = 0000;                             // Set Lower Sensor Limit     
int elbowUpperLimit = 9999;                             // Set Upper Sensor Limit
int elbowPos;
int elbowTarget;
//
// Wrist
//
const int wristSensorPin = A11;                           // Pin for the Opto sensor
Adafruit_DCMotor *wristMotor = AFMSbot.getMotor(4);     // Wrist Motor
const int wristRange = 120;                             // Range of the joint in degrees
const int wristSpeed = 75;                              // Speed to run the Motor 1-255  
int wristRangeRegisters;                                // Number of sensor registers in Range
int wristLowerLimit = 0000;                             // Set Lower Sensor Limit     
int wristUpperLimit = 9999;                             // Set Upper Sensor Limit
int wristPos;
int wristTarget;
//
// Fingers
//
Adafruit_DCMotor *fingerMotor = AFMStop.getMotor(2);    // Finger Motor
const int fingerSpeed = 75;                             // Speed to run the Motor 1-255 
//
// Additional Hardware
//
int fingerLEDPin = 8;                                   // Head LED

        


// TODO - Add TFT


//
// ----- Program configuration -----
//
boolean serialState = false;                            // Status of the serial interface
int controlMode;
int loopCycles = 0;                                                // Check how many cycles


//
//
// -------------------------------------------------------------------------------------------
//
// 
void setup() {
  //
  // Establish serial communications / output if available
  //
  Serial.begin(9600);                                              // Initialize serial communications at 9600 bps:
  /*
  delay(4000);                                                     // Wait for Serial comms link to establish
  if (Serial.available() > 0)   {                                  // Check if Serial Connection is detected
      message("Serial is detected");                             // Serial State Message
     serialState = true;                                           // Enable Serial Comms
  } else {
    Serial.end();                                                  // Disable Serial Comms
  }
  */
  message("Serial is detected");   
  //
  // Initialise the screeen
  //
  
  //tft.reset();                                                      // Reset the TFT
  //uint16_t identifier = tft.readID();                               // Get TFT identifier
  //tft.begin(identifier);                                            // Start the driver
  
  //
  // Initialise the Arm
  //
  AFMSbot.begin();                                              // Initialise bottom Motor shield and set frequency
  AFMStop.begin();                                              // Initialise top Motor shield and set frequency
  message("motors initiated"); 
  //pinMode(fingerLEDPin, OUTPUT);                                    // Initialise the Grabber Light
  //currentPosition();                                                // Get current Arm position                     
  calibrate();
  //boolean testState = calibrate();                                  // Initiate the Calibration
  //if (testState = false) {                                          // Check for test failure
    //message("Self-Tests Failed - Manual Mode Only");                // Test status message
    //controlMode = 1;                                                // Set control mode to default - 1 (Manual)    
  //} else {                                                          // Test passes
    //message("Self-Tests Passed");                                   // Test status message
    //displayMenu();                                                  // Display the Main Menu
  //}
  moveRotate();                                                     // Move the rotate Axis to Home Position
  moveBase();                                                       // Move the base Axis to Home Position
  moveElbow();                                                      // Move the elbow Axis to Home Position
  moveWrist();                                                      // Move the wrist Axis to Home Position
 
}
//
//
//
void loop() {

  if (controlMode == 1) {                                           // Joystick controls for Manual Mode
    //
    // Joystick Inputs
    //
    joystickValueX = analogRead(joystickPinX);                      // Capture Joystick input x-axis
    joystickValueY = analogRead(joystickPinY);                      // Capture Joystick input y-axis
    int xLowTrigger = 490;                                          // set x-axis trigger value (left)
    int xHighTrigger = 530;                                         // set x-axis trigger value (right)
    int yLowTrigger = 490;                                          // set y-axis trigger value (up)
    int yHighTrigger = 530;                                         // set y-axis trigger value (down)
    //
    // Joystick button input - Mode Selection
    //
    if (joystickValueY > 1020) {                                    // Check for Button Press
      delay(500);                                                   // De-Bounce
      if (joystickMode == 1) {                                      // Mode 1
        joystickMode = 2;                                           // Switch to Mode 2
        message("Joystick Mode 2");
      } else if (joystickMode ==2) {                                // Mode 2
        joystickMode = 3;                                           // Switch to Mode 3
        message("Joystick Mode 3");
        digitalWrite(fingerLEDPin,HIGH);                            // Light the Grabber LED
      } else if (joystickMode == 3) {                               // Mode 3
        digitalWrite(fingerLEDPin,LOW);                             // Turn off the Grabber LED               
        joystickMode = 1;                                           // Switch to Mode 1
        message("Joystick Mode 1");
      }
      message("joystickMode = " + String(joystickMode));            // Send Message for current Mode
    }
    //
    // Manual Input
    //
    if (joystickMode == 1) {
        moveRotate();
        moveBase();
    } else if (joystickMode == 2) {
        moveElbow();
        moveWrist();
    } else if (joystickMode == 3) {
        moveWrist();
        moveFingers();
    }
  }
  savePosition();
  
}
//
// -------------------------------------------------------------------------------------------
//
// ----- Position Data -----
//
void currentPosition() {
  //
  // TODO - Translate position into 255 
  // TODO - EEPROM.read 
}
//
// ----- Return to Home or Save -----
//
void savePosition() {
  if (loopCycles == 50000) {
    controlMode = 9;
    rotateTarget = 0;
    baseTarget = 0;
    elbowTarget = 0;
    wristTarget = 0;
    moveRotate();
    moveBase();
    moveElbow();
    moveWrist();
    loopCycles = 0;
    controlMode = 1;
  } else {
    loopCycles ++;
  }
  // TODO - EEPROM.update
}
//
// ----- Move Rotate -----
//
void moveRotate() {
  //
  while (rotatePos > rotateLowerLimit && rotatePos < rotateUpperLimit) {            // Continue whilst in axis range
    if (controlMode == 1) {                                                         // Check to see if in Manual Mode
      if (joystickValueX < xLowTrigger) {                                           // x-Axis - Left
        rotateMotor->run(BACKWARD);                                                 // Set Motor diretion Backward
        rotateMotor->setSpeed(rotateSpeed);                                         // Set Motor Speed
        if  (analogRead(rotateSensorPin) == 1) {                                   // Capture sensor triggers
          rotatePos --;                                                             // decrement the sensor Register
        }
      } else if (joystickValueX > xHighTrigger) {                                   // x-Axis - Right
        rotateMotor->run(FORWARD);                                                  // Set Motor direction Forwards
        rotateMotor->setSpeed(rotateSpeed);                                         // Set Motor Speed
        if  (analogRead(rotateSensorPin) == 1) {                                   // Capture sensor triggers
          rotatePos ++;                                                             // increment the sensor Register
        }
      }
    } else {                                                                        // Program Mode
      while (rotatePos != rotateTarget) {                                           // Continue whilst Position not reached
        if (rotateTarget < rotatePos) {  
          rotateMotor->run(BACKWARD);                                               // Set Motor direction
          rotateMotor->setSpeed(rotateSpeed);                                       // Set Motor Speed
          if  (analogRead(rotateSensorPin) == 1) {                                 // Capture sensor triggers
            rotatePos --;                                                           // Decrement Sensor Register
          }
        } else if (rotateTarget > rotatePos) {                                      
          rotateMotor->run(FORWARD);                                                // Set Motor direction
          rotateMotor->setSpeed(rotateSpeed);                                       // Set Motor speed
          if  (analogRead(rotateSensorPin) == 1) {                                 // Capture sensor triggers
            rotatePos ++;                                                           // Increment Sensor Register
          }
        }
      }
    }
  }
  rotateMotor->setSpeed(0);                                                         // Stop the Motor
  rotateMotor->run(RELEASE);                                                        // Release the Motor
  message("Rotate Movement to " + String(rotatePos));                               // Show End Position
 // 
}
//
// ----- Move Base -----
//
void moveBase() {
  //
  //while (basePos > baseLowerLimit && basePos < baseUpperLimit) {                    // Continue whilst in axis range
    if (controlMode == 1 && baseTarget != 0) {                                      // Check to see if in Manual Mode
      if (joystickValueY < yLowTrigger) {                                           // x-Axis - Left
        baseMotor->run(BACKWARD);                                                   // Set Motor diretion Backward
        baseMotor->setSpeed(baseSpeed);                                             // Set Motor Speed
        if  (analogRead(baseSensorPin) == 1) {                                     // Capture sensor triggers
          basePos --;                                                               // decrement the sensor Register
        }
      } else if (joystickValueY > yHighTrigger) {                                   // x-Axis - Right
        baseMotor->run(FORWARD);                                                    // Set Motor direction Forwards
        baseMotor->setSpeed(baseSpeed);                                             // Set Motor Speed
        if  (analogRead(baseSensorPin) == 1) {                                     // Capture sensor triggers
          basePos ++;                                                               // increment the sensor Register
        }
      }
    } else {                                                                        // Program Mode
      while (basePos != baseTarget) {                                               // Continue whilst Position not reached
        if (baseTarget < basePos) {  
          baseMotor->run(BACKWARD);                                                 // Set Motor direction
          baseMotor->setSpeed(baseSpeed);                                           // Set Motor Speed
          if  (analogRead(baseSensorPin) == 1) {                                   // Capture sensor triggers
            basePos --;                                                             // Decrement Sensor Register
          }
        } else if (baseTarget > basePos) {                                      
          baseMotor->run(FORWARD);                                                  // Set Motor direction
          baseMotor->setSpeed(baseSpeed);                                           // Set Motor speed
          if  (analogRead(baseSensorPin) == 1) {                                   // Capture sensor triggers
            basePos ++;                                                             // Increment Sensor Register
          }
        }
      }
    }
  //}
  baseMotor->setSpeed(0);                                                           // Stop the Motor
  baseMotor->run(RELEASE);                                                          // Release the Motor
  message("Base Movement to " + String(basePos));                                   // Show End Position
 //
}
//
// ----- Move Elbow -----
//
void moveElbow() {
  //
  while (elbowPos > elbowLowerLimit && elbowPos < elbowUpperLimit) {                // Continue whilst in axis range
    if (controlMode == 1) {                                                         // Check to see if in Manual Mode
      if (joystickValueY < yLowTrigger) {                                           // x-Axis - Left
        elbowMotor->run(BACKWARD);                                                  // Set Motor diretion Backward
        elbowMotor->setSpeed(elbowSpeed);                                           // Run the Motor
        if  (analogRead(elbowSensorPin) == 1) {                                    // Capture sensor triggers
          elbowPos --;                                                              // decrement the sensor Register
        }
      } else if (joystickValueY > yHighTrigger) {                                   // x-Axis - Right
        elbowMotor->run(FORWARD);                                                   // Set Motor direction Forwards
        elbowMotor->setSpeed(elbowSpeed);                                           // Run the Motor
        if  (analogRead(elbowSensorPin) == 1) {                                    // Capture sensor triggers
          elbowPos ++;                                                              // increment the sensor Register
        }
      }
    } else {                                                                        // Program Mode
      while (elbowPos != elbowTarget) {                                             // Continue whilst Position not reached
        if (elbowTarget < elbowPos) {  
          elbowMotor->run(BACKWARD);                                                // Set Motor direction
          elbowMotor->setSpeed(elbowSpeed);                                         // Set Motor Speed
          if  (analogRead(elbowSensorPin) == 1) {                                  // Capture sensor triggers
            elbowPos --;                                                            // Decrement Sensor Register
          }
        } else if (elbowTarget > elbowPos) {                                      
          elbowMotor->run(FORWARD);                                                 // Set Motor direction
          elbowMotor->setSpeed(elbowSpeed);                                         // Set Motor speed
          if  (analogRead(elbowSensorPin) == 1) {                                  // Capture sensor triggers
            elbowPos ++;                                                            // Increment Sensor Register
          }
        }
      }
    }
  }
  elbowMotor->setSpeed(0);                                                          // Stop the Motor
  elbowMotor->run(RELEASE);                                                         // Release the Motor
  message("Elbow Movement to " + String(elbowPos));                                 // Show End Position
 //
}
//
// ----- Move Wrist -----
//
void moveWrist() {
  //
  while (wristPos > wristLowerLimit && wristPos < wristUpperLimit) {                // Continue whilst in axis range
    if (controlMode == 1) {                                                         // Check to see if in Manual Mode
      if (joystickValueX < xLowTrigger) {                                           // x-Axis - Left
        wristMotor->run(BACKWARD);                                                  // Set Motor diretion Backward
        wristMotor->setSpeed(wristSpeed);                                           // Run the Motor
        if  (analogRead(wristSensorPin) == 1) {                                    // Capture sensor triggers
          wristPos --;                                                              // decrement the sensor Register
        }
      } else if (joystickValueX > xHighTrigger) {                                   // x-Axis - Right
        wristMotor->run(FORWARD);                                                   // Set Motor direction Forwards
        wristMotor->setSpeed(wristSpeed);                                           // Run the Motor
        if  (analogRead(wristSensorPin) == 1) {                                    // Capture sensor triggers
          wristPos ++;                                                              // increment the sensor Register
        }
      }
    } else {                                                                        // Program Mode
      while (wristPos != wristTarget) {                                              // Continue whilst Position not reached
        if (wristTarget < wristPos) {  
          wristMotor->run(BACKWARD);                                                // Set Motor direction
          wristMotor->setSpeed(wristSpeed);                                         // Set Motor Speed
          if  (analogRead(wristSensorPin) == 1) {                                  // Capture sensor triggers
            wristPos --;                                                            // Decrement Sensor Register
          }
        } else if (wristTarget > wristPos) {                                      
          wristMotor->run(FORWARD);                                                 // Set Motor direction
          wristMotor->setSpeed(wristSpeed);                                         // Set Motor speed
          if  (analogRead(wristSensorPin) == 1) {                                  // Capture sensor triggers
            wristPos ++;                                                            // Increment Sensor Register
          }
        }
      }
    }
  }
  wristMotor->setSpeed(0);                                                          // Stop the Motor
  wristMotor->run(RELEASE);                                                         // Release the Motor
  message("Wrist Movement to " + String(wristPos));                                 // Show End Position
 //
}
//
// ----- Move Fingers -----
//
void moveFingers() {
  //
  // TODO - while current to fingers is less than x
  //
  if (controlMode == 1) {                                                           // Check to see if in Manual Mode
    if (joystickValueY < yLowTrigger) {                                             // x-Axis - Left
        fingerMotor->run(BACKWARD);                                                 // Set Motor diretion Backward
        fingerMotor->setSpeed(fingerSpeed);                                         // Run the Motor  
    } else if (joystickValueY > yHighTrigger) {                                     // x-Axis - Right
        fingerMotor->run(FORWARD);                                                  // Set Motor direction Forwards
        fingerMotor->setSpeed(fingerSpeed);                                         // Run the Motor
    }
  } else {                                                                          // Program Mode
     // TODO - Sort out a way of opening when the other movements done
     // TODO - Then closing on the object
  }
  fingerMotor->setSpeed(0);                                                         // Stop the Motor
  fingerMotor->run(RELEASE);                                                        // Release the Motor
 //
}
//
// ----- Calibrate Function -----
//
void calibrate() {
  int testStatus = true;                                                            // Assume the tests will complete ok
  int testDuration = 2000;                                                          // Set the standard test cycles
  int count = 0;
  //
  // Initiate Tests
  //
  message("Calibration initiated, STAND CLEAR!!");                                  // Send Warning message
  delay(5000);                                                                      // Wait before commencing calibration
  digitalWrite(fingerLEDPin,HIGH);                                                  // Light the Grabber LED
  //
  // Rotate
  //
  while (rotatePos > rotateLowerLimit && rotatePos < rotateUpperLimit) {            // Continue whilst in axis range
    rotateMotor->run(FORWARD);                                                      // Set Motor to forward Mode
    while (count < testDuration) {
      rotateMotor->setSpeed(rotateSpeed);                                           // Run the Motor
      if  (analogRead(rotateSensorPin) == 1) {                                      // Capture sensor triggers
        rotatePos ++; 
      }
      count++;
    }
    rotateMotor->setSpeed(0);                                                       // Stop the Motor
    if (rotatePos == 0) {                                                           // If no response from Sensor
      testStatus = false;                                                           // Fail the test
      message("rotate Sensor failure");
    } else {
      message("rotate sensor detected - at " + String(rotatePos));
    }
    rotateMotor->run(BACKWARD);                                                     // Change the direction
    while (count > 0) {
      rotateMotor->setSpeed(rotateSpeed);                                           // Run the Motor
      if  (analogRead(rotateSensorPin) == 1) {                                      // Capture sensor triggers
        rotatePos --; 
      }
      count--;
    }
    rotateMotor->setSpeed(0);                                                       // Stop the Motor
    rotateTarget = 0;                                                               // Set Target to Home
  }
  //
  // Base
  //
  while (basePos > baseLowerLimit && basePos < baseUpperLimit) {                    // Continue whilst in axis range
    baseMotor->run(FORWARD);                                                        // Set Motor to forward Mode
    while (count < testDuration) {
      baseMotor->setSpeed(baseSpeed);                                               // Run the Motor
      if  (analogRead(baseSensorPin) == 1) {                                        // Capture sensor triggers
        basePos ++; 
      }
      count++;
    }
    baseMotor->setSpeed(0);                                                         // Stop the Motor
    if (basePos == 0) {                                                             // If no response from Sensor
      testStatus = false;                                                           // Fail the test
      message("base Sensor failure");
    } else {
      message("base sensor detected - at " + String(basePos));
    }
    baseMotor->run(BACKWARD);                                                       // Change the direction
    while (count > 0) {
      baseMotor->setSpeed(baseSpeed);                                               // Run the Motor
      if  (analogRead(baseSensorPin) == 1) {                                        // Capture sensor triggers
        basePos --; 
      }
      count--;
    }
    baseMotor->setSpeed(0);                                                         // Stop the Motor
    baseTarget = 0;                                                                 // Set Target to Home
  }
  //
  // Elbow
  //
  while (elbowPos > elbowLowerLimit && elbowPos < elbowUpperLimit) {                // Continue whilst in axis range
    elbowMotor->run(FORWARD);                                                       // Set Motor to forward Mode
    while (count < testDuration) {
      elbowMotor->setSpeed(baseSpeed);                                              // Run the Motor
      if  (analogRead(elbowSensorPin) == 1) {                                       // Capture sensor triggers
        elbowPos ++; 
      }
      count++;
    }
    elbowMotor->setSpeed(0);                                                        // Stop the Motor
    if (elbowPos == 0) {                                                            // If no response from Sensor
      testStatus = false;                                                           // Fail the test
      message("elbow Sensor failure");
    } else {
      message("elbow sensor detected - at " + String(elbowPos));
    }
    elbowMotor->run(BACKWARD);                                                      // Change the direction
    while (count > 0) {
      elbowMotor->setSpeed(elbowSpeed);                                             // Run the Motor
      if  (analogRead(elbowSensorPin) == 1) {                                       // Capture sensor triggers
        elbowPos --; 
      }
      count--;
    }
    elbowMotor->setSpeed(0);                                                        // Stop the Motor
    elbowTarget = 0;                                                                // Set Target to Home
  }
  //
  // Wrist
  //
  while (wristPos > wristLowerLimit && wristPos < wristUpperLimit) {                // Continue whilst in axis range
    wristMotor->run(FORWARD);                                                       // Set Motor to forward Mode
    while (count < testDuration) {
      wristMotor->setSpeed(wristSpeed);                                             // Run the Motor
      if  (analogRead(wristSensorPin) == 1) {                                       // Capture sensor triggers
        wristPos ++; 
      }
      count++;
    }
    wristMotor->setSpeed(0);                                                        // Stop the Motor
    if (wristPos == 0) {                                                            // If no response from Sensor
      testStatus = false;                                                           // Fail the test
      message("wrist Sensor failure");
    } else {
      message("wrist sensor detected - at " + String(wristPos));
    }
    wristMotor->run(BACKWARD);                                                      // Change the direction
    while (count > 0) {
      wristMotor->setSpeed(wristSpeed);                                             // Run the Motor
      if  (analogRead(wristSensorPin) == 1) {                                       // Capture sensor triggers
        wristPos --; 
      }
      count--;
    }
    wristMotor->setSpeed(0);                                                        // Stop the Motor
    wristTarget = 0;                                                                // Set Target to Home
  }
  message("Test Cycle completed " + String(testStatus) + " at " + String(testDuration) + " cycles");
  digitalWrite(fingerLEDPin,LOW);                                                  // Turn Off the Grabber LED
  //return testStatus;
}
//
// ----- Message Function -----
//
void message(String content) {
  //if(serialState == true) {
    Serial.println(content);
  //}
  // TODO - Print message to screen
  return;
}
//
// ----- Main Menu -----
//
void displayMenu() {
  
  // TODO - If tests failed, limit selection
  // TODO - Default to Manual Mode
  
  
}



    

  // TO-DO - Go to Sleep
  
 
  
  // Write Position (Store) 
  


