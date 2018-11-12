#ifndef CONTROLS_H
#define CONTROLS_H

#include "nav.h"
#include "myImu.h"
#include "util.h"
#include "ir.h"

MotorCommand wallFollow(Adafruit_BNO055& bno, MotorCommand lastCommand) 
{
  const unsigned PERIOD = 10;
  static unsigned counter = PERIOD;
  if(++counter < PERIOD) return lastCommand;
  counter = 0;
  
  static bool initialized = false;
  static sensors_event_t initial_imu;
  if(!initialized)
  {
     bno.getEvent(&initial_imu);  
  }
  
	// example based on imu branch code
	// not actually good code

  return driveStraight(bno, initial_imu, lastCommand);
}

MotorCommand climbRamp1(/* add params */) 
{}


enum SearchState {  Positioning, FirstPass, Reposition, SecondPass, GetThisPost };
enum SearchSubState {  SSub1, SSub2, SSub3, SSub4, SSub5, SSub6, Complete };

SearchSubState searchPosition(SearchSubState ss, MotorCommand* mc)
{
  static unsigned counter = 0;
  static TurnState ts;
  
  switch(ss)
  {
    // open loop backing up
    case SSub1:
      mc->leftV = -100;
      mc->rightV = -100;
      if(++counter > 80)
      {
        ss = SSub2;
        counter = 0;  
      } 
      break;

    // turning to face wood wall
    case SSub2:
      if(counter == 0) 
      {
        ts.initialized = false;
        counter++;  
      }
      
      if(turnOnSpot(ts, -90, mc))
      {
        ss = SSub3;
        counter = 0;
      }
      break;

    // drive towards wall until value of n cm away
    case SSub3:
      // TODO: copy driving straight code
      break;

    case SSub4: 
      if(counter == 0) 
      {
        ts.initialized = false;
        counter++;  
      }
      
      if(turnOnSpot(ts, -90, mc))
      {
        ss = SSub5;
        counter = 0;
      }
      break;

    // follow with wall on right at certain distance
    // do so until wall in front is n away
    case SSub5:
      // TODO: copy driving straight code
      break;    
      
    // turn away from wall
    case SSub6:
      if(counter == 0) 
      {
        ts.initialized = false;
        counter++;  
      }
      
      if(turnOnSpot(ts, -90, mc))
      {
        ss = Complete;
        counter = 0;
      }
      break;
    
    default:
      mc->leftV = 0;
      mc->rightV = 0;
      break;
    
  }

  return ss;
}

SearchSubState searchFirstPass(SearchSubState ss, MotorCommand* mc)
{

  int irAnalog /* read analog value for right IR sensor */ ;
  int cm_r = irAnalogToCm(irAnalog);
  static int cm_r_last = cm_r;
  int diff = cm_r - cm_r_last;

  irAnalog  /* read analog value for front IR sensor */ ;
  int cm_f = irAnalogToCm(irAnalog);

  const int JUMP_THRESHOLD = 7;
  const int FRONT_THRESHOLD = 7;
  
  // if the range sensors value jumps, then we've found the post
  if(cm_r - cm_r_last > JUMP_THRESHOLD)
  {
    // post found
    ss = Complete;
  }
  else if(cm_f < FRONT_THRESHOLD)
  {
    // need to turn around
    ss = SSub6;  
  }
  
  // otherwise we just keep driving straight
  // TODO: copy driving straight code
}


SearchSubState searchGetThisPost(SearchSubState ss, MotorCommand* mc)
{

  static unsigned counter = 0;
  static TurnState ts;
  
  switch(ss)
  {
    // turn to face the target
    case SSub1:
      if(counter == 0) 
      {
        ts.initialized = false;
        counter++;  
      }
      
      if(turnOnSpot(ts, 90, mc))
      {
        ss = SSub2;
        counter = 0;
      }
      break;

    // drive straight
    case SSub2:
      // TODO: copy driving straight code
      break;
  }

  return ss;
}

SearchSubState searchRepositioning(SearchSubState ss, MotorCommand* mc)
{

  static unsigned counter = 0;
  static TurnState ts;
  
  switch(ss)
  {
    // turn to face the target
    case SSub1:
      if(counter == 0) 
      {
        ts.initialized = false;
        counter++;  
      }
      
      if(turnOnSpot(ts, -90, mc))
      {
        ss = SSub2;
        counter = 0;
      }
      break;

    // drive straight
    case SSub2:
      // TODO: copy driving straight code
      break;

    // turn before starting next path
    case SSub3:
      if(counter == 0) 
      {
        ts.initialized = false;
        counter++;  
      }
      
      if(turnOnSpot(ts, -90, mc))
      {
        ss = Complete;
        counter = 0;
      }
      break;
      
  }

  return ss;
}

MotorCommand search(/* add params */) 
{
  static SearchState state = Positioning;
  static SearchSubState subState = SSub1;

  MotorCommand mc;
  
  switch(state)
  {
    case Positioning:
      subState = searchPosition(subState, &mc);
      if(subState == Complete)
      {
        state = FirstPass;
        subState = SSub1;
      }
      break;
      
    case FirstPass:
      subState = searchFirstPass(subState, &mc);
      if(subState == Complete)  // found the post
      {
        state = GetThisPost;
        subState = SSub1;
      }
      else if(subState == SSub6) // got to end without finding post
      {
        state = Reposition;  
        subState = SSub1;
      }
      break;    
      
    case Reposition:
      subState = searchRepositioning(subState, &mc);
      if(subState == Complete)  // found the post
      {
        state = SecondPass;
        subState = SSub1;
      }
      break; 
      
    case GetThisPost:
      subState = searchGetThisPost(subState, &mc);
      if(subState == Complete)  // found the post -- done
        while(1){;}
        
      break;
    
  }

  return mc;
  
}

MotorCommand return1(/* add params */) 
{}

MotorCommand ramp2(/* add params */) 
{}

MotorCommand return2(/* add params */) 
{}

#endif // CONTROLS_H
