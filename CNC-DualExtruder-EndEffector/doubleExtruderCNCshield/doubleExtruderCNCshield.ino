int enablePin = 7;
int dirPinM1 = 8; // X axis

int stepPinM1 = 9;

int POT = A5;

int oldstepDelay = 0;
int stepDelay = 0;
bool start = true;
bool dir = false;
bool enableMot = 1;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(POT,INPUT);
  pinMode(enablePin,OUTPUT);
  pinMode(dirPinM1,OUTPUT);
  pinMode(stepPinM1,OUTPUT);
  start = 0;
  Serial.println("Starting");
}


int index = 0;
char c;
int gotSpeed = 0;
int mspeed = 10000;


void loop() {
 






  if(start == 1){
    digitalWrite(enablePin,LOW);  
    digitalWrite(dirPinM1, dir);
    }
  if(start == 0){
    digitalWrite(enablePin,HIGH);  
    }
  int maxdelay = 700;
  int potPos = analogRead(POT);
  //stepdelayMicroseconds = map(analogRead(POT),0,1024,1,maxdelayMicroseconds);
  // make the motors move both irections depending on pots position
  if(potPos < 500){
    start = 1;
    digitalWrite(dirPinM1,HIGH);  
    stepDelay = map(potPos,0,500,1,maxdelay);
  }
  else if(potPos > 600){
    start = 1;
    digitalWrite(dirPinM1,LOW);
    stepDelay = map(potPos,600, 1023, maxdelay,1);
  }
  else{
    start = 0;
  }
  
// Serial.println(potPos);

}

void motorStep(){
    //if(start == 1){
        digitalWrite(stepPinM1,HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(stepPinM1,LOW);
        delayMicroseconds(stepDelay);
     // }
  }
