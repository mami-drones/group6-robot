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
void setup()
{}
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
int state = 0, staterevers = 0; //Состояние движения и поворота
void loop()
{
  AllTime = millis();
  DeltaTime = AllTime - StopCycleTime;
  {
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

    Serial.print(state);
    Serial.print(" \\\ ");
                 Serial.println(Sum);
                 if (state == 0)
                 {
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



                 SpeedL = 40;
                 SpeedR = 40;
                 DeltaSum = Sum - LastSum;
                 StopCycleTime = AllTime;
                 /*
                 Serial.print(Reg);
                 Serial.print(" ");
                 Serial.print(Error1);
                 Serial.print(" ");
                 Serial.print(Error2);
                 Serial.print(" ");
                 Serial.println(Error3);
                 */

                 if (s9 == 1 or s10 == 1)
                 {
                 state = 2;

               }
               }

                 if (state == 2)
                 {
                 Serial.print("--------------— ");
                 Serial.println(staterevers);
                 if (staterevers == 0)
                 {

                 {
                 digitalWrite(4, HIGH);
                 analogWrite(5, 30);
                 digitalWrite(7, LOW);
                 analogWrite(6, 30);
               }
                 if (Sum == 0)
                 {
                 staterevers = 1;
               }

               }
                 if (staterevers == 1)
                 {
                 digitalWrite(4, HIGH);
                 analogWrite(5, 30);
                 digitalWrite(7, LOW);
                 analogWrite(6, 30);
                 if (Sum != 0)
                 state = 0 ;

               }

               }
               }
               }