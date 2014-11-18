/*-----------------------------------------------------------------------*/
/* Stellaris USB module                                                  */
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/

/*
 * This file was modified from a sample available from the FatFs
 * web site. It was modified to work with the SitaraWare USB stack.
 */

#include "dbg.h"
#include "global.h"
#include "sd_card.h"
#include "diskio.h"
#include "ff.h"



typedef struct _fatDevice
{
    /* Pointer to underlying device/controller */
    void *dev;
    
    /* File system pointer */
    FATFS *fs;

	/* state */
	unsigned int initDone;

}fatDevice;


#define DRIVE_NUM_MMCSD     0
#define DRIVE_NUM_MAX      2


fatDevice fat_device;


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS
disk_initialize(
    BYTE bValue)                /* Physical drive number (0) */
{
	unsigned int status;
   
    if (fat_device.initDone != 1)
    {
        struct cardInfo *card = &sd0;
        
        /* SD Card init */
        status = sdcard_init(card);

        if (status == 0)
        {
            PRINT("\r\nCard Init Failed \r\n");
            return STA_NOINIT;
        }
        else
        {
            if (card->cardType == MMCSD_CARD_SD)
            {
   						/* PRINT("\n\rSD CARD\n\r");	  */
            }

            else if (card->cardType == MMCSD_CARD_MMC)
            {
              /* PRINT("\r\nMMC Card\n\r"); */
            }

				}
		fat_device.initDone = 1;
    }
        
    return 0;
}



/*-----------------------------------------------------------------------*/
/* Returns the current status of a drive                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE drv)                   /* Physical drive number (0) */
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* This function reads sector(s) from the disk drive                     */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE drv,               /* Physical drive number (0) */
    BYTE* buff,             /* Pointer to the data buffer to store read data */
    DWORD sector,           /* Physical drive nmuber (0) */
    BYTE count)             /* Sector count (1..255) */
{
	struct cardInfo *card = &sd0;

  /* READ BLOCK */
	if (sdcard_read(card, buff, sector, count) == 1)
	{
 		return RES_OK;
	}

  return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* This function writes sector(s) to the disk drive                     */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
    BYTE ucDrive,           /* Physical drive number (0) */
    const BYTE* buff,       /* Pointer to the data to be written */
    DWORD sector,           /* Start sector number (LBA) */
    BYTE count)             /* Sector count (1..255) */
{
	struct cardInfo *card = &sd0;
	
	/* WRITE BLOCK */
	if(sdcard_write(card,(BYTE*) buff, sector, count) == 1)
	{
		return RES_OK;
	}

  return RES_ERROR;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE drv,               /* Physical drive number (0) */
    BYTE ctrl,              /* Control code */
    void *buff)             /* Buffer to send/receive control data */
{
	return RES_OK;
}

/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */

DWORD get_fattime (void)
{
    return    ((2007UL-1980) << 25) // Year = 2007
            | (6UL << 21)           // Month = June
            | (5UL << 16)           // Day = 5
            | (11U << 11)           // Hour = 11
            | (38U << 5)            // Min = 38
            | (0U >> 1)             // Sec = 0
            ;
}
