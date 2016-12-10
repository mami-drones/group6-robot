//с кассы приходит сигнал роботу с перечнем продуктов, перечень выглядит, как двумерный массив
//например: "[0][10][5]" бананы(10), кол-во 5 штук.
//           [1][13][5]  яблоки(13), кол-во 5 штук.
//           [2][16][4]  ананасы(16), кол-во 4 штуки.                                                    
//робот определяет в каких полках эти продукты и прокладывает маршрут
//едет до первой нужной полки
//останавливается под полкой и подает ей сигнал о приезде с перечнем товаров
//получает обратый сигнал от полки, что товар сброшен
//едет до следующей нужной полки...
//...
//ищет путь до зоны выгрузки
//едет до зоны выгрузки
//подает сигнал о завершении и возвращается в очередь

#include <SPI.h>   
#include "RF24.h"

RF24 radio(9, 10);
const uint64_t pipe00 = 0xE8E8F0F0E1LL;
int pipeBuffer[3];
int messageForShelf = 1337;

void setup() 
{
 Serial.begin(57600);
 while(!Serial) //пока не установится скорость обмена данными, ждать по пол секунды
 {    
   delay(500);
 }
 Serial.println("connection speed aprroved");
 radio.begin();
 radio.enableAckPayload();
}

void loop() 
{
 while(radio.available())
 {
   radio.openReadingPipe(1, pipe00);
   radio.startListening();
   radio.read(&pipeBuffer, sizeof(pipeBuffer));
   
   if (pipeBuffer[0] == 0 && pipeBuffer[1] == 0 && pipeBuffer[2] == 0)
   {
      int *currentListOfShipments[] = pipeBuffer[];
      Serial.println("I got the list of shipments!");
      radio.stopListening();
      radio.closeReadingPipe(1, pipe00);
      radio.openWritingPipe(pipe00);
      /*
      ↓тут должен быть алгоритм для поиска оптимальной последовательности посещения полок, но, слава богу, полка у нас одна :)
        whichShelfHasShipment(pipeBuffer[0]);
        searchBestShelfOrder(var)
      ↑тут должен быть алгоритм для поиска оптимальной последовательности посещения полок, но, слава богу, полка у нас одна :)
      */
      //pathfinding.findPath(1); ищем путь к единственной нашей полке
      //moving.moveTo(); едем к полке
      radio.write(&messageForShelf, sizeof(messageForShelf));
      bool shipmentDropped = false;
      while (!shipmentDropped)
      {
        delay(500);
        Serial.println("Waiting for shipment.");
        if (radio.isAckPayloadAvailable())
        {
          int anyTh;
          radio.read(&anyTh,sizeof(anyTh));
          Serial.printf("Got feedback! Command num is [%lu]",anyTh);
          if (anyTh == 1)
          {
            shipmentDropped = true;
            Serial.print("Got the shipment");     
          }
        }
      }
      Serial.print ("Going to unloading zone.");
      //pathfinding.findPath(404); ищем путь на зону выгрузки
      //moving.moveTo(); едем на зону выгрузки
      Serial.print ("Im in unloading zone!");
      bool readyToGo = false;
      while (!readyToGo)
      {
        if (radio.isAckPayloadAvailable())
        {
          int anyTh;
          radio.read(&anyTh,sizeof(anyTh));
          Serial.printf("Got feedback! Command num is [%lu]",anyTh);
          if (anyTh == 1)
          {
            readyToGo = true;
            Serial.print("Ready to home!");     
          }
        }
      }
      //pathfinding.findPath(-1); ищем путь на базу
      //moving.moveTo(); едем на базу
      Serial.print("Work done!");     
   }             
 }
 Serial.println("Radio is not available!");
 delay(500);
}
/*
 int whichShelfHasShipment (int shipmentNum){
 int a;
 if (shipmentNum < 333){
   a = 1;
  else if ((shipmentNum >= 333) && (shipmentNum < 666)){
   a = 2;
  }
  else if (shipmentNum >= 666){
   a = 3;
  }
 }
 return a;
}
 */
