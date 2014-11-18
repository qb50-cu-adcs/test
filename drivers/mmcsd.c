/* file: 		mmcsd.c
 * author: 	jacob cook
 * created on: 6/25/2014
 */

#include "dbg.h"
#include "types.h"
#include "io.h"
#include "sd_card.h"
#include "mmcsd.h"


/* -- mmcsd_init -----------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
int mmcsd_init(struct mmcsdCtlr *ctlr)
{
	/* softreset of the controller */
	if(mmcsd_softreset(ctlr) != 0)
	{
		PRINT("MMC Controller reset failed\n\r");
		return -1;
	}

	/* line reset */
	if(mmcsd_linereset(ctlr, MMC_RESET_ALL) != 0)
	{
		PRINT("MMC Line reset failed\n\r");
		return -1;
	}
	
	/* set bus voltage capabilities */
	mmcsd_volt_capa(ctlr, MMC_CAPA_VOLT_1P8 | MMC_CAPA_VOLT_3P0);

	/* set config to Auto idle */
	mmcsd_sys_config(ctlr, MMC_AUTOIDLE);

	/* set bus width (should be 1 during initialization) */
	mmcsd_bus_width(ctlr, MMC_BUS_WIDTH_1BIT);

	/* set bus volatage */
	mmcsd_bus_volt(ctlr, MMC_HCTL_SDVS_3P0);

	/* power bus on */
	if(mmcsd_power(ctlr, MMC_HCTL_PWR_ON) != 0)
	{
		PRINT("MMC bus did not power on\n\r");
		return -1;
	}

	/* Set bus frequency for initialization process */
	if(mmcsd_set_bus_freq(ctlr, MMC_INIT_CLK) != 0)
	{
		PRINT("MMC: setting bus frequency failed.\n\r");
		return -1;
	}
	
	/* Send init sequence to card */
	if(mmcsd_send_init_seq(ctlr) != 0)
	{
		PRINT("MMC: init sequence failed.\n\r");
	}

	return 0;
}

/* -- mmcsd_read_data ----------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
unsigned int mmcsd_read_data(struct mmcsdCtlr *ctlr)
{
	return readl(&ctlr->sd_data);
}

/* -- mmcsd_write_data ---------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void mmcsd_write_data(struct mmcsdCtlr *ctlr, unsigned int data)
{
	writel(data, &ctlr->sd_data);
}

/* -- mmcsd_get_tran_status ----------------------------------------------
 * 
 * NOTE: this function attempts to identify a Data timeout by polling 
 * the DTO flag in the STAT register. It is not clear that this will 
 * work beacuse I have not been able to get the CT and the BRR flags to
 * set indicating data is ready to be read. I had to resort to polling 
 * the BRE bit in the PSTATE register. See "mmcsd_read_complete".
 * ---------------------------------------------------------------------*/
unsigned int mmcsd_get_tran_status(struct mmcsdCtlr *ctlr, unsigned int dir)
{
	unsigned int status;

	/* wait for trans to complete or timeout */
	while( !mmcsd_tran_complete(ctlr) && !mmcsd_tran_timeout(ctlr) );

	/* check exit condition */
	status = mmcsd_tran_complete(ctlr);

	/* clear complete and timeout flags */
	mmcsd_clear_status(ctlr, MMC_STAT_TC | MMC_STAT_DTO);
	
	/* return 1 if cmd completed else return 0 */
	return status;
}	

/* -- mmcsd_write_complete -----------------------------------------------
 *
 * -------------------------------------------------------------------- */
unsigned int mmcsd_write_complete(struct mmcsdCtlr *ctlr)
{
	return mmcsd_get_status(ctlr, MMC_STAT_BWR) ? 1 : 0;
	/*return (readl(&ctlr->sd_pstate) & MMC_PSTATE_BWE) ? 1 : 0;*/
}

/* -- mmcsd_read_complete ------------------------------------------------
 *
 * NOTE: Polling the TC and/or the BRR bits in the STAT register does not 
 * seem to work when determining when the read blk transfer is complete.
 * ---------------------------------------------------------------------*/
unsigned int mmcsd_read_complete(struct mmcsdCtlr *ctlr)
{
	return mmcsd_get_status(ctlr, MMC_STAT_BRR) ? 1 : 0;
	/*return (readl(&ctlr->sd_pstate) & MMC_PSTATE_BRE) ? 1 : 0;*/
}
	
/* -- mmcsd_tran_complete -----------------------------------------------
 * 
 * --------------------------------------------------------------------*/
unsigned int mmcsd_tran_complete(struct mmcsdCtlr *ctlr)
{
	return mmcsd_get_status(ctlr, MMC_STAT_TC) ? 1 : 0;
}

/* -- mmcsd_tran_timeout ------------------------------------------------
 * 
 * --------------------------------------------------------------------*/
unsigned int mmcsd_tran_timeout(struct mmcsdCtlr *ctlr)
{
	return mmcsd_get_status(ctlr, MMC_STAT_DTO) ? 1 : 0;
}

/* -- mmcsd_set_blklen --------------------------------------------------
 * 
 * --------------------------------------------------------------------*/
void mmcsd_set_blklen(struct mmcsdCtlr *ctlr, unsigned int blklen)
{
	and_regl(~MMC_BLK_BLEN_MASK, &ctlr->sd_blk);
	or_regl(blklen, &ctlr->sd_blk);
}

/* -- mmcsd_get_rsp -----------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void mmcsd_get_rsp(struct mmcsdCtlr *ctlr, unsigned int *rsp)
{
	unsigned int i;
	for(i=0;i<4;i++)
	{
		*rsp++ = readl(&ctlr->sd_rsp[i]);
	}
}	

/* -- mmcsd_send_cmd ----------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void mmcsd_send_cmd(struct mmcsdCtlr *ctlr, unsigned int cmd,
										unsigned int cmdarg, void *data,
										unsigned int nblks, unsigned int dmaEn)
{
	/* set bits in CMD register to deal with Data */
	if(data != NULL)
	{
		cmd |= (MMC_CMD_DP | MMC_CMD_MSBS | MMC_CMD_BCE);
		cmd |=MMC_CMD_DP;
	}
	
	/* dma enable (Not using this feature at the moment) */
	if(dmaEn == 1)
	{
		cmd |= MMC_CMD_DE;
	}

	/* set number of blocks */
	and_regl(~MMC_BLK_NBLK_MASK, &ctlr->sd_blk);
	or_regl((nblks << MMC_BLK_NBLK_SHIFT), &ctlr->sd_blk);

	/* Set cmd arg and cmd */
	writel(cmdarg, &ctlr->sd_arg);
	writel(cmd, &ctlr->sd_cmd);	
	
}

/* -- mmcsd_en_status_flags ----------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void mmcsd_en_status_flags(struct mmcsdCtlr *ctlr, unsigned int flags)
{
	or_regl(flags, &ctlr->sd_ie);
}

/* -- mmcsd_dis_status_flags ---------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void mmcsd_dis_status_flags(struct mmcsdCtlr *ctlr, unsigned int flags)
{
	and_regl(~flags, &ctlr->sd_ie);
}

/* -- mmcsd_get_cmd_status -----------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
unsigned int mmcsd_get_cmd_status(struct mmcsdCtlr *ctlr)
{
	unsigned int status;

	/* wait for command to complete or timeout */
	
	/*mmcsd_is_cmd_complete(ctlr);*/
	while( !mmcsd_cmd_complete(ctlr) && !mmcsd_cmd_timeout(ctlr) )
		;

	status = mmcsd_get_status(ctlr,0xFFFFFFFF);
	/* PRINT("STATUS = 0x%X\n\r",status); */

	/* see if the command completed */
	status = mmcsd_cmd_complete(ctlr); 

	/* clear complete and timeout flags */
	mmcsd_clear_status(ctlr, MMC_STAT_CC | MMC_STAT_CTO);
	
	/* return 1 if cmd completed else return 0 */
	return status;
}	

/* -- mmcsd_cmd_complete -------------------------------------------------
 *	returns 
 *		0 - not complete
 *		1 - complete
 * ---------------------------------------------------------------------*/
unsigned int mmcsd_cmd_complete(struct mmcsdCtlr *ctlr)
{
	return mmcsd_get_status(ctlr, MMC_STAT_CC) ? 1 : 0;
}

/* -- mmcsd_cmd_timeout --------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
unsigned int mmcsd_cmd_timeout(struct mmcsdCtlr *ctlr)
{
	return (mmcsd_get_status(ctlr, MMC_STAT_CTO) >> 16) ? 1 : 0;
}

/* -- mmcsd_get_status ---------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
unsigned int mmcsd_get_status(struct mmcsdCtlr *ctlr, unsigned int mask)
{
	return (readl(&ctlr->sd_stat) & mask);
}

/* -- mmcsd_data_timeout -------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void mmcsd_data_timeout(struct mmcsdCtlr *ctlr, unsigned int timeout)
{
	/* Mask out Data Timeout bits in SYCTL register */
	and_regl(~MMC_SYSCTL_DTO_MASK, &ctlr->sd_sysctl);
	/* set data timeout */
	or_regl(timeout, &ctlr->sd_sysctl);
}

/* -- mmcsd_is_card_present ----------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
int mmcsd_is_card_present(struct mmcsdCtlr *ctlr)
{
	return ( (readl(&ctlr->sd_pstate) & MMC_PSTAT_CINS) >> 
						MMC_PSTAT_CINS_SHIFT );
}

/* -- mmcsd_send_init_seq ------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
int mmcsd_send_init_seq(struct mmcsdCtlr *ctlr)
{
	unsigned int retval;

	/* Enable Command Complete signal */
	or_regl(MMC_ISE_CC_SIGEN, &ctlr->sd_ie);

	/* Initiate the INIT stream NOTE: this seems backward to the TRM */
	or_regl(MMC_CON_INIT, &ctlr->sd_con);

	/*writel(0x00, &ctlr->sd_cmd);*/
	ctlr->sd_cmd = 0x00;
	 	
	/* wait for command to complete */
	retval = mmcsd_is_cmd_complete(ctlr);

	/* clear status and init flags */
	and_regl(~MMC_CON_INIT, &ctlr->sd_con);
	mmcsd_clear_status(ctlr, 0xFFFFFFFF);

	return retval;	
}

/* -- mmcsd_clear_status -------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void mmcsd_clear_status(struct mmcsdCtlr *ctlr, unsigned int flag)
{
	writel(flag, &ctlr->sd_stat);
}	

/* -- mmcsd_is_cmd_complete ----------------------------------------------
 *
 * ---------------------------------------------------------------------*/
int mmcsd_is_cmd_complete(struct mmcsdCtlr *ctlr)
{
	unsigned int timeout = 0xFFFF;
	
	do
	{
		if(readl(&ctlr->sd_stat) & MMC_STAT_CC)
		{
			break;
		}
	}while(--timeout);

	if(!timeout)
	{
		return -1;
	}	
	
	return 0;
}	

/* -- mmcsd_set_bus_freq -------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
int mmcsd_set_bus_freq(struct mmcsdCtlr *ctlr, unsigned int busFreq)
{
	unsigned int fdiv = 0;

	/* first enable in the internal clocks */
	if(mmcsd_int_clk(ctlr, MMC_INT_CLK_ON) != 0)
	{
		return -1;
	}

	/* check to see if we should bypass the divisor */
	if(busFreq <= MMC_SYS_CLK)
	{
		/* make sure we stay with in upper and lower division bounds */
		fdiv = MMC_SYS_CLK/busFreq;
		fdiv = (fdiv < 2) ? 2 : fdiv;
		fdiv = (fdiv > 1023) ? 1023 : fdiv;

		/* Now ensure that set freq is less then or equal to desired */
		while((MMC_SYS_CLK/fdiv) > busFreq)
		{
			if(fdiv == 1023)
			{
				/* desired clock freq is too low */
				return -1;
			}
			fdiv++;
		}
	}
		
	/* mask out CLKD bits of SYSCTL register */
	and_regl(~MMC_SYSCTL_CLKD_MASK, &ctlr->sd_sysctl);
	or_regl((fdiv << MMC_SYSCTL_CLKD_SHIFT), &ctlr->sd_sysctl);

	/* wait for interface clock to stabalize */
	if(mmcsd_is_clk_stable(ctlr) != 0)
	{
		return -1;
	}
		
	/* enable the clock to the card */
	or_regl(MMC_SYSCTL_CEN, &ctlr->sd_sysctl);

	return 0;
}			

/* -- mmcsd_int_clk ------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
int mmcsd_int_clk(struct mmcsdCtlr *ctlr, unsigned int state)
{

	/* mask out the internal clock enable (ICE) bit in control register */
	and_regl(~MMC_SYSCTL_ICE, &ctlr->sd_sysctl);

	/* set clock state */
	or_regl(state, &ctlr->sd_sysctl);

	/* if turning the clock on check to ensure it stabalizes */
	if(state == MMC_INT_CLK_ON)
	{
		if(mmcsd_is_clk_stable(ctlr) != 0)
		{
			/* clock did not stabalize */	
			return -1;
		}
	}

	/* Check sysctl register */
	/*PRINT("SYSCTLR 0x%X = 0x%X\n\r",&ctlr->sd_sysctl,*/
				/*readl(&ctlr->sd_sysctl));*/
	
	return 0;
}		

/* -- mmcsd_is_clk_stable ------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
int mmcsd_is_clk_stable(struct mmcsdCtlr *ctlr)
{
	unsigned int timeout = 0xFFFF;
	do
	{
		if(readl(&ctlr->sd_sysctl) & MMC_SYSCTL_ICS)
		{
			break;
		}
	}while(--timeout);
	
	/*PRINT(" clk stable TO = 0x%X\n\r", timeout);*/

	/* check exit state */
	if(!timeout)
		return -1;
	
	return 0;
}	

/* -- mmcsd_power --------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
int mmcsd_power(struct mmcsdCtlr *ctlr, unsigned int power)
{
	volatile unsigned int timeout = 0xFFFFF;
	
	/* clear out power bit of HCTL register */
	and_regl(~MMC_HCTL_PWR_MASK, &ctlr->sd_hctl);
	/* set/clear power bit */
	or_regl(power, &ctlr->sd_hctl);

	if(power == MMC_HCTL_PWR_ON)
	{
		do
		{
			if(readl(&ctlr->sd_hctl) & MMC_HCTL_PWR_ON)
			{
				break;
			}
		}while(--timeout);
	}

	/*PRINT("Power on TO = 0x%X\n\r", timeout);*/

	/* check exit condition */
	if(!timeout)
		return -1;

	return 0;
}

/* -- mmcsd_bus_volt -----------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void mmcsd_bus_volt(struct mmcsdCtlr *ctlr, unsigned int voltage)
{
	/* mask off SDVS bits in hctl register */
	and_regl(~MMC_HCTL_SDVS_MASK, &ctlr->sd_hctl);
	
	/* set SDVS bits */
	or_regl(voltage, &ctlr->sd_hctl);
}


/* -- mmcsd_bus_width ----------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void mmcsd_bus_width(struct mmcsdCtlr *ctlr, unsigned int width)
{
	switch(width)
	{
		case MMC_BUS_WIDTH_8BIT:
			/* set DW8 bit in con register */
			or_regl(MMC_CON_DW8, &ctlr->sd_con);
			break;
		case MMC_BUS_WIDTH_4BIT:
			/* clear DW8 bit in con register */
			and_regl(~MMC_CON_DW8, &ctlr->sd_con);
			/* set DTW bit in HTCL register */
			or_regl(MMC_HCTL_DTW, &ctlr->sd_hctl);
			break;
		case MMC_BUS_WIDTH_1BIT:
			/* clear DW8 bit in con register */
			and_regl(~MMC_CON_DW8, &ctlr->sd_con);
			/* clear DTW bit in HTCL register */
			and_regl(~MMC_HCTL_DTW, &ctlr->sd_hctl);
			break;
	}
}

/* -- mmcsd_sys_config ---------------------------------------------------
 * TODO: better bit masking needs to be done here.
 * ---------------------------------------------------------------------*/
void mmcsd_sys_config(struct mmcsdCtlr *ctlr, unsigned int flag)
{
	/* zero out configuration register */
	and_regl(~MMC_SYSCONFIG_MASK, &ctlr->sd_sysconfig);

	/* set config register according to past config flags */
	or_regl(flag, &ctlr->sd_sysconfig);
}

/* -- mmcsd_softreset ----------------------------------------------------
 * This function performs a soft reset of the mmc/sd controller
 * ---------------------------------------------------------------------*/
int mmcsd_softreset(struct mmcsdCtlr *ctlr)
{
	volatile unsigned int timeout = 0xFFFF;
	
	/* set the soft reset bit in the config register */
	or_regl(MMC_SOFTRESET, &ctlr->sd_sysconfig);
	
	/* wait until reset is complete or we time out */
	do
	{
		if(readl(&ctlr->sd_sysstatus) & MMC_RESETDONE)
		{
			break;
		}
	}while(--timeout);
	
	/*PRINT("softreset TO = 0x%X\n\r", timeout);*/

	/* check exit condition */
	if(!timeout)
		return -1;

	return 0;
}

/* -- mmcsd_linereset ----------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
int mmcsd_linereset(struct mmcsdCtlr *ctlr, unsigned int flag)
{
	volatile unsigned int timeout = 0xFFFF;
	
	/* write reset flag to sys Ctl register */
	or_regl(flag, &ctlr->sd_sysctl);

	/* wait for desired lines to reset */
	do
	{
		if((readl(&ctlr->sd_sysctl) & flag)	!= flag )
		{
			break;
		}
	}while(--timeout);
	
	/*PRINT("line reset TO = 0x%X\n\r", timeout);*/

	/* check exit condition */
	if(!timeout)
		return -1;

	return 0;
}

/* -- mmcsd_volt_capa ----------------------------------------------------
 * This function sets the mmc bus volatage capabilities. 
 * ---------------------------------------------------------------------*/
void mmcsd_volt_capa(struct mmcsdCtlr *ctlr, unsigned int flag)
{
	/* mask out voltage bits is CAPA register */
	and_regl(~MMC_CAPA_VOLT_MASK, &ctlr->sd_capa);
	/* set voltage bits in CAPA register */
	or_regl(flag, &ctlr->sd_capa);
}
