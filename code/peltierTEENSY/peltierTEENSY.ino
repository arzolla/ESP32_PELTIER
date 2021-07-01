
#include <DHT.h>
#include <LiquidCrystal.h>

/////////PINOUTS/////////////
#define dht_read 7       //pino de dados do DHT
#define pelt_en 11       //pino do PowerMOSFET que chaveia o peltier
#define fan_tch 6     //RPM do fan

#define rs 17
#define en 12
#define d4 13
#define d5 14
#define d6 15
#define d7 16

#define botao_A 21
#define botao_B 20
#define botao_C 19
#define botao_D 18


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

DHT dht(dht_read, DHT22);

float set_temp = 20;
float temp = 25;
float humidity = 80;
float delta_t = 2;
char pelt_status = 0;
char flag_mustbe = 0;

void setup() {

  //////////////SERIAL SETUP/////////
  Serial.begin(9600);
  Serial.println("Aguardando dados...");
  
  // Iniclaiza o sensor DHT
  dht.begin();

  //LCD
  lcd.begin(20, 4);
  
  //PINMODES
  pinMode(dht_read, INPUT_PULLUP);
  pinMode(fan_tch, INPUT_PULLUP);
  pinMode(pelt_en, OUTPUT);
  pinMode(botao_A,INPUT_PULLUP);
  pinMode(botao_B,INPUT_PULLUP);
  pinMode(botao_C,INPUT_PULLUP);
  pinMode(botao_D,INPUT_PULLUP);
  digitalWrite(pelt_en,LOW);
  
}


///////////////////////PROGRAM//////////////
void loop() {

 //simulaçao de variaçao de temp
 /*
  if(pelt_status){
    temp = temp - 0.81;
  }
  else{
    temp = temp + 0.78;
  }
*/

////////////COMANDOS DE TEMPERATURA///////

/*
for(unsigned long loop_start = millis();2000 > millis() - loop_start; ){
  while(digitalRead(botao_A) == HIGH){
    Serial.println("A pressionado");
    set_temp++;
    while(digitalRead(botao_A) and CheckFan( ));
    SecurityCheck();
    while(digitalRead(botao_A));

  }
  while(digitalRead(botao_B) == HIGH){
    Serial.println("B pressionado");
    set_temp--;
    while(digitalRead(botao_B) and CheckFan());
    SecurityCheck();
    while(digitalRead(botao_B));

  }
  SecurityCheck();

}
*/

//temperaturas de chaveamento
  float on_temp = set_temp + delta_t;
  float off_temp = set_temp - delta_t;


/////////////TEMPERATURE AND HUMIDITY SENSOR////////////

  // Leitura da umidade
float humidity = dht.readHumidity();
  // Leitura da temperatura (Celsius)
 // float temp = dht.readTemperature();
    
 //erro caso nao consiga ler
  if (isnan(humidity) || isnan(temp))
  {
    Serial.println("Falha ao ler dados do sensor DHT !!!");
    lcd.setCursor(2,0);
    lcd.print("Falha ao ler dados do sensor DHT !!!");
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

  //Mostrar no LCD
  lcd.setCursor(0,1);
  lcd.print("Umidade:    ");
  lcd.print(humidity,1);
  lcd.print(" %");
  
  lcd.setCursor(0,2);
  lcd.print("Temp atual: ");
  lcd.print(temp,1);
  lcd.print(" oC");
  
  lcd.setCursor(0,3);
  lcd.print("Temp sel:   ");
  lcd.print(set_temp,1);
  lcd.print(" oC"); 
  


////////PROGRAMA DE CHAVEAMENTO DO PELTIER////

	//liga o peltier
  if(pelt_status == 0 and temp > on_temp){
    flag_mustbe = 1;
     
    digitalWrite(pelt_en, HIGH);
    pelt_status = 1;

    SecurityCheck();
    
    Serial.println("Peltier Ligado");
    lcd.setCursor(0,0);
    lcd.print(" *PELTIER LIGADO*   ");
    
    Serial.print(temp);
    Serial.print(" > ");
    Serial.println(on_temp);


  }
  
  ////desligar o peltier
  if(pelt_status == 1 and temp < off_temp){
    flag_mustbe = 0;
    digitalWrite(pelt_en,LOW);
    pelt_status = 0;
    Serial.println("Peltier Desligado");

    lcd.setCursor(0,0);
    lcd.print(" *PELTIER DESLIGADO*");
    Serial.print(temp);
    Serial.print(" < ");
    Serial.println(off_temp);  
  }
  SecurityCheck();
  delay(2000);  ////TIRAR ESSE DELAY



}

/////FUNÇAO QUE CHECA SE FAN ESTA LIGADO///

char CheckFan(){
  unsigned long periodON = pulseIn(fan_tch, HIGH, 100000);
  unsigned long periodOFF = pulseIn(fan_tch, LOW, 100000);
  unsigned long state = periodON*periodOFF;

  if(state){
    return 1; //fan ligado
  }
  else{
    return 0; //fan desligado
  }
}

////////FUNCAO CHECAGEM DE SEGURANÇA////
 
void SecurityCheck(){
    char fanstate = CheckFan(); 
    
    while(flag_mustbe==1 and fanstate == 0){
      digitalWrite(pelt_en, LOW);
      pelt_status = 0;
      //serial print
      Serial.println("!!!!!!FAN TRAVADO!!!!!!!!");
      Serial.println("Peltier desligado por seguranca!!");
      Serial.println();
      
      //lcd print
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("!!!!!!!!!!!!!!!!!!!");
      lcd.setCursor(0,1);
      lcd.print("FAN TRAVADO");
      lcd.setCursor(0,2);
      lcd.print("Peltier desligado");
      lcd.setCursor(0,3);
      lcd.print("por seguranca");
      
      
      delay(1500);
      delay(1500);
      fanstate = CheckFan(); 
    }
       
}
