/*  Teensy_sPWM.c
	Copyright 2012 Brandon Foltz 
	Released under MIT license (see license.txt)
	
	This file is part of a simple software pulse width modulation library for
	use with Teensy and Arduino devices. */
	
//some of these may not be necessary
//std avr libs
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

//other
#include "Arduino/Arduino.h"
#include "Teensy_sPWM.h"

//debug
#include "usb_debug_only.h"
#include "print.h"

//#define TEENSY_2 
/* We're building for the standard teensy 2.0. This should be
defined at compile time later on. */

#ifdef TEENSY_2
	#define MAX_PINS 25
#endif
#ifdef TEENSY_2pp
	#define MAX_PINS 46
#endif

struct PWM_pin {
	char port;
	char pin;
	uint8_t pwmPercent;
	/*TODO: replace pwmPercent with a boolean val that represents whether the 
	loop needs to do anything with it. Faster than checking greater/less than 
	values for pwmPercent. If we need to retrieve the PWM value of a given pin,
	we can calculate it on the fly from other values here. */
	long int usOn;
	long int usOff;
	long int usOnRemaining;
	long int usOffRemaining;
};

static struct PWM_pin teensyPin[MAX_PINS + 1];
static uint32_t usPulseLength = 0;
static uint32_t startTime = 0, endTime = 0, deltaTime = 0; //PWM_loop vars
static char pwmOptions = 0;

int PWM_init(unsigned int Hz, char options)
{
	init();
	usPulseLength = 1000000 / Hz;
	pwmOptions = options;
	
	//go through and set all the ports and pins in abstracted order
	/* TODO: Not sure how to implement this with other boards. Maybe use a 
	define at build time to decide how the pins get labeled per board? */
	#ifdef TEENSY_2
	teensyPin[0].port = 'B';
	teensyPin[0].pin = 0;
	teensyPin[1].port = 'B';
	teensyPin[1].pin = 1;
	teensyPin[2].port = 'B';
	teensyPin[2].pin = 2;
	teensyPin[3].port = 'B';
	teensyPin[3].pin = 3;
	teensyPin[4].port = 'B';
	teensyPin[4].pin = 7;
	teensyPin[5].port = 'D';
	teensyPin[5].pin = 0;
	teensyPin[6].port = 'D';
	teensyPin[6].pin = 1;
	teensyPin[7].port = 'D';
	teensyPin[7].pin = 2;
	teensyPin[8].port = 'D';
	teensyPin[8].pin = 3;
	teensyPin[9].port = 'C';
	teensyPin[9].pin = 6;
	teensyPin[10].port = 'C';
	teensyPin[10].pin = 7;
	teensyPin[11].port = 'D';
	teensyPin[11].pin = 5;
	teensyPin[12].port = 'D';
	teensyPin[12].pin = 4;
	teensyPin[13].port = 'D';
	teensyPin[13].pin = 6;
	teensyPin[14].port = 'D';
	teensyPin[14].pin = 7;
	teensyPin[15].port = 'B';
	teensyPin[15].pin = 4;
	teensyPin[16].port = 'B';
	teensyPin[16].pin = 5;
	teensyPin[17].port = 'B';
	teensyPin[17].pin = 6;
	teensyPin[18].port = 'F';
	teensyPin[18].pin = 7;
	teensyPin[19].port = 'F';
	teensyPin[19].pin = 6;
	teensyPin[20].port = 'F';
	teensyPin[20].pin = 5;
	teensyPin[21].port = 'F';
	teensyPin[21].pin = 4;
	teensyPin[22].port = 'F';
	teensyPin[22].pin = 1;
	teensyPin[23].port = 'F';
	teensyPin[23].pin = 0;
	teensyPin[24].port = 'F';
	teensyPin[24].pin = 5;
	teensyPin[25].port = 'E';
	teensyPin[25].pin = 6;	
	#endif
	#ifdef TEENSY_2pp
	teensyPin[0].port = 'B';
	teensyPin[0].pin = 7;
	teensyPin[1].port = 'D';
	teensyPin[1].pin = 0;
	teensyPin[2].port = 'D';
	teensyPin[2].pin = 1;
	teensyPin[3].port = 'D';
	teensyPin[3].pin = 2;
	teensyPin[4].port = 'D';
	teensyPin[4].pin = 3;
	teensyPin[5].port = 'D';
	teensyPin[5].pin = 4;
	teensyPin[6].port = 'D';
	teensyPin[6].pin = 5;
	teensyPin[7].port = 'D';
	teensyPin[7].pin = 6;
	teensyPin[8].port = 'D';
	teensyPin[8].pin = 7;
	teensyPin[9].port = 'E';
	teensyPin[9].pin = 0;
	teensyPin[10].port = 'E';
	teensyPin[10].pin = 1;
	teensyPin[11].port = 'C';
	teensyPin[11].pin = 0;
	teensyPin[12].port = 'C';
	teensyPin[12].pin = 1;
	teensyPin[13].port = 'C';
	teensyPin[13].pin = 2;
	teensyPin[14].port = 'C';
	teensyPin[14].pin = 3;
	teensyPin[15].port = 'C';
	teensyPin[15].pin = 4;
	teensyPin[16].port = 'C';
	teensyPin[16].pin = 5;
	teensyPin[17].port = 'C';
	teensyPin[17].pin = 6;
	teensyPin[18].port = 'C';
	teensyPin[18].pin = 7;
	teensyPin[19].port = 'F';
	teensyPin[19].pin = 7;
	teensyPin[20].port = 'F';
	teensyPin[20].pin = 6;
	teensyPin[21].port = 'F';
	teensyPin[21].pin = 5;
	teensyPin[22].port = 'F';
	teensyPin[22].pin = 4;
	teensyPin[23].port = 'F';
	teensyPin[23].pin = 3;
	teensyPin[24].port = 'F';
	teensyPin[24].pin = 2;
	teensyPin[25].port = 'F';
	teensyPin[25].pin = 1;
	teensyPin[26].port = 'F';
	teensyPin[26].pin = 0;
	teensyPin[27].port = 'E';
	teensyPin[27].pin = 6;
	teensyPin[28].port = 'E';
	teensyPin[28].pin = 7;
	teensyPin[29].port = 'B';
	teensyPin[29].pin = 0;
	teensyPin[30].port = 'B';
	teensyPin[30].pin = 1;
	teensyPin[31].port = 'B';
	teensyPin[31].pin = 2;
	teensyPin[32].port = 'B';
	teensyPin[32].pin = 3;
	teensyPin[33].port = 'B';
	teensyPin[33].pin = 4;
	teensyPin[34].port = 'B';
	teensyPin[34].pin = 5;
	teensyPin[31].port = 'B';
	teensyPin[31].pin = 6;
	teensyPin[32].port = 'E';
	teensyPin[32].pin = 4;
	teensyPin[33].port = 'E';
	teensyPin[33].pin = 5;
	teensyPin[34].port = 'A';
	teensyPin[34].pin = 0;
	teensyPin[35].port = 'A';
	teensyPin[35].pin = 1;
	teensyPin[36].port = 'A';
	teensyPin[36].pin = 2;
	teensyPin[37].port = 'A';
	teensyPin[37].pin = 3;
	teensyPin[38].port = 'A';
	teensyPin[38].pin = 4;
	teensyPin[39].port = 'A';
	teensyPin[39].pin = 5;
	teensyPin[40].port = 'A';
	teensyPin[40].pin = 6;
	teensyPin[41].port = 'A';
	teensyPin[41].pin = 7;
	/* 46 I/O pins? well this is what I counted out...
	teensyPin[42].port = 'F';
	teensyPin[42].pin = 1;
	teensyPin[43].port = 'F';
	teensyPin[43].pin = 1;
	teensyPin[44].port = 'F';
	teensyPin[44].pin = 1;
	teensyPin[45].port = 'F';
	teensyPin[45].pin = 1;
	teensyPin[46].port = 'F';
	teensyPin[46].pin = 1;
	*/
	#endif
	
	//initialize all the pins to very much off
	int i = 0;
	for (i = 0; i <= MAX_PINS; i++)
	{
		teensyPin[i].pwmPercent = 0;
		teensyPin[i].usOn = 0;
		teensyPin[i].usOff = 0;
		teensyPin[i].usOnRemaining = 0;
		teensyPin[i].usOffRemaining = 0;	
		set_pin(teensyPin[i].port, teensyPin[i].pin, 0);
	}
	
	return 1;
}

void PWM_loop(void)
{
	endTime = micros();
	deltaTime = endTime - startTime;
	startTime = endTime;
	
	int i = 0;
	for (i = 0; i <= MAX_PINS; i++)
	{		
		if (teensyPin[i].pwmPercent > 0 && teensyPin[i].pwmPercent < 100) 
		{
			if (teensyPin[i].usOnRemaining > 0)
			{
				teensyPin[i].usOnRemaining -= deltaTime;
				if (teensyPin[i].usOnRemaining <= 0)
					set_pin(teensyPin[i].port, teensyPin[i].pin, 0);
			}
			else if (teensyPin[i].usOffRemaining > 0)
			{
				teensyPin[i].usOffRemaining -= deltaTime;
				if (teensyPin[i].usOffRemaining <= 0)
				{
					if (pwmOptions & MAKE_UP_LOST_TIME)
					{
						//off time has ended, so reset values
						teensyPin[i].usOnRemaining = teensyPin[i].usOn - teensyPin[i].usOnRemaining;
						teensyPin[i].usOffRemaining = teensyPin[i].usOff - teensyPin[i].usOffRemaining;
						set_pin(teensyPin[i].port, teensyPin[i].pin, 1);
					}
					else
					{
						//off time has ended, so reset values
						teensyPin[i].usOnRemaining = teensyPin[i].usOn;
						teensyPin[i].usOffRemaining = teensyPin[i].usOff;
						set_pin(teensyPin[i].port, teensyPin[i].pin, 1);
					}
				}
			}
		}
	}
}

int set_pin_PWM(char port, char pin, uint8_t pwmPercent)
{
	if (pwmPercent < 0 || pwmPercent > 100)
		return 0; //invalid percentage yo!
	
	if (port < 'A' || port > 'F')
		return 0;
		
	if (pin < 0 || pin > 7)
		return 0;
		
	uint8_t i = 0;
	for (i = 0; i <= MAX_PINS; i++)
	{
		if (teensyPin[i].port == port && teensyPin[i].pin == pin)
		{
			set_abstract_pin_PWM(i, pwmPercent);
			return 1;
		}
	}
	
	return 0;
}

int set_pin_PWM_normalized(char port, char pin, float normPwm)
{
	if (normPwm < 0 || normPwm > 1.0)
		return 0; //invalid percentage yo!
	
	if (port < 'A' || port > 'F')
		return 0;
		
	if (pin < 0 || pin > 7)
		return 0;
		
	uint8_t i = 0;
	for (i = 0; i <= MAX_PINS; i++)
	{
		if (teensyPin[i].port == port && teensyPin[i].pin == pin)
		{
			set_abstract_pin_PWM_normalized(i, normPwm);
			return 1;
		}
	}
	
	return 0;
}

int set_abstract_pin_PWM(uint8_t pin, uint8_t pwmPercent)
{
	if (pwmPercent < 0 || pwmPercent > 100)
		return 0; //invalid percentage yo!

	teensyPin[pin].pwmPercent = pwmPercent;
	float pwmDec;
	pwmDec = (float)pwmPercent / 100;
	
	set_PWM_values(pin, pwmDec);
	
	if (pwmPercent == 0)
		set_pin(teensyPin[pin].port, teensyPin[pin].pin, 0);
	else if (pwmPercent == 100)
		set_pin(teensyPin[pin].port, teensyPin[pin].pin, 1);
		
	return 1;
}

int set_abstract_pin_PWM_normalized(uint8_t pin, float normPwm)
{
	if (normPwm < 0.0 || normPwm > 1.0)
		return 0; //invalid pwm value!

	if (pin < 0 || pin > MAX_PINS)
		return 0;
		
	set_PWM_values(pin, normPwm);
	
	if (normPwm == 0.0)
		set_pin(teensyPin[pin].port, teensyPin[pin].pin, 0);
	else if (normPwm == 1.0)
		set_pin(teensyPin[pin].port, teensyPin[pin].pin, 1);
		
	return 1;
}

int set_all_abstract_pins_PWM(uint8_t pwmPercent)
{
	if (pwmPercent < 0 || pwmPercent > 100)
		return 0; //invalid percentage yo!

	float pwmDec;
	pwmDec = (float)pwmPercent / 100;
	
	//precalculate these values... much faster.
	long int usOn, usOff, usOnRemaining, usOffRemaining;
	usOn = usPulseLength * pwmDec;
	usOff = usPulseLength - usOn;
	usOnRemaining = usOn;
	usOffRemaining = usOff;
	
	int i = 0;
	for (i = 0; i <= MAX_PINS; i++)
	{
		teensyPin[i].pwmPercent = pwmPercent;
		teensyPin[i].usOn = usOn;
		teensyPin[i].usOff = usOff;
		teensyPin[i].usOnRemaining = usOnRemaining;
		teensyPin[i].usOffRemaining = usOffRemaining;
	}
	
	return 1;
}

static int set_pin(char charport, uint8_t pin, uint8_t val)
{
	uint8_t port = 0;

	//check for invalid values, return 0 if they are invalid.
	if (charport < 'A' || charport > 'F')
		return 0;
	
	if (pin < 0 || pin > 7)
		return 0; 
		
	port = charport - 'A';
	
	//set the pin
	if (val == 0)
	{
		// make the pin an output
		*(uint8_t *)(0x21 + port * 3) |= (1 << pin);
		// drive it low
		*(uint8_t *)(0x22 + port * 3) &= ~(1 << pin);
	}
	else
	{
		// make the pin an output
		*(uint8_t *)(0x21 + port * 3) |= (1 << pin);
		// drive it high
		*(uint8_t *)(0x22 + port * 3) |= (1 << pin);
	}
	
	return 1;
}

static int set_PWM_values(uint8_t abstractPin, float pwmDec)
{
	if (pwmDec < 0.0 || pwmDec > 1.0)
		return 0; //invalid pwm value!

	if (abstractPin < 0 || abstractPin > MAX_PINS)
		return 0;
		
	teensyPin[abstractPin].pwmPercent = pwmDec * 100; /*TODO: edit this when 
	pwmPercent is changed to a float. */
	
	teensyPin[abstractPin].usOn = usPulseLength * pwmDec;
	teensyPin[abstractPin].usOff = usPulseLength - teensyPin[abstractPin].usOn;
	teensyPin[abstractPin].usOnRemaining = teensyPin[abstractPin].usOn;
	teensyPin[abstractPin].usOffRemaining = teensyPin[abstractPin].usOff;
	
	return 1;
}
