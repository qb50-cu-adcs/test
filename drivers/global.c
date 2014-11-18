/* ***********************************************************************
 * file: 		global.c
 * author:	Kyle Litzer, Jacob Cook
 * created on:	10/23/2014
 *  
 * Enables global use structures for I2C and sensor drivers.
 * ***********************************************************************
 */

#include "mmcsd.h"
#include "sd_card.h"
#include "i2c.h"
#include "timer.h"
#include "pwm.h"
#include "gpio.h"
#include "torque_rod.h"
#include "ad7991.h"
#include "magnetometer.h"
/* #include mag, gyro, etc. */
#include "global.h" 

/* SD Card controllers */
struct mmcsdCtlr *mmcsd0 = (struct mmcsdCtlr *)MMC0_BASE;
struct cardInfo sd0;

/* I2C Global Structs */
struct i2c *i2c0 = (struct i2c *)I2C0_BASE;
struct i2c *i2c1 = (struct i2c *)I2C1_BASE;
struct i2c *i2c2 = (struct i2c *)I2C2_BASE;

/* GPIO bank controllers */
struct gpioCtlr *gpio_bank1 = (struct gpioCtlr *)GPIO1_BASE;

/* AD7991 Global Structs */
struct ad7991_info adc1;
struct ad7991_info adc2;
struct ad7991_info adc3;

/* magnatometer structures */
struct magInfo mag1;
struct magInfo mag2;
struct magInfo mag3;

/* declare 3 torque rod structures */
struct torq_rod x_rod;
struct torq_rod y_rod;
struct torq_rod z_rod;

/* declare 3 timers for torque rods */
struct tmrCtlr *tmr4 = (struct tmrCtlr *)TIMER4_BASE;
struct tmrCtlr *tmr5 = (struct tmrCtlr *)TIMER5_BASE;
struct tmrCtlr *tmr7 = (struct tmrCtlr *)TIMER7_BASE;

/* three PWM structures for torque rods*/
struct pwmInfo pwm4;
struct pwmInfo pwm5;
struct pwmInfo pwm7;

/* three gpio outputs to handle polarity of torque rods */
struct gpio_pin pol_x;
struct gpio_pin pol_y;
struct gpio_pin pol_z;

