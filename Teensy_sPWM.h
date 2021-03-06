/*  Teensy_sPWM.h
	Copyright 2012 Brandon Foltz 
	Released under MIT license (see license.txt)
	
	This file is part of a simple software pulse width modulation library for
	use with Teensy and Arduino devices. */

#ifndef TEENSY_SPWM
#define TEENSY_SPWM

#ifdef TEENSY_2
	#define TEENSY_MAX_PINS 25
#endif
#ifdef TEENSY_2pp
	#define TEENSY_MAX_PINS 46
#endif
		
enum {
	MAKE_UP_LOST_TIME = 0x01
	} enumOptions;

/* Sets up software PWM library. Runs things like init() from the Arduino
library and initializes variables. 

Hz: sets pulse width. Eg. 100Hz = 10ms pulse width. */
int PWM_init(unsigned int Hz, char options);

/* Loop that controls pins for PWM. Needs to be called from your programs main
loop quite often, depending on the frequency/pulse width you set initially. */
void PWM_loop(void);

/* Sets the PWM value for a given pin. 

port: Should be a character between A and F (inclusive).
pin: Should be an int between 0 and 7 (inclusive).
pwmPercent: Should be an int between 0 and 100 (inclusive). */
int set_pin_PWM(char port, char pin, uint8_t pwmPercent);

/* Sets the PWM value for a given pin. 

port: Should be a character between A and F (inclusive).
pin: Should be an int between 0 and 7 (inclusive).
normPwm: Should be a float between 0.0 and 1.0 (inclusive). */
int set_pin_PWM_normalized(char port, char pin, float normPwm);

/* Sets the PWM value for a given abstracted pin. 

pin: Should be an int between 0 and 25 (inclusive).
pwmPercent: Should be an int between 0 and 100 (inclusive). */
int set_abstract_pin_PWM(uint8_t pin, uint8_t pwmPercent);

/* Sets the PWM value for a given abstracted pin. 

pin: Should be an int between 0 and 25 (inclusive).
normPwm: Should be an float between 0.0 and 1.0 (inclusive). */
int set_abstract_pin_PWM_normalized(uint8_t pin, float normPwm);

/* Sets the PWM value for all pins. 

pwmPercent: int between 0 and 100 (inclusive) */
int set_all_abstract_pins_PWM(uint8_t pwmPercent);

/* Sets the PWM value for all pins. 

normPwm: float between 0.0 and 1.0 (inclusive) */
int set_all_abstract_pins_PWM_normalized(float normPwm);

/* Sets the PWM value for all pins in a given range.

start: int between 0 and the number of available pins.
end: int between 0 and the number of available pins. Must be greater than start.
pwmPercent: int between 0 and 100 (inclusive) */
int set_abstract_pin_range_PWM(int start, int end, uint8_t pwmPercent);

/* Sets the PWM value for all pins in a given range.

start: int between 0 and the number of available pins.
end: int between 0 and the number of available pins. Must be greater than start.
normPwm: float between 0.0 and 1.0 (inclusive) */
int set_abstract_pin_range_PWM_normalized(int start, int end, float normPwm);

#endif

