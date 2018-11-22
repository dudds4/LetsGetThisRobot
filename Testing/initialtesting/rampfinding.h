#ifndef RAMPFIND_H
#define RAMPFIND_H

#include "util.h"

const double FWDIST = 27;
const double ROB_OFFSET = 8;
const double SWDIST = 25;
const int GOAL_AVG = 250;

struct RampFinder 
{
  TurnState ts;
  double initialYaw, initialPitch, initialRoll;
  int state = 0;

  bool isDone() { return true; }
  
  MotorCommand run(MotorCommand lastCommand)
  {  
      if(state == 0)
      { // drive straight until front ir below threshold
        
        if(frontIr.getDist() < FWDIST)
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
      else if(state == 1)
      { // 90 deg turn CCW
        if(turnOnSpot(ts, -90, &lastCommand)) 
        { 
          Serial.println("Entering state 2");
          state = 2;
          initialYaw = getYaw();
          lastCommand.reset();
        }
      }
      else if(state == 2)
      { // Follow the wall!
        if(frontIr.getDist() < SWDIST)
        {
          Serial.println("Entering state 3");
          state = 3;
          lastCommand.reset();
          ts.reset();
        }
        else
        {
          lastCommand = wallFollow(initialYaw, FWDIST - ROB_OFFSET, GOAL_AVG, lastCommand);
        }
      }
      else if(state == 3)
      { // 90 deg turn CCW
        if(turnOnSpot(ts, -90, &lastCommand)) 
        {
          Serial.println("Entering state 4");
          state = 4;
          initialYaw = getYaw();
          initialPitch = getPitch();
          initialRoll = getRoll();
          lastCommand.reset();
        } 
      }
      else if(state == 4)
      { // drive forward until make contact with ramp
        const double PITCH_THRESHOLD = 10;
        // drive along the wall until we get that ramp booiiis
        if(abs(getPitch() - initialPitch) > PITCH_THRESHOLD)
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


// drives straight until it sees a wall, then turns, and starts wall following..
// not really a utility function, this needs to move
//
//MotorCommand turnAtWall(double dist, int goalAvg, MotorCommand lastCommand)
//{
//
//  getIR();  //update ir readings
//  static TurnState ts;
//  static int state = 0;
//
//  if(state == 0)
//  {
//    double frontDist = irAnalogToCm(frontIr.getAvg());
//
//    if (frontDist <= dist)
//    {
//      state = 1;
//      ts = TurnState();
//      Serial.println("Transition to turning state");  
//    } 
//    else
//      return genWallFollow(dist, goalAvg, lastCommand);    
//  }
//  else if(state == 1)
//  {
//     if(turnOnSpot(ts, 90, &lastCommand))
//     {
//        lastCommand.rightV = goalAvg;
//        lastCommand.leftV = goalAvg;
//        
//        Serial.println("Transition to second driving state");
//        state = 2; 
//     }
//
//     return lastCommand;
//  }
//  else if(state == 2)
//  {
//    return genWallFollow(dist, goalAvg, lastCommand);        
//  }
//  
//}

#endif //RAMPFIND_H
