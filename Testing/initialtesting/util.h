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
  int initialYaw;
  int lastYaw;
  int goalYaw;
};

// returns yaw read from global imu sensor
int getYaw();

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
MotorCommand driveStraight(sensors_event_t initial, MotorCommand lastCommand, int goalAvg);

// drives straight using the mounted IR to travel a straight line
MotorCommand genWallFollow(double dist, int goalAvg, MotorCommand lastCommand);

MotorCommand wfIMU(sensors_event_t initial, double dist, int goalAvg, MotorCommand lastCommand);

#endif
