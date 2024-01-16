#include <Herkulex.h>

// Checks which motors are attached and reading real values
// Goes one motor at a time, waiting for serial input to move on to the next
void setup()  
{
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Serial.println("Begin");
  Herkulex.beginSerial2(115200); //open serial port 12

  for(int i = 0; i < 0xFE; i++) {
    Herkulex.reboot(i); //reboot first motor
    Herkulex.setLed(i,0);
  }

  delay(500); 
  Herkulex.initialize(); //initialize motors

  Herkulex.torqueOFF(0xFE);
}

void loop(){
  int count = 0;
 
  for(int i = 0; i < 0xFD; i++) {
    int angle = Herkulex.getAngle(i, false); // Assume 0201, does not matter for seeing if they are connected
    
    if(angle != -166 && angle != -83) {
      count++;
      Herkulex.torqueOFF(i);
      Serial.print("\nGot servo ");
      Serial.print(i);
      Serial.print(" Angle: ");
      Serial.print(angle);
      Herkulex.setLed(i,LED_GREEN);

      while (Serial.available() == 0) {} //wait for data available, any input into serial monitor
      Serial.readString(); // eat input so serial is unavailable for next ID

      Herkulex.setLed(i,0);
      // Herkulex.clearError(i);
      delay(10);
    }
  }

  Serial.print("\nAll IDs scanned, found ");
  Serial.print(count);
}