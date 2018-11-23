#include "sensors.h"
#include "controls.h"
#include "rampfinding.h"
#include "rampclimbing.h"
#include "basefinding.h"
#include "homefinding.h"

#include "Adafruit_BNO055.h"

static MotorCommand mc;

#define BTN_HI 23
#define BTN_LOW 24
#define BTN_SGL 25

double initialYaw;

void setup() 
{
  Serial.begin(9600);

  pinMode(BTN_HI, OUTPUT);
  pinMode(BTN_LOW, OUTPUT);
  pinMode(BTN_SGL, INPUT);
  delay(1);
  digitalWrite(BTN_HI, HIGH);
  digitalWrite(BTN_LOW, LOW);
  unsigned counter = 0;
  while(counter < 40)
  {
    if(digitalRead(BTN_SGL) == HIGH) counter++;
    else counter = 0;
    delay(1);   
  }

  initialYaw = getYaw();
  
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
  
  Serial.println("Finished setup");
}

enum Section { FindRamp, ClimbRamp, BaseFinding , comingHome};

unsigned loopCounter = 0;

RampFinder rampFinder;
RampClimber rampClimber;
BaseFinder baseFinder;
HomeFinder homeFinder;

const bool TEST_SEPARATE = true;

void killMotors()
{
  setMotorVoltage(motorLeft, 0);
  setMotorVoltage(motorRight, 0);  
}

TurnState ts;
Section currentSection = comingHome;

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

//  if(turnOnSpot(ts, 90, &mc))
//  {
//    killMotors();
//    while(1) ;
//  }
//    
//  setMotorVoltage(motorLeft, mc.leftV);
//  setMotorVoltage(motorRight, mc.rightV);
//
//  return;
  
  switch(currentSection)
  {
    case FindRamp:
      mc = rampFinder.run(mc);      
      if(rampFinder.isDone())
      {
        if(TEST_SEPARATE)
        {
          Serial.println("Completed section: find ramp");
          killMotors();
          while(1) {}  
        } 
        else
        {
          Serial.println("Entering Climb Ramp State"); 
          currentSection = ClimbRamp;
        }
        
      }
      break;

    case ClimbRamp:

      mc = rampClimber.run(mc);
      if(rampClimber.isDone())
      {
        if(TEST_SEPARATE)
        {
          Serial.println("Completed section: climb ramp");
          killMotors();
          while(1) {}  
        } 
        else
        {
          currentSection = BaseFinding;    
          Serial.println("Entering Find Base State");  
        }
        
      }
      break;

    case BaseFinding:
      mc = baseFinder.run(mc);
      if(baseFinder.isDone())
      {
        Serial.println("Basefinder done");
        if(TEST_SEPARATE)
        {
          Serial.println("Completed section: base finding");
          killMotors();
          while(1) {}  
        } 
        else
          currentSection = BaseFinding;        
      }

      break;

      case comingHome:
      mc = homeFinder.run(mc);
      if(homeFinder.isDone())
      {
        if(TEST_SEPARATE)
        {
          Serial.println("Completed section: climb ramp");
          while(1) {}  
        } 
        else
          currentSection = comingHome;        
      }

      break;

    default: 
      mc.reset();
      break;
  }

  setMotorVoltage(motorLeft, mc.leftV);
  setMotorVoltage(motorRight, mc.rightV);
}
