/* 

@Dev_Murilo - email: contato@devmurilo.com.br

*/
#include <stdio.h>
//unsigned long int time;  //variavel

//tring to implment softstart

//### variaveis para leitura do ADC de cada porta de entrada
int Tensao_carregador = 0;       // variavel tensão carregadpr
int Tensao_bateria = 0;          // variavel tensão bateria interna
int Tensao_bateria_externa = 0;  // variavel tensão bateria externa

//#### portas de saida destinadas a controle dos mosfets
int carregador = 5;       // porta de controle de mosfet carrergador
int bateria = 6;          // porta de controle de mosfet bateria
int bateria_externa = 7;  // porta de controle de mosfet bateria externa
int rele_bat = 8;              // porta de controle de rele
int rele_bat2 = 3;              // porta de controle de rele

//### variaveis para salvar estados do sistema
bool bateria_carregada = true;
bool bateria_externa_carregada = false;
bool carregador_conectado = false;
bool bateria_externa_conectada = false;
bool bateria_conectada = true;

// 0% = 530
// 100% = 675

//## variaveis para contador comunicação serial
unsigned long previousMillis = 0; 
unsigned long previousMillis2= 0; 
const long interval = 1000;       
const long interval2 = 10000;
unsigned long currentMillis = millis();

// softstart
bool softstartComplete = false; // Flag de softstart completo
const int pwmPin = bateria; // Pino PWM para controle do softstart
const int softstartTime = 5000; // Tempo de duração do softstart em milissegundos

void setup() {
  Serial1.begin(115200);  
  Serial.begin(115200); 
  analogReadResolution(10); 
  
  pinMode(carregador, OUTPUT);       // saida mosfet carregador + led azul
  pinMode(bateria, OUTPUT);          // saida Mosfet bateria 1 + led verde
  pinMode(bateria_externa, OUTPUT);  // saida mosfet bateria 2 + led amarelo
  
  
  pinMode(rele_bat, OUTPUT);            // carregar bateria interna
  pinMode(rele_bat2, OUTPUT);            // carregar bateria externa

  //#### fazendo varredura inicial bateria interna
  
  Tensao_bateria = analogRead(2);
  //Serial.println("tensão da bateria: ");
  //Serial.print(Tensao_bateria);
  //Serial.println("");

  //if(Tensao_bateria<=530){
  //  bateria_carregada=false;
  //}else digitalWrite(bateria,HIGH)  
  if(Tensao_bateria<=530){
    bateria_carregada=false;
  }else softstart();

  //#### fazendo varredura inicial bateria externa
  Tensao_bateria_externa = analogRead(4);
  //Serial.println("tensão da bateria externa: ");
  //Serial.print(Tensao_bateria_externa);
  //Serial.println("");
  if (Tensao_bateria_externa <= 200) {
    bateria_externa_carregada = false;
    bateria_externa_conectada = false;
  } else {
    bateria_externa_conectada = true;
  }
  if (Tensao_bateria_externa >= 540) {
    bateria_externa_carregada = true;
  }
  
  //time = millis();
  //Serial.println("Tempo decorrido para setup");
  //Serial.println(time);
}


// Soft start function
void softstart(){
  while (pwmValue < 1023) {
        analogWrite(pwmPin, pwmValue);
        pwmValue += increment;
        delay(softstartTime / (1023 / increment));
    }
    softstartComplete = true;
    return;
}

void loop() {
    Tensao_carregador = analogRead(1);
    Tensao_bateria = analogRead(2);
    Tensao_bateria_externa = analogRead(4);  
    {  // VERIFICA BATERIA EXTERNA
      if(Tensao_bateria_externa<=540){
        bateria_externa_carregada=false;
        bateria_externa_conectada=false;
      }
      if(Tensao_bateria_externa>=580){
          bateria_externa_carregada=true;
          bateria_externa_conectada=true;
      }

      // bateria carregada e conectada
      if(bateria_externa_carregada==true && bateria_externa_conectada==true){
      //Serial.println("CONDIÇÃO E");

      digitalWrite(bateria_externa,HIGH);
      delay(1);
      digitalWrite(bateria,LOW);
      bateria_conectada=false;
      }

      // bateria ausente externa desconectada ou descarregada e a interna carregada
      if(bateria_externa_carregada==false || bateria_externa_conectada==false && bateria_carregada==true){
      //Serial.println("CONDIÇÃO OU");
      digitalWrite(bateria,HIGH);
      delay(1);
      digitalWrite(bateria_externa,LOW);
      }
    }
    { // verifica carregador
      if(currentMillis - previousMillis2 >= interval2) {
        previousMillis2 = currentMillis; // salva o tempo atual

        digitalWrite(rele_bat2,LOW);
        digitalWrite(rele_bat,LOW);
        digitalWrite(bateria,HIGH);
        delay(1);
        digitalWrite(bateria_externa,LOW);
        delay(1);
        Tensao_carregador=analogRead(A0);
        Tensao_bateria_externa=analogRead(A2);
        if(Tensao_carregador>=400){
          carregador_conectado=true;
        } else carregador_conectado = false;
        
        if(carregador_conectado==true){
          if(Tensao_bateria_externa>=100){
            if(Tensao_bateria>=675){
              digitalWrite(rele_bat2,HIGH);
              digitalWrite(rele_bat,LOW);
            }
            else {
            digitalWrite(rele_bat2,LOW);
            digitalWrite(rele_bat,HIGH);
            }  
          } else {
            digitalWrite(rele_bat2,LOW);
            digitalWrite(rele_bat,HIGH);
          
          }
          if(Tensao_bateria>=675 && Tensao_bateria_externa>= 675){
            digitalWrite(rele_bat2,HIGH);
            digitalWrite(rele_bat,HIGH);
            Serial.println("CARREGANDO AS DUAS");
          }
        } else {
          digitalWrite(rele_bat2,LOW);
          digitalWrite(rele_bat,LOW);
        }
        
      }
    }
      
  // envia informações  
  if (currentMillis - previousMillis >= interval) {
    // bateria externa conectada 0 ou 1 , tensão externa, tensão bateria interna, 
    String Concatenada = String(bateria_externa_conectada)+","+String(Tensao_bateria_externa)+","+String(Tensao_bateria)+","+String(carregador_conectado)+","+String(Tensao_carregador);
    previousMillis = currentMillis; // salva o tempo atual
    Serial1.println(Concatenada); 
    Serial.println(Concatenada);  
  }  
  //String Concatenada = String(bateria_externa_conectada)+","+String(Tensao_bateria_externa)+","+String(Tensao_bateria)+","+String(Tensao_carregador);
  //Serial.println(Concatenada);  

  currentMillis = millis();
}
