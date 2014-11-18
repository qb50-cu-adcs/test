/*
*  magnetometer.h
*
*
*  Created by vishnuumallik on 9/23/14.
*
*
*/

#ifndef __MAGNETOMETER_H
#define __MAGNETOMETER_H

/* Slave address definitions for the magnetometers */
#define MAG_ADDRESS1 (0x1E) /* Refer to LIS3MDL.pdf page 17 (section 5.1.1). Since SDA/SA1 pin has power, the address is 0011110b*. This one specifficaly is for magnetomter "mag_inside" */
#define MAG_ADDRESS2 (0x1E) /* Address for second Magnetometer "mag_Rwing" assuming it is the same make */
#define MAG_ADDRESS3 (0x1E) /* Address for this magnetomter "mag_Lwing" */

#define X_LSB_ADDRESS 0x28
struct magInfo
{
    char slave_addr;
    int X;
    int Y;
    int Z;
    struct i2c *i2c_port;
};

/*extern struct magInfo mag_inside;
*extern struct magInfo mag_Rwing;
*extern struct magInfo mag_Lwing;
*/
 
 /* function prototypes */
void mag_init(struct magInfo *mag);
void mag_read(struct magInfo *mag);


#endif
