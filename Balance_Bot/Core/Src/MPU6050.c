/*
 * MPU6050.c
 *
 *  Created on: May 28, 2026
 *      Author: mmurr
 */


#include "MPU6050.h"
#include <stdio.h>
#include <math.h>

#define ABS(x) (((x) > 0) ? (x) : (-(x)))


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

void MPU6050_init(mpu6050_t* inst, mpu6050_config_t* config){
	inst->config = config->config;
	inst->hi2c = config->hi2c;
	inst->address = config->address;
	inst->accel_offset = config->accel_offset;
	inst->gyro_offset = config->gyro_offset;
	inst->accel_weight = config->accel_weight;

	inst->tx_data = 0;

	Vector3f zero = {
			.x = 0,
			.y = 0,
			.z = 0
	};
	inst->current_angle = zero;
	inst->current_accel = zero;
	inst->last_time = HAL_GetTick();

	inst->calibration_count = config->calibration_count;
	inst->calibration_counter = 0;
	inst->calibrating = 1;
	inst->calibration_done = 0;
	inst->accel_calibration_accumulator[0] = 0;
	inst->accel_calibration_accumulator[1] = 0;
	inst->accel_calibration_accumulator[2] = 0;
	inst->gyro_calibration_accumulator[0] = 0;
	inst->gyro_calibration_accumulator[1] = 0;
	inst->gyro_calibration_accumulator[2] = 0;


	uint8_t whoami = (MPU6050_read_reg(inst, WHO_AM_I_REG) >> 1) & (0x3F);

	if(whoami != ((inst->address >> 1) & 0x3F)){
		printf("MPU6050 whoami failed. Received: 0x%02X\r\n", whoami);
		return;
	}

	printf("MPU6050 whoami success!\r\n");
	uint8_t buffer;

	buffer = inst->config.ClockSource & 0x07; //change the 7th bits of register
	buffer |= (inst->config.Sleep_Mode_Bit << 6) &0x40; // change only the 7th bit in the register
	MPU6050_write_reg(inst, PWR_MAGT_1_REG, buffer);


	//Set the Digital Low Pass Filter
	buffer = 0;
	buffer = inst->config.CONFIG_DLPF & 0x07;
	MPU6050_write_reg(inst, CONFIG_REG, buffer);


	//Select the Gyroscope Full Scale Range
	buffer = 0;
	buffer = (inst->config.Gyro_Full_Scale << 3) & 0x18;
	MPU6050_write_reg(inst, GYRO_CONFIG_REG, buffer);


	//Select the Accelerometer Full Scale Range
	buffer = 0;
	buffer = (inst->config.Accel_Full_Scale << 3) & 0x18;
	MPU6050_write_reg(inst, ACCEL_CONFIG_REG, buffer);

	// Sample rate divider
	MPU6050_write_reg(inst, SMPLRT_DIV_REG, 0);


	switch(inst->config.Accel_Full_Scale) {
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
		printf("Invalid accel scale: %d\r\n", inst->config.Accel_Full_Scale);
	}

	switch(inst->config.Gyro_Full_Scale){
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
		printf("Invalid gyro scale: %d\r\n", inst->config.Gyro_Full_Scale);
	}

	// Enable FIFO
	//buffer = 0;
	//buffer = (0b111 << 4) | (0b1 << 3); // Enable gyro and accel in fifo
	//MPU6050_write_reg(inst, FIFO_EN_REG, buffer);

	// Enable Data ready interrupt
	buffer = 0;
	buffer = 1; // Data ready interrupt en
	MPU6050_write_reg(inst, INT_ENABLE_REG, buffer);


	// User control
	//buffer = 0;
	//buffer = (1 << 6) | (1 << 2); // FIFO enable and reset fifo
	//MPU6050_write_reg(inst, USER_CNT_REG, buffer);
}



void MPU6050_rdy_isr(mpu6050_t* inst){
	inst->tx_data = ACCEL_XOUT_H_REG;
	HAL_I2C_Master_Transmit_DMA(inst->hi2c, inst->address << 1, &(inst->tx_data), 1);
}

void MPU6050_dma_tx_isr(mpu6050_t* inst){
	HAL_I2C_Master_Receive_DMA(inst->hi2c, inst->address << 1, inst->rx_buffer, 2*3*2 + 2);
}

void MPU6050_dma_rx_isr(mpu6050_t* inst){
	MPU6050_process_fifo_data(inst);
}

void MPU6050_process_fifo_data(mpu6050_t* inst){
	uint32_t now_time = HAL_GetTick();
	float deltatime = (now_time - inst->last_time)/1000.0f;
	inst->last_time = now_time;

	// RX Buffer needs to be byte-swapped for endianness
	int16_t accel_data[3];
	uint16_t temp = ((((uint16_t)inst->rx_buffer[6]) << 8) | ((uint16_t)inst->rx_buffer[7]));
	int16_t gyro_data[3];
	for(int i = 0; i < 3; i++){
		accel_data[i] = (int16_t)((((uint16_t)inst->rx_buffer[0 + i*2]) << 8) | ((uint16_t)inst->rx_buffer[1 + i*2]));
		gyro_data[i] = (int16_t)((((uint16_t)inst->rx_buffer[8 + i*2]) << 8) | ((uint16_t)inst->rx_buffer[9 + i*2]));
	}

	inst->current_temp = ((float)temp)/340.0f + 36.53f;

	if(inst->calibrating){
		for(int i = 0; i < 3; i++){
			inst->accel_calibration_accumulator[i] += (int32_t)accel_data[i];
			inst->gyro_calibration_accumulator[i] += (int32_t)gyro_data[i];
		}

		if(++inst->calibration_counter >= inst->calibration_count){
			inst->calibrating = 0;
			inst->calibration_done = 1;

			for(int i = 0; i < 3; i++){
				inst->accel_calibration_accumulator[i] /= inst->calibration_count;
				inst->gyro_calibration_accumulator[i] /= inst->calibration_count;
			}
		}
		return;
	}

	// Apply offsets
	int16_t offset_accel_data[] = {
			accel_data[0] + inst->accel_offset.x,
			accel_data[1] + inst->accel_offset.y,
			accel_data[2] + inst->accel_offset.z,
	};
	int16_t offset_gyro_data[] = {
			gyro_data[0] + inst->gyro_offset.x,
			gyro_data[1] + inst->gyro_offset.y,
			gyro_data[2] + inst->gyro_offset.z,
	};

	// Scale and convert to float
	float accel_scaled[3];
	float gyro_scaled[3];
	for(int i = 0; i < 3; i++){
		accel_scaled[i] = ((float)offset_accel_data[i])*inst->accel_scale;
		gyro_scaled[i] = ((float)offset_gyro_data[i])*inst->gyro_scale;
	}


	// Set current accel
	inst->current_accel.x = accel_scaled[0];
	inst->current_accel.y = accel_scaled[1];
	inst->current_accel.z = accel_scaled[2];


	// Calculate accel magnitude for use in kalman filter weight
	// The further from 1g the accel is, the less trustworthy it is
	float accel_mag = sqrtf(accel_scaled[0]*accel_scaled[0] + accel_scaled[1]*accel_scaled[1] + accel_scaled[2]*accel_scaled[2]);
	float accel_weight_factor = 1 - ABS(accel_mag - 1);
	if(accel_weight_factor < 0)
		accel_weight_factor = 0;


	Vector3f gyro_est_angle; // Estimated angle from gyro only
	gyro_est_angle.x = inst->current_angle.x + gyro_scaled[0]*deltatime;
	gyro_est_angle.y = inst->current_angle.y + gyro_scaled[1]*deltatime;
	gyro_est_angle.z = inst->current_angle.z + gyro_scaled[2]*deltatime;


	Vector3f accel_est_angle; // Estimated angle from accel only
	accel_est_angle.x = atan2f(accel_scaled[1], accel_scaled[2])*180.0f/M_PI;
	accel_est_angle.y = atan2f(-accel_scaled[0], accel_scaled[2])*180.0f/M_PI;


	// Weight accel/gyro and apply it to current angle
	float accel_weight = inst->accel_weight*accel_weight_factor;
	inst->current_angle.x = (accel_weight*accel_est_angle.x + gyro_est_angle.x)/(1 + accel_weight);
	inst->current_angle.y = (accel_weight*accel_est_angle.y + gyro_est_angle.y)/(1 + accel_weight);
	inst->current_angle.z = gyro_est_angle.z;
}

