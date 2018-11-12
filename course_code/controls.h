#ifndef CONTROLS_H
#define CONTROLS_H

#include "nav.h"
#include "myImu.h"
#include "ir.h"
#include "sensors.h"

MotorCommand wallFollow(Adafruit_BNO055& bno, MotorCommand lastCommand) 
{
  const unsigned PERIOD = 10; 
  static unsigned counter = PERIOD;
  unsigned int ir1Array[] = {0,0,0,0,0};
  if(++counter < PERIOD) {
    for(int i = sizeof(ir1Array); i>0; i--) { //shift values
      ir1Array[i] = ir1Array[i-1];
    }
    ir1Array[0] = analogRead(ir1);
    return lastCommand;
  }
  counter = 0;

  int sum = 0; 
  for(int i = sizeof(ir1Array); i>0; i--) { //take avg and discard rly out-of-range values
      sum = sum + ir1Array[i];
  }
  double ir1Avg = sum / 5;

  /****** IMU stuff ******/
  static bool initialized = false;                   
  static sensors_event_t initial_imu;
  if(!initialized)
  {
     bno.getEvent(&initial_imu);  
  }
  initialized = 1; 
	
  /*********************************** METAL WALL ***********************************/
  static bool rightWall = false;
  if(irAnalogToCm(ir1Avg) < 20 && !rightWall)
    rightWall = true;
  if(rightWall) {
    //turn left
    
  }
  
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
