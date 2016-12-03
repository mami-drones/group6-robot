/*
получить сигнал от полки № х
найти путь до полки
отправиться к полке
ожидать сигнал от полки: "товар получен"
найти путь до выхода
отправиться к выходу
вернуться домой
*/

#include <SPI.h>          //библиотека для радиомодуля
#include "nRF24L01.h"     //библиотека для радиомодуля
#include "RF24.h"         //библиотека для радиомодуля
#include "Arina's_code.ino" //включение файла с картой и функцией для поиска пути
#include "Vanya's_code.ino" //включение файла с алогоритмами движения

RF24 radio(9, 10);   //подключение радиомодуля к пинам 9 и 10
bool workDone;  //переменная для статуса работы
const uint64_t pipe00 = 0xE8E8F0F0E1LL;    //задание адреса для пайпов
char pipeBuffer[30]; //буффер с типом char, который является массивом из 30 элементов.
char neededMessage[1]; //массив длинной 1, для записи нужного сообщения

void setup() {
  workDone = false;
  Serial.begin(57600);   //необходимая скорость обмена данными
  radio.begin();    //необходимая для старта радио функция. просто позволяет работать
  radio.openReadingPipe(1, pipe00);  //открываем первый по счету пайп по такому-то адресу
  radio.startListening();       //радиомодуль в режим приемника
}

void loop() {
  if (radio.available()) {                         //если есть что принимать, робот работает
     radio.read(&pipeBuffer, sizeof(pipeBuffer));  //данные записываются в буфер pipeBuffer, размер буфера соответствует переменной pipeBuffer
     for (int i = 2, i < 21, i++) {    //сообщения приходят в виде "1_blah blah blah_2", где 1 - номер полки, а 2 - номер робота, которому идет заказ.
       neededMessage[0] += pipeBuffer[i]; //считывает с 3 по 22 ячейку буфера, т.е ту часть, где сам текст, а затем составляет сообщение
     }
     switch (neededMessage[0]) {        //переключатель считывает необходимое сообщение и делает дела
        case "ready to deploy": {
          moving.moveTo(pathfinding.findway(pipeBuffer[0]));  //берет первый символ из сообщения в буфере, который является номером полки
          break;
        }
        case "shipment on board": {
          moving.moveTo(pathfinding.findway(1337)) //пусть 1337 означает точку выгрузки товара
          workDone = true;
          delay(30000); //после приезда на точку выгрузки товара, пусть подождет 30 сек, пока товар заберут
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
