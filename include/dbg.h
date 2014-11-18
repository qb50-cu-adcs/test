/* ***********************************************************************
 * file: 		dbg.h
 * author:	jacob cook
 * created on:	7/3/2014
 *
 * This file contains everything thats needed for the Debug serial port 
 * including a couple macros for ease of access.
 * ***********************************************************************/

#ifndef __DBG_H
#define __DBG_H

#include <sys/stat.h>
#include "uart.h"

#define DEBUG

#ifdef DEBUG
/* debug string */
char dbgstr[50];

/* macro for using debug port */
#define PRINT(fmt,args...) \
		sprintf(dbgstr, fmt, ##args); \
		dbg_puts(dbgstr)

/* macro for initializing debug port */
#define INIT_DBG_PORT()	enable_uart0_pin_mux(); \
												dbg_setup(); 

#else

/* replace debug function with white space */
#define PRINT(fmt,arg...) 
#define INIT_DBG_PORT()

#endif /* DEBUG */

/* dbg prototypes */
void dbg_setup(void);
void dbg_puts(char *str);
void dbg_putc(char c);
char dbg_getc();

/* STDIO low level functions */
int _close(int file);
int _fstat(int file, struct stat *st);
int _isatty(int file);
int _lseek(int file, int ptr, int dir);
int _open(const char *name, int flags, int mode);
int _read(int file, char *ptr, int len);
caddr_t _sbrk(ptrdiff_t incr);
int _write(int file, char *ptr, int len);

#include <stdio.h>

#endif
