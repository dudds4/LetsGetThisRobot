#ifndef UTIL_H
#define UTIL_H

#include "myImu.h"
#include "controls.h"


void setMotorVoltage(int EN, int IN1, int IN2, int v)
{
   if(v > 255) v = 255;
   else if(v < -255) v = -255;
   
   digitalWrite(EN, HIGH);// motor speed 
   if(v > 0)
   {
     analogWrite(IN2, v); //right motor
     digitalWrite(IN1,LOW); 
   }
   else
   {
     analogWrite(IN1, -1*v); //right motor
     digitalWrite(IN2,LOW);  
   }
}


struct TurnState 
{
  bool initialized = false;
  int initialYaw;
  int lastYaw;
  int goalYaw;
};

Adafruit_BNO055 g_bno;

int getYaw()
{
    sensors_event_t imu_event; 
    g_bno.getEvent(&imu_event);
    return imu_event.orientation.x;
}

bool turnOnSpot(TurnState &ts, int deg, MotorCommand* mc)
{
  if(!ts.initialized)
  {
    Serial.println("initializing");
    ts.initialYaw = getYaw();
    ts.lastYaw = ts.initialYaw;
    ts.goalYaw = ts.initialYaw + deg;
    ts.initialized = true;
  }

  int yaw = getYaw();

  // check wrap around
  const int WRAP_THRESH = 30;
  
  // case 1: we've wrapped around from 360 to 0
  if(ts.lastYaw > (360-WRAP_THRESH) && yaw < WRAP_THRESH)
    ts.goalYaw -= 360;
  // case 2: we've wrapped around from 0 to 360
  else if(ts.lastYaw < WRAP_THRESH && yaw > (360-WRAP_THRESH))
    ts.goalYaw += 360;

  ts.lastYaw = yaw;

  // compute directions to turn
  int diff =  ts.goalYaw - yaw;

  Serial.print(yaw);
  Serial.print(" ");
  Serial.println(ts.goalYaw);
  
  // case 1: we've finished the turn
  const int DONE_THRESHOLD = 3;
  if(abs(diff) < DONE_THRESHOLD)
  {
      mc->leftV = 0;
      mc->rightV = 0;
      return true;
  }

  const int MOTOR_V = 200;
  
  // case 2: we need to turn CW
  if(diff < 0)
  {
    mc->leftV = MOTOR_V;
    mc->rightV = -1 * MOTOR_V;
  }
  // case 3: we need to turn CCW
  else
  {
    mc->leftV = -1 * MOTOR_V;
    mc->rightV = MOTOR_V;
  }


  return false;
}

bool wallOnRight(TurnState ts, MotorCommand* mc, Adafruit_BNO055& bno, sensors_event_t initial_imu, MotorCommand lastCommand) { //make sure wall is on right at 20 cm away (use second ir)
  bool tos = turnOnSpot(ts, 10, mc);
  for(int i = 0; i < 4; i++) {
    MotorCommand drvStr = driveStraight(bno, initial_imu, lastCommand);
  }
  return turnOnSpot(ts, -10, mc);
  
}

#endif
