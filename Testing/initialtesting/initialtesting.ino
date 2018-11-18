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
  
  initializeIMU(bno);
  
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT); 
  pinMode(IN3,OUTPUT); 
  pinMode(IN4,OUTPUT); 

  lastCommand.leftV = 200;
  lastCommand.rightV = 200;
   
}

void loop() {
  ir_data1 = analogRead(ir1);
  ir_data2 = analogRead(ir2);
  //initMotors();
  //newCommand = driveStraight(bno, initial_imu, lastCommand);
  //Serial.print("Distance: "); Serial.println(ir_data1); 
  //if(ir_data1 > 400) { //NOTE: 13 inches (33cm)
    //Serial.println(irAnalogToCm(ir_data1));
  bool tos = turnOnSpot(ts, 90, &newCommand);

   digitalWrite(ENA, HIGH);// motor speed 
   if(newCommand.rightV > 0)
   {
     analogWrite(IN2, newCommand.rightV); //right motor
     digitalWrite(IN1,LOW); 
   }
   else
   {
     analogWrite(IN1, -1*newCommand.rightV); //right motor
     digitalWrite(IN2,LOW);  
   }

   digitalWrite(ENA2, HIGH);
   if(newCommand.leftV > 0)
   {
     analogWrite(IN4, newCommand.leftV); //right motor
     digitalWrite(IN3,LOW); 
   }
   else
   {
     analogWrite(IN3, -1*newCommand.leftV); //right motor
     digitalWrite(IN4,LOW);  
   }
  
  Serial.println(tos);
  //}
  delay(600);
  //motortest();
}
