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

#define UPDATE_MILLIS 250
static long int start = 0, end = 0; endCycle = 0; idleTime = 0;
static int load = 0;
void idle_loop(void);

#define SWEEP_SPEED 100

int main(void)
{
	CPU_PRESCALE(0x00); //16mhz
	//set up usb debugging
	usb_init();
	//wait a few seconds for it to setup
	_delay_ms(1000);
	
	PWM_init(100, MAKE_UP_LOST_TIME);
		
	//sweep demo!
	int pwm = 0, i = 0;
	int sweepDir = 1;
	unsigned long int endTime;
	endTime = millis() + SWEEP_SPEED;

	/* You'll probably note that the LED's flicker a little bit during the
	sweep period. I'm not entirely sure why this is, I don't the think 
	set_all_abstract_pins series of functions are particularly slow, but
	more testing is surely required. */
	while (1)
	{
		if (millis() >= endTime)
		{
			endTime = millis() + SWEEP_SPEED;
			if (pwm >= 100)
				sweepDir = 0;
			else if (pwm <= 0)
				sweepDir = 1;
			
			if (sweepDir == 1)
				pwm += 1;
			else if (sweepDir == 0)
				pwm -= 1;
			
			/*set_pin_PWM('D', 0, pwm);
			set_pin_PWM('B', 0, pwm);
			set_pin_PWM('B', 1, pwm);*/

			//set_all_abstract_pins_PWM_normalized((float)pwm / 100);
			set_all_abstract_pins_PWM(pwm);
		}
		idle_loop();
	}
		
	return 0;
}

void idle_loop(void)
{
	/*if (endCycle < millis() || endCycle == 0)
	{
		endCycle = millis() + UPDATE_MILLIS;
		load = 50 * ((float)idleTime / (float)UPDATE_MILLIS);
		if (load > 100)
			load = 100;
		idleTime = 0;
		
		set_pin_PWM('F', 0, 50 - load); //set meter to show load
	}*/
	start = millis();
	
	/* idle loop code here */
	PWM_loop();
	/* idle loop code here */
	
	end = millis();
	idleTime += (end - start);
}

