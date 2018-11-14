#ifndef UTIL_H
#define UTIL_H

#include "myImu.h"

// Object used by the turn on spot function to track state
struct TurnState;

// Turn on spot algorithm uses IMU as feedback
// Call it periodically passing a TurnState object.
// It (more or less) automatically checks the change in rotation since the first time you call it

/* Example usage:

static TurnState ts; // declare the object static so that it is only initialized once
MotorCommand mc;     // function takes MotorCommand object as an output variable

if(turnOnSpot(ts, 90, &mc))
{
  // turn is complete
}
else
{
  // turn is not complete yet
}

*/

bool turnOnSpot(TurnState ts, int deg, MotorCommand* mc);

struct TurnState 
{
  bool initialized = false;
  double initialYaw;
  double lastYaw;
  double goalYaw;
};

Adafruit_BNO055 g_bno;

double getYaw()
{
    sensors_event_t imu_event; 
    g_bno.getEvent(&imu_event);
    return imu_event.orientation.x;
}

double getVertAccel()
{
    imu::Vector<3> xyz = g_bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    return xyz[0]; 
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
  const int WRAP_THRESH = 30; // WRAP_THRESH will need to be larger if turning is faster.. certainly can't be more than like 100
  
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

  const int MOTOR_V = 100; // MOTOR_V probably needs to be tuned
  
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
