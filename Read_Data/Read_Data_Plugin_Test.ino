/*
Author:     BlitzResearch 
Version:    0.1
Platform:   Arduino
Title:      Simulator Cockpit Master
About:      Iniitate standard controls and debug funtions and utilise the XPData Library from http://arduino.svglobe.com to setup UDP communications to Xplane.
*/
//---------------------------
#include <SPI.h>         
#include <Ethernet.h>
#include <XPData.h>         // -- include XPlaneData library
//-----------------------------
byte MAC[] = {0x90, 0xA2, 0xDA, 0x0D, 0x9B, 0x64};         // MAC-Address of Arduino Ethernet shield
unsigned int XPR = 5080;                                   // 5080 (plugin), 49001 (X-plane) port to to receive data from Plugin
unsigned int XPW = 49000;                                  // port to send data to X-Plane
byte IP[] = {0, 0, 0, 0};                                  // This Arduino IP address (0,0,0,0 - for auto)
//------------------------------------

// Set the Variables for the program
int debug = 0;
String debugOut;

// Set the XPlane Read Variables
float AP_FD_Status;
float AP_Status;
float AP_Speed_State;
float AP_HDG_State;
float AP_ALT_State;
float AP_VS_State;
float AP_Nav_State;
float AP_12Bank_State;

// Set the Panel Variables
int AP_FD_Light;

//----------------------
void setup() {
  XPDStart (XPR, XPW, MAC, IP);                           //==! init XPlaneData
  Serial.begin(9600);                                     // Start Serial Interface
  while (Serial.available()==0 && millis()<4000);         // Check to see if Serial connection in place
  if (Serial.available()>0)   {                           // If Serial Connection is detected
     int test = Serial.read();                            // Clear the input buffer
     Serial.println("DEBUG \n");                          // State Debug Mode acive on Console
     debug = 1;                                           // Enable debug
  }
  if (moduleDetect() == true) {
    debugOut = "Additional Modules detected \n";
  } else {
    debugOut = "No additional modules detected \n";
  }
    
}
//--------------------------
void loop() 
{  
  XPDScan();  // Read Outputs from X-Plane ARDRef plugin (as defined in the out_1.cfg file)
  // --------------------------------
  // Get AutoPilot States if changed
  if (GetData(1) != AP_FD_Status) {
    AP_FD_Status = GetData(1);
    if (AP_FD_Status == 1) {
      digitalWrite(AP_FD_Light, HIGH);
    } else {
      digitalWrite(AP_FD_Light, LOW);
    }
    debugOut += AP_FD_Status;
  }
  if (GetData(2) != AP_Status) {
    AP_Status = GetData(2);
    Serial.println(AP_Status);
  }

  
  if (debug == 1 && debugOut != debugOut) {                // Check if debug enabled and if the content has changed
    Serial.print(debugOut);                                // Send content of debugOut to the console
    debugOut = "";                                         // Reset the content of debugOut
  }

} 
// --------------------------
// Function to determine if additional modules are attached & load appropriate headers
//
boolean moduleDetect() {
  return false;
}



 //=======================

/* -- this "out_1.cfg" file used for plugin:

1 3 CRJ/autopilot/fd_button 1
2 3 CRJ/autopilot/ap_eng_button 1
3 3 CRJ/autopilot/ap_disc_button 1
4 3 CRJ/autopilot/speed_button 1
5 3 CRJ/autopilot/hdg_button 1
6 3 CRJ/autopilot/alt_button 1
7 3 CRJ/autopilot/v_button 1
8 3 CRJ/autopilot/nav_button 1
9 3 CRJ/autopilot/12_bank_button 1

*/
