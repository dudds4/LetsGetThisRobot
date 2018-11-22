#include "sensors.h"
#include "controls.h"
#include "rampfinding.h"
#include "Adafruit_BNO055.h"

static MotorCommand newCommand;
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

enum Section { FindRamp, DriveStraightWIMU, DriveStraightWIR, DriveAndTurn, TurnAtWall, wallfIMU };
Section currentSection = FindRamp;
int state = 0;

unsigned loopCounter = 0;

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
  
  const int GOAL_AVG = 250;

  const double FWDIST = 27;
  const double ROB_OFFSET = 8;
  const double SWDIST = 30;

  switch(currentSection)
  {
    case FindRamp:
      
      if(state == 0)
      { // drive straight until front ir below threshold
        
        if(frontIr.getDist() < FWDIST)
        {
          Serial.println("Entering state 1");
          state = 1;
          ts.reset();
          lastCommand = MotorCommand();
        }
        else
        {
          lastCommand = driveStraight(initialYaw, lastCommand, GOAL_AVG);
        }
      }
      else if(state == 1)
      { // 90 deg turn left
        if(turnOnSpot(ts, -90, &lastCommand)) 
        { 
          Serial.println("Entering state 2");
          state = 2;
          initialYaw = getYaw();
          lastCommand.reset();
        }
      }
      else if(state == 2)
      { // Follow the wall!
        if(frontIr.getDist() < SWDIST)
        {
          Serial.println("Entering state 3");
          state = 3;
          lastCommand.reset();
          ts.reset();
        }
        else
        {
          lastCommand = wallFollow(initialYaw, FWDIST - ROB_OFFSET, GOAL_AVG, lastCommand);
        }
      }
      else if(state == 3)
      {
        if(turnOnSpot(ts, -90, &lastCommand)) 
        {
          Serial.println("Entering state 4");
          state = 4;
          initialYaw = getYaw();
          initialPitch = getPitch();
          initialRoll = getRoll();
          lastCommand.reset();
        } 
      }
      else if(state == 4)
      {
        const double PITCH_THRESHOLD = 10;
        // drive along the wall until we get that ramp booiiis
        if(abs(getPitch() - initialPitch) > PITCH_THRESHOLD)
        {
          Serial.print(getPitch());
          Serial.print(" ");
          Serial.print(getRoll());
          lastCommand.reset();
        }
        else
        {
          lastCommand = wallFollow(initialYaw, SWDIST - ROB_OFFSET, GOAL_AVG, lastCommand);
        }
      }
    
      break;

    default: 
      lastCommand = MotorCommand();
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
