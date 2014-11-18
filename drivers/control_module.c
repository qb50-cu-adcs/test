/* ***********************************************************************
 * file: 		control_module.c
 * author: 	jacob cook
 * created on: 3/24/2014
 *
 * This file contains lowlevel routines to set all the pin mux. 
 *
 * reference files:
 *  u-boot/board/ti/am335x/mux.c
 * 	u-boot/arch/arm/cpu/armv7/am33xx/mux.c
 * 	u-boot/arch/arm/include/asm/arch-am33xx/mux.h
 *  u-boot/arch/arm/include/asm/arch-am33xx/mux_am33xx.h
 *
 * ***********************************************************************
 */

#include "types.h"
#include "io.h"
#include "control_module.h"

/* static function prototypes */
static void configure_module_pin_mux(struct module_pin_mux *mod_pin_mux);

/* Module pin mux struct declarations */
static struct module_pin_mux uart0_pin_mux[] = {
	{CONF_UART0_RXD, (MODE(0) | PULLUP_EN | RXACTIVE)},
	{CONF_UART0_TXD, (MODE(0) | PULLUDEN)},
	{-1}};

static struct module_pin_mux i2c0_pin_mux[] = {
	{CONF_I2C0_SDA, (MODE(0) | RXACTIVE | PULLUDDIS | PULLUP_EN | SLEWCTRL)},
	{CONF_I2C0_SCL, (MODE(0) | RXACTIVE | PULLUDDIS | PULLUP_EN | SLEWCTRL)},
	{-1}};

static struct module_pin_mux i2c1_pin_mux[] = {
	{CONF_I2C1_SDA, (MODE(2) | RXACTIVE | PULLUDDIS | PULLUP_EN | SLEWCTRL)},
	{CONF_I2C1_SCL, (MODE(2) | RXACTIVE | PULLUDDIS | PULLUP_EN | SLEWCTRL)},
	{-1}};

static struct module_pin_mux i2c2_pin_mux[] = {
	{CONF_I2C2_SDA, (MODE(3) | RXACTIVE | PULLUDDIS | PULLUP_EN | SLEWCTRL)},
	{CONF_I2C2_SCL, (MODE(3) | RXACTIVE | PULLUDDIS | PULLUP_EN | SLEWCTRL)},
	{-1}};

static struct module_pin_mux mmc0_pin_mux[] = {
	{CONF_MMC0_DAT3, (MODE(0) | RXACTIVE | PULLUDEN | PULLUP_EN)},
	{CONF_MMC0_DAT2, (MODE(0) | RXACTIVE | PULLUDEN | PULLUP_EN)},
	{CONF_MMC0_DAT1, (MODE(0) | RXACTIVE | PULLUDEN | PULLUP_EN)},
	{CONF_MMC0_DAT0, (MODE(0) | RXACTIVE | PULLUDEN | PULLUP_EN)},
	{CONF_MMC0_CLK, (MODE(0) | RXACTIVE | PULLUDEN | PULLUP_EN)},
	{CONF_MMC0_CMD, (MODE(0) | RXACTIVE | PULLUDEN | PULLUP_EN)},
	{CONF_MMC0_SDCD, (MODE(5) | RXACTIVE | PULLUDEN | PULLUP_EN)},
	{-1}};	
	

/* For Torque Rods, 3 PWM timers and 3 pol GPIOs */
static struct module_pin_mux torq_rod_pin_mux[] = {
	{CONF_TMR4_OUT, (MODE(2) | PULLUDEN )},
	{CONF_TMR5_OUT, (MODE(2) | PULLUDEN )},
	{CONF_TMR7_OUT, (MODE(2) | PULLUDEN )},
	{CONF_GPIO1_6, (MODE(7) | PULLUDDIS)},
	{CONF_GPIO1_2, (MODE(7) | PULLUDDIS)},
	{CONF_GPIO1_3, (MODE(7) | PULLUDDIS)},
	{-1}};

static struct module_pin_mux tmr4_pin_mux[] = {
	{CONF_TMR4_OUT, (MODE(2) | PULLUDEN )},
	{-1}};

static struct module_pin_mux tmr5_pin_mux[] = {
	{CONF_TMR5_OUT, (MODE(2) | PULLUDEN )},
	{-1}};
static struct module_pin_mux tmr7_pin_mux[] = {
	{CONF_TMR7_OUT, (MODE(2) | PULLUDEN )},
	{-1}};


static struct module_pin_mux led_pin_mux[] = {
	{CONF_GPMC_A5, (MODE(7) | PULLUP_EN)}, 
	{CONF_GPMC_A6, (MODE(7) | PULLUDDIS)},
	{CONF_GPMC_A7, (MODE(7) | PULLUDDIS)},
	{CONF_GPMC_A8, (MODE(7) | PULLUDDIS)},
	{-1}};


/* -- enable_uart0_pin_mux ----------------------------------------------
 *
 * enables uart0 mux pins for use 
 * --------------------------------------------------------------------*/
void enable_uart0_pin_mux(void)
{
	configure_module_pin_mux(uart0_pin_mux);
}

/* -- enable_i2c0_pin_mux -----------------------------------------------
 *
 * enables i2c0  mux pins for use 
 * --------------------------------------------------------------------*/
void enable_i2c0_pin_mux(void)
{
	configure_module_pin_mux(i2c0_pin_mux);
}

/* -- enable_i2c1_pin_mux -----------------------------------------------
 *
 * enables i2c1  mux pins for use 
 * --------------------------------------------------------------------*/
void enable_i2c1_pin_mux(void)
{
	configure_module_pin_mux(i2c1_pin_mux);
}

/* -- enable_i2c2_pin_mux -----------------------------------------------
 *
 * enables i2c2  mux pins for use 
 * --------------------------------------------------------------------*/
void enable_i2c2_pin_mux(void)
{
	configure_module_pin_mux(i2c2_pin_mux);
}

/* -- enable_mmc0_pin_mux -----------------------------------------------
 *
 * enables mmc0  mux pins for use 
 * --------------------------------------------------------------------*/
void enable_mmc0_pin_mux(void)
{
	configure_module_pin_mux(mmc0_pin_mux);
}

/* -- enable_torq_rod_pin_mux -------------------------------------------
 *
 * enables torque rod output pins
 * --------------------------------------------------------------------*/
void enable_torq_rod_pin_mux(void)
{
	configure_module_pin_mux(torq_rod_pin_mux);
}

/* -- enable_tmr4_pin_mux -----------------------------------------------
 *
 * enables tmr4 output pin
 * --------------------------------------------------------------------*/
void enable_tmr4_pin_mux(void)
{
	configure_module_pin_mux(tmr4_pin_mux);
}

/* -- enable_tmr5_pin_mux -----------------------------------------------
 *
 * enables tmr5 output pin
 * --------------------------------------------------------------------*/
void enable_tmr5_pin_mux(void)
{
	configure_module_pin_mux(tmr5_pin_mux);
}

/* -- enable_tmr7_pin_mux -----------------------------------------------
 *
 * enables tmr7 output pin
 * --------------------------------------------------------------------*/
void enable_tmr7_pin_mux(void)
{
	configure_module_pin_mux(tmr7_pin_mux);
}


/* -- enable_led_pin_mux ------------------------------------------------
 *
 * enable led mux pins for use
 * --------------------------------------------------------------------*/
void enable_led_pin_mux(void)
{
	configure_module_pin_mux(led_pin_mux);
}


/* -- config_module_pin_mux ---------------------------------------------
 * 
 * configures the pin mux for the module
 * --------------------------------------------------------------------*/
static void configure_module_pin_mux(struct module_pin_mux *mod_pin_mux)
{
	int i;
	
	if(!mod_pin_mux)
		return;

	/* loop through the passed structure and set mux reg values */
	for(i=0; mod_pin_mux[i].mux_reg != -1; i++)
	{
		MUX_CFG(mod_pin_mux[i].val, mod_pin_mux[i].mux_reg);
	}
}



