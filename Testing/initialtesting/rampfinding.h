#ifndef RAMPFIND_H
#define RAMPFIND_H

#include "util.h"

const double FWDIST = 27;
const double SWDIST = 25;

const double ROB_OFFSET = 8;

const int GOAL_AVG = 250;

const int SW_THRESH = 2;

struct RampFinder
{
	TurnState ts;
	double initialYaw, initialPitch, initialRoll;
	int state = 0;
	int successCounter = 0;

	double errorSum = 0;
	double lastError = 0;

	bool isDone() { return true; }

	MotorCommand run(MotorCommand lastCommand)
	{
		if (state == 0)
		{ 
		  // drive straight until front ir below threshold
			if (frontIr.getDist() < FWDIST)
			{
				Serial.println("Entering state 1");
				state = 1;
				ts.reset();
				lastCommand = MotorCommand();
			}
			else
			{
				lastCommand = driveStraight(initialYaw, lastCommand, GOAL_AVG);
			}
		}
		else if (state == 1)
		{ 
		  // 90 deg turn CCW
			if (turnOnSpot(ts, -90, &lastCommand))
			{
				Serial.println("Entering state 2");
				state = 2;
				initialYaw = getYaw();
				lastCommand.reset();
			}
		}
		else if (state == 2)
		{
			// Follow the wall!
			//implementing control to ensure our error is less than 1 cm

			int error = frontIr.getDist() - SWDIST;

			if (frontIr.getDist() < SWDIST)
			{
				state = 3;
			}
			else if (state == 3)
			{
				if (abs(error) < SW_THRESH)
				{
					successCounter++;
				}
				else
				{
					successCounter = 0;

					// move motors + or - to reduce the error
					const double Kp = 5, Ki = 0.1;

					// prevent overshoot from integral gain
					if (error*lastError < 0) 
					  errorSum = 0;

					errorSum += Ki * error;
					double result = Kp * error + errorSum;

					lastCommand.set(result, result);
				}

				if (successCounter > 2)
				{
					Serial.println("Entering state 3");
					state = 4;
					lastCommand.reset();
					ts.reset();
				}
			}
			else
			{
				lastCommand = wallFollow(initialYaw, FWDIST - ROB_OFFSET, GOAL_AVG, lastCommand);
			}
		}
		else if (state == 4)
		{ // 90 deg turn CCW
			if (turnOnSpot(ts, -90, &lastCommand))
			{
				Serial.println("Entering state 4");
				state = 5;
				initialYaw = getYaw();
				initialPitch = getPitch();
				initialRoll = getRoll();
				lastCommand.reset();
			}
		}
		else if (state == 5)
		{ // drive forward until make contact with ramp
			const double PITCH_THRESHOLD = 10;
			// drive along the wall until we get that ramp booiiis
			if (abs(getPitch() - initialPitch) > PITCH_THRESHOLD)
			{
				Serial.print(getPitch());
				Serial.print(" ");
				Serial.println(getRoll());
				lastCommand.reset();
			}
			else
			{
				lastCommand = wallFollow(initialYaw, SWDIST - ROB_OFFSET, GOAL_AVG, lastCommand);
			}
		}

		return lastCommand;
	}

};

#endif //RAMPFIND_H
