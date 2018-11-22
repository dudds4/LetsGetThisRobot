#ifndef SENSORS_H
#define SENSORS_H

#include <Adafruit_BNO055.h>
#include <math.h>

struct AveragedSensor
{
  #define SENSOR_FILTER_N 20
  
  AveragedSensor() = delete;
  AveragedSensor(int p) : pin(p) {}

  void refresh();
  double getAvg() { return avg; }
  unsigned getMedian();
  
protected:  
  int pin;
  unsigned int filterArray[SENSOR_FILTER_N];
  double avg = 0;    
};

struct IrSensor : AveragedSensor
{  
  IrSensor() = delete;
  IrSensor(int p) : AveragedSensor(p) {}

  double getDist();
};

struct Antenna : AveragedSensor
{
    Antenna() = delete;
    Antenna(int digpin, int readpin) : AveragedSensor(readpin), digPin(digpin) {}

    unsigned getRaw();
private:
  int digPin;
};

/* Global Sensors Variables */
extern Adafruit_BNO055 bno;
extern IrSensor frontIr;
extern IrSensor rightIr;

extern Antenna rampIR_L;
extern Antenna rampIR_R;

// convenience function, initializes both ir sensor analog pins
void initializeIR();

// convenience function, calls IrSensor::refresh on both ir sensors
bool getIR();

// convert the analog value read from the arduino to cm
inline double irAnalogToCm(int analogValue) 
{ 
//  return 1619.1 * pow(analogValue, -0.591);
  return 3986.7 * pow(analogValue, -0.919) - 4.5;
}

// initialize an IMU sensor
void initializeIMU();

#endif // SENSORS_H
