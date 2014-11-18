/* ***********************************************************************
 * file: 		clocks.c
 * author: 	jacob cook
 * created on: 1/20/2014
 *
 * This file contains lowlevel routines to interface with system clocks.
 * 
 * 	- Pulls from u-boot/arch/arm/cpu/armv7/am3xx/clocks_am33xx.c
 * ***********************************************************************
 */

#include "types.h"
#include "io.h"
#include "clocks.h" 
#include "dbg.h"

/* internal function prototypes */
static void mpu_pll_config(void);
static void core_pll_config(void);
static void per_pll_config(void);
static void enable_interface_clocks(void);
static void power_domain_wkup_transition(void);
static void enable_per_clocks(void);

/* -- pll_init -----------------------------------------------------------
 * 
 * This function configures and enables the critical PLL for booting. 
 * This includes configuring the MPU, core, and peripheral PLL, enabling
 * the interface clocks, and turning on the clocks to peripherals needed
 * during booting. 
 *
 * NOTE: 	this function could be used to configure other devices needed 
 * 				for the main application.
 * -----------------------------------------------------------------------
 */
void pll_init(void)
{
	/* Configure PLL clock speeds */
	mpu_pll_config();
	core_pll_config();
	per_pll_config();

	/* Enable all interface clocks */
	enable_interface_clocks();

	/* Power domain wake up transitions */
	power_domain_wkup_transition();

	/* Enable the peripheral clocks */
	enable_per_clocks();

}


/* -- mpu_pll_config -----------------------------------------------------
 * 
 * This function configures the MPU PLL. See clocks.h for PLL settings:
 * MPU_PLL_M, MPU_PLL_N, MPU_PLL_M2
 * -----------------------------------------------------------------------
 */ 

static void mpu_pll_config(void)
{
	u32 clkmode, clksel, div_m2;
	
	/* read current settings of MPU PLL */
	clkmode = readl(CM_CLKMODE_DPLL_MPU);
	clksel = readl(CM_CLKSEL_DPLL_MPU);
	div_m2 = readl(CM_DIV_M2_DPLL_MPU);

	/* set the PLL to bypass mode */
	writel(PLL_BYPASS_MODE, CM_CLKMODE_DPLL_MPU);
	
	/* Read IDLEST register to verify bypass mode */
	while (readl(CM_IDLEST_DPLL_MPU) != ST_MN_BYPASS)
		;
	
	/* set clock to desired frequency (see clock.h for settings */
	clksel = clksel & (~CLK_SEL_MASK);
	clksel = clksel | ((MPU_PLL_M << CLK_SEL_SHIFT) | MPU_PLL_N);
	writel(clksel, CM_CLKSEL_DPLL_MPU);

	div_m2 = div_m2 & ~CLK_DIV_MASK;
	div_m2 = div_m2 | MPU_PLL_M2;
	writel(div_m2, CM_DIV_M2_DPLL_MPU);

	/* relock PLL with new settings */
	clkmode = clkmode | CLK_MODE_SEL;
	writel(clkmode, CM_CLKMODE_DPLL_MPU);

	/* read IDLEST register to verify that PLL is locked */
	while (readl(CM_IDLEST_DPLL_MPU) != ST_DPLL_CLK)
	;
}


/* -- core_pll_config ----------------------------------------------------
 *
 * This function configures the core PLL. See clocks.h for PLL settings:
 * CORE_PLL_M, CORE_PLL_N, CORE_M4, CORE_M5, CORE_M6.
 * -----------------------------------------------------------------------
 */
static void core_pll_config(void)
{
	u32 clkmode, clksel, div_m4, div_m5, div_m6;

	/* read out current clock settings */
	clkmode = readl(CM_CLKMODE_DPLL_CORE);
	clksel = readl(CM_CLKSEL_DPLL_CORE);
	div_m4 = readl(CM_DIV_M4_DPLL_CORE);
	div_m5 = readl(CM_DIV_M5_DPLL_CORE);
	div_m6 = readl(CM_DIV_M6_DPLL_CORE);

	/* Set the core PLL to bypass Mode */
	writel(PLL_BYPASS_MODE, CM_CLKMODE_DPLL_CORE);

	/* Verify transition to bypass mode */
	while (readl(CM_IDLEST_DPLL_CORE) != ST_MN_BYPASS)
		;

	/* Set core clock speeds based on core M and N */
	clksel = clksel & (~CLK_SEL_MASK);
	clksel = clksel | ((CORE_PLL_M << CLK_SEL_SHIFT) | CORE_PLL_N);
	writel(clksel, CM_CLKSEL_DPLL_CORE);

	/* M4 */
	div_m4 = div_m4 & (~CLK_DIV_MASK);
	div_m4 = div_m4 | CORE_PLL_M4;
	writel(div_m4, CM_DIV_M4_DPLL_CORE);

	/* M5 */
	div_m5 = div_m5 & (~CLK_DIV_MASK);
	div_m5 = div_m5 | CORE_PLL_M5;
	writel(div_m5, CM_DIV_M5_DPLL_CORE);

	/* M6 */
	div_m6 = div_m6 & (~CLK_DIV_MASK);
	div_m6 = div_m6 | CORE_PLL_M6;
	writel(div_m6, CM_DIV_M6_DPLL_CORE);

	/* lock the PLL */
	clkmode = clkmode | CLK_MODE_SEL;
	writel(clkmode, CM_CLKMODE_DPLL_CORE);

	/* verify PLL is locked */
	while (readl(CM_IDLEST_DPLL_CORE) != ST_DPLL_CLK)
		;
}

/* -- per_pll_config -----------------------------------------------------
 * 
 * This function configures the peripheral PLL. See clock.h for PLL 
 * settings: PER_PLL_M, PER_PLL_N, PER_PLL_M2, DPLL_CLKDCOLDO_GATE_CTRL 
 * -----------------------------------------------------------------------
 */
static void per_pll_config(void)
{
	u32 clkmode, clksel, div_m2;

	/* read current Peripheral PLL settings */
	clkmode = readl(CM_CLKMODE_DPLL_PER);
	clksel = readl(CM_CLKSEL_DPLL_PER);
	div_m2 = readl(CM_DIV_M2_DPLL_PER);

	/* Put PLL in standby mode */
	writel(PLL_BYPASS_MODE, CM_CLKMODE_DPLL_PER);
	
	/* Verify bypass mode */
	while (readl(CM_IDLEST_DPLL_PER) != ST_MN_BYPASS)
		;

	/* Set Clock configuration */
	clksel = clksel & (~CLK_SEL_MASK);
	clksel = clksel | ((PER_PLL_M << CLK_SEL_SHIFT) | PER_PLL_N);
	writel(clksel, CM_CLKSEL_DPLL_PER);

	div_m2 = div_m2 & (~CLK_DIV_MASK);
	div_m2 = div_m2 | PER_PLL_M2;
	writel(div_m2, CM_DIV_M2_DPLL_PER);

	/* lock PLL */
	clkmode = clkmode | CLK_MODE_SEL;
	writel(clkmode, CM_CLKMODE_DPLL_PER);

	/* Verify PLL is locked */
	while (readl(CM_IDLEST_DPLL_PER) != ST_DPLL_CLK)
		;

	/* enable clock output */
	writel(DPLL_CLKDCOLDO_GATE_CTRL, CM_CLKDCOLDO_DPLL_PER);

}

/* -- enable_interface_clocks --------------------------------------------
 * 
 * This function enables all interface clocks. These are the L3 and L4 
 * interface clocks. TODO: why is GPIO0 enabled here? 
 * -----------------------------------------------------------------------
 */
static void enable_interface_clocks(void)
{
	/* Enable L3 interconnect clocks */
	writel(PRCM_MOD_EN, CM_PER_L3_CLKCTRL);
	while (readl(CM_PER_L3_CLKCTRL) != PRCM_MOD_EN)
		;
	
	/* Enable the L4LS clocks */
	writel(PRCM_MOD_EN, CM_PER_L4LS_CLKCTRL);
	while (readl(CM_PER_L4LS_CLKCTRL) != PRCM_MOD_EN)
		;

	/* NOT SURE WHAT THIS IS. NOT IS REFRENCE MANUAL */
	writel(PRCM_MOD_EN, CM_PER_L4FW_CLKCTRL);
	while (readl(CM_PER_L4FW_CLKCTRL) != PRCM_MOD_EN)

	/* Enable L4 Wakeup clocks */
	writel(PRCM_MOD_EN, CM_WKUP_L4WKUP_CLKCTRL);
	while (readl(CM_WKUP_L4WKUP_CLKCTRL) != PRCM_MOD_EN)
		;

	/* Enable L3 Instr clocks */
	writel(PRCM_MOD_EN, CM_PER_L3_INSTR_CLKCTRL);
	while (readl(CM_PER_L3_INSTR_CLKCTRL) != PRCM_MOD_EN)
		;

	/* Enable L4 fast clocks */
	writel(PRCM_MOD_EN, CM_PER_L4HS_CLKCTRL);
	while (readl(CM_PER_L4HS_CLKCTRL) != PRCM_MOD_EN)
		;

	/* Enable GPIO0 clocks */
	writel(PRCM_MOD_EN, CM_WKUP_GPIO0_CLKCTRL);
	while (readl(CM_WKUP_GPIO0_CLKCTRL) != PRCM_MOD_EN)
		;
}

/* -- power_domain_wkup_transition ---------------------------------------
 * 
 * This function forces the power domain wakup transition.  This ensures
 * that the L3 and L4 interface clocks are active before using the
 * peripherals.
 * -----------------------------------------------------------------------
 */
static void power_domain_wkup_transition(void)
{
	writel(PRCM_FORCE_WAKEUP, CM_PER_L3_CLKSTCTRL);
	writel(PRCM_FORCE_WAKEUP, CM_PER_L4LS_CLKSTCTRL);
	writel(PRCM_FORCE_WAKEUP, CM_WKUP_CLKSTCTRL);
	writel(PRCM_FORCE_WAKEUP, CM_PER_L4FW_CLKCTRL);
	writel(PRCM_FORCE_WAKEUP, CM_PER_L3S_CLKSTCTRL);
	writel(PRCM_FORCE_WAKEUP, CM_PER_OCPWP_L3_CLKSTCTRL);
}

/* -- enable_per_clocks --------------------------------------------------
 * 
 * This function enables the clocks of the peripherals needed for booting.
 *
 * TODO: Trim boot clocks that are not needed for custom boot.
 * -----------------------------------------------------------------------
 */
static void enable_per_clocks(void)
{
	/* Enable Control Module (Redudant, should have been done in ROM boot */
	writel(PRCM_MOD_EN, CM_WKUP_CONTROL_CLKCTRL);
	while (readl(CM_WKUP_CONTROL_CLKCTRL) != PRCM_MOD_EN)
		;

	/* enable the module clock */
	writel(PRCM_MOD_EN, CM_PER_TIMER2_CLKCTRL);
	while (readl(CM_PER_TIMER2_CLKCTRL) != PRCM_MOD_EN)
		;

	/* select the Master OSC 24 MHz as time2 clock source */
 	writel(0x1, CLKSEL_TIMER2_CLK); /* reset value */

	/* Enable Timers 4,5 and 7 used for PWM signals */
	writel(PRCM_MOD_EN, CM_PER_TIMER4_CLKCTRL);
	while(readl(CM_PER_TIMER4_CLKCTRL) != PRCM_MOD_EN)
		;
 	writel(0x1, CLKSEL_TIMER4_CLK); 

	writel(PRCM_MOD_EN, CM_PER_TIMER5_CLKCTRL);
	while(readl(CM_PER_TIMER5_CLKCTRL) != PRCM_MOD_EN)
		;
 	writel(0x1, CLKSEL_TIMER5_CLK);

	writel(PRCM_MOD_EN, CM_PER_TIMER7_CLKCTRL);
	while(readl(CM_PER_TIMER7_CLKCTRL) != PRCM_MOD_EN)
		;
 	writel(0x1, CLKSEL_TIMER7_CLK);

	/* UART0 */
	writel(PRCM_MOD_EN, CM_WKUP_UART0_CLKCTRL);
	while (readl(CM_WKUP_UART0_CLKCTRL) != PRCM_MOD_EN)
		;

	/* UART1 */
	writel(PRCM_MOD_EN, CM_PER_UART1_CLKCTRL);
	while (readl(CM_PER_UART1_CLKCTRL) != PRCM_MOD_EN)
		;
	
	/* GPMC */
	writel(PRCM_MOD_EN, CM_PER_GPMC_CLKCTRL);
	while (readl(CM_PER_GPMC_CLKCTRL) != PRCM_MOD_EN)
		;

	/* ELM */
	writel(PRCM_MOD_EN, CM_PER_ELM_CLKCTRL);
	while (readl(CM_PER_ELM_CLKCTRL) != PRCM_MOD_EN)
		;

	/* MMC0 */
	writel(PRCM_MOD_EN, CM_PER_MMC0_CLKCTRL);
	while (readl(CM_PER_MMC0_CLKCTRL) != PRCM_MOD_EN)
		;

	/* MMC1 */
	writel(PRCM_MOD_EN, CM_PER_MMC1_CLKCTRL);
	while (readl(CM_PER_MMC1_CLKCTRL) != PRCM_MOD_EN)
		;

	/* i2c0 MIGHT NOT USE */
	writel(PRCM_MOD_EN, CM_WKUP_I2C0_CLKCTRL);
	while (readl(CM_WKUP_I2C0_CLKCTRL) != PRCM_MOD_EN)
		;

	/* i2c1 */
	writel(PRCM_MOD_EN, CM_PER_I2C1_CLKCTRL);
	while (readl(CM_PER_I2C1_CLKCTRL) != PRCM_MOD_EN)
		;

	/* i2c2 */
	writel(PRCM_MOD_EN, CM_PER_I2C2_CLKCTRL);
	while (readl(CM_PER_I2C2_CLKCTRL) != PRCM_MOD_EN)
		;

	/* gpio1 module */
	writel(PRCM_MOD_EN, CM_PER_GPIO1_CLKCTRL);
	while (readl(CM_PER_GPIO1_CLKCTRL) != PRCM_MOD_EN)
		;

	/* gpio2 module */
	writel(PRCM_MOD_EN, CM_PER_GPIO2_CLKCTRL);
	while (readl(CM_PER_GPIO2_CLKCTRL) != PRCM_MOD_EN)
		;

	/* gpio3 module */
	writel(PRCM_MOD_EN, CM_PER_GPIO3_CLKCTRL);
	while (readl(CM_PER_GPIO3_CLKCTRL) != PRCM_MOD_EN)
		;

	/* Ethernet NOT USING*/
	writel(PRCM_MOD_EN, CM_PER_CPGMAC0_CLKCTRL);
	while (readl(CM_PER_CPGMAC0_CLKCTRL) != PRCM_MOD_EN)
		;

	/* spi0  NOT USING */
	writel(PRCM_MOD_EN, CM_PER_SPI0_CLKCTRL);
	while (readl(CM_PER_SPI0_CLKCTRL) != PRCM_MOD_EN)
		;

	/* RTC MIGHT USE */
	writel(PRCM_MOD_EN, CM_RTC_RTC_CLKCTRL);
	while (readl(CM_RTC_RTC_CLKCTRL) != PRCM_MOD_EN)
		;

	/* MUSB MIGHT USE */
	writel(PRCM_MOD_EN, CM_PER_USB0_CLKCTRL);
	while (readl(CM_PER_USB0_CLKCTRL) != PRCM_MOD_EN)
		;

}
