#ifndef BASE_FIND_H
#define BASE_FIND_H

#include "nav.h"
#include "util.h"
#include "sensors.h"

struct BaseFinder
{
private:
  bool foundBase = false;
  double turnAngle;
  double drivingYaw;

  double getTurnAngle(double goalYaw)
  {
        double tA = goalYaw - (getYaw() - yaw);
        
        if(tA > 180) tA -= 360;
        if(tA < -180) tA += 360;  
        return tA;
  }
  
public:

  bool isDone() { return foundBase; }
  
	MotorCommand run(MotorCommand lc)
  {
    if(!initialized) 
    {
      yaw = getYaw();  
      Serial.println("Base finder starting..., state 0");
      initialized++;    
      state = 0;
    }

    if(initialized++ < 5)
    {
      pitchI = getPitch();
      rollI = getRoll();  
      lastIrReading = rightIr.getDist();
      return MotorCommand();
    }
    
    if(state == 0) // driving forward
    {
      if(frontIr.getDist() < 35)
      {
        state = 1;  
        Serial.println("Entering State 1");
      }
      else
      {
        lc = driveStraight(yaw, lc, 200);
      }
    }
    else if(state == 1) // skip
    {
      // TODO: find heading
      state = 2;
      Serial.println("Entering State 2");
      ts.reset();
    }
    else if(state == 2) // turn
    {
      if(turnOnSpot(ts, -90, &lc))
      {
        state = 3;    
        Serial.println("Entering State 3: Driving Backwards");
        drivingYaw = getYaw();
        lastIrReading = rightIr.getDist();
      }
    }
    else if(state == 3) // drive forwards
    { 
      static unsigned long timeCounter = 0;
      if(++timeCounter > 15)
      {
        state = 4;
        Serial.println("Entering State 4: Turn");
        turnAngle = getTurnAngle(180);
        ts.reset();
      }
      else
      {
        lc = driveStraight(drivingYaw, lc, 200); 
      }
    }
    else if(state == 4) // turn
    {
      if(turnOnSpot(ts, turnAngle, &lc))
      {
        Serial.println("Entering State 5");
        state = 5;
        drivingYaw = getYaw();
        lastIrReading = rightIr.getDist();
      }
    }
    else if(state == 5) // drive straight
    {
      if(frontIr.getDist() < 35)
      {
        state = 6;
        turnAngle = getTurnAngle(-90);
        ts.reset();  
        lc.reset();
        Serial.println("Entering State 6");
      }
      else
      {
        lc = driveStraight(drivingYaw, lc, 150); 
      }
    }
    else if(state == 6)
    {
      if(turnOnSpot(ts, turnAngle, &lc))
      {
        Serial.println("Entering State 7");
        state = 7;
        drivingYaw = getYaw();
      }
    }
    else if(state == 7)
    {       
      
      static unsigned long timeCounter = 0;
      if(frontIr.getDist() < 35)
      {
        turnAngle = getTurnAngle(0);
        state = 8;
        ts.reset();
        Serial.println("Entering State 8");
      }
      else
      {
        lc = driveStraight(drivingYaw, lc, -200); 
      }
    }
    else if(state == 8)
    {
      if(turnOnSpot(ts, turnAngle, &lc))
      {
        Serial.println("Entering State 9");
        state = 9;
        drivingYaw = getYaw();
      }
    }
    else if(state == 9)
    {       
      
      if(frontIr.getDist() < 35)
      {
        Serial.println("Giving up");
        while(1) {}

      }
      else
      {
        lc = driveStraight(drivingYaw, lc, 150); 
      }
    }
    else if(state == 10)
    {
      if(turnOnSpot(ts, turnAngle, &lc))
      {
        Serial.println("Entering State 11");
        state = 11;
        drivingYaw = getYaw();
      }
    }
    else if(state == 11)
    {       
      
      static unsigned long timeCounter = 0;
      if(++timeCounter > 1000)
      {
        turnAngle = getTurnAngle(0);
        state = 12;
        ts.reset();
        Serial.println("Entering State 12");
      }
      else
      {
        lc = driveStraight(drivingYaw, lc, 150); 
      }
    }
    else if(state == 100) // we've found the post
    {      
      if(turnOnSpot(ts, 90, &lc))
      {
        state = 101;
        drivingYaw = getYaw();  
      }
    }
    else if(state == 101)
    {
      driveStraight(drivingYaw, lc, 200);
    }
    
    double irReading = rightIr.getDist();

    if(state > 4 && state < 100)
    {
      // check if we saw the base
//      Serial.println(irReading);
      const double BASE_IR_TOL = 20;
      
      if(abs(irReading - lastIrReading) > BASE_IR_TOL && irReading < 70 && initialized)
      {
        Serial.print(irReading);
        Serial.print(" ");
        Serial.println(lastIrReading);
        Serial.println("Found base with ir");
        state = 100;
        lc.reset();
      }
    }
    
    lastIrReading = irReading;
//
//    // check if we ran over base
//    const double ROTATION_TOL = 40;
//    double pdiff = getPitch() - pitchI;
//    double rdiff = getRoll() - rollI;
//    double rotation = rdiff*rdiff+pdiff*pdiff;
//    
////    Serial.println(rotation);
//    
//    if(rotation > 30) 
//    {
//      Serial.println("Found Base; rotation");
//      state = 999;
//    }

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
