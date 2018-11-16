#ifndef MYIR_H
#define MYIR_H

#include <math.h>
#include "util.h"

inline double irAnalogToCm(int analogValue)
{
  return 1619.1 * pow(analogValue, -0.591);
}

bool wallOnRight(TurnState ts, MotorCommand* mc, Adafruit_BNO055& bno, sensors_event_t initial_imu, MotorCommand lastCommand) { //make sure wall is on right at 20 cm away (use second ir)
  bool tos = turnOnSpot(ts, 10, mc);
  for(int i = 0; i < 4; i++) {
    MotorCommand drvStr = driveStraight(bno, initial_imu, lastCommand);
  }
  return turnOnSpot(ts, -10, mc);
  
}

#endif // MYIR_H
