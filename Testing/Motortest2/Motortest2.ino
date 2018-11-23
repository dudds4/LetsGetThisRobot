
int IN1=8; //right
int IN2=7; //right
int ENA=99;

int IN3=6; //left
int IN4=5; //left
int ENA2=99;
void setup()
{
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT); 
 pinMode(IN3,OUTPUT); 
 pinMode(IN4,OUTPUT); 
}
void loop()
{  
 delay(2000);
 digitalWrite(ENA, HIGH);// motor speed  
 analogWrite(IN1,200);// rotate forward
 digitalWrite(IN2,LOW);
 //delay(2000);  
 //digitalWrite(IN1,HIGH);// rotate reverse
 //digitalWrite(IN2,LOW);
 //delay(2000);

 digitalWrite(ENA2, HIGH);// motor speed  
 analogWrite(IN3,200);// rotate forward
 digitalWrite(IN4,LOW);
 //delay(2000);  
 //digitalWrite(IN3,HIGH);// rotate reverse
 //digitalWrite(IN4,LOW);
 //delay(2000);
}
