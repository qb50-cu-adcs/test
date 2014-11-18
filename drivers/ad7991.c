/* ***********************************************************************
 * file: 	   ACD7991.c
 * author: 	   Kyle Litzer
 * created on: 9/20/2014
 *
 * This file contains routines to interface with the AD7991 Analog
 * Devices analog to digital converter (ADC) used with the OSRAM (SFH2430) 
 * sun sensors. 
 * The i2c driver is utilized here to perform many of the low level i2c
 * operations required to communicate and draw data from the sensors. Each
 * of the functions contained within this driver are documented and
 * described below as they are declared.
 * ***********************************************************************
 */
 
/* Include Files */
#include "types.h" // not sure what this is for yet
#include "i2c.h" // I2C driver for low level operations

/* Include Files for Debugging */
#include "dbg.h" // For debug routines
#include "io.h" /* Standard C library */

/* Header for this .c file */
#include "ad7991.h"

/* static function prototypes */
void ad7991_save_value(struct ad7991_info *adc, short chn);

/* Initialize I2C peripheral by writing data into the 
 * Configuration Register
 *
 * 		@param deviceVersion should be one 
 *               of two addresses in header
 *			0 - refers to AD7991-0
 *			1 - refers to AD7991-1 
 *
 *		@param registerValue - Configuration 
 *               Register Structure is defined
 *		 under "Internal Register Structure" 
 *               on page 20 of the AD7991 data sheet
 *
 * http://www.analog.com/static/imported-files/data_sheets/AD7991_7995_7999.pdf
 *
 */
void ad7991_init(struct ad7991_info *adc)
{

	/* Setting the specified ADC address as slave */
	i2c_slave(adc->i2c_port, adc->address, I2C_ADDR_7BIT);

	/* Writing to the ADC's 8 bit Configuration Register */
	i2c_tx(adc->i2c_port, &adc->config, 1, I2C_STOP); 
}

/* Reads the High byte and the Low byte of the Conversion Register
 *
 *		@param convValue - It is used to store the conversion value. 
 *
 *		@param channel - Stores the channel number for the current 
 *                               conversion.
 */
void ad7991_read(struct ad7991_info *adc)
{
	unsigned int i;
	char rxData[8];
	int len;
	short ch;

	/* set rxData to zeros */
	for(i=0;i<8;i++)
		rxData[i] = 0;

	len = 0;
	for(i=0;i<4;i++)
	{
		len += ((adc->config >> (i+4)) & 0x01)*2;
	}
	
	/* Setting the specified ADC address as slave */
	i2c_slave(adc->i2c_port, adc->address, I2C_ADDR_7BIT);

	/* Reading from the ADC's 16 bit Conversion Register */
	i2c_rx(adc->i2c_port,rxData,len, I2C_STOP);

	/* save data to ch_data array */
	for(i=0;i<len/2;i++)
	{
		ch = (rxData[2*i] << 8) | rxData[2*i+1];
		ad7991_save_value(adc, ch);
	}
}

/* -- ad7991_save_value --------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void ad7991_save_value(struct ad7991_info *adc, short chn)
{
	int ch;

	/* save data to specific channel */
	ch = (chn & 0x3000) >> 12;
	if(ch < 4)
	{
		adc->ch_data[ch] = (chn & 0x0FFF) >> (12 - AD7991_BITSNUMBER);
	}
}


/* Convert Raw Data sample (rxData?) to volts
 *
 * 		@param rawSample - The data sample.
 * 		@param vRef - The value of the voltage reference used by the 
 *                            device.
 *
 * 		@return voltage - The result of the conversion expressed as 
 *                                volts.
 */
float ad7991_conv_to_volts(short raw_sample, float v_ref)
{
	/* Variable Definitions & Initializations */
	float voltage = 0;
	
	voltage = v_ref * (float)raw_sample / (1 << AD7991_BITSNUMBER);
	
	return voltage;
}
