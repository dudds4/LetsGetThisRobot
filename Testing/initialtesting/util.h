#ifndef UTIL_H
#define UTIL_H

#include "nav.h"
#include "sensors.h"

//sensors_event_t imuWF; //wall follow

// utility for printing a motor command to serial
void printCommand(MotorCommand mc);

// utility for mapping positive / negative voltage to motor pins
void setMotorVoltage(Motor m, int v);

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

#endif
