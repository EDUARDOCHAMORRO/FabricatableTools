#include <AccelStepper.h>
 
AccelStepper stepper(1, 9, 8);//(define el tipo de control del motor,pin de paso,pin direccion)
 
int pos = 50000;
 
void setup()
{
  stepper.setMaxSpeed(2000); //velocidad maxima permitida 800
  stepper.setAcceleration(1000);// relacion entre aceleracion/desaceleracion 500
}
 
void loop()
{
  if (stepper.distanceToGo() == 0)//distancia de la posicion actual y la destino
 
  {
    delay(500);
    pos = -pos;
    stepper.moveTo(pos);//establece la posicion de destino
  }
  stepper.run();
}
