// PWM Practice


int MOTOR_PIN1 = 5;
int MOTOR_PIN2 = 6;

void setup() 
{
  
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);

  // initialize serial
//  Serial.begin (9600);
}

const float MAX_VOLTAGE = 5000.0f;

inline int voltToPWM(float mV)
{
  int result = (mV * 255.0f) / MAX_VOLTAGE;
  
  if(result < 0)
    result = 0;
  else if(result > 255)
    result = 255;

  return result;
}

inline void setMotorVoltage(int motorPin, float mV)
{
  analogWrite(motorPin, voltToPWM(mV));  
}

volatile unsigned volts = 2000;
const unsigned T = 10;

void loop() 
{
  // put your main code here, to run repeatedly:

  // 5 V over 5 seconds = 5000 mV over 5000 ms
  const float increment = 3000.0f / 7000.0f * T;
  
  volts += increment;
  if(volts > 4900)
  {
    setMotorVoltage(MOTOR_PIN1, 0);
    setMotorVoltage(MOTOR_PIN2, 0);
    while(1);
  }
  
  setMotorVoltage(MOTOR_PIN1, volts);
  setMotorVoltage(MOTOR_PIN2, volts);
  Serial.print(volts);
  Serial.print('\n');
  
  delay(T);
  
}
