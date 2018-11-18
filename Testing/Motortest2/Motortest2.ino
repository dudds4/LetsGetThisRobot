
int IN1=4; //right
int IN2=5; //right
int ENA=10;

int IN3=6; //left
int IN4=7; //left
int ENA2=11;
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
