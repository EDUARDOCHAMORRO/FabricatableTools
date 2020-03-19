int enablePin = 8;
int dirPinM1 = 6; 
int Pulse = 7;
int start;
int internalled = 13;


void setup() {
  pinMode(internalled,OUTPUT);
  pinMode(enablePin,OUTPUT);
  pinMode(dirPinM1,OUTPUT);
  pinMode(Pulse,OUTPUT);
  
  start = 0;

}




void loop() {
        
        digitalWrite(enablePin,LOW);
        digitalWrite(internalled,HIGH);
        digitalWrite(dirPinM1,HIGH);
        digitalWrite(Pulse,HIGH);
        delayMicroseconds(200);
        digitalWrite(Pulse,LOW);
        delayMicroseconds(200);

/*
         digitalWrite(internalled,LOW);
        digitalWrite(dirPinM1,LOW);
        digitalWrite(Pulse,HIGH);
        delayMicroseconds(2000);
        digitalWrite(Pulse,LOW);
        delayMicroseconds(200);
   */
        
      
  }
