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
#include <util/delay.h>

//other
#include <Arduino/Arduino.h>
#include "Teensy_sPWM.h"

//debug
#include "usb_debug_only.h"
#include "print.h"

struct PWM_pin {
	char port;
	char pin;
	uint8_t pwmPercent;
	long int usOn;
	long int usOff;
	long int usOnRemaining;
	long int usOffRemaining;
};

static struct PWM_pin teensyPin[26];
static uint32_t usPulseLength = 0;

//here for testing. Remove when turned to a library.
int main(void)
{
	//set up usb debugging
	usb_init();
	
	//wait a few seconds for it to setup
	_delay_ms(1000);
	
	
	if (PWM_init(120))
		set_pin('B', 1, 1);
		
	set_abstract_pin_PWM(0, 25);
	set_pin_PWM('B', 1, 15);
	set_pin_PWM('F', 0, 5);
	//set_abstract_pin_PWM(23, 15);
	
	//performance testing, remove this
	/* int i = 0;
	for (i = 2; i < 23; i++)
		set_abstract_pin_PWM(i, 25); */
	
	while (1)
	{
		PWM_loop();
	}
		
	return 0;
}

int PWM_init(unsigned int Hz)
{
	init();
	usPulseLength = 1000000 / Hz;
	
	//go through and set all the ports and pins in abstracted order
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
	
	//initialize all the pins to very much off
	int i = 0;
	for (i = 0; i <= 25; i++)
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

static uint32_t startTime = 0, endTime = 0, deltaTime = 0;
void PWM_loop(void)
{
	endTime = micros();
	deltaTime = endTime - startTime;
	startTime = endTime;
	
	int i = 0;
	for (i = 0; i <= 25; i++)
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
					//off time has ended, so reset values
					teensyPin[i].usOnRemaining = teensyPin[i].usOn - teensyPin[i].usOnRemaining;
					teensyPin[i].usOffRemaining = teensyPin[i].usOff - teensyPin[i].usOffRemaining;
					set_pin(teensyPin[i].port, teensyPin[i].pin, 1);
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
	for (i = 0; i <= 25; i++)
	{
		if (teensyPin[i].port == port && teensyPin[i].pin == pin)
		{
			set_abstract_pin_PWM(i, pwmPercent);
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
	
	teensyPin[pin].usOn = usPulseLength * pwmDec;
	teensyPin[pin].usOff = usPulseLength - teensyPin[pin].usOn;
	teensyPin[pin].usOnRemaining = teensyPin[pin].usOn;
	teensyPin[pin].usOffRemaining = teensyPin[pin].usOff;
	
	if (pwmPercent == 0)
		set_pin(teensyPin[pin].port, teensyPin[pin].pin, 0);
	else if (pwmPercent == 100)
		set_pin(teensyPin[pin].port, teensyPin[pin].pin, 1);
		
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

static int calculate_PWM_timing(int abstractPin, float pwmDec)
{
	if (pwmDec < 0.0 || pwmDec > 1.0)
		return 0; //invalid pwm value!

	if (abstractPin < 0 || abstractPin > 25)
		return 0;
		
	teensyPin[abstractPin].pwmPercent = pwmDec * 100; /*TODO: edit this when 
	pwmPercent is changed to a float. */
	
	teensyPin[pin].usOn = usPulseLength * pwmDec;
	teensyPin[pin].usOff = usPulseLength - teensyPin[pin].usOn;
	teensyPin[pin].usOnRemaining = teensyPin[pin].usOn;
	teensyPin[pin].usOffRemaining = teensyPin[pin].usOff;
	
	return 1;
}
