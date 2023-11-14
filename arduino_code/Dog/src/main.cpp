#include <Arduino.h>
#include "U8glib.h"
#include "DFRobotDFPlayerMini.h"
#include "lcd.cpp"
#include <TimerOne.h>
#include <TimerThree.h>
#include <TimerFour.h>
#include <Servo.h>
#include <SharpIR.h>
#include <L298N.h>

#include "bt_commands.cpp"
#include "rus_font.h"

//==== MILLISTIMER MACRO ====
#define EVERY_MS(x)                  \
  static uint32_t tmr;               \
  bool flag = millis() - tmr >= (x); \
  if (flag)                          \
    tmr += (x);                      \
  if (flag)
//===========================

#define SERVO1 11
#define SERVO2 35
#define SERVO_DEFAULT 86
Servo servo1;
Servo servo2;

U8GLIB_ST7920_128X64_4X u8g(
    5, 6, 7, 8, 9, 10, 24, 25,
    4, 2, 3);

#define SER 20
#define LATCH 22
#define CLK 21

#define IR_SENSOR_F A0
// #define IR_SENSOR_L A1
// #define IR_SENSOR_R A2

SharpIR sensorF(SharpIR::GP2Y0A02YK0F, IR_SENSOR_F);
// SharpIR sensorL(SharpIR::GP2Y0A21YK0F, IR_SENSOR_L);
// SharpIR sensorR(SharpIR::GP2Y0A21YK0F, IR_SENSOR_R);

int distanceF, distanceL, distanceR;

DFRobotDFPlayerMini myDFPlayer;

#define PIN_IN1 38    // Вывод управления направлением вращения мотора №1
#define PIN_IN2 39    // Вывод управления направлением вращения мотора №1
#define PIN_IN3 40    // Вывод управления направлением вращения мотора №2
#define PIN_IN4 41    // Вывод управления направлением вращения мотора №2

// Create motor instances
L298N motor_left(PIN_IN1, PIN_IN2);
L298N motor_right(PIN_IN3, PIN_IN4);


uint8_t power1 = 105; // Значение ШИМ (или скорости вращения)
uint8_t power2 = 105;
uint8_t led = 1;
uint8_t mode2_flag = 1;

// bool gav_gav_flag = true;
// bool no_no_flag = true;
// bool yes_yes_flag = true;
// unsigned long gav_gav_timeout = 0;

bool stage_1 = false;
bool stage_2 = false;
bool stage_3 = false;
bool stage_4 = false;
bool stage_5 = false;
bool stage_6 = false;

namespace ChockerEnums
{
  enum
  {
    Mode1 = 0,
    Mode2 = 1,
    Mode3 = 2,
    Mode4 = 3
  };
}

int current_chocker_mode = ChockerEnums::Mode1;

void blink_choker()
{
  switch (current_chocker_mode)
  {
  case ChockerEnums::Mode1:
    digitalWrite(LATCH, 0);
    shiftOut(SER, CLK, MSBFIRST, led);
    digitalWrite(LATCH, 1);
    if (led < 127)
      led = led << 1;
    else
      led = 1;
    break;
  case ChockerEnums::Mode2:
    for (int i = 0; i < 2; i++)
    {
      digitalWrite(LATCH, 0);
      shiftOut(SER, CLK, MSBFIRST, led);
      digitalWrite(LATCH, 1);
    }
    if (led > 0)
      led = led >> 1;
    else
      led = 4;

    break;
  case ChockerEnums::Mode3:
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(LATCH, 0);
      shiftOut(SER, CLK, MSBFIRST, led);
      digitalWrite(LATCH, 1);
    }
    if (led > 0)
      led = led >> 1;
    else
      led = 4;

    break;
  case ChockerEnums::Mode4:
    digitalWrite(LATCH, 0);
    shiftOut(SER, CLK, MSBFIRST, led);
    digitalWrite(LATCH, 1);
    if (led == 170)
      led = (led << 1) + 1;
    else
      led = 170;
    break;

  default:
    break;
  }
}

void change_chocker_anim(int mode)
{
  Timer3.detachInterrupt();
  current_chocker_mode = mode;
  switch (current_chocker_mode)
  {
  case ChockerEnums::Mode1:
    Timer3.setPeriod(200000);
    led = 1;
    break;
  case ChockerEnums::Mode2:
    Timer3.setPeriod(400000);
    led = 4;
    break;
  case ChockerEnums::Mode3:
    Timer3.setPeriod(200000);
    led = 170;
    break;
  case ChockerEnums::Mode4:
    Timer3.setPeriod(200000);
    led = 170;
    break;
  default:
    break;
  }
  Timer3.attachInterrupt(blink_choker);
}

void init_chocker()
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(LATCH, 0);
    shiftOut(SER, CLK, MSBFIRST, 0);
    digitalWrite(LATCH, 1);
  }
  Timer3.initialize(300000);
}

uint8_t bt_msg_flag = 0;
uint8_t rpi_msg_flag = 0;

char bt_msg = ' ';
char rpi_msg = ' ';

const int NUM_READ = 10;

int runMiddleArifmOptim(int newVal) {
  static int t = 0;
  static int vals[NUM_READ];
  static int average = 0;
  if (++t >= NUM_READ) t = 0; // перемотка t
  average -= vals[t];         // вычитаем старое
  average += newVal;          // прибавляем новое
  vals[t] = newVal;           // запоминаем в массив
  return average / NUM_READ;
}

void draw_lcd(unsigned char *bits);

void bt_reader()
{
  if (Serial3.available())
  {
    bt_msg = (char)Serial3.read();
    bt_msg_flag = 1;
  }

  if (Serial2.available())
  {
    rpi_msg = (char)Serial2.read();
    if (rpi_msg != '\0')
      rpi_msg_flag = 1;
  }

  // distanceF = runMiddleArifmOptim(sensorF.getDistance());
  // distanceL = runMiddleArifmOptim(sensorL.getDistance());
  // distanceR = runMiddleArifmOptim(sensorR.getDistance());

  // distanceF = sensorF.getDistance();
  // distanceL = sensorL.getDistance();
  // distanceR = sensorR.getDistance();

  // Serial.println("front" + String(distanceF));
  // Serial.println("left" + String(distanceL));
  // Serial.println("right" + String(distanceR));

  // if(distanceF > 99) {
  //   Serial3.print("F"+String(distanceF));
  // } else {
  //   Serial3.print("F0"+String(distanceF));
  //   gav_gav_flag = true;
  // }
  // if(distanceL < 10) {
  //   Serial3.print("L0"+String(distanceL));
  // } else {
  //   Serial3.print("L"+String(distanceL));
  // }
  // if(distanceL < 10) {
  //   Serial3.print("R0"+String(distanceR));
  // } else {
  //   Serial3.print("R"+String(distanceR));
  // }
}

void init_bt()
{
  Serial3.begin(9600);
  Timer1.initialize(50000);
  Timer1.attachInterrupt(bt_reader);
}

void init_servo()
{
  servo1.attach(SERVO1);
  servo2.attach(SERVO2);

  servo1.write(SERVO_DEFAULT);
  servo2.write(SERVO_DEFAULT);
}

void init_rpi()
{
  Serial2.begin(9600);
  Serial2.setTimeout(100);
  Timer4.initialize(400000);
}

void draw_lcd(unsigned char *bits)
{
  u8g.firstPage();
  do
  {
    u8g.setColorIndex(1);
    u8g.drawBox(0, 0, 128, 64);
    u8g.setColorIndex(0);
    u8g.drawXBMP(0, 0, lcd_width, lcd_height, bits);
  } while (u8g.nextPage());
  delay(100);
}

void draw_stages()
{
  u8g.firstPage();
  do
  {
    // u8g.setColorIndex(1);
    // u8g.drawBox(0, 0, 128, 64);

    u8g.setColorIndex(1);
    u8g.setFont(rus6x12);

    int p_y = 7;

    u8g.setPrintPos(2, p_y);
    u8g.print("1 Перехват радио");
    u8g.print((stage_1 ? "    V" : ""));
    p_y += 11;

    u8g.setPrintPos(2, p_y);
    u8g.print(" 2 Танк подбит");
    u8g.print((stage_2 ? "      V" : ""));
    p_y += 11;

    u8g.setPrintPos(2, p_y);
    u8g.print("  3 Лабиринт 1");
    u8g.print((stage_3 ? "      V" : ""));
    p_y += 11;

    u8g.setPrintPos(2, p_y);
    u8g.print("   4 Лабиринт 2");
    u8g.print((stage_4 ? "     V" : ""));
    p_y += 11;

    u8g.setPrintPos(2, p_y);
    u8g.print("    5 Шлагбаум");
    u8g.print((stage_5 ? "      V" : ""));
    p_y += 11;

    u8g.setPrintPos(2, p_y);
    u8g.print("      6 Финиш");
    u8g.print((stage_6 ? "       V" : ""));

    // u8g.setColorIndex(0);
    // u8g.drawXBMP(0, 0, lcd_width, lcd_height, bits);
  } while (u8g.nextPage());
  delay(150);
}

void init_motors()
{
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
}

void stop_motors()
{
  analogWrite(PIN_IN1, 0);
  analogWrite(PIN_IN2, 0);
  analogWrite(PIN_IN3, 0);
  analogWrite(PIN_IN4, 0);
}

inline void test_motors()
{
  {EVERY_MS(1000){

      if (power1 < 255){
          analogWrite(PIN_IN1, power1);
  analogWrite(PIN_IN2, 0);
  power1 += 30; // Увеличиваем скорость
}
else
{
  power1 = 0;
}
}
}
{
  EVERY_MS(1000)
  {
    if (power2 < 255)
    {
      // analogWrite(PIN_IN2, power2);
      analogWrite(PIN_IN3, power2);

      // analogWrite(PIN_IN1, 0);
      analogWrite(PIN_IN4, 0);
      power2 += 30; // Уменьшаем скорость
    }
    else
    {
      power2 = 0;
    }
  }
}
}

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);

  if (!myDFPlayer.begin(Serial1))
  { // Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
      ;
  }
  Serial.println("DFPlayer Mini online.");

  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  // myDFPlayer.enableLoopAll();
  myDFPlayer.volume(15);

  init_chocker();
  change_chocker_anim(ChockerEnums::Mode2);

  init_bt();

  init_servo();

  draw_stages();

  // draw_lcd(face_1_bits);

  init_rpi();

  init_motors();
  stop_motors();

  pinMode(SER, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);

  Serial.println("Full init done.");
}

void loop()
{
  //test_motors();
  
  // draw_lcd(face_2_bits);

//   if (millis() - gav_gav_timeout > 1000){ // Вместо 10000 подставьте нужное вам значение паузы 
//     gav_gav_timeout = millis(); 
//     // gav_gav_flag = true;
//     no_no_flag = true;
//     yes_yes_flag = true;
//  }

//  if(gav_gav_flag) {
//     myDFPlayer.volume(20);
//     myDFPlayer.play(4);
//     gav_gav_flag = false;
//     // myDFPlayer.volume(10);
//   }

  bool stages_updated = false;

  if (bt_msg_flag)
  {
    Serial.print("bt: ");
    Serial.println(bt_msg);

    // Servo
    switch (bt_msg)
    {
    case '(':
      servo1.attach(SERVO1);
      servo1.write(66);
      delay(500);
      servo1.detach();
      break;
    case ')':
      servo1.attach(SERVO1);
      servo1.write(106);
      delay(500);
      servo1.detach();
      break;
    case '+':
      servo2.attach(SERVO2);
      servo2.write(66);
      delay(500);
      servo2.detach();
      break;
    case '-':
      servo2.attach(SERVO2);
      servo2.write(106);
      delay(500);
      servo2.detach();
      break;
    case '_':
      servo1.attach(SERVO1);
      servo2.attach(SERVO2);
      servo1.write(SERVO_DEFAULT);
      servo2.write(SERVO_DEFAULT);
      delay(500);
      servo1.detach();
      servo2.detach();
      break;

    // Choker
    // case 'z':
    //   change_chocker_anim(ChockerEnums::Mode1);
    //   break;
    // case 'x':
    //   change_chocker_anim(ChockerEnums::Mode2);
    //   break;
    // case 'c':
    //   change_chocker_anim(ChockerEnums::Mode3);
    //   break;

    // Player
    // case 'v':
    //   myDFPlayer.volume(0);
    //   break;
    // case 'b':
    //   myDFPlayer.volume(5);
    //   break;
    // case 'n':
    //   myDFPlayer.volume(10);
    //   break;
    // case 'm':
    //   myDFPlayer.volume(15);
    //   break;
    // case '<':
    //   myDFPlayer.volume(20);
    //   break;
    case '!':
      myDFPlayer.play(1);
      break;
    case '@':
      myDFPlayer.play(2);
      break;
    case '#':
      myDFPlayer.play(3);
      break;
    case '%':
      myDFPlayer.stop();
      break;

    // LCD
    case 'o':
      // draw_lcd(face_1_bits);
      break;
    case 'p':
      // draw_lcd(face_2_bits);
      break;
    case 'a':
      // draw_lcd(face_3_bits);
      break;

    // Racing
    case 'q': // ВПЕРЕД
      motor_left.forward();
      motor_right.forward();
      break;
    case 'e': // СТОП
    case '9': // СТОП
      motor_left.stop();
      motor_right.stop();
      break;
    case 'w': // НАЗАД
      motor_left.backward();
      motor_right.backward();
      break;
    case '3': // ВЛЕВО
    case '4': // ВЛЕВО СИЛЬНО
      //motor_left.setSpeed(128);
      motor_left.stop();
      motor_right.forward();
      break;
    case '7': // ВПРАВО
    case '8': // ВПРАВО СИЛЬНО
      motor_left.forward();
      motor_right.stop();
      break;
    
    case stage1CommandOn:
      stage_1 = true;
      stages_updated = true;
      break;
    case stage1CommandOff:
      stage_1 = false;
      stages_updated = true;
      break;
    
    case stage2CommandOn:
      stage_2 = true;
      stages_updated = true;
      break;
    case stage2CommandOff:
      stage_2 = false;
      stages_updated = true;
      break;

    case stage3CommandOn:
      stage_3 = true;
      stages_updated = true;
      break;
    case stage3CommandOff:
      stage_3 = false;
      stages_updated = true;
      break;
    
    case stage4CommandOn:
      stage_4 = true;
      stages_updated = true;
      break;
    case stage4CommandOff:
      stage_4 = false;
      stages_updated = true;
      break;
    
    case stage5CommandOn:
      stage_5 = true;
      stages_updated = true;
      break;
    case stage5CommandOff:
      stage_5 = false;
      stages_updated = true;
      break;
    
    case stage6CommandOn:
      stage_6 = true;
      stages_updated = true;
      break;
    case stage6CommandOff:
      stage_6 = false;
      stages_updated = true;
      break;

    case stageAllCommandOff:
      stage_1 = false;
      stage_2 = false;
      stage_3 = false;
      stage_4 = false;
      stage_5 = false;
      stage_6 = false;
      stages_updated = true;
      break;

    default:
      break;
    }

    bt_msg_flag = 0;
  }

  if(stages_updated) {
    draw_stages();
  }

  if (rpi_msg_flag)
  {
    Serial.print("rpi: ");
    Serial.println(rpi_msg);

    switch (rpi_msg)
    {
    case 'o':
      Serial3.print("O");
      // if(gav_gav_flag) {
      //   myDFPlayer.volume(20);
      //   myDFPlayer.play(4);
      //   gav_gav_flag = false;
      //   // myDFPlayer.volume(10);
      // }
      break;
    case 'g':
      Serial3.print("G");
      // if(no_no_flag){
      //   servo2.attach(SERVO2);
      //   servo2.write(SERVO_DEFAULT-20);
      //   delay(250);
      //   // servo2.write(90);
      //   // delay(250);
      //   servo2.write(SERVO_DEFAULT+20);
      //   delay(250);
      //   servo2.write(SERVO_DEFAULT);
      //   delay(250);
      //   servo2.detach();
      //   no_no_flag = false;
      // }
      break;
    case 'b':
      Serial3.print("B");
      // if(yes_yes_flag){
      //   servo1.attach(SERVO1);
      //   servo1.write(SERVO_DEFAULT-20);
      //   delay(250);
      //   // servo2.write(90);
      //   // delay(250);
      //   servo1.write(SERVO_DEFAULT+20);
      //   delay(250);
      //   servo1.write(SERVO_DEFAULT);
      //   delay(250);
      //   servo1.detach();
      //   yes_yes_flag = false;
      // }
      break;

    default:
      break;
    }
    rpi_msg_flag = 0;
  }
}
