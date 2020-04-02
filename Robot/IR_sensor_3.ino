//Distances from the centroid of the robot to the centre of each sensor in mm
#include "ENGG1500Lib.h"
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

float x1 = -15.0;
float x2 = 0;
float x3 = 15.0;
//Variables to store each data point in
float w1;
float w2;
float w3;
//Variables for storing the numerator and denominator of Equation 1
float den = 0;
float num = 0;

void setup() {
// Initialise pins for line sensors
leftForwards();
rightForwards();
pinMode(IN1, OUTPUT); //set IN1 as an output
pinMode(IN2, OUTPUT); //set IN2 as an output
pinMode(IN3, OUTPUT); //set IN3 as an output
pinMode(IN4, OUTPUT); //set IN4 as an output
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);

pinMode(5,OUTPUT);
pinMode(6,OUTPUT); 
pinMode(8,OUTPUT);
pinMode(9,OUTPUT);
pinMode(10,OUTPUT); 
pinMode(11,OUTPUT); 

pinMode(A1, INPUT);
pinMode(A2, INPUT);
pinMode(A3, INPUT);
Serial.begin(115200);

}
void loop() {
  
// TODO: Take sensor measurements using "w1 = analogRead(pin)" (store sensor
//...data in w1, w2, w3)
w1 = analogRead(A1);
w2 = analogRead(A2);
w3 = analogRead(A3);
// TODO: Calculate numerator of weighted average, store in num
num = ((w1 * x1) + (w2 * x2) + (w3 * x3));
// TODO: Calculate denominator of weighted average, store in den
den = (w1 + w2 + w3);
float lineDist = num/den;
Serial.print("Distance from line = ");
Serial.println(lineDist);
if(lineDist > 1){
   analogWrite(5, 155);
   analogWrite(6, 0);
}else{
  analogWrite(5, 0);
   analogWrite(6, 0);
}
delay(50); // 50ms delay
}
void leftForwards(void) 
{
digitalWrite(8,LOW);
digitalWrite(9,HIGH);
}
void rightForwards(void)
{
digitalWrite(10,LOW); 
digitalWrite(11,HIGH); 
}
