/*
 * lis3dsh.h
 *
 *  Created on: May 7, 2026
 *      Author: mmurr
 */

#ifndef INC_LIS3DSH_H_
#define INC_LIS3DSH_H_

#define LIS3DSH_CTRL_REG4 0x20
#define LIS3DSH_CTRL_REG1 0x21
#define LIS3DSH_CTRL_REG2 0x22
#define LIS3DSH_CTRL_REG3 0x23
#define LIS3DSH_CTRL_REG5 0x24
#define LIS3DSH_CTRL_REG6 0x25
#define LIS3DSH_FIFO_CTRL 0x2E



typedef enum {
	ODR_PWR_DOWN = 0,
	ODR_3_125HZ = 1,
	ODR_6_25HZ = 2,
	ODR_12_5HZ = 3,
	ODR_25HZ = 4,
	ODR_50HZ = 5,
	ODR_100HZ = 6,
	ODR_400HZ = 7,
	ODR_800HZ = 8,
	ODR_1600HZ = 9
} LIS3DSH_ODR;


typedef enum {
	BW_800HZ = 0,
	BW_200HZ = 1,
	BW_400HZ = 2,
	BW_50HZ = 3,
} LIS3DSH_BW;

typedef enum {
	FS_2G = 0,
	FS_4G = 1,
	FS_6G = 2,
	FS_8G = 3,
	FS_16G = 4
} LIS3DSH_FS;


#endif /* INC_LIS3DSH_H_ */
