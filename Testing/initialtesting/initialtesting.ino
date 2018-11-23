#include "sensors.h"
#include "controls.h"
#include "rampfinding.h"
#include "rampclimbing.h"
#include "basefinding.h"

#include "Adafruit_BNO055.h"

static MotorCommand mc;

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
  
  Serial.println("Finished setup");
}

enum Section { FindRamp, ClimbRamp, BaseFinding };

unsigned loopCounter = 0;

RampFinder rampFinder;
RampClimber rampClimber;
BaseFinder baseFinder;

const bool TEST_SEPARATE = true;
Section currentSection = FindRamp;

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
    
  switch(currentSection)
  {
    case FindRamp:
      mc = rampFinder.run(mc);      
      if(rampFinder.isDone())
      {
        if(TEST_SEPARATE)
        {
          Serial.println("Completed section: find ramp");
          while(1) {}  
        } 
        else
          currentSection = ClimbRamp;
        
      }
      break;

    case ClimbRamp:
      mc = rampClimber.run(mc);
      if(rampClimber.isDone())
      {
        if(TEST_SEPARATE)
        {
          Serial.println("Completed section: climb ramp");
          while(1) {}  
        } 
        else
          currentSection = BaseFinding;
        
      }
      break;

    case BaseFinding:
      mc = baseFinder.run(mc);
      if(baseFinder.isDone())
      {
        if(TEST_SEPARATE)
        {
          Serial.println("Completed section: climb ramp");
          while(1) {}  
        } 
        else
          currentSection = BaseFinding;        
      }

      break;

    default: 
      mc.reset();
      break;
  }

  setMotorVoltage(motorLeft, mc.leftV);
  setMotorVoltage(motorRight, mc.rightV);
}


#include "basefinding.h"
