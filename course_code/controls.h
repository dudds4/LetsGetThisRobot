#ifndef CONTROLS_H
#define CONTROLS_H

#include "nav.h"
#include "imu.h"

MotorCommand wallFollow(MotorCommand lastCommand) 
{
	// example based on imu branch code
	// not actually good code
	const unsigned PERIOD = 10;

	static unsigned counter = PERIOD;
	
	if(++counter < PERIOD) return lastCommand;
	counter = 0;

	return driveStraight();
}

MotorCommand climbRamp1(/* add params */) {}

MotorCommand wallZigzag(Adafruit_BNO055& bno, sensors_event_t initial, MotorCommand lastCommand) 
{
  //set left motor speed 
  //set right motor speed
  //need to take in imu status 
  //this function is called based on if y = 90 or - 90 
  
   /* Get a new sensor event */ 
  sensors_event_t imu_event; 
  bno.getEvent(&imu_event);

  int angle = imu_event.orientation.z;
  int origin = 0;
  

  int differenceY = initial.orientation.y - imu_event.orientation.y;
  int differenceX = initial.orientation.z - imu_event.orientation.z;
  
  if(abs(differenceY) > 5) 
  { // Robot is on the wall

      //begin controlling the motors
      //speed up the left motor first causing a right
      
      if( abs(differenceX) < 45 && (origin - angle < 0) )
      {
        lastCommand.leftV += 30;
        lastCommand.rightV -= 30;
      }
  
      else( abs(differenceX) > 45 && (origin - angle > 0) )
      {
        //set init to curr here 
        initial.orientation.z = imu.orientation.z;
        lastCommand.leftV -= 30;
        lastCommand.rightV += 30;
      }

  }
  else{}
  
  
}

MotorCommand search(/* add params */) {}

MotorCommand return1(/* add params */) {}

MotorCommand ramp2(/* add params */) {}

MotorCommand return2(/* add params */) {}

#endif // CONTROLS_H
