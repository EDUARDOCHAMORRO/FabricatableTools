//CODE TO CONTROL PELLET WASP EXTRUDER
//Custom control box and rewind developped at IaaC-FabLabBarcelona 
//Beans Reels Design  CC BY-NC-SA
//(c)27-10-2020  Eduardo Chamorro "eduardochamorromartin@gmail.com"

//===========================================================================
//============================= ROBOT CONTROL===========================
//===========================================================================
int octocoupler1 = 57;       // ROBOT OUTPUT NUMBER 24CONNECTED TO I0.5 pin 57 on pellet extruder logic controller
int robot1Activate = 300;    // calibration to turn the motor based on analog input signal
int robot1 =0;               // variable of robotmotor control
int resetretraction = 1;     //variable to reset the retraction just once on robot io control
//===========================================================================
//============================= PID / FANS / T SENSOR ===========================
//===========================================================================
int PIDcontrol = 37;// cooling fans connected to relay R.08 pin 37
int coolingfans = 24;// cooling fans connected to relay R.04 pin 24
int TempAlarm = 54;// TEMPERATURE ALARM PIN FROM PID pin connected to I0.2 pin 54 

//===========================================================================
//============================= MOTOR DEFINITIONS ===========================
//===========================================================================
int stepperEN = 6;// stepper driver EN pin connected to rQ0.2 pin 6  (PWM PIN)
int stepperDIR = 5;// stepper driver DIR pinconnected to rQ0.1 pin 5 (PWM PIN)
int stepperPUL = 4;// stepper driver PUL pin connected to Q0.0 pin 4 (PWM PIN)
  
  int stepDelay =5000;
  //stepDelayE0 = 20;
  int retractionSpeed = 1000;
  int retractionAmount = 200;
  //int reextrusionAmount = 560;
//===========================================================================
//============================= VARIABLES ===========================
//===========================================================================
int serialInput = 2000;     //By defect speed if no serial input is used
String inBuffer = "";
int MINTEMPREACHED=0; //Value declaration to not allow extruding below than 180º degrees

//===========================================================================
//============================= RUNNING CODE ================================
//===========================================================================
void setup() {
  Serial.begin(115200);  // initialize SERIAL PORT TO CONTROL OVER SERIAL MONITOR.
  while (!Serial);
  Serial.println("Initialization - wait");
  // initialize digital pin as an output.
  pinMode(PIDcontrol, OUTPUT);  // initialize digital pin as an output.
  pinMode(coolingfans, OUTPUT);  // initialize digital pin as an output.
  pinMode(stepperEN, OUTPUT);  // initialize digital pin as an output.
  pinMode(stepperDIR, OUTPUT);  // initialize digital pin as an output.
  pinMode(stepperPUL, OUTPUT);  // initialize digital pin as an output.

          digitalWrite(stepperEN, HIGH);   // DISABLE MOTOR
          digitalWrite(stepperDIR, HIGH);   // rotate stepper motor CCW
          Serial.print("Motor-OFF /");
          Serial.print("Speed- ");
          Serial.print(stepDelay);
          Serial.print(" /");
          
          digitalWrite(PIDcontrol, HIGH);   // turn the PID CONTROL-TEMPERATURE ON
          Serial.print("PIDTº-ON /");
          
          digitalWrite(coolingfans, LOW);   // turn the cooling fans ON
          Serial.println("CoolingFans-OFF /");

       
  
  
  
  Serial.println("Initialization finish");
  Serial.println("SAFETY ON - NO EXTRUSION ALLOWED BELLOW 180ºDegrees");

  Serial.println("Command input online, write command to perform action");
  Serial.println("OFF-ON --->Motor 0-1 / Cooling Fans 2-3 / PID 4-5 / SPEED 500-5000");
  Serial.println("-------------------------");


}

void loop() {                              // the loop function runs over and over again forever
  MINTEMPREACHED = digitalRead(TempAlarm); // read the PID ALARM TO CHECK IF THE MIN PRINT TEMPERATURE HAS BEEN REACHED (180DEGREES)
  robot1 = analogRead(octocoupler1);        // read the robot octocoupler
  motorRetraction();
  //Serial.println(robot1);                 // print robot output
        if(MINTEMPREACHED==1){
          //Serial.print("MOTOR CAN ROTATE - ABOVE 180º");
        }                     
             serialcommands();             //call void to update values based on serial commands inputs       
             motorStep();                  //call the motor rotation void
}



//===========================================================================
//============================= LOOP FINISH - VOIDS START ===========================
//===========================================================================

void serialcommands()
{
    if  (Serial.available() > 0)
       {
             // ignore line feed and carriage return
            if (Serial.peek() == '\n' || Serial.peek() == '\r')
            {
               Serial.read();  // read and discard LF & CR
            }
            else
            {
                 serialInput = Serial.parseInt();
                 //serialInput = Serial.read();
                        if (serialInput == 0)
                            {
                              
                              Serial.print("MOTOR - OFF - NO EXTRUSION - ");
                              Serial.print("Type '1' to extrude /");
                              Serial.println("litte retraction happening");
                              digitalWrite(stepperDIR, LOW);// rotate stepper motor CW!!RETRACTION
                                  if(MINTEMPREACHED == 1)
                                  {
                                  for (int i = 0; i <= retractionAmount ; i++) {
                                               digitalWrite(stepperPUL,HIGH);
                                               delayMicroseconds(retractionSpeed);
                                               digitalWrite(stepperPUL,LOW); 
                                               delayMicroseconds(retractionSpeed);
                                                 
                                              }
                                   }
                              digitalWrite(stepperEN, HIGH);   // DISABLE MOTOR                
                                            
                            }
                         if (serialInput == 1)
                            {
                              digitalWrite(stepperDIR, HIGH);   // rotate stepper motor CCW -Extrusion direction
                              digitalWrite(stepperEN, LOW);   // ENABLE MOTOR EXTRUDE
                              Serial.print("MOTOR - ON -EXTRUSION!!! - ");
                              Serial.println("Type '0' to STOP");
                            }
                          if (serialInput == 2)
                            {
                              digitalWrite(coolingfans, LOW);   // DISABLE COOLING FANS
                              Serial.print("COOLING FAN - OFF - ");
                              Serial.println("Type '3' to ENABLE");
                            } 
                          if (serialInput == 3)
                            {
                              digitalWrite(coolingfans, HIGH);   // ENABLE COOLING FANS
                              Serial.print("COOLING FAN - ON!!! - ");
                              Serial.println("Type '2' to DISABLE");
                            }
                           if (serialInput == 4)
                            {
                              digitalWrite(PIDcontrol, LOW);   // DISABLE PIDcontrol
                              Serial.print("PIDcontrol - OFF - ");
                              Serial.print("Type '5' to ENABLE");
                              Serial.println("NO EXTRUSION ALLOWED WITH PID OFF");

                            } 
                          if (serialInput == 5)
                            {
                              digitalWrite(PIDcontrol, HIGH);   // ENABLE PIDcontrol
                              Serial.print("PIDcontrol - ON!!! - ");
                              Serial.println("Type '4' to DISABLE");
                            }                
                                             
                        if (serialInput >= 6 && serialInput < 500)
                            {
                              Serial.print("NewMotorSpeed-");
                              Serial.println(serialInput);
                              Serial.println("cannot execute command, too low number");
                            }
                                        
                         if (serialInput >= 500 && serialInput < 5000)
                            {
                              
                              digitalWrite(stepperEN, LOW);   // ENABLE MOTOR
                              digitalWrite(stepperDIR, HIGH);   // rotate stepper motor CCW -Extrusion direction
                              Serial.print("MOTOR - ON /");
                              Serial.print("Previous Speed-");
                              Serial.print(stepDelay );
                              Serial.print(" /Updating Speed to-");
                              stepDelay = serialInput;
                              Serial.println(stepDelay);
                              configcommands();
                              
                                          
                             }
              }
         }
     
         
}
void configcommands(){
  Serial.println("Command input online, write command to perform action");
  Serial.println("OFF-ON --->Motor 0-1 / Cooling Fans 2-3 / PID 4-5 / SPEED 500-5000");
  Serial.println("-------------------------");
}

void motorStep(){
if(MINTEMPREACHED == 1 && robot1 >= robot1Activate)
  {   //Condition to not allow the motor rotation if a MIN of 180 degrees has not been reached
        digitalWrite(stepperPUL,HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(stepperPUL,LOW);
        delayMicroseconds(stepDelay);
        resetretraction = 0;
  }
 }
 
void motorRetraction(){
  if (MINTEMPREACHED == 1 && robot1 <= robot1Activate){
                                  if(MINTEMPREACHED == 1){
                                    for (int r = 0; r <= resetretraction ; r++) {
                                        for (int i = 0; i <= retractionAmount ; i++) {
                                              Serial.println("litte retraction happening");
                                               digitalWrite(stepperDIR, LOW);// rotate stepper motor CW!!RETRACTION
                                               digitalWrite(stepperEN, LOW);   // ENABLE MOTOR
                                               digitalWrite(stepperPUL,HIGH);
                                               delayMicroseconds(retractionSpeed);
                                               digitalWrite(stepperPUL,LOW); 
                                               delayMicroseconds(retractionSpeed);  
                                                                                     }
                                                                                    }
                                              }                
                            }
  }
