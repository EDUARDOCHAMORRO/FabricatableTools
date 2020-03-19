int enablePin = 5;
int dirPinM1 = 6; 

int Pulse = 7;

int POT = A5;

//int oldstepDelay = 0;
int stepDelay = 0;
bool start = true;
bool dir = false;
bool enableMot = 1;
int maxdelay = 3000;
int mindelay = 250;


void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);  if uncommented the motors runs slower due to serial wait timings
  pinMode(POT,INPUT);
  pinMode(enablePin,OUTPUT);
  pinMode(dirPinM1,OUTPUT);
  pinMode(Pulse,OUTPUT);
  start = 0;
 // Serial.println("Starting");
}




void loop() {

        
        
        
        if(start == 1){
          digitalWrite(enablePin,LOW);  
          digitalWrite(dirPinM1, dir);
          }
        if(start == 0){
          digitalWrite(enablePin,HIGH);  
          }
      
      
          
        
        
        int potPos = analogRead(POT);
       // Serial.println(potPos);
        
        //stepdelayMicroseconds = map(analogRead(POT),0,1024,1,maxdelayMicroseconds);
        // make the motors move both irections depending on pots position
        if(potPos < 500){
          start = 1;
          digitalWrite(dirPinM1,HIGH);  
          stepDelay = map(potPos,0,500,mindelay,maxdelay);
          //Serial.println("clockwise");
          //Serial.println(stepDelay);
        }
        else if(potPos > 600){
          start = 1;
          digitalWrite(dirPinM1,LOW);
          stepDelay = map(potPos,600, 1023, maxdelay,mindelay);
          //Serial.println("counterclockwise");
          //Serial.println(stepDelay);
        }
        else{
          start = 0;
        }
        
        motorStep();
}

void motorStep(){

  
    //if(start == 1){
        digitalWrite(Pulse,HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(Pulse,LOW);
        
     // }
  }
