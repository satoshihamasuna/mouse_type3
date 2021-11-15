/*
 * interface.c
 *
 *  Created on: May 9, 2021
 *      Author: sato1
 */

#include "index.h"
#include "glob_var.h"
#include "mymacro.h"

void MY_LED(uint8_t led_num){
	HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, (led_num >> 5) & 0x01);
    HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, (led_num >> 4) & 0x01);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, (led_num >> 3) & 0x01);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, (led_num >> 2) & 0x01);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, (led_num >> 1) & 0x01);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (led_num >> 0) & 0x01);
}

void check_LED(_Bool is_enable){
	switch(is_enable){
		case true	:
			HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, true);
			break;
		case false	:
			HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, false);
			break;
	}
}

void check_LED_toggle(int count){
	for(int i = 0 ;i < count*2 ; i++){
		HAL_GPIO_TogglePin(LED6_GPIO_Port, LED6_Pin);
		HAL_Delay(50);
	}
}

void read_button(){
	int is_button = (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin));

	if( is_button == true ){
		int push_time = 0; 	check_LED(false);
		while( HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) ){
			push_time++;	HAL_Delay(10);
			if(push_time > 100 && is_mode_enable == false)	check_LED(true);
		}
		if(is_mode_enable){
			is_mode_enable = false;
		}else{
			if(push_time <= 100)	mouse_mode = (mouse_mode == 31) ? 0 : mouse_mode + 1 ;
			else	is_mode_enable = true;
		}
	}
	MY_LED(((uint8_t)is_mode_enable) << 5 |mouse_mode);
}

void print_log(){
	printf("Spd_Kp,%2.4lf,Spd_Ki,%2.4lf,Spd_Kd,%2.4lf\n",SPEED_P_GAIN,SPEED_I_GAIN,SPEED_D_GAIN);	HAL_Delay(10);
	printf("Omg_Kp,%2.4lf,Omg_Ki,%2.4lf,Omg_Kd,%2.4lf\n\n\n",OMEGA_P_GAIN,OMEGA_I_GAIN,OMEGA_D_GAIN);	HAL_Delay(10);
	printf("time,tar_speed,speed,tar_omega,omega,");	HAL_Delay(10);
	printf("x_acc,y_acc,z_acc\n");	HAL_Delay(10);
	//printf("sens_fr,sens_fl,sens_r,sens_l\n"); 			HAL_Delay(10);
	for(int i = 0;i < LOG_COUNT;i++){
		printf("%4d,%.3lf,%.3lf,%.3lf,%.3lf,",i,speed_log[0][i],speed_log[1][i],omega_log[0][i],omega_log[1][i]);	HAL_Delay(10);
		printf("%.3lf,%.3lf,%.3lf\n",accel_log[0][i],accel_log[1][i],accel_log[2][i]);								HAL_Delay(10);
		//printf("%4d,%4d,%4d,%4d\n",sens_log[0][i],sens_log[1][i],sens_log[2][i],sens_log[3][i]); 					HAL_Delay(10);
	}
}
void print_log2(){
	printf("time,enc_R.count,enc_L.count,r_duty,l_duty\n");	HAL_Delay(10);
	for(int i = 0;i < LOG_COUNT;i++){
		printf("%4d,%4d,%4d,%d,%d\n",i,position_log[0][i],position_log[1][i],duty_log[0][i],duty_log[1][i]);	HAL_Delay(10);
	}
}

void print_irsens_log() {
	printf("sen_fr.value,sen_fl.value,sen_r.value,sen_l.value\n");
	for(int i = 0;i < LOG_COUNT;i++){
		printf("%4d,%f,%4d,%4d,%d,%d\n",i,length_log[i],sens_log[0][i],sens_log[1][i],sens_log[2][i],sens_log[3][i]);	HAL_Delay(10);
	}
}

void disp_map(){
	for( int y = MAZE_SIZE_Y - 1 ; y >= 0 ; y-- ){
		for(int x = 0; x < MAZE_SIZE_X ; x++ ){
			if(wall[x][y].north == WALL)	{	printf("+---");	HAL_Delay(10);	}
			else							{	printf("+   "); HAL_Delay(10);	}
			//if(x == MAZE_SIZE_X - 1)		{	printf("+\n");	HAL_Delay(5);	}
		}
		printf("+\n");	HAL_Delay(10);

		for(int x = 0; x < MAZE_SIZE_X ; x++ ){
			if(wall[x][y].west == WALL)		{	printf("|%3d",map[x][y]);	HAL_Delay(10);	}
			else							{	printf(" %3d",map[x][y]);	HAL_Delay(10);	}
			//if(x == MAZE_SIZE_X - 1)		{	printf("|\n");				HAL_Delay(5);	}
		}
		printf("|\n");				HAL_Delay(5);
	}
	for(int x = 0; x < MAZE_SIZE_X ; x++)	{	printf("+---"); HAL_Delay(5);	}	printf("+\n");
}

