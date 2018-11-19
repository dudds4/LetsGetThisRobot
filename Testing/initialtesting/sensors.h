#ifndef SENSORS_H
#define SENSORS_H

#include <Adafruit_BNO055.h>
#include <math.h>

#define ir1    0 // Front
#define ir2    1 // Right side

struct IrSensor 
{
  
  #define IR_SENSOR_FILTER_N 5
  
  IrSensor() = delete;
  IrSensor(int p) : pin(p) {}

  void refresh()
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
  
  double getAvg() { return avg; }
  
private:  
  int pin;
  unsigned int filterArray[IR_SENSOR_FILTER_N];
  double avg = 0;
};

/*IR Variables*/

IrSensor frontIr(ir1);
IrSensor rightIr(ir2);

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

inline double irAnalogToCm(int analogValue)
{
  return 1619.1 * pow(analogValue, -0.591);
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

#endif // SENSORS_H
