#ifndef MYIMU_H
#define MYIMU_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "nav.h"

void initializeIMU(Adafruit_BNO055& bno)
{
  /* Initialise the sensor */
  if(!bno.begin()) 
  {
    Serial.print("No BNO055 detected!");
    while(1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);
}

#ifndef MAX
#define MAX(a,b) (a > b ? a : b)
#endif

#ifndef MIN
#define MIN(a,b) (a > b ? b : a)
#endif

MotorCommand translateWithinLimits(MotorCommand c)
{
  // if either command voltage is higher than MOTOR_V_MAX, we slide them both back down
  if(c.leftV > MOTOR_V_MAX || c.rightV > MOTOR_V_MAX)
  {
  	int offset = MAX(c.leftV - MOTOR_V_MAX, c.rightV - MOTOR_V_MAX);
  	c.leftV -= offset;
  	c.rightV -= offset;
  }

  if(c.leftV < -1 * MOTOR_V_MAX || c.rightV < -1 * MOTOR_V_MAX)
  {
  	int offset = -1 * MIN(c.leftV + MOTOR_V_MAX, c.rightV + MOTOR_V_MAX);
  	c.leftV += offset;
  	c.rightV += offset;
  }
}

MotorCommand driveStraight(Adafruit_BNO055& bno, sensors_event_t initial, MotorCommand lastCommand)
{
   /* Get a new sensor event */ 
  sensors_event_t imu_event; 
  bno.getEvent(&imu_event);

  int diff = initial.orientation.x - imu_event.orientation.x;
  
  if(abs(diff) > 3) 
  { // Robot went off course
    if((diff > 3 && diff < 180) || (diff < -180 && diff > -357)) 
    { 	// Robot turned left
      	//speed up left motor
		lastCommand.leftV += 30;
    }
    else if ((diff > -180 && diff <-3) || (diff < 357 && diff> 180)) 
    { 	// Robot turned right
      	//speed up right motor
		lastCommand.rightV += 30;
    }
  }
  else
  {
  	// maybe slowly make rightV == leftV?
  	// double avg = (lastCommand.leftV + lastCommand.rightV) / 2.0;
  	// lastCommand.leftV = 0.5 * avg + 0.5 * lastCommand.leftV;
  	// lastCommand.rightV = 0.5 * avg + 0.5 * lastCommand.rightV;
  }

  return translateWithinLimits(lastCommand);

}



#endif // MYIMU_H