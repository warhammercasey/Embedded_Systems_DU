/*
 * MPU6050.c
 *
 *  Created on: May 28, 2026
 *      Author: mmurr
 */


#include "MPU6050.h"


uint8_t MPU6050_read_reg(mpu6050_t* inst, uint8_t addr){
	uint8_t res;
	HAL_I2C_Master_Transmit(inst->hi2c, inst->address << 1, &addr, 1, 10);
	HAL_I2C_Master_Receive(inst->hi2c, inst->address << 1, &res, 1, 100);
	return res;
}

void MPU6050_write_reg(mpu6050_t* inst, uint8_t addr, uint8_t data){
	uint8_t buf[2] = {addr, data};
	HAL_I2C_Master_Transmit(inst->hi2c, inst->address << 1, buf, 2, 100);
}

void MPU6050_init(mpu6050_t* inst){
	uint8_t whoami = (MPU6050_read_reg(inst, WHO_AM_I_REG) >> 1) & (0x3F);

	if(whoami != ((inst->address >> 1) & 0x3F)){
		printf("MPU6050 whoami failed. Received: 0x%02X\r\n", whoami);
		return;
	}

	printf("MPU6050 whoami success!\r\n");
	uint8_t buffer;

	buffer = inst->config->ClockSource & 0x07; //change the 7th bits of register
	buffer |= (inst->config->Sleep_Mode_Bit << 6) &0x40; // change only the 7th bit in the register
	MPU6050_write_reg(inst, PWR_MAGT_1_REG, buffer);


	//Set the Digital Low Pass Filter
	buffer = 0;
	buffer = inst->config->CONFIG_DLPF & 0x07;
	MPU6050_write_reg(inst, CONFIG_REG, buffer);


	//Select the Gyroscope Full Scale Range
	buffer = 0;
	buffer = (inst->config->Gyro_Full_Scale << 3) & 0x18;
	MPU6050_write_reg(inst, GYRO_CONFIG_REG, buffer);


	//Select the Accelerometer Full Scale Range
	buffer = 0;
	buffer = (inst->config->Accel_Full_Scale << 3) & 0x18;
	MPU6050_write_reg(inst, ACCEL_CONFIG_REG, buffer);

	// Sample rate divider
	MPU6050_write_reg(inst, SMPLRT_DIV_REG, 0);


	switch(inst->config->Accel_Full_Scale) {
	case AFS_SEL_2g:
		inst->accel_scale = (2.0f/32768.0f);
		break;

	case AFS_SEL_4g:
		inst->accel_scale = (4.0f/32768.0f);
		break;

	case AFS_SEL_8g:
		inst->accel_scale = (8.0f/32768.0f);
		break;

	case AFS_SEL_16g:
		inst->accel_scale = (16.0f/32768.0f);
		break;

	default:
		printf("Invalid accel scale: %d\r\n", inst->config->Accel_Full_Scale);
	}

	switch(inst->config->Gyro_Full_Scale){
	case FS_SEL_250:
		inst->gyro_scale = (250.0f/32768.0f);
		break;

	case FS_SEL_500:
		inst->gyro_scale = (500.0f/32768.0f);
		break;

	case FS_SEL_1000:
		inst->gyro_scale = (1000.0f/32768.0f);
		break;


	case FS_SEL_2000:
		inst->gyro_scale = (2000.0f/32768.0f);
		break;

	default:
		printf("Invalid gyro scale: %d\r\n", inst->config->Gyro_Full_Scale);
	}

	// Enable FIFO
	buffer = 0;
	buffer = (0b111 << 4) | (0b1 << 3); // Enable gyro and accel in fifo
	MPU6050_write_reg(inst, FIFO_EN_REG, buffer);

	// Enable FIFO ready interrupt
	buffer = 0;
	buffer = 1; // Data ready interrupt en
	MPU6050_write_reg(inst, INT_ENABLE_REG, buffer);


	// User control
	buffer = 0;
	buffer = (1 << 6) | (1 << 2); // FIFO enable and reset fifo
	MPU6050_write_reg(inst, USER_CNT_REG, buffer);
}
