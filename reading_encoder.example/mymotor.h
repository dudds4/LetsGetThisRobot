#ifndef MY_MOTOR_H
#define MY_MOTOR_H

#include <Arduino.h>
#include "myencoder.h"

struct Motor {
	
	Motor() = delete;
	
	Motor(int pinH, int pinL, int encA, int encB)
	: enc(encA, encB)
	{
		_pinH = pinH;
		_pinL = pinL;
	}
	
	void initialize()
	{
		enc.initialize();
		
		pinMode(_pinH, OUTPUT);
		pinMode(_pinL, OUTPUT);
		
		digitalWrite(_pinH, LOW);
		digitalWrite(_pinL, LOW);	
	}
	
	void update() 
	{		
		float error = setpoint - w;
		
		float pTerm = Kp * error;
		
		iTerm = iTerm + Ki * error;

		setVoltage(pTerm + iTerm);
	}
	
	bool updateEncoder() { enc.update(); }
	
	void setSetpoint(float sp) {setpoint = sp;}
	void setGains(float P, float I) {Kp = P; Ki = I; }
	
private:
	
	//millivolts
	void setVoltage(int mV)
	{
	  const float MAX_V = 5000.0f;
	  
	  int pwm = (mV * 255.0f / MAX_V);
	  
	  if(pwm > 0)
	  {
		analogWrite(_pinH, pwm); 
		analogWrite(_pinL, 0);   
	  }
	  else
	  {
		analogWrite(_pinH, 0); 
		analogWrite(_pinL, -1*pwm);   
	  }
	  
	}
	
	Encoder enc;
	
	int _pinH;
	int _pinL;
	
	float setpoint = 0;
	float w = 0;
	float Kp=0, Ki=0;
	float iTerm = 0;
};

#endif //MY_MOTOR_H

