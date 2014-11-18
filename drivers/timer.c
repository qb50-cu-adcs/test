/* file: 		timer.c
 * author: 	jacob cook
 * created on: 7/9/2014
 */

#include "dbg.h"
#include "types.h"
#include "io.h"
#include "timer.h"


/* -- timer_soft_reset ---------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void timer_soft_reset(struct tmrCtlr *ctlr)
{
	/* set soft reset bit */
	or_regl(0x1, &ctlr->tiocp_cfg);

	/* wait for reset to complete */
	while(readl(&ctlr->tiocp_cfg) & 0x1);
}

/* -- timer_start --------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void timer_start(struct tmrCtlr *ctlr)
{
	timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
	or_regl(TMR_TCLR_ST, &ctlr->tclr);
}

/* -- timer_stop ---------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void timer_stop(struct tmrCtlr *ctlr)
{
	timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
	and_regl(~TMR_TCLR_ST, &ctlr->tclr);
}

/* -- timer_disable_int --------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void timer_disable_int(struct tmrCtlr *ctlr, unsigned int flags)
{
	or_regl(flags, &ctlr->irqenable_clr);
}

/* -- timer_enable_int ---------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void timer_enable_int(struct tmrCtlr *ctlr, unsigned int flags)
{
	or_regl(flags, &ctlr->irqenable_set);
}

/* -- timer_check_rstatus ------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int timer_check_rstatus(struct tmrCtlr *ctlr, unsigned int flag)
{
	return (readl(&ctlr->irqstatus_raw) & flag) ? 1 : 0;
}

/* -- timer_check_status ------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int timer_check_status(struct tmrCtlr *ctlr, unsigned int flag)
{
	return (readl(&ctlr->irqstatus) & flag) ? 1 : 0;
}

/* -- timer_clear_status -------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void timer_clear_status(struct tmrCtlr *ctlr, unsigned int flag)
{
	or_regl(flag, &ctlr->irqstatus);
}

/* -- timer_wait_to_write ------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void timer_wait_to_write(struct tmrCtlr *ctlr, unsigned int reg)
{
	if(readl(&ctlr->tsicr) & TMR_TSICR_POSTED)
		while(reg & readl(&ctlr->twps));
}

/* -- timer_set_prescaler ------------------------------------------------
 * divisor  = 2^(presvale + 1) 
 * ---------------------------------------------------------------------*/
void timer_set_prescaler(struct tmrCtlr *ctlr, unsigned int prescale)
{
	if(prescale == 0)
	{
		timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
		/* clear PRE bit is tclr register */
		and_regl(~TMR_TCLR_PRE, &ctlr->tclr);
		return;
	}

	/* set prescale value */
	timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
	and_regl(~TMR_TCLR_PTV_MASK, &ctlr->tclr);
	timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
	or_regl((prescale <<  TMR_TCLR_PTV_SHIFT), &ctlr->tclr);

	/* set PRE bit to enable prscaling */
	timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
	or_regl(TMR_TCLR_PRE, &ctlr->tclr);
}

/* -- timer_set_polarity ------------------------------------------------
 * 
 * --------------------------------------------------------------------*/
void timer_set_polarity(struct tmrCtlr *ctlr, unsigned int pol)
{
	/* Clear polarity bit */
	timer_wait_to_write(ctlr,TMR_TWPS_TCLR);
	and_regl(~TMR_TCLR_SCPWM_NEG, &ctlr->tclr);

	/* write it polarity bit */
	timer_wait_to_write(ctlr, TMR_TWPS_TCLR);
	or_regl((pol << 7), &ctlr->tclr);
}	

/* -- timer_set_period ---------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void timer_set_period(struct tmrCtlr *ctlr, float period)
{
	unsigned int tldr;
	unsigned int count;
 
	/* calculate reload value.  ASSUMES NO DIVISOR */
	count = (unsigned int)(period/1000/TMR_SYS_CLK_PERIOD);
	tldr = 0xFFFFFFFF - count + 1;
	
	/* write value to register */
	timer_set_reload(ctlr, tldr);
}


/* -- timer_set_reload ---------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void timer_set_reload(struct tmrCtlr *ctlr, unsigned int load_val)
{
	timer_wait_to_write(ctlr, TMR_TWPS_TLDR);
	writel(load_val, &ctlr->tldr);
}

/* -- timer_set_cnt ------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void timer_set_cnt(struct tmrCtlr *ctlr, unsigned int cnt)
{
	timer_wait_to_write(ctlr, TMR_TWPS_TCRR);
	writel(cnt, &ctlr->tcrr);
}

/* -- timer_trigger_reload -----------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void timer_trigger_reload(struct tmrCtlr *ctlr)
{
	timer_wait_to_write(ctlr, TMR_TWPS_TTGR);
	writel(0xFFFF,&ctlr->ttgr);
}

