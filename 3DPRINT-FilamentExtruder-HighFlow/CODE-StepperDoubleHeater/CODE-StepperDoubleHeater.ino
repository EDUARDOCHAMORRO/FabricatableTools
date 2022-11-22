/*
================================================================================
  Multifunctional ROBOT Control BOX (MRCB) 
  Beans Firmware

  (c) 2019 BeabsFirmware
  All code complies with GPLv2 and/or GPLv3

================================================================================
Greetings! Thank you for choosing Marlin as your 3D printer firmware.
To configure BEANS you must edit Configuration.h
located in the root 'Bean' folder.

================================================================================
  Based on Marlin Firmware
  (c) 2011-2018 MarlinFirmware
  Portions of Marlin are (c) by their respective authors.
  All code complies with GPLv2 and/or GPLv3
================================================================================
*/
#include <AutoPID.h>

#include "thermistor.h"
//
//OUTPUTS PWM
//
#define SERVO0_PIN        7 
#define SERVO1_PIN        6
#define SERVO2_PIN        5
#define SERVO3_PIN        4
//
//SENSORS
//
#define X_MIN_PIN           3
#define X_MAX_PIN           2
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19
#define Z_MIN_PROBE_PIN    32
//
// Temperature Sensors
//
#define TEMP_0_PIN         13   // Analog Input
#define TEMP_1_PIN         15   // Analog Input
#define TEMP_2_PIN         14   // Analog Input

// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
  #define X_CS_PIN         53


#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
  #define Y_CS_PIN         49


#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
  #define Z_CS_PIN         40

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24
  #define E0_CS_PIN        42


#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30
  #define E1_CS_PIN        44
//
// Heaters / Fans
//
#define MOSFET_D8_PIN      8
#define MOSFET_D9_PIN      9
#define MOSFET_D10_PIN    10
//
// Misc. Functions
//
#define SDSS               53
#define LED_PIN            13
#define PS_ON_PIN          12
/*

//
//LCD   ==> REMEMBER TO INSTALL U8glib to your ARDUINO library folder: http://code.google.com/p/u8glib/wiki/u8glib
//
 #ifndef ST7920_DELAY_1
    #define ST7920_DELAY_1 DELAY_NS(125)
  #endif
  #ifndef ST7920_DELAY_2
    #define ST7920_DELAY_2 DELAY_NS(125)
  #endif
  #ifndef ST7920_DELAY_3
    #define ST7920_DELAY_3 DELAY_NS(125)
  #endif
*/
//===========================================================================
//============================= TEMPERATURE SENSOR ==========================
//===========================================================================
thermistor therm0(A13,0);  // Analog Pin which is connected to the 3950 temperature sensor, and 0 represents TEMP_SENSOR_0 (see configuration.h for more information).
thermistor therm1(A15,0);  // Analog Pin which is connected to the 3950 temperature sensor, and 0 represents TEMP_SENSOR_0 (see configuration.h for more information).
thermistor therm2(A14,0);  // Analog Pin which is connected to the 3950 temperature sensor, and 0 represents TEMP_SENSOR_0 (see configuration.h for more information).

//===========================================================================
//============================= PID Settings ================================
//===========================================================================
// PID Tuning Guide here: http://reprap.org/wiki/PID_Tuning
//pid settings and gains
#define OUTPUT_MIN 0
#define OUTPUT_MAX 255
#define KP .12
#define KI .0003
#define KD 0
double temperature0, setPoint0, outputVal0;
double temperature1, setPoint1, outputVal1;
double temperature2, setPoint2, outputVal2;
//input/output variables passed by reference, so they are updated automatically
AutoPID myPID0(&temperature0, &setPoint0, &outputVal0, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);
AutoPID myPID1(&temperature1, &setPoint1, &outputVal1, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);
AutoPID myPID2(&temperature2, &setPoint2, &outputVal2, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);
//===========================================================================
//============================= MOTOR DEFINITIONS ===========================
//===========================================================================

  int stepDelayE0 =1000;
  //stepDelayE0 = 20;
  int retractionSpeed = 1000;
  int retractionAmount = 300;
  int reextrusionAmount = 200; //560
//===========================================================================
//============================= DEBOUNCING OCTOCOUPLER ======================
//===========================================================================
 int octocoupler1;
 int octocoupler1State;
 int lastoctocoupler1State = LOW;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 1;    // the debounce time; increase if the output flickers
//===========================================================================
//============================= RUNNING CODE ================================
//===========================================================================
void setup() {
  Serial.begin(250000);
  // CONFIG PINS
  //TEMPERATURE
  pinMode(TEMP_0_PIN, INPUT);
  pinMode(TEMP_1_PIN, INPUT);
  pinMode(TEMP_2_PIN, INPUT);
  //MOSFETS
  pinMode(MOSFET_D8_PIN, OUTPUT);
  pinMode(MOSFET_D9_PIN, OUTPUT);
  pinMode(MOSFET_D10_PIN, OUTPUT);
  //MOTORS
  pinMode(E0_STEP_PIN, OUTPUT);
  pinMode(E0_DIR_PIN, OUTPUT);
  pinMode(E0_ENABLE_PIN, OUTPUT);
  pinMode(E0_CS_PIN, OUTPUT);
  //SENSORS OCTOCOUPLER
  pinMode(X_MIN_PIN, INPUT);
  pinMode(X_MAX_PIN, INPUT);
  pinMode(Y_MAX_PIN, INPUT);
  pinMode(Z_MIN_PIN, INPUT);
  pinMode(Z_MAX_PIN, INPUT);
  pinMode(Z_MIN_PROBE_PIN, INPUT);
//
  //PID CONFIG
    //if temperature is more than 4 degrees below or above setpoint, OUTPUT will be set to min or max respectively
  myPID0.setBangBang(4);
 //myPID1.setBangBang(4);
 // myPID2.setBangBang(4);

  //set PID update interval to 4000ms
  myPID0.setTimeStep(1);
  //myPID1.setTimeStep(1);
  //myPID2.setTimeStep(2000);
}

void loop() {
  
  //--------PID TEMPERATURE-----

  setPoint0 = 235; // 220 DEGREES
  setPoint1 = 235; // 220 DEGREES
  setPoint2 = 235; // 220 DEGREES

  double temp0 = therm0.analog2temp(); // read temperature sensor 0
 // double temp1 = therm1.analog2temp(); // read temperature sensor 1
  //double temp2 = therm2.analog2temp(); // read temperature sensor 2
  temperature0 = temp0;
 // temperature1 = temp1;
 // temperature2 = temp2;
  myPID0.run(); //call every loop, updates automatically at certain time interval
 // myPID1.run(); //call every loop, updates automatically at certain time interval
 // myPID2.run(); //call every loop, updates automatically at certain time interval
  analogWrite(MOSFET_D8_PIN, outputVal0);
  analogWrite(MOSFET_D9_PIN, outputVal0);
  analogWrite(MOSFET_D10_PIN, outputVal0);
 //analogWrite(MOSFET_D9_PIN, outputVal1);
 // analogWrite(MOSFET_D10_PIN, outputVal2);
  

     
    
     //--------EXTRUDER E0-----//
  int reading1 = digitalRead(Z_MIN_PIN);
 
  if (reading1 != lastoctocoupler1State) {
              lastDebounceTime = millis();// reset the debouncing timer
            }

            
            if ((millis() - lastDebounceTime) > debounceDelay) {
              // whatever the reading is at, it's been there for longer than the debounce
              // delay, so take it as the actual current state:
          
                     if ( temp0 <= (setPoint0-20)){                         // prevent cold extrude
                       digitalWrite(E0_ENABLE_PIN, HIGH);
                       }else{
                                if (reading1 != octocoupler1State) {        // if the button state has changed:
                                    octocoupler1State = reading1;
                              
                                    
                                    if (octocoupler1State == HIGH) {        // only toggle the LED if the new button state is HIGH
                                          digitalWrite(E0_DIR_PIN,LOW);      // DO A SMALL RETRACTION - value related to reextrusion
                                          for (int i = 0; i <= retractionAmount ; i++) {
                                               digitalWrite(E0_STEP_PIN,HIGH);
                                               stepDelayE0 = retractionSpeed ;
                                               delayMicroseconds(stepDelayE0);
                                               digitalWrite(E0_STEP_PIN,LOW);   
                                              }
                                            
                                          digitalWrite(E0_ENABLE_PIN, HIGH); // turn extruder motor E0 motor off
                                          // octocoupler1 = !octocoupler1;
                                      }else{
                                          digitalWrite(E0_ENABLE_PIN, LOW); // turn extruder motor E0 motor on and reextrude
                                            for (int i = 0; i <= reextrusionAmount ; i++) {
                                               digitalWrite(E0_STEP_PIN,HIGH);
                                               stepDelayE0 = retractionSpeed ;
                                               delayMicroseconds(stepDelayE0);
                                               digitalWrite(E0_STEP_PIN,LOW);   
                                              }
                                          }
                                }
                        }
              
              }
            
            lastoctocoupler1State = reading1;           // save the reading. Next time through the loop, it'll be the lastButtonState:

      //--------Extruder Motor Run-----//
            digitalWrite(E0_DIR_PIN,HIGH);  
            digitalWrite(E0_STEP_PIN,HIGH);
            delayMicroseconds(stepDelayE0);
            digitalWrite(E0_STEP_PIN,LOW);

      //--------SERIAL PRINT DEBBUGGING-----//
         Serial.print("Temperature: "); 
           Serial.print((String)temp0);
           Serial.print(" C --- ");
           Serial.print("outputVal0: "); 
           Serial.println(outputVal0);
           Serial.println(octocoupler1State);
           //Serial.flush();
  /*
  if (octocoupler1 ==1){
            //digitalWrite(E0_CS_PIN ,HIGH);       
            digitalWrite(E0_ENABLE_PIN,LOW);
            digitalWrite(E0_DIR_PIN,LOW);  
                    stepDelayE0 = 50;
            digitalWrite(E0_STEP_PIN,HIGH);
            delayMicroseconds(stepDelayE0);
            digitalWrite(E0_STEP_PIN,LOW);
            }
            else{
              digitalWrite(E0_ENABLE_PIN,HIGH);
              }*/
}
