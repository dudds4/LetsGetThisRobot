#include "sensors.h"

#define ir1    0 // Front
#define ir2    1 // Right side

IrSensor frontIr(ir1);
IrSensor rightIr(ir2);
Adafruit_BNO055 bno(55);

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

void initializeIMU(Adafruit_BNO055& bno)
{
  /* Initialise the sensor */
  if(!bno.begin()) 
  {
    Serial.print("No BNO055 detected!");
    while(1);
  }

  bno.setExtCrystalUse(true);
}

void IrSensor::refresh()
{
  double sum = 0;
  
  // shift values up
  for(int i = IR_SENSOR_FILTER_N - 1; i > 0; i--) 
  {
    filterArray[i] = filterArray[i-1];
    sum += filterArray[i];  
  }
  
  // read new value
  filterArray[0] = analogRead(pin);
  sum += filterArray[0];
  avg = sum / IR_SENSOR_FILTER_N;
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
