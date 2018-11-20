#ifndef CONTROLS_H
#define CONTROLS_H

#include "nav.h"
#include "sensors.h"
#include "util.h"

int facingWall = 0;

MotorCommand wallFollow(Adafruit_BNO055& bno, MotorCommand lastCommand) 
{


unsigned int ir1Array[] = {0,0,0,0,0};
unsigned int ir2Array[] = {0,0,0,0,0};


  const unsigned PERIOD = 10; 
  static unsigned counter = PERIOD;
  
  if(++counter < PERIOD) 
  {
    for(int i = sizeof(ir1Array); i>0; i--) 
    { //shift values
      ir1Array[i] = ir1Array[i-1];
      ir2Array[i] = ir2Array[i-1];
    }
    
    ir1Array[0] = analogRead(ir1);
    ir2Array[0] = analogRead(ir2);
    
    return lastCommand;
  }
  
  counter = 0;

  int sum = 0; 
  int sum2 = 0;
  for(int i = sizeof(ir1Array); i>0; i--) 
  { 
      //take avg and discard rly out-of-range values
      sum = sum + ir1Array[i];
      sum2 = sum2 + ir2Array[i];
  }
  double ir1Avg = sum / 5;
  double ir2Avg = sum2 / 5;

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
