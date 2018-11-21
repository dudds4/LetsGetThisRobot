#include "sensors.h"
#include "controls.h"
#include "rampfinding.h"
#include "Adafruit_BNO055.h"

static MotorCommand newCommand;
static MotorCommand lastCommand;
static TurnState ts;

void setup() 
{
  Serial.begin(9600);
  
  bool changePWMFrequencies = false;
  if(changePWMFrequencies)
  {    
    // change up PWM frequencies for motor PWM pins
    // Timer 3 for outputs 2/3/5
    TCCR1B &= ~(0x07);
    TCCR1B |= 0x02;
    
    // Timer 4 for outputs 9,10  
    TCCR2B &= ~(0x07);
    TCCR2B |= 0x02;
  }

}

bool doneTurn = false;
bool initializedTest = false;
sensors_event_t initial_imu;

enum Test { RunMotors, DriveStraightWIMU, DriveStraightWIR, DriveAndTurn, TurnAtWall };
Test currentTest = RunMotors;

void loop() 
{
  const int GOAL_AVG = 170;

  if(currentTest == RunMotors)
  {
    Serial.println("run motors");
  }
  else if(currentTest == DriveStraightWIMU)
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

//  setMotorVoltage(motorLeft, lastCommand.leftV);
//  setMotorVoltage(motorRight, lastCommand.rightV);
  
  delay(600);
}
