#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>

#include "Arduino/Arduino.h"
#include "Teensy_sPWM.h"

//debug
#include "usb_debug_only.h"
#include "print.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

#define IDLE_TIMER_PRECISION 200

#define UPDATE_MILLIS 250
static long int start = 0, end = 0; endCycle = 0; idleTime = 0;
static int load = 0;
void idle_loop(void);

int main(void)
{
	CPU_PRESCALE(0x00); //16mhz
	//set up usb debugging
	usb_init();
	//wait a few seconds for it to setup
	_delay_ms(1000);
	
	PWM_init(100);
		
	set_abstract_pin_PWM(0, 15);
	set_pin_PWM('B', 1, 95);
	set_pin_PWM('F', 0, 25);
	//set_abstract_pin_PWM(23, 15);
	
	//performance testing, remove this
	/*int i = 0;
	for (i = 2; i < 23; i++)
		set_abstract_pin_PWM(i, 75); */
		
	//sweep demo!
	int pwm = 0, i = 0;
	int sweepDir = 1;
	unsigned int endTime;
	endTime = millis() + 50;
	while (1)
	{
	
		//FIXME: there's some bug here that causes crash/lock after ~1 minute
		if (millis() >= endTime)
		{
			endTime = millis() + 50;
			if (pwm >= 100)
				sweepDir = 0;
			else if (pwm <= 0)
				sweepDir = 1;
			
			if (sweepDir == 1)
				pwm += 1;
			else if (sweepDir == 0)
				pwm -= 1;
			
			set_pin_PWM('D', 0, pwm);
			
			//_delay_us(2000);
		}
		idle_loop();
	}
		
	return 0;
}

void idle_loop(void)
{
	if (endCycle < millis() || endCycle == 0)
	{
		endCycle = millis() + UPDATE_MILLIS;
		load = 40 * ((float)idleTime / (float)UPDATE_MILLIS);
		if (load > 100)
			load = 100;
		idleTime = 0;
		
		set_pin_PWM('F', 0, 40 - load); //set meter to show load
	}
	start = millis();
	
	/* idle loop code here */
	PWM_loop();
	/* idle loop code here */
	
	end = millis();
	idleTime += (end - start);
}

