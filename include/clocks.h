/* ***********************************************************************
 * file: 		clocks.h
 * author:	jacob cook
 * created on:	1/20/2014
 * 
 * TODO file description
 * ***********************************************************************
 */

#ifndef __CLOCKS_H
#define __CLOCKS_H

/* External Oscillator Frequency (Y2) */
#define OSC (24000000) /* 24 Mhz */
#define OSC_M (OSC/1000000)

/* PRCM Tools */
#define PRCM_MOD_EN		(0x2)
#define PRCM_MOD_DIS	(0x0)
#define PRCM_FORCE_WAKEUP (0x2)

/* Tools for setting up PLLs */
#define PLL_BYPASS_MODE (0x00000004)
#define ST_MN_BYPASS		(0x00000100)
#define ST_DPLL_CLK			(0x00000001)
#define CLK_SEL_MASK		(0x0007FFFF)
#define CLK_DIV_MASK		(0x0000001F)
#define CLK_DIV2_MASK		(0x0000007F)
#define CLK_SEL_SHIFT		(0x8)
#define CLK_MODE_SEL		(0x00000007)

/* MPU PLL settings - Fmpu = 550 MHz */
#define MPU_PLL_M (550) /* MPU clock speed in Mhz */
#define MPU_PLL_N (OSC_M-1) /* MPU clock divisor */
#define MPU_PLL_M2 (1)

/* CORE PLL settings - Fcore = 1 GHz */
#define CORE_PLL_M (1000) /* Core clock speed in Mhz */
#define CORE_PLL_N (OSC_M-1) /* core clock divisor */
#define CORE_PLL_M4 (10) 	/* CORE_CLKOUTM4 = 200 MHz */
#define CORE_PLL_M5 (8)		/* CORE_CLKOUTM5 = 250 MHz */
#define CORE_PLL_M6 (4)		/* CORE_CLKOUTM6 = 500 MHz */

/* Periphereal PLL Settings
 * USB PHY clock needs to be 960 MHz and clkout needs to be 192 Mhz
 * for periphereal devices.
 */
#define PER_PLL_M (960)
#define PER_PLL_N (OSC_M-1)
#define PER_PLL_M2 (5)
#define DPLL_CLKDCOLDO_GATE_CTRL	(0x300) /* enable clock output */

/* CM PER L4LS CLKSTCTRL Register Flags */
#define CLKACTIVITY_I2C_FCLK (0x1 << 24)
#define CLKACTIVITY_L4LS_GCLK (0x1 << 8)

/* Clock Module (CM) Memory base address locations */
#define CM_PER 		(0x44E00000)	/* CM Peipheral Registers */
#define CM_WKUP 	(0x44E00400)	/* CM Wakeup Registers */
#define CM_DPLL 	(0x44E00500)	/* CM PLL Registers */
#define CM_MPU 		(0x44E00600)	/* CM MPU Registers */
#define CM_DEVICE (0x44E00700)	/* CM Device Registers */
#define CM_RTC 		(0x44E00800)	/* CM RTC Registers */
#define CM_GFX 		(0x44E00900)	/* CM Graphics Controller Register */

/* CM_PER Regisiters */
#define CM_PER_L4LS_CLKSTCTRL		(CM_PER + 0x00)
#define CM_PER_L3S_CLKSTCTRL 		(CM_PER + 0x04)
#define CM_PER_L3_CLKSTCTRL			(CM_PER + 0x0C)
#define CM_PER_CPGMAC0_CLKCTRL	(CM_PER + 0x14)
#define CM_PER_USB0_CLKCTRL			(CM_PER + 0x1C)
#define CM_PER_GPMC_CLKCTRL			(CM_PER + 0x30)
#define CM_PER_MMC0_CLKCTRL			(CM_PER + 0x3C)
#define CM_PER_ELM_CLKCTRL			(CM_PER + 0x40)
#define CM_PER_I2C2_CLKCTRL			(CM_PER + 0x44)
#define CM_PER_I2C1_CLKCTRL			(CM_PER + 0x48)
#define CM_PER_SPI0_CLKCTRL			(CM_PER + 0x4C)
#define CM_PER_L4LS_CLKCTRL			(CM_PER + 0x60)
#define CM_PER_L4FW_CLKCTRL			(CM_PER + 0x64)
#define CM_PER_UART1_CLKCTRL		(CM_PER + 0x6C)
#define CM_PER_TIMER7_CLKCTRL		(CM_PER + 0x7C)
#define CM_PER_TIMER2_CLKCTRL		(CM_PER + 0x80)
#define CM_PER_TIMER4_CLKCTRL		(CM_PER + 0x88)
#define CM_PER_GPIO1_CLKCTRL		(CM_PER + 0xAC)
#define CM_PER_GPIO2_CLKCTRL		(CM_PER + 0xB0)
#define CM_PER_GPIO3_CLKCTRL		(CM_PER + 0xB4)
#define CM_PER_L3_INSTR_CLKCTRL (CM_PER + 0xDC)
#define CM_PER_L3_CLKCTRL				(CM_PER + 0xE0)
#define CM_PER_TIMER5_CLKCTRL		(CM_PER + 0xEC)
#define CM_PER_TIMER6_CLKCTRL		(CM_PER + 0xF0)
#define CM_PER_MMC1_CLKCTRL			(CM_PER + 0xF4)
#define CM_PER_L4HS_CLKCTRL			(CM_PER + 0x120)
#define CM_PER_OCPWP_L3_CLKSTCTRL (CM_PER + 0x12C)

/* CM_WKUP Clock Module Wakeup Registers */
/* TODO: fill in the rest of the register addresses */
#define CM_WKUP_CLKSTCTRL				(CM_WKUP + 0x00)
#define CM_WKUP_CONTROL_CLKCTRL	(CM_WKUP + 0x04)
#define CM_WKUP_GPIO0_CLKCTRL		(CM_WKUP + 0x08)
#define CM_WKUP_L4WKUP_CLKCTRL	(CM_WKUP + 0x0C)
#define CM_IDLEST_DPLL_MPU 			(CM_WKUP + 0x20)
#define CM_CLKSEL_DPLL_MPU 			(CM_WKUP + 0x2C)
#define CM_IDLEST_DPLL_CORE			(CM_WKUP + 0x5C)
#define CM_CLKSEL_DPLL_CORE			(CM_WKUP + 0x68)
#define CM_IDLEST_DPLL_PER			(CM_WKUP + 0x70)
#define CM_CLKDCOLDO_DPLL_PER		(CM_WKUP + 0x7C)
#define CM_DIV_M4_DPLL_CORE			(CM_WKUP + 0x80)
#define CM_DIV_M5_DPLL_CORE			(CM_WKUP + 0x84)
#define CM_CLKMODE_DPLL_MPU 		(CM_WKUP + 0x88)
#define CM_CLKMODE_DPLL_PER 		(CM_WKUP + 0x8C)
#define CM_CLKMODE_DPLL_CORE		(CM_WKUP + 0x90)
#define CM_CLKSEL_DPLL_PER			(CM_WKUP + 0x9C)
#define CM_DIV_M2_DPLL_MPU 			(CM_WKUP + 0xA8)
#define CM_DIV_M2_DPLL_PER			(CM_WKUP + 0xAC)
#define CM_WKUP_UART0_CLKCTRL		(CM_WKUP + 0xB4)
#define CM_WKUP_I2C0_CLKCTRL		(CM_WKUP + 0xB8)
#define CM_DIV_M6_DPLL_CORE			(CM_WKUP + 0xD8)

/* RTC Registers */
#define CM_RTC_RTC_CLKCTRL			(CM_RTC + 0x00)

/* DPLL Registers */
#define CLKSEL_TIMER7_CLK				(CM_DPLL + 0x04)
#define CLKSEL_TIMER2_CLK				(CM_DPLL + 0x08)
#define CLKSEL_TIMER4_CLK				(CM_DPLL + 0x10)
#define CLKSEL_TIMER5_CLK				(CM_DPLL + 0x18)
#define CLKSEL_TIMER6_CLK				(CM_DPLL + 0x1C)

/* external function prototypes */
void pll_init(void);

#endif
