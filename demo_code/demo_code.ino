#include "mymotor.h"

//SimpleTimer msTimer;

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
//  Serial.begin (9600);

  // initialize button 
  pinMode(BUT_SET, OUTPUT);
  digitalWrite(BUT_SET, 1);
  pinMode(BUT_MES, INPUT);
  
}

int state = 0;
int stateCounter = 0;

void loop()
{
  const int PERIOD = 1;
  delay(PERIOD);

  // wait for the button press
  if(!state && (digitalRead(BUT_MES) == LOW)) return;
  if(!state) state = 1;

  bool testM1 = true;
  bool testForward = true;
  int signV = testForward ? 1 : -1;

  if(state == 1)
  { 
    state = 2;

    digitalWrite(MOT1_H, HIGH);
    digitalWrite(MOT1_L, LOW);
    digitalWrite(MOT2_H, HIGH);
    digitalWrite(MOT2_L, LOW);
    
    stateCounter = 0;
  }
  else if(state == 2)
  { 
    mot1.setSetpoint(20);
    mot1.setGains(60, 2);

    mot2.setSetpoint(20);
    mot2.setGains(60, 2);

    stateCounter++;
    if(stateCounter > 5000)
    {
      state = 999;

      analogWrite(MOT1_H, 0);
      analogWrite(MOT1_L, 0);
      analogWrite(MOT2_H, 0);
      analogWrite(MOT2_L, 0);
      
      mot2.setGains(0, 0);
      mot1.setGains(0, 0);
      mot1.setSetpoint(0);
      mot2.setSetpoint(0);
    }
  }

  // motor controls
  if(state < 10)
  {
      static int updateCounter = 0;
      if(++updateCounter > 10)
      {
//          mot1.update(PERIOD);
//          mot2.update(PERIOD);        
          updateCounter = 0;
      }
  }
  
}
