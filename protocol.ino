/*
получить сигнал от полки № х
найти путь до полки
отправиться к полке
ожидать сигнал от полки: "товар получен"
найти путь до выхода
отправиться к выходу
почистить переменные
вернуться домой
*/

#include "Arina's_code.ino" //включение файла с картой и функцией для поиска пути
#include "Vanya's_code.ino" //включение файла с алогоритмами движения

short shelfStatusPin = 13;           //пин для сигнала о готовности полки
short shelfStatusPin_Number = 12;    //пин для получения номера полки
short shipmentStatusPin = 11; //пин для сигнала о готовности товара
bool shelfStatus;    //переменная для обработки сигнала о готовности полки
bool shipmentStatus; //переменная для обработки сигнала о готовности товара
bool workDone;       //переменная для статуса окончания действий робота.

void setup() {
  pinMode(shelfStatusPin, INPUT);        //пин в режим получения сигнала
  pinMode(shelfStatusPin_Number, INPUT); //пин в режим получения сигнала
  pinMode(shipmentStatusPin, INPUT);     //пин в режим получения сигнала
  bool shelfStatus = LOW;    //обнуление статуса готовности полки
  bool shipmentStatus = LOW; //обнуление статуса готовности товара
  bool workDone = false;     //обнуление статуса занятости
}

void loop() {
  
  shelfStatus = digitalRead(shelfStatusPin);    //статус готовности - полученный или не полученный сигнал с полки  
  if (shelfStatus == HIGH) {                                    //если сигнал получен, то
     shelfStatusPin_Number = digitalRead("туц туц");            //получаю номер полки, от которой пошел сигнал
     moving.moveTo(pathfinding.findway(shelfSignalPin_Number)); //робот нашел путь до полки № n, двинулся к точке назначения
   }
   
  shipmentStatus = digitalRead(shelfStatusPin); //статус готовности - полученный или не полученный сигнал о товаре
  if (shipmentStatus == HIGH) {                                 //если сигнал получен, то
     moving.moveTo(pathfinding.findway(landingZone));           //робот нашел путь до зоны выгрузки, двинулся к точке назначения
     workDone = true;                                           //работа выполнена, теперь обнулить нужные переменные и двинуть домой
   }

  if (workDone) {
     shelfStatusPin_Number = digitalWrite("пиу пиу")     //обнуляю данные о номере готовой полки
     shelfStatus = LOW;                                  //обнуляю статус готовности полки
     shipmentStatus = LOW;                               //обнуляю статус готовности товара
     workDone = false;                                   //обнуляю статус занятости
     moving.moveTo(pathfinding.findway(startZone));      //робот нашел путь домой, отправился домой
   }
      
}
  








  
