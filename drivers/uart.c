/* file: 		uart.c
 * author: 	jacob cook
 * created on: 7/11/2014
 */

#include "dbg.h"
#include "types.h"
#include "io.h"
#include "uart.h"

/* -- uart_soft_reset ----------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void uart_soft_reset(struct uartCtlr *ctlr)
{
	/* perform reset */
	or_regb(UART_SYSC_SOFTRESET, &ctlr->sysc);

	/* make sure it finishes */
	while (!(readb(&ctlr->syss) & UART_SYSS_RESETDONE));

	/* Disable smart idle */
	and_regb(~UART_SYSC_IDLEMODE_MASK, &ctlr->sysc);
	or_regb(UART_NO_IDLE, &ctlr->sysc);
}

	

/* -- uart_enable_int ----------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void uart_enable_int(struct uartCtlr *ctlr, unsigned int flags)
{
	or_regb(flags, &ctlr->ier_uart);
}

/* -- uart_disable_int ---------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void uart_disable_int(struct uartCtlr *ctlr, unsigned int flags)
{
	and_regb(~(char)flags, &ctlr->ier_uart);
}

/* -- uart_txsr_empty ---------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int uart_txsr_empty(struct uartCtlr *ctlr)
{
	return (readb(&ctlr->lsr_uart) & UART_LSR_TXSRE) ? 1 : 0;
}

/* -- uart_txfifo_empty ---------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int uart_txfifo_empty(struct uartCtlr *ctlr)
{
	return (readb(&ctlr->lsr_uart) & UART_LSR_TXFIFOE) ? 1 : 0;
}

/* -- uart_rxfifo_empty ---------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
unsigned int uart_rxfifo_empty(struct uartCtlr *ctlr)
{
	return (readb(&ctlr->lsr_uart) & UART_LSR_RXFIFOE) ? 1 : 0;
}

/* -- uart_mode_select ---------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void uart_mode_select(struct uartCtlr *ctlr, unsigned int mode)
{
	/* clear mode selection */
	and_regb(~UART_MODE_MASK, &ctlr->mdr1);
	/* set mode */
	or_regb(mode, &ctlr->mdr1);
}
	
/* -- uart_config_mode ---------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void uart_config_mode(struct uartCtlr *ctlr, unsigned int conf_mode)
{
	if(conf_mode == UART_CONF_MODE_OP)
	{
		/* clear divide enable bit */
		and_regb(~UART_LCR_DIV_EN, &ctlr->lcr);
	}
	else if(conf_mode == UART_CONF_MODE_A)
	{
		/* set divide enable bit */
		or_regb(UART_LCR_DIV_EN, &ctlr->lcr);
	}
	else if(conf_mode == UART_CONF_MODE_B)
	{
		/* set LCR = 0xBF */
		writeb(0xBF, &ctlr->lcr);
	}
}

/* -- uart_parity --------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void uart_parity(struct uartCtlr *ctlr, unsigned int parity)
{
	/* clear out current parity selection */
	and_regb(~UART_PARITY_MASK, &ctlr->lcr);
	/* set parity */
	or_regb((char)parity, &ctlr->lcr);
}

/* -- uart_stop_bit -----------------------------------------------------
 *
 * --------------------------------------------------------------------*/
void uart_stop_bit(struct uartCtlr *ctlr, unsigned int stopbit)
{
	/* clear stop bit */
	and_regb(~UART_LCR_STOP, &ctlr->lcr);
	/* set stop bit */
	or_regb((char)stopbit, &ctlr->lcr);
}

/* -- uart_data_len ------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void uart_data_len(struct uartCtlr *ctlr, unsigned int datalen)
{
	/* clear data len bits */
	and_regb(~UART_LCR_LEN_MASK, &ctlr->lcr);
	/* set len */
	or_regb((char)datalen, &ctlr->lcr);
}

/* -- uart_baudrate ------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void uart_baudrate(struct uartCtlr *ctlr, unsigned int baudrate,
									unsigned int mode)
{
	unsigned int divisor = 0;

	/* calculate divisor value */
	if(baudrate == 0)
	{
		divisor = 0;
	}
	else if(mode == UART_MODE_13X)
	{	
		divisor = UART_SYS_CLK/(13*baudrate);
	}
	else if(mode == UART_MODE_16X)
	{
		divisor = UART_SYS_CLK/(16*baudrate);
	}

	/* enable access to the Divisor latch registers */
	uart_config_mode(ctlr, UART_CONF_MODE_A);

	/* write divisor value to latch registers */
	writeb((char)(divisor & 0xFF), &ctlr->dll);
	writeb((char)((divisor >> 8) & 0xFF), &ctlr->dlh);

	/* disable access to the divisor latch registers */
	uart_config_mode(ctlr, UART_CONF_MODE_OP);
}

/* -- uart_setup ---------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void uart_setup(struct uartInfo *uart)
{
	struct uartCtlr *ctlr = uart->ctlr;
		
	uart_soft_reset(ctlr);

	while(!uart_txsr_empty(ctlr));
	
	/* disable interrupts */
	uart_disable_int(ctlr, UART_IER_ALL);
	/* put in default mode for configuration */
	uart_mode_select(ctlr, UART_MODE_DEFAULT);
	/* zero baudrate divisors */
	uart_baudrate(ctlr, 0, 0);
	/* Force RTS and DTR to active (low) */
	writeb((UART_MCR_DTR | UART_MCR_RTS), &ctlr->mcr);
	/* enable and clear rx and tx buffers */
	writeb((UART_FCR_TXFIFO_CLR | UART_FCR_RXFIFO_CLR | UART_FCR_FIFO_EN),
					&ctlr->fcr);
	/* set parity and stop bits */
	uart_parity(ctlr, uart->parity);
	uart_stop_bit(ctlr, uart->stopbit);
	/* Set data len */
	uart_data_len(ctlr, uart->datalen);
	/* set baudrate */
	uart_baudrate(ctlr, uart->baudrate, uart->mode);
	/* Put in specified mode */
	uart_mode_select(ctlr, uart->mode);	
}

/* -- uart_putc ----------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
void uart_putc(struct uartInfo *uart, char c)
{
	struct uartCtlr *ctlr = uart->ctlr;
	/* wait for room in the tx buffer */
	while(!uart_txfifo_empty(ctlr));
	/* write char to tx holding register */
	writeb(c, &ctlr->thr);
}

/* -- uart_puts ----------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void uart_puts(struct uartInfo *uart, char *str, int len)
{
	unsigned int i;
	
	if(len == -1)
	{
		/* keep loding until we hit the NULL character */
		while(*str != 0)
		{
			uart_putc(uart, *str++);
		}
	}
	else
	{
		/* load number of bytes specified */
		for(i=len;i!=0;i--)
		{
			uart_putc(uart, *str++);
		}
	}
}	

/* uart_getc -------------------------------------------------------------
 *
 * ---------------------------------------------------------------------*/
char uart_getc(struct uartInfo *uart)
{
	struct uartCtlr *ctlr = uart->ctlr;

	while(!uart_rxfifo_empty(ctlr));
	return readb(&ctlr->rhr);
}

