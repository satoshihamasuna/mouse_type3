/*
 * myinterrupt.c
 *
 *  Created on: May 9, 2021
 *      Author: sato1
 */
#include "index.h"
#include "glob_var.h"
#include "mymacro.h"
#include "math.h"

const float delta_t = 0.0010;

void interrupt_start(){
	HAL_TIM_Base_Start_IT(&htim10);
}

void interrupt_main(){
	if(run_mode == STRAIGHT_MODE){
		tar_speed += accel * delta_t;
		tar_omega = 0.0; tar_angle = 0.0;
		tar_degree = tar_angle * 180.0/PI;
		if(tar_speed > max_speed)		tar_speed = max_speed;
	}
	else if(run_mode == TURN_MODE){

		tar_speed += accel * delta_t;
		if(tar_speed > max_speed)	tar_speed = max_speed;

		tar_omega += omega_accel * delta_t;
		tar_angle += tar_omega * delta_t;
		tar_degree = tar_angle * 180.0/PI;

		if(turn_dir == LEFT){
			if(tar_omega >= max_omega) {tar_omega = max_omega; omega_accel = 0.0; }
		}
		else if(turn_dir == RIGHT){
			if(tar_omega <= max_omega){ tar_omega = max_omega; omega_accel = 0.0; }
		}
	}

	if(run_mode == STRAIGHT_MODE || run_mode == DIALOG_MODE){
		if(con_wall.enable == true){
		con_wall.p_error = con_wall.error;

		if(run_mode == DIALOG_MODE){
			if(sen_r.error < 0) sen_r.error = 0;
			if(sen_l.error < 0) sen_l.error = 0;
		}

		if((sen_r.is_controll == true)&&(sen_l.is_controll == true)){
			con_wall.error = (float)(sen_r.error - sen_l.error);

		}
		else {
			con_wall.error = 2.0*(float)(sen_r.error - sen_l.error);
		}

		con_wall.diff = con_wall.error-con_wall.p_error;
		con_wall.sum += con_wall.error;

		con_wall.p_omg = con_wall.omg;
		con_wall.control =(CON_WALL_P_GAIN*con_wall.error + 0.0*CON_WALL_I_GAIN*con_wall.sum + 0.0*CON_WALL_D_GAIN*con_wall.diff);
		con_wall.omg = con_wall.control/10.0; //+ 0.9*con_wall.omg;

		tar_omega = con_wall.omg;//con_wall.p_omg;

		}
	}

	I_speed += speed;	I_tar_speed += tar_speed;
	I_omega += omega;	I_tar_omega += tar_omega;

	//PID制御
	V_r = V_l = 0;
	if(run_mode == STRAIGHT_MODE || run_mode == TURN_MODE  || run_mode == DIALOG_MODE){
	//speed control
		//V_r += 0.001*accel*3.8/Battery_check();//0.45
		//V_l -= 0.001*accel*3.8/Battery_check();//0.45

		V_r += (tar_speed - speed) * SPEED_P_GAIN;
		V_l -= (tar_speed - speed) * SPEED_P_GAIN;

		V_r += (I_tar_speed - I_speed) * SPEED_I_GAIN;
		V_l -= (I_tar_speed - I_speed) * SPEED_I_GAIN;

		V_r -= (p_speed - speed) * SPEED_I_GAIN;
		V_l += (p_speed - speed) * SPEED_I_GAIN;

	//omega control
		//V_r += (0.01*omega_accel*3.8/Battery_check())/1000.0;
		//V_l += (0.01*omega_accel*3.8/Battery_check())/1000.0;

		V_r += (tar_omega - omega) * OMEGA_P_GAIN;
		V_l += (tar_omega - omega) * OMEGA_P_GAIN;

		V_r += (I_tar_omega - I_omega) * OMEGA_I_GAIN;
		V_l += (I_tar_omega - I_omega) * OMEGA_I_GAIN;

		V_r -= (p_omega - omega) * OMEGA_D_GAIN;
		V_l -= (p_omega - omega) * OMEGA_D_GAIN;

	}

	if(run_mode == STRAIGHT_MODE || run_mode == TURN_MODE  || run_mode == DIALOG_MODE){
		if(ABS(V_r) > 5.0){
			motor_out_r = (int)(SIGN(V_r) * 5.0 * 180.0);
		}else{
			motor_out_r = (int)(V_r * 180.0);
		}
		Motor_Set_Duty_Right(motor_out_r);

		if(ABS(V_l) > 5.0){
			motor_out_l = (int)(SIGN(V_l) * 5.0 * 180.0);
		}else{
			motor_out_l = (int)(V_l * 180.0);
		}
		Motor_Set_Duty_Left(motor_out_l);

	}else if(run_mode == NON_CON_MODE){
		Motor_Set_Duty_Right(0);
		Motor_Set_Duty_Left(0);
	}else if(run_mode == ADJUST_MODE){
		Motor_Set_Duty_Right(200);
		Motor_Set_Duty_Left(200);
	}



	if(log_flag == 1 && log_count < LOG_COUNT){
		position_log[0][log_count] = enc_R.count;
		position_log[1][log_count] = enc_L.count;

		speed_log[0][log_count] = tar_speed;		speed_log[1][log_count] = speed;
		speed_log[2][log_count] = enc_R.speed;		speed_log[3][log_count] = enc_L.speed;

		omega_log[0][log_count] = tar_omega;		omega_log[1][log_count] = omega;

		sens_log[0][log_count] = sen_fr.value;		sens_log[1][log_count] = sen_fl.value;
		sens_log[2][log_count] = sen_r.value;		sens_log[3][log_count] = sen_l.value;

		sens_log2[0][log_count] = sen_r.c_value; 	sens_log2[1][log_count] = sen_l.c_value;
		sens_log2[2][log_count] = sen_r.cnt;		sens_log2[3][log_count] = sen_l.cnt;

		accel_log[x_axis][log_count] = read_accel_x_axis();
		accel_log[y_axis][log_count] = read_accel_y_axis();
		accel_log[z_axis][log_count] = read_accel_z_axis();

		duty_log[0][log_count] = motor_out_r;
		duty_log[1][log_count] = motor_out_l;
		length_log[log_count]  = len_mouse;

		log_count++;
	}
	check_error();
}

void get_speed(){

	enc_R.p_count = enc_R.count;	enc_R.count	= read_enc_R_count();
	enc_L.p_count = enc_L.count;	enc_L.count	= read_enc_L_count();

	enc_R.diff_pulse = (int32_t)enc_R.count - (int32_t)enc_R.p_count;
	if((enc_R.diff_pulse > ENC_CNT_HALF || enc_R.diff_pulse < (-1)*(ENC_CNT_HALF) ) && enc_R.p_count > ENC_CNT_HALF ){
		enc_R.diff_pulse = ((ENC_CNT_MAX) - enc_R.p_count) + enc_R.count;
	}
	else if((enc_R.diff_pulse > ENC_CNT_HALF || enc_R.diff_pulse < (-1)*(ENC_CNT_HALF) ) && enc_R.p_count <= ENC_CNT_HALF){
		enc_R.diff_pulse = (enc_R.count - ENC_CNT_MAX) - enc_R.p_count;
	}
	//enc_R.speed = 0.1*(float)enc_R.diff_pulse*MMPP+0.9*enc_R.speed;
	enc_R.speed = (float)enc_R.diff_pulse*MMPP;

	enc_L.diff_pulse = (int32_t)enc_L.count - (int32_t)enc_L.p_count;
	if((enc_L.diff_pulse > ENC_CNT_HALF || enc_L.diff_pulse < (-1)*(ENC_CNT_HALF) ) && enc_L.p_count > ENC_CNT_HALF ){
		enc_L.diff_pulse = ((ENC_CNT_MAX) - enc_L.p_count) + enc_L.count;
	}
	else if((enc_L.diff_pulse > ENC_CNT_HALF || enc_L.diff_pulse < (-1)*(ENC_CNT_HALF) ) && enc_L.p_count <= ENC_CNT_HALF){
		enc_L.diff_pulse = (enc_L.count - ENC_CNT_MAX) - enc_L.p_count;
	}
	//enc_L.speed = 0.1*(float)enc_L.diff_pulse*MMPP+0.9*enc_L.speed;
	enc_L.speed = (float)enc_L.diff_pulse*MMPP;
	p_speed = speed;p_omega = omega;
	speed =(enc_R.speed - enc_L.speed)/2.0; //(m/s) = (mm/ms)


	float degree_per_second = read_gyro_yaw();//(read_gyro_yaw()-gyro_ref)*k;
	omega = degree_per_second * PI / 180.0;	//(rad/s)

	degree += degree_per_second/1000.0;
	len_mouse += speed;

	//calc_eular
	//calc_eular_from_accel(delta_t);
	//calc_eular_from_gyro(delta_t);
	//calc_eular_from_accel_and_gyro(delta_t);
	//imu_ahrs_update();
	//photo sensor

	sen_fr.value 	= (int16_t)adc_value[0];
	sen_l.value 	= (int16_t)adc_value[1];
	sen_r.value 	= (int16_t)adc_value[2];
	sen_fl.value 	= (int16_t)adc_value[3];

	sen_r.ref = 1650;
	sen_l.ref = 1500;

	sen_r.th_control = sen_r.ref-400;
	sen_l.th_control = sen_r.ref-400;

	sen_fr.is_wall = (sen_fr.value >= TH_SEN_FR ) ? true : false;
	sen_fl.is_wall = (sen_fl.value >= TH_SEN_FL ) ? true : false;
	sen_r.is_wall  = (sen_r.value >= TH_SEN_R ) ? true : false;
	sen_l.is_wall  = (sen_l.value >= TH_SEN_L ) ? true : false;

	if(sen_r.is_wall == true && sen_fr.value > STOP_SEN_FR )	sen_r.is_wall = false;
	if(sen_l.is_wall == true && sen_fl.value > STOP_SEN_FL )	sen_l.is_wall = false;

	if(sen_r.is_wall == true)	sen_r.cnt++;
	else						sen_r.cnt = 0;

	if(sen_l.is_wall == true)	sen_l.cnt++;
	else						sen_l.cnt = 0;

	if(run_mode == TURN_MODE || run_mode == DIALOG_MODE){
		sen_l.cnt = 0;		sen_r.cnt = 0;
	}

	if(sen_r.value > sen_r.th_control){
		sen_r.error = sen_r.value - sen_r.ref;
		sen_r.is_controll = true;
		/*if((sen_r.value-sen_r.c_value) > 250 || (sen_r.value-sen_r.c_value) < -250){
			if(sen_r.error < 0)	sen_r.error = 0;
		}*/
	}
	else{
		sen_r.error = 0;
		sen_r.is_controll = false;
	}


	if(sen_l.value > sen_l.th_control){
		sen_l.error = sen_l.value - sen_l.ref;
		sen_l.is_controll = true;
		/*if((sen_l.value-sen_l.c_value) > 250 || (sen_l.value-sen_l.c_value) < -250){
			if(sen_l.error < 0)	sen_l.error = 0;
		}*/

	}
	else{
		sen_l.error = 0;
		sen_l.is_controll = false;
	}

	if(hosei_f == true && hosei_led_cnt < 81){
		hosei_led_cnt++;
		if(hosei_led_cnt % 20 == 0){HAL_GPIO_TogglePin(LED6_GPIO_Port, LED6_Pin);}
	}else if(is_mode_enable == true){
		//HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin,GPIO_PIN_SET);
	}

}


void check_error(){
		if((ABS(V_r) > 5.0 || ABS(V_l) > 5.0) && (run_mode == STRAIGHT_MODE || run_mode == TURN_MODE) && accel == 0.0){
			//error_cnt++;
		}

		if((ABS(read_accel_y_axis()) > 15.0 || ABS(read_accel_x_axis()) > 15.0 ) && (run_mode == STRAIGHT_MODE || run_mode == TURN_MODE || run_mode == DIALOG_MODE) && accel == 0.0){
			error_cnt = error_cnt + 10;
		}


		if((ABS(tar_omega - read_gyro_yaw()) > 1000.0 ) && (run_mode == STRAIGHT_MODE || run_mode == TURN_MODE || run_mode == DIALOG_MODE) && accel == 0.0){
			error_cnt = error_cnt + 50;
		}

		if(error_cnt > 50){
			MY_LED(0xff);
			Motor_Stop();
			write_save_data();
			NVIC_SystemReset();
		}

}

