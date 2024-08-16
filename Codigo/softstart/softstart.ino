#include <Arduino.h>

bool softstartComplete = false; // Flag de softstart completo
const int pwmPin = 4; // Pino PWM para controle do softstart
const int softstartTime = 10000; // Tempo de duração do softstart em milissegundos

void softstart() {
    Serial.println("Softstart");
    int pwmValue = 0;
    int increment = 1;

    // Aumenta gradualmente o valor do PWM até atingir o máximo
    while (pwmValue < 1023) {
        analogWrite(pwmPin, pwmValue);
        pwmValue += increment;
        delay(softstartTime / (1023 / increment));
    }

    /*
    // Mantém o valor máximo do PWM por um tempo
    delay(softstartTime);

    
    // Diminui gradualmente o valor do PWM até desligar completamente
    while (pwmValue > 0) {
        analogWrite(pwmPin, pwmValue);
        pwmValue -= increment;
        delay(softstartTime / (1023 / increment));
    }

    // Desliga o PWM
    analogWrite(pwmPin, 0);
  */  
    softstartComplete = true;
    return;
}

void setup() {
    pinMode(pwmPin, OUTPUT);
    Serial.begin(115200);
    Serial.println("saindo Setup");
}

void loop() {
    Serial.println("entrando Loop");
    // Chama a função de softstart
    if(softstartComplete!=true) {
        //softstart();
    }
    analogWrite(pwmPin,1);
    delay(1000);
     
}