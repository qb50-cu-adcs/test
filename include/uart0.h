/* ***********************************************************************
 * file: 		uart0.h
 * author:	jacob cook
 * created on:	3/24/2014
 * 
 * TODO file description
 *
 * reference files:
 * 	u-boot/include/ns16550.h
 * 	u-boot/include/configs/am335x.h
 * ***********************************************************************
 */

#ifndef __UART0_H
#define __UART0_H


/* UART0 register base */
#define UART0 (0x44E09000)

/* define mode UART mode spacific register */
#define UART0_THR								(UART0 + 0x00)
#define UART0_RHR								UART0_THR
#define UART0_DLL								UART0_THR
#define	UART0_IER								(UART0 + 0x04)
#define UART0_DLH								UART0_IER
#define UART0_EFR								(UART0 + 0x08)
#define UART0_FCR								UART0_EFR
#define UART0_LCR								(UART0 + 0x0C)
#define UART0_MCR								(UART0 + 0x10)
#define UART0_LSR								(UART0 + 0x14)
#define UART0_MSR								(UART0 + 0x18)
#define UART0_SPR								(UART0 + 0x1C)
#define UART0_MDR1							(UART0 + 0x20)
#define UART0_UASR							(UART0 + 0x38)
#define UART0_ACREG							(UART0 + 0x3C)
#define	UART0_SCR								(UART0 + 0x40)
#define UART0_SSR								(UART0 + 0x44)


/* define non-mode specific registers */
#define UART0_ACREG 						(UART0 + 0x3C)
#define UART0_SCR								(UART0 + 0x40)
#define UART0_SSR								(UART0 + 0x44)
#define UART0_EBLR							(UART0 + 0x48)
#define UART0_MVR								(UART0 + 0x50)
#define UART0_SYSC							(UART0 + 0x54)
#define UART0_SYSS							(UART0 + 0x58)
#define UART0_WER								(UART0 + 0x5C)
#define UART0_CFPS							(UART0 + 0x60)
#define UART0_RXFIFO_LVL				(UART0 + 0x64)
#define UART0_TXFIFO_LVL				(UART0 + 0x68)
#define UART0_IER2							(UART0 + 0x6C)
#define UART0_ISR2							(UART0 + 0x70)
#define UART0_FREQ_SEL					(UART0 + 0x74)
#define UART0_MDR3							(UART0 + 0x80)
#define UART0_TX_DMA_THRESHOLD	(UART0 + 0x84)


/* These are the definitions for the FIFO Control Register */

#define UART0_FCR_FIFO_EN	0x01 /* Fifo enable */
#define UART0_FCR_CLEAR_RCVR	0x02 /* Clear the RCVR FIFO */
#define UART0_FCR_CLEAR_XMIT	0x04 /* Clear the XMIT FIFO */
#define UART0_FCR_DMA_SELECT	0x08 /* For DMA applications */
#define UART0_FCR_TRIGGER_MASK	0xC0 /* Mask for the FIFO trigger range */
#define UART0_FCR_TRIGGER_1	0x00 /* Mask for trigger set at 1 */
#define UART0_FCR_TRIGGER_4	0x40 /* Mask for trigger set at 4 */
#define UART0_FCR_TRIGGER_8	0x80 /* Mask for trigger set at 8 */
#define UART0_FCR_TRIGGER_14	0xC0 /* Mask for trigger set at 14 */

/*
 * These are the definitions for the Modem Control Register
 */
#define UART0_MCR_DTR	0x01		/* DTR   */
#define UART0_MCR_RTS	0x02		/* RTS   */
#define UART0_MCR_OUT1	0x04		/* Out 1 */
#define UART0_MCR_OUT2	0x08		/* Out 2 */
#define UART0_MCR_LOOP	0x10		/* Enable loopback test mode */

#define UART0_MCR_DMA_EN	0x04
#define UART0_MCR_TX_DFR	0x08

/*
 * These are the definitions for the Line Control Register
 *
 * Note: if the word length is 5 bits (UART0_LCR_WLEN5), then setting
 * UART0_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
 */
#define UART0_LCR_WLS_MSK 0x03		/* character length select mask */
#define UART0_LCR_WLS_5	0x00		/* 5 bit character length */
#define UART0_LCR_WLS_6	0x01		/* 6 bit character length */
#define UART0_LCR_WLS_7	0x02		/* 7 bit character length */
#define UART0_LCR_WLS_8	0x03		/* 8 bit character length */
#define UART0_LCR_STB	0x04		/* # stop Bits, off=1, on=1.5 or 2) */
#define UART0_LCR_PEN	0x08		/* Parity eneble */
#define UART0_LCR_EPS	0x10		/* Even Parity Select */
#define UART0_LCR_STKP	0x20		/* Stick Parity */
#define UART0_LCR_SBRK	0x40		/* Set Break */
#define UART0_LCR_DIV_EN	0x80		/* Divisor latch access bit */

/*
 * These are the definitions for the Line Status Register
 */
#define UART0_LSR_DR	0x01		/* Data ready */
#define UART0_LSR_OE	0x02		/* Overrun */
#define UART0_LSR_PE	0x04		/* Parity error */
#define UART0_LSR_FE	0x08		/* Framing error */
#define UART0_LSR_BI	0x10		/* Break */
#define UART0_LSR_TXFIFOE	0x20		/* TX FIFO empty */
#define UART0_LSR_TXSRE	0x40		/* TX FIFO and shift register empty */
#define UART0_LSR_ERR	0x80		/* Error */

#define UART0_MSR_DCD	0x80		/* Data Carrier Detect */
#define UART0_MSR_RI	0x40		/* Ring Indicator */
#define UART0_MSR_DSR	0x20		/* Data Set Ready */
#define UART0_MSR_CTS	0x10		/* Clear to Send */
#define UART0_MSR_DDCD	0x08		/* Delta DCD */
#define UART0_MSR_TERI	0x04		/* Trailing edge ring indicator */
#define UART0_MSR_DDSR	0x02		/* Delta DSR */
#define UART0_MSR_DCTS	0x01		/* Delta CTS */

/*
 * These are the definitions for the Interrupt Identification Register
 */
#define UART0_IIR_NO_INT	0x01	/* No interrupts pending */
#define UART0_IIR_ID	0x06	/* Mask for the interrupt ID */

#define UART0_IIR_MSI	0x00	/* Modem status interrupt */
#define UART0_IIR_THRI	0x02	/* Transmitter holding register empty */
#define UART0_IIR_RDI	0x04	/* Receiver data interrupt */
#define UART0_IIR_RLSI	0x06	/* Receiver line status interrupt */

/*
 * These are the definitions for the Interrupt Enable Register
 */
#define UART0_IER_MSI	0x08	/* Enable Modem status interrupt */
#define UART0_IER_RLSI	0x04	/* Enable receiver line status interrupt */
#define UART0_IER_THRI	0x02	/* Enable Transmitter holding register int. */
#define UART0_IER_RDI	0x01	/* Enable receiver data interrupt */

/* function prototypes */
void uart0_init(void);
void uart0_putc(char c);
void uart0_puts(char *s);
unsigned char uart0_getc(void);
unsigned int uart0_tstc(void);

#endif
