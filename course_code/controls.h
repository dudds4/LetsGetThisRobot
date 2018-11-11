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

MotorCommand search(/* add params */) {}

MotorCommand return1(/* add params */) {}

MotorCommand ramp2(/* add params */) {}

MotorCommand return2(/* add params */) {}

#endif // CONTROLS_H