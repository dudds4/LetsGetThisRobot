#ifndef RAMP_CLIMB_H
#define RAMP_CLIMB_H

struct RampClimber
{
  TurnState ts;
  double initialYaw, initialPitch, initialRoll;
  int state = 0;

  bool isDone() { return true; }
  
  MotorCommand run(MotorCommand mc)
  {
    
//    Serial.print();
    
    /* L8R
    if(state == 0) 
    { // Going up
      const double PITCH_THRESHOLD = 10;
      if(abs(getPitch() - initialPitch) > PITCH_THRESHOLD) //imu y value goes to 0
        state = 1;
      else 
      {
        lastCommand = driveRamp();
      }
    }
    else if(state == 1)
    { // Moving across top
      if( ) //
        state = 2; //imu y value increases
    }
    else if(state == 2)
    { // Going down
      
    }*/
      return MotorCommand();
  }  
  
};

#endif //RAMP_CLIMB_H
