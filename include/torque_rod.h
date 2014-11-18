/* ***********************************************************************
 * file: 		torque_rod.h
 * author:	jacob cook
 * created on:	10/3/2014
 * ***********************************************************************
 */

#ifndef __TORQUE_ROD_H
#define __TORQUE_ROD_H

#define TORQ_ROD_DIPOLE_MAX	(100.0) /* mA*m^2 */
#define TORQ_ROD_PWM_PERIOD	(1) /*ms*/

struct torq_rod
{
	volatile double dipole;
	volatile unsigned int polarity;
	volatile unsigned int duty_cycle;	
	struct gpio_pin *pol_pin;
	struct pwmInfo *pwm;
};

void torq_rod_init();
void torq_rod_on(struct torq_rod *rod);
void torq_rod_off(struct torq_rod *rod);
void torq_rod_set_dipole(struct torq_rod *rod, float torque);

#endif
