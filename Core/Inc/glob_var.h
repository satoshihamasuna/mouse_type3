/*
 * glob_var.h
 *
 *  Created on: May 1, 2021
 *      Author: sato1
 */

#include "mytypedef.h"
#include "mymacro.h"

#ifndef INC_GLOB_VAR_H_
#define INC_GLOB_VAR_H_
#define GLOBAL
#else
#define GLOBAL extern
#endif


//maze
GLOBAL t_position mypos;
GLOBAL uint16_t map[MAZE_SIZE_X][MAZE_SIZE_Y];
GLOBAL t_wall wall[MAZE_SIZE_X][MAZE_SIZE_Y];
GLOBAL t_local_wall wall_inf;
GLOBAL t_bool dist_correct_FR;
GLOBAL t_sensor sen_r,sen_l,sen_fr,sen_fl;

GLOBAL uint16_t save_map[MAZE_SIZE_X][MAZE_SIZE_Y];
GLOBAL t_wall save_wall[MAZE_SIZE_X][MAZE_SIZE_Y];

//mouse
GLOBAL t_bool is_mode_enable;
GLOBAL uint8_t mouse_mode;

GLOBAL t_encoder enc_R,enc_L;

GLOBAL uint16_t adc_value[5];
GLOBAL uint16_t battery;

GLOBAL int16_t gyro_data[3];
GLOBAL int16_t accel_data[3];

GLOBAL int run_mode;
GLOBAL int turn_dir;

GLOBAL float len_mouse;	//(mm)

//speed 機体の中心速度[m/s]を取り扱う
GLOBAL float speed;	//(m/s)

GLOBAL float tar_speed;
GLOBAL float accel;
GLOBAL float max_speed;

GLOBAL float I_speed;
GLOBAL float I_tar_speed;

GLOBAL float p_speed;

//omega 角速度[rad/m]を取り扱う
GLOBAL float omega;	//センサからの入力
GLOBAL float omega_accel;	//自分で設定
//GLOBAL float omega_jerk;	//自分で設定

//GLOBAL float tar_omega_accel;
//GLOBAL float max_omega_accel;
GLOBAL float tar_omega;
GLOBAL float max_omega;
GLOBAL float tar_angle;
GLOBAL float tar_degree;

GLOBAL float I_omega;
GLOBAL float I_tar_omega;

GLOBAL float p_omega;

//degree 角度[°]を取り扱う
GLOBAL float degree;
GLOBAL float gyro_ref;


//テスト用
GLOBAL float accel_2_theta_roll;
GLOBAL float accel_2_theta_pitch;
GLOBAL float accel_2_theta_yaw;

GLOBAL float gyro_2_theta_roll;
GLOBAL float gyro_2_theta_pitch;
GLOBAL float gyro_2_theta_yaw;

GLOBAL float gyro_2_omega_roll;
GLOBAL float gyro_2_omega_yaw;
GLOBAL float gyro_2_omega_pitch;

GLOBAL float gyro_2_delta_roll;
GLOBAL float gyro_2_delta_yaw;
GLOBAL float gyro_2_delta_pitch;

GLOBAL float theta_roll;
GLOBAL float theta_pitch;
GLOBAL float theta_yaw;

GLOBAL float global_theta_roll;
GLOBAL float global_theta_pitch;
GLOBAL float global_theta_yaw;


GLOBAL float omega_roll;
GLOBAL float omega_yaw;
GLOBAL float omega_pitch;

GLOBAL float gyro_theta_roll;
GLOBAL float gyro_theta_pitch;
GLOBAL float gyro_theta_yaw;

GLOBAL float        q0;
GLOBAL float        q1;
GLOBAL float        q2;
GLOBAL float        q3;

//壁制御
GLOBAL t_control con_wall;

//PWM set variable
GLOBAL float V_r;
GLOBAL float V_l;
GLOBAL int motor_out_r;
GLOBAL int motor_out_l;


GLOBAL int error_cnt;

//log
GLOBAL t_bool log_flag;
GLOBAL int log_count;
GLOBAL int duty_log[2][LOG_COUNT];
GLOBAL int position_log[2][LOG_COUNT];
GLOBAL float speed_log[4][LOG_COUNT];	//0:tar_speed,1:speed
GLOBAL float omega_log[2][LOG_COUNT];	//0:tar_omega,1:omega
GLOBAL float accel_log[3][LOG_COUNT];
GLOBAL float length_log[LOG_COUNT];
GLOBAL uint16_t sens_log[4][LOG_COUNT];
GLOBAL int16_t sens_log2[4][LOG_COUNT];

//correction
t_bool r_wall_check, l_wall_check, hosei_f;
int hosei_led_cnt;

/* INC_GLOB_VAR_H_ */
