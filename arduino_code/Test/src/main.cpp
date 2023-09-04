#include <Arduino.h>
#include "U8glib.h"
#include "DFRobotDFPlayerMini.h"
#include "lcd.cpp"
#include <TimerOne.h>
#include <TimerThree.h>
#include <TimerFour.h>
#include <Servo.h>
#include <SharpIR.h>

//==== MILLISTIMER MACRO ====
#define EVERY_MS(x)                  \
  static uint32_t tmr;               \
  bool flag = millis() - tmr >= (x); \
  if (flag)                          \
    tmr += (x);                      \
  if (flag)
//===========================


#define SERVO1 11
#define SERVO2 12

Servo servo1;
Servo servo2;

U8GLIB_ST7920_128X64_4X u8g(
    5, 6, 7, 8, 9, 10, 24, 25,
    4, 2, 3);

#define SER 20
#define LATCH 22
#define CLK 21

#define IR_SENSOR A0
SharpIR sensor( SharpIR::GP2Y0A02YK0F, IR_SENSOR );
int distance;

DFRobotDFPlayerMini myDFPlayer;
#define PIN_IN1 38    // Вывод управления направлением вращения мотора №1
#define PIN_IN2 39    // Вывод управления направлением вращения мотора №1
#define PIN_IN3 40    // Вывод управления направлением вращения мотора №2
#define PIN_IN4 41    // Вывод управления направлением вращения мотора №2
uint8_t power1 = 105; // Значение ШИМ (или скорости вращения)
uint8_t power2 = 105;
uint8_t led = 1;
uint8_t mode2_flag = 1;

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
    if(rpi_msg != '\0')
      rpi_msg_flag = 1;
  }

  // distance = sensor.getDistance();
  // Serial.print("Distance is ");
  // Serial.println(distance);
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

  servo1.write(90);
  servo2.write(90);
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

    // u8g.setFont(u8g_font_unifont);
    // u8g.setPrintPos(0, 20);
    // u8g.setColorIndex(1);
    // u8g.print("Hello World!");
  } while (u8g.nextPage());
  delay(100);
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

  if (!myDFPlayer.begin(Serial1)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println("DFPlayer Mini online.");

  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  // myDFPlayer.enableLoopAll();
  myDFPlayer.volume(15);

  init_chocker();
  change_chocker_anim(ChockerEnums::Mode2);

  init_bt();

  init_servo();


  draw_lcd(klas1_bits);

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

  if(bt_msg_flag) {
    Serial.print("bt: ");
    Serial.println(bt_msg);

  // Servo
  switch (bt_msg)
  {
  case 'q':
    servo1.write(75);
    break;
  case 'w':
    servo1.write(90);
    break;

  // Choker
  case 'a':
    change_chocker_anim(ChockerEnums::Mode1);
    break;
  case 's':
    change_chocker_anim(ChockerEnums::Mode2);
    break;
  case 'd':
    change_chocker_anim(ChockerEnums::Mode3);
    break;

  // Player
  case '0':
    myDFPlayer.volume(0);
    break;
  case '1':
    myDFPlayer.volume(5);
    break;
  case '2':
    myDFPlayer.volume(10);
    break;
  case '3':
    myDFPlayer.volume(15);
    break;
  case '4':
    myDFPlayer.volume(20);
    break;
  case '5':
    myDFPlayer.volume(25);
    break;
  case '6':
    myDFPlayer.volume(30);
    break;
  case 'b':
    myDFPlayer.play(1);
    break;
  case 'n':
    myDFPlayer.play(2);
    break;
  case 'm':
    myDFPlayer.play(3);
    break;

  // LCD
  case 'y':
    draw_lcd(klas1_bits);
    break;
  case 'u':
    draw_lcd(svin_bits);
    break;
  case 'i':
    draw_lcd(sonic_bits);
    break;


  default:
    break;
  }

    bt_msg_flag = 0;
  }

  if(rpi_msg_flag) {
    Serial.print("rpi: ");
    Serial.println(rpi_msg);

    switch (rpi_msg)
    {
    case 'o':
      Serial3.println("ORANGE");
      break;
    case 'g':
      Serial3.println("GREEN");
      break;
    case 'b':
      Serial3.println("BLUE");
      break;
    
    default:
      break;
    }
    rpi_msg_flag = 0;
  }
  
}
