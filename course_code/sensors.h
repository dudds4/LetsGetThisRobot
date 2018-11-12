#ifndef SENSORS_H
#define SENSORS_H

#define ir1    8 // Front
#define ir2    9 // Right side

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
