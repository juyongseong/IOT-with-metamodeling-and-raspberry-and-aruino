
/*****************************************************************************/
/*                                                                           */
/*                                PROJECT                                    */
/*                         Harkerton in JBNU                                 */
/*                       Due on 12 May 2019 (Sun)                            */
/*                                                                           */
/*****************************************************************************/
/*                Programming Copright & Environment                         */
/*                                                                           */
/*            Group           : fruit                                        */
/*            Team members    : 성주용, 정윤성, 조성국, 김종희                   */
/*            PL              : C++                                          */
/*            Compiler        : Arduino                                      */
/*            OS              : Linux                                        */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*  Objective : 원하는 목표 지점에 이동 후 Object를 Get & Put 작업을 한다.         */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*  Revision History :                                                       */
/*  1. ADOxx 명령 처리                                                        */
/*   - problem : 라인 트레이서를 인식 후 첫번 째 ADOxx 명령 처리 이후의 명령         */
/*               처리가 되지 않는 문제가 발생                                    */
/*   - solution : 클레스 내 라인 트레이서 함수 분리                               */
/*****************************************************************************/
/*****************************************************************************/
/*                          Included Header Files                            */
/*****************************************************************************/
#include <SoftwareSerial.h>
#include <Servo.h>
#include <ArduinoJson.h>

/*****************************************************************************/
/*                                 DEFINE's                                  */
/*****************************************************************************/
/*  Left DC Motor -----------------------------------------------------------*/
#define EN1   10
#define EN2   5
#define ENA   3 //PWM Pin

/*  Right DC Motor ----------------------------------------------------------*/
#define EN3   4
#define EN4   2
#define ENB   6 //PWM Pin

/*  Line - Sensor -----------------------------------------------------------*/
#define LS_C 12 //Center Line-Sensor
#define LS_L 13 //Left Line-Sensor
#define LS_R 11 //Right Line-Sensor

/*  Arduino Bluetooth -------------------------------------------------------*/
#define BT_RXD 7 // 실제적으로 Arduino Bluetooth의 TXD를 꽂는 번호
#define BT_TXD 8 // 실제적으로 Arduino Bluetooth의 RXD를 꽂는 번호

/*  Survo Motor -------------------------------------------------------------*/
#define M3 A0 //몸통 서브모터
#define M4 9 //집게 서브모터

/*  Acceleration ------------------------------------------------------------*/
#define carSpeed 104
#define turnSpeed 150
/*****************************************************************************/
/*                         Structure Definitions                             */
/*****************************************************************************/
/* Description --------------------------------------------------------------*/
struct PR //return var for parsing result
{
   int index;
   String response;
};

/*****************************************************************************/
/*                             Class Definitions                             */
/*****************************************************************************/
/* Car ----------------------------------------------------------------------*/
class Car
{
public:
   Car(int pin1, int pin2, int pin3, int pin4, int pinA, int pinB); //생성자
   void on(String dir); //Parsing된 매개변수 문자열을 통해 Car 작동
   void off(); //작동중인 Car off

private:
   int pin_EN1;
   int pin_EN2;
   int pin_EN3;
   int pin_EN4;

   int pin_ENA;
   int pin_ENB;
};

Servo m3;
Servo m4;

/* Robot ARM ----------------------------------------------------------------*/
class Robot_ARM
{
public:
   Robot_ARM(int pin1, int pin2); //생성자
   void Rotate_Motor(String m, int exe_degree);
   // m : Parsing 값, exe_degree : 움직이는 각도

private:
   /*Servo m3;
   Servo m4;*/
   int pin_EN1;
   int pin_EN2;
};

/*****************************************************************************/
/*                        Function Prototype                                 */
/*****************************************************************************/
SoftwareSerial bluetooth(BT_RXD, BT_TXD);
struct PR parsing();
void control(Car car, Robot_ARM arm);
void Recognition();

/*****************************************************************************/
/*                          Global Variables                                 */
/*****************************************************************************/
int m3_pos = 0;
int m4_pos = 170;

/*****************************************************************************/
/*                         Class member function                             */
/*****************************************************************************/
Car::Car(int pin1, int pin2, int pin3, int pin4, int pinA, int pinB)
   :pin_EN1(pin1), pin_EN2(pin2), pin_EN3(pin3)
   , pin_EN4(pin4), pin_ENA(pinA), pin_ENB(pinB)
{
   pinMode(pin_EN1, OUTPUT);
   pinMode(pin_EN2, OUTPUT);
   pinMode(pin_EN3, OUTPUT);
   pinMode(pin_EN4, OUTPUT);

   pinMode(pin_ENA, OUTPUT);
   pinMode(pin_ENB, OUTPUT);
}
void Car::on(String dir)
{
   delay(500);

   if (dir == "F")
   {
      digitalWrite(pin_EN1, HIGH);
      digitalWrite(pin_EN2, LOW);
      digitalWrite(pin_EN3, HIGH);
      digitalWrite(pin_EN4, LOW);

      analogWrite(pin_ENA, carSpeed);
      analogWrite(pin_ENB, carSpeed);

      delay(600);
   }
   else if (dir == "TL")
   {
      digitalWrite(pin_EN1, LOW);
      digitalWrite(pin_EN2, LOW);
      digitalWrite(pin_EN3, HIGH);
      digitalWrite(pin_EN4, LOW);

      analogWrite(pin_ENA, 0);
      analogWrite(pin_ENB, turnSpeed);

      delay(800);
   }
   else if (dir == "TR")
   {
      digitalWrite(pin_EN1, HIGH);
      digitalWrite(pin_EN2, LOW);
      digitalWrite(pin_EN3, LOW);
      digitalWrite(pin_EN4, LOW);

      analogWrite(pin_ENA, turnSpeed);
      analogWrite(pin_ENB, 0);

      delay(800);
   }
   else if (dir == "ST")
   {
      digitalWrite(pin_EN1, LOW);
      digitalWrite(pin_EN2, LOW);
      digitalWrite(pin_EN3, LOW);
      digitalWrite(pin_EN4, LOW);
      analogWrite(pin_ENA, 0);
      analogWrite(pin_ENB, 0);
   }
}
void Car::off() {
   digitalWrite(pin_EN1, LOW);
   digitalWrite(pin_EN2, LOW);
   digitalWrite(pin_EN3, LOW);
   digitalWrite(pin_EN4, LOW);
   analogWrite(pin_ENA, 0);
   analogWrite(pin_ENB, 0);
}

Robot_ARM::Robot_ARM(int pin1, int pin2) : pin_EN1(pin1), pin_EN2(pin_EN2)
{

   //초기값 초기화를 생성자로 불러낼경우 초기화가 깔끔하지만 대기시간에도 전력을 지속적으로 잡아먹는다.
   //Reset
   /*m3.attach(M3);
   m3.write(m3_pos);

   m4.attach(M4);
   m4.write(m4_pos);
  */
}

void Robot_ARM::Rotate_Motor(String m, int exe_degree)
{
   int c_degree;
   m3.detach(); //혹시 모를 모터 유휴상태시 전기를 먹게되는 현상 방지
   m4.detach();
   
   if (m.compareTo("M3") == 0)
   {

      c_degree = m3_pos;
   
      if (c_degree <= exe_degree)
      {
         for (int pos = c_degree; pos <= exe_degree; pos += 1)
         {
            Serial.println(pos);
            m3.attach(M3);
            m3.write(pos);
            m3_pos = pos;

            delay(5);
         }
      }

      else if (c_degree > exe_degree)
      {
         for (int pos = c_degree; pos > exe_degree; pos -= 1)
         {
            Serial.println(pos);
            m3.attach(M3);
            m3.write(pos);
            m3_pos = pos;

            delay(5);
         }
      }
      delay(3000);
      m3.detach(); //종료시 전기를 제거
   }
   else if (m.compareTo("M4") == 0)
   {

      c_degree = m4_pos;
      if (c_degree <= exe_degree)
      {
         for (int pos = c_degree; pos <= exe_degree; pos += 1)
         {
            m4.attach(M4);
            m4.write(pos);
            m4_pos = pos;

            delay(5);
         }
      }

      else if (c_degree > exe_degree)
      {
         for (int pos = c_degree; pos > exe_degree; pos -= 1)
         {
            m4.attach(M4);
            m4.write(pos);
            m4_pos = pos;

            delay(5);
         }
      }
      delay(3000);
      m4.detach();
   }
}
/*****************************************************************************/
/*                                Function                                   */
/*****************************************************************************/
struct PR parsing()
{
   struct PR result;
   String response = "";
   int temp = 0;

   if (bluetooth.available()) {
      long int time = millis(); //현재 시간 설정
      int timeout = 3000; //ms
      while ((time + timeout) > millis()) {
         while (bluetooth.available()) {
            char c = bluetooth.read();
            response += c;
         }
      }
      Serial.println("response :" + response);
   }
   if (Serial.available()) {
      bluetooth.write(Serial.read());
   }

   /////JSON 파싱 시작/////
   String seperator = "@";
   result.index = response.indexOf(seperator);
   result.response = response;

   return result;
}

void control(Car car, Robot_ARM arm)
{
   struct PR parsing_result = parsing();
   int startindex = parsing_result.index;
   String response = parsing_result.response;
   int exe_time;

  
      String json_raw_data = response.substring(startindex + 1); //@바로 앞 인덱스
      StaticJsonBuffer<2500> jsonBuffer;
      JsonObject& json_data = jsonBuffer.parseObject(json_raw_data);
      /////JSON 파싱 끝/////

      /////모터 제어 시작/////
      int state = atoi(json_data["state"]);
      for (int i = 1; i <= state; i++)
      {
         String st(i);
         String temp = json_data[st + "_t"];

         if (temp.compareTo("car") == 0)
         {
            String qs = json_data[st + "_qs"];
            exe_time = atoi(json_data[st + "_time"]);

            car.off();
            delay(1000);

            Recognition(); //라인트레이서 인식 시작

            //parsing된 명령 값 판단
            if (qs == "ST") break;
            else car.on(qs);
         }
         else if (temp.compareTo("arm") == 0)
         {

            String m = json_data[st + "_m"];
            int exe_degree = atoi(json_data[st + "_d"]);

            car.off();
            arm.Rotate_Motor(m, exe_degree);
         }
      }
   
}
void Recognition()
{
   int in = 1;

   while (in)
   {
      int LS_center = digitalRead(LS_C);
      int LS_left = digitalRead(LS_L);
      int LS_right = digitalRead(LS_R);

      if ((LS_center == 1 && LS_left == 1 && LS_right == 1) == 1)  // 셋 다 인식되면 전진
      {
         Serial.println("직진");
         digitalWrite(EN1, HIGH);
         digitalWrite(EN2, LOW);
         digitalWrite(EN3, HIGH);
         digitalWrite(EN4, LOW);

         analogWrite(ENA, 90);
         analogWrite(ENB, carSpeed);
      }
      else if ((LS_center == 1 && LS_left == 1 && LS_right == 0) == 1 ||
         (LS_center == 0 && LS_left == 1 && LS_right == 0) == 1) // 오른쪽만 인식되면 좌회전
      {
         Serial.println("좌회전");
         digitalWrite(EN1, LOW);
         digitalWrite(EN2, LOW);
         digitalWrite(EN3, HIGH);
         digitalWrite(EN4, LOW);

         analogWrite(ENA, carSpeed);
         analogWrite(ENB, carSpeed);
      }
      else if ((LS_center == 1 && LS_left == 0 && LS_right == 1) == 1 ||
         (LS_center == 0 && LS_left == 0 && LS_right == 1) == 1)// 왼쪽만 인식되면 우회전
      {
         Serial.println("우회전");
         digitalWrite(EN1, HIGH);
         digitalWrite(EN2, LOW);
         digitalWrite(EN3, LOW);
         digitalWrite(EN4, LOW);

         analogWrite(ENA, carSpeed);
         analogWrite(ENB, carSpeed);
      }
      else if ((LS_center == 0 && LS_left == 0 && LS_right == 0) == 1) // 정지
      {
         Serial.println("정지");
         digitalWrite(EN1, LOW);
         digitalWrite(EN2, LOW);
         digitalWrite(EN3, LOW);
         digitalWrite(EN4, LOW);

         analogWrite(ENA, 0);
         analogWrite(ENB, 0);

         in = 0;
      }
   }
}

/*****************************************************************************/
/*                            main() Function                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Algorithm : Parsing된 값에 따라 행동을 취한다.                                */
/*        1. RC Car를 검은색 라인에서 실행시킨다.                                 */
/*        2. RC Car가 따라가다 라인을 벗어나 멈추게 되면                           */
/*                             ADOxx에서 명령을 받아온다.                       */
/*        3. 받은 명령(파싱된 값)을 다음과 같이 실행한다.                           */
/*         - "F"  : 1초간 직진                                                */
/*         - "TL" : 2초간 좌회전                                               */
/*         - "TR" : 2초간 우회전                                               */
/*        4. 다시 1. 로 돌아간다.                                              */
/*        5. 이는 정지 신호가 있을 때까지 계속 진행해야 하는 상황이지만               */
/*           평가 시, 빠른 진행을 위해 한바퀴만 진행 할 수 있도록                   */
/*           ADOxx에서 "STOP" 명령도 설정한다                                   */
/*                                                                           */
/*****************************************************************************/

void setup()
{
   pinMode(LS_C, INPUT);
   pinMode(LS_L, INPUT);
   pinMode(LS_R, INPUT);



   Serial.begin(9600);
   bluetooth.begin(9600);

}
void loop() {
   PR parsing();
   
   Car car(EN1, EN2, EN3, EN4, ENA, ENB);
   Robot_ARM arm(M3, M4);
   control(car, arm);
}
