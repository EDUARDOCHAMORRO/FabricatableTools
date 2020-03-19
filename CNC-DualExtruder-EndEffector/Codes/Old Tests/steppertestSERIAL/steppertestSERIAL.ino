const int dirPin = 8;
const int stepPin = 9;
 
const int steps = 200;
const int stepDelay = 800;
 
void setup() {
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
   // Marcar los pines como salida
   pinMode(dirPin, OUTPUT);
   pinMode(stepPin, OUTPUT);
}
String inBuffer = "";
int go = 0 ;
void loop() {

while (Serial.available()>0){
      char dato = Serial.read();
      inBuffer += dato;
      Serial.println(inBuffer);


if (inBuffer == "U")
    {    //Activar una direccion y fijar la velocidad con stepDelay
         digitalWrite(dirPin, HIGH);
        
         // Giramos 200 pulsos para hacer una vuelta completa      AT 600 STEPDELAY  / 50000 TO HALF PIPE
         while(go == 0){
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(stepDelay);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(stepDelay);
            while (Serial.available() > 0){
              Serial.println(Serial.read());
              if(Serial.read()){
                go = 1;
                char dato = Serial.read();
                inBuffer += dato;
                //Serial.println(inBuffer);
              }
            }
            
          }
         //Serial.println("moving up");
         inBuffer = "";
         
     }
  
     
     else if (inBuffer == "D")
    {
     
             //Activar una direccion y fijar la velocidad con stepDelay
         digitalWrite(dirPin, LOW);
         
         // Giramos 200 pulsos para hacer una vuelta completa      AT 600 STEPDELAY  / 50000 TO HALF PIPE
         while(go == 0){ 
          {
                      digitalWrite(stepPin, HIGH);
                      delayMicroseconds(stepDelay);
                      digitalWrite(stepPin, LOW);
                      delayMicroseconds(stepDelay);
                       while (Serial.available() > 0)
                       {
                        Serial.println(Serial.read());
                            if(Serial.read())
                            {
                           
                              go = 1;
                              char dato = Serial.read();
                              inBuffer += dato;
                             
                              //Serial.println(inBuffer);
                            }
                       }
          }
         }
         
         //Serial.println("moving DOWN");
         inBuffer = "";
         
         
     }

      go = 0;
      inBuffer = "";
     /* else if (inBuffer == "S")
    {
             //Activar una direccion y fijar la velocidad con stepDelay
         digitalWrite(dirPin, LOW);
         stepDelay = 800;
         // Giramos 200 pulsos para hacer una vuelta completa      AT 600 STEPDELAY  / 50000 TO HALF PIPE
         for (int x = 0; x < steps ; x++) {
            digitalWrite(stepPin, LOW);
            delayMicroseconds(stepDelay);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(stepDelay);
          }
         // Serial.println("STOPPED");
         inBuffer = "";
          
         
     }
     */
}
}
