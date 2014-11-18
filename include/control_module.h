/* ***********************************************************************
 * file: 		control_module.h
 * author:	jacob cook
 * created on:	3/24/2014
 * 
 * Refer to the Data sheet Ball Characteristics Table for multiplexed
 * IO pin asignments, and TRM Control module registers for register 
 * address.
 * ***********************************************************************
 */

#ifndef __CONTROL_MODULE_H
#define __CONTROL_MODULE_H

#define MUX_CFG(v,a) __raw_writel(v,a)

struct module_pin_mux 
{
	unsigned int mux_reg;
	unsigned int val;
};

/* PAD Control Fields */
#define SLEWCTRL	(0x1 << 6)
#define RXACTIVE	(0x1 << 5)
#define PULLDOWN_EN	(0x0 << 4) /* Pull Down Selection */
#define PULLUP_EN	(0x1 << 4) /* Pull Up Selection */
#define PULLUDEN	(0x0 << 3) /* Pull up/down enabled */
#define PULLUDDIS	(0x1 << 3) /* Pull up/down disabled */
#define MODE(val)	val	/* used for Readability */

/* Control module base address */
#define CONTROL_MODULE (0x44E10000)

/* Control module registers */
/* UART0 */
#define CONF_UART0_RXD	(CONTROL_MODULE + 0x970)
#define CONF_UART0_TXD	(CONTROL_MODULE + 0x974)
/* I2C0 */
#define CONF_I2C0_SDA		(CONTROL_MODULE + 0x988)
#define CONF_I2C0_SCL		(CONTROL_MODULE + 0x98C)
/* I2C1 */
#define CONF_SPI0_D1		(CONTROL_MODULE + 0x958) /* SDA */
#define CONF_SPI0_CS0		(CONTROL_MODULE + 0x95C) /* SCL */
#define CONF_I2C1_SDA CONF_SPI0_CS0
#define CONF_I2C1_SCL CONF_SPI0_D1
/* I2C2 */
#define CONF_UART1_CTSN		(CONTROL_MODULE + 0x978)
#define CONF_UART1_RTSN		(CONTROL_MODULE + 0x97C)
#define CONF_I2C2_SDA CONF_UART1_CTSN
#define CONF_I2C2_SCL CONF_UART1_RTSN
/* MMC0 */
#define CONF_MMC0_DAT3	(CONTROL_MODULE + 0x8F0)
#define CONF_MMC0_DAT2	(CONTROL_MODULE + 0x8F4)
#define CONF_MMC0_DAT1	(CONTROL_MODULE + 0x8F8)
#define CONF_MMC0_DAT0	(CONTROL_MODULE + 0x8FC)
#define CONF_MMC0_CLK		(CONTROL_MODULE + 0x900)
#define CONF_MMC0_CMD		(CONTROL_MODULE + 0x904)
#define CONF_SPI0_CS1		(CONTROL_MODULE + 0x960)
#define CONF_MMC0_SDCD	CONF_SPI0_CS1	
/* TIMER 4 */
#define CONF_ADVn_ALE		(CONTROL_MODULE + 0x890)
#define CONF_TMR4_OUT		(CONF_ADVn_ALE)
/* TIMER 5 */
#define CONF_GPMC_BEN0_CLE (CONTROL_MODULE + 0x89C)
#define CONF_TMR5_OUT		(CONF_GPMC_BEN0_CLE)
/* TIMER 7 */
#define CONF_GPMC_OEN_REN		(CONTROL_MODULE + 0x894)
#define CONF_TMR7_OUT		(CONF_GPMC_OEN_REN)
/* GPIO 1-6 */
#define CONF_GPMC_AD6		(CONTROL_MODULE + 0x818)
#define CONF_GPIO1_6		(CONF_GPMC_AD6)
/* GPIO 1-2 */
#define CONF_GPMC_AD2		(CONTROL_MODULE + 0x808)
#define CONF_GPIO1_2		(CONF_GPMC_AD2)
/* GPIO 1-3 */
#define CONF_GPMC_AD3		(CONTROL_MODULE + 0x80C)
#define CONF_GPIO1_3		(CONF_GPMC_AD3)
/* LEDs */
#define CONF_GPMC_A5		(CONTROL_MODULE + 0x854)
#define CONF_GPMC_A6		(CONTROL_MODULE + 0x858)
#define CONF_GPMC_A7		(CONTROL_MODULE + 0x85C)
#define CONF_GPMC_A8		(CONTROL_MODULE + 0x860)

/* function prototypes */
void enable_uart0_pin_mux(void);
void enable_i2c0_pin_mux(void);
void enable_i2c1_pin_mux(void);
void enable_i2c2_pin_mux(void);
void enable_mmc0_pin_mux(void);
void enable_torq_rod_pin_mux(void);
void enable_tmr4_pin_mux(void);
void enable_tmr5_pin_mux(void);
void enable_tmr7_pin_mux(void);
void enable_led_pin_mux(void);

#endif
