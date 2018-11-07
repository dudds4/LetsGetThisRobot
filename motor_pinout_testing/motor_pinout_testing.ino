#include "mymotor.h"

// PINS

// motor 1 = motor right : pins
int ENC1_PIN_A = 18, ENC1_PIN_B = 19;
int MOT1_H = 7, MOT1_L = 6;

// motor 2 = motor left : pins
int ENC2_PIN_A = 2, ENC2_PIN_B = 3;
int MOT2_H = 10, MOT2_L = 11;

// button pin
int BUT_SET = 13, BUT_MES = 12;

// Global Declarations
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
  attachInterrupt(digitalPinToInterrupt(ENC2_PIN_A), updateEnc2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC2_PIN_B), updateEnc2, CHANGE);

  // initialize serial
  Serial.begin (9600);

  // initialize motor 
  pinMode(BUT_SET, OUTPUT);
  digitalWrite(BUT_SET, 1);
  pinMode(BUT_MES, INPUT);
  
}

int state = 0;

void drive(int MOT_H, int MOT_L, int pwm)
{
  if(pwm >= 0)  
  {
    analogWrite(MOT_H, pwm);
    analogWrite(MOT_L, 0);  
  }
  else
  {
    analogWrite(MOT_H, 0);
    analogWrite(MOT_L, -1*pwm);
  }
}

void loop()
{
  delay(1);

  // wait for the button press
  if(!state && (digitalRead(BUT_MES) == LOW)) return;
  if(!state) state = 1;

  if(state == 1)
  {
    Serial.print("Button pressed! \n");
    state = 2;
  }
  else if(state == 2)
  {
    bool testM1 = false;
    bool testForward = true;

    int signV = testForward ? 1 : -1;
    
    if(testM1)
    {
      drive(MOT1_H, MOT1_L, signV * (3.0f/5) * 255);
      analogWrite(MOT2_L, 0);
      analogWrite(MOT2_H, 0);
    } 
    else
    {
      drive(MOT2_H, MOT2_L, signV * (3.0f/5) * 255);
      analogWrite(MOT1_L, 0);
      analogWrite(MOT1_H, 0);
    }

    Serial.print("Motor ");
    Serial.print(testM1 ? 1 : 2);
    Serial.print(": ");
    
    Serial.print(testM1 ? mot1.count() : mot2.count());
    Serial.print("\n");
  
  }

  

}
