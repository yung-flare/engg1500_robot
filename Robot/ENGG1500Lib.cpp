#include "Arduino.h"
#include "ENGG1500Lib.h"
#include <avr/interrupt.h>
#include <stdint.h>
#include <math.h>

//These will eventually overflow but it will take 214 million revolutions,
//To put this in perspective: If the wheels were rotating constantly at 1,000 RPM 
//it would take 149 days for the counter to overflow.
//So... I'm OK with that
static  uint32_t left_enc  = 0;
static  uint32_t right_enc = 0;

/*
Private functions
*/
static void _encoder_enable_interrupt_left(void)
{
	EIMSK |= (1 << INT0);  // Enable external interrupt INT0
}

static void _encoder_disable_interrupt_left(void)
{
    EIMSK &= ~(
        	(1<<INT0)      // INT0
        	);
}

static void _encoder_enable_interrupt_right(void)
{
	EIMSK |= (1 << INT1);
}

static void _encoder_disable_interrupt_right(void)
{
    EIMSK &= ~(
        	(1<<INT1)
        	);
}

/*
Initialisation function
*/
void enc_init(void)
{
	pinMode(2,INPUT);		//set pin 2 and 3 as inputs
	pinMode(3,INPUT);
	sei();                    // Enable global interrupts
	EIMSK |= (1 << INT0);     // Enable external interrupt INT0
	EIMSK |= (1 << INT1);     // Enable external interrupt INT1
	EICRA |= (1 << ISC00)|(1 << ISC01)|(1 << ISC10)|(1 << ISC11);    // Trigger INT0,INT1 on falling edge
}


/*
These functions return the encoder count of each wheel
*/
uint32_t enc_getLeft(void)
{
    _encoder_disable_interrupt_left();
    uint32_t count = left_enc;
    _encoder_enable_interrupt_left();
    return count;
}

uint32_t enc_getRight(void)
{
    _encoder_disable_interrupt_right();
    uint32_t count = right_enc;
    _encoder_enable_interrupt_right();
    return count;
}
/*
This function clears the variables that hold the encoder count
*/
void enc_clear(void)
{
	_encoder_disable_interrupt_left();
	_encoder_disable_interrupt_right();
	right_enc=0;
	left_enc=0;
	_encoder_enable_interrupt_left();
	_encoder_enable_interrupt_right();
}

/*
Encoders measure position not velocity we need to employ state estimation
in order to "estimate" velocity properly. One could take a difference of encoder measurements
and then divide this by the time difference, this is called a finite difference method
however this does not take into accound information known about the sensor (encoder resolution). 
Implimented here is a very simple, discrete linear observer (aka a steady state KF, content from ENGG3440/ELEC4410).

States describe the condition of a system at a particular time. E.g., velocity and position. In general we would like to 
have measurements of all states of a system, however sometimes that is not the case. But knowing how what we can measure relates to the states
can be used to observe/estimate these states.

To put at ease any doubts you may have about the need for this method as opposed to calculating directly the average speed, 
you are encouraged to test this function using the example provided and plot the difference using Matlab or Excel.

Put simply, you have states and you have measurements that aren't those.

Must be called at 5HZ
*/
int getVelEstimate(double &leftVel_local,double &rightVel_local)
{
	const int N_states = 4;
	const double scalar = 0.05*2*M_PI;
	// const M_obs = 2;
	//Declare static variables (states) here
	static double x[] = {0,0,0,0};
	double x_next[4];
	double y[]={scalar*enc_getLeft(),scalar*enc_getRight()};
	// x_next = A*x + B*y

//	x_next[0] = -0.243164594397768*x[0]+0.2*x[1]+1.243164594397768*y[0];
//	x_next[1] = -2.444032889894781*x[0]+1.0*x[1]+2.444032889894781*y[0];
//	x_next[2] = -0.243164594397768*x[2]+0.2*x[3]+1.243164594397768*y[1];
//	x_next[3] = -2.444032889894781*x[2]+1.0*x[3]+2.444032889894781*y[1];

//  x_next[0] = -0.417049778950670*x[0]+0.2*x[1]+1.417049778950670*y[0];
//  x_next[1] = -3.027673358059119*x[0]+1.0*x[1]+3.027673358059119*y[0];
//  x_next[2] = -0.417049778950670*x[2]+0.2*x[3]+1.417049778950670*y[1];
//  x_next[3] = -3.027673358059119*x[2]+1.0*x[3]+3.027673358059119*y[1];
  
    x_next[0] = -0.079386961422284*x[0]+0.2*x[1]+1.079386961422284*y[0];
    x_next[1] = -1.932144464025477*x[0]+1.0*x[1]+1.932144464025477*y[0];
    x_next[2] = -0.079386961422284*x[2]+0.2*x[3]+1.079386961422284*y[1];
    x_next[3] = -1.932144464025477*x[2]+1.0*x[3]+1.932144464025477*y[1];

	for (int i = 0; i < N_states; ++i)
	{
		x[i] = x_next[i];
	}

	leftVel_local  = x[1];
	rightVel_local = x[3];

	return 0;//could be used to return error code
}


//////////////////////////////
//Interrupt service routines//
//////////////////////////////
ISR(INT0_vect)//when pin 2 changes from high to low this code is executed
{
  left_enc++; 
}

ISR(INT1_vect)//when pin 3 changes from high to low this code is executed
{
  right_enc++;
}
