#include "nav.h"

Motor motorLeft = 
{
  .en = 99,
  .in1 = 5,
  .in2 = 6,
};

Motor motorRight = 
{
  .en = 99,
  .in1 = 7,
  .in2 = 8
};

void initMotor(Motor m)
{  
  pinMode(m.in1,OUTPUT); 
  pinMode(m.in2,OUTPUT);
  
  digitalWrite(m.in1,LOW);
  digitalWrite(m.in2,LOW);
}

void initMotors() 
{
  initMotor(motorRight);
  initMotor(motorLeft);
}
