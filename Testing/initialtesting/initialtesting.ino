
#include "sensors.h"
#include "controls.h"
#include <Adafruit_BNO055.h>

static MotorCommand newCommand;
static MotorCommand lastCommand;
static TurnState ts;
//ts.initialized = true;

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
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

enum Test { DriveStraightWIMU, DriveStraightWIR };

void loop() 
{
  Test currentTest = DriveStraightWIR;

  
  
  //ir_data1 = analogRead(ir1);
  //ir_data2 = analogRead(ir2);
  
  //
  //newCommand = driveStraight(bno, initial_imu, lastCommand);

  //if(getIR()); //updates ir values
   //Serial.println("Updated IR");

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
    
    lastCommand = driveStraight(bno, initial_imu, lastCommand, GOAL_AVG);
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
   
  /*if(ir1Avg > 450) {
    doneTurn = false;
  }
  if(!doneTurn)
    Serial.println("turn");*/
  /*if(!doneTurn) {
    doneTurn = turnOnSpot(ts, 90, &newCommand);
  }
  else     
    newCommand = driveStraight(bno, initial_imu, lastCommand);*/
  
  /*else
  {
   setMotorVoltage(ENA, IN1, IN2, 0);
   setMotorVoltage(ENA2, IN3, IN4, 0);
   while(1){}  
  }*/
  
//   printCommand(lastCommand);
   setMotorVoltage(motorLeft, lastCommand.leftV);
   setMotorVoltage(motorRight, lastCommand.rightV);
  
  delay(20);
}
