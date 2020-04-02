#include "ENGG1500Lib.h"
#define ECHO 12
#define TRIG 7
#define ENCCLICKS 100
#include <Wire.h>
#include "SparkFun_APDS9960.h"
#define ECHO 12
#define TRIG 7
// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint8_t proximity_data = 0;
unsigned int distance_mm = 0; //This variable will hold the distance

enum {
ENA = 5,
ENB = 6,
IN1 = 8,
IN2 = 9,
IN3 = 10,
IN4 = 11,
ENCL = 2,
ENCR = 3,
};

int wheelPWM = ENCCLICKS;
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

void setup() 
{  
Serial.begin(9600); //begin serial communication
Serial.begin(115200);
pinMode(ENA, OUTPUT); 
pinMode(ENB, OUTPUT); 
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
pinMode(ENCL, INPUT); // initialising left click
pinMode(ENCR, INPUT); // initalising right click
delay(2000);
digitalWrite(IN1, 0);
digitalWrite(IN2, 1);
digitalWrite(IN3, 0);
digitalWrite(IN4, 1);
analogWrite(ENA, wheelPWM); //  initial values for wheel
analogWrite(ENB, wheelPWM); // initial values for wheel

pinMode(ECHO,INPUT); //Initialise pin 12 as an input
pinMode(TRIG,OUTPUT); //Initialise pin 7 as an output
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

enc_init(); //initialise encoders
delay(2000);
Serial.begin(9600);
Serial.println("PWM,ENC_L,ENC_R");
//TODO: set motor direction

// Initialize Serial port
Serial.begin(9600);
// Initialize APDS-9960 (configure I2C and initial values)
if ( apds.init() ) {
// Serial.println(F("APDS-9960 initialization complete"));
} else {
Serial.println(F("Something went wrong during APDS-9960 init!"));
}
// Adjust the Proximity sensor gain
if ( !apds.setProximityGain(PGAIN_2X) ) {
  Serial.println(F("Something went wrong trying to set PGAIN"));
}
// Start running the APDS-9960 proximity sensor (no interrupts)
if ( apds.enableProximitySensor(false) ) {
// Serial.println(F("Proximity sensor is now running"));
} else {
Serial.println(F("Something went wrong during sensor init!"));
}
// Initialise the Ultrasonic sensor
pinMode(ECHO, INPUT); //Initialise pin 12 as an input
pinMode(TRIG, OUTPUT); //Initialise pin 7 as an output
}

void loop() {
unsigned int distance_mm = 0; //This variable will hold the distance
distance_mm = sonar_mm(); //storing the sonar value as a distance
Serial.print("Distance="); //print the result to the serial monitor
Serial.println(distance_mm); 
if (distance_mm < 100) {
  analogWrite(5,255); // if it sees a wall it will turn
  analogWrite(6,0);
  delay(200);
  }else{
  analogWrite(5,155);
  analogWrite(6,155);
  }
 Serial.print(digitalRead(ENCL));
Serial.print("\t");
Serial.println(digitalRead(ENCR) + 2);
delay(1);

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

unsigned int sonar_mm(void){
long duration = 0;
const float speed_sound = 340.29;// m/s, 
digitalWrite(TRIG, HIGH);// The ultrasonic burst is triggered by a HIGH pulse of 10 microseconds.
delayMicroseconds(10);
digitalWrite(TRIG, LOW); //read length of time pulse
duration = pulseIn(ECHO, HIGH); //This function measures a pulsewidth and returns
return (unsigned int)(0.5 * duration * 1e-6 * speed_sound * 1e3); // unsigned so there is no negative distance
}

{   
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

{
void leftForwards(void) //This function sets IN1 = LOW and IN2 = HIGH in order to set
//...the direction to forwards for motor 1
{
digitalWrite(8, LOW); //IN1
digitalWrite(9, HIGH); //IN2
}
void leftBackwards(void) //This function sets IN1 = HIGH and IN2 = LOW in order to set
//... the direction to backwards for motor 1
{
digitalWrite(8, HIGH); //IN1
digitalWrite(9, LOW); //IN2
}
void rightForwards(void) //This function sets IN3 = LOW and IN4 = HIGH in order to set
//... the direction to forwards for motor 2
{
digitalWrite(10, LOW); //IN3
digitalWrite(11, HIGH); //IN4
}
{
void rightBackwards(void) //This function sets IN3 = HIGH and IN4 = LOW in order to
//...set the direction to forwards for motor 2
{
digitalWrite(10, HIGH); //IN3
digitalWrite(11, LOW); //IN4
}
}

while (1) {}; //infinite loop to stop the experiment
}
unsigned int sonar_mm(void) {
long duration = 0;
const float speed_sound = 340.29;// m/s, "const" makes the compiler able to optimise
//... the program where this variable is used, cool!
// Read in a distance from the ultrasonic distance sensor:
// The ultrasonic burst is triggered by a HIGH pulse of 10 microseconds.
digitalWrite(TRIG, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG, LOW);
//read length of time pulse
duration = pulseIn(ECHO, HIGH); //This function measures a pulsewidth and returns
//...the width in microseconds


// convert the time into a distance
// the code "(unsigned int)" turns the result of the distance calculation
// into an integer instead of a floating point (decimal or fractional) number.
return(unsigned int)(0.5 * duration * 1e-6 * speed_sound * 1e3);
}
}
