#include <Wire.h>
#include "CompactArduino.h"
#include "KtaneModule.h"

#define PIN_LED_STATUS 13

#define PIN_FIO_1 4
#define PIN_FIO_2 5
#define PIN_FIO_3 6
#define PIN_FIO_4 7
#define PIN_FIO_5 8
#define PIN_FIO_6 9

#define PIN_LED_1 10
#define PIN_LED_2 11
#define PIN_LED_3 12

#define MAX_FIOS 6

#define ACAO_CORRETA  1
#define ACAO_ERRADA   2

volatile byte respostaSlave = 0;

int address = 0x00;
bool fioStatus[6];
bool fioStatusAntigo[6];
int fios[6] = {-1, -1, -1, -1, -1, -1};
int erros = 0;

void setup() {
  // setando pinos de endereço
  setPin(PIN_ADDRESS_0, INPUT);
  setPin(PIN_ADDRESS_1, INPUT);
  setPin(PIN_ADDRESS_2, INPUT);
  
  //setando pinos dos fios
  setPin(PIN_FIO_1, INPUT);
  setPin(PIN_FIO_2, INPUT);
  setPin(PIN_FIO_3, INPUT);
  setPin(PIN_FIO_4, INPUT);
  setPin(PIN_FIO_5, INPUT);
  setPin(PIN_FIO_6, INPUT);

  //setando pinos dos leds
  setPin(PIN_LED_STATUS, OUTPUT);
  setPin(PIN_LED_1, OUTPUT);
  setPin(PIN_LED_2, OUTPUT);
  setPin(PIN_LED_3, OUTPUT);
  
  address = readPin(PIN_ADDRESS_2) << 2 | readPin(PIN_ADDRESS_1) << 1 | readPin(PIN_ADDRESS_0);

  Wire.begin(address);
  Serial.begin(9600);
  Serial.print("Valor: ");
  Serial.println(address);
  Serial.println("FIM");
  
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.println("Esperando todos os fios serem conectados");
  while(!verificaFiosConectados());
  Serial.println("Todos os fios estão conectados!");
  copyArray(fioStatus, fioStatusAntigo, MAX_FIOS);
}

void loop() {
  randomizeGame();

  int fiosRetirados = 0;
  // ajuste de fios já retirados
  for(int x = 0; x < MAX_FIOS; x++){
    if(fioStatus[fios[x]]){
      fiosRetirados = x;
      break;
    }
  }

  if(verificaTodosFiosDesconectados()){
    writePin(PIN_LED_STATUS, HIGH);
    writePin(PIN_LED_1, LOW);
    writePin(PIN_LED_2, LOW);
    writePin(PIN_LED_3, LOW);
    while(true);
  }

  Serial.print("Fio a ser retirado: ");
  Serial.println(fios[fiosRetirados]);
  bool fioRetirado = false;
  while(!fioRetirado){
    readFios();

    int resp = verificaHouveFioRetirado();
    if(resp != -1){
      Serial.print("Fio retirado: ");
      Serial.println(resp);
      
      if(resp != fios[fiosRetirados]){
        respostaSlave |= ACAO_ERRADA;
        erros++;
        Serial.println("Fio errado!!!");
      } else {
        writePin(PIN_LED_STATUS, HIGH);
        delay(150);
        writePin(PIN_LED_STATUS, LOW);
        respostaSlave |= ACAO_CORRETA;
        Serial.println("Fio retirado com sucesso.");
      }

      fioRetirado = true;
    }
    delay(50);
  }

  if(erros > 2){
    while(true){
      writePin(PIN_LED_1, random(2));
      writePin(PIN_LED_2, random(2));
      writePin(PIN_LED_3, random(2));
      delay(1000);
    }
  }

  //copia o status dos fios atuais para o antigo
  copyArray(fioStatus, fioStatusAntigo, MAX_FIOS);
}

bool verificaTodosFiosDesconectados(){
  for(int x = 0; x < MAX_FIOS; x++){
    if(fioStatus[x]){
      return false;
    }
  }
  return true;
}

void copyArray(bool origin[], bool destination[], int sizeArray){
  for(int x = 0; x < sizeArray; x++){
    destination[x] = origin[x];
  }
}

int verificaHouveFioRetirado(){
  for(int x = 0; x < MAX_FIOS; x++){
    if(fioStatusAntigo[x] != fioStatus[x]){
      return x;
    }
  }
  return -1;
}

bool verificaFiosConectados(){
  readFios();
  for(int x = 0; x < MAX_FIOS; x++){
    if(!fioStatus[x]){
      return false;
    }
  }
  return true;
}

bool readFios(){
  fioStatus[0] = readPin(PIN_FIO_1);
  fioStatus[1] = readPin(PIN_FIO_2);
  fioStatus[2] = readPin(PIN_FIO_3);
  fioStatus[3] = readPin(PIN_FIO_4);
  fioStatus[4] = readPin(PIN_FIO_5);
  fioStatus[5] = readPin(PIN_FIO_6);
}

bool existInArray(int num, int arr[], int lengthArray){
  for(int x = 0; x < lengthArray; x++){
    if(num == arr[x]){
      return true;
    }
  }
  return false;
}

void randomizeGame(){
  int randN = analogRead(A0)%8 + 1;
  randomSeed(randN--);
  
  Serial.print("Random Seed: ");
  Serial.println(randN);

  Serial.println(randN / 4);
  Serial.println((randN % 4)/2);
  Serial.println(randN % 2);
  writePin(PIN_LED_3, randN / 4);
  writePin(PIN_LED_2, (randN % 4)/2);
  writePin(PIN_LED_1, randN % 2);

  for(int x = 0; x < MAX_FIOS; x++){
    fios[x] = -1;
  }
  
  for(int x = 0; x < MAX_FIOS; x++){
    int num = random(MAX_FIOS);
    if(existInArray(num, fios, MAX_FIOS)){
      x--;
      continue;
    }
    fios[x] = num;
  }
  Serial.print("Fios: ");
  for(int x = 0; x < MAX_FIOS; x++){
    Serial.print(fios[x]);
    if(x + 1 != MAX_FIOS)
      Serial.print(", ");
  }
  Serial.println();
}

void receiveEvent(int numBytes){
  
}

void requestEvent(){
//  respostaSlave
//  Wire.write(respostaSlave);
}
