#include "myImu.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55);
sensors_event_t initial_imu;

unsigned long loopCounter = 0;

enum CourseState
{
  WallFollowing,
  Ramp1,
  Search,
  Return1,
  Ramp2,
  Return2
}

CourseState cState = WallFollowing;
MotorCommand motCmd;

unsigned long sleepUntilTime;
const unsigned LOOP_PERIOD = 10;

void setup(void)
{
  // Serial.begin(9600);

  initializeIMU(bno);

  bno.getEvent(&initial_imu);

  sleepUntilTime = millis();
}

void loop(void)
{
  loopCounter++;

  // figure out what to set our motors two based on current state / strategy
  switch(cState)
  {
    case WallFollowing  : motCmd = wallFollow(motCmd);  break;
    case Ramp1          : motCmd = climbRamp1(/* add params */);  break;
    case Search         : motCmd = search(/* add params */);      break;
    case Return1        : motCmd = return1(/* add params */);     break;
    case Ramp2          : motCmd = ramp2(/* add params */);       break;
    case Return2        : motCmd = return2(/* add params */);  break;
  }

  // Run motor commands - this will depend on the motor driver module we get

  // analogWrite(motorL, motCmd.leftV);
  // analogWrite(motorR, motCmd.rightV);

  // sleep until next cycle
  sleepUntilTime += LOOP_PERIOD;
  long sleepFor = sleepUntilTime - millis();
  if(sleepFor > 0) delay(sleepFor);
}




