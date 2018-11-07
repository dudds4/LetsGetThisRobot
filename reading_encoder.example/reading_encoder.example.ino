#include "mymotor.h"

// PINS

// motor 1 pins
int ENC1_PIN_A = 2, ENC1_PIN_B = 3;
int MOT1_H = 2, MOT1_L = 3;

// motor 2 pins
int ENC2_PIN_A = 4, ENC2_PIN_B = 5;
int MOT2_H = 2, MOT2_L = 3;

// button pin
int BUT_SET = 13, BUT_MES = 9;

Motor mot1(MOT1_H, MOT1_L, ENC1_PIN_A, ENC1_PIN_B);
Motor mot2(MOT2_H, MOT2_L, ENC2_PIN_A, ENC2_PIN_B);

void updateEnc1() { mot1.updateEncoder(); }
void updateEnc2() { mot2.updateEncoder(); }

void setup() 
{
  // initialize motor 1
  mot1.initialize();
  attachInterrupt(digitalPinToInterrupt(ENC1_PIN_A), updateEnc1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC1_PIN_B), updateEnc1, CHANGE);
  
  // initialize motor 2
  mot2.initialize();
  attachInterrupt(digitalPinToInterrupt(ENC2_PIN_A), updateEnc1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC2_PIN_B), updateEnc1, CHANGE);

  // initialize serial
  Serial.begin (9600);  


  // initialize motor 
  pinMode(BUT_SET, OUTPUT);
  digitalWrite(BUT_SET, 1);
  pinMode(BUT_MES, INPUT);
  
}

int state = 0;

void loop()
{

  delay(1);

  // wait for the button press
  if(!state && (digitalRead(BUT_MES) == LOW)) return;
  if(!state) state = 1;

  Serial.print("Button pressed! \n");

  

  

}
