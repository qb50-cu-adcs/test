/* file: 		util.c
 * author: 	jacob cook
 * created on: 7/10/2014
 */

#include "dbg.h"
#include "types.h"
#include "io.h"
#include "timer.h"
#include "util.h"

static struct tmrCtlr *tmr2 = (struct tmrCtlr *)TIMER2_BASE;

/* static function prototypes */
static void delay_timer_setup(void);

/* -- util_setup ---------------------------------------------------------
 * This fucntion is used to setup misc. utilities.
 * ---------------------------------------------------------------------*/
void util_setup(void)
{
	delay_timer_setup();
}

/* -- delay_timer_setup --------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
static void delay_timer_setup(void)
{
	/* reset the controller */
	timer_soft_reset(tmr2);
	
	/* disable interrupts */
	timer_disable_int(tmr2, TMR_ALL_INT);

	/* set posted mode */
	or_regl(TMR_TSICR_POSTED, &tmr2->tsicr);
	
	/* TCLR register should be zero so we won't fuck with it */
}	

/* -- delay_sec ---------------------------------------------------------
 *
 * --------------------------------------------------------------------*/
void delay_sec(unsigned int sec)
{
	unsigned int i;
	for(i=0;i<sec;i++)
	{
		delay_ms(1000);
	}
}

/* -- delay_ms ----------------------------------------------------------
 *
 * --------------------------------------------------------------------*/
void delay_ms(unsigned int ms)
{
	/* clear interrupt flags */
	timer_clear_status(tmr2, TMR_ALL_INT);

	/* Set delay period */
	timer_set_period(tmr2, ms);
	/* trigger a load into the counter register */
	timer_trigger_reload(tmr2);
	
	/* start timer */
	timer_start(tmr2);
	
	/* wait for timer to expire */
	while(!timer_check_rstatus(tmr2, TMR_OVF));

	/* clear flag and exit */	
	timer_clear_status(tmr2, TMR_ALL_INT);
}

/* -- stopwatch_start ----------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void stopwatch_start(void)
{
	/* write a zero to the counter register */			
	timer_set_cnt(tmr2, 0);

	/* start timer */
	timer_start(tmr2);
}

/* -- stopwatch_stop -----------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int stopwatch_stop(void)
{
	unsigned int cnt;

	/* stop timer */
	timer_stop(tmr2);

	/* read counter register */
	cnt = readl(&tmr2->tcrr);

	/* calculate ms */
	return cnt*TMR_SYS_CLK_PERIOD*1000;
}
