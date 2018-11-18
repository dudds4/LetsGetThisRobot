#include "myImu.h"
#include "sensors.h"
#include "controls.h"

#include <Adafruit_BNO055.h>

static MotorCommand newCommand;
static MotorCommand lastCommand;
static TurnState ts;
//ts.initialized = false;

Adafruit_BNO055 bno = Adafruit_BNO055(55);
sensors_event_t initial_imu;

void setup() {
  Serial.begin(9600);
  
//  initializeIMU(bno);
  
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT); 
  pinMode(IN3,OUTPUT); 
  pinMode(IN4,OUTPUT); 
  
  // change up PWM frequencies for motor PWM pins
  // Timer 3 for outputs 2/3/5
  bool fastpwm = false;

  if(fastpwm)
  {
    // change prescaler
    TCCR3B &= ~(0x07);
    TCCR3B |= 0x02;
  
    // set wave form generation mode to phase correct
    TCCR3A &= ~(0x03);
    TCCR3B &= ~(0x18);
    TCCR3A |= 0x01;
    
    
    // Timer 4 for outputs 6,7,8  
    TCCR4B &= ~(0x07);
    TCCR4B |= 0x02;
  
    // set wave form generation mode to phase correct
    TCCR4A &= ~(0x03);
    TCCR4B &= ~(0x18);
    TCCR4A |= 0x01;
  }


//  lastCommand.leftV = 200;
//  lastCommand.rightV = 200;
   
}

bool doneTurn = false;

void loop() {
  ir_data1 = analogRead(ir1);
  ir_data2 = analogRead(ir2);

  newCommand.rightV = 200;
  newCommand.leftV= 200;

//  if(!doneTurn)
//    doneTurn = turnOnSpot(ts, 90, &newCommand);
//  else
//  {
//   setMotorVoltage(ENA, IN1, IN2, 0);
//   setMotorVoltage(ENA2, IN3, IN4, 0);
//   while(1){}  
//  }
  
   setMotorVoltage(ENA, IN1, IN2, newCommand.rightV);
   setMotorVoltage(ENA2, IN3, IN4, newCommand.leftV);
  
  delay(20);
}
