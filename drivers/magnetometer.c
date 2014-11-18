/* file: 		magnetometer.c
 * author: 	Vishnuu Mallik and Jacob Cook
 * created on: 09/21/2014

 USE THIS !!!
 */

#include "dbg.h"
#include "types.h"
#include "io.h"
#include "i2c.h"
#include "magnetometer.h"


/*#define MAG_ADDRESS1 (0x01)  SOME NUMBER !!! */
 /* Address of the Least significant bit */

/* -- mag_init : INITIALIZATION ROUTINE FOR INSIDE MAGNETOMETER --------
 * 
 * ---------------------------------------------------------------------*/
void mag_init(struct magInfo *mag)   
{
	char cmd[2];

	/* Set mangetometer slave address */
	i2c_slave(mag->i2c_port, mag->slave_addr, I2C_ADDR_7BIT);
    
	/* Set control register 1 */
	cmd[0] = 0x20;
	cmd[1] = 0xF0;
    /*PRINT("\n\r--- CON1 ---\n\r");*/
 	i2c_tx(mag->i2c_port,cmd,2,I2C_STOP);
    
	/* Set control register 2 */
  cmd[0] = 0x21;
  cmd[1] = 0x60; /* Full scale selection of _+4 guass */
  /*PRINT("\n\r--- CON2 ---\n\r");*/
  i2c_tx(mag->i2c_port,cmd,2,I2C_STOP);
    
	/* Set control register 3 */
	cmd[0] = 0x22;
	cmd[1] = 0x00; /* Continous conversion mode */
  /*PRINT("\n\r--- CON3 ---\n\r");*/
	i2c_tx(mag->i2c_port,cmd,2,I2C_STOP);
	
	/* Set control register 4 */
	cmd[0] = 0x23;
	cmd[1] = 0x0C;
  /*PRINT("\n\r--- CON4 ---\n\r");*/
	i2c_tx(mag->i2c_port,cmd,2,I2C_STOP);
    
}

/* -- READING DATA -------------------------------------------------------
 * 
 * ---------------------------------------------------------------------*/
void mag_read(struct magInfo *mag)
{
	char mag_data[6];
	char start_address = X_LSB_ADDRESS;
	
	short X_S;
	short Y_S;
	short Z_S;

	/* Set slave address */	
	i2c_slave(mag->i2c_port, mag->slave_addr, I2C_ADDR_7BIT);
	
	/*Start data transmission by setting read register to X LSB */
	i2c_tx(mag->i2c_port, &start_address, 1, I2C_RESTART); 

	/* read magnetometer data */	
	i2c_rx(mag->i2c_port, mag_data, 6, I2C_STOP);
	
	/* save data to 16 bit registers */
	X_S = (mag_data[1] << 8) | mag_data[0];
	Y_S = (mag_data[3] << 8) | mag_data[2];
	Z_S = (mag_data[5] << 8) | mag_data[4];

	/* set data values in magnetometer structure */
	mag->X = X_S;
	mag->Y = Y_S;
	mag->Z = Z_S;
	
	/* Apply scale factor here */
	
}
