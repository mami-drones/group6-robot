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

//===================================Переменные для алгоритма движения=======================================\\
#define LLsensor A4 //Левый боковой сенсор 
#define RLsensor A3 //Правый боковой сенсор
#define Lsensor1 A1 //Первый датчик на линейке
#define Lsensor2 12 //Второй датчик на линейке
#define Lsensor3 11 //Третий датчик на линейке
#define Lsensor4 8 //Четвертый датчик на линейке
#define Lsensor5 3 //Пятый датчик на линейке
#define Lsensor6 2 //Шестой датчик на линейке
#define Lsensor7 1 //Седьмой датчик на линейке
#define Lsensor8 0 //Восьмой датчик на линейке

float Sum = 0; //Сумма весов присвоенных сенсорам линейки
int SpeedL = 40; //Скорость левого мотора
int SpeedR = 40; //Скорость правого мотора
int s1, s2, s3, s4, s5, s6, s7, s8, s9, s10; //Показание датчиков в виде (0/1)
float k1 = 0.1, k2 = 2, k3 = 0; //Коэффициенты ПИД регулятора
float DeltaSum = 0, LastSum = 0, DeltaTime = 0;
float Error1, Error2, Error3, Reg; //Переменные для рассчета ПИД регулятора
float StopCycleTime = 0; //Конечное время цикла движения вперед
int AllSum = 0; //общая сумма весов присвоенных сенсорам линейки
int CurrentError = 0; //Текущая ошибка ПИД регулятора
unsigned long AllTime; //Время выполнения программы
int state = 1, staterevers = 0; //Состояние движения и поворота
//==========================================================================================================//

#include <SPI.h>   
#include "RF24.h"
#include "nRF24L01.h"

RF24 radio(9, 10);
const uint64_t pipe00 = 0xE8E8F0F0E1LL;
int pipeBuffer[3][3];

void setup(){
 Serial.begin(57600);
 while(!Serial){delay(500);} //пока не установится скорость обмена данными, ждать по пол секунды {delay(500);}
 Serial.println("The programm has been started.");
 radio.begin();
 radio.openReadingPipe(1, pipe00);
 radio.startListening();
}

void loop() 
{

 radio.openReadingPipe(1, pipe00);
 radio.startListening();
 
   
 while(radio.available())
 { 
   radio.read(&pipeBuffer, sizeof(pipeBuffer));
   
   if (pipeBuffer[0][0] == 0 && pipeBuffer[0][1] == 0 && pipeBuffer[0][2] == 0)
   {
      delay(500);
      radio.read(&pipeBuffer, sizeof(pipeBuffer));
      int currentListOfShipments[3][3];
      for (int z = 0; z < 3; z++)
      {
        for (int g = 0; g < 3; g++)
        {
          currentListOfShipments[z][g] = pipeBuffer[z][g];
          Serial.print("[");Serial.print(currentListOfShipments[z][g]);Serial.print("] ");    
        }
       Serial.println("");
      }
      Serial.println("I got the list of shipments!");
      radio.stopListening();
      radio.openWritingPipe(pipe00);
      //pathfinding.findPath(1); ищем путь к единственной нашей полке
      int messageForShelf[3];
      for (int z = 0; z < 3; z++)
      {
        messageForShelf[z] = currentListOfShipments[z][0];
      }
      radio.write(&messageForShelf, sizeof(messageForShelf));
      radio.startListening();
      bool shipmentDropped = false;
      while (!shipmentDropped)
      {
        delay(500);
        Serial.println("Waiting for shipment.");
        if (radio.available())
        {
          int anyTh;
          radio.read(&anyTh,sizeof(anyTh));
          Serial.print("Got feedback! Command num is ");
          Serial.println(anyTh);
          if (anyTh == 1)
          {
            shipmentDropped = true;
            Serial.println("Got the shipment");     
          }
        }
      }
      Serial.println ("Going to unloading zone.");
      //pathfinding.findPath(404); ищем путь на зону выгрузки
      Serial.println ("Im in unloading zone!");
      bool readyToGo = false;
      while (!readyToGo)
      {
        delay(500);
        Serial.println("Waiting for signal form unloading zone.");
        if (radio.isAckPayloadAvailable())
        {
          int anyTh;
          radio.read(&anyTh,sizeof(anyTh));
          Serial.print("Got feedback! Command num is ");
          Serial.println(anyTh);
          if (anyTh == 1)
          {
            readyToGo = true;
            Serial.println("Ready to home!");     
          }
        }
      }
      //pathfinding.findPath(-1); ищем путь на базу
      Serial.println("Work done!");
   }             
 }
 Serial.println("Radio is not available!");
 delay(500);
}

void sensors() {
  s1 = digitalRead(Lsensor1);
  s2 = digitalRead(Lsensor2);
  s3 = digitalRead(Lsensor3);
  s4 = digitalRead(Lsensor4);
  s5 = digitalRead(Lsensor5);
  s6 = digitalRead(Lsensor6);
  s7 = digitalRead(Lsensor7);
  s8 = digitalRead(Lsensor8);
  s9 = digitalRead(RLsensor);
  s10 = digitalRead(LLsensor);
}

void stp() {
  analogWrite(5, 0);
  analogWrite(6, 0);
}

void forward() {
  while ((s9 != 1) or (s10 != 1))
  {
    sensors();

    AllTime = millis();
    DeltaTime = AllTime - StopCycleTime;
    if (s1 == 1) {
      Sum = Sum + 5;
    }
    if (s2 == 1) {
      Sum = Sum + 2;
    }
    if (s3 == 1) {
      Sum = Sum + 1;
    }
    if (s4 == 1) {
      Sum = Sum + 0.5;
    }
    if (s5 == 1) {
      Sum = Sum - 0.5;
    }
    if (s6 == 1) {
      Sum = Sum - 1;
    }
    if (s7 == 1) {
      Sum = Sum - 2;
    }
    if (s8 == 1) {
      Sum = Sum - 5;
    }

    CurrentError = Sum - LastSum;
    Error1 = (CurrentError / DeltaTime) * k1; //дифф
    Error2 = Sum * k2; //пропорциональный
    Error3 = (AllSum / AllTime) * k3; //интегральный
    Reg = Error1 + Error2 + Error3;
    
    if (Sum < 0) {
      SpeedL = SpeedL - Reg;
      SpeedR = 0;
    }
    if (Sum > 0) {
      SpeedR = SpeedR + Reg;
      SpeedL = 0;
    }

    digitalWrite(4, HIGH);
    analogWrite(5, SpeedL);
    digitalWrite(7, HIGH);
    analogWrite(6, SpeedR);

    AllSum = AllSum + Sum;
    LastSum = Sum;
    Sum = 0;

    SpeedL = 50;
    SpeedR = 50;
    DeltaSum = Sum - LastSum;
    StopCycleTime = AllTime;
  }
  stp();
}

void left() {
  while (s8 != 1)
  {
    sensors();
    digitalWrite(4, HIGH);
    analogWrite(5, 40);
    digitalWrite(7, LOW);
    analogWrite(6, 40);
  }
  stp();
}

void right() {
  while (s1 != 1)
  {
    sensors();
    digitalWrite(4, LOW);
    analogWrite(5, 40);
    digitalWrite(7, HIGH);
    analogWrite(6, 40);
  }
  stp();
}


//================================================поиск пути==============================================
void findWay(int k){

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int row_h;
int row_v;
int shelf_1 = -1;
int shelf_2 = -2;
int shelf_3 = -3;
int start = -11;
int finish = -99;
int robot_x = 0; //стартовая координата "х" для робота
int robot_y = 4; //стартовая координата "у" для робота
int shelf_x = 2; //координата "х" нужной полки
int shelf_y = 0; //координата "у" нужной полки
int shelf = -2; //номер нужной полки
int n = 0; //переменные, необходимые для выполнения последующих условий и циклов
int m = 1;
int h_up;
int h_down;
int v_left;
int v_right;
int need;
int need_1 = 0;
int need_2 = 0;
int x;
int y;
int x_1 = 0;
int y_1 = 0;
int x_2 = 0;
int y_2 = 0;
int stock[5][5];
int way;
int lengtharray;
int i;
int hint;
int direction_r = 4;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
for (row_h = 0; row_h < 5; ++row_h) {
    for (row_v = 0; row_v < 5; ++row_v) {
      stock[row_h][row_v] = 0;

    }
  }
  stock[0][1] = shelf_1;
  stock[2][0] = shelf_2;
  stock[4][4] = shelf_3;
  stock[0][4] = start;
  stock[4][0] = finish;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  delay (3000);
  h_up = robot_x - 1;
  h_down = robot_x + 1;
  v_left = robot_y - 1;
  v_right = robot_y + 1;

  if (h_up >= 0) {
    if (stock[h_up][robot_y] == shelf) {
      n = 1;
    }
  }
  if (h_down <= 4) {
    if (stock[h_down][robot_y] == shelf) {
      n = 1;
    }
  }
  if (v_left >= 0) {
    if (stock[robot_x][v_left] == shelf) {
      n = 1;
    }
  }
  if (v_right <= 4) {
    if (stock[robot_x][v_right] == shelf) {
      n = 1;
    }
  }

  if (n == 0) {
    if (h_up >= 0 && stock[h_up][robot_y] == 0) {
      stock[h_up][robot_y] = m;
    }
    if (h_down <= 4 && stock[h_down][robot_y] == 0) {
      stock[h_down][robot_y] = m;

    }
    if (v_left >= 0 && stock[robot_x][v_left] == 0) {
      stock[robot_x][v_left] = m;
    }
    if (v_right <= 4 && stock[robot_x][v_right] == 0) {
      stock[robot_x][v_right] = m;
    }
  }
  while (n == 0) {
    for (row_h = 0; row_h < 5; ++row_h) {
      for (row_v = 0; row_v < 5; ++row_v) {
        if (stock[row_h][row_v] == m) {
          robot_x = row_h;
          robot_y = row_v;
          h_up = robot_x - 1;
          h_down = robot_x + 1;
          v_left = robot_y - 1;
          v_right = robot_y + 1;
          if (h_up >= 0) {
            if (stock[h_up][robot_y] == shelf) {
              n = 1;
            }
          }
          if (h_down <= 4) {
            if (stock[h_down][robot_y] == shelf) {
              n = 1;
            }
          }
          if (v_left >= 0) {
            if (stock[robot_x][v_left] == shelf) {
              n = 1;
            }
          }
          if (v_right <= 4) {
            if (stock[robot_x][v_right] == shelf) {
              n = 1;
            }
          }
          if (n == 0) {

            if (h_up >= 0 && stock[h_up][robot_y] == 0) {
              stock[h_up][robot_y] = m + 1;
            }
            if (h_down <= 4 && stock[h_down][robot_y] == 0) {
              stock[h_down][robot_y] = m + 1;
            }
            if (v_left >= 0 && stock[robot_x][v_left] == 0) {
              stock[robot_x][v_left] = m + 1;
            }
            if (v_right <= 4 && stock[robot_x][v_right] == 0) {
              stock[robot_x][v_right] = m + 1;
            }
          }
        }
      }
    }
    m = m + 1;
  }
  
   //for (row_h = 0; row_h < 5; ++row_h) {
   //for (row_v = 0; row_v < 5; ++row_v) {
      //Serial.print(stock[row_h][row_v]);
      //Serial.print(" ");
  //}
      //Serial.println(" "); 
  //}
  
  h_up = shelf_x - 1;
  h_down = shelf_x + 1;
  v_left = shelf_y - 1;
  v_right = shelf_y + 1;
  if (v_left >= 0 && v_right <= 4) {
    if (stock[shelf_x][v_left] != 0 && stock[shelf_x][v_right] != 0) {
      if (stock[shelf_x][v_right] < stock[shelf_x][v_left]) {
        need_1 = stock[shelf_x][v_right];
        x_1 = shelf_x;
        y_1 = v_right;
      }
      else {
        need_1 = stock[shelf_x][v_left];
        x_1 = shelf_x;
        y_1 = v_left;
      }
    }

    else {
      if (stock[shelf_x][v_left] == 0) {
        if (stock[shelf_x][v_right] != 0) {
          need_1 = stock[shelf_x][v_right];
          x_1 = shelf_x;
          y_1 = v_right;
        }
      }
      else {
        need_1 = stock[shelf_x][v_left];
        x_1 = shelf_x;
        y_1 = v_left;
      }
    }
  }
  else if (v_left < 0 && v_right <= 4) {
    need_1 = stock[shelf_x][v_right];
    x_1 = shelf_x;
    y_1 = v_right;
  }
  else if (v_left >= 0 && v_right > 4) {
    need_1 = stock[shelf_x][v_left];
    x_1 = shelf_x;
    y_1 = v_left;
  }
  
  if (h_up >= 0 && h_down <= 4) {
    if (stock[h_up][shelf_y] != 0 && stock[h_down][shelf_y] != 0) {
      if (stock[h_up][shelf_y] < stock[h_down][shelf_y]) {
        need_2 = stock[h_up][shelf_y];
        x_2 = h_up;
        y_2 = shelf_y;
      }
      else {
        need_2 = stock[h_down][shelf_y];
        x_2 = h_down;
        y_2 = shelf_y;
      }
    }

    else {
      if (stock[h_up][shelf_y] == 0) {
        if (stock[h_down][shelf_y] != 0) {
          need_1 = stock[h_down][shelf_y];
          x_2 = h_down;
          y_2 = shelf_y;
        }
      }
      else {
        need_2 = stock[h_up][shelf_y];
        x_2 = h_up;
        y_2 = shelf_y;
      }
    }
  }
  else if (h_up < 0 && h_down <= 4) {
    need_2 = stock[h_down][shelf_y];
    x_2 = h_down;
    y_2 = shelf_y;
  }
  else if (h_up >= 0 && h_down > 4) {
    need_2 = stock[h_up][shelf_y];
    x_2 = h_up;
    y_2 = shelf_y;
  }
  if (need_1 > 0 && need_2 > 0) {
    if (need_1 < need_2) {
      need = need_1;
      x = x_1;
      y = y_1;
    }
    else {
      need = need_2;
      x = x_2;
      y = y_2;
    }
  }
  if (need_1 == 0) {
    need = need_2;
    x = x_2;
    y = y_2;
  }
  if (need_2 == 0) {
    need = need_1;
    x = x_1;
    y = y_1;
  }
  lengtharray = need + 1;
  
  int way[lengtharray]; 

  if (x < shelf_x) {
    way[lengtharray - 1] = 1;
  }
  if (x > shelf_x) {
    way[lengtharray - 1] = 2;
  }
  if (y < shelf_y) {
    way[lengtharray - 1] = 3;
  }
  if (y > shelf_y) {
    way[lengtharray - 1] = 4;
  }
  lengtharray = lengtharray - 1;
  m = need - 1;

  hint = m;
 
    for (i = 0; i < hint; ++i) {

    h_up = x - 1;
    h_down = x + 1;
    v_left = y - 1;
    v_right = y + 1;
  
    if (h_up >= 0) {

      if (stock[h_up][y] == m) {  
        way[lengtharray - 1] = 1;
        x = h_up;
        y = y;
      }
    }
    if (h_down <= 4) {
      if (stock[h_down][y] == m) {
        way[lengtharray - 1] = 2;
        x = h_down;
        y = y;
      }
    }
    if (v_left >= 0) {
      if (stock[x][v_left] == m) {
        way[lengtharray - 1] = 3;
        x = x;
        y = v_left;
      }
    }
    if (v_right <= 4) {
      if (stock[x][v_right] == m) {

        way[lengtharray - 1] = 4;
        x = x;
        y = v_right;
      }
    }
    m = m - 1;
    lengtharray = lengtharray - 1;
  }
  h_up = x - 1;
  h_down = x + 1;
  v_left = y - 1;
  v_right = y + 1;

  if (h_up >= 0) {
    if (stock[h_up][y] == stock[0][4]) {
      way[lengtharray-1] = 1;
    }
  }
  if (h_down <= 4) {
    if (stock[h_down][y] == stock[0][4]) {
      way[lengtharray-1] = 2;
    }
  }
  if (v_left >= 0) {
    if (stock[x][v_left] == stock[0][4]) {
      way[lengtharray-1] = 3;
    }
  }
  if (v_right <= 4) {
    if (stock[x][v_right] == stock[0][4]) {
      way[lengtharray-1] = 4;
    }
  }
  lengtharray = need + 1;

  //for (i = 0; i < lengtharray; ++i) {
  //  Serial.println(way[i]);
  //} 

if(direction_r == 1){
  if(way[0]==1){
    forward();
    
  }
  if(way[0]==2){
    right();
    right();
    forward();
  }
  if(way[0]==3){
    left();
    forward();
  }
  if(way[0]==4){
    right();
    forward();
}
}
if(direction_r == 2){
  if(way[0]==1){
    left();
    left();
    forward();
}
  if(way[0]==2){
    forward();
}
  if(way[0]==3){
    right();
    forward();
}
  if(way[0]==4){
    left();
    forward();
}
}
if(direction_r == 3){
  if(way[0]==1){
    right();
    forward();
}
  if(way[0]==2){
    left();
    forward();
}
  if(way[0]==3){
    forward();
  }
  if(way[0]==4){
    left();
    left();
    forward();
}
}
if(direction_r == 4){
  if(way[0]==1){
    left();
    forward();
}
  if(way[0]==2){
    right();
    forward();
}
  if(way[0]==3){
    left();
    left();
    forward();
}
  if(way[0]==4){
    forward();
}
}


for (i=1; i<lengtharray; ++i){
  if (way[i-1] == 1){
    if(way[i]==1){
      forward();
    }
    if(way[i]==2){
      left();
      left();
      forward();
    }
    if(way[i]==3){
      left();
      forward();
    }
    if(way[i]==4){
      right();
      forward();
    }
}

  if (way[i-1] == 2){
    if(way[i]==1){
      left();
      left();
      forward();
    }
    if(way[i]==2){
      forward();
    }
    if(way[i]==3){
      right();
      forward();
    }
    if(way[i]==4){
      left();
      forward();
    }
}

  if (way[i-1] == 3){
    if(way[i]==1){
      right();
      forward();
    }
    if(way[i]==2){
      left();
      forward();
    }
    if(way[i]==3){
      forward();
    }
    if(way[i]==4){
      left();
      left();
      forward();
    }
}

  if (way[i-1] ==4){
    if(way[i]==1){
      left();
      forward();
    }
    if(way[i]==2){
      right();
      forward();
    }
    if(way[i]==3){
      left();
      left();
      forward();
    }
    if(way[i]==4){
      forward();
    }
}
}
}
