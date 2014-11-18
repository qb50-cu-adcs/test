/* ***********************************************************************
 * file: 		global.h
 * author:	Kyle Litzer, Jacob Cook
 * created on:	10/23/2014
 * 
 * Enables global use structures for I2C and sensor drivers.
 ************************************************************************
 */

#ifndef __GLOBAL_H
#define __GLOBAL_H

/* SD Card controllers */
extern struct mmcsdCtlr *mmcsd0;
extern struct cardInfo sd0;

/* I2C controllers */
extern struct i2c *i2c0;
extern struct i2c *i2c1;
extern struct i2c *i2c2;

/* GPIO bank controllers */
struct gpioCtlr *gpio_bank1;

/* AD7991 Global Structs */
extern struct ad7991_info adc1;
extern struct ad7991_info adc2;
extern struct ad7991_info adc3;

/* Magnetometer */
extern struct magInfo mag1;
extern struct magInfo mag2;
extern struct magInfo mag3;

/* Torque rods */
extern struct torq_rod x_rod;
extern struct torq_rod y_rod;
extern struct torq_rod z_rod;

/* declare 3 timers for torque rods */
extern struct tmrCtlr *tmr4;
extern struct tmrCtlr *tmr5;
extern struct tmrCtlr *tmr7;

/* three PWM structures for torque rods*/
extern struct pwmInfo pwm4;
extern struct pwmInfo pwm5;
extern struct pwmInfo pwm7;

/* three gpio outputs to handle polarity of torque rods */
extern struct gpio_pin pol_x;
extern struct gpio_pin pol_y;
extern struct gpio_pin pol_z;

#endif /* __GLOBAL_H__ */
