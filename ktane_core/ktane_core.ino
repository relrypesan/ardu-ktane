#include <Wire.h>
#include "CompactArduino.h"

#define PIN_SDA A4
#define PIN_SCL A5

#define MAX_ADDRESS 8

#define LED_WARNING 13

typedef struct MODULE {
  int address;
  struct MODULE *nextModule;
} Module;

Module *modules, *currentMod;

// ----------- Body ------------
void setup() {
  int numbersModules = 0;
  modules = currentMod = NULL;

  setPin(LED_WARNING, OUTPUT);

  Wire.begin();
  Serial.begin(9600);
  Serial.println("Iniciando programa...");

  // delay para garantir que os modulos já estaram iniciados e endereçados
  delay(1000);

  for (int x = 0; x < MAX_ADDRESS; x++) {
      //inicia a transmição com um endereço e finaliza para verificar se houve resposta(se existe)
    Wire.beginTransmission(x);
    int error = Wire.endTransmission();

    //retorno 0(zero) resposta do endereço foi bem sucedida
    if (error == 0) {
      if (modules == NULL) {
        modules = currentMod = (Module*) malloc(sizeof(Module));
      } else {
        currentMod->nextModule = (Module*) malloc(sizeof(Module));
        currentMod = currentMod->nextModule;
      }

      currentMod->address = x;
      currentMod->nextModule = NULL;
    }
  }

  currentMod = modules;

  // Caso não exista nenhum modulo, fica no loop de alerta
  if (currentMod == NULL) {
    Serial.println("ALERTA!!!");
    Serial.println("Não existe nenhum módulo configurado.");
    while (true) {
      writePin(LED_WARNING, HIGH);
      delay(250);
      writePin(LED_WARNING, LOW);
      delay(250);
    }
  }

  while (currentMod != NULL) {
    Serial.print("modulo: ");
    Serial.println(currentMod->address);
    Serial.print("nextMo: ");
    Serial.println((int)currentMod->nextModule);
    currentMod = currentMod->nextModule;
  }

  Serial.println("Fim do SETUP.");
}

void loop() {
  currentMod = modules;
  while(currentMod != NULL){
    Wire.requestFrom(currentMod->address, 1);
    while(Wire.available()){
      byte b = Wire.read();
      Serial.println(b);
    }
    currentMod = currentMod->nextModule;
  }
}
