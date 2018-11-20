#ifndef MYNAV_H
#define MYNAV_H

#define MOTOR_V_MAX 255

struct Motor 
{
  int en;
  int in1;
  int in2;
};

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

struct MotorCommand
{
	int leftV;
	int rightV;
};

#endif // MYNAV_H
