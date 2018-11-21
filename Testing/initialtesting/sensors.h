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

  void refresh();
  double getAvg() { return avg; }
  unsigned getMedian();
  
private:  
  int pin;
  unsigned int filterArray[IR_SENSOR_FILTER_N];
  double avg = 0;
};

/*Global Sensors Variables*/

extern Adafruit_BNO055 bno;
extern IrSensor frontIr;
extern IrSensor rightIr;

void initializeIR();
bool getIR();

inline double irAnalogToCm(int analogValue) { return 1619.1 * pow(analogValue, -0.591); }
void initializeIMU(Adafruit_BNO055& bno);

#endif // SENSORS_H
