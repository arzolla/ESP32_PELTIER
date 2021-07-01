
#include <DHT.h>

/////////PINOUTS/////////////
#define dht_read D1       //pino de dados do DHT
#define pelt_en D0       //pino do PowerMOSFET que chaveia o peltier
#define fan_tch D3     //RPM do fan

#define botao_A D6
#define botao_B D7
#define botao_C D5
#define botao_D D8

DHT dht(dht_read, DHT22);

float set_temp = 20;
float temp = 25;
float humidity = 80;
float delta_t = 2;
char pelt_status = 0;
char flag_mustbe = 0;

void setup() {
yield();
ESP.wdtFeed();
  //////////////SERIAL SETUP/////////
  Serial.begin(74880);
  Serial.println("Aguardando dados...");
  // Iniclaiza o sensor DHT
  dht.begin();
  
  pinMode(dht_read, INPUT_PULLUP);
  pinMode(fan_tch, INPUT_PULLUP);
  pinMode(pelt_en, OUTPUT);
  pinMode(botao_A,INPUT_PULLUP);
  pinMode(botao_B,INPUT_PULLUP);
  pinMode(botao_C,INPUT_PULLUP);
  pinMode(botao_D,INPUT_PULLUP);
  digitalWrite(pelt_en, HIGH);
  
}


///////////////////////PROGRAM//////////////
void loop() {
/*
 //simulaçao de variaçao de temp
  if(pelt_status){
    temp = temp - 0.81;
  }
  else{
    temp = temp + 0.78;
  }
*/

////////////COMANDOS DE TEMPERATURA///////


for(unsigned long loop_start = millis();2000 > millis() - loop_start; ){
  while(digitalRead(botao_A) == LOW){
    Serial.println("A pressionado");
    set_temp++;
    while(digitalRead(botao_A) and CheckFan( ));
    SecurityCheck();
    while(digitalRead(botao_A));
    yield();
    ESP.wdtFeed();
  }
  while(digitalRead(botao_B) == LOW){
    Serial.println("B pressionado");
    set_temp--;
    while(digitalRead(botao_B) and CheckFan());
    SecurityCheck();
    while(digitalRead(botao_B));
    yield();
    ESP.wdtFeed();
  }
  SecurityCheck();
  yield();
  ESP.wdtFeed();
}

//temperaturas de chaveamento
  float on_temp = set_temp + delta_t;
  float off_temp = set_temp - delta_t;


/////////////TEMPERATURE AND HUMIDITY SENSOR////////////

  // Leitura da umidade
  float humidity = dht.readHumidity();
  // Leitura da temperatura (Celsius)
  float temp = dht.readTemperature();
    
 //erro caso nao consiga ler
  if (isnan(humidity) || isnan(temp))
  {
    Serial.println("Falha ao ler dados do sensor DHT !!!");
    delay(500);
    return;
  }
  
  // Mostra a umidade no serial monitor e no display
  Serial.print("Umidade : ");
  Serial.print(humidity);
  Serial.println(" %");
  //Serial.println();

  // Mostra a temperatura atual no serial monitor
  Serial.print("Temperatura atual: ");
  Serial.print(temp);
  Serial.println(" oC  ");
 
   // Mostra a temperatura selecionada no serial monitor
  Serial.print("Temperatura selecionada: ");
  Serial.print(set_temp);
  Serial.println(" oC  ");
  Serial.println();


////////PROGRAMA DE CHAVEAMENTO DO PELTIER////

	//liga o peltier
  if(pelt_status == 0 and temp > on_temp){
    flag_mustbe = 1;
     
    digitalWrite(pelt_en, LOW); //LOW = led ligado
    pelt_status = 1;

    SecurityCheck();
    
    Serial.println("Peltier Ligado");
    
    Serial.print(temp);
    Serial.print(" > ");
    Serial.println(on_temp);
    yield();
    ESP.wdtFeed();

  }
  
  ////desligar o peltier
  if(pelt_status == 1 and temp < off_temp){
    flag_mustbe = 0;
    digitalWrite(pelt_en, HIGH); //HIGH = led desligado
    pelt_status = 0;
    Serial.println("Peltier Desligado");
    
    Serial.print(temp);
    Serial.print(" < ");
    Serial.println(off_temp);
    yield();
    ESP.wdtFeed();  
  }

  



}

/////FUNÇAO QUE CHECA SE FAN ESTA LIGADO///
char CheckFan(){
  unsigned long periodON = pulseIn(fan_tch, HIGH, 50000);
  unsigned long periodOFF = pulseIn(fan_tch, LOW, 50000);
  unsigned long state = periodON*periodOFF;

  if(state){
    return 1; //fan ligado
  }
  else{
    return 0; //fan desligado
  }
}

////////CHECAGEM DE SEGURANÇA////
 
void SecurityCheck(){
    char fanstate = CheckFan(); 
    
    while(flag_mustbe==1 and fanstate == 0){
      digitalWrite(pelt_en, HIGH); // HIGH = led desligado
      pelt_status = 0;
      Serial.println("!!!!!!FAN TRAVADO!!!!!!!!");
      Serial.println("Peltier desligado por seguranca!!");
      Serial.println();
      delay(1500);
      yield();
      ESP.wdtFeed();
      delay(1500);
      fanstate = CheckFan(); 
    }
       
}
