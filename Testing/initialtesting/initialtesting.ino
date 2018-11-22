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
//  Serial.println("before initialization");
  initializeIMU();
  initializeIR();
  initMotors();
//  Serial.println("after initialization");
  
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
Test currentTest = DriveStraightWIR;

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
  
  const int GOAL_AVG = 250;

  if(currentTest == RunMotors)
  {
    
    sensors_event_t event; 
    bno.getEvent(&event);

    static unsigned counter = 0;
    if(++counter > 25)
    {
//      double yaw = event.orientation.x;
//      Serial.println(yaw);
      
      double ravg = rightIr.getMedian();
      double favg = frontIr.getMedian();
      Serial.print("r: ");
      Serial.print(ravg);
      counter = 0;
      Serial.print(", f: ");
      Serial.println(irAnalogToCm(favg));  
    }
    
//    setMotorVoltage(motorLeft, 200);
//    setMotorVoltage(motorRight, 200);

  }
  else if(currentTest == DriveStraightWIMU)
  {
    if(!initializedTest)
    {
      Serial.println("initializing drive straight /imu");
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
      initializedTest = true;
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

//  setMotorVoltage(motorLeft, 200);
//  setMotorVoltage(motorRight, 200);

}
