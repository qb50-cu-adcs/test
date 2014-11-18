/* ***********************************************************************
 * file: 		io.h
 * author:	jacob cook
 * created on:	4/6/2014
 * 
 * TODO file description
 * ***********************************************************************
 */

#ifndef __IO_H
#define __IO_H

/*
 * Generic virtual read/write.  Note that we don't support half-word
 * read/writes.  We define __arch_*[bl] here, and leave __arch_*w
 * to the architecture specific code.
 */
#define __getb(a)			(*(volatile unsigned char *)(a))
#define __getw(a)			(*(volatile unsigned short *)(a))
#define __getl(a)			(*(volatile unsigned int *)(a))

#define __putb(v,a)		(*(volatile unsigned char *)(a) = (v))
#define __putw(v,a)		(*(volatile unsigned short *)(a) = (v))
#define __putl(v,a)		(*(volatile unsigned int *)(a) = (v))

#define __raw_writeb(v,a)	__putb(v,a)
#define __raw_writew(v,a)	__putw(v,a)
#define __raw_writel(v,a)	__putl(v,a)

#define __raw_readb(a)		__getb(a)
#define __raw_readw(a)		__getw(a)
#define __raw_readl(a)		__getl(a)

/*
 * define a barrier to prevent the gcc compiler from reordering 
 * the instructions.
 */
#define dmb()		__asm__ __volatile__ ("" : : : "memory")
#define __iormb()	dmb()
#define __iowmb()	dmb()

#define writeb(v,c)	({ u8  __v = v; __iowmb(); __putb(__v,c); __v; })
#define writew(v,c)	({ u16 __v = v; __iowmb(); __putw(__v,c); __v; })
#define writel(v,c)	({ u32 __v = v; __iowmb(); __putl(__v,c); __v; })

#define readb(c)	({ u8  __v = __getb(c); __iormb(); __v; })
#define readw(c)	({ u16 __v = __getw(c); __iormb(); __v; })
#define readl(c)	({ u32 __v = __getl(c); __iormb(); __v; })

/* define macros to logically OR and AND registers */
#define or_regl(v,c)		( writel( ( readl(c) | v ), c) )
#define and_regl(v,c)		( writel( ( readl(c) & v ), c) )
#define or_regb(v,c)		( writeb( ( readb(c) | v ), c) )
#define and_regb(v,c)		( writeb( ( readb(c) & v ), c) )

#endif
