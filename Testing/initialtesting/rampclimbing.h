#ifndef RAMP_CLIMB_H
#define RAMP_CLIMB_H

const double IR_THRESH = 500;
const double IMU_THRESH = 5;
//const int GOAL_AVG = 250;
const int DELTA = 5;

struct RampClimber
{
  TurnState ts;
  double initialYaw, initialPitch, initialRoll;
  int state = 0;
  unsigned loopCtr = 0;
  unsigned ctr = 0;
  
  MotorCommand run(MotorCommand lastCommand)
  {
    double irL = rampIR_L.getRaw();
    double irR = rampIR_R.getRaw();
    double diff;
    bool correction = false;
    bool onRamp = false;

    if(state == 0)
    {
      initialYaw = getYaw();
      initialPitch = getPitch();
      lastCommand = MotorCommand();
      ts.reset();
      Serial.println("Finished state 0");
      state = 1;
    }
    if(state == 1) 
    { // Getting on ramp
      //Serial.print(initialPitch); Serial.print(" ");Serial.print(getPitch()); Serial.print(" ");Serial.print(initialPitch - getPitch()); 
      if(initialPitch - getPitch() > IMU_THRESH) //on the ramp
      {
        Serial.print("irL: "); Serial.print(irL); Serial.print(" irR: "); Serial.println(irR); //Serial.print(" diff: "); Serial.println(irL - irR); 
        if(irL - irR < -1*IR_THRESH) // too far right
        {
          Serial.println("need to turn left");
          //reverse, turn CCW, forward
          if(ctr < 500)
          {
            lastCommand.leftV = -50;
            lastCommand.rightV = -50;
           // if(reverse(lastCommand))
              Serial.print("reverse "); Serial.print(lastCommand.leftV); Serial.print(" "); Serial.println(lastCommand.rightV);
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
        else if(irL - irR > IR_THRESH)
        {
          Serial.println("need to turn right");

          if(ctr < 500)
          {
            lastCommand.leftV = -50;
            lastCommand.rightV = -50;
           // if(reverse(lastCommand))
              Serial.print("reverse "); Serial.print(lastCommand.leftV); Serial.print(" "); Serial.println(lastCommand.rightV);
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
          //state = 2;     test    
        }
      }
      else
      {
        Serial.println("driving straight");
        lastCommand = driveStraight(initialYaw, lastCommand, GOAL_AVG);
      }
    }
    else if(state == 2)
    { // driving on ramp
      //Serial.print("ir: "); Serial.print(irL); Serial.println(" "); Serial.println(irR); Serial.println(" "); Serial.println(irL - irR); 
      //Serial.println("entered state 2");
      if((irL - irR) < -1*IR_THRESH || correction) //need to turn left
      {
        correction = true;
        Serial.println("Need to turn left");
        if(loopCtr++ < 5) 
        {
          lastCommand.rightV += DELTA;
          //Serial.print(loopCtr); Serial.print(" ");
        }   
        else 
        { //correction
          
          diff = initialYaw - getYaw();
          Serial.print("Diff: "); Serial.print(diff); Serial.print(" "); Serial.println("Correcting");
          
          if ((diff > -180 && diff < -1*IMU_THRESH) || (diff < (360-IMU_THRESH) && diff > 180)) //need to speed up right still
            lastCommand.rightV += DELTA;
          else if((diff > IMU_THRESH && diff < 180) || (diff < -180 && diff > (-360 + IMU_THRESH))) 
            lastCommand.leftV += DELTA;
          else
            correction = false;
        }
          
      }
      else if (irL - irR > IR_THRESH) //need to turn right
      {
        Serial.println("need to turn right");
        lastCommand.leftV += DELTA;
      }
      else //straight
      {
        Serial.println("driving straight");
        lastCommand = driveStraight(initialYaw, lastCommand, GOAL_AVG);
      }
    }
      return lastCommand;
  }  
  
};

#endif //RAMP_CLIMB_H
