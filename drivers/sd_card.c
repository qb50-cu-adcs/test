/* file: 		sd_card.c
 * author: 	jacob cook
 * created on: 6/26/2014
 */

#include "dbg.h"
#include "types.h"
#include "io.h"
#include "sd_card.h"
#include "mmcsd.h"

/* allocate a buffer for data transfers */
#define BUFFER_SIZE 128
static char dataBuffer[BUFFER_SIZE] __attribute__((aligned(BUFFER_SIZE)));	

/* -- sdcard_write -------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int sdcard_write(struct cardInfo *card, void *ptr, 
											unsigned int block, unsigned int nblks)
{
	struct sdCmd cmd;
	unsigned int address;	
	
	/* High capicity cards are addressed by blocks while standard
   * capacity is in actaul bytes
   */
	if(card->high_capacity)
	{
		address = block;
	}
	else
	{
		address = block*card->blklen;
	}

	/* set up controller for block transfers */
	mmcsd_set_blklen(card->ctlr, 512);
	
	/* set up cmd */
	cmd.flags = SD_CMDRSP_WRITE | SD_CMDRSP_DATA;
	cmd.arg = address;
	cmd.nblks = nblks;

	if(nblks > 1)
	{
		cmd.idx = SD_CMD(25);
		cmd.flags |= SD_CMDRSP_ABORT;
	}
	else
	{
		cmd.idx = SD_CMD(24);
	}

	/* send write command to sd card */
	if(sdcard_send_cmd(card, &cmd) == 0)
	{
		PRINT("CMD FAILED\n\r");
		return 0;
	}


	/* write data */
	if(sdcard_write_data(card, ptr, nblks) == 0)
	{
		PRINT("DATA READ FAILED\n\r");
		return 0;
	}

	if(mmcsd_get_tran_status(card->ctlr, 0) == 0)
	{
		PRINT("TRANS FAILED\n\r");
		return 0;
	}

	if(nblks > 1)
	{
		if(sdcard_stop(card) == 0)
			return 0;
	}

	return 1;
}	

/* -- sdcard_read --------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int sdcard_read(struct cardInfo *card, void *ptr, 
													unsigned int block, unsigned int nblks)
{
	unsigned int address;
	struct sdCmd cmd;

	/* High capicity cards are addressed by blocks while standard
   * capacity is in actaul bytes
   */
	if(card->high_capacity)
	{
		address = block;
	}
	else
	{
		address = block*card->blklen;
	}

	/* set up controller for block transfers */
	mmcsd_set_blklen(card->ctlr, 512);

	/* set up cmd */
	cmd.flags = SD_CMDRSP_READ | SD_CMDRSP_DATA;
	cmd.arg = address;
	cmd.nblks = nblks;

	if(nblks > 1)
	{
		cmd.idx = SD_CMD(18);
		cmd.flags |= SD_CMDRSP_ABORT;
	}
	else
	{
		cmd.idx = SD_CMD(17);
	}

	/* send read command to sd card */
	if(sdcard_send_cmd(card, &cmd) == 0)
	{
		PRINT("CMD FAILED\n\r");
		return 0;
	}

	/* read data */
	if(sdcard_read_data(card, ptr, nblks) == 0)
		return 0;

	if(mmcsd_get_tran_status(card->ctlr, 0) == 0)
		return 0;

	if(nblks > 1)
	{
		if(sdcard_stop(card) == 0)
			return 0;
	}

	return 1;
}	
	
/* -- sdcard_read_data ---------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int sdcard_read_data(struct cardInfo *card, void *ptr, 
																	unsigned int nblks)
{
	unsigned int i;
	unsigned int *data = (unsigned int *)ptr;

	for(i=nblks;i!=0;i--)
	{
		if(sdcard_read_blk(card,512,data) == 0)
			return 0;
		data += 128; /* 512/4 */
	}

	return 1;
}

/* -- sdcard_write_data --------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int sdcard_write_data(struct cardInfo *card, void *ptr, 
												unsigned int nblks)
{
	unsigned int i;
	unsigned int *data = (unsigned int *)ptr;

	for(i=nblks;i!=0;i--)
	{
		if(sdcard_write_blk(card, 512, data) == 0)
			return 0;
		data +=  128; /* 512/4 */
	}

	return 1;
}

/* -- sdcard_write_blk ---------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int sdcard_write_blk(struct cardInfo *card, unsigned int blklen,
                              unsigned int *data)
{
	unsigned int i;

	/* adjust for 32 bit writes */
	blklen = blklen/4;

	/* check to see if data is ready */
	while(!mmcsd_write_complete(card->ctlr));
	
	mmcsd_clear_status(card->ctlr, MMC_STAT_BWR);

	/* write data to data buffer */
	for(i=blklen;i!=0;i--)
	{
		mmcsd_write_data(card->ctlr,*data++);
	}

	return 1;
}

/* -- sdcard_stop --------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int sdcard_stop(struct cardInfo *card)
{
	struct sdCmd cmd;
	
	/* build command */
	cmd.idx = SD_CMD(12);
	cmd.flags = SD_CMDRSP_BUSY;
	cmd.arg = 0;

	return sdcard_send_cmd(card, &cmd);
}

/* -- sdcard_set_buswidth ------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int sdcard_set_buswidth(struct cardInfo *card)
{

	struct sdCmd acmd;

	acmd.idx = SD_CMD(6);
	acmd.arg = SD_BUSWIDTH_1BIT >> 1;
	acmd.flags = 0;

	/* If card and controller support 4 bit set buswidth to 4 */
	if((card->buswidth & SD_BUSWIDTH_4BIT) && 
		(card->buswidth & SD_BUSWIDTH_4BIT))
	{
		acmd.arg = SD_BUSWIDTH_4BIT >> 1;
	}

	/* send application command */
	if(sdcard_send_acmd(card, &acmd) == 0)
		return 0;
	
	/* configure controller to operate with same buswidth */
	mmcsd_bus_width(card->ctlr, (0x1 << acmd.arg));

	/* update info struct */
	card->buswidth = 0x1 << acmd.arg;

	return 1;
}

/* -- sdcard_set_bus_freq ------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int sdcard_set_bus_freq(struct cardInfo *card)
{
	struct sdCmd cmd;

	/* unsigned int freq; */
	
		/* set blklen for data transfer */
		mmcsd_set_blklen(card->ctlr, 64);
		
		/* set up cmd */
		cmd.idx = SD_CMD(6);
		cmd.arg = ((SD_SWITCH_MODE & SD_CMD6_GRP1_SEL) | SD_CMD6_GRP1_HS);
		cmd.flags = SD_CMDRSP_READ | SD_CMDRSP_DATA;
		cmd.nblks = 1;
		cmd.data = dataBuffer;

		/* send cmd */
		if(sdcard_send_cmd(card, &cmd) == 0)
			return 0;
		
		/* read cmd data */
		if(sdcard_read_blk(card, 64, (unsigned int *)dataBuffer) == 0)
			return 0;

		/* freq = card->transpeed; */

		/* check High speed flag */		
		if((dataBuffer[16] & 0xF) == SD_CMD6_GRP1_HS)
		{
			card->transpeed = SD_TSPEED_50MBPS;
			/* PRINT("SD Card is HIGH SPEED\n\r"); */
		}
		
		/* Set bus frequency in Info struct */
		if(card->transpeed == SD_TSPEED_50MBPS)
		{
			card->busfreq = 48000000;	/* max frequency we're capable of */
		}
		else
		{
			card->busfreq = 25000000;
		}

		/* set mmcsd bus frequency */
		if(mmcsd_set_bus_freq(card->ctlr,card->busfreq) != 0)
			return 0;
		
		return 1;
}	
		
/* -- sdcard_init --------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
int sdcard_init(struct cardInfo *card)
{
	unsigned int i;
	unsigned int timeout = 0xFFFF;
	unsigned int retval = 0;
	struct sdCmd cmd;
	
	/* Enable Interrupt flags.  Note we are not using interrupts but we 
 	 * polling these flags.
   */
	sdcard_en_stat_flags(card);

	/* CMD0 - Reset card */
	/* PRINT("Card Reset\n\r"); */
	retval = sdcard_reset(card);

	if(retval == 0)
		return 0;

	/* check card type */
	/* PRINT("Type Check\n\r"); */
	retval = sdcard_check_type(card);	
	retval = 1;

	if(retval == 1)
	{
		/* set card type */
		card->cardType = MMCSD_CARD_SD;

		/* Reset card again */
		retval = sdcard_reset(card);
	
		if(retval == 0)
			return 0;
		
		/* set operating voltage */
		cmd.idx = SD_CMD(8);
		cmd.flags = 0;
		cmd.arg = (SD_VOLT_2P7_3P6 | SD_CHECK_PATTERN);
	
		retval = sdcard_send_cmd(card, &cmd);
		if(retval == 0)
			return 0;

		/* Poll card until the card status (BIT31 of OCR) is powered up */
		do
		{
			cmd.idx = SD_CMD(41);
			cmd.flags = 0;
			cmd.arg = (SD_OCR_VDD_WILDCARD | SD_OCR_HIGH_CAPACITY);
	
			sdcard_send_acmd(card, &cmd);
		}while(!(cmd.rsp[0] & (unsigned int)(0x1 << 31)) && --timeout);

		/* check exit condition */
		if(timeout == 0)
			return 0;

		/* store OCR and High capacity information */
		card->ocr = cmd.rsp[0];
		card->high_capacity = (card->ocr & SD_OCR_HIGH_CAPACITY) ? 1 : 0;
		
		/* Send command 2 to get card Id register */	
		cmd.idx = SD_CMD(2);
		cmd.flags = SD_CMDRSP_136BIT;
		cmd.arg = 0;
		retval = sdcard_send_cmd(card, &cmd);
		
		/* copy card Id register to data structure */
		for(i=0;i<4;i++)
		{
			card->cid[i] = cmd.rsp[i];
		}

		/* Send CMD3 to get relative address from card */
		cmd.idx = SD_CMD(3);
		cmd.flags = 0;
		cmd.arg = 0;
		if(sdcard_send_cmd(card, &cmd) == 0)
			return 0;
		
		card->rca = SD_RCA_ADDR(cmd.rsp[0]);
	
		/* send CMD9 to get card specific data */
		cmd.idx = SD_CMD(9);
		cmd.flags = SD_CMDRSP_136BIT;
		cmd.arg = card->rca << 16;
		if(sdcard_send_cmd(card, &cmd) == 0)
			return 0;
	
		/* copy CSD to data structure */
		for(i=0;i<4;i++)
			card->csd[i] = cmd.rsp[i];

		/* store CSD data (struture version handled in macros) */
		card->transpeed = SD_CSD_TSPEED(card);
		card->blklen = SD_CSD_RDBLKLEN(card);
		card->size = SD_CSD_SIZE(card);
		card->nblks = SD_CSD_NBLK(card);

		/* set blk len to 512 bytes if not high capacity */
		cmd.idx = SD_CMD(16);
		cmd.flags = SD_CMDRSP_NONE;
		cmd.arg = 512;
		if(sdcard_send_cmd(card, &cmd) == 0)
			return 0;
		else 
			card->blklen = 512;
		
		/* Select Card */
		cmd.idx = SD_CMD(7);
		cmd.flags = SD_CMDRSP_BUSY;
		cmd.arg = card->rca << 16;
		if(sdcard_send_cmd(card, &cmd) == 0)
			return 0;
		
		/* Send ACMD51 */
		cmd.idx = SD_CMD(51);
		cmd.flags = SD_CMDRSP_READ | SD_CMDRSP_DATA;
		cmd.arg = card->rca << 16;
		cmd.nblks = 1;
		cmd.data = dataBuffer;

		/* set mmcsd blk size to 8 (size of expected data from sd card */
		mmcsd_set_blklen(card->ctlr, 8);

		if(sdcard_send_acmd(card, &cmd) == 0)
			return 0;
	
	
		/* Get data from buffer */			
		if(sdcard_read_blk(card, 8, (unsigned int *)dataBuffer) == 0)
		{
			PRINT("NO Data\n\r");
			return 0;
		}


		/* Parse out the sd card data */
		card->scr[0] = (dataBuffer[3] << 24) | (dataBuffer[2] << 16) |
									 (dataBuffer[1] << 8) | (dataBuffer[0]);
		card->scr[1] = (dataBuffer[7] << 24) | (dataBuffer[6] << 16) |
									 (dataBuffer[5] << 8) | (dataBuffer[4]);

		/* store SD card version and bus width */
		card->sd_ver = SD_SCR_VER(card);
		card->buswidth = SD_SCR_BUSWIDTH(card);	

		/* set buswidth */
		if(sdcard_set_buswidth(card) == 0)
		{
			PRINT("Failed to set the bus width\n\r");
		}
		
		/* set bus frequency */
		if(sdcard_set_bus_freq(card) == 0)
		{
			PRINT("Failed to set the bus frequency\n\r");
		}
		
		/* clear all status flags in mmcsd controller */
		mmcsd_clear_status(card->ctlr, 0xFFFFFFFF);
	}
	else 
	{
		return 0;
	}

	return 1;
}

/* -- sdcard_print_info --------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void sdcard_print_info(struct cardInfo *card)
{
	unsigned int tspeed;
	unsigned int t_val[] = {0,10,12,13,15,20,25,30,35,
													40,45,50,55,60,65,70,80};
	unsigned int t_rate[] = {10,100,1000,10000};
	unsigned int tunit = 'k';

	/* card type */
	if(card->cardType == MMCSD_CARD_SD)
	{
		PRINT("\n\r*** SD Card Info Puke ***\n\r");

		/* Card Version */
		PRINT("  Version ");
		if(card->sd_ver == 0)
		{
			PRINT("1.0\n\r");
		}
		else if(card->sd_ver == 1)
		{
			PRINT("1.1\n\r");
		}
		else if(card->sd_ver == 2)
		{			
			PRINT("2.0\n\r");
		}
		else 
		{	
			PRINT("unknown");
		}

		/* Card Capacity */
		if(card->high_capacity)
		{
			PRINT("  High Capacity\n\r");
		}
		
		/* Card Size */
		PRINT("  card size                  %u KB\n\r", card->size);	
		PRINT("  max read data block len    %u Bytes\n\r", card->blklen);
		PRINT("  Number of Block            %u\n\r", card->nblks);

		/* transfer speed */
		tspeed = (t_rate[(card->transpeed & 0x7)] *
						t_val[(card->transpeed & 0x78) >> 3] );
		if(tspeed >= 10000)
		{
			tspeed = tspeed/1000;
			tunit = 'M';
		}
		PRINT("  max. data transfer rate    %u %cHz\n\r", tspeed,tunit);
		
		/* bus width support */
		PRINT("  Bus Widths Supported       ");
		if(card->buswidth == 5)
		{
			PRINT("1 and 4 bit\n\r");
		}
		else if(card->buswidth == 1)
		{
			PRINT("1 bit\n\r");
		}
		else if(card->buswidth == 4)
		{
			PRINT("4 bit\n\r");
		}
		else
		{
			PRINT("Un-identifiable\n\r");
		}
	}
	else
	{
		PRINT("MMC Card info puke\n\r - We got nothin\n\r");
	}
}

/* -- sdcard_read_blk ----------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int sdcard_read_blk(struct cardInfo *card, unsigned int blklen,
															unsigned int *data)
{
	unsigned int i;
	
	blklen = blklen/4;
	
	/* check to see if data is ready */
	while(!mmcsd_read_complete(card->ctlr));
	mmcsd_clear_status(card->ctlr,MMC_STAT_BRR);

	/* copy mmcsd data buffer into local buffer */
 	/* NOTE: blklen/4 to handle 32bit register access */
	for(i=0;i<blklen;i++)
	{
		*data++ = mmcsd_read_data(card->ctlr);
	}

	return 1;
}

/* -- sdcard_check_type --------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
unsigned int sdcard_check_type(struct cardInfo *card)
{
	struct sdCmd cmd;
	unsigned int status;

	cmd.idx = SD_CMD(55);
	cmd.flags = 0;
	cmd.arg = 0;

	status = sdcard_send_acmd(card, &cmd);
	return status;	
}

/* -- sdcard_send_acmd ------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int sdcard_send_acmd(struct cardInfo *card, struct sdCmd *cmd)
{
	unsigned int status = 0;
	struct sdCmd cmdapp;
	
	/* build preceeding app command */
	cmdapp.idx = SD_CMD(55);
	cmdapp.flags = 0;
	cmdapp.arg = card->rca << 16;
	
	if(sdcard_send_cmd(card, &cmdapp) == 0)
		return 0;

	status = sdcard_send_cmd(card, cmd);

	return status;	
}
		
	

/* -- sdcard_reset -------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
int sdcard_reset(struct cardInfo *card)
{
	struct sdCmd cmd;
	unsigned int status;
	
	/* build CMD0 */
	cmd.idx = SD_CMD(0);
	cmd.flags = SD_CMDRSP_NONE;
	cmd.arg = 0;

	/* send CMD0 */
	status = sdcard_send_cmd(card, &cmd);
	
	return status;
}

/* -- sdcard_send_cmd ----------------------------------------------------
 * return
 *		1 if successful
 *    0 if unsuccessful
 *
 * ---------------------------------------------------------------------*/
int sdcard_send_cmd(struct cardInfo *card, struct sdCmd *cmd)
{
	unsigned int cmdtype;
	unsigned int rsptype;
	unsigned int rw;
	unsigned int nblks = 0;
	unsigned int mmcCmd;
	unsigned int dataPresent;
	unsigned int status;

	/* Determine Command Type */
	switch(cmd->flags & SD_CMD_MASK)
	{
		case SD_CMDRSP_STOP:
			cmdtype = MMC_CMD_TYPE_SUSPEND;
			break;
		case SD_CMDRSP_FS:
			cmdtype = MMC_CMD_TYPE_RESUME;
			break;
		case SD_CMDRSP_ABORT:
			cmdtype = MMC_CMD_TYPE_ABORT;
			break;
		default:
			cmdtype = MMC_CMD_TYPE_OTHER;
	}

	/* Determine Response Type */
	switch(cmd->flags & SD_RSP_MASK)
	{
		case SD_CMDRSP_NONE:
			rsptype = MMC_RSP_TYPE_NONE;
			break;
		case SD_CMDRSP_BUSY:
			rsptype = MMC_RSP_TYPE_48WBUSY;
			break;
		case SD_CMDRSP_136BIT:
			rsptype = MMC_RSP_TYPE_136BITS;
			break;
		default:
			rsptype = MMC_RSP_TYPE_48BITS;
	}

	/* Determine Read/Write */
	rw = (cmd->flags & SD_CMDRSP_READ) ? MMC_CMD_RD : MMC_CMD_WR;

	/* Determine if there is command data */
	dataPresent = (cmd->flags & SD_CMDRSP_DATA) ? 1 : 0;
	if(dataPresent)
	{
		nblks = cmd->nblks;
		/* clear transfer complete status flag */
		mmcsd_clear_status(card->ctlr, MMC_STAT_TC);
		/* set data transfer timeout */
		mmcsd_data_timeout(card->ctlr, MMC_DATA_TIMEOUT(27));
	}

	/* build command to load into mmc controller */
	mmcCmd = MMC_CMD(cmd->idx,cmdtype,rsptype,rw);

	/* Send command to sd card */
	mmcsd_send_cmd(card->ctlr, mmcCmd, cmd->arg, 
										(void *)dataPresent, nblks, 0);

	/* See if command went through */
	status = mmcsd_get_cmd_status(card->ctlr);


	/* Get command response */
	if(status == 1)
	{
		mmcsd_get_rsp(card->ctlr, cmd->rsp);
	}

	return status;
}

/* -- sdcard_en_stat_flags -----------------------------------------------
 *
 * NOTE: having trouble getting TC, BRR, and BWR bits to set!
 * ---------------------------------------------------------------------*/
void sdcard_en_stat_flags(struct cardInfo *card)
{
	unsigned int flags;
	/* build flags register to enable int flags */
	flags = MMC_ISE_CC_SIGEN | MMC_ISE_TC_SIGEN | 
					MMC_ISE_CTO_SIGEN | MMC_ISE_DTO_SIGEN |
					MMC_ISE_BRR_SIGEN | MMC_ISE_BWR_SIGEN |
					MMC_ISE_CERR_SIGEN;


	/* set flags in mmcsd controller */
	mmcsd_en_status_flags(card->ctlr, flags);
}
