#include "nav.h"

Motor motorLeft = 
{
  .en = 99,
  .in1 = 11,
  .in2 = 10
};

Motor motorRight = 
{
  .en = 99,
  .in1 = 9,
  .in2 = 3
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
