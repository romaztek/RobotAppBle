// #include <SoftwareSerial.h>         // Библиотека программного UART для BT-модуля
#include <TimerOne.h>             // Библиотека для прерывания по таймеру для BT-модуля
#include "iarduino_MultiServo.h"  // Библиотека для управления сервоприводами
#include "ArduinoMotorShieldR3.h" // Библиотека для управления двигателями
#include "DFPlayer_Mini_Mp3.h"    // Библиотека для управления mp3-плеером
#include <U8glib.h>               // Библиотека для управления ЖК-экраном
#include "rus.h"                  // Русский шрифт для ЖВ-экрана
#include "lcd.cpp"
#include "servo_defines.h"   // Директивы для сервоприводов
#include "servo_functions.h" // Функции для сервоприводов
#include "bt_defines.h"

ArduinoMotorShieldR3 md;

// SoftwareSerial Serial1(7, 10); // RX, TX

volatile char bt_cmd;

volatile bool bt_flag = false;
volatile bool servo_flag = false;

volatile bool forward_flag = false;
//                          D0 D1 D2 D3 D4 D5  D6 D7 E   RS

U8GLIB_ST7920_128X64_1X *u8g;
#define u8g_logo_width 128
#define u8g_logo_height 64

int max_speed = 200;
int half_speed = 100;

void blue_interrupt()
{
  bt_cmd = Serial1.read();

  if (bt_cmd == servoStopCommand)
  {
    servo_flag = false;
  }
  if (bt_cmd != -1)
    bt_flag = true;
}

void setup()
{
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(53, OUTPUT);

  pinMode(PIN_SPINE, OUTPUT);
  pinMode(PIN_RIGHT_SHOULDER_1, OUTPUT);
  pinMode(PIN_LEFT_SHOULDER_1, OUTPUT);
  pinMode(PIN_RIGHT_SHOULDER_2, OUTPUT);
  pinMode(PIN_LEFT_SHOULDER_2, OUTPUT);
  pinMode(PIN_RIGHT_HAND_1, OUTPUT);
  pinMode(PIN_LEFT_HAND_1, OUTPUT);
  pinMode(PIN_RIGHT_HAND_2, OUTPUT);
  pinMode(PIN_LEFT_HAND_2, OUTPUT);
  pinMode(PIN_BELT, OUTPUT);
  pinMode(PIN_NECK, OUTPUT);
  pinMode(PIN_HEAD, OUTPUT);

  // u8g = new U8GLIB_ST7920_128X64_1X(A5,A4,A3,A2,A1,A0, 2, 4, 8, 9);
  u8g = new U8GLIB_ST7920_128X64_1X(A5, A4, A3, A2, A1, A0, 50, 51, 52, 53);
  // u8g->firstPage();
  // do
  // {
  //   u8g->setColorIndex(1); // Выбираем белый цвет
  //   // u8g->setFont(rus5x7);                                  // Выбираем шрифт rus5x7
  //   u8g->setColorIndex(0); // Выбираем цвет фона
  //   u8g->setColorIndex(1);
  //   u8g->drawXBMP(0, 0, u8g_logo_width, u8g_logo_height, valakaz_u8g_bits);
  // } while (u8g->nextPage());

  md.init();

  Serial.begin(9600);
  Serial2.begin(9600);
  mp3_set_serial(Serial2);
  delay(1);
  mp3_single_loop(false);
  delay(1);
  mp3_set_EQ(0); // Bass
  delay(1);
  mp3_set_volume(30);
  delay(1);

  MSS.begin();
  moveToHome();

  // flag = true;

  //

  // caucas();

  // flying();

  pinMode(8, OUTPUT);

  Timer1.initialize(50000);
  Timer1.attachInterrupt(blue_interrupt);

  // Bt module
  Serial1.begin(38400);
}

int draw_idx = 0;
int draw_idx_prev = 0;

void draw(int index)
{
  u8g->firstPage();
  do
  {
    switch (index)
    {
      u8g->setColorIndex(1);
    case 0:
      u8g->drawXBMP(0, 0, u8g_logo_width, u8g_logo_height, face_1_bits);
      // u8g->drawStr(0, 0, "ODIN");
      break;
    case 1:
      u8g->drawXBMP(0, 0, u8g_logo_width, u8g_logo_height, face_2_bits);
      // u8g->drawStr(0, 0, "DWA");
      break;
    case 2:
      u8g->drawXBMP(0, 0, u8g_logo_width, u8g_logo_height, face_3_bits);
      // u8g->drawStr(0, 0, "TRI");
      break;
    default:
      break;
    }
  } while (u8g->nextPage());

  delay(1000);
}

void loop()
{

  // if(draw_idx_prev != draw_idx)
  //   delay(1000);

  if (bt_flag)
  {
    Serial.println("get command");
    switch (bt_cmd)
    {
    case connectedCommand:
      Serial.println("Connected");
      break;
    case servoReturnToDefaultCommand:
      Serial.println("home");
      servo_flag = false;
      moveToHome();
      break;
    case servoHelloCommand:
      Serial.println("hello");
      servo_flag = true;

      greeting();
      break;
    case servoDanceCommand:
      Serial.println("dance");
      servo_flag = true;

      caucas();
      break;
    case servoByeCommand:
      Serial.println("bye");
      servo_flag = true;

      goodb();
      break;
    case shlagbaumUp:
      Serial.println("shlagbaumUp");
      servo_flag = true;

      shlagbaum_up2();
      break;
    case shlagbaumDown:
      Serial.println("shlagbaumDown");
      servo_flag = true;

      shlagbaum_down2();
      break;
    case faceImage1Command:
      Serial.println("face1");
      draw(0);
      break;

    case faceImage2Command:
      Serial.println("face2");
      draw(1);
      break;
    
    case faceImage3Command:
      Serial.println("face3");
      draw(2);
      break;
      
    case audio1Command:
      Serial.println("audio1");
      //draw(1);
      mp3_stop();
      mp3_play(1);
      break;
      
    case audio2Command:
      Serial.println("audio2");
      //draw(0);
      mp3_stop();
      mp3_play(2);
      break;
    
    case audio3Command:
      Serial.println("audio3");
      //draw(0);
      mp3_stop();
      mp3_play(3);
      break;

    case 'L':
      Serial.println("gav");
      //draw(0);
      mp3_stop();
      mp3_play(4);
      break;
    
    case '%':
      Serial.println("stop music");
      mp3_stop();
      break;

    case 'v':
      mp3_set_volume(0);
      break;
    case 'b':
      mp3_set_volume(5);
      break;
    case 'n':
      mp3_set_volume(10);
      break;
    case 'm':
      mp3_set_volume(15);
      break;
    case '<':
      mp3_set_volume(20);
      break;

    case forwardCommand: // ВПЕРЕД
        md.setM1Speed(-max_speed);
        md.setM2Speed(-max_speed);
      break;
    case driveStopCommand: // СТОП
      md.setM1Speed(0);
      md.setM2Speed(0);
      break;
    case backwardCommand: // НАЗАД
      md.setM1Speed(max_speed);
      md.setM2Speed(max_speed);
      break;
    case driveLeftCommandHigh: // ВЛЕВО
      md.setM1Speed(-max_speed);
      md.setM2Speed(-half_speed);
      break;
    case driveRightCommandHigh: // ВПРАВО
      md.setM1Speed(-half_speed);
      md.setM2Speed(-max_speed);
      break;
    case centerCommand: //
      md.setM1Speed(0);
      md.setM2Speed(0);
      break;

    case getStr: // 
      

    default:
      break;
    }

    // switch(bt_cmd) {
    //   case '8': // ВПЕРЕД
    //     md.setM1Speed(-max_speed);
    //     md.setM2Speed(-max_speed);
    //     break;
    //   case '5': // СТОП
    //     md.setM1Speed(0);
    //     md.setM2Speed(0);
    //     break;
    //   case '2': // НАЗАД
    //     md.setM1Speed(max_speed);
    //     md.setM2Speed(max_speed);
    //     break;
    //   case '4': // ВЛЕВО
    //     md.setM1Speed(-max_speed);
    //     md.setM2Speed(-half_speed);
    //     break;
    //   case '6': // ВПРАВО
    //     md.setM1Speed(-half_speed);
    //     md.setM2Speed(-max_speed);
    //     break;

    //   case 'H':
    //     moveToHome(1);
    //     bt_cmd = ' ';
    //     break;

    //   // Подождите меня!
    //   /*case 'a':
    //     mp3_stop();
    //     mp3_play(1);
    //     bt_cmd = ' ';
    //     break;
    //   */

    //   case '+':
    //     zaryadka();
    //     bt_cmd = ' ';
    //     break;

    //   case '-':
    //     mp3_stop();
    //     mp3_set_volume(20);
    //     mp3_play(1);
    //     zaryadka();
    //     bt_cmd = ' ';
    //     break;

    //   case '&':
    //     mp3_stop();
    //     mp3_set_volume(20);
    //     mp3_play(1);
    //     shaloon();
    //     bt_cmd = ' ';
    //     break;

    //   // Подождите меня!
    //   case 'a':
    //     mp3_stop();
    //     mp3_play(1);
    //     bt_cmd = ' ';
    //     break;

    //   // Я хотел бы вас поздравить
    //   case 's':
    //     mp3_stop();
    //     mp3_play(2);
    //     i = RIGHT_SHOULDER_2_DEFAULT;
    //     j = LEFT_SHOULDER_2_DEFAULT;
    //     for(; i >= RIGHT_SHOULDER_2_MAX; i-=4, j+=4) {
    //       MSS.servoWrite(PIN_RIGHT_SHOULDER_2, i);
    //       delay(15);
    //       MSS.servoWrite(PIN_LEFT_SHOULDER_2, j);
    //     }
    //     delay(150);
    //     for(; i <= RIGHT_SHOULDER_2_MID_MAX; i+=4, j-=4) {
    //       MSS.servoWrite(PIN_RIGHT_SHOULDER_2, i);
    //       delay(15);
    //       MSS.servoWrite(PIN_LEFT_SHOULDER_2, j);
    //       delay(15);
    //     }
    //     for(; i >= RIGHT_SHOULDER_2_MAX; i-=4, j+=4) {
    //       MSS.servoWrite(PIN_RIGHT_SHOULDER_2, i);
    //       delay(15);
    //       MSS.servoWrite(PIN_LEFT_SHOULDER_2, j);
    //     }
    //     delay(150);
    //     for(; i <= RIGHT_SHOULDER_2_MID_MAX; i+=4, j-=4) {
    //       MSS.servoWrite(PIN_RIGHT_SHOULDER_2, i);
    //       delay(15);
    //       MSS.servoWrite(PIN_LEFT_SHOULDER_2, j);
    //       delay(15);
    //     }
    // delay(150);
    //     bt_cmd = ' ';
    //     break;

    //   // Дорогой Сергей Витальевич
    //   case 'd':
    //     mp3_stop();
    //     mp3_play(3);
    //     bt_cmd = ' ';
    //     break;

    //   // Лезгинка
    //   case 'f':
    //     mp3_stop();
    //     // mp3_play(4);
    //     caucas();
    //     bt_cmd = ' ';
    //     break;

    //   // Дорогие выпускники
    //   case 'g':
    //     mp3_stop();
    //     mp3_play(5);
    //     bt_cmd = ' ';
    //     break;

    //   // Стих
    //   case 'h':
    //     mp3_stop();
    //     mp3_play(6);
    //     for(i = LEFT_SHOULDER_1_DEFAULT, j = RIGHT_SHOULDER_1_DEFAULT; i >= 120; i-=4, j+=4) {
    //       MSS.servoWrite(PIN_LEFT_SHOULDER_1, i);
    //       delay(15);
    //       MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
    //       DELAY_AND_CHECK
    //     }
    //     bt_cmd = ' ';
    //     break;

    //   // Желаю вам удачи!
    //   case 'j':
    //     goodb();
    //     bt_cmd = ' ';
    //     break;

    //   // Все, я устал
    //   case 'k':
    //     mp3_stop();
    //     mp3_play(8);
    //     bt_cmd = ' ';
    //     break;

    //   case 'z':
    //     mp3_set_volume(5);
    //     break;
    //   case 'x':
    //     mp3_set_volume(10);
    //     break;
    //   case 'c':
    //     mp3_set_volume(15);
    //     break;
    //   case 'v':
    //     mp3_set_volume(20);
    //     break;
    //   case 'b':
    //     mp3_set_volume(25);
    //     break;
    //   case 'n':
    //     mp3_set_volume(30);
    //     break;
    //   case 'm':
    //     max_speed = 140;
    //     half_speed = 70;
    //     break;
    //   case '<':
    //     max_speed = 170;
    //     half_speed = 85;
    //     break;
    //   case '>':
    //     max_speed = 200;
    //     half_speed = 100;
    //     break;
    //   case '?':
    //     mp3_stop();
    //     bt_cmd = ' ';
    //     break;

    // }
    bt_flag = false;
  }
}
