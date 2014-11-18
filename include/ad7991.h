/* ***********************************************************************
 * file: 		ad7991.h
 * author:	Kyle Litzer
 * created on:	9/20/2014
 * 
 * Analog Devices analog to digital converter (ADC) 7991 driver header file
 * ***********************************************************************
 */
 
#ifndef __AD7991_H
#define __AD7991_H
 
/* Define ADC Manufacturer I2C Address */
#define AD7991_0_ADDRESS  (0x28) /* Given as 0x28 for AD7991-0 */
#define AD7991_1_ADDRESS 	(0x29) /* Given as 0x29 for AD7991-1 */

/* Define Number of Bits Available for an AD7991 Conversion */
#define AD7991_BITSNUMBER	 12 

/* Define Number of Bits of AD7991 Register Value */
#define AD7991_REGISTERLENGTH	 1 
#define AD7991_REGISTERRESULTLENGTH    2

/* Define Capable Port Frequencies of AD7991 */
#define AD7991_100KHZ	 (0) /* 100 kHz */
#define AD7991_400KHZ	 (1) /* 400 kHz */

/* Configuration Register Definition (registerValue variable) */
#define AD7991_CHANNEL(ch)     	((1 << ch) << 4)
#define AD7991_REF_SEL					(1 << 3)
#define AD7991_FLTR							(1 << 2)
#define AD7991_BIT_TRIAL_DELAY	(1 << 1)
#define AD7991_SAMPLE_DELAY			(1 << 0)

struct ad7991_info
{
	char address;
	char config;
	unsigned int ch_data[4];
	struct i2c *i2c_port;
};


/* Function Prototypes */
void ad7991_init(struct ad7991_info *adc);
void ad7991_read(struct ad7991_info *adc);
float ad7991_conv_to_volts(short raw_sample, float v_ref);

#endif /* __AD7991_H__ */
