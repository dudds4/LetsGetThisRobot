#ifndef MY_MOTOR_H
#define MY_MOTOR_H

#include <Arduino.h>
#include "myencoder.h"

// Motor constants

const float R_ = 2.5;
const float K_ = 0.0384;

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
	
	void update(int periodMS) 
	{
    int encNew = enc.count();
    int w_ = encNew - encLast;
    
    w = 0.2*w + 0.8*w_;

    encLast = encNew;
    
		double error = setpoint - w;
		float pTerm = Kp * error;
		
		iTerm = iTerm + Ki * error;

    const double iTermLimit = 200000;
    
    // saturate the iTerm
    if(iTerm > iTermLimit) iTerm = iTermLimit;
    if(iTerm > -1*iTermLimit) iTerm = -1*iTermLimit;

    double result = pTerm + iTerm;

    // apply static friction offset
    if(result > 100) result += 2000;
    else if(result < -100) result -= 2000;

//    bool applyCurrentLimit = true;
//    float currentLimit = 1.65 * 1000; //mA
//    if(applyCurrentLimit)
//    {
//        float approxBackEMF = w * K_;
//        float maxV = approxBackEMF + currentLimit * R_;
//        float minV = approxBackEMF - currentLimit * R_;
//    
//        if(result > maxV) result = maxV;
//        if(result > minV) result = minV;
//    }

    Serial.print(result); 


		setVoltage(result);  
	}
	
	bool updateEncoder() { return enc.update(); }
 
  int count() { return enc.count(); }
  int getW() { return w; }
  
	void setSetpoint(float sp) {setpoint = sp;}
	void setGains(float P, float I) {Kp = P; Ki = I; }
	
private:
	
	//millivolts
	void setVoltage(int mV)
	{
	  const float MAX_V = 5000.0f;
	  
	  int pwm = (mV * 255.0f) / MAX_V;
	  
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

  int encLast = 0;
  
	float setpoint = 0;
	float w = 0;
	float Kp=0, Ki=0;
	double iTerm = 0;
 
};

#endif //MY_MOTOR_H
