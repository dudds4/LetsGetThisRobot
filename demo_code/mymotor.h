#ifndef MY_MOTOR_H
#define MY_MOTOR_H

#include <Arduino.h>
#include "myencoder.h"

#include <stdio.h>

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
    int countDiff = encNew - encLast;
    
//    1000 * (2 * pi ()) / (224.4*4) / periodMS = about = 7 / periodMS
//    float w_ = (0.007f * countDiff) / periodMS;

    double w_ = (7.0f * countDiff) / periodMS;
    double scale = 0.4;
    volatile double wScale = w_ * scale;
        
//    float w_ = encNew - encLast;
    
    w = wScale + ((1-scale) * w);

    encLast = encNew;
    
		double error = setpoint - w;
		double pTerm = Kp * error;  
		
		iTerm = iTerm + Ki * error;

    const double iTermLimit = 200000;
    
    // saturate the iTerm
    if(iTerm > iTermLimit) iTerm = iTermLimit;
    if(iTerm > -1*iTermLimit) iTerm = -1*iTermLimit;

    double result = pTerm + iTerm;

    // apply static friction offset
    if(result > 100) result += 1000;
    else if(result < -100) result -= 1000;

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

    static unsigned cc = 0;
    if(++cc > 100)
    {
      char buf[1000];
      int l = sprintf(buf, "%d %d \n", w, w_);
      buf[l] = '\0';
      Serial.print(buf);
    cc = 0;      
    }
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
    if(pwm > 255) pwm = 255;
    if(pwm < -255) pwm = -255;
	  
	  if(pwm > 0)
	  {
		  analogWrite(_pinH, pwm); 
		  digitalWrite(_pinL, LOW);   
	  }
	  else
	  {
      digitalWrite(_pinH, LOW);
		  analogWrite(_pinL, -1*pwm);   
	  }
	  
	}
	
	Encoder enc;
	
	int _pinH;
	int _pinL;

  long long encLast = 0;
  
	float setpoint = 0;
	double w = 0;
	float Kp=0, Ki=0;
	double iTerm = 0;
 
};

#endif //MY_MOTOR_H
