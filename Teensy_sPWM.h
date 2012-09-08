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
int set_pin_PWM(char port, uint8_t pin, uint8_t pwmPercent);

/* Sets a pin high or low. 

port: Should be a character between A and F (inclusive).
pin: Should be an int between 0 and 7 (inclusive).
val: Anything non-zero sets the pin high, 0 sets low. */
static int set_pin(char charport, uint8_t pin, uint8_t val);
