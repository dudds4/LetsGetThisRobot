#include "sensors.h"
#include "controls.h"
#include "rampfinding.h"
#include "rampclimbing.h"

#include "Adafruit_BNO055.h"


static MotorCommand mc;
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
  
  bool changePWMFrequencies = true;
  if(changePWMFrequencies)
  {    
    // change up PWM frequencies for motor PWM pins
    TCCR3B &= ~(0x07);
    TCCR3B |= 0x02;
    TCCR4B &= ~(0x07);
    TCCR4B |= 0x02;
  }

  initialYaw = getYaw();
  initialPitch = getPitch();
  initialRoll = getRoll();
  ts.reset();
  Serial.println("Finished setup");
}

bool doneTurn = false;
bool initializedTest = false;
sensors_event_t initial_imu;

enum Section { FindRamp, ClimbRamp, DriveStraightWIMU, DriveStraightWIR, DriveAndTurn, TurnAtWall, wallfIMU };
Section currentSection = FindRamp;
int state = 0;

unsigned loopCounter = 0;
RampFinder rampFinder;
RampClimber rampClimber;
//BaseFinder baseFinder;
double turnAngle = 90;

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
  if(++counter > 18) { shouldPrint = true; counter = 0; }

//  mc = baseFinder.run(mc);
    
//  switch(currentSection)
//  {
//    case FindRamp:
//      lastCommand = rampFinder.run(lastCommand);
//      Serial.print(frontIr.getDist());
//      Serial.print(" ");
//      Serial.println(rightIr.getDist());
//      
////      if(rampFinder.isDone())
////        currentSection = ClimbRamp;
//      break;
//
//    case ClimbRamp:
//      lastCommand = rampClimber.run(lastCommand);
//      
//      break;
//
//    default: 
//      lastCommand.reset();
//      break;
//  }

  setMotorVoltage(motorLeft, mc.leftV);
  setMotorVoltage(motorRight, mc.rightV);

}


#include "basefinding.h"
