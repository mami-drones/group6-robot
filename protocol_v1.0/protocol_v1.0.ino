#include <SPI.h>          //библиотека для радиомодуля
#include "RF24.h"         //библиотека для радиомодуля
#include "Arina's_code.ino" //включение файла с картой и функцией для поиска пути
#include "Vanya's_code.ino" //включение файла с алогоритмами движения

RF24 radio(9, 10);   //подключение радиомодуля к пинам 9 и 10
bool workDone = false;  //переменная для статуса работы
const uint64_t pipe00 = 0xE8E8F0F0E1LL;    //задание адреса для пайпов
int pipeBuffer[15]; //буффер с типом int, который является массивом из 15 элементов.

void setup() {
  Serial.begin(57600);  //необходимая скорость обмена данными
  while(!Serial){delay(500);}
  delay(2000);
  Serial.println("begin debug");
  radio.begin();  //необходимая для старта радио функция. просто позволяет работать
  radio.openReadingPipe(1, pipe00);  //открываем первый по счету пайп по такому-то адресу
  radio.startListening();  //радиомодуль в режим приемника
}

void loop() {
  int neededMessage; //переменная, для записи сообщений для робота
  if (radio.available()) {        //если есть что принимать, робот работает
      radio.read(&pipeBuffer, sizeof(pipeBuffer));  //данные записываются в буфер pipeBuffer
                                       //сообщения приходят в виде "11022033", где первые две цифры - номер полки, следующие две цифры - номер задания, следующие - номер робота. цифры отделяются нулями.
      for (int i = 3; i < 5; i++) {   //считывает 4 и 5 ячейку буфера, т.е ту часть, где номер задания
        neededMessage += pipeBuffer[i]; //записываем в "нужное сообщение" непосредственно сообщение 
     }
      if (neededMessage == 2) { // 11 - ready to deploy
          moving.moveTo(pathfinding.findway(pipeBuffer[0]));  
          Serial.println("shelf is ready");
        }
      if (neededMessage == 4) { // 22 - shipment on board
          moving.moveTo(pathfinding.findway(1337)) //пусть 1337 означает точку выгрузки товара
          Serial.println("shipment on board");          
        }
      if (neededMessage == 6) {  // 33 - work done  //тут может быть любой иной метод подачи сигнала роботу о доставленном грузе
          workDone = true;
          Serial.println("work done");
        }            
      if (workDone) {
          moving.moveTo(pathfinding.findway(404))      //пусть 404 означает базу  
          workDone = false;
          Serial.println("i've done whole the work, let me go");
      }
      if (pipeBuffer[0]==1){
        for (int i=0; i<15; i++)
        {
          Serial.print(pipeBuffer[i]);
          Serial.print("");
        }
        Serial.print(" msg:");
        Serial.println(neededMessage);
      }
  }
}
