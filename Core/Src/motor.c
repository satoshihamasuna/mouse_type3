/*
 * motor.c
 *
 *  Created on: May 9, 2021
 *      Author: sato1
 */

#include "index.h"
#include "glob_var.h"

#define PCLK	(HAL_RCC_GetPCLK1Freq())
#define PWMFREQ		(40000)
#define DUTY_MAX	(950)
#define DUTY_MIN	(10)
#define MOT_SET_COMPARE_AIN1(x)	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,x)
#define MOT_SET_COMPARE_AIN2(x)	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,x)
#define MOT_SET_COMPARE_BIN1(x)	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,x)
#define MOT_SET_COMPARE_BIN2(x)	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,x)

void Motor_Start(){
	  HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_2);
	  HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_3);
	  HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_4);
}

void Motor_Set_Duty_Right(int16_t duty_r){
	uint32_t pulse_r;
	if(ABS(duty_r) > DUTY_MAX){
		pulse_r = (uint32_t)(PCLK/PWMFREQ*DUTY_MAX/1000)-1;
	}else if(ABS(duty_r) < DUTY_MIN){
		pulse_r = (uint32_t)(PCLK/PWMFREQ*DUTY_MIN/1000)-1;
	}else{
		pulse_r = (uint32_t)(PCLK/PWMFREQ*ABS(duty_r)/1000);
	}

	if(duty_r > 0){
		MOT_SET_COMPARE_BIN1(0);
		MOT_SET_COMPARE_BIN2(pulse_r);
	}else if(duty_r < 0){
		MOT_SET_COMPARE_BIN1(pulse_r);
		MOT_SET_COMPARE_BIN2(0);
	}else{
		MOT_SET_COMPARE_BIN1(0);
		MOT_SET_COMPARE_BIN2(0);
	}
}

void Motor_Set_Duty_Left(int16_t duty_l){
	uint32_t pulse_l;
	if(ABS(duty_l) > DUTY_MAX){
		pulse_l = (uint32_t)(PCLK/PWMFREQ*DUTY_MAX/1000)-1;
	}else if(ABS(duty_l) < DUTY_MIN){
		pulse_l = (uint32_t)(PCLK/PWMFREQ*DUTY_MIN/1000)-1;
	}else{
		pulse_l = (uint32_t)(PCLK/PWMFREQ*ABS(duty_l)/1000);
	}

	if(duty_l > 0){
		MOT_SET_COMPARE_AIN1(0);
		MOT_SET_COMPARE_AIN2(pulse_l);
	}else if(duty_l < 0){
		MOT_SET_COMPARE_AIN1(pulse_l);
		MOT_SET_COMPARE_AIN2(0);
	}else{
		MOT_SET_COMPARE_AIN1(0);
		MOT_SET_COMPARE_AIN2(0);
	}

}
void Motor_Stop(){
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
}
