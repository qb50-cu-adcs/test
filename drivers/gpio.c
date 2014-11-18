/* file: 		gpio.c
 * author: 	jacob cook
 * created on: 10/23/2014
 */

#include <math.h>

#include "dbg.h"
#include "types.h"
#include "io.h"
#include "gpio.h"

/* -- gpio_set_oe --------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void gpio_set_oe(struct gpioCtlr *gpio_bank, unsigned int pins)
{
	and_regl(!pins, &gpio_bank->oe);
}

/* -- gpio_clr_oe ------ -------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void gpio_clr_oe(struct gpioCtlr *gpio_bank, unsigned int pins)
{
	or_regl(pins, &gpio_bank->oe);
}

/* -- gpio_pin_en_output -------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void gpio_pin_en_output(struct gpio_pin *pin)
{
	gpio_set_oe(pin->gpio_bank, GPIO_BIT(pin->bit));
}

/* -- gpio_pin_dis_output -------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void gpio_pin_dis_output(struct gpio_pin *pin)
{
	gpio_clr_oe(pin->gpio_bank, GPIO_BIT(pin->bit));
}	

/* -- gpio_set_dataout ---------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void gpio_set_dataout(struct gpioCtlr *gpio_bank, unsigned int pins)
{
	or_regl(pins, &gpio_bank->setdataout);
}

/* -- gpio_clr_dataout ---------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void gpio_clr_dataout(struct gpioCtlr *gpio_bank, unsigned int pins)
{
	or_regl(pins, &gpio_bank->cleardataout);
}

/* -- gpio_set_pin -------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void gpio_set_pin(struct gpio_pin *pin)
{
	gpio_set_dataout(pin->gpio_bank, GPIO_BIT(pin->bit));
}

/* -- gpio_clr_pin -------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void gpio_clr_pin(struct gpio_pin *pin)
{
	gpio_clr_dataout(pin->gpio_bank, GPIO_BIT(pin->bit));
}

