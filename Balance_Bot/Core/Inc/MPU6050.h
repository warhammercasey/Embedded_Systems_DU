/*
 * MPU6050.h
 *
 *  Created on: May 28, 2026
 *      Author: mmurr
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "main.h"
#include <math.h>			//Pow()

//Define Registers
#define WHO_AM_I_REG			0x75
#define MPU_ADDR					0x68
#define PWR_MAGT_1_REG		0x6B
#define CONFIG_REG				0x1A
#define GYRO_CONFIG_REG		0x1B
#define ACCEL_CONFIG_REG	0x1C
#define SMPLRT_DIV_REG		0x19
#define INT_STATUS_REG		0x3A
#define ACCEL_XOUT_H_REG	0x3B
#define TEMP_OUT_H_REG		0x41
#define GYRO_XOUT_H_REG		0x43
#define FIFO_EN_REG 			0x23
#define INT_ENABLE_REG 		0x38
#define I2CMACO_REG 			0x23
#define USER_CNT_REG			0x6A
#define FIFO_COUNTH_REG 	0x72
#define FIFO_R_W_REG 			0x74

//1- MPU Configuration
typedef struct
{
	uint8_t ClockSource;
	uint8_t Gyro_Full_Scale;
	uint8_t Accel_Full_Scale;
	uint8_t CONFIG_DLPF;
	uint8_t Sleep_Mode_Bit;

}MPU_ConfigTypeDef;

//2- Clock Source ENUM
enum PM_CLKSEL_ENUM
{
	Internal_8MHz 	= 0x00,
	X_Axis_Ref			= 0x01,
	Y_Axis_Ref			= 0x02,
	Z_Axis_Ref			= 0x03,
	Ext_32_768KHz		= 0x04,
	Ext_19_2MHz			= 0x05,
	TIM_GENT_INREST	= 0x07
};
//3- Gyro Full Scale Range ENUM (deg/sec)
enum gyro_FullScale_ENUM
{
	FS_SEL_250 	= 0x00,
	FS_SEL_500 	= 0x01,
	FS_SEL_1000 = 0x02,
	FS_SEL_2000	= 0x03
};
//4- Accelerometer Full Scale Range ENUM (1g = 9.81m/s2)
enum accel_FullScale_ENUM
{
	AFS_SEL_2g	= 0x00,
	AFS_SEL_4g,
	AFS_SEL_8g,
	AFS_SEL_16g
};
//5- Digital Low Pass Filter ENUM
enum DLPF_CFG_ENUM
{
	DLPF_260A_256G_Hz = 0x00,
	DLPF_184A_188G_Hz = 0x01,
	DLPF_94A_98G_Hz 	= 0x02,
	DLPF_44A_42G_Hz 	= 0x03,
	DLPF_21A_20G_Hz 	= 0x04,
	DLPF_10_Hz 				= 0x05,
	DLPF_5_Hz 				= 0x06
};


typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
}Vector3;

typedef struct{
	float x;
	float y;
	float z;
}Vector3f;

typedef struct {
	MPU_ConfigTypeDef config;
	I2C_HandleTypeDef* hi2c;
	uint8_t address;
	Vector3 accel_offset;
	Vector3 gyro_offset;
	float accel_weight;
	uint32_t calibration_count;

}mpu6050_config_t;


typedef struct{
	MPU_ConfigTypeDef config;
	I2C_HandleTypeDef* hi2c;
	uint8_t address;
	Vector3 accel_offset;
	Vector3 gyro_offset;
	float accel_weight;
	float accel_scale;
	float gyro_scale;
	uint8_t tx_data; // Persistent byte used for dma tx
	Vector3f current_angle;
	Vector3f current_accel;
	float current_temp; // C
	uint32_t last_time;

	int32_t accel_calibration_accumulator[3];
	int32_t gyro_calibration_accumulator[3];
	uint8_t calibrating;
	int32_t calibration_count;
	int32_t calibration_counter;
	uint8_t calibration_done;

	// Raw fifo data order:
	// accel x, y, z, temp, gyro x, y, z
	uint8_t rx_buffer[2*3*2 + 2];
} mpu6050_t;


uint8_t MPU6050_read_reg(mpu6050_t* inst, uint8_t addr);
void MPU6050_write_reg(mpu6050_t* inst, uint8_t addr, uint8_t data);

void MPU6050_init(mpu6050_t* inst, mpu6050_config_t* config);

void MPU6050_rdy_isr(mpu6050_t* inst);
void MPU6050_dma_rx_isr(mpu6050_t* inst);
void MPU6050_dma_tx_isr(mpu6050_t* inst);

void MPU6050_process_fifo_data(mpu6050_t* inst);

#endif /* INC_MPU6050_H_ */
