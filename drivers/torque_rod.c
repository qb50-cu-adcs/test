/* file: 		torque_rod.c
 * author: 	jacob cook
 * created on: 10/3/2014
 */

#include <math.h>

#include "dbg.h"
#include "types.h"
#include "io.h"
#include "timer.h"
#include "pwm.h"
#include "gpio.h"
#include "global.h"
#include "torque_rod.h"

/* polarity definitions */
#define POS (1)
#define NEG	(0)



/* GPIO PINS */
/* X-pol gpio1-6 */
/* Y-pol gpio1-2 */
/* Z-pol gpio1-3 */


/* -- torq_rod_init ------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void torq_rod_init()
{
	/* set pwm timers */
	pwm4.ctlr = tmr4;
	pwm5.ctlr = tmr5;
	pwm7.ctlr = tmr7;

	/* initialize pwm controllers */
	pwm_setup(&pwm4);
	pwm_setup(&pwm5);
	pwm_setup(&pwm7);
	
	/* set intial conditions for pwm signals */	
	pwm_polarity(&pwm4, PWM_POL_POS);
	pwm_polarity(&pwm5, PWM_POL_POS);
	pwm_polarity(&pwm7, PWM_POL_POS);

	pwm_period(&pwm4, TORQ_ROD_PWM_PERIOD); 
	pwm_period(&pwm5, TORQ_ROD_PWM_PERIOD); 
	pwm_period(&pwm7, TORQ_ROD_PWM_PERIOD); 

	pwm_duty_cycle(&pwm4, 0);
	pwm_duty_cycle(&pwm5, 0);
	pwm_duty_cycle(&pwm7, 0);

	/* assign pwm controllers to each rod */
	x_rod.pwm = &pwm4;
	y_rod.pwm = &pwm5;
	z_rod.pwm = &pwm7;	

	/* set up gpios for polarity outputs */
	pol_x.bit = 6;
	pol_x.gpio_bank = gpio_bank1;
	pol_y.bit = 2;
	pol_y.gpio_bank = gpio_bank1;
	pol_z.bit = 3;
	pol_z.gpio_bank = gpio_bank1;
	/* enable gpio outputs */
	gpio_pin_en_output(&pol_x);
	gpio_pin_en_output(&pol_y);
	gpio_pin_en_output(&pol_z);
	/* set ouputs low */

	/* assign gpio pins for polarity outputs */
	x_rod.pol_pin = &pol_x;
	y_rod.pol_pin = &pol_y;
	z_rod.pol_pin = &pol_z;

	x_rod.duty_cycle = 0;
	y_rod.duty_cycle = 0;
	z_rod.duty_cycle = 0;
	
	x_rod.polarity = POS;	
	y_rod.polarity = POS;	
	z_rod.polarity = POS;	

	x_rod.dipole = 0.0;
	y_rod.dipole = 0.0;
	z_rod.dipole = 0.0;
}
	
/* -- torque_rod_on ------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void torq_rod_on(struct torq_rod *rod)
{
	pwm_start(rod->pwm);	
}


/* -- torq_rod_off ------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void torq_rod_off(struct torq_rod *rod)
{
	pwm_stop(rod->pwm);	
}

/* -- torq_rod_set_torq -----------------------------------------------
 *
 * ----------------------------------------------------------------------*/
void torq_rod_set_dipole(struct torq_rod *rod, float dipole)
{
	
	rod->dipole = dipole;
	/* set rod polarity */
	if(dipole < 0)
	{
		rod->polarity = NEG;
		dipole = fabs(dipole);	
		gpio_clr_pin(rod->pol_pin);
	}
	else
	{
		rod->polarity = POS;
		gpio_set_pin(rod->pol_pin);
	}
	
	/* calculate duty cycle */
	if(dipole >= TORQ_ROD_DIPOLE_MAX)
	{
		rod->duty_cycle = 100;
	}
	else 
	{
		rod->duty_cycle = (dipole/(double)TORQ_ROD_DIPOLE_MAX)*100.0;
	}
	/* set duty cycle */
	pwm_duty_cycle(rod->pwm, rod->duty_cycle);

}
