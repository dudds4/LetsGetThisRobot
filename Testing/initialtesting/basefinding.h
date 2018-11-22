#ifndef BASE_FIND_H
#define BASE_FIND_H

#include "nav.h"
#include "util.h"
#include "sensors.h"

struct BaseFinder
{
  bool foundBase() { return found; }
	MotorCommand run(MotorCommand lc)
  {
    if(!initialized) yaw = getYaw();
    
    if(state == 0)
    {
      if(frontIr.getDist() < 15)
      {
        state = 1;      
      }
      else
      {
        lc = driveStraight(yaw, lc, 200);   
        lc.set(0, 0);
      }
    }
    else if(state == 1)
    {
      // TODO: find heading
      state = 2;  
      ts.reset();
    }
    else if(state == 2)
    {
      if(turnOnSpot(ts, -90, &lc))
      {
        state = 3;  
      }
    }

    // check if we found the base
    imu::Vector<3> vec = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    Serial.print(vec[0]);
    Serial.print(" ");
    Serial.print(vec[1]);
    Serial.print(" ");
    Serial.println(vec[2]);

    return lc;
  }

private:
  int state = 0;
  int initialized = 0;
  double yaw;
  bool found;
  TurnState ts;
  
};

#endif
