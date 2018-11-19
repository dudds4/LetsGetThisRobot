#ifndef SENSORS_H
#define SENSORS_H

#include <Adafruit_BNO055.h>
#include <math.h>

#define ir1    0 // Front
#define ir2    1 // Right side

/*IR Variables*/
int xval=0;
uint16_t ir_data1 = 0;
uint16_t ir_data2 = 0;
unsigned int ir1Array[] = {1,1,1,1,1};
unsigned int ir2Array[] = {1,1,1,1,1};
double ir1Avg, ir2Avg;


void initializeIR() 
{
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
}

bool getIR() 
{
  int newValue1 = analogRead(ir1), newValue2 = analogRead(ir2);
  int sum1 = newValue1, sum2 = newValue2;

  int N = sizeof(ir1Array) / sizeof(ir1Array[0]);
  
  for(int i = N - 1; i > 0; i--) 
  { 
    //shift values
    ir1Array[i] = ir1Array[i-1];
    ir2Array[i] = ir2Array[i-1];
        
    sum1 += ir1Array[i];
    sum2 += ir2Array[i];
  }

  ir1Array[0] = analogRead(ir1);
  ir2Array[0] = analogRead(ir2);
  
  ir1Avg = sum1 / N;
  ir2Avg = sum2 / N;

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
