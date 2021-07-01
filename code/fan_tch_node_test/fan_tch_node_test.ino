
#define fan_tch 6
#define source_on 11



void setup() {
//  yield();
 // ESP.wdtFeed();
  //////////////SERIAL SETUP/////////
//  Serial.begin(74880);
  Serial.begin(9600);
  pinMode(fan_tch, INPUT_PULLUP);
  pinMode(source_on, OUTPUT);
}

void loop() {
    
    unsigned long periodON = pulseIn(fan_tch, HIGH, 100000);
    unsigned long periodOFF = pulseIn(fan_tch, LOW, 100000);
    Serial.println(periodON);
    Serial.println(periodOFF);
   // yield();
   // ESP.wdtFeed();
   
    delay(1500);
    digitalWrite(source_on, HIGH);
    delay(2000);

 
}
