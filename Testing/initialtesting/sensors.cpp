#include "sensors.h"

#define ir1    A0 // Front
#define ir2    A1 // Right side

// ramp irs
#define IR_L 46
#define IR_R 48

IrSensor frontIr(ir1);
IrSensor rightIr(ir2);
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void initializeIR() 
{
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
}

bool getIR() 
{
  frontIr.refresh();
  rightIr.refresh();

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

  Serial.println("before set ext crystal!");

  bno.setExtCrystalUse(true);
}

void IrSensor::refresh()
{
  double sum = 0;
  
  // shift values up
  for(int i = IR_SENSOR_FILTER_N - 1; i > 0; i--) 
  {
    filterArray[i] = filterArray[i-1];
    sum += filterArray[i] / (double)IR_SENSOR_FILTER_N;  
  }
  
  // read new value
  filterArray[0] = analogRead(pin);
  sum += filterArray[0] / (double)IR_SENSOR_FILTER_N;
  avg = sum ;
}
    
unsigned IrSensor::getMedian() 
{
  unsigned sorted[IR_SENSOR_FILTER_N];
  memcpy(sorted, filterArray, sizeof(unsigned)*IR_SENSOR_FILTER_N);

  unsigned minIdx, tmp;
  
  // sort sorted, selection sort
  for(int i = 0; i < IR_SENSOR_FILTER_N-1; ++i)
  {
    minIdx = i;

    // find min value
    for(int j = i+1; j < IR_SENSOR_FILTER_N; ++j)
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

  return sorted[IR_SENSOR_FILTER_N/2];
}
