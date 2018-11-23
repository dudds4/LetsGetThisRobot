#ifndef RAMP_CLIMB_H
#define RAMP_CLIMB_H

const double IR_THRESH = 500;
const double IMU_THRESH = 5;
//const int GOAL_AVG = 250;
const int DELTA_R = 15;
const int DELTA_L = 20;

struct RampClimber
{
  TurnState ts;
  double initialYaw, initialPitch, initialRoll;
  int state = 0;
  unsigned loopCtr = 0;
  unsigned ctr = 0;
  
  MotorCommand run(MotorCommand lastCommand)
  {
    double irL = rampIR_L.getMedian(); //for finding ramp initially
    double irR = rampIR_R.getMedian(); //for driving on ramp
    double diff;

    bool onRamp = false;

    if(state == 0)
    {
      initialYaw = getYaw();
      initialPitch = getPitch();
      lastCommand = MotorCommand();
      //lastCommand.leftV = 200;
      //lastCommand.rightV = 200;
      ts.reset(); 
      Serial.println("Finished state 0");
      state = 2; //skip state 1 for testing
    }
    if(state == 1) 
    { // Getting on ramp

      if(initialPitch - getPitch() > IMU_THRESH) //on the ramp
      {
        if(irL > 900) // too far right
        {
          Serial.println("need to turn left");
          //reverse, turn CCW, forward
          if(ctr < 500)
          {
            lastCommand.leftV = -100;
            lastCommand.rightV = -100;

            ctr++;
          }
          else
          {
            ctr = 0;
            //lastCommand.reset();
            if(turnOnSpot(ts, -5, &lastCommand))
              lastCommand = driveStraight(initialYaw, lastCommand, GOAL_AVG);
          }
        }
        else if(irL < 300)
        {
          Serial.println("need to turn right");

          if(ctr < 500)
          {
            lastCommand.leftV = -100;
            lastCommand.rightV = -100;
            ctr++;
          }
          else 
          {
            ctr = 0;
            lastCommand.reset();
            if(turnOnSpot(ts, -5, &lastCommand))
              lastCommand = driveStraight(initialYaw, lastCommand, GOAL_AVG);
          }
        }
        else 
        {
          lastCommand = driveStraight(initialYaw, lastCommand, GOAL_AVG);  
        }
      }
      else
      {
        Serial.println("driving straight");
        lastCommand = driveStraight(initialYaw, lastCommand, GOAL_AVG);
      }
    }
    else if(state == 2)
    { 
      bool shouldPrint = subsamplePrint(20);
      const int V_INC = 30;
      const int V_DEC = 0;
      const int DIFF_SAT = 100;
      
      // driving on ramp
      if(shouldPrint)
      {
        Serial.println(irR);
      }
      
      if(irR > 800) //need to turn left
      {
        if(shouldPrint) Serial.println("Turn left");

        if(lastCommand.rightV < lastCommand.leftV)
          lastCommand.rightV = lastCommand.leftV;

        if(lastCommand.rightV - lastCommand.leftV < DIFF_SAT)
        {
          lastCommand.rightV += V_INC;
          lastCommand.leftV -= V_DEC;          
        }
      }
      else if (irR < 700) //need to turn right
      {
        if(shouldPrint) Serial.println("Turn right");

        if(lastCommand.rightV > lastCommand.leftV)
          lastCommand.leftV = lastCommand.rightV;  
          
        if(lastCommand.leftV - lastCommand.rightV < DIFF_SAT)
        {
          lastCommand.leftV += V_INC;
          lastCommand.rightV -= V_DEC;        
        }  
      }
      else //straight
      {
        if(shouldPrint) Serial.println("driving straight");
        lastCommand = driveStraight(initialYaw, lastCommand, GOAL_AVG);
      }
      
      lastCommand = translateWithinLimits(lastCommand);
    }
      return lastCommand;
  }  
  
};

#endif //RAMP_CLIMB_H
