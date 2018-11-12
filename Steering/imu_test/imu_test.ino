#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

int xval=0;

void setup(void)
{
  Serial.begin(9600);

  /* Initialise the sensor */
  if(!bno.begin()) {
    Serial.print("No BNO055 detected!");
    while(1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);

  sensors_event_t imu_event; 
  bno.getEvent(&imu_event);

  xval = imu_event.orientation.x, 4;

}

void loop(void)
{
   /* Get a new sensor event */ 
  sensors_event_t imu_event; 
  bno.getEvent(&imu_event);
  
  /* Display the floating point data */
  //Serial.print("X: ");  Serial.println(imu_event.orientation.x, 4);
  //Serial.print("\tY: ");  Serial.print(imu_event.orientation.y, 4);
  //Serial.print("\tZ: ");  Serial.println(imu_event.orientation.z, 4);

  delay(200);

  int diff = xval - imu_event.orientation.x;
  Serial.println(diff);
    
  if(abs(diff) > 3) { // Robot went off course
    if((diff > 3 && diff < 180) || (diff < -180 && diff >-357)) { // Robot turned left
      //speed up left motor
      Serial.println("Turn right");
    }
    else if ((diff > -180 && diff <-3) || (diff < 357 && diff> 180)) { // Robot turned right
      //speed up right motor
      Serial.println("Turn left");
    }
  }
  else {
    // Run motors normally 
    Serial.println("Going straight");
  } 
  
  delay(200);
}

/*
    if(abs(diff) > 10) { // Robot went off course
    if((diff >= 10 && diff < 100) || (diff <=350 && diff > -250)) { // Robot turned left
      //speed up left motor
      Serial.println("Turn right");
    }
    else if ((diff <=-10 && diff > -100) || (diff <= 350 && diff> 250)) { // Robot turned right
      //speed up right motor
      Serial.println("Turn left");
    }
  }
*/


