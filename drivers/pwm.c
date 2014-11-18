/* file: 		pwm.c
 * author: 	jacob cook
 * created on: 7/9/2014
 */

#include "dbg.h"
#include "types.h"
#include "io.h"
#include "timer.h"
#include "pwm.h"

/* -- pwm_start ----------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void pwm_start(struct pwmInfo *pwm)
{
	timer_start(pwm->ctlr);
}

/* -- pwm_stop -----------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void pwm_stop(struct pwmInfo *pwm)
{
	timer_stop(pwm->ctlr);
}

/* -- pwm_polarity -------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void pwm_polarity(struct pwmInfo *pwm, unsigned int pol)
{
	timer_set_polarity(pwm->ctlr, pol);
	pwm->polarity = pol;
}

/* -- pwm_period ---------------------------------------------------------
 * period in ms
 * ---------------------------------------------------------------------*/
void pwm_period(struct pwmInfo *pwm, unsigned int period)
{
	/* set timer period */
	timer_set_period(pwm->ctlr, period);
	pwm->period = period;
}

/* -- pwm_duty_cycle -----------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void pwm_duty_cycle(struct pwmInfo *pwm, unsigned int duty_cycle)
{
	struct tmrCtlr *ctlr = pwm->ctlr;
	unsigned int tldr;
	unsigned int match;

	/* if the DC is 100 reverse the polarity */
	if(duty_cycle == 100)
	{
		timer_set_polarity(ctlr,PWM_POL_NEG);
		timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
		and_regl(~TMR_TCLR_TRG_MASK, &ctlr->tclr);
	}
	else if(duty_cycle == 0)
	{
		timer_set_polarity(ctlr,PWM_POL_POS);
		timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
		and_regl(~TMR_TCLR_TRG_MASK, &ctlr->tclr);
	}
	else
	{
		timer_set_polarity(pwm->ctlr,PWM_POL_POS);
		timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
		or_regl(TMR_TCLR_TRG_OVMA, &ctlr->tclr);

		/* calculate match value */
		tldr = readl(&ctlr->tldr);
		match = (0xFFFFFFFF - tldr)*duty_cycle/100;
		match += tldr;

		/* write match value to timer match register */
		timer_wait_to_write(ctlr, TMR_TWPS_TMAR);
		writel(match, &ctlr->tmar);

		/* trigger reload to restart with new duty cycle */
		timer_trigger_reload(ctlr);
	}
	/* set duty cycle in structure */
	pwm->duty_cycle = duty_cycle;
}

/* -- pwm_setup ----------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void pwm_setup(struct pwmInfo *pwm)
{
	struct tmrCtlr *ctlr = pwm->ctlr;
	unsigned int flags;
	
	/* reset the controller	*/
	timer_soft_reset(ctlr);

	/* disable interrupts */
	timer_disable_int(ctlr, TMR_ALL_INT);

	/* set to posted mode */
	or_regl(TMR_TSICR_POSTED, &ctlr->tsicr);

	/* set CLR flags for PWM function */
	flags = TMR_TCLR_GPO_OUT | TMR_TCLR_PT | 
					TMR_TCLR_TRG_OVMA | TMR_TCLR_CE | TMR_TCLR_AR;	
	
	timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
	writel(flags, &ctlr->tclr);
	
	/* set the prescalar to 0 */
	timer_set_prescaler(ctlr, 0);

} 

