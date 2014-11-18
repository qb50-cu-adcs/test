/* ***********************************************************************
 * file: 		sd_card.h
 * author:	jacob cook
 * created on:	6/26/2014
 * ***********************************************************************
 */

#ifndef __SD_CARD_H
#define __SD_CARD_H



struct cardInfo
{
	unsigned int cardType;
	unsigned int rca; /* Relative Card Address */
	unsigned int ocr;
	unsigned int cid[4];
	unsigned int csd[4];	
	unsigned int scr[2];
	unsigned int sd_ver;
	unsigned int buswidth;
	unsigned int transpeed;
	unsigned int busfreq;
	unsigned int high_capacity;
	unsigned int blklen;
	unsigned int nblks;
	unsigned int size;
	struct mmcsdCtlr *ctlr;
};

/* card info definitions */
#define MMCSD_CARD_SD 1
#define MMCSD_CARD_MMC 0


struct sdCmd
{
	unsigned int idx;
	unsigned int flags;
	unsigned int arg;
	char *data;
	unsigned int nblks;
	unsigned int rsp[4];
};

#define SD_CMD(x) (x)

/* SD Operating Voltage */
#define SD_VOLT_2P7_3P6 (0x000100)

/* SD Card Pattern Check */
#define SD_CHECK_PATTERN	(0xAA)

/* SD High Capacity */
#define SD_OCR_HIGH_CAPACITY	(0x1 << 30)

/* Voltage flags */
#define SD_OCR_VDD_WILDCARD		(0x1FF << 15)

/* Bus width macros */
#define SD_BUSWIDTH_1BIT		MMC_BUS_WIDTH_1BIT
#define SD_BUSWIDTH_4BIT		MMC_BUS_WIDTH_4BIT	

/* CMD6 Switch mode arguments for High speed cards */
#define SD_SWITCH_MODE		(0x80FFFFFF)
#define SD_CMD6_GRP1_SEL	(0xFFFFFFF0)
#define SD_CMD6_GRP1_HS		(0x1)

/* CSD register macros */
#define SD_CSD_VERSION(crd) \
				(((crd)->csd[3] & 0xC0000000) >> 30) /* bits 126:127 */

#define SD_CSD_TSPEED(crd)	((crd)->csd[3] & 0xFF) /* 96:103 */
#define SD_TSPEED_25MBPS	(0x32)
#define SD_TSPEED_50MBPS	(0x5A)
#define SD_CSD_RDBLKLEN(crd)	(1 << (((crd)->csd[2] & 0xF0000) >> 16) )
#define SD_CSD_SIZE(crd) ( SD_CSD_VERSION((crd)) ? SD_CSD1_SIZE((crd)) : \
																									 SD_CSD0_SIZE((crd)) )
#define SD_CSD_NBLK(crd) ( SD_CSD_VERSION((crd)) ? \
		( (SD_CSD1_SIZE((crd))/SD_CSD_RDBLKLEN((crd)))*1024 ) : \
		SD_CSD0_NUMBLK((crd)) )

	/* Version 1 */
#define SD_CSD1_DEV_SIZE(crd)	( (((crd)->csd[1] & 0xFFFF0000) >> 16 ) | \
																(((crd)->csd[2] & 0x3F) << 16 ) )
#define SD_CSD1_SIZE(crd)			(((SD_CSD1_DEV_SIZE(crd)) + 1) * (512))
	/* Version 0 */
#define SD_CSD0_DEV_SIZE(crd) ((((crd)->csd[1] & 0xC0000000) >> 30 ) | \
																(((crd)->csd[2] & 0x3FF) << 2 ) )
#define SD_CSD0_MULT(crd)			(((crd)->csd[1] & 0x38000) >> 15 )
#define SD_CSD0_NUMBLK(crd) 	((SD_CSD0_DEV_SIZE((crd)) + 1) * \
																(1 << (SD_CSD0_MULT((crd)) + 2))) 
#define SD_CSD0_SIZE(crd)			((SD_CSD0_NUMBLK((crd))) * \
																(1 << (SD_CSD_RDBLKLEN(crd))))

/* SCR register macros */
#define SD_SCR_VER(crd) ((crd)->scr[0] & 0xF)
#define SD_SCR_BUSWIDTH(crd) (((crd)->scr[0] & 0xF00) >> 8)
	

/* RCA register address and stat macros */
#define SD_RCA_ADDR(x)		((0xFFFF0000 & x) >> 16)
#define SD_RCA_STAT(x)		(0xFFFF & x)

/* Command/Response flags */
#define SD_CMD_MASK				(0x7)
#define SD_RSP_MASK				(0x38)
#define SD_CMDRSP_STOP		(0x1 << 0)
#define SD_CMDRSP_FS			(0x1 << 1)
#define SD_CMDRSP_ABORT		(0x1 << 2)
#define SD_CMDRSP_NONE 		(0x1 << 3)
#define SD_CMDRSP_BUSY		(0x1 << 4)
#define SD_CMDRSP_136BIT	(0x1 << 5)
#define SD_CMDRSP_DATA		(0x1 << 6)
#define SD_CMDRSP_READ		(0x1 << 7)
#define SD_CMDRSP_WRITE		(0x1 << 8)

#include "mmcsd.h"

/* function prototypes */
unsigned int sdcard_write(struct cardInfo *card, void *ptr, 
											unsigned int block, unsigned int nblks);
unsigned int sdcard_read(struct cardInfo *card, void *ptr,
											unsigned int block, unsigned int nblks);
unsigned int sdcard_read_data(struct cardInfo *card, void *ptr,
												unsigned int nblks);
unsigned int sdcard_write_data(struct cardInfo *card, void *ptr, 
												unsigned int nblks);
unsigned int sdcard_write_blk(struct cardInfo *card, unsigned int blklen,
                              unsigned int *data);
unsigned int sdcard_stop(struct cardInfo *card);
unsigned int sdcard_set_buswidth(struct cardInfo *card);
unsigned int sdcard_set_bus_freq(struct cardInfo *card);
int sdcard_init(struct cardInfo *card);
void sdcard_print_info(struct cardInfo *card);
unsigned int sdcard_read_blk(struct cardInfo *card, unsigned int blklen,
															unsigned int *data);
unsigned int sdcard_check_type(struct cardInfo *card);
unsigned int sdcard_send_acmd(struct cardInfo *card, struct sdCmd *cmd);
int sdcard_reset(struct cardInfo *card);
int sdcard_send_cmd(struct cardInfo *card, struct sdCmd *cmd);
void sdcard_en_stat_flags(struct cardInfo *card);

#endif
