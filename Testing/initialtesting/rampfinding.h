#ifndef RAMPFIND_H
#define RAMPFIND_H

#include "util.h"

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
