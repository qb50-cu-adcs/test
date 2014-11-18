/* ***********************************************************************
 * file: 		mmcsd.h
 * author:	jacob cook
 * created on:	6/25/2014
 * ***********************************************************************
 */

#ifndef __MMCSD_H
#define __MMCSD_H

#define	MMC0_BASE	(0x48060000)
#define MMC1_BASE	(0x481D8000) 
#define MMC2_BASE (0x47810000)

/* System clock specifications */
#define MMC_SYS_CLK (96000000) /* 96 Mhz */
#define MMC_INIT_CLK (400000) /* 400 Khz */

/* System configuration flags */
#define MMC_SYSCONFIG_MASK 		(0x31F)
#define MMC_CLK_ACTIVITY_MASK (0x300)
#define MMC_INTERFACE_CLK 		(0x1 << 8)
#define MMC_FUNCTIONAL_CLK	 	(0x1 << 9)
#define MMC_ENAWAKEUP					(0x1 << 2)
#define MMC_SOFTRESET					(0x1 << 1)
#define MMC_AUTOIDLE					(0x1)

/* SYSSTATUS System Status flags */
#define MMC_RESETDONE		(0x1)

/* Bus width macros */
#define MMC_BUS_WIDTH_8BIT		(0x8)
#define MMC_BUS_WIDTH_4BIT		(0x4)
#define MMC_BUS_WIDTH_1BIT		(0x1)

/* CON Configuration Register */
#define MMC_CON_INIT		(0x1 << 1)
#define MMC_CON_DW8 		(0x1 << 5) /* 8 bit data bus */


/* SYSCTRL System Control Register */
#define MMC_SYSCTL_DTO_SHIFT	(16)
#define MMC_SYSCTL_DTO_MASK		(0xFF << MMC_SYSCTL_DTO_SHIFT)
#define MMC_DATA_TIMEOUT(n)		((((n) - 13) & 0xF) << MMC_SYSCTL_DTO_SHIFT)
#define MMC_SYSCTL_CLKD_MASK (0xFFC0)
#define MMC_SYSCTL_CLKD_SHIFT (0x6)
#define MMC_SYSCTL_ICE		(0x1)
#define MMC_INT_CLK_ON		(0x1)
#define MMC_INT_CLK_OFF		(0x0)
#define MMC_SYSCTL_ICS		(0x1 << 1)
#define MMC_SYSCTL_CEN		(0x1 << 2)

/* PSTAT Host controller status register */
#define MMC_READ (1)
#define MMC_WRITE (0)
#define MMC_PSTAT_CINS_SHIFT (16)
#define MMC_PSTAT_CINS	(0x1 << MMC_PSTAT_CINS_SHIFT)
#define MMC_PSTATE_BRE 	(0x1 << 11)
#define MMC_PSTATE_BWE	(0x1 << 10)

/* HCTL Host Controller Register */
#define MMC_HCTL_PWR_MASK		(0x1 << 8)
#define MMC_HCTL_PWR_ON			(0x1 << 8)
#define MMC_HCTL_PWR_OFF		(0x0 << 8)
#define MMC_HCTL_SDVS_MASK 	(0xE00)
#define MMC_HCTL_SDVS_1P8		(0x5 << 9)
#define MMC_HCTL_SDVS_3P0		(0x6 << 9)
#define MMC_HCTL_SDVS_3P3		(0x7 << 9)
#define MMC_HCTL_DTW				(0x1 << 1)

/* STAT Interrupt status register */
#define MMC_STAT_CC		(0x1)
#define MMC_STAT_TC		(0x1 << 1)
#define MMC_STAT_BWR	(0x1 << 4)
#define MMC_STAT_BRR	(0x1 << 5)
#define MMC_STAT_CTO	(0x1 << 16)
#define MMC_STAT_DTO	(0x1 << 20)

/* ISE Interrupt signal Enable register */
#define MMC_ISE_CC_SIGEN	(0x1)
#define MMC_ISE_TC_SIGEN	(0x1 << 1)
#define MMC_ISE_BWR_SIGEN	(0x1 << 4)
#define MMC_ISE_BRR_SIGEN	(0x1 << 5)
#define MMC_ISE_CTO_SIGEN (0x1 << 16)
#define MMC_ISE_DTO_SIGEN (0x1 << 20)
#define MMC_ISE_CERR_SIGEN (0x1 << 28)

/* BLK Block length an number register */
#define MMC_BLK_BLEN_MASK		(0xFFF)
#define MMC_BLK_NBLK_SHIFT	(16)
#define MMC_BLK_NBLK_MASK		(0xFFFF << MMC_BLK_NBLK_SHIFT)

/* Line Reset Definitions */
#define MMC_RESET_DATA 	(0x1 << 26) 
#define MMC_RESET_CMD 	(0x1 << 25)
#define MMC_RESET_ALL		(0x1 << 24)

/* Voltage Capability Settings */
#define MMC_CAPA_VOLT_MASK 	(0x7 << 24)
#define MMC_CAPA_VOLT_1P8 	(0x1 << 26)
#define MMC_CAPA_VOLT_3P0 	(0x1 << 25)
#define MMC_CAPA_VOLT_3P3 	(0x1 << 24)

/* CMD command register */
#define MMC_CMD_DP					(0x1 << 21)
#define MMC_CMD_MSBS				(0x1 << 5)
#define MMC_CMD_BCE					(0x1 << 1)
#define MMC_CMD_DE					(0x1)
#define MMC_CMD(cmd, type, rsptype, rw)	 \
		( cmd << MMC_CMD_INDX_SHIFT | type | rsptype | rw )
	/* cmd index */
#define MMC_CMD_INDX_SHIFT	(24)
	/* cmd type */
#define MMC_CMD_CMD_TYPE_MASK (0x3 << 22)
#define MMC_CMD_TYPE_OTHER		(0x0 << 22)
#define MMC_CMD_TYPE_SUSPEND	(0x1 << 22)
#define MMC_CMD_TYPE_RESUME		(0x2 << 22)
#define MMC_CMD_TYPE_ABORT		(0x3 << 22)
	/* rsp type */
#define MMC_CMD_RSP_TYPE_MASK (0x3 << 16)
#define MMC_RSP_TYPE_NONE			(0x0 << 16)
#define MMC_RSP_TYPE_136BITS	(0x1 << 16)
#define MMC_RSP_TYPE_48BITS		(0x2 << 16)
#define MMC_RSP_TYPE_48WBUSY	(0x3 << 16)
	/* read/write */
#define MMC_CMD_DDIR	(0x1 << 4)
#define MMC_CMD_RD		(0x1 << 4)
#define MMC_CMD_WR		(0x0 << 4)

struct mmcsdCtlr 
{
	unsigned int res1[68];			/* 0x00 - 0x10C */
	unsigned int sd_sysconfig;	/* 0x110 */
	unsigned int sd_sysstatus;	/* 0x114 */
	unsigned int res2[3];				/* 0x118 - 0x120 */
	unsigned int sd_csre;				/* 0x124 */
	unsigned int sd_systest;		/* 0x128 */
	unsigned int sd_con;				/* 0x12C */
	unsigned int sd_pwcnt;			/* 0x130 */
	unsigned int res3[51];			/* 0x134 - 0x1FC */
	unsigned int sd_sdmasa;		  /* 0x200 */
	unsigned int sd_blk;				/* 0x204 */
	unsigned int sd_arg;				/* 0x208 */
	unsigned int sd_cmd;				/* 0x20C */
	unsigned int sd_rsp[4];			/* 0x210 0x21C */
	unsigned int sd_data;				/* 0x220 */
	unsigned int sd_pstate;			/* 0x224 */
	unsigned int sd_hctl;				/* 0x228 */
	unsigned int sd_sysctl;			/* 0x22C */
	unsigned int sd_stat;				/* 0x230 */
	unsigned int sd_ie;					/* 0x234 */
	unsigned int sd_ise;				/* 0x238 */
	unsigned int sd_ac12;				/* 0x23C */
	unsigned int sd_capa;				/* 0x240 */
	unsigned int res4;					/* 0x244 */	
	unsigned int sd_cur_capa;		/* 0x248 */
	unsigned int res5;					/* 0x24C */	
	unsigned int sd_fe;					/* 0x250 */
	unsigned int sd_admaes;			/* 0x254 */
	unsigned int sd_admasal;		/* 0x258 */
	unsigned int sd_admasah;		/* 0x25C */
	unsigned int res6[36];			/* 0x260 - 0x2F8 */
	unsigned int sd_rev;				/* 0x2FC */
};

/* function prototypes */
int mmcsd_init(struct mmcsdCtlr *ctlr);
unsigned int mmcsd_read_data(struct mmcsdCtlr *ctlr);
void mmcsd_write_data(struct mmcsdCtlr *ctlr, unsigned int data);
unsigned int mmcsd_get_tran_status(struct mmcsdCtlr *ctlr, 
																			unsigned int dir);
unsigned int mmcsd_write_complete(struct mmcsdCtlr *ctlr);
unsigned int mmcsd_read_complete(struct mmcsdCtlr *ctlr);
unsigned int mmcsd_tran_complete(struct mmcsdCtlr *ctlr);
unsigned int mmcsd_tran_timeout(struct mmcsdCtlr *ctlr);
void mmcsd_set_blklen(struct mmcsdCtlr *ctlr, unsigned int blklen);
void mmcsd_get_rsp(struct mmcsdCtlr *ctlr, unsigned int *rsp);
void mmcsd_send_cmd(struct mmcsdCtlr *ctlr, unsigned int cmd,
										unsigned int cmdarg, void *data,
										unsigned int nblks, unsigned int dmaEn);
void mmcsd_en_status_flags(struct mmcsdCtlr *ctlr, unsigned int flags);
void mmcsd_dis_status_flags(struct mmcsdCtlr *ctlr, unsigned int flags);
unsigned int mmcsd_get_cmd_status(struct mmcsdCtlr *ctlr);
unsigned int mmcsd_cmd_complete(struct mmcsdCtlr *ctlr);
unsigned int mmcsd_cmd_timeout(struct mmcsdCtlr *ctlr);
unsigned int mmcsd_get_status(struct mmcsdCtlr *ctlr,	unsigned int mask);
void mmcsd_data_timeout(struct mmcsdCtlr *ctlr, unsigned int timeout);
int mmcsd_is_card_present(struct mmcsdCtlr *ctlr);
int mmcsd_send_init_seq(struct mmcsdCtlr *ctlr);
void mmcsd_clear_status(struct mmcsdCtlr *ctlr, unsigned int flag);
int mmcsd_is_cmd_complete(struct mmcsdCtlr *ctlr);
int mmcsd_set_bus_freq(struct mmcsdCtlr *ctlr, unsigned int busFreq);
int mmcsd_int_clk(struct mmcsdCtlr *ctlr, unsigned int state);
int mmcsd_is_clk_stable(struct mmcsdCtlr *ctlr);
int mmcsd_power(struct mmcsdCtlr *ctlr, unsigned int power);
void mmcsd_bus_volt(struct mmcsdCtlr *ctlr, unsigned int voltage);
void mmcsd_bus_width(struct mmcsdCtlr *ctlr, unsigned int width);
void mmcsd_sys_config(struct mmcsdCtlr *ctlr, unsigned int flag);
int mmcsd_softreset(struct mmcsdCtlr *ctlr);
int mmcsd_linereset(struct mmcsdCtlr *ctlr, unsigned int flag);
void mmcsd_volt_capa(struct mmcsdCtlr *ctlr, unsigned int flag);

#endif
