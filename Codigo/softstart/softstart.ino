#include <Arduino.h>

bool softstartComplete = false; // Flag de softstart completo
const int pwmPin = 4; // Pino PWM para controle do softstart
const int softstartTime = 5000; // Tempo de duração do softstart em milissegundos

void softstart() {
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
}

void loop() {
    // Chama a função de softstart
    if(softstartComplete!=true) {
        softstart();
    }
    analogWrite(pwmPin, 2);
    // Outras ações do programa
}