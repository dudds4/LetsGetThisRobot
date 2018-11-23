#ifndef BASE_FIND_H
#define BASE_FIND_H

#include "nav.h"
#include "util.h"
#include "sensors.h"

struct BaseFinder
{
private:
  bool foundBase = false;
public:

  bool complete() { return foundBase; }
  
	MotorCommand run(MotorCommand lc)
  {
    if(!initialized) 
    {
      pitchI = getPitch();
      rollI = getRoll();
      yaw = getYaw(); 
      
      Serial.println("base finder starting...");
    }
    
    state = 99;
    
    if(state == 0)
    {
      if(frontIr.getDist() < 15)
      {
        state = 1;  
      }
      else
      {
        lc = driveStraight(yaw, lc, 200); 
      }
    }
    else if(state == 1)
    {
      // TODO: find heading
      state = 2;
      ts.reset();
    }
    else if(state == 2)
    {
      if(turnOnSpot(ts, 90, &lc))
      {
        state = 3;  
        yaw = getYaw();
      }
    }
    else if(state == 3) // drive backwards
    { 
      static unsigned long timeCounter = 0;
      if(++timeCounter > 1000)
      {
        state = 4;
        ts.reset();
      }
      else
      {
        lc = driveStraightBackward(yaw, lc, -200); 
      }
    }
    else if(state == 4) // turn
    {
      if(turnOnSpot(ts, -90, &lc))
      {
        state = 5;
        yaw = getYaw();
      }
    }
    else if(state == 5)
    {
      static unsigned long timeCounter = 0;
      if(++timeCounter > 500)
      {
        state = 6;
        ts.reset();
      }
      else
      {
        lc = driveStraightBackward(yaw, lc, -200); 
      }
    }
    else if(state == 6)
    {
      if(turnOnSpot(ts, 90, &lc))
      {
        state = 7;
        yaw = getYaw();
      }
    }
    else if(state == 7)
    {       
      static unsigned long timeCounter = 0;
      if(++timeCounter > 1000)
      {
        while(1) {}
      }
      else
      {
        lc = driveStraight(yaw, lc, -200); 
      }
    }
    else if(state == 100)
    {
      // we just saw the base on our right
      if(turnOnSpot(ts, 90, &mc))
      {
        state = 101;
        yaw = getYaw();  
      }
    }
    else if(state == 101)
    {
      driveStraight(yaw, mc, 200);
    }

    if(state < 100)
    {
      // check if we saw the base
      double irReading = rightIr.getDist();
      const double BASE_IR_TOL = 5;
      
      if(abs(irReading - lastIrReading) > BASE_IR_TOL && initialized)
      {
        Serial.println("Found base with ir");
        state = 100;
      }
      
      lastIrReading = irReading;
    }

    static unsigned uu = 0;
    if(++uu > 5)
    {
      uu=0;
    }

    // check if we ran over base
    const double ROTATION_TOL = 1;
    double pdiff = getPitch() - pitchI;
    double rdiff = getRoll() - rollI;
    double rotation = rdiff*rdiff+pdiff*pdiff;
    
    Serial.println(rotation);
    
    if(rotation > ROTATION_TOL) 
    {
      Serial.println("Found Base; rotation");
      state = 999;
    }

    initialized = true;
    return lc;
  }

private:
  int state = 0;
  int initialized = 0;
  double pitchI;
  double rollI;
  double yaw;
  
  double lastIrReading;
  
  TurnState ts;  
};

#endif
