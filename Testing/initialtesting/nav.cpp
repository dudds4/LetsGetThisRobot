#include "nav.h"

Motor motorLeft = 
{
  .en = 12,
  .in1 = 2,
  .in2 = 3
};

Motor motorRight = 
{
  .en = 11,
  .in1 = 9,
  .in2 = 10 
};

void initMotor(Motor m)
{  
//  pinMode(m.en,OUTPUT);
  pinMode(m.in1,OUTPUT); 
  pinMode(m.in2,OUTPUT);
  
//  digitalWrite(m.en, HIGH);
  digitalWrite(m.in1,LOW);
  digitalWrite(m.in2,LOW);
}

void initMotors() 
{
  initMotor(motorRight);
  initMotor(motorLeft);
}
