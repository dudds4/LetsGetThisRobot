#include "sensors.h"
#include "controls.h"
#include "Adafruit_BNO055.h"

static MotorCommand newCommand;
static MotorCommand lastCommand;
static TurnState ts;

void setup() 
{
  Serial.begin(9600);
  
  initializeIMU(bno);
  initializeIR();
  initMotors();
  
  bool changePWMFrequencies = true;
  if(changePWMFrequencies)
  {    
    // change up PWM frequencies for motor PWM pins
    // Timer 3 for outputs 2/3/5
    TCCR3B &= ~(0x07);
    TCCR3B |= 0x02;
    
    // Timer 4 for outputs 6,7,8  
    TCCR4B &= ~(0x07);
    TCCR4B |= 0x02;
  }
}

bool doneTurn = false;
bool initializedTest = false;
sensors_event_t initial_imu;

enum Test { DriveStraightWIMU, DriveStraightWIR, DriveAndTurn, TurnAtWall };
Test currentTest = TurnAtWall;

void loop() 
{
  const int GOAL_AVG = 200;
  
  if(currentTest == DriveStraightWIMU)
  {
    if(!initializedTest)
    {
      Serial.println("initializing drive straight");
      bno.getEvent(&initial_imu);
      lastCommand.leftV = GOAL_AVG;
      lastCommand.rightV = GOAL_AVG;
      initializedTest = true; 
    }
  
    lastCommand = driveStraight(initial_imu, lastCommand, GOAL_AVG);
  }
  else if(currentTest == DriveStraightWIR)
  {
    if(!initializedTest)
    {
      lastCommand.leftV = GOAL_AVG;
      lastCommand.rightV = GOAL_AVG; 
    }
  
    // follow at 25 cm
    lastCommand = genWallFollow(25, GOAL_AVG, lastCommand);
  }
  else if(currentTest == TurnAtWall)
  {
    if(!initializedTest)
    {
      lastCommand.leftV = GOAL_AVG;
      lastCommand.rightV = GOAL_AVG; 
    }
    
    // follow at 25 cm
    lastCommand = turnAtWall(260, GOAL_AVG, lastCommand);
  }
  else if (currentTest == DriveAndTurn)
  {
    if(!initializedTest)
    {
      lastCommand.leftV = GOAL_AVG;
      lastCommand.rightV = GOAL_AVG; 
    }
  
    lastCommand = turnAtWall(260, GOAL_AVG, lastCommand);
  }
   
  setMotorVoltage(motorLeft, lastCommand.leftV);
  setMotorVoltage(motorRight, lastCommand.rightV);
  
  delay(20);
}
