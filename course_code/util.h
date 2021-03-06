#ifndef UTIL_H
#define UTIL_H

#include "myImu.h"
#include "controls.h"

struct TurnState 
{
  bool initialized = false;
  int initialYaw;
  int lastYaw;
  int goalYaw;
};

Adafruit_BNO055 g_bno;

int getYaw()
{
    sensors_event_t imu_event; 
    g_bno.getEvent(&imu_event);
    return imu_event.orientation.x;
}

bool turnOnSpot(TurnState ts, int deg, MotorCommand* mc)
{
  if(!ts.initialized)
  {
    ts.initialYaw = getYaw();
    ts.lastYaw = ts.initialYaw;
    ts.goalYaw = ts.initialYaw + deg;
    ts.initialized = true;
  }

  int yaw = getYaw();

  // check wrap around
  const int WRAP_THRESH = 30;
  
  // case 1: we've wrapped around from 360 to 0
  if(ts.lastYaw > (360-WRAP_THRESH) && yaw < WRAP_THRESH)
    ts.goalYaw -= 360;
  // case 2: we've wrapped around from 0 to 360
  else if(ts.lastYaw < WRAP_THRESH && yaw > (360-WRAP_THRESH))
    ts.goalYaw += 360;

  ts.lastYaw = yaw;

  // compute directions to turn
  int diff =  ts.goalYaw - yaw;
  
  // case 1: we've finished the turn
  const int DONE_THRESHOLD = 3;
  if(abs(diff) < DONE_THRESHOLD)
  {
      mc->leftV = 0;
      mc->rightV = 0;
      return true;
  }

  const int MOTOR_V = 100;
  
  // case 2: we need to turn CW
  if(diff > 0)
  {
    mc->leftV = MOTOR_V;
    mc->rightV = -1 * MOTOR_V;
  }
  // case 3: we need to turn CCW
  else
  {
    mc->leftV = -1 * MOTOR_V;
    mc->rightV = MOTOR_V;
  }

  return false;
}

#endif
