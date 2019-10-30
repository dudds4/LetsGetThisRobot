#ifndef UTIL_H
#define UTIL_H

#include "nav.h"
#include "sensors.h"

#define MAX_APPLIED 170

//sensors_event_t imuWF; //wall follow

// utility for printing a motor command to serial
void printCommand(MotorCommand mc);

// utility for mapping positive / negative voltage to motor pins
void setMotorVoltage(Motor m, int v);

bool subsamplePrint(int ss);


void setMotorVoltage(int EN, int IN1, int IN2, int v)
{
   if(v > 255) v = 255;
   else if(v < -255) v = -255;
   
   digitalWrite(EN, HIGH);// motor speed 
   if(v > 0)
   {
     analogWrite(IN2, v); //right motor
     digitalWrite(IN1,LOW); 
   }
   else
   {
     analogWrite(IN1, -1*v); //right motor
     digitalWrite(IN2,LOW);  
   }
}


struct TurnState 
{
  bool initialized = false;
  double initialYaw;
  double lastYaw;
  double goalYaw;
  double errorSum = 0;
  double lastError = 0;
  void reset() { initialized = false; }
};

// returns yaw read from global imu sensor
double getYaw();
double getPitch();
double getRoll();

// runs a turn, outputs to MotorCommand* mc, returns true when turn is complete
bool turnOnSpot(TurnState &ts, int deg, MotorCommand* mc);

#ifndef MAX
#define MAX(a,b) (a > b ? a : b)
#endif

#ifndef MIN
#define MIN(a,b) (a > b ? b : a)
#endif

MotorCommand translateWithinLimits(MotorCommand c);

// -- utility functions for common navigation tasks --

// drives straight using IMU to correct orientation
MotorCommand driveStraight(double initialAngle, MotorCommand lastCommand, int goalAvg);
MotorCommand driveStraightBackward(double initialAngle, MotorCommand lastCommand, int goalAvg);

// drives straight using the mounted IR to travel a straight line
//MotorCommand genWallFollow(double dist, int goalAvg, MotorCommand lastCommand);

MotorCommand wallFollow(double initialAngle, double dist, int goalAvg, MotorCommand lastCommand);

bool reverse(MotorCommand lastCommand);

#endif
