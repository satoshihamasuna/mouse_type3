/*
 * gyro.c
 *
 *  Created on: 2021/03/20
 *      Author: sato1
 */

#include "index.h"
#include "math.h"
#include "glob_var.h"
#include "ICM20648.h"

static uint8_t imu_address = ACCEL_XOUT_H|0x80; //ACCEL_X_HIGH_BYTE
static uint8_t imu_value[13];

static uint8_t renc_address[2] = {0x00,0x00};
static uint8_t renc_value[2];

static uint8_t lenc_address[2] = {0x00,0x00};
static uint8_t lenc_value[2];

uint8_t read_byte( uint8_t reg ){
	uint8_t val = 0x00;
	uint8_t dammy = 0x00;
	reg = reg | 0x80; //mask


	HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &reg, 1 , 100);
	HAL_SPI_TransmitReceive(&hspi1, &dammy, &val, 1, 100);
	//HAL_SPI_Receive(&hspi1, &val, 1, 100);
	HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin,GPIO_PIN_SET);

	return val;
}

void write_byte(uint8_t reg, uint8_t val){
	reg = reg & 0x7F; //mask

	HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &reg, 1 , 100);
	HAL_SPI_Transmit(&hspi1, &val, 1 , 100);
	HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin,GPIO_PIN_SET);

}

void myspi_init(){
	  HAL_GPIO_WritePin(LENC_CS_GPIO_Port, LENC_CS_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(RENC_CS_GPIO_Port, RENC_CS_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin,GPIO_PIN_SET);

}

void gyro_init(){
	//gyro_ref = 0.0;
	write_byte( PWR_MGMT_1    , 0x81 ); 	//Reset all register
	HAL_Delay(50);
	write_byte( USER_CTRL     , 0x10 );
	HAL_Delay(50);
	write_byte( PWR_MGMT_1    , 0x01 );		//set default value of 0x06
	HAL_Delay(50);

	write_byte( REG_BANK_SEL  , 0x20 );		//change user bank from bank0 to bank2
	HAL_Delay(50);
	write_byte( GYRO_CONFIG_1 , 0x0f );		//set low pass filter, Gyro_sensor_FS = 2000dps,
	HAL_Delay(50);
	write_byte( ACCEL_CONFIG  , 0x23 );		//set low pass filter, Accel_sensor_FS = ,
	HAL_Delay(50);
	write_byte( ACCEL_CONFIG_2, 0x00 );		//set low pass filter, Accel_sensor_FS = ,
	HAL_Delay(50);

	write_byte( REG_BANK_SEL  , 0x00 );		//change user bank from bank2 to bank0
	HAL_Delay(50);
	write_byte( PWR_MGMT_1    , 0x01 );		//set default value of 0x06,
	HAL_Delay(50);

	q0 = 1.0f;
	q1 = 0.0f;
	q2 = 0.0f;
	q3 = 0.0f;
}

void myspi_start(){
	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	  HAL_TIM_Base_Start_IT(&htim4);
}

void IMU_read_DMA(){
	HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin,GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive_DMA(&hspi1, &imu_address, imu_value, sizeof(imu_value)/sizeof(uint8_t));
}

void RENC_read_DMA(){
	HAL_GPIO_WritePin(RENC_CS_GPIO_Port, RENC_CS_Pin,GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive_DMA(&hspi1, renc_address, renc_value, sizeof(renc_value)/sizeof(uint8_t));

}

void LENC_read_DMA(){
	HAL_GPIO_WritePin(LENC_CS_GPIO_Port, LENC_CS_Pin,GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive_DMA(&hspi1, lenc_address, lenc_value, sizeof(lenc_value)/sizeof(uint8_t));

}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi){
	if(HAL_GPIO_ReadPin(GYRO_CS_GPIO_Port, GYRO_CS_Pin) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin,GPIO_PIN_SET);
		accel_data[x_axis] = (((int16_t)imu_value[1]<<8 ) | ( (int16_t)imu_value[2]&0x00ff ) );
		accel_data[y_axis] = (((int16_t)imu_value[3]<<8 ) | ( (int16_t)imu_value[4]&0x00ff ) );
		accel_data[z_axis] = (((int16_t)imu_value[5]<<8 ) | ( (int16_t)imu_value[6]&0x00ff ) );
		gyro_data[x_axis] = (((int16_t)imu_value[7]<<8 ) | ( (int16_t)imu_value[8]&0x00ff ) );
		gyro_data[y_axis] = (((int16_t)imu_value[9]<<8 ) | ( (int16_t)imu_value[10]&0x00ff ) );
		gyro_data[z_axis] = (((int16_t)imu_value[11]<<8 ) | ( (int16_t)imu_value[12]&0x00ff ) );
	}
	else if(HAL_GPIO_ReadPin(RENC_CS_GPIO_Port, RENC_CS_Pin) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(RENC_CS_GPIO_Port, RENC_CS_Pin,GPIO_PIN_SET);
	}
	else if(HAL_GPIO_ReadPin(LENC_CS_GPIO_Port, LENC_CS_Pin) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(LENC_CS_GPIO_Port, LENC_CS_Pin,GPIO_PIN_SET);
	}

}

uint16_t read_enc_R_count(){
	return	((uint16_t)(renc_value[0]) << 8 |(uint16_t)renc_value[1]) >> 4;
}

uint16_t read_enc_L_count(){
	return	((uint16_t)(lenc_value[0]) << 8 |(uint16_t)lenc_value[1]) >> 4;
}

float read_gyro_yaw(){
	return  (float)gyro_data[z_axis]*(-1.0f) / GYRO_FS_SEL_3;
}

float read_gyro_x_axis(){
	return  (float)gyro_data[x_axis]*(1.0f) / GYRO_FS_SEL_3;
}

float read_gyro_y_axis(){
	return  (float)gyro_data[y_axis]*(1.0f) / GYRO_FS_SEL_3;
}

float read_gyro_z_axis(){
	return  (float)gyro_data[z_axis]*(1.0f) / GYRO_FS_SEL_3;
}

float read_accel_x_axis(){
	return  (float)accel_data[x_axis] / ACCEL_FS_SEL_1 * 9.8;
}

float read_accel_y_axis(){
	return  (float)accel_data[y_axis] / ACCEL_FS_SEL_1 * 9.8;
}

float read_accel_z_axis(){
	return  (float)accel_data[z_axis] / ACCEL_FS_SEL_1 * 9.8;
}


//参考URL:http://watako-lab.com/2019/02/15/3axis_acc/
void calc_eular_from_accel(float delta_t){
	accel_2_theta_roll 	=	atan2f(read_accel_y_axis(),read_accel_z_axis());//atan2(y,x)
	accel_2_theta_pitch =	atan2f(-read_accel_x_axis(),sqrtf(read_accel_y_axis()*read_accel_y_axis()+read_accel_z_axis()*read_accel_z_axis()));
}

//参考URL:http://watako-lab.com/2019/02/28/3axis_gyro/
void calc_eular_from_gyro(float delta_t){
	gyro_2_delta_roll 	= 1.0f * read_gyro_x_axis()* PI / 180.0*delta_t + sinf(gyro_2_theta_roll)*sinf(gyro_2_theta_pitch)/cosf(gyro_2_theta_pitch)	*read_gyro_y_axis()* PI / 180.0*delta_t + cosf(gyro_2_theta_roll)*sinf(gyro_2_theta_pitch)/cosf(gyro_2_theta_pitch)	*read_gyro_z_axis()* PI / 180.0*delta_t;
	gyro_2_delta_pitch 	= 0.0f * read_gyro_x_axis()* PI / 180.0*delta_t + cosf(gyro_2_theta_roll)													*read_gyro_y_axis()* PI / 180.0*delta_t - sinf(gyro_2_theta_roll)													*read_gyro_z_axis()* PI / 180.0*delta_t;
	gyro_2_delta_yaw	= 0.0f * read_gyro_x_axis()* PI / 180.0*delta_t + sinf(gyro_2_theta_roll)*1.0f/cosf(gyro_2_theta_pitch)						*read_gyro_y_axis()* PI / 180.0*delta_t + cosf(gyro_2_theta_roll)*1.0/cosf(gyro_2_theta_pitch)						*read_gyro_z_axis()* PI / 180.0*delta_t;

	gyro_2_omega_roll 	= gyro_2_delta_roll/delta_t;
	gyro_2_omega_pitch 	= gyro_2_delta_pitch/delta_t;
	gyro_2_omega_yaw 	= gyro_2_delta_yaw/delta_t;

	gyro_2_theta_roll 	+= gyro_2_delta_roll;
	gyro_2_theta_pitch 	+= gyro_2_delta_pitch;
	gyro_2_theta_yaw 	+= gyro_2_delta_yaw;
}

//相補フィルターで適当に計算
void calc_eular_from_accel_and_gyro(float delta_t){
	float k = 0.1;
	gyro_2_delta_roll 	= 1.0f * read_gyro_x_axis()* PI / 180.0*delta_t + sinf(theta_roll)*sinf(theta_pitch)/cosf(theta_pitch)	*read_gyro_y_axis()* PI / 180.0*delta_t + cosf(theta_roll)*sinf(theta_pitch)/cosf(theta_pitch)	*read_gyro_z_axis()* PI / 180.0*delta_t;
	gyro_2_delta_pitch 	= 0.0f * read_gyro_x_axis()* PI / 180.0*delta_t + cosf(theta_roll)										*read_gyro_y_axis()* PI / 180.0*delta_t - sinf(theta_roll)										*read_gyro_z_axis()* PI / 180.0*delta_t;
	gyro_2_delta_yaw	= 0.0f * read_gyro_x_axis()* PI / 180.0*delta_t + sinf(theta_roll)*1.0f/cosf(theta_pitch)				*read_gyro_y_axis()* PI / 180.0*delta_t + cosf(theta_roll)*1.0/cosf(theta_pitch)				*read_gyro_z_axis()* PI / 180.0*delta_t;

	accel_2_theta_roll 	=	atan2f(read_accel_y_axis(),read_accel_z_axis());//atan2(y,x)
	accel_2_theta_pitch =	atan2f(-read_accel_x_axis(),sqrtf(read_accel_y_axis()*read_accel_y_axis()+read_accel_z_axis()*read_accel_z_axis()));

	omega_roll 	= gyro_2_delta_roll/delta_t;
	omega_pitch 	= gyro_2_delta_pitch/delta_t;
	omega_yaw 	= gyro_2_delta_yaw/delta_t;

	gyro_theta_roll 	+= gyro_2_delta_roll;
	gyro_theta_pitch 	+= gyro_2_delta_pitch;
	gyro_theta_yaw 		+= gyro_2_delta_yaw;

	theta_roll	= k*gyro_theta_roll		+ (1-k)*accel_2_theta_roll;
	theta_pitch = k*gyro_theta_pitch	+ (1-k)*accel_2_theta_pitch;
	theta_yaw	= gyro_theta_yaw;


}


volatile float        exInt, eyInt, ezInt;                   /* error integral */
static volatile float gx, gy, gz, ax, ay, az, mx, my, mz;
#define Kp 2.0f
#define Ki 0.01f

void imu_ahrs_update(void){
	float norm;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez, halfT;
	float tempq0,tempq1,tempq2,tempq3;

	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
	float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;

	gx = read_gyro_x_axis()*PI/180;
	gy = read_gyro_y_axis()*PI/180;
	gz = read_gyro_z_axis()*PI/180;
	ax = read_accel_x_axis();
	ay = read_accel_y_axis();
	az = read_accel_z_axis();
	mx = 0.0;
	my = 0.0;
	mz = 0.0;

	halfT       = (1.0) / 2000.0f;

	norm = sqrtf(ax*ax + ay*ay + az*az);
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

	hx = 2.0f*mx*(0.5f - q2q2 - q3q3) + 2.0f*my*(q1q2 - q0q3) + 2.0f*mz*(q1q3 + q0q2);
	hy = 2.0f*mx*(q1q2 + q0q3) + 2.0f*my*(0.5f - q1q1 - q3q3) + 2.0f*mz*(q2q3 - q0q1);
	hz = 2.0f*mx*(q1q3 - q0q2) + 2.0f*my*(q2q3 + q0q1) + 2.0f*mz*(0.5f - q1q1 - q2q2);
	bx = sqrt((hx*hx) + (hy*hy));
	bz = hz;

	vx = 2.0f*(q1q3 - q0q2);
	vy = 2.0f*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	wx = 2.0f*bx*(0.5f - q2q2 - q3q3) + 2.0f*bz*(q1q3 - q0q2);
	wy = 2.0f*bx*(q1q2 - q0q3) + 2.0f*bz*(q0q1 + q2q3);
	wz = 2.0f*bx*(q0q2 + q1q3) + 2.0f*bz*(0.5f - q1q1 - q2q2);

	ex = (ay*vz - az*vy) + (my*wz - mz*wy);
	ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
	ez = (ax*vy - ay*vx) + (mx*wy - my*wx);


	if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
	{
		exInt = exInt + ex * Ki * halfT;
		eyInt = eyInt + ey * Ki * halfT;
		ezInt = ezInt + ez * Ki * halfT;

		gx = gx + Kp*ex + exInt;
		gy = gy + Kp*ey + eyInt;
		gz = gz + Kp*ez + ezInt;
	}

	tempq0 = q0 + (-q1*gx - q2*gy - q3*gz) * halfT;
	tempq1 = q1 + (q0*gx + q2*gz - q3*gy) * halfT;
	tempq2 = q2 + (q0*gy - q1*gz + q3*gx) * halfT;
	tempq3 = q3 + (q0*gz + q1*gy - q2*gx) * halfT;

	/* normalise quaternion */
	norm = sqrtf(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
	q0 = tempq0 / norm;
	q1 = tempq1 / norm;
	q2 = tempq2 / norm;
	q3 = tempq3 / norm;


	theta_roll	= atan2f(2.0*q2*q3 + 2.0*q0*q1, -2.0*q1*q1 - 2.0*q2*q2 + 1.0);
	theta_pitch = -asinf(-2.0*q1*q3 + 2.0*q0*q2);
	theta_yaw	= -atan2f(2.0*q1*q2 + 2.0*q0*q3, -2.0*q2*q2 - 2.0*q3*q3 + 1.0);


}




