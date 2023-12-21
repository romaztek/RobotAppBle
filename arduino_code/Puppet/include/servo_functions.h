#include "servo_defines.h"

#include "iarduino_MultiServo.h"
#include <SoftwareSerial.h>

iarduino_MultiServo MSS;

int speed = 1;
volatile bool flag;

int i, j;

int left_shoulder_1_value = -1;
int right_shoulder_1_value = -1;
int left_shoulder_2_value = -1;
int right_shoulder_2_value = -1;
int left_hand_1_value = -1;
int right_hand_1_value = -1;

void moveToHome(bool fast = 0) {  
  
    MSS.servoWrite(PIN_NECK, NECK_DEFAULT);             
    delay(15);
    delay(500);
    
    MSS.servoWrite(PIN_HEAD, HEAD_DEFAULT);             
    delay(15);
    delay(500);
    
    MSS.servoWrite(PIN_BELT, BELT_DEFAULT);
    delay(15);
    delay(500);
    
    MSS.servoWrite(PIN_SPINE, SPINE_DEFAULT);
    delay(15);
    delay(500);
    
    MSS.servoWrite(PIN_RIGHT_SHOULDER_2, RIGHT_SHOULDER_2_DEFAULT);             
    delay(15);
    MSS.servoWrite(PIN_LEFT_SHOULDER_2, LEFT_SHOULDER_2_DEFAULT);             
    delay(15);
    delay(500);
    
    MSS.servoWrite(PIN_RIGHT_HAND_1, RIGHT_HAND_1_DEFAULT);
    delay(15);
    MSS.servoWrite(PIN_LEFT_HAND_1, LEFT_HAND_1_DEFAULT);
    delay(15);
    delay(500);
    
    MSS.servoWrite(PIN_RIGHT_HAND_2, RIGHT_HAND_2_DEFAULT);
    delay(15);
    MSS.servoWrite(PIN_LEFT_HAND_2, LEFT_HAND_2_DEFAULT);
    delay(15);
    delay(500);
    
    MSS.servoWrite(PIN_LEFT_SHOULDER_1, LEFT_SHOULDER_1_DEFAULT);             
    delay(15);
    MSS.servoWrite(PIN_RIGHT_SHOULDER_1, RIGHT_SHOULDER_1_DEFAULT);             
    delay(15);
    delay(500);
    
    MSS.servoWrite(PIN_LEFT_SHOULDER_1, LEFT_SHOULDER_1_DEFAULT);             
    delay(15);
    MSS.servoWrite(PIN_RIGHT_SHOULDER_1, RIGHT_SHOULDER_1_DEFAULT);             
    delay(15);
    delay(500);
    
      
    left_shoulder_2_value = LEFT_SHOULDER_2_DEFAULT;
    right_shoulder_2_value = LEFT_SHOULDER_2_DEFAULT;
    
    right_hand_1_value = RIGHT_HAND_1_DEFAULT;
    left_hand_1_value = LEFT_HAND_1_DEFAULT;
    flag = false;
    
    Serial.print("move to home done");
    Serial1.write('R');
}



void test_r_hand() {
  for(i = 0; i <= 150; i++) {
    MSS.servoWrite(PIN_RIGHT_HAND_1, i);
    delay(15);
    if(!flag) return;
  }
  for(i = 75; i >= 0; i--) {
    MSS.servoWrite(PIN_RIGHT_HAND_1, i);
    delay(15);
    if(!flag) return;
  }
}

void test_l_hand() {
  for(i = 0; i <= 150; i++) {
    MSS.servoWrite(PIN_LEFT_HAND_1, i);
    delay(15);
    if(!flag) return;
  }
  for(i = 150; i >= 0; i--) {
    MSS.servoWrite(PIN_LEFT_HAND_1, i);
    delay(15);
    if(!flag) return;
  }
}

void zaryadka() {
  while(1) {
    for(i = 180, j = 0; i >= 100; i--, j++) {
      MSS.servoWrite(PIN_LEFT_SHOULDER_1, i);
      MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
      delay(15);
      if(!flag) return;
    }
    delay(300);
    for(i = 100, j = 80; i <= 180; i++, j--) {
      MSS.servoWrite(PIN_LEFT_SHOULDER_1, i);
      MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
      delay(15);
      if(!flag) return;
    }
    delay(300);
  }
}


void hello() {
  while(1) {
    for(i = LEFT_SHOULDER_1_DEFAULT, j = RIGHT_SHOULDER_1_DEFAULT; i >= 80; i-=4, j+=4) {
      MSS.servoWrite(PIN_LEFT_SHOULDER_1, i);
      delay(15);
      MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
      DELAY_AND_CHECK
    }
    
    for(i = RIGHT_HAND_2_DEFAULT, j = LEFT_HAND_2_DEFAULT; i < 110; i+=4, j-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      delay(15);
      MSS.servoWrite(PIN_LEFT_HAND_2, j);
      DELAY_AND_CHECK
    }
    
    delay(400);
    
    for(i = 110; i >= 0; i-=4, j+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      delay(15);
      MSS.servoWrite(PIN_LEFT_HAND_2, j);
      DELAY_AND_CHECK
    }
    
    for(i = 80, j = 100; i <= 180; i+=4, j-=4) {
      MSS.servoWrite(PIN_LEFT_SHOULDER_1, i);
      delay(15);
      MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
      DELAY_AND_CHECK
    }
    delay(400);
  }
}

void flying(bool fast = 0) {  
  speed = fast ? 2 : 1;
  i = RIGHT_SHOULDER_2_DEFAULT;
  j = LEFT_SHOULDER_2_DEFAULT;
  
  mp3_stop();
  mp3_set_volume(20);
  mp3_play(1);
    
  while(1) {
    for(; i >= RIGHT_SHOULDER_2_MAX; i-=4, j+=4) {
        MSS.servoWrite(PIN_RIGHT_SHOULDER_2, i);
        delay(15);
        MSS.servoWrite(PIN_LEFT_SHOULDER_2, j);
        DELAY_AND_CHECK
    }
  
    delay(150);
    
    
    for(; i <= RIGHT_SHOULDER_2_MID_MAX; i+=4, j-=4) {
        MSS.servoWrite(PIN_RIGHT_SHOULDER_2, i);
        delay(15);
        MSS.servoWrite(PIN_LEFT_SHOULDER_2, j);
        DELAY_AND_CHECK
    }
    delay(150);
  }
  
}

void belt_move() {
  while(1) {
      for(i = BELT_DEFAULT; i <= BELT_MAX; i++) {
        MSS.servoWrite(PIN_BELT, i);          
        DELAY_AND_CHECK
      }
      for(; i >= BELT_DEFAULT; i--) {
        MSS.servoWrite(PIN_BELT, i);          
        DELAY_AND_CHECK
      }
      for(; i >= BELT_MIN; i--) {
        MSS.servoWrite(PIN_BELT, i);          
        DELAY_AND_CHECK
      }
      
      for(; i <= BELT_DEFAULT; i++) {
        MSS.servoWrite(PIN_BELT, i);          
        DELAY_AND_CHECK
      }
  }
}

void neck_move() {
  while(1) {
    for(i = NECK_DEFAULT; i >= NECK_MIN; i--) {
      MSS.servoWrite(PIN_NECK, i);   
      DELAY_AND_CHECK
    }
    for(; i <= NECK_DEFAULT; i++) {
      MSS.servoWrite(PIN_NECK, i);   
      DELAY_AND_CHECK
    }
    for(; i <= NECK_MAX; i++) {
      MSS.servoWrite(PIN_NECK, i);   
      DELAY_AND_CHECK
    }
    for(; i >= NECK_DEFAULT; i--) {
      MSS.servoWrite(PIN_NECK, i);   
      DELAY_AND_CHECK
    }
  }
}

void head_move() {
  while(1) {
    for(i = HEAD_DEFAULT; i >= HEAD_MIN; i--) {
      MSS.servoWrite(PIN_HEAD, i);   
      DELAY_AND_CHECK
    }
    for(; i <= HEAD_DEFAULT; i++) {
      MSS.servoWrite(PIN_HEAD, i);   
      DELAY_AND_CHECK
    }
    for(; i <= HEAD_MAX; i++) {
      MSS.servoWrite(PIN_HEAD, i);   
      DELAY_AND_CHECK
    }
    for(; i >= HEAD_DEFAULT; i--) {
      MSS.servoWrite(PIN_HEAD, i);   
      DELAY_AND_CHECK
    }
  }
}


void spine_move() {
  while(1) {
    for(i = SPINE_DEFAULT; i >= SPINE_DEFAULT - 25; i--) {
      MSS.servoWrite(PIN_SPINE, i);
      DELAY_AND_CHECK
    }
    for(i = SPINE_DEFAULT - 25; i <= SPINE_DEFAULT; i++) {
      MSS.servoWrite(PIN_SPINE, i);
      DELAY_AND_CHECK
    }
    delay(500);
    for(i = SPINE_DEFAULT; i <= SPINE_DEFAULT + 25; i++) {
      MSS.servoWrite(PIN_SPINE, i);
      DELAY_AND_CHECK
    }
    for(i = SPINE_DEFAULT + 25; i >= SPINE_DEFAULT; i--) {
      MSS.servoWrite(PIN_SPINE, i);
      DELAY_AND_CHECK
    }
    delay(500);
  }
}

void pinguin() {
  int d = 0;
  while(d < 16) {
  for(i = SPINE_DEFAULT; i >= SPINE_DEFAULT - 15; i--) {
      MSS.servoWrite(PIN_SPINE, i);
      delay(10);
      DELAY_AND_CHECK
    }
    for(i = SPINE_DEFAULT - 15; i <= SPINE_DEFAULT; i++) {
      MSS.servoWrite(PIN_SPINE, i);
      delay(10);
      DELAY_AND_CHECK
    }
    delay(500);
    for(i = SPINE_DEFAULT; i <= SPINE_DEFAULT + 15; i++) {
      MSS.servoWrite(PIN_SPINE, i);
      delay(10);
      DELAY_AND_CHECK
    }
    for(i = SPINE_DEFAULT + 15; i >= SPINE_DEFAULT; i--) {
      MSS.servoWrite(PIN_SPINE, i);
      delay(10);
      DELAY_AND_CHECK
    }
    delay(500);
    d++;
  }
  
}

void greeting() {
    for(i = LEFT_SHOULDER_1_DEFAULT, j = RIGHT_SHOULDER_1_DEFAULT; i >= 80; i-=2, j+=2) {
      MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
      DELAY_AND_CHECK
    }
    
    for(i = RIGHT_HAND_2_DEFAULT, j = LEFT_HAND_2_DEFAULT; i <= 110; i+=2, j-=2) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      DELAY_AND_CHECK
    }

    // машет правой рукой
    for(i = RIGHT_HAND_1_DEFAULT; i <= RIGHT_HAND_1_MIN; i+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i >= RIGHT_HAND_1_DEFAULT; i-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i >= RIGHT_HAND_1_MAX; i-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i <= RIGHT_HAND_1_DEFAULT; i+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    // машет правой рукой
    for(i = RIGHT_HAND_1_DEFAULT; i <= RIGHT_HAND_1_MIN; i+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i >= RIGHT_HAND_1_DEFAULT; i-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i >= RIGHT_HAND_1_MAX; i-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i <= RIGHT_HAND_1_DEFAULT; i+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    // опустить руку

    for(i = 110; i >= RIGHT_HAND_2_DEFAULT; i-=2) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      DELAY_AND_CHECK
    }

    for(i = 80, j = RIGHT_SHOULDER_1_DEFAULT; i <= LEFT_SHOULDER_1_DEFAULT; i+=2, j-=2) {
      MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
      DELAY_AND_CHECK
    }
    
}

void shlagbaum_down() 
{
    // for(i = LEFT_SHOULDER_1_DEFAULT, j = RIGHT_SHOULDER_1_DEFAULT; i >= 80; i-=2, j+=2) {
    //   MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
    //   DELAY_AND_CHECK
    // }
    // опустить кисть
    for(i = 60; i <= 110; i+=2) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      DELAY_AND_CHECK
    }
    
}

void shlagbaum_up() {
    // поднять кисть
    for(i = 110; i >= 60; i-=2) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      DELAY_AND_CHECK
    }

    // for(i = 80, j = RIGHT_SHOULDER_1_DEFAULT; i <= LEFT_SHOULDER_1_DEFAULT; i+=2, j-=2) {
    //   MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
    //   DELAY_AND_CHECK
    // }
}

void goodb() {    
    for(i = LEFT_SHOULDER_1_DEFAULT, j = RIGHT_SHOULDER_1_DEFAULT; i >= 80; i-=2, j+=2) {
      MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
      DELAY_AND_CHECK
    }
    
    for(i = RIGHT_HAND_2_DEFAULT, j = LEFT_HAND_2_DEFAULT; i <= 110; i+=2, j-=2) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      DELAY_AND_CHECK
    }

    // машет правой рукой
    for(i = RIGHT_HAND_1_DEFAULT; i <= RIGHT_HAND_1_MIN; i+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }
    

    for(; i >= RIGHT_HAND_1_DEFAULT; i-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i >= RIGHT_HAND_1_MAX; i-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i <= RIGHT_HAND_1_DEFAULT; i+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    // машет правой рукой
    for(i = RIGHT_HAND_1_DEFAULT; i <= RIGHT_HAND_1_MIN; i+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i >= RIGHT_HAND_1_DEFAULT; i-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i >= RIGHT_HAND_1_MAX; i-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i <= RIGHT_HAND_1_DEFAULT; i+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    // опустить руку

    for(i = 110; i >= RIGHT_HAND_2_DEFAULT; i-=2) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      DELAY_AND_CHECK
    }

    for(i = 80, j = RIGHT_SHOULDER_1_DEFAULT; i <= LEFT_SHOULDER_1_DEFAULT; i+=2, j-=2) {
      MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
      DELAY_AND_CHECK
    }
    
}

void i_am_jopa()
{
    for(i = RIGHT_SHOULDER_2_DEFAULT, j = LEFT_SHOULDER_2_DEFAULT; i >= RIGHT_SHOULDER_2_MID_MAX; i-=4, j+=4) {
        MSS.servoWrite(PIN_RIGHT_SHOULDER_2, i);
        delay(15);
        MSS.servoWrite(PIN_LEFT_SHOULDER_2, j);
        DELAY_AND_CHECK
    }
  
    delay(150);
    
    for(; i <= RIGHT_SHOULDER_2_DEFAULT; i+=2, j-=2) {
        MSS.servoWrite(PIN_RIGHT_SHOULDER_2, i);
        delay(15);
        MSS.servoWrite(PIN_LEFT_SHOULDER_2, j);
        DELAY_AND_CHECK
    }
    delay(150);
}

void may_i_greet()
{
    for(i = NECK_DEFAULT; i >= 52; i--) {
      MSS.servoWrite(PIN_NECK, i);   
      DELAY_AND_CHECK
    }

    for(i = HEAD_DEFAULT; i >= 15; i--) {
     MSS.servoWrite(PIN_HEAD, i);   
      DELAY_AND_CHECK
    }

    
    for(; i <= 25; i++) {
     MSS.servoWrite(PIN_HEAD, i);   
      DELAY_AND_CHECK
    }

    
    mp3_stop();
    mp3_play(4);


    delay(6900);

    for(i; i <= 46; i++) {
     MSS.servoWrite(PIN_HEAD, i);   
      DELAY_AND_CHECK
    }

    for(i = 52; i <= NECK_DEFAULT; i++) {
      MSS.servoWrite(PIN_NECK, i);   
      DELAY_AND_CHECK
    }
      
    /*for(; i <= NECK_DEFAULT; i++) {
      MSS.servoWrite(PIN_NECK, i);   
      DELAY_AND_CHECK
    }*/
    /*for(; i <= NECK_MAX; i++) {
      MSS.servoWrite(PIN_NECK, i);   
      DELAY_AND_CHECK
    }
    for(; i >= NECK_DEFAULT; i--) {
      MSS.servoWrite(PIN_NECK, i);   
      DELAY_AND_CHECK
    }*/
}


void shaloon()
{
    for(i = RIGHT_HAND_2_DEFAULT, j = LEFT_HAND_2_DEFAULT; i <= 110; i+=2, j-=2) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      MSS.servoWrite(PIN_LEFT_HAND_2, j);
      DELAY_AND_CHECK
    }

    /*for(i = RIGHT_HAND_1_MAX; i <= RIGHT_HAND_1_DEFAULT; i+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }*/
    
    for(int f = 0; f < 2; f++) {
      for(i = 180, j = 0; i >= 100; i--, j++) {
        MSS.servoWrite(PIN_LEFT_SHOULDER_1, i);
        MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
        delay(15);
        if(!flag) return;
      }
      delay(300);
      for(i = 100, j = 80; i <= 180; i++, j--) {
        MSS.servoWrite(PIN_LEFT_SHOULDER_1, i);
        MSS.servoWrite(PIN_RIGHT_SHOULDER_1, j);
        delay(15);
        if(!flag) return;
      }
      delay(300);
    }
    
    /*for(; i >= RIGHT_HAND_1_DEFAULT; i-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i >= RIGHT_HAND_1_MAX; i-=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }

    for(; i <= RIGHT_HAND_1_DEFAULT; i+=4) {
      MSS.servoWrite(PIN_RIGHT_HAND_1, i);
      DELAY_AND_CHECK
    }*/
}

void caucas()
{
  i = RIGHT_SHOULDER_2_DEFAULT;
  j = LEFT_SHOULDER_2_DEFAULT;
    
  for(; i >= 100; i-=4, j+=4) {
      MSS.servoWrite(PIN_RIGHT_SHOULDER_2, i);
      delay(15);
      MSS.servoWrite(PIN_LEFT_SHOULDER_2, j);
      DELAY_AND_CHECK
  }
  
  for(j = 180; j >= 70; j-=2) {
    MSS.servoWrite(PIN_LEFT_HAND_2, j);
    DELAY_AND_CHECK
  }
    
  while(1) {
    for(i = 0, j = 70; i <= 110; i+=2, j+=2) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      MSS.servoWrite(PIN_LEFT_HAND_2, j);
      DELAY_AND_CHECK
    }
    delay(400);
    for(; i >= 0; i-=2, j-=2) {
      MSS.servoWrite(PIN_RIGHT_HAND_2, i);
      MSS.servoWrite(PIN_LEFT_HAND_2, j);
      DELAY_AND_CHECK
    }
    delay(400);
  }

}