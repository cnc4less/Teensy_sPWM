/*  Teensy_sPWM.h
	Copyright 2012 Brandon Foltz 
	Released under MIT license (see license.txt)
	
	This file is part of a simple software pulse width modulation library for
	use with Teensy and Arduino devices. */
	
/* Sets up software PWM library. Runs things like init() from the Arduino
library and initializes variables. 

Hz: sets pulse width. Eg. 100Hz = 10ms pulse width. */	
int PWM_init(unsigned int Hz);

/* Loop that controls pins for PWM. Needs to be called from your programs main
loop quite often, depending on the frequency/pulse width you set initially. */
void PWM_loop(void);

/* Sets the PWM value for a given pin. 

port: Should be a character between A and F (inclusive).
pin: Should be an int between 0 and 7 (inclusive).
pwmPercent: Should be an int between 0 and 100 (inclusive). */
int set_pin_PWM(char port, char pin, uint8_t pwmPercent);

/* Sets the PWM value for a given abstracted pin. 

pin: Should be an int between 0 and 25 (inclusive).
pwmPercent: Should be an int between 0 and 100 (inclusive). */
int set_abstract_pin_PWM(uint8_t pin, uint8_t pwmPercent);

/* Sets a pin high or low. 

port: Should be a character between A and F (inclusive).
pin: Should be an int between 0 and 7 (inclusive).
val: Anything non-zero sets the pin high, 0 sets low. */
static int set_pin(char charport, uint8_t pin, uint8_t val);

/* Sets the usOn/usOff/usOnRemaining/usOffRemaining values for the given 
outputPin. 

abstractPin: Should be a value representing a Teensy 2.0 pin from 0 to 25.
pwmDec: Should be a float between 0.0 and 1.0 representing PWM value. Multiply
this value by 100 for equivalent PWM percentage. */
static int calculate_PWM_timing(int abstractPin, float pwmDec);
