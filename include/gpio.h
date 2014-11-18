/* ***********************************************************************
 * file: 		gpio.h
 * author:	jacob cook
 * created on:	10/23/2014
 * ***********************************************************************
 */

#ifndef __GPIO_H
#define __GPIO_H

#define GPIO0_BASE  (0x44E07000)
#define GPIO1_BASE	(0x4804C000)
#define GPIO2_BASE	(0x481AC000)
#define GPIO3_BASE	(0x481AE000)

#define GPIO_BIT(x) (0x1 << x)
 
struct gpioCtlr
{
	unsigned int revision; 				/* 0x00 */
	unsigned int res1[3];					/* 0x04 - 0x0C */
	unsigned int sysconfig;				/* 0x10 */
	unsigned int res2[3];					/* 0x14 - 0x1C */
	unsigned int eoi;							/* 0x20 */
	unsigned int irqstatus_raw0; 	/* 0x24 */	
	unsigned int irqstatus_raw1;	/* 0x28 */
	unsigned int irqstatus_0;			/* 0x2C */
	unsigned int irqstatus_1;			/* 0x30 */
	unsigned int irqstatus_set0;	/* 0x34 */
	unsigned int irqstatus_set1;	/* 0x38 */
	unsigned int irqstatus_clear0;	/* 0x3C */
	unsigned int irqstatus_clear1;	/* 0x40 */
	unsigned int irqwaken_0;			/* 0x44 */
	unsigned int irqwaken_1;			/* 0x48 */
	unsigned int res3[50];				/* 0x4C - 0x110 */
	unsigned int sysstatus;				/* 0x114 */
	unsigned int res4[6];					/* 0x118 - 0x12C */
	unsigned int ctrl;						/* 0x130 */
	unsigned int oe;							/* 0x134 */
	unsigned int datain;					/* 0x138 */
	unsigned int dataout;					/* 0x13C */
	unsigned int leveldetect0;		/* 0x140 */
	unsigned int leveldetect1;		/* 0x144 */
	unsigned int risingdetect;		/* 0x148 */
	unsigned int fallingdetect;		/* 0x14C */
	unsigned int debounceenable;	/* 0x150 */
	unsigned int debounceingtime;	/* 0x154 */
	unsigned int res5[14];				/* 0x158 - 0x18C */
	unsigned int cleardataout;		/* 0x190 */
	unsigned int setdataout;			/* 0x194 */
};

struct gpio_pin
{
	unsigned int bit;
	struct gpioCtlr *gpio_bank;
};

/* function prototypes */
void gpio_set_oe(struct gpioCtlr *gpio, unsigned int pins);
void gpio_clr_oe(struct gpioCtlr *gpio, unsigned int pins);
void gpio_pin_en_output(struct gpio_pin *pin);
void gpio_pin_dis_output(struct gpio_pin *pin);
void gpio_set_dataout(struct gpioCtlr *gpio_bank, unsigned int pins);
void gpio_clr_dataout(struct gpioCtlr *gpio_bank, unsigned int pins);
void gpio_set_pin(struct gpio_pin *pin);
void gpio_clr_pin(struct gpio_pin *pin);
		
#endif
