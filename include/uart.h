/* ***********************************************************************
 * file: 		uart.h
 * author:	jacob cook
 * created on:	7/11/2014
 * ***********************************************************************
 */

#ifndef __UART_H
#define __UART_H

#define UART_SYS_CLK (48000000)

#define UART0_BASE	(0x44E09000)
#define UART1_BASE	(0x48022000)
#define UART2_BASE	(0x48024000)
#define UART3_BASE 	(0x481A6000)
#define UART4_BASE	(0x481A8000)
#define UART5_BASE	(0x481AA000)

struct uartInfo
{
	unsigned int baudrate;
	unsigned int datalen;
	unsigned int parity;
	unsigned int stopbit;
	unsigned int mode;
	struct uartCtlr *ctlr;
};

struct uartCtlr
{
	unsigned int thr; 				/* 0x00 */
#define rhr thr
#define dll thr
	unsigned int ier_irda;		/* 0x04 */
#define ier_cir 	ier_irda
#define ier_uart 	ier_irda
#define dlh 			ier_irda
	unsigned int efr;					/* 0x08 */
#define iir_uart	efr
#define iir_cir		efr
#define fcr				efr
#define iir_irda	efr
	unsigned int lcr;					/* 0x0C */
	unsigned int mcr;					/* 0x10 */
#define xon1_addr1	mcr
	unsigned int xon1_addr2;	/* 0x14 */
#define lsr_cir		xon1_addr2
#define lsr_irda	xon1_addr2
#define lsr_uart	xon1_addr2
	unsigned int tcr;					/* 0x18 */
#define msr		tcr
#define xoff1	tcr
	unsigned int spr;					/* 0x1C */
#define tlr		spr
#define xoff2	spr
	unsigned int mdr1;				/* 0x20 */
	unsigned int mdr2;				/* 0x24 */
	unsigned int txfll;				/* 0x28 */
#define sflsr		txfll
	unsigned int resume;			/* 0x2C */
#define txflh		resume
	unsigned int rxfll;				/* 0x30 */
#define sfregl	rxfll
	unsigned int sfregh;			/* 0x34 */
#define rxflh		sfregh
	unsigned int blr;					/* 0x38 */
#define uasr	blr
	unsigned int acreg;				/* 0x3C */
	unsigned int scr;					/* 0x40 */
	unsigned int ssr;					/* 0x44 */
	unsigned int eblr;				/* 0x48 */
	unsigned int res1;				/* 0x4C */
	unsigned int mvr;					/* 0x50 */
	unsigned int sysc;				/* 0x54 */
	unsigned int syss;				/* 0x58 */
	unsigned int wer;					/* 0x5C */
	unsigned int cfps;				/* 0x60 */
	unsigned int rxfifo_lvl;	/* 0x64 */
	unsigned int txfifo_lvl;	/* 0x68 */
	unsigned int ier2;				/* 0x6C */
	unsigned int isr2;				/* 0x70 */
	unsigned int freq_sel;		/* 0x74 */
	unsigned int res2[2];			/* 0x78 - 0x7C */
	unsigned int mdr3;				/* 0x80 */
	unsigned int tx_dma_threshold; /* 0x84 */
};

/* UART_LCR */
#define UART_CONF_MODE_OP		(1)
#define UART_CONF_MODE_A		(2)
#define UART_CONF_MODE_B		(3)
#define UART_LCR_DIV_EN			(0x1 << 7)
#define UART_LCR_BREAK_EN		(0x1 << 6)
#define UART_LCR_PARITY_T2	(0x1 << 5)
#define UART_LCR_PARITY_T1	(0x1 << 4)
#define	UART_LCR_PARITY_EN	(0x1 << 3)
#define UART_PARITY_MASK		(0x7 << 3)
#define UART_PARITY_ODD			(0x1 << 3)
#define UART_PARITY_EVEN		(0x3 << 3)
#define UART_PARITY_NONE		(0x0 << 3)
#define UART_LCR_STOP				(0x1 << 2)
#define UART_STOP_1BIT			(0x0 << 2)
#define UART_STOP_1P5BIT		(0x1 << 2)
#define UART_LCR_LEN_MASK		(0x3)
#define UART_5BIT				(0x0)
#define UART_6BIT				(0x1)
#define UART_7BIT				(0x2)
#define UART_8BIT				(0x3)

/* UART_LSR */
#define UART_LSR_TXSRE			(0x1 << 6)
#define UART_LSR_TXFIFOE		(0x1 << 5)
#define UART_LSR_RXFIFOE		(0x1)

/* UART IER */
#define UART_IER_CTSIT 			(0x1 << 7)
#define UART_IER_RTSIT			(0x1 << 6)
#define UART_IER_XOFFIT			(0x1 << 5)
#define UART_IER_SLEEPMODE	(0x1 << 4)
#define UART_IER_MODEMSTSIT	(0x1 << 3)
#define UART_IER_LINESTSIT	(0x1 << 2)
#define UART_IER_THRIT			(0x1 << 1)
#define UART_IER_RHRIT			(0x1 << 0)
#define UART_IER_ALL				(0xFF)

/* MDR1 */
#define UART_MODE_MASK		(0x7)
#define UART_MODE_16X			(0x0)
#define UART_MODE_SIR			(0x1)
#define UART_MODE_16X_AB	(0x2)
#define UART_MODE_13X			(0x3)
#define UART_MODE_MIR			(0x4)
#define	UART_MODE_FIR			(0x5)
#define UART_MODE_CIR			(0x6)
#define UART_MODE_DEFAULT	(0x7)

/* MCR */
#define UART_MCR_DTR	(0x1)
#define UART_MCR_RTS	(0x1 << 1)

/* FCR */
#define UART_FCR_TXFIFO_CLR	(0x1 << 2)
#define UART_FCR_RXFIFO_CLR	(0x1 << 1)
#define UART_FCR_FIFO_EN		(0x1)

/* SYSC */
#define UART_SYSC_IDLEMODE_MASK (0x3 << 3)
#define UART_FORCE_IDLE					(0x0 << 3)
#define UART_NO_IDLE						(0x1 << 3)
#define UART_SMART_IDLE					(0x2 << 3)
#define UART_SMART_IDLE_WKUP		(0x3 << 3)
#define UART_SYSC_SOFTRESET			(0x1 << 1)

/* SYSS */
#define UART_SYSS_RESETDONE (0x1)

/* function prototypes */
void uart_enable_int(struct uartCtlr *ctlr, unsigned int flags);
void uart_disable_int(struct uartCtlr *ctlr, unsigned int flags);
unsigned int uart_txsr_empty(struct uartCtlr *ctlr);
unsigned int uart_txfifo_empty(struct uartCtlr *ctlr);
unsigned int uart_rxfifo_empty(struct uartCtlr *ctlr);
void uart_mode_select(struct uartCtlr *ctlr, unsigned int mode);
void uart_config_mode(struct uartCtlr *ctlr, unsigned int conf_mode);
void uart_parity(struct uartCtlr *ctlr, unsigned int parity);
void uart_stop_bit(struct uartCtlr *ctlr, unsigned int stopbit);
void uart_data_len(struct uartCtlr *ctlr, unsigned int datalen);
void uart_baudrate(struct uartCtlr *ctlr, unsigned int baudrate,
									unsigned int mode);
void uart_setup(struct uartInfo *uart);
void uart_putc(struct uartInfo *uart, char c);
void uart_puts(struct uartInfo *uart, char *str, int len);
char uart_getc(struct uartInfo *uart);

#endif
