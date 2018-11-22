#include "sensors.h"
#include "controls.h"
#include "rampfinding.h"
#include "rampclimbing.h"
#include "Adafruit_BNO055.h"

static MotorCommand lastCommand;
static TurnState ts;

double initialYaw;
double initialPitch;
double initialRoll;

void setup() 
{
  Serial.begin(9600);
  
  initializeIMU();
  initializeIR();
  initMotors();
  
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

  initialYaw = getYaw();
  Serial.println("Finished setup");
}

bool doneTurn = false;
bool initializedTest = false;
sensors_event_t initial_imu;
double turnAngle = 0;

enum Section { FindRamp, ClimbRamp, DriveStraightWIMU, DriveStraightWIR, DriveAndTurn, TurnAtWall, wallfIMU };
Section currentSection = FindRamp;
int state = 0;

unsigned loopCounter = 0;
RampFinder rampFinder;
RampClimber rampClimber;

void loop() 
{
  if(loopCounter++ < 20) 
  {
    getIR();
    delay(1);
    return;
  }
  loopCounter = 0;
  
  bool shouldPrint = false;
  static unsigned counter = 0;
  if(++counter > 25) { shouldPrint = true; counter = 0; }

  switch(currentSection)
  {
    case FindRamp:
      lastCommand = rampFinder.run(lastCommand);
      if(rampFinder.isDone())
        currentSection = ClimbRamp;
      break;

    case ClimbRamp:
      lastCommand = rampClimber.run(lastCommand);
      
      break;

    default: 
      lastCommand.reset();
      break;
  }
  
//  if(currentSection == RunMotors)
//  {
//    
//    sensors_event_t event; 
//    bno.getEvent(&event);
//
//  }
//  else if(currentTest == DriveStraightWIMU)
//  {
//    if(!initializedTest)
//    {
//      Serial.println("initializing drive straight /imu");
//      bno.getEvent(&initial_imu);
//      lastCommand.leftV = GOAL_AVG;
//      lastCommand.rightV = GOAL_AVG;
//      initializedTest = true; 
//    }
//  
//    lastCommand = driveStraight(initial_imu, lastCommand, GOAL_AVG);
//  }
//  else if(currentTest == DriveStraightWIR)
//  {
//    if(!initializedTest)
//    {
//      lastCommand.leftV = GOAL_AVG;
//      lastCommand.rightV = GOAL_AVG; 
//      initializedTest = true;
//    }
//  
//    // follow at 10 cm
//    lastCommand = wfIMU(initial_imu, 20, GOAL_AVG, lastCommand); //test
//  }
//  else if(currentTest == TurnAtWall)
//  {
//    if(!initializedTest)
//    {
//      lastCommand.leftV = GOAL_AVG;
//      lastCommand.rightV = GOAL_AVG; 
//    }
//    
//    // follow at 25 cm
//    lastCommand = turnAtWall(260, GOAL_AVG, lastCommand);
//  }
//  else if (currentTest == DriveAndTurn)
//  {
//    if(!initializedTest)
//    {
//      lastCommand.leftV = GOAL_AVG;
//      lastCommand.rightV = GOAL_AVG; 
//    }
//  
//    lastCommand = turnAtWall(260, GOAL_AVG, lastCommand);
//  }

  setMotorVoltage(motorLeft, lastCommand.leftV);
  setMotorVoltage(motorRight, lastCommand.rightV);

//  setMotorVoltage(motorLeft, 0);
//  setMotorVoltage(motorRight, 0);

}
