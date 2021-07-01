
#include <FreqCount.h>

void setup() {
//  FreqCount.begin(1000);  //1 sec gate
//  FreqCount.begin(6000);  //6 sec gate
//  FreqCount.begin(100);    //100mili gate
  Serial.begin(9600);

}

void loop() {
   FreqCount.begin(6000); 
  while(1){
  if (FreqCount.available()) {
 /*
  //for 1 sec gate
    unsigned long twohertz = FreqCount.read();
    unsigned long hertz,rpm;
    hertz = twohertz/2;
    rpm = hertz*60;
    Serial.println("Hertz is: ");
    Serial.println(hertz);
    
    Serial.println("RPM is: ");
    Serial.println(rpm);
*/
    //for 6 sec gate
    unsigned long tworp6sec = FreqCount.read();
    unsigned long rp6sec, rpm;
    rp6sec = tworp6sec/2;
    rpm = rp6sec*10;
    Serial.println("Rotations per 6 seconds are: ");
    Serial.println(rp6sec);
    
    Serial.println("RPM are: ");
    Serial.println(rpm);
 
 /*
   //for 100 mili gate
    delay(1000);
    unsigned long twodecihertz = FreqCount.read();
    unsigned long decihertz,rpm;
    decihertz = twodecihertz/2;
    rpm = decihertz*600;
    Serial.println("Hertz is: ");
    Serial.println(decihertz*10);
    
    Serial.println("RPM is: ");
    Serial.println(rpm);
 
  */
  }
  }


}
