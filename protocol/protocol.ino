#include <SPI.h>          //библиотека для радиомодуля
#include "RF24.h"         //библиотека для радиомодуля
#include "Arina's_code.ino" //включение файла с картой и функцией для поиска пути
#include "Vanya's_code.ino" //включение файла с алогоритмами движения

RF24 radio(9, 10);   //подключение радиомодуля к пинам 9 и 10
bool workDone = false;  //переменная для статуса работы
const uint64_t pipe00 = 0xE8E8F0F0E1LL;    //задание адреса для пайпов
char pipeBuffer[30]; //буффер с типом char, который является массивом из 30 элементов.
char* neededMessage[]; //массив, для записи сообщений для робота

void setup() {
  Serial.begin(57600);  //необходимая скорость обмена данными
  radio.begin();  //необходимая для старта радио функция. просто позволяет работать
  radio.openReadingPipe(1, pipe00);  //открываем первый по счету пайп по такому-то адресу
  radio.startListening();  //радиомодуль в режим приемника
}

void loop() {
  if (radio.available()) {        //если есть что принимать, робот работает
      radio.read(&pipeBuffer, sizeof(pipeBuffer));  //данные записываются в буфер pipeBuffer
                                       //сообщения приходят в виде "1_blah blah blah", где 1 - номер полки
      for (int i = 2, i < 30, i++) {   //считывает с 3 по 30 ячейку буфера, т.е ту часть, где сам текст, а затем составляет сообщениe.
        neededMessage[0] += pipeBuffer[i]; //в первую ячейку массива запишется "строка" в виде массива символов 
     }
    switch (neededMessage[0]) {        //переключатель считывает необходимое сообщение и делает дела
        case "ready to deploy": {
          moving.moveTo(pathfinding.findway(pipeBuffer[0]));  //берет первый символ из сообщения в буфере, который является номером полки
          break;
        }
        case "shipment on board": {
          moving.moveTo(pathfinding.findway(1337)) //пусть 1337 означает точку выгрузки товара
          break;
        }
        case "work done": {   //вместо еще одного case может быть любой иной метод подачи сигнала роботу о доставленном грузе
          workDone = true;
          break;
        }
        break;
     }
    if (workDone) {
      moving.moveTo(pathfinding.findway(404))      //пусть 404 означает базу  
      workDone = false;
    }
  }
}
