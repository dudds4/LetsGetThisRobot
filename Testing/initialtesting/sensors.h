#ifndef SENSORS_H
#define SENSORS_H

#define ir1    0 // Front
#define ir2    1 // Right side

/*IR Variables*/
int xval=0;
uint16_t ir_data1 = 0;
uint16_t ir_data2 = 0;
unsigned int ir1Array[] = {1,1,1,1,1};
unsigned int ir2Array[] = {1,1,1,1,1};
double ir1Avg, ir2Avg;
int sum, sum2; 

void initializeIR() {
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
}

bool getIR() {
  ir1Array[0] = analogRead(ir1);
  ir2Array[0] = analogRead(ir2);
  sum = 0; sum2 = 0;
  
  for(int i = sizeof(ir1Array)/sizeof(ir1Array[0]); i>0; i--) { //shift values
    ir1Array[i] = ir1Array[i-1];
    ir2Array[i] = ir2Array[i-1];
    ir1Array[0] = analogRead(ir1);
    ir2Array[0] = analogRead(ir2);
    sum = sum +  ir1Array[0];
    sum2 = sum2 + ir2Array[0];
  }
  ir1Avg = sum / 5;
  ir2Avg = sum2 / 5;

  //outputVal = map(IR_data, 0, 1023, 0, 255);

 // Serial.print("Front: "); Serial.println(ir1Avg);
 // Serial.print("Left: "); Serial.println(ir2Avg);
  /*if(ir1Avg > 450)
    Serial.print("Turn");
  else
    Serial.print("Not yetttt");*/
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

#endif // SENSORS_H
