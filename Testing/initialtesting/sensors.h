#ifndef SENSORS_H
#define SENSORS_H

#define ir1    0 // Front
#define ir2    1 // Right side

uint16_t ir_data1 = 0;
uint16_t ir_data2 = 0;

void initializeIR() {
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
}

void initializeIMU(Adafruit_BNO055& bno)
{
  /* Initialise the sensor */
  if(!bno.begin()) 
  {
    Serial.print("No BNO055 detected!");
    while(1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);
}

#endif // SENSORS_H
