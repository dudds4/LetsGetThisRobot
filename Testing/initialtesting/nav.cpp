#include "nav.h"

Motor motorLeft = 
{
  .en = 10,
  .in1 = 8,
  .in2 = 5
};

Motor motorRight = 
{
  .en = 11,
  .in1 = 6,
  .in2 = 7
};

void initMotor(Motor m)
{  
  pinMode(m.en,OUTPUT);
  pinMode(m.in1,OUTPUT); 
  pinMode(m.in2,OUTPUT);
  
  digitalWrite(m.en, HIGH);
  digitalWrite(m.in1,LOW);
  digitalWrite(m.in2,LOW);
}

void initMotors() 
{
  initMotor(motorRight);
  initMotor(motorLeft);
}
