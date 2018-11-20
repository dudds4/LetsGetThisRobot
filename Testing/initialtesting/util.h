#ifndef UTIL_H
#define UTIL_H

void printCommand(MotorCommand mc)
{
  Serial.print(mc.leftV);
  Serial.print(" ");
  Serial.println(mc.rightV);  
}

void setMotorVoltage(Motor m, int v)
{
   if(v > 255) v = 255;
   else if(v < -255) v = -255;
   
   digitalWrite(m.en, HIGH);// motor speed 
   if(v > 0)
   {
     analogWrite(m.in2, v); //right motor
     digitalWrite(m.in1, LOW); 
   }
   else
   {
     analogWrite(m.in1, -1*v); //right motor
     digitalWrite(m.in2,LOW);  
   }
}

struct TurnState 
{
  bool initialized = false;
  int initialYaw;
  int lastYaw;
  int goalYaw;
};

Adafruit_BNO055 g_bno;

int getYaw()
{
    sensors_event_t imu_event; 
    g_bno.getEvent(&imu_event);
    return imu_event.orientation.x;
}

bool turnOnSpot(TurnState &ts, int deg, MotorCommand* mc)
{
  if(!ts.initialized)
  {
    Serial.println("initializing");
    ts.initialYaw = getYaw();
    ts.lastYaw = ts.initialYaw;
    ts.goalYaw = ts.initialYaw + deg;
    ts.initialized = true;
  }

  int yaw = getYaw();

  // check wrap around
  const int WRAP_THRESH = 30;
  
  // case 1: we've wrapped around from 360 to 0
  if(ts.lastYaw > (360-WRAP_THRESH) && yaw < WRAP_THRESH)
    ts.goalYaw -= 360;
  // case 2: we've wrapped around from 0 to 360
  else if(ts.lastYaw < WRAP_THRESH && yaw > (360-WRAP_THRESH))
    ts.goalYaw += 360;

  ts.lastYaw = yaw;

  // compute directions to turn
  int diff =  ts.goalYaw - yaw;

  Serial.print(yaw);
  Serial.print(" ");
  Serial.println(ts.goalYaw);
  
  // case 1: we've finished the turn
  const int DONE_THRESHOLD = 3;
  if(abs(diff) < DONE_THRESHOLD)
  {
      mc->leftV = 0;
      mc->rightV = 0;
      return true;
  }

  const int MOTOR_V = 200;
  
  // case 2: we need to turn CW
  if(diff < 0)
  {
    mc->leftV = MOTOR_V;
    mc->rightV = -1 * MOTOR_V;
  }
  // case 3: we need to turn CCW
  else
  {
    mc->leftV = -1 * MOTOR_V;
    mc->rightV = MOTOR_V;
  }


  return false;
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

  if(c.leftV < 0) c.leftV = 0;
  if(c.rightV < 0) c.rightV = 0;

  return c;
}

MotorCommand driveStraight(Adafruit_BNO055& bno, sensors_event_t initial, MotorCommand lastCommand, int goalAvg)
{
   /* Get a new sensor event */ 
  sensors_event_t imu_event; 
  bno.getEvent(&imu_event);

  int diff = initial.orientation.x - imu_event.orientation.x;

  const int DIFF_THRESH = 5;
  const int V_STEP = 10;
  
  if(abs(diff) > DIFF_THRESH) 
  { // Robot went off course
    if((diff > DIFF_THRESH && diff < 180) || (diff < -180 && diff > (-360 + DIFF_THRESH))) 
    {   
        // Robot turned left
        //speed up left motor
        lastCommand.leftV += V_STEP;
    }
    else if ((diff > -180 && diff < -1*DIFF_THRESH) || (diff < (360-DIFF_THRESH) && diff > 180)) 
    {   
        // Robot turned right
        //speed up right motor
        lastCommand.rightV += V_STEP;
    }
  }
  else
  {
    // maybe slowly make rightV == leftV?
     double avg = (lastCommand.leftV + lastCommand.rightV) / 2.0;
     double multi = sqrt(abs(goalAvg / avg)) * avg / abs(avg); 
     
     lastCommand.leftV =  multi * 0.5 * (avg + lastCommand.leftV);
     lastCommand.rightV = multi * 0.5 * (avg + lastCommand.rightV);     

  }

  return translateWithinLimits(lastCommand);

}

//MotorCommand detFrontWall(double distance, int goalAvg, MotorCommand lastCommand)
//{
//  getIR();  //update ir readings
//  
//  double frontDist = ir1Avg;
//  double sideDist = ir2Avg;
//  static int counter = 0;
//  if(++counter > 10)
//  {
//    //Serial.print(ir2Avg);
//    //Serial.print(rDist);
//    //Serial.print(" ");
//    counter=0;
//  }
//
//  double diff = distance - frontDist;
//  Serial.print(frontDist);
//  Serial.print(" ");
//  Serial.println(diff);
//
//  const double DIFF_THRESH = 50;
//  const int V_STEP = 30;
//
//   if(diff > DIFF_THRESH)
//  {
//    //do nothing 
//  }
//  else if(diff < DIFF_THRESH)
//  {
//   while(sideDist != frontDist)
//   {
//      lastCommand.rightV = 0;
//      lastCommand.leftV = V_STEP;
//   }
//  }
//}





MotorCommand genWallFollow(double distance, int goalAvg, MotorCommand lastCommand)
{
  getIR();  //update ir readings
  
  double rDist = ir2Avg;
  static int counter = 0;
  if(++counter > 10)
  {
    //Serial.print(ir2Avg);
    //Serial.print(rDist);
    //Serial.print(" ");
    counter=0;
  }
  double diff = distance - rDist;
  Serial.print(rDist);
  Serial.print(" ");
  Serial.println(diff);

  const double DIFF_THRESH = 5;
  const int V_STEP = 40;

  if(diff > DIFF_THRESH)
  {
    lastCommand.rightV += V_STEP + 5;
    //lastCommand.leftV -= (V_STEP -10) ;..;l

    Serial.print("Right motor increase"); Serial.println(lastCommand.rightV);
  }
  else if(diff < -1 * DIFF_THRESH)
  {
    lastCommand.leftV += V_STEP;
    //lastCommand.rightV += (V_STEP -10);
    Serial.print("Left motor increase"); Serial.println(lastCommand.leftV);
  }
  else
  {
    // maybe slowly make rightV == leftV?
     double avg = (lastCommand.leftV + lastCommand.rightV) / 2.0;
     double multi = sqrt(abs(goalAvg / avg)) * avg / abs(avg); 
     
     lastCommand.leftV =  multi * 0.5 * (avg + lastCommand.leftV); //low pass
     lastCommand.rightV = multi * 0.5 * (avg + lastCommand.rightV);     

  }

  return translateWithinLimits(lastCommand);

}



MotorCommand turnAtWall(double distance, int goalAvg, MotorCommand lastCommand)
{
  
  getIR();  //update ir readings
  static TurnState ts;
  static MotorCommand* mc;
        
        double frontDist = ir1Avg;
        double sideDist = ir2Avg;
        static int counter = 0;
        if(++counter > 10)
        {
          Serial.print(ir1Avg);
          //Serial.print(rDist);
          //Serial.print(" ");
          counter=0;
        }
      
        double diffFront = distance - frontDist;
        double diffSide = distance - sideDist;
        Serial.print(frontDist);
        Serial.print(sideDist);
        Serial.print(" ");
        Serial.println(diffFront);
        Serial.println(diffSide);
  
        const double DIFF_THRESH_F = 50;
        const int V_STEP_F = 30;

        if (diffFront < DIFF_THRESH_F)
          bool turn = turnOnSpot(ts, 90, mc);
        else
          lastCommand = genWallFollow(260, goalAvg, lastCommand);
  
}



bool wallOnRight(TurnState ts, MotorCommand* mc, Adafruit_BNO055& bno, sensors_event_t initial_imu, MotorCommand lastCommand) 
{ 
  //make sure wall is on right at 20 cm away (use second ir)
  
  bool tos = turnOnSpot(ts, 10, mc);
  for(int i = 0; i < 4; i++) 
  {
//    MotorCommand drvStr = driveStraight(bno, initial_imu, lastCommand);
  }
  return turnOnSpot(ts, -10, mc);
  
}

#endif
