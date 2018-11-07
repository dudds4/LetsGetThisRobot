#ifndef MY_ENCODER_H
#define MY_ENCODER_H

#include <Arduino.h>

int mapCodeToResult[16] = 
{
  0, -1, 1, 0,
  1, 0, 0, -1,
  -1, 0, 0, 1,
  0, 1, -1, 0
};

struct Encoder {
	
	Encoder() = delete;
	
	Encoder(int pinA, int pinB)
	{
		_pinA = pinA;
		_pinB = pinB;
	}
	
	void initialize()
	{
		pinMode (_pinA, INPUT);
		pinMode (_pinB, INPUT);

		aLast = digitalRead(_pinA) ? 1 : 0;
		bLast = digitalRead(_pinB) ? 1 : 0;
	}
	
	int count() { return _count; }
	
	bool update() 
	{
		// read and update encoder
		int encA = digitalRead(_pinA) ? 1 : 0;
		int encB = digitalRead(_pinB) ? 1 : 0;

		bool aChanged = encA != aLast;
		bool bChanged = encB != bLast;
		
		bool success = !(aChanged && bChanged);

    int result = 0;
    
		if(aChanged || bChanged)
		{
			unsigned int pulseCode = (aLast << 3) + (bLast << 2) + (encA << 1) + encB;
			result = mapCodeToResult[pulseCode];
			_count += result;
		}

    if(!result)
    {
      //Serial.print("Encoder error\n");  
    }

		aLast = encA;
		bLast = encB;
		
		return success;
	}
	
private:
	
	int _pinA;
	int _pinB;
	
	int aLast;
	int bLast;
	
	int _count = 0;
};

#endif //MY_ENCODER_H
