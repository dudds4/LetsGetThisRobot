#include "util.h"

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef LIMIT
#define LIMIT(x, high, low) (MAX(MIN(x, high),low))
#endif
  
bool subsamplePrint(int ss)
{
  static unsigned x = 0;
  if(++x > ss)
  {
    x = 0;
    return true;  
  }  
  return false;
}

void printCommand(MotorCommand mc)
{
  Serial.print(mc.leftV);
  Serial.print(" ");
  Serial.println(mc.rightV);  
}

void setMotorVoltage(Motor m, int v)
{
  // this is because our motors are rated for 6V, and we have a 9V supply

  
   if(v > MAX_APPLIED) v = MAX_APPLIED;
   else if(v < -1*MAX_APPLIED) v = -1*MAX_APPLIED;
   
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

double getYaw()
{
    sensors_event_t imu_event; 
    bno.getEvent(&imu_event);
    return imu_event.orientation.x;
}

double getPitch()
{
    sensors_event_t imu_event; 
    bno.getEvent(&imu_event);
    return imu_event.orientation.y;  
}

double getRoll()
{
    sensors_event_t imu_event; 
    bno.getEvent(&imu_event);
    return imu_event.orientation.z;  
}

bool turnOnSpot(TurnState &ts, int deg, MotorCommand* mc)
{
  if(!ts.initialized)
  {
    ts.initialYaw = getYaw();
    ts.lastYaw = ts.initialYaw;
    ts.goalYaw = ts.initialYaw + deg;
    ts.errorSum = 0;
    ts.lastError = 0;
    mc->reset();
    ts.initialized = true;
  }

  int yaw = getYaw();

  // check wrap around
  const int WRAP_THRESH = 30;
  
  // check positive and negative wrap arounds
  if(ts.lastYaw > (360-WRAP_THRESH) && yaw < WRAP_THRESH)       ts.goalYaw -= 360;
  else if(ts.lastYaw < WRAP_THRESH && yaw > (360-WRAP_THRESH))  ts.goalYaw += 360;
  ts.lastYaw = yaw;

  // compute directions to turn
  double diff =  ts.goalYaw - yaw;

//  Serial.print(diff);
//  Serial.print(" -- ");

  // this line prevents overshooting because of built up I gain
  if(diff*ts.lastError < 0) ts.errorSum = 0;

  // compute stiction factor
  double vel = diff - ts.lastError;
  double stictionFactor = 1 / ( 1 + vel*vel);
  
  // case 1: we've finished the turn
  const double DONE_THRESHOLD = 1.8;
  if(abs(diff) < DONE_THRESHOLD && abs(ts.lastError) < DONE_THRESHOLD)
  {
      mc->leftV = 0;
      mc->rightV = 0;
      return true;
  }  
  ts.lastError = diff;

  // case 2, PI control
  const double Kp = 0.9, Ki = 0.25;
  ts.errorSum += Ki * diff * stictionFactor;
  double result = Kp * diff + ts.errorSum; 
  
  // apply stiction
  const int STICTION_POS_R = 70, STICTION_POS_L = 70;
  const int STICTION_NEG_R = -70, STICTION_NEG_L = -75;
  
  mc->leftV = result       + (stictionFactor*((result > 0) ? STICTION_POS_L : STICTION_NEG_L));
  mc->rightV = -1 * result + (stictionFactor*((result < 0) ? STICTION_POS_R : STICTION_NEG_R));

  const int MAXES = 170;
  mc->leftV = LIMIT(mc->leftV, MAXES, -1*MAXES);
  mc->rightV = LIMIT(mc->rightV, MAXES, -1*MAXES);

//  *mc = translateWithinLimits(*mc);

//  printCommand(*mc);
  
  return false;
}

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

MotorCommand driveStraight(double initialAngle, MotorCommand lastCommand, int goalAvg)
{

  double diff = initialAngle - getYaw();

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
     if(avg == 0)
     {
       lastCommand.leftV =  20;
       lastCommand.rightV = 20;
     }
     else
     {
       double multi = sqrt(abs(goalAvg / avg)) * avg / abs(avg); 
       
       lastCommand.leftV =  multi * 0.5 * (avg + lastCommand.leftV);
       lastCommand.rightV = multi * 0.5 * (avg + lastCommand.rightV);      
     }
     
  }

  return translateWithinLimits(lastCommand);

}

MotorCommand driveStraightBackward(double initialAngle, MotorCommand lastCommand, int goalAvg)
{
  double diff = initialAngle - getYaw();

  const int DIFF_THRESH = 5;
  const int V_STEP = 10;
  
  if(abs(diff) > DIFF_THRESH) 
  { // Robot went off course
    if((diff > DIFF_THRESH && diff < 180) || (diff < -180 && diff > (-360 + DIFF_THRESH))) 
    {   
        // Robot turned left
        //speed up left motor
        lastCommand.rightV -= V_STEP;
    }
    else if ((diff > -180 && diff < -1*DIFF_THRESH) || (diff < (360-DIFF_THRESH) && diff > 180)) 
    {   
        // Robot turned right
        //speed up right motor
        lastCommand.leftV -= V_STEP;
    }
  }
  else
  {
    // maybe slowly make rightV == leftV?
     double avg = (lastCommand.leftV + lastCommand.rightV) / 2.0;
     
     if(avg == 0)
     {
       lastCommand.leftV =  -20;
       lastCommand.rightV = -20;
     }
     else
     {
       double multi = sqrt(abs(goalAvg / avg)) * avg / abs(avg); 
       
       lastCommand.leftV =  multi * 0.5 * (avg + lastCommand.leftV);
       lastCommand.rightV = multi * 0.5 * (avg + lastCommand.rightV);      
     }
     
  }

  return translateWithinLimits(lastCommand);
}

MotorCommand wallFollow(double initialAngle, double dist, int goalAvg, MotorCommand lastCommand)
{
  bool shouldPrint = subsamplePrint(20);
  
  double angle = (getYaw() - initialAngle)*M_PI/180.0;
  if(angle > M_PI) angle -= 2*M_PI;
  
  double irAvg = rightIr.getMedian();
  double diff = irAnalogToCm(irAvg)*cos(angle) - dist;

  const double DIFF_THRESH = 5;
  const double ANGLE_STEP = 10;
  
  const double Kdiff = 3.333;
  const double Kangle = 6.666; // 20

  const double ANGLE_THRESH = 7 * M_PI/180.0;
  const double A_ = 9 / (ANGLE_THRESH*ANGLE_THRESH);

  if(angle > ANGLE_THRESH / 2 && lastCommand.rightV < lastCommand.leftV)
  { lastCommand.rightV = lastCommand.leftV; }  
  else if(angle < -1 * ANGLE_THRESH / 2 && lastCommand.leftV < lastCommand.rightV)
  { lastCommand.leftV = lastCommand.rightV; }

  // apply gains
  double diff_gain = 1/(1+angle*angle*A_);
  double angle_gain = 1 - diff_gain;

  double deltaDiff = diff_gain*Kdiff*diff;
  double deltaAngle = angle_gain*Kangle*angle;
  
  lastCommand.leftV -= deltaAngle/2;
  lastCommand.rightV += deltaAngle/2;

  if(deltaDiff > 0) lastCommand.leftV += deltaDiff;
  else lastCommand.rightV -= deltaDiff;
  
  if(abs(angle) < ANGLE_THRESH && abs(diff) < DIFF_THRESH)
  {
    // maybe slowly make rightV == leftV?
     double avg = (lastCommand.leftV + lastCommand.rightV) / 2.0;
     double multi = sqrt(abs(goalAvg / avg)) * avg / abs(avg); 
     
     lastCommand.leftV =  multi * 0.5 * (avg + lastCommand.leftV); //low pass
     lastCommand.rightV = multi * 0.5 * (avg + lastCommand.rightV);     

  }
  
  lastCommand = translateWithinLimits(lastCommand);

  if(shouldPrint)
  {
    Serial.print(angle);
    Serial.print(" ");  
    Serial.print(diff + dist);
    Serial.print(" ");  
    Serial.print(dist);
    Serial.print(" ");  
    printCommand(lastCommand);
  }

  return lastCommand;
}

bool reverse(MotorCommand lastCommand)
{
  lastCommand.leftV = -1 *50;
  lastCommand.rightV = -1 *50;
  
  //delay(1000);
  return true;
}
