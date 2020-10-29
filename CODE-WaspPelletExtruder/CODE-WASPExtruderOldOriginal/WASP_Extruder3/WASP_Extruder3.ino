//// Libraries ////
#include<RS232.h>
#include<RS485.h>

//// MOTOR Variables ////
//#include <AccelStepper.h>
//AccelStepper Xaxis(1, 3, 6);
int OutStep = Q0_0;
int OutDir = Q0_1;
int OutEnable = Q0_2; 
bool MotorEnable = false;
float MotorRev = 800;
float MotorSpeed = 0; // rev per minute
unsigned long LastRunTime = 0; 

//// Heater Variables ////
int OutHeater = R0_1;
int OutFan = R0_2;
int InTemp = I0_2; 
bool HeaterEnable = false;
bool FanEnable = false;
float HeaterTemp = 0; 
float HeaterTempActual = 0;
unsigned long LastReadTime = 0; 

//// Robot Variables ////
int inRobot = I0_0;

//// COM Variables ////
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  //// MOTOR setup////
  pinMode(OutDir, OUTPUT);
  pinMode(OutStep, OUTPUT);
  pinMode(OutEnable, OUTPUT);
  digitalWrite(OutEnable, !MotorEnable);

  //// Heater setup////
  pinMode(InTemp, INPUT);
  //analogReference(EXTERNAL);

  //// COM setup////
  inputString.reserve(200);
  Serial.begin(9600);
  Serial.println("Hello");

  
  //Init
  CMD("?"); //Display Default values
  //CMD("T 205");
  //CMD("F 1");
  //CMD("M 1"); //Motor ON
  //CMD("S 01"); //Set speed in Rev/min
}

void loop() {

  
  // MOTOR Controller
  int timeElapsed = millis()-LastRunTime;
  LastRunTime = millis();
  if(MotorEnable && MotorSpeed!=0){
    float StepPerMillis = MotorRev * MotorSpeed /60000 ;//400 step per rev * 10 rev per minute / 60 000 = 0.0666 per milisec = 66.6 per sec = 4000 per minute
    float MillisPerStep = 1/StepPerMillis ;// 0.0666 step per milisec = 66.6 per sec = 4000 per minute
      
    //Serial.println((String)timeElapsed + " elapsed");
      
    if(timeElapsed > (MillisPerStep+1)){
      
      //Serial.println((String)(timeElapsed/(MillisPerStep+1)) + " Steps");
      
      for(int steps = 0; steps <= timeElapsed/(MillisPerStep+1); steps++){
        digitalWrite(OutStep, HIGH);
          delayMicroseconds(2);
        digitalWrite(OutStep, LOW);
          delayMicroseconds(2);
      }
    }else{
      //Serial.println((String)MillisPerStep + " MillisPerStep");
      digitalWrite(OutStep, HIGH);
      if(MillisPerStep<1) delayMicroseconds(MillisPerStep*1000/2); 
      else delay(MillisPerStep/2);  
      digitalWrite(OutStep, LOW);
      if(MillisPerStep<1) delayMicroseconds(MillisPerStep*1000/2); 
      else delay(MillisPerStep/2);
    }
  }
  
  // HEATER Controller
    int timeElapsedRead = millis()-LastReadTime;
    if (timeElapsedRead > 5000){ // if 5 sec passed
      float reading = analogRead(InTemp);
      LastReadTime = millis();
//      Serial.print("Analog reading "); 
//      Serial.println(reading);

      // convert the value to resistance
      int SERIESRESISTOR = 4.7;//KOhm
      int THERMISTORNOMINAL = 100;//KOhm
      int TEMPERATURENOMINAL = 25;
      int BCOEFFICIENT =  3990;
      float RESISTANCECORRECTION = 2.5;
      
      //reading = 1023 / reading - 1;
      //reading = (1023*.5) / reading - 1; //M-Duino Analog is on 10V, signal is on 5V. max=509)
      reading = (1023*1.4) / reading - 1; //M-Duino Analog is on 10V, signal is on 14V. max>1024)
      reading = SERIESRESISTOR / reading;
      reading = reading * RESISTANCECORRECTION;
//      Serial.print("Thermistor resistance "); 
//      Serial.println(reading);
      
      float steinhart;
      steinhart = reading / THERMISTORNOMINAL;     // (R/Ro)
      steinhart = log(steinhart);                  // ln(R/Ro)
      steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
      steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
      steinhart = 1.0 / steinhart;                 // Invert
      steinhart -= 273.15;                         // convert to C
      
//      Serial.print("Temperature "); 
//      Serial.print(steinhart);
//      Serial.println(" *C");
      if(steinhart>0)
        HeaterTempActual = steinhart;
      Serial.println ("T= " + (String)HeaterTempActual + " / " + (String)HeaterTemp + " °C");    

/*      
      float R1 = 100000;
      float logR2, R2, T, Tc, Tf;
      float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
      int Vo = analogRead(InTemp);
      R2 = R1 * ((1023.0*.5) / (float)Vo - 1.0);
      logR2 = log(R2);
      T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
      Tc = T - 273.15;
      Tf = (Tc * 9.0)/ 5.0 + 32.0;
      
      Serial.print("Temperature: "); 
      Serial.print(Tf);
      Serial.print(" F; ");
      Serial.print(Tc);
      Serial.println(" C"); 
      
/*      // convert the value to resistance
      //Voltatge = reading / 1023 * 10;
      //Vout V * 1000 = Vout mV = 10 mV/°C × (Temp in °C)
      //(Temp in °C) = (Vout V * 1000) / 10 mV/°C 
      //(Temp in °C) = Vout V * 100 
      HeaterTempActual = (reading / 1023) * 1000;
      Serial.println("TA= " + (String)HeaterTempActual + " °C");
*/      
      if(HeaterTempActual<HeaterTemp && !HeaterEnable){
        HeaterEnable = true;
        digitalWrite(OutHeater, HeaterEnable);
        Serial.println("H= 1 Heater ON");        
      }
      if(HeaterTempActual>HeaterTemp && HeaterEnable){
        HeaterEnable = false;
        digitalWrite(OutHeater, HeaterEnable);
        Serial.println("H= 0 Heater OFF");        
      } 

    }

  
  // COM
  if (stringComplete) {
    CMD(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  while (Serial.available()>0) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

boolean CMD (String inString){
  String inChar = inString.substring(0,1);
  inChar.toUpperCase();
  if (inChar == "S"){
    MotorSpeed = inString.substring(1).toFloat();
    Serial.println("S= " + (String)MotorSpeed + " rev/min");
    return true;
  }else if (inChar == "M"){
    MotorEnable = inString.substring(1).toInt();
    digitalWrite(OutEnable, !MotorEnable);
    Serial.print("M= " + (String)MotorEnable); if (MotorEnable) Serial.println(" Motor ON"); else Serial.println(" Motor OFF");
    return true;
  }else if (inChar == "H"){
    HeaterEnable = inString.substring(1).toInt();
    digitalWrite(OutHeater, HeaterEnable);
    Serial.print("H= " + (String)HeaterEnable); if (HeaterEnable) Serial.println(" Heater ON"); else Serial.println(" Heater OFF");
    return true;
  }else if (inChar == "F"){
    FanEnable = inString.substring(1).toInt();
    digitalWrite(OutFan, FanEnable);
    Serial.print("H= " + (String)FanEnable); if (FanEnable) Serial.println(" Fan ON"); else Serial.println(" Fan OFF");
    return true;
  }else if (inChar == "T"){
    HeaterTemp = inString.substring(1).toFloat();
    Serial.println("T = " + (String)HeaterTemp + " °C");
    return true;
  }else if (inChar == "?"){
    Serial.println("------------");
    Serial.print  ("M = " + (String)MotorEnable); if (MotorEnable) Serial.println(" Motor ON"); else Serial.println(" Motor OFF");
    Serial.println("S = " + (String)MotorSpeed + " rev/min");
    Serial.println("ST= " + (String)(MotorRev * MotorSpeed /60) + " step/s");
    Serial.print  ("H = " + (String)HeaterEnable); if (HeaterEnable) Serial.println(" Heater ON"); else Serial.println(" Heater OFF");
    Serial.print  ("F = " + (String)FanEnable); if (FanEnable) Serial.println(" Fan ON"); else Serial.println(" Fan OFF");
    Serial.println("T = " + (String)HeaterTemp + " °C");
    Serial.println("TA= " + (String)HeaterTempActual + " °C");
    Serial.println("------------");
    return true;
  }else {
    Serial.print("Error processing :");
    Serial.print(inString);
    return false;
  }
}
