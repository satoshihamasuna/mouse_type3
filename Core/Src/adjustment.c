/*
 * adjustment.c
 *
 *  Created on: 2021/10/18
 *      Author: sato1
 */


#include "index.h"
#include "glob_var.h"
#include "mymacro.h"

/*void adjustment_PID_speed(){
	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	tar_omega=0.0;
	len_mouse = 0.0;
	run_mode = STRAIGHT_MODE;
	accel = 0.0;
	max_speed = SEARCH_SPEED;
	tar_speed = SEARCH_SPEED;
	log_flag = true;log_count = 0;int cnt = 0;
	while(len_mouse < 90.0){	printf("sp:%lf,len:%lf\n",speed,len_mouse);	}
	while(cnt <= 600){	max_speed = 0.0;	tar_speed = 0.0;	cnt++;	HAL_Delay(2);	}
	run_mode = NON_CON_MODE;log_flag = false;log_count = 0;

}*/

void adjustment_wall(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	enkaigei();
	run_mode = NON_CON_MODE;log_flag = false;
}

void adjustment_PID_speed(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(SECTION*6+15.0,SEARCH_ACCEL*1.5,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}

void adjustment_PID_omega(){
	I_omega = 0.0;I_tar_omega = 0.0;
	I_speed = 0.0;I_tar_speed = 0.0;

	run_mode = TURN_MODE;

	accel = 0.0;	max_speed = 0.0;	tar_speed = 0.0;
	log_count = 0;log_flag = true;

	degree = 0.0;
	tar_omega = 4.5*PI;

	while( degree < 360.0 * 2.0);
	tar_omega = 0.0 ;

	HAL_Delay(400);

	run_mode = NON_CON_MODE;log_flag = false;

}

void adjustment_slalom_R(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(HALF_SECTION,SEARCH_ACCEL,0.3,0.3);
	sla_R90();
	straight(HALF_SECTION,SEARCH_ACCEL,0.3,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_L(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(HALF_SECTION,SEARCH_ACCEL,0.3,0.3);
	sla_L90();
	straight(HALF_SECTION,SEARCH_ACCEL,0.3,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_R1(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	diagonal(SEARCH_ACCEL,0.5,0.5);
	v_R90();
	diagonal(SEARCH_ACCEL,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_L1(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	diagonal(SEARCH_ACCEL,0.5,0.5);
	v_L90();
	diagonal(SEARCH_ACCEL,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_R2(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.5);
	long_turn_R90();
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_L2(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.5);
	long_turn_L90();
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_R3(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.5);
	long_turn_R180();
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_L3(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.5);
	long_turn_L180();
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_R4(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.5);
	in_R135();
	out_L135();
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_L4(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.5);
	in_L135();
	out_R135();
	straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_R5(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(HALF_SECTION,SEARCH_ACCEL*2,0.5,0.5);
	in_R45();
	out_L45();
	straight(HALF_SECTION,SEARCH_ACCEL*2,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
void adjustment_slalom_L5(){
	len_mouse = 0.0; degree = 0.0;
	log_flag = true;log_count = 0;
	straight(HALF_SECTION,SEARCH_ACCEL*2,0.5,0.5);
	in_L45();
	out_R45();
	straight(HALF_SECTION,SEARCH_ACCEL*2,0.5,0.0);
	run_mode = NON_CON_MODE;log_flag = false;
}
