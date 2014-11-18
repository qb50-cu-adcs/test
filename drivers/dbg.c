/* ***********************************************************************
 * file: 		dbg.c
 * author: 	jacob cook
 * created on: 7/3/2014
 * **********************************************************************/
#include "dbg.h"

static struct uartInfo dbgprt;
static struct uartCtlr *uart0 = (struct uartCtlr *)UART0_BASE;

/* -- dbg_setup ----------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void dbg_setup(void)
{
		dbgprt.baudrate = 115200; 
  	dbgprt.datalen = UART_8BIT; 
		dbgprt.parity = UART_PARITY_NONE; 
		dbgprt.stopbit = UART_STOP_1BIT; 
		dbgprt.mode = UART_MODE_16X; 
  	dbgprt.ctlr = uart0; 
		uart_setup(&dbgprt);
}	

/* -- dbg_puts -----------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void dbg_puts(char *str)
{
	uart_puts(&dbgprt, str, -1);
}

/* -- dbg_putc -----------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void dbg_putc(char c)
{
	uart_putc(&dbgprt, c);
}

/* -- dbg_getc -----------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
char dbg_getc()
{
	return uart_getc(&dbgprt);
}


/* --stdio ---------------------------------------------------------------
 * standard function definitions inorder to use stdio.h 
 * ---------------------------------------------------------------------*/
int _close(int file) { return 0; }
int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}
int _isatty(int file) { return 1; }
int _lseek(int file, int ptr, int dir) { return 0; }
int _open(const char *name, int flags, int mode) { return -1; }
int _read(int file, char *ptr, int len) {
	return 1;
}

static char *heap_end = 0;
caddr_t _sbrk(ptrdiff_t incr)
{
	extern char HeapBase;
	extern char HeapLimit;
	char *prev_heap_end;

	if(heap_end == 0)
	{
		heap_end = &HeapBase;
	}
	
	prev_heap_end = heap_end;

	if(heap_end + incr > &HeapLimit)
	{
		return (caddr_t)0;
	}
	
	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

int _write(int file, char *ptr, int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		uart_putc(&dbgprt,*ptr++);
	}
	return len;
}


