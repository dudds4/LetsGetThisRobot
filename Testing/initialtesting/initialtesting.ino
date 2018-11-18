#include "nav.h"
#include "myImu.h"
#include "sensors.h"
#include <Adafruit_BNO055.h>

static MotorCommand newCommand;
static MotorCommand lastCommand;

Adafruit_BNO055 bno = Adafruit_BNO055(55);
sensors_event_t initial_imu;

void setup() {
  Serial.begin(9600);
  
  initializeIMU(bno);
  
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT); 
  pinMode(IN3,OUTPUT); 
  pinMode(IN4,OUTPUT); 
   
}

void loop() {
  Serial.println("entered loop");
  //initMotors();
  //newCommand = driveStraight(bno, initial_imu, lastCommand);
  delay(2000);
  motortest();
}
