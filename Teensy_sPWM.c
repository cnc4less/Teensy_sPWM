/*  Teensy_sPWM.c
	Copyright 2012 Brandon Foltz 
	Released under MIT license (see license.txt)
	
	This file is part of a simple software pulse width modulation library for
	use with Teensy and Arduino devices. */
	
	
//some of these may not be necessary
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <Arduino/Arduino.h>
#include "teensy_sPWM.h"

struct PWM_pin {
	uint8_t pwmPercent;
	uint32_t us_on;
	uint32_t us_off;
	uint32_t us_on_remaining;
	uint32_t us_off_remaining;
};
