/* ***********************************************************************
 * file: 		pwm.h
 * author:	jacob cook
 * created on:	7/9/2014
 * ***********************************************************************
 */

#ifndef __PWM_H
#define __PWM_H

#define PWM_POL_POS (0)
#define PWM_POL_NEG (1)

struct pwmInfo
{
	unsigned int period; 				/* ms */
	unsigned int duty_cycle;		/* % */
	unsigned int polarity;			/* 0 = positive pulse */
	struct tmrCtlr *ctlr;
};

void pwm_start(struct pwmInfo *pwm);
void pwm_stop(struct pwmInfo *pwm);
void pwm_polarity(struct pwmInfo *pwm, unsigned int pol);
void pwm_period(struct pwmInfo *pwm, unsigned int period);
void pwm_duty_cycle(struct pwmInfo *pwm, unsigned int duty_cycle);
void pwm_setup(struct pwmInfo *pwm);

#endif
