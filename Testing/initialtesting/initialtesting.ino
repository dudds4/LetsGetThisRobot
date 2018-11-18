#include "myImu.h"
#include "sensors.h"
#include "controls.h"
#include <Adafruit_BNO055.h>

static MotorCommand newCommand;
static MotorCommand lastCommand;
static TurnState ts;
//ts.initialized = true;

Adafruit_BNO055 bno = Adafruit_BNO055(55);
sensors_event_t initial_imu;

void setup() {
  Serial.begin(9600);
  
  initializeIMU(bno);
  initializeIR();
  
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT); 
  pinMode(IN3,OUTPUT); 
  pinMode(IN4,OUTPUT); 
  
  // change up PWM frequencies for motor PWM pins
  // Timer 3 for outputs 2/3/5
  TCCR3B &= ~(0x07);
  TCCR3B |= 0x02;
  
  // Timer 4 for outputs 6,7,8  
  TCCR4B &= ~(0x07);
  TCCR4B |= 0x02;

  lastCommand.leftV = 200;
  lastCommand.rightV = 200;
   
}

bool doneTurn = false;

void setMotorVoltage(int EN, int IN1, int IN2, int v)
{
   if(v > 255) v = 255;
   else if(v < -255) v = -255;
   
   digitalWrite(EN, HIGH);// motor speed 
   if(v > 0)
   {
     analogWrite(IN2, v); //right motor
     digitalWrite(IN1,LOW); 
   }
   else
   {
     analogWrite(IN1, -1*v); //right motor
     digitalWrite(IN2,LOW);  
   }
}

void loop() {
  //ir_data1 = analogRead(ir1);
  //ir_data2 = analogRead(ir2);
  
  //initMotors();
  //newCommand = driveStraight(bno, initial_imu, lastCommand);

  //if(getIR()); //updates ir values
   //Serial.println("Updated IR");

  newCommand = driveStraight(bno, initial_imu, lastCommand);

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
  
  // setMotorVoltage(ENA, IN1, IN2, newCommand.rightV);
  // setMotorVoltage(ENA2, IN3, IN4, newCommand.leftV);
  
  delay(20);
}
