#ifndef RAMPFIND_H
#define RAMPFIND_H

#include "util.h"

const double FWDIST = 20;
const double SWDIST = 26; // larger than 23

const double ROB_OFFSET = 4; // smaller than 8

const int GOAL_AVG = 250;

const int SW_THRESH = 1;

struct RampFinder
{
	TurnState ts;
	double initialYaw = 0, initialPitch, initialRoll;
	int state = 0;
	int successCounter = 0;

	double errorSum = 0;
	double lastError = 0;
private:
  bool _isDone = false;
  double turnAngle;
public:
	bool isDone() { return _isDone; }

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
				Serial.println("Entering state 2, wall follow");
				state = 2;
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
        Serial.println("Entering state 3, position");
				state = 3;
			}
      else
      {
        lastCommand = wallFollow(initialYaw - 90, FWDIST - ROB_OFFSET, 140, lastCommand);
      }
	  }
		else if (state == 3)
		{
      int error = frontIr.getDist() - SWDIST;
      
			if (abs(error) < SW_THRESH)
			{
				successCounter++;
			}
			else
			{
				successCounter = 0;

				// move motors + or - to reduce the error
				const double Kp = 10, Ki = 0.5;

				// prevent overshoot from integral gain
				if (error*lastError < 0) 
				  errorSum = 0;

				errorSum += Ki * error;
				double result = Kp * error + errorSum;

				lastCommand.set(result, result);
			}

			if (successCounter > 2)
			{
				Serial.println("Entering state 4, turn");
				state = 4;
				lastCommand.reset();
				ts.reset();
        turnAngle = (180 + initialYaw) - getYaw();  
        if(turnAngle > 180) turnAngle -= 360;
        if(turnAngle < -180) turnAngle += 360;
        
			}
		}
		else if (state == 4)
		{ // 90 deg turn CCW
			if (turnOnSpot(ts, turnAngle, &lastCommand))
			{
				Serial.println("Entering state 5");
				state = 5;
//        _isDone = true;
        initialYaw = getYaw();
				lastCommand.reset();
			}
		}
		else if (state == 5)
		{ 
//		  // drive forward until make contact with ramp
//			const double PITCH_THRESHOLD = 20;
//			// drive along the wall until we get that ramp booiiis
//			if(getPitch() > PITCH_THRESHOLD)
//			{
//				Serial.print(getPitch());
//				Serial.print(" ");
//				Serial.println(getRoll());
//				lastCommand.reset();
//			}
//			else
//			{
//				lastCommand = wallFollow(initialYaw, SWDIST - ROB_OFFSET, GOAL_AVG, lastCommand);
//			}

      // Getting on ramp
      const double PITCH_THRESH = 20;
      if(getPitch() > PITCH_THRESH) //on the ramp
      {
          Serial.println("Got on ramp!");
          // give up
          while(1) {}
      }
      else
      {
          double rDist = rightIr.getDist();
          double error = rDist - SWDIST;
          double bandwidth = 1;
          double A_ = 9 / bandwidth*bandwidth;
          double mSpeed = 75 + 75 / (1 + A_ * error * error);
          
          lastCommand = wallFollow(initialYaw, SWDIST, mSpeed, lastCommand);

//          Serial.println(frontIr.getDist());
      }

		}

		return _isDone ? MotorCommand() : lastCommand;
	}

};

#endif //RAMPFIND_H
