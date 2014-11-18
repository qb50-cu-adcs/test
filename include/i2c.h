/* ***********************************************************************
 * file: 		i2c.h
 * author:	jacob cook
 * created on:	6/3/2014
 * 
 * I2C header file
 * ***********************************************************************
 */

#ifndef __i2c_H
#define __i2c_H

#define I2C0_BASE (0x44E0B000)
#define I2C1_BASE (0x4802A000)
#define I2C2_BASE (0x4819C000)

/* defines for choosing address length */
#define I2C_ADDR_7BIT  0
#define I2C_ADDR_10BIT 1

/* Define restart flags */
#define I2C_RESTART		1
#define I2C_STOP			0

struct i2c 
{
	unsigned int revnb_lo;				/* 0x00 */
	unsigned int revnb_hi;				/* 0x04 */
	unsigned int res1[2];					/* 0x08 - 0x0C */
	unsigned int sysc;						/* 0x10 */
	unsigned int res2[4];					/* 0x14 - 0x20 */
	unsigned int irqstatus_raw;		/* 0x24 */
	unsigned int irqstatus;				/* 0x28 */
	unsigned int irqenable_set;		/* 0x2C */
	unsigned int irqenable_clr;		/* 0x30 */
	unsigned int we;							/* 0x34 */
	unsigned int dmarxenable_set;	/* 0x38 */
	unsigned int dmatxenable_set;	/* 0x3C */
	unsigned int dmarxenable_clr;	/* 0x40 */
	unsigned int dmatxenable_clr;	/* 0x44 */
	unsigned int dmarxwake_en;		/* 0x48 */
	unsigned int dmatxwake_en;		/* 0x4C */
	unsigned int res3[16];				/* 0x50 - 0x8C */	
	unsigned int syss;						/* 0x90 */
	unsigned int buf;							/* 0x94 */
	unsigned int cnt;							/* 0x98 */
	unsigned int data; 					/* 0x9C */
	unsigned int res4;						/* 0xA0 */
	unsigned int con;							/* 0xA4 */
	unsigned int oa;							/* 0xA8 */
	unsigned int sa;							/* 0xAC */
	unsigned int psc;							/* 0xB0 */
	unsigned int scll;						/* 0xB4 */
	unsigned int sclh;						/* 0xB8 */
	unsigned int systest;					/* 0xBC */
	unsigned int bufstat;					/* 0xC0 */
	unsigned int oa1;							/* 0xC4 */
	unsigned int oa2;							/* 0xC8 */
	unsigned int oa3;							/* 0xCC */
	unsigned int actoa;						/* 0xD0 */
	unsigned int sblock;					/* 0xD4 */
  
};

/* function prototypes */
int i2c_init(struct i2c *i2c_port, unsigned int scl_freq);
void i2c_slave(struct i2c *i2c_port, int slave_addr, int xsa);
int i2c_tx(struct i2c *i2c_port, char *data, int len, int restart);
int i2c_rx(struct i2c *i2c_port, char *data, int len, int restart);

#endif
