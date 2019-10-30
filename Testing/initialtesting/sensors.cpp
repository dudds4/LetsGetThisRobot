#include "sensors.h"

// ir range sensors
#define ir1    A0 // Front
#define ir2    A1 // Right side

// ir ramp sensors
#define ir_L_ctrl 30
#define ir_R_ctrl 46

#define ir_L A2
#define ir_R A3

IrSensor frontIr(ir1);
IrSensor rightIr(ir2);

Antenna rampIR_L(ir_L_ctrl, ir_L);
Antenna rampIR_R(ir_R_ctrl, ir_R);

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void initializeIR() 
{
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir_L, INPUT);
  pinMode(ir_R, INPUT);

  pinMode(ir_L_ctrl, OUTPUT);
  pinMode(ir_R_ctrl, OUTPUT);

  digitalWrite(ir_L_ctrl, HIGH);
  digitalWrite(ir_R_ctrl, HIGH);
}
bool getIR() 
{
  frontIr.refresh();
  rightIr.refresh();
  rampIR_L.refresh();
  rampIR_R.refresh();

  return true;
}

void initializeIRramp() 
{
  pinMode(ir_L, INPUT);
  pinMode(ir_R, INPUT);
}
bool getIRramp() 
{
  rampIR_L.refresh();
  rampIR_R.refresh();

  return true;
}

void initializeIMU()
{
  /* Initialise the sensor */
  if(!bno.begin()) 
  {
    Serial.print("No BNO055 detected!");
    while(1);
  }

  bno.setExtCrystalUse(true);
}

void AveragedSensor::refresh()
{
  double sum = 0;
  
  // shift values up
  for(int i = SENSOR_FILTER_N - 1; i > 0; i--) 
  {
    filterArray[i] = filterArray[i-1];
    sum += filterArray[i] / (double)SENSOR_FILTER_N;  
  }
  
  // read new value
  filterArray[0] = analogRead(pin);
  sum += filterArray[0] / (double)SENSOR_FILTER_N;
  avg = sum ;
}

double IrSensor::getDist()
{
  return irAnalogToCm(getMedian());
}

unsigned Antenna::getRaw()
{
  return analogRead(pin);  
}
    
unsigned AveragedSensor::getMedian() 
{
  unsigned sorted[SENSOR_FILTER_N];
  memcpy(sorted, filterArray, sizeof(unsigned)*SENSOR_FILTER_N);

  unsigned minIdx, tmp;
  
  // sort sorted, selection sort
  for(int i = 0; i < SENSOR_FILTER_N-1; ++i)
  {
    minIdx = i;

    // find min value
    for(int j = i+1; j < SENSOR_FILTER_N; ++j)
    {
      if(sorted[j] < sorted[minIdx]) 
        minIdx = j;
    }

    // swap
    if(minIdx != i)
    {
      tmp = sorted[i];
      sorted[i] = sorted[minIdx];
      sorted[minIdx] = tmp;  
    }
  }

  return sorted[SENSOR_FILTER_N/2];
}
