#include <FreqCount.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
unsigned long rpm = RPMCount(1000);
 Serial.println("rpm is: ");
 Serial.println(rpm);

}

//Retorna os RPMs de um fan convencional lidos pelo pino 11, em relaçao ao gate escolhido 
unsigned long RPMCount(float gate){
  FreqCount.begin(gate);
  
  while(!FreqCount.available());
  
  unsigned long pulse = FreqCount.read();
  Serial.println(pulse);
  float ratio = (60000/gate)/2;
  Serial.println(ratio);
  float freqRPM = pulse*ratio;
  FreqCount.end();
  return freqRPM;

    
  }
  
