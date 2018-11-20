#ifndef CONTROLS_H
#define CONTROLS_H

#include "nav.h"
#include "sensors.h"
#include "util.h"

int facingWall = 0;

MotorCommand wallFollow(Adafruit_BNO055& bno, MotorCommand lastCommand) 
{
  const unsigned PERIOD = 10; 
  static unsigned counter = PERIOD;

  // if you want to run this control at a lower frequency, you can
  // skip logic and just continue with previous command
  // this is example code though, you can choose not to do this..
  if(++counter < PERIOD) { return lastCommand; }
  counter = 0;

  getIR();
  double ir1Avg = frontIr.getAvg();
  double ir2Avg = rightIr.getAvg();

  /****** IMU stuff ******/
  static bool initialized = false;                   
  static sensors_event_t initial_imu;
  if(!initialized)
  {
     bno.getEvent(&initial_imu);  
  }
  initialized = 1; 
	
  /*********************************** METAL WALL ***********************************/  
  
 /* static TurnState ts;
  static MotorCommand* mc;
  ts.initialized = false;
  unsigned int face = 0; // If rotate 90 CW, increment. If rotate 90 CCW, decrement

  if(irAnalogToCm(ir2Avg) > 20)
    bool wor = wallOnRight(ts, mc, bno, initial_imu, lastCommand);

  if(irAnalogToCm(ir1Avg) < 20) {
    if(turnOnSpot(ts, -90, mc))
      face--;
  }*/

 /*********************************** FIND RAMP 1***********************************/  
  static TurnState ts;
  static MotorCommand* mc;
  ts.initialized = false;
  unsigned int face = 0; // If rotate 90 CW, increment. If rotate 90 CCW, decrement

  //if(irAnalogToCm(ir2Avg) > 20) //
    //bool wor = wallOnRight(ts, mc, bno, initial_imu, lastCommand);

  if(irAnalogToCm(ir1Avg) < 33 && face < 3) { //NOTE: 13 inches (33cm)
    if(turnOnSpot(ts, 90, mc))
      face++;

    if(irAnalogToCm(ir2Avg) > 33) { //ir sensor on left
      bool tos = turnOnSpot(ts, -10, mc);
      for(int i = 0; i < 4; i++) {
//        MotorCommand drvStr = driveStraight(bno, initial_imu, lastCommand);
      }
    }
  }

  facingWall = initial_imu.orientation.x;
  
//  return driveStraight(bno, initial_imu, lastCommand);
}

MotorCommand climbRamp1(/* add params */) 
{}

/*MotorCommand findRamp2(TurnState ts, MotorCommand mc*,  bno, initial_imu, lastCommand, ir1Avg, ir2Avg) 
{
    int face = 0;
    
    if(initial_imu.orientation.x != facingWall)
      bool tos = turnOnSpot(ts, 5, mc);
    else if (ir1Avg > 33) {
      MotorCommand drvstr = driveStraight(bno, initial_imu, lastCommand);
    }
    else {
      if(irAnalogToCm(ir1Avg) < 33 && face < 3) { //NOTE: 13 inches (33cm)
        if(turnOnSpot(ts, 90, mc))
          face++;
      MotorCommand drvstr = driveStraight(bno, initial_imu, lastCommand);
    }  
}*/

MotorCommand search(/* add params */) 
{
  
  
}

MotorCommand return1(/* add params */) 
{}

MotorCommand ramp2(/* add params */) 
{}

MotorCommand return2(/* add params */) 
{}

#endif // CONTROLS_H
