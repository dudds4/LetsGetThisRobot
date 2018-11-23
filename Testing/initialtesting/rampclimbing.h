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
  int state = 2;

  const float SWDIST = 26; // bigger than 23

  unsigned loopCtr = 0;
  unsigned ctr = 0;

  int flatCounter = 0;
  int isFlat = 0;

private:
  bool _isDone = false;
  
public:
  bool isDone() { return _isDone; }
  
  MotorCommand run(MotorCommand lastCommand)
  {
    double irL = rampIR_L.getMedian(); //for finding ramp initially
    double irR = rampIR_R.getMedian(); //for driving on ramp
    double diff;

    bool onRamp = false;

    if(state == 0)
    {
      Serial.println("Entering state 0");

      // delay 20 iterations
      if(++ctr < 20)
      {
        lastCommand.reset();
      }
      else
      {
        ctr = 0;
        initialYaw = getYaw();
        initialPitch = getPitch();
        initialRoll = getRoll();
        lastCommand.reset();
        ts.reset();
        Serial.println("Finished state 0");
        state = 1;
      }      
    }
    else if(state == 1) 
    { 

    }
    else if(state == 2)
    { 
      bool shouldPrint = 0 && subsamplePrint(20);
      const int V_INC = 15;
      const int V_DEC = 0;
      const int DIFF_SAT = 100;
      
      // driving on ramp
//      if(shouldPrint)
//      {
//        Serial.println(irR);
//      }

      const int VMAX = 180;
      const int VMAXR = 160;
      const int VMIN = 90;
      
      if(irR > 800) //need to turn left
      {
        if(shouldPrint) Serial.println("Turn left");

        if(lastCommand.rightV < lastCommand.leftV) lastCommand.rightV = lastCommand.leftV;
        
        lastCommand.rightV += V_INC;
        
        if(lastCommand.rightV > VMAXR) 
        {
          lastCommand.leftV -= lastCommand.rightV - VMAXR;
          lastCommand.rightV = VMAXR;
        }
        if(lastCommand.leftV < VMIN) lastCommand.leftV = VMIN;
        
      }
      else if (irR < 700) //need to turn right
      {
        if(shouldPrint) Serial.println("Turn right");

        if(lastCommand.rightV > lastCommand.leftV) lastCommand.leftV = lastCommand.rightV;  
         
        lastCommand.leftV += V_INC;
           
        if(lastCommand.leftV > VMAX) 
        {
          lastCommand.rightV -= lastCommand.leftV - VMAX;
          lastCommand.leftV = VMAX;
        }
        
        if(lastCommand.rightV < VMIN) lastCommand.rightV = VMIN;
        
      }
      else //straight
      {
        if(shouldPrint) Serial.println("driving straight");
        lastCommand = driveStraight(initialYaw, lastCommand, 200);
      }
      
      lastCommand = translateWithinLimits(lastCommand);

      // check flats for end conditions
      const int PITCH_FLAT_THRESH = 10;
      if(getPitch() < PITCH_FLAT_THRESH)
      {
        if(!isFlat) flatCounter++;
        
        isFlat = 1;
      }
      else
        isFlat = 0;

      if(flatCounter > 1)
        _isDone = true;
        
    }

    if(_isDone) Serial.println("Hit flat code");
    
    return _isDone ? MotorCommand() : lastCommand;
  }  
  
};

#endif //RAMP_CLIMB_H
