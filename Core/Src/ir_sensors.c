/*
 * ir_sensors.c
 *
 *  Created on: May 9, 2021
 *      Author: sato1
 */
#include "index.h"
#include "glob_var.h"
#include "mymacro.h"
#include <stdio.h>
#include "math.h"

void ir_sensor_Initialize(){
	//HAL_GPIO_WritePin(photo_fr_GPIO_Port, photo_fr_Pin, 1);
	//HAL_GPIO_WritePin(photo_dl_GPIO_Port, photo_dl_Pin, 1);
	//HAL_GPIO_WritePin(photo_dr_GPIO_Port, photo_dr_Pin, 1);
	//HAL_GPIO_WritePin(photo_fl_GPIO_Port, photo_fl_Pin, 1);

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_value, 5);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	HAL_TIM_OC_Start_IT(&htim5, TIM_CHANNEL_2);
	HAL_TIM_OC_Start_IT(&htim5, TIM_CHANNEL_3);

}

void ir_sensor_interrupt(){
	static int i = 0;
/*
	HAL_GPIO_WritePin(photo_fr_GPIO_Port, photo_fr_Pin, i == 9);
	HAL_GPIO_WritePin(photo_dl_GPIO_Port, photo_dl_Pin, i == 1);
	HAL_GPIO_WritePin(photo_dr_GPIO_Port, photo_dr_Pin, i == 3);
	HAL_GPIO_WritePin(photo_fl_GPIO_Port, photo_fl_Pin, i == 5);
*/
///*
	HAL_GPIO_WritePin(photo_fr_GPIO_Port, photo_fr_Pin, i == 0);
	HAL_GPIO_WritePin(photo_dl_GPIO_Port, photo_dl_Pin, i == 2);
	HAL_GPIO_WritePin(photo_dr_GPIO_Port, photo_dr_Pin, i == 4);
	HAL_GPIO_WritePin(photo_fl_GPIO_Port, photo_fl_Pin, i == 6);
//*/

	//if(i == 9){
	//	  get_speed();
	//	  interrupt_main();
	//}
	i = i + 1;
	if(i == 10) i = 0;
}

float Battery_check(){
	return ((float)(adc_value[4]))/4096.0*3.3*2;
}
