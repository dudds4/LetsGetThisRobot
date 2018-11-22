#ifndef MYNAV_H
#define MYNAV_H

#define MOTOR_V_MAX 255
#include "Arduino.h"

struct Motor 
{
  int en;
  int in1;
  int in2;
};

struct MotorCommand
{
  int leftV;
  int rightV;
  void reset() { leftV = 0; rightV = 0; }
};

extern Motor motorLeft;
extern Motor motorRight;

void initMotor(Motor m);
void initMotors();


#endif // MYNAV_H
