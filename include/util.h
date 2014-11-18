/* ***********************************************************************
 * file: 		util.h
 * author:	jacob cook
 * created on:	7/10/2014
 * ***********************************************************************
 */

#ifndef __UTIL_H
#define __UTIL_H

void util_setup(void);
void delay_sec(unsigned int sec);
void delay_ms(unsigned int ms);
void stopwatch_start(void);
unsigned int stopwatch_stop(void);

#endif
