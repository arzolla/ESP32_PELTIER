
#define fan_tch 12

void setup() {
   pinMode(fan_tch, INPUT_PULLUP);

}

void loop() {


//unsigned long periodON = pulseIn(fan_tch, HIGH, timeout);
 //unsigned long periodOFF = pulseIn(fan_tch, LOW, timeout); 
 //Serial.println(periodON);
 //Serial.println(periodOFF);

 unsigned long totalperiod = PulsePeriod(fan_tch,50000);
 if(totalperiod){
  Serial.println("FAN ligado");
  }
 else{
  Serial.println("FAN desligado");
  }
  
  float freqHz = 1000000/(totalperiod); //periodo total, em microsec
  float freqRPM = (freqHz*60)/2;  //dois pulsos por volta, divide por 2
  Serial.print("Hz: ");
  Serial.println(freqHz);
  Serial.print("RPM: ");
  Serial.println(freqRPM);
  delay(4000);
  }

  //funçao que retorna periodo do pulso em um pino, para timeout escolhido
 unsigned long PulsePeriod(int pin,unsigned long timeout){  //timeout em microsec, quanto maior, melhor a resolução
  
 unsigned long periodON = pulseIn(pin, HIGH, timeout);
 unsigned long periodOFF = pulseIn(pin, LOW, timeout); 
 Serial.println(periodON);
 Serial.println(periodOFF);
  
 unsigned long totalperiod = (periodON + periodOFF); //periodo total, em microsec

 return totalperiod;
  
  }
  

  
