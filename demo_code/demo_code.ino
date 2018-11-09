#include "mymotor.h"

//SimpleTimer msTimer;

// PINS

// motor 1 = motor right : pins
int ENC1_PIN_A = 19, ENC1_PIN_B = 18;
int MOT1_H = 7, MOT1_L = 8;

// motor 2 = motor left : pins
int ENC2_PIN_A = 2, ENC2_PIN_B = 3;
int MOT2_H = 5, MOT2_L = 6;

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

  // change up PWM frequencies for motor PWM pins
  // Timer 3 for outputs 2/3/5
  TCCR3B &= ~(0x07);
  TCCR3B |= 0x02;
  
  // Timer 4 for outputs 6,7,8  
  TCCR4B &= ~(0x07);
  TCCR4B |= 0x02;
  
// initialize serial
  Serial.begin (9600);

  // initialize button 
  pinMode(BUT_SET, OUTPUT);
  digitalWrite(BUT_SET, 1);
  pinMode(BUT_MES, INPUT);


}

int state = 0;
int stateCounter = 0;
unsigned endTime;

void loop()
{
  static bool started = false;
  if(!started)
  {
    endTime = millis() + 5000;
    started = true;
  }
    
  const int PERIOD = 1;
  delay(PERIOD);

  // wait for the button press
  if(!state && (digitalRead(BUT_MES) == LOW)) return;
  if(!state) state = 1;

  if(state == 1)
  { 
    state = 2;
    
    mot1.setGains(0, 0.00000001);
    mot2.setGains(0, 0.00000001);
    
    stateCounter = 0;
  }
  else if(state == 2)
  { 

    mot1.setSetpoint(6);
    mot2.setSetpoint(6);
    mot1.update(PERIOD);
//    mot2.update(PERIOD);

    int currentTime = millis();

    if(currentTime > endTime) 
    {
      state = 999;
      Serial.print("Done\n");
    }
  }
  else
  {
    digitalWrite(MOT1_H, LOW);
    digitalWrite(MOT1_L, LOW);    
    digitalWrite(MOT2_H, LOW);
    digitalWrite(MOT2_L, LOW);
  }

  
}
