#ifndef HOME_FIND_H
#define HOME_FIND_H

#include "nav.h"
#include "util.h"
#include "sensors.h"

struct HomeFinder
{
private:
  bool foundHome = false;
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

  bool isDone() { return foundHome; }
  
  MotorCommand run(MotorCommand lc)
  {
    if(!initialized) 
    {
      yaw = getYaw();  
      Serial.println("home finder starting...");
      initialized++;    
      state = 0;
    }

    if(initialized++ < 5) ///go back to 
    {
      pitchI = getPitch();
      rollI = getRoll();  
      lastIrReading = rightIr.getDist();
      return MotorCommand();
    }
    
    
    if(state == 0)
    {
      if(frontIr.getDist() < 25)
      {
        state = 1;  
        Serial.println("Entering State 1");
      }
      else
      {
        lc = driveStraight(yaw, lc, 200); 
      }
    }
    else if(state == 1) //skip
    {
      state = 2;
      Serial.println("Entering State 2");
      ts.reset();
    }
    else if(state == 2) //turn right
    {
      if(turnOnSpot(ts, 90, &lc))
      {
        state = 3;  
        Serial.println("Entering State 3");
        drivingYaw = getYaw();
        lastIrReading = rightIr.getDist(); 
        ts.reset();
      }
    }
    else if(state == 3)  //drive straight
    { 
      //static unsigned long timeCounter = 0;
      if(frontIr.getDist() < 20)
      {
        state = 4;
        Serial.println("Entering State 4");
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
        state = 5;
        Serial.println("Entering State 5");
        drivingYaw = getYaw();
      }
    }
    else if(state == 5)
    {
      static unsigned long timeCounter = 0;
      if(frontIr.getDist() < 15)
      {
        state = 6;
        Serial.println("Entering State 6");
        turnAngle = getTurnAngle(270);
        ts.reset();
      }
      else
      {
        lc = driveStraight(drivingYaw, lc, 200); 
      }
    }
    else if(state == 6)
    {
      if(turnOnSpot(ts, turnAngle, &lc))
      {
        state = 7;
        drivingYaw = getYaw();
      }
    }
    else if(state == 7)
    {       
      static unsigned long timeCounter = 0;
      if(frontIr.getDist() < 15)
      {
        while(1) {}
      }
      else
      {
        lc = driveStraight(drivingYaw, lc, -200); 
      }
    }
    else if(state == 100)
    {
      // we just saw the base on our right
      if(turnOnSpot(ts, 90, &lc))
      {
        state = 101;
        Serial.println("Entering State 101: driving towards base");
        drivingYaw = getYaw();  
      }
    }
    else if(state == 101)
    {
      lc = driveStraight(drivingYaw, lc, 200);
    }

    double irReading = rightIr.getDist();

    if(state > 2 && state < 100)
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
