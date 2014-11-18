/* file: test.c
 * author: Jacob Cook
 * created on: 5/21/2014
 *
 */

#include "dbg.h"
#include "io.h"
#include "types.h"
#include "clocks.h"
#include "uart.h"
#include "control_module.h"
#include "i2c.h"
#include "sd_card.h"
#include "mmcsd.h"
#include "timer.h"
#include "pwm.h"
#include "util.h"
#include "test.h"
#include "magnetometer.h"
#include "ad7991.h"
#include "global.h"
#include "torque_rod.h"
#include "gpio.h"
#include "ff.h"

/*************************************************************************
**             ADCS MAIN LOOP 
*************************************************************************/

int main()
{
	FATFS fs;
	FIL fsrc, fdst;      // file objects
	BYTE buffer[4096];   // file copy buffer
	FRESULT res;         // FatFs function common result code
	WORD br, bw;         // File R/W count

	/* Configuring the system clocks */
	INIT_DBG_PORT();
	pll_init();
	util_setup();
    
	/* Perform necessary pin muxing */
	enable_mmc0_pin_mux();
	enable_i2c0_pin_mux();	
	enable_i2c1_pin_mux();
	enable_i2c2_pin_mux();
	enable_torq_rod_pin_mux();

	/* SD card init functions */
	sd0.ctlr = mmcsd0;
	mmcsd_init(sd0.ctlr);


	/* FILE SYSTEM DEMO */
	/* copies contents of srcfile.txt to a new file dstfile.txt */

	// Register a work area to logical drive 0
	f_mount(0, &fs);

	// Open source file
	res = f_open(&fsrc, "srcfile.txt", FA_OPEN_EXISTING | FA_READ);
	if (res) PRINT("RES1 = %u\n\r",res);

	// Create destination file
	res = f_open(&fdst, "dstfile.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if (res) PRINT("RES2 = %u\n\r",res);

	// Copy source to destination
	for (;;) {
			res = f_read(&fsrc, buffer, sizeof(buffer), &br);
			if (res || br == 0) break;      // error or eof
			res = f_write(&fdst, buffer, br, &bw);
			if (res || bw < br) break;   // error or disk full
	}

	// Close all files
	f_close(&fsrc);
	f_close(&fdst);

	// Unregister a work area before discard it
	f_mount(0, NULL);

	while(1);

	return 1;

}
