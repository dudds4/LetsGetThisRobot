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

  int origin = imu_event.orientation.x;
  
  bool clockwise = 1;
  
  int differenceY = initial.orientation.y - imu_event.orientation.y;
  int differenceX = initial.orientation.x - imu_event.orientation.x;
  
  if(abs(differenceY) > 5) 
  { // Robot has just got on the wall

      //begin controlling the motors
      //speed up the left motor first causing a right

      //robot is now "steady" on the wall
      if(abs(differenceY) < 1 )
      {
          if( abs(differenceX) < 45 && (clockwise) )
          {
            lastCommand.leftV += 30;
            lastCommand.rightV -= 30;
          }
    
          clockwise = 0;
          //origin = imu_event.orientation.x;
    
          //
          else( (abs(differenceX) < 45  || abs(differenceX) > 315 ) && (!clockwise) )
          {
           
            //set init to curr here 
            lastCommand.leftV -= 30;
            lastCommand.rightV += 30;
          }
    
          clockwise = 1;

      }
      
  }
  else{
   //perform turning function to turn around 
   }
  
  
}

MotorCommand search(/* add params */) {}

MotorCommand return1(/* add params */) {}

MotorCommand ramp2(/* add params */) {}

MotorCommand return2(/* add params */) {}

#endif // CONTROLS_H
