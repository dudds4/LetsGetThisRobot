#ifndef MYNAV_H
#define MYNAV_H

#define MOTOR_V_MAX 255

int IN1=4;
int IN2=5;
int ENA=10;

int IN3=6;
int IN4=7;
int ENA2=11;

pinMode(IN1,OUTPUT);
pinMode(IN2,OUTPUT); 
pinMode(IN3,OUTPUT); 
pinMode(IN4,OUTPUT); 

struct MotorCommand
{
	int leftV;
	int rightV;
};

#endif // MYNAV_H
