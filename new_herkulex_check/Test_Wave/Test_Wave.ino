#include <Herkulex.h>

// Basic Wave (just right wrist)

int counter2 = 0;

//Checks which motors are attached and reading real values
void setup()  
{
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Serial.println("Begin"); 
  Herkulex.beginSerial2(115200); //open serial port 2 

  for(int i =0;i<0xFE;i++)
    Herkulex.reboot(i); //reboot first motor

  delay(500); 
  Herkulex.initialize(); //initialize motors
 
  Herkulex.torqueON(25);
}

void loop(){
  int counter = 0;
  counter2++;

  // Counts servos
  for(int i =0;i<0xFD;i++){
    int angle = Herkulex.getAngle(i, false);
    if(angle != -166 && angle != -83){
      Serial.print("Got servo ");
      Serial.print(i);
      Serial.print(" Angle: ");
      Serial.println(angle);
      Herkulex.setLed(i,LED_BLUE);
      if(i != 25)
        Herkulex.torqueOFF(i);
      counter++;
    }
  }
  Serial.println("Found " + String(counter) + " Motors");

  // Waves
  if(counter2 % 2 == 0)
  {
    Serial.println("here");
    Herkulex.moveOneAngle(25, 20, 1000, LED_BLUE, false);
  }
  else
  {
    Herkulex.moveOneAngle(25, -20, 1000, LED_BLUE, false);
  }

  delay(1000);
}