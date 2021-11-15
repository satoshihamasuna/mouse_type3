/*
 * run.c
 *
 *  Created on: 2021/05/16
 *      Author: sato1
 */


#include "index.h"
#include "glob_var.h"
#include "math.h"

#define CORRECTION_CNT 20
#define CORECTION_LENGTH 14.5

void extream_straight(){
	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;
}

void straight(float len_target,float acc,float max_sp,float end_sp){
	error_cnt = 0;
	con_wall.enable = false;
	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;


	tar_omega = 0.0;omega_accel = 0.0;

	run_mode = STRAIGHT_MODE;
	if(end_sp == 0.0){

		max_speed = max_sp;accel = acc;

		while((len_target-0.0)-len_mouse > 1000.0*(max_sp*max_sp)/(2.0*acc)){
			if(tar_speed >= max_speed){
				accel = 0.0;con_wall.enable = true;
			}
		}
		con_wall.enable = false;
		accel = -acc;
		while(len_mouse < len_target - 0.0){
			if(tar_speed < 0.05){
				accel = 0.0;	tar_speed = 0.05;
			}
		}
		accel = 0.0; max_speed = 0.0; tar_speed = 0.0;

		HAL_Delay(100);

	}else{
		//MY_LED(0x10);
		accel = acc;
		max_speed = max_sp;
		r_wall_check=0, l_wall_check=0, hosei_f=0; hosei_led_cnt = 0;
		/*
		if(len_target == SECTION){
			r_wall_check = sen_r.is_wall;
			l_wall_check = sen_l.is_wall;
		}
		if(len_target == HALF_SECTION || len_target == HALF_SECTION-10){
			r_wall_check = sen_r.is_wall;
			l_wall_check = sen_l.is_wall;
		}*/
		while((len_target-0.0)-len_mouse > 1000.0*(max_sp*max_sp - end_sp*end_sp)/(2.0*acc)){
			if(tar_speed >= max_speed){
				con_wall.enable = true;
				accel = 0.0;
			}
			if(len_target == SECTION){
		        if((sen_r.is_wall==false) && (r_wall_check==true) && (hosei_f==0)){
		            len_mouse = 56.0;//(len_mouse+56.0)/2;
		            hosei_f=1;
		        }
		        if((sen_l.is_wall==false) && (l_wall_check==true) && (hosei_f==0)){
		            len_mouse = 56.0;//(len_mouse+56.0)/2;
		            hosei_f=1;
		        }
			}
			if(len_target == HALF_SECTION || len_target == HALF_SECTION-10 ){
		        if((sen_r.is_wall==false) && (r_wall_check==true) && (hosei_f==0)){
		            len_mouse = 15.0;//(len_mouse+15.0)/2;
		            hosei_f=1;
		        }
		        if((sen_l.is_wall==false) && (l_wall_check==true) && (hosei_f==0)){
		            len_mouse = 15.0;//(len_mouse+15.0)/2;
		            hosei_f=1;
		        }
			}
			r_wall_check = sen_r.is_wall;
			l_wall_check = sen_l.is_wall;

		}
		con_wall.enable = false;
		accel = -acc;
		while(len_mouse < len_target){
			if(tar_speed <= end_sp){
				tar_speed = end_sp; accel = 0.0;
			}
		}

	}

	accel = 0.0;	len_mouse = 0.0;

}

void max_straight(float len_target,float acc,float max_sp,float end_sp){
	error_cnt = 0;
	con_wall.enable = false;
	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;


	tar_omega = 0.0;omega_accel = 0.0;

	run_mode = STRAIGHT_MODE;
	if(end_sp == 0.0){

		max_speed = max_sp;accel = acc;

		while((len_target-0.0)-len_mouse > 1000.0*(max_sp*max_sp)/(2.0*acc)){
			if(tar_speed >= max_speed){
				accel = 0.0;
			}
		}
		con_wall.enable = false;
		accel = -acc;
		while(len_mouse < len_target - 1.0){
			if(tar_speed < 0.05){
				accel = 0.0;	tar_speed = 0.05;
			}
		}
		accel = 0.0; max_speed = 0.0; tar_speed = 0.0;

		HAL_Delay(100);

	}else{
		//MY_LED(0x10);
		accel = acc;
		max_speed = max_sp;

		while((len_target-0.0)-len_mouse > 1000.0*(max_sp*max_sp - end_sp*end_sp)/(2.0*acc)){
			if(tar_speed >= max_speed){
				accel = 0.0;
			}
		}
		con_wall.enable = false;
		accel = -acc;
		while(len_mouse < len_target){
			if(tar_speed <= end_sp){
				tar_speed = end_sp; accel = 0.0;
			}
		}

	}

	accel = 0.0;	len_mouse = 0.0;

}

void STOP_WALL(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	con_wall.enable = false;

	tar_omega = 0.0;

	run_mode = STRAIGHT_MODE;

		max_speed = SEARCH_SPEED;accel = SEARCH_ACCEL;
		//con_wall.enable = true;

		while(sen_fr.value <= STOP_SEN_FR && sen_fl.value <= STOP_SEN_FL){
			if(tar_speed >= max_speed){
				accel = 0.0;

			}
		}
		con_wall.enable = false;

		accel = -SEARCH_ACCEL;
		float local_length = len_mouse;
		while(len_mouse - local_length < 15.0 + 7.5 - 1.0){
			if(tar_speed < 0.05){
				accel = 0.0;	tar_speed = 0.05;
			}
		}
		accel = 0.0; max_speed = 0.0; tar_speed = 0.0;

		HAL_Delay(100);

		accel = 0.0;	len_mouse = 0.0;

		run_mode = NON_CON_MODE;
}

void STOP_WALL2(int millis){
	error_cnt = 0;

	int cnt = 0;
	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	con_wall.enable = false;

	run_mode = TURN_MODE; 		turn_dir = LEFT;
	max_speed = SEARCH_SPEED;	max_omega = SEARCH_TURN_SPEED;

	float set_sp_err,set_sp_pre_err,set_sp_sum_err;
	float set_om_err,set_om_pre_err,set_om_sum_err;

	while(cnt < millis){
		error_cnt = 0;
		if(sen_fr.value >= STOP_SEN_FR && sen_fl.value >= STOP_SEN_FL){
			set_sp_pre_err = set_sp_err;	set_om_pre_err = set_om_err;
			set_sp_err = (float)(( SET_SEN_FR - sen_fr.value ) + ( SET_SEN_FL - sen_fl.value )) / 2.0 ;
			set_om_err = (float)(( SET_SEN_FR - sen_fr.value ) - ( SET_SEN_FL - sen_fl.value )) / 2.0 ;
			set_sp_sum_err += set_sp_err;	set_om_sum_err += set_om_err;

			tar_speed = 0.001 * set_sp_err + 0.0 * set_sp_sum_err + 0.0 * (set_sp_err-set_sp_pre_err);
			if(fabs(tar_speed) > 0.15) tar_speed = SIGN(tar_speed)*0.15;
			tar_omega = 0.01 * set_om_err + 0.0 * set_om_sum_err + 0.0 * (set_om_err-set_om_pre_err);
			if(fabs(tar_omega) > SEARCH_TURN_SPEED) tar_omega = SIGN(tar_omega)*SEARCH_TURN_SPEED;

		}else{
			tar_speed = 0.0;	tar_omega = 0.0;
		}

		cnt++;
		HAL_Delay(1);
	}



	HAL_Delay(100);

	accel = 0.0; 		max_speed = 0.0; tar_speed = 0.0;
	omega_accel = 0.0;	max_omega = 0.0; tar_omega = 0.0;
	len_mouse = 0.0;

	HAL_Delay(100);
	run_mode = NON_CON_MODE;
}

void turn_R720(float omega_acc,float max_omg){

	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	accel = 0.0; max_speed = 0.0; tar_speed = 0.0;
	omega_accel = 0.0; max_omega = 0.0; tar_omega = 0.0;
	float local_degree = 0.0;

	local_degree = degree;

	run_mode = TURN_MODE;	turn_dir = RIGHT;

	//符号　符号を変えないといけない
	omega_accel = -omega_acc; max_omega = -max_omg;

	while(((-720.0) - (degree - local_degree)) < -(max_omega*max_omega)/(2.0*omega_acc)*180.0/PI){
		if(tar_omega <= max_omega){
			//I_omega = 0.0;I_tar_omega = 0.0;
		}
	}

	omega_accel = omega_acc;

	while((degree - local_degree) > -720.0 + 0.0){
		if(tar_omega > -0.05*PI) {
			tar_omega = -0.05*PI;
			omega_accel = 0.0;
		}
	}

	omega_accel = 0.0; max_omega = 0.0; tar_omega = 0.0;
	sen_r.cnt = 0;sen_l.cnt = 0;
	HAL_Delay(200);

	accel = 0.0;	len_mouse = 0.0;

	run_mode = NON_CON_MODE;

}

void turn_R90(float omega_acc,float max_omg){

	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	accel = 0.0; max_speed = 0.0; tar_speed = 0.0;
	omega_accel = 0.0; max_omega = 0.0; tar_omega = 0.0;

	float local_degree = 0.0;

	local_degree = degree;

	run_mode = TURN_MODE;	turn_dir = RIGHT;

	//符号　符号を変えないといけない
	omega_accel = -omega_acc; max_omega = -max_omg;

	while(((-90.0) - (degree - local_degree)) < -(max_omega*max_omega)/(2.0*omega_acc)*180.0/PI){
		if(tar_omega <= max_omega){
			//I_omega = 0.0;I_tar_omega = 0.0;
		}
	}

	omega_accel = omega_acc;

	while((degree - local_degree) > -90.0 + 0.0){
		if(tar_omega > -0.05*PI) {
			tar_omega = -0.05*PI;
			omega_accel = 0.0;
		}
	}

	omega_accel = 0.0; max_omega = 0.0; tar_omega = 0.0;
	sen_r.cnt = 0;sen_l.cnt = 0;
	HAL_Delay(200);

	accel = 0.0;	len_mouse = 0.0;

	run_mode = NON_CON_MODE;

}



void turn_L90(float omega_acc,float max_omg){

	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	accel = 0.0; max_speed = 0.0; tar_speed = 0.0;
	omega_accel = 0.0; max_omega = 0.0; tar_omega = 0.0;

	float local_degree = 0.0;

	local_degree = degree;

	run_mode = TURN_MODE;	turn_dir = LEFT;

	omega_accel = omega_acc; max_omega = max_omg;

	//accel and const turn
	while(((90.0) - (degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI){
		if(tar_omega >= max_omega){
			//I_omega = 0.0;I_tar_omega = 0.0;
		}
	}

	omega_accel = -omega_acc;

	while((degree - local_degree) < 90.0 - 0.0){
		if(tar_omega < 0.05*PI) {
			tar_omega = 0.05*PI;
			omega_accel = 0.0;
		}
	}

	omega_accel = 0.0; max_omega = 0.0; tar_omega = 0.0;
	sen_r.cnt = 0;sen_l.cnt = 0;
	HAL_Delay(200);

	accel = 0.0;	len_mouse = 0.0;

	run_mode = NON_CON_MODE;

}

void turn_L180(float omega_acc,float max_omg){

	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	accel = 0.0; max_speed = 0.0; tar_speed = 0.0;
	omega_accel = 0.0; max_omega = 0.0; tar_omega = 0.0;

	float local_degree = 0.0;

	local_degree = degree;

	run_mode = TURN_MODE;	turn_dir = LEFT;

	omega_accel = omega_acc; max_omega = max_omg;

	//accel and const turn
	while(((180.0) - (degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI){
		if(tar_omega >= max_omega){
			//I_omega = 0.0;I_tar_omega = 0.0;
		}
	}

	omega_accel = -omega_acc;

	while((degree - local_degree) < 180.0 - 0.0){
		if(tar_omega < 0.05*PI) {
			tar_omega = 0.05*PI;
			omega_accel = 0.0;
		}
	}

	omega_accel = 0.0; max_omega = 0.0; tar_omega = 0.0;
	sen_r.cnt = 0;sen_l.cnt = 0;
	HAL_Delay(200);

	accel = 0.0;	len_mouse = 0.0;

	run_mode = NON_CON_MODE;
}

void sla_L90(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	accel = 0.0; max_speed = 0.3; tar_speed = 0.3;
	omega_accel = 0.0; max_omega = 0.0; tar_omega = 0.0;
	con_wall.enable = false;
	float local_length = 0.0;

	local_length = len_mouse;run_mode = STRAIGHT_MODE;
	con_wall.enable = true;
	sen_r.cnt = 0;sen_l.cnt = 0;

	accel = -SLA_SEARCH_ACCEL;
	if(sen_fr.is_wall == true && sen_fl.is_wall == true){
		while(sen_fr.value < STOP_SLA_FR || sen_fl.value < STOP_SLA_FL);
	}else{
		while((len_mouse - local_length)< 13.7){
			if(tar_speed <= SLA_SEARCH_SPEED){
				 max_speed = SLA_SEARCH_SPEED;	tar_speed = SLA_SEARCH_SPEED; 	accel = 0.0;
			}
		}
	}

	max_speed = SLA_SEARCH_SPEED;	tar_speed = SLA_SEARCH_SPEED; 	accel = 0.0;
	float local_degree = 0.0;	local_degree = tar_degree;//local_degree = degree;
	run_mode = TURN_MODE;	turn_dir = LEFT;
	omega_accel = SLA_SEARCH_TURN_ACCEL; max_omega = SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	//while(((90.0-0.0) - (degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);
	while(((90.0+SLA_L90_ERR) - (tar_degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = -omega_acc;

	while((tar_degree - local_degree) < 90.0 + SLA_L90_ERR ){
		if(tar_omega < 0.05*PI) {	tar_omega = 0.05*PI;	omega_accel = 0.0;	}
	}
	tar_omega = 0.0;	omega_accel = 0.0;

	con_wall.enable = true;
	sen_r.cnt = 0;sen_l.cnt = 0;
	local_length = len_mouse;run_mode = STRAIGHT_MODE;

	accel = 0.0; 		max_speed = SLA_SEARCH_SPEED;	tar_speed = SLA_SEARCH_SPEED;
	omega_accel = 0.0; 	max_omega = 0.0; 				tar_omega = 0.0;

	while((len_mouse - local_length)< 13.7);
	accel = 0.0; max_speed = SLA_SEARCH_SPEED;tar_speed = SLA_SEARCH_SPEED;
	len_mouse = 0.0;
	con_wall.enable = false;
	run_mode = STRAIGHT_MODE;

}

void sla_R90(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	con_wall.enable = false;

	accel = 0.0; max_speed = 0.3; tar_speed = 0.3;
	omega_accel = 0.0; max_omega = 0.0; tar_omega = 0.0;
	float local_length = 0.0;

	local_length = len_mouse;run_mode = STRAIGHT_MODE;

	con_wall.enable = true;
	sen_r.cnt = 0;sen_l.cnt = 0;


	if(sen_fr.is_wall == true || sen_fl.is_wall == true){
		while(sen_fr.value < STOP_SLA_FR || sen_fl.value < STOP_SLA_FL){
			if(tar_speed <= SLA_SEARCH_SPEED){
				 max_speed = SLA_SEARCH_SPEED;	tar_speed = SLA_SEARCH_SPEED; 	accel = 0.0;
			}
		}
		max_speed = SLA_SEARCH_SPEED;	tar_speed = SLA_SEARCH_SPEED; 	accel = 0.0;
	}else{
		while((len_mouse - local_length)< 13.7){
			if(tar_speed <= SLA_SEARCH_SPEED){
				 max_speed = SLA_SEARCH_SPEED;	tar_speed = SLA_SEARCH_SPEED; 	accel = 0.0;
			}
		}
	}

	max_speed = SLA_SEARCH_SPEED;	tar_speed = SLA_SEARCH_SPEED; 	accel = 0.0;

	float local_degree = 0.0;	local_degree = tar_degree;//local_degree = degree;

	run_mode = TURN_MODE;	turn_dir = RIGHT;

	omega_accel = -SLA_SEARCH_TURN_ACCEL; max_omega = -SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	//while(((-90.0+0.0) - (degree - local_degree)) < (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);
	while(((-90.0+SLA_R90_ERR) - (tar_degree - local_degree)) < (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = SLA_SEARCH_TURN_ACCEL;

	while((tar_degree - local_degree) > -90.0 + SLA_R90_ERR){
		if(tar_omega > -0.05*PI) {	tar_omega = -0.05*PI;	omega_accel = 0.0;	}
	}

	omega_accel = 0.0;	tar_omega = 0.0;	max_omega = 0.0;

	sen_r.cnt = 0;sen_l.cnt = 0;
	local_length = len_mouse;	run_mode = STRAIGHT_MODE;
	accel = 0.0; max_speed = SLA_SEARCH_SPEED;tar_speed = SLA_SEARCH_SPEED;
	while((len_mouse - local_length)< 13.7);

	len_mouse = 0.0;

	con_wall.enable = false;

	run_mode = STRAIGHT_MODE;


}

void long_turn_R90(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	r_wall_check=0, l_wall_check=0, hosei_f=0; hosei_led_cnt = 0;

	while((len_mouse - local_length)< 37.10){
		if((sen_r.is_wall==false) && (r_wall_check==true) && (hosei_f==0)){
			local_length = 0.0;
			len_mouse = 15.0;//(len_mouse+56.0)/2;
			hosei_f=1;
	    }
	    if((sen_l.is_wall==false) && (l_wall_check==true) && (hosei_f==0)){
	    	local_length = 0.0;
	    	len_mouse = 15.0;//(len_mouse+56.0)/2;
	    	hosei_f=1;
	    }
		r_wall_check = sen_r.is_wall;
		l_wall_check = sen_l.is_wall;
	}

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;		turn_dir = RIGHT;
	omega_accel = -SLA_SEARCH_TURN_ACCEL; max_omega = -SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((-90.0+SLA_L90_ERR) - (tar_degree - local_degree)) < (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = SLA_SEARCH_TURN_ACCEL;

	while((tar_degree - local_degree) > -90.0 ){
		if(tar_omega >- 0.05*PI) {	tar_omega = -0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	con_wall.enable = true;	sen_r.cnt = 0;	sen_l.cnt = 0;

	while((len_mouse - local_length)< 37.10);

	len_mouse = 0.0;	run_mode = STRAIGHT_MODE;
}

void long_turn_L90(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	r_wall_check=0, l_wall_check=0, hosei_f=0; hosei_led_cnt = 0;

	while((len_mouse - local_length)< 37.10){
		if((sen_r.is_wall==false) && (r_wall_check==true) && (hosei_f==0)){
			local_length = 0.0;
			len_mouse = 15.0;//(len_mouse+56.0)/2;
			hosei_f=1;
	    }
	    if((sen_l.is_wall==false) && (l_wall_check==true) && (hosei_f==0)){
	    	local_length = 0.0;
	    	len_mouse = 15.0;//(len_mouse+56.0)/2;
	    	hosei_f=1;
	    }
		r_wall_check = sen_r.is_wall;
		l_wall_check = sen_l.is_wall;
	}

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;	turn_dir = LEFT;
	omega_accel = SLA_SEARCH_TURN_ACCEL; max_omega = SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((90.0+SLA_L90_ERR) - (tar_degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = -omega_acc;

	while((tar_degree - local_degree) < 90.0 ){
		if(tar_omega < 0.05*PI) {	tar_omega = 0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	con_wall.enable = true;	sen_r.cnt = 0;	sen_l.cnt = 0;

	while((len_mouse - local_length)< 37.10);

	len_mouse = 0.0;	run_mode = STRAIGHT_MODE;
}

void long_turn_R180(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	//while((len_mouse - local_length)< 34.56);
	r_wall_check=0, l_wall_check=0, hosei_f=0; hosei_led_cnt = 0;

	while((len_mouse - local_length)< 34.56){
		if((sen_r.is_wall==false) && (r_wall_check==true) && (hosei_f==0)){
			local_length = 0.0;
			len_mouse = 15.0;//(len_mouse+56.0)/2;
			hosei_f=1;
	    }
	    if((sen_l.is_wall==false) && (l_wall_check==true) && (hosei_f==0)){
	    	local_length = 0.0;
	    	len_mouse = 15.0;//(len_mouse+56.0)/2;
	    	hosei_f=1;
	    }
		r_wall_check = sen_r.is_wall;
		l_wall_check = sen_l.is_wall;
	}


	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;		turn_dir = RIGHT;
	omega_accel = -SLA_SEARCH_TURN_ACCEL; max_omega = -LONG_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((-180.0+SLA_L90_ERR) - (tar_degree - local_degree)) < (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = SLA_SEARCH_TURN_ACCEL;

	while((tar_degree - local_degree) > -180.0 ){
		if(tar_omega >- 0.05*PI) {	tar_omega = -0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	con_wall.enable = true;	sen_r.cnt = 0;	sen_l.cnt = 0;

	while((len_mouse - local_length)< 34.56);

	len_mouse = 0.0;	run_mode = STRAIGHT_MODE;
}

void long_turn_L180(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	//while((len_mouse - local_length)< 34.56);
	r_wall_check=0, l_wall_check=0, hosei_f=0; hosei_led_cnt = 0;

	while((len_mouse - local_length)< 34.56){
		if((sen_r.is_wall==false) && (r_wall_check==true) && (hosei_f==0)){
			local_length = 0.0;
			len_mouse = 15.0;//(len_mouse+56.0)/2;
			hosei_f=1;
	    }
	    if((sen_l.is_wall==false) && (l_wall_check==true) && (hosei_f==0)){
	    	local_length = 0.0;
	    	len_mouse = 15.0;//(len_mouse+56.0)/2;
	    	hosei_f=1;
	    }
		r_wall_check = sen_r.is_wall;
		l_wall_check = sen_l.is_wall;
	}

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;	turn_dir = LEFT;
	omega_accel = SLA_SEARCH_TURN_ACCEL; max_omega = LONG_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((180.0+SLA_L90_ERR) - (tar_degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = -omega_acc;

	while((tar_degree - local_degree) < 180.0 ){
		if(tar_omega < 0.05*PI) {	tar_omega = 0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	con_wall.enable = true;	sen_r.cnt = 0;	sen_l.cnt = 0;

	while((len_mouse - local_length)< 34.56);

	len_mouse = 0.0;	run_mode = STRAIGHT_MODE;
}

void in_L45(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	r_wall_check=0, l_wall_check=0, hosei_f=0; hosei_led_cnt = 0;
	//while((len_mouse - local_length)< 15.3){
	while((len_mouse - local_length)< 17.94){
		if((sen_r.is_wall==false) && (r_wall_check==true) && (hosei_f==0)){
			local_length = 0.0;
			len_mouse = 15.0;//(len_mouse+56.0)/2;
			hosei_f=1;
	    }
	    if((sen_l.is_wall==false) && (l_wall_check==true) && (hosei_f==0)){
	    	local_length = 0.0;
	    	len_mouse = 15.0;//(len_mouse+56.0)/2;
	    	hosei_f=1;
	    }
		r_wall_check = sen_r.is_wall;
		l_wall_check = sen_l.is_wall;
	}

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;	turn_dir = LEFT;
	omega_accel = D45_TURN_ACCEL; max_omega = SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((45.0+SLA_L90_ERR) - (tar_degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = -omega_acc;

	while((tar_degree - local_degree) < 45.0 ){
		if(tar_omega < 0.05*PI) {	tar_omega = 0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = DIALOG_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	//while((len_mouse - local_length)< 34.48);
	while((len_mouse - local_length)< 37.05);

	len_mouse = 0.0;	run_mode = DIALOG_MODE;
}

void out_L45(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;	sen_r.cnt = 0;	sen_l.cnt = 0;

	local_length = len_mouse;	run_mode = DIALOG_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	while((len_mouse - local_length)< 34.48);

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;	turn_dir = LEFT;
	omega_accel = SLA_SEARCH_TURN_ACCEL; max_omega = SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((45.0+SLA_L90_ERR) - (tar_degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = -omega_acc;

	while((tar_degree - local_degree) < 45.0 ){
		if(tar_omega < 0.05*PI) {	tar_omega = 0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	while((len_mouse - local_length)< 15.3);

	len_mouse = 0.0;	run_mode = STRAIGHT_MODE;
}

void in_R45(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;//	sen_r.cnt = 0;	sen_l.cnt = 0;

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	r_wall_check=0, l_wall_check=0, hosei_f=0; hosei_led_cnt = 0;
	//while((len_mouse - local_length)< 15.3){
	while((len_mouse - local_length)< 17.94){
		if((sen_r.is_wall==false) && (r_wall_check==true) && (hosei_f==0)){
			local_length = 0.0;
			len_mouse = 15.0;//(len_mouse+56.0)/2;
			hosei_f=1;
	    }
	    if((sen_l.is_wall==false) && (l_wall_check==true) && (hosei_f==0)){
	    	local_length = 0.0;
	    	len_mouse = 15.0;//(len_mouse+56.0)/2;
	    	hosei_f=1;
	    }
		r_wall_check = sen_r.is_wall;
		l_wall_check = sen_l.is_wall;
	}

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;		turn_dir = RIGHT;
	omega_accel = -D45_TURN_ACCEL;; max_omega = -SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((-45.0+SLA_L90_ERR) - (tar_degree - local_degree)) < (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = D45_TURN_ACCEL;;

	while((tar_degree - local_degree) > -45.0 ){
		if(tar_omega >- 0.05*PI) {	tar_omega = -0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = DIALOG_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	//while((len_mouse - local_length)< 34.48);
	while((len_mouse - local_length)< 37.05);

	len_mouse = 0.0;	run_mode = DIALOG_MODE;
}

void out_R45(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;	sen_r.cnt = 0;	sen_l.cnt = 0;

	local_length = len_mouse;	run_mode = DIALOG_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	while((len_mouse - local_length)< 34.48);

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;		turn_dir = RIGHT;
	omega_accel = -SLA_SEARCH_TURN_ACCEL; max_omega = -SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((-45.0+SLA_L90_ERR) - (tar_degree - local_degree)) < (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = SLA_SEARCH_TURN_ACCEL;

	while((tar_degree - local_degree) > -45.0 ){
		if(tar_omega > -0.05*PI) {	tar_omega = -0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	while((len_mouse - local_length)< 15.3);

	len_mouse = 0.0;	run_mode = STRAIGHT_MODE;
}

void in_R135(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;	//sen_r.cnt = 0;	sen_l.cnt = 0;

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	r_wall_check=0, l_wall_check=0, hosei_f=0; hosei_led_cnt = 0;
	//while((len_mouse - local_length)< 24.64){
	while((len_mouse - local_length)< 24.64){
		if((sen_r.is_wall==false) && (r_wall_check==true) && (hosei_f==0)){
			local_length = 0.0;
			len_mouse = 15.0;//(len_mouse+56.0)/2;
			hosei_f=1;
	    }
	    if((sen_l.is_wall==false) && (l_wall_check==true) && (hosei_f==0)){
	    	local_length = 0.0;
	    	len_mouse = 15.0;//(len_mouse+56.0)/2;
	    	hosei_f=1;
	    }
		r_wall_check = sen_r.is_wall;
		l_wall_check = sen_l.is_wall;
	}

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;		turn_dir = RIGHT;
	omega_accel = -SLA_SEARCH_TURN_ACCEL; max_omega = -SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((-135.0+SLA_L90_ERR) - (tar_degree - local_degree)) < (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = SLA_SEARCH_TURN_ACCEL;

	while((tar_degree - local_degree) > -135.0 ){
		if(tar_omega >- 0.05*PI) {	tar_omega = -0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = DIALOG_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	while((len_mouse - local_length)< 16.75);

	len_mouse = 0.0;	run_mode = DIALOG_MODE;
}

void out_R135(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;	sen_r.cnt = 0;	sen_l.cnt = 0;

	local_length = len_mouse;	run_mode = DIALOG_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	while((len_mouse - local_length)< 16.75);

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;		turn_dir = RIGHT;
	omega_accel = -SLA_SEARCH_TURN_ACCEL; max_omega = -SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((-135.0+SLA_L90_ERR) - (tar_degree - local_degree)) < (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = SLA_SEARCH_TURN_ACCEL;

	while((tar_degree - local_degree) > -135.0 ){
		if(tar_omega >- 0.05*PI) {	tar_omega = -0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	while((len_mouse - local_length)< 24.64);

	len_mouse = 0.0;	run_mode = STRAIGHT_MODE;
}

void in_L135(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;//	sen_r.cnt = 0;	sen_l.cnt = 0;

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	r_wall_check=0, l_wall_check=0, hosei_f=0; hosei_led_cnt = 0;
	//while((len_mouse - local_length)< 24.64){
	while((len_mouse - local_length)< 24.64){
		if((sen_r.is_wall==false) && (r_wall_check==true) && (hosei_f==0)){
			local_length = 0.0;
			len_mouse = 15.0;//(len_mouse+56.0)/2;
			hosei_f=1;
	    }
	    if((sen_l.is_wall==false) && (l_wall_check==true) && (hosei_f==0)){
	    	local_length = 0.0;
	    	len_mouse = 15.0;//(len_mouse+56.0)/2;
	    	hosei_f=1;
	    }
		r_wall_check = sen_r.is_wall;
		l_wall_check = sen_l.is_wall;
	}


	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;	turn_dir = LEFT;
	omega_accel = SLA_SEARCH_TURN_ACCEL; max_omega = SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((135.0+SLA_L90_ERR) - (tar_degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = -omega_acc;

	while((tar_degree - local_degree) < 135.0 ){
		if(tar_omega < 0.05*PI) {	tar_omega = 0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = DIALOG_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	while((len_mouse - local_length)< 16.75);

	len_mouse = 0.0;	run_mode = DIALOG_MODE;
}

void out_L135(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;	sen_r.cnt = 0;	sen_l.cnt = 0;

	local_length = len_mouse;	run_mode = DIALOG_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;

	while((len_mouse - local_length)< 16.75);

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;	turn_dir = LEFT;
	omega_accel = SLA_SEARCH_TURN_ACCEL; max_omega = SLA_SEARCH_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((135.0+SLA_L90_ERR) - (tar_degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = -omega_acc;

	while((tar_degree - local_degree) < 135.0 ){
		if(tar_omega < 0.05*PI) {	tar_omega = 0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = STRAIGHT_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	while((len_mouse - local_length)< 24.64);

	len_mouse = 0.0;	run_mode = STRAIGHT_MODE;
}

void v_R90(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;	sen_r.cnt = 0;	sen_l.cnt = 0;

	local_length = len_mouse;	run_mode = TURN_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;
	int log_cnt = 0;	int flag = 0;
	//while((len_mouse - local_length)< 10.74);
	while((len_mouse - local_length)< 10.74+7.5){
		if(sen_fl.value > STOP_SEN_V90_FL) break;
		if(log_cnt > CORRECTION_CNT && sen_r.cnt == 0 && flag == 0){
			local_length = 0.0;len_mouse =  V90_STOP_SS; flag = 1;
		}
		if((len_mouse - local_length) > 15.5 && flag == 0 && sen_r.is_wall == false) break;
	}

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;		turn_dir = RIGHT;
	omega_accel = -V90_TURN_ACCEL; max_omega = -V90_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((-90.0+SLA_L90_ERR) - (tar_degree - local_degree)) < (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = V90_TURN_ACCEL;

	while((tar_degree - local_degree) > -90.0 ){
		if(tar_omega >- 0.05*PI) {	tar_omega = -0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = DIALOG_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	//while((len_mouse - local_length)< 10.74);
	while((len_mouse - local_length)< 10.74+7.5);
	len_mouse = 0.0;	run_mode = DIALOG_MODE;
}

void v_L90(){
	error_cnt = 0;

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	float local_length = 0.0;
	accel = 0.0;	max_speed = 0.5; 	tar_speed = 0.5;
	con_wall.enable = true;	sen_r.cnt = 0;	sen_l.cnt = 0;

	local_length = len_mouse;	run_mode = TURN_MODE;
	omega_accel = 0.0; 			max_omega = 0.0;
	int log_cnt = 0;	int flag = 0;
	//while((len_mouse - local_length)< 10.74);
	while((len_mouse - local_length)< 10.74+7.5){
		if(sen_fr.value > STOP_SEN_V90_FR) break;
		if(log_cnt > CORRECTION_CNT && sen_l.cnt == 0 && flag == 0){
			local_length = 0.0;len_mouse =  V90_STOP_SS; flag = 1;
		}
		if((len_mouse - local_length) > 15.5 && flag == 0 && sen_l.is_wall == false) break;
	}

	float local_degree = 0.0;	local_degree = tar_degree;
	run_mode = TURN_MODE;	turn_dir = LEFT;
	omega_accel = V90_TURN_ACCEL; max_omega = V90_TURN_SPEED;

	float omega_acc = omega_accel;

	while(((90.0+SLA_L90_ERR) - (tar_degree - local_degree)) > (max_omega*max_omega)/(2.0*omega_acc)*180.0/PI);

	omega_accel = -omega_acc;

	while((tar_degree - local_degree) < 90.0 ){
		if(tar_omega < 0.05*PI) {	tar_omega = 0.05*PI;	omega_accel = 0.0;	}
	}

	local_length = len_mouse;	run_mode = DIALOG_MODE;

	accel = 0.0; 		max_speed = 0.5;		tar_speed = 0.5;
	omega_accel = 0.0; 	max_omega = 0.0; 		tar_omega = 0.0;

	//while((len_mouse - local_length)< 10.74);
	while((len_mouse - local_length)< 10.74+7.5);
	len_mouse = 0.0;	run_mode = DIALOG_MODE;
}
void diagonal(float acc,float max_sp,float end_sp){
	error_cnt = 0;
	con_wall.enable = true;
	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;
	float len_target = DIAG_SECTION;
	tar_omega = 0.0;omega_accel = 0.0;

	run_mode = DIALOG_MODE;
	if(end_sp == 0.0){

		max_speed = max_sp;accel = acc;

		while((len_target-0.0)-len_mouse > 1000.0*(max_sp*max_sp)/(2.0*acc)){
			if(tar_speed >= max_speed){		accel = 0.0;	}
		}
		accel = -acc;
		while(len_mouse < len_target - 1.0){
			if(tar_speed < 0.05){	accel = 0.0;	tar_speed = 0.05;	}
		}
		accel = 0.0; max_speed = 0.0; tar_speed = 0.0;
		HAL_Delay(100);

	}else{
		//MY_LED(0x10);
		accel = acc;
		max_speed = max_sp;

		while((len_target-0.0)-len_mouse > 1000.0*(max_sp*max_sp - end_sp*end_sp)/(2.0*acc)){
			if(tar_speed >= max_speed){		accel = 0.0;	}
		}
		accel = -acc;
		while(len_mouse < len_target){
			if(tar_speed <= end_sp){	tar_speed = end_sp; accel = 0.0;	}
		}

	}

	accel = 0.0;	len_mouse = 0.0;
}

void enkaigei(){

	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	tar_speed = 0.0;	accel = 0.0; 		max_speed = 0.0;
	tar_omega = 0.0; 	omega_accel = 0.0; 	max_omega = 0.0;

	degree = 0.0;

	con_wall.enable = true;

	run_mode = STRAIGHT_MODE;	turn_dir = LEFT;

	while(!(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin))){
		error_cnt = 0;
	}

	run_mode = NON_CON_MODE;

}
