#ifndef CONTROLS_H
#define CONTROLS_H

#include "nav.h"
#include "myImu.h"

MotorCommand wallFollow(Adafruit_BNO055& bno, MotorCommand lastCommand) 
{
  const unsigned PERIOD = 10;
  static unsigned counter = PERIOD;
  if(++counter < PERIOD) return lastCommand;
  counter = 0;
  
  static bool initialized = false;
  static sensors_event_t initial_imu;
  if(!initialized)
  {
     bno.getEvent(&initial_imu);  
  }
  
	// example based on imu branch code
	// not actually good code

  return driveStraight(bno, initial_imu, lastCommand);
}

MotorCommand climbRamp1(/* add params */) 
{}

MotorCommand search(/* add params */) 
{
  
  
}

MotorCommand return1(/* add params */) 
{}

MotorCommand ramp2(/* add params */) 
{}

MotorCommand return2(/* add params */) 
{}

#endif // CONTROLS_H
