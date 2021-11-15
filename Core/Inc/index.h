/*
 * index.h
 *
 *  Created on: 2021/03/20
 *      Author: sato1
 */

#ifndef INC_INDEX_H_
#define INC_INDEX_H_

#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>
#include "mymacro.h"

//flash
void eraseALL();
void save_data();
void read_save_data();
void write_save_data();

//function of gyro and encoder
uint8_t read_byte( uint8_t reg );
void write_byte(uint8_t reg, uint8_t val);
void myspi_init();
void gyro_init();
void myspi_start();
void IMU_read_DMA();
void RENC_read_DMA();
void LENC_read_DMA();

float read_gyro_yaw();
float read_gyro_x_axis();
float read_gyro_y_axis();
float read_gyro_z_axis();
float read_accel_x_axis();
float read_accel_y_axis();
float read_accel_z_axis();

void calc_eular_from_accel(float delta_t);
void calc_eular_from_gyro(float delta_t);
void calc_eular_from_accel_and_gyro(float delta_t);
void imu_ahrs_update(void);

uint16_t read_enc_R_count();
uint16_t read_enc_L_count();

void print_irsens_log();
void ir_sensor_Initialize();
void ir_sensor_interrupt();
float Battery_check();

void MY_LED(uint8_t led_num);
void check_LED(_Bool is_enable);
void read_button();
void check_LED_toggle(int count);
void print_log();
void print_log2();
void disp_map();

void Motor_Start();
void Motor_Set_Duty_Right(int16_t duty_r);
void Motor_Set_Duty_Left(int16_t duty_l);
void Motor_Stop();

void interrupt_start();
void get_speed();
void interrupt_main();
float Battery_check();
//run
void straight(float len,float acc,float max_sp,float end_sp);
void max_straight(float len_target,float acc,float max_sp,float end_sp);

void STOP_WALL();
void STOP_WALL2(int millis);
void check_error();

//turnの関数は内部で方向を決める，大きさを引数にする
void turn_R90(float omega_acc,float max_omg);
void turn_L90(float omega_acc,float max_omg);
void turn_L180(float omega_acc,float max_omg);
void enkaigei();
void turn_R720(float omega_acc,float max_omg);
void sla_L90();
void sla_R90();
void in_R45();
void out_R45();
void in_L45();
void out_L45();
void in_R135();
void out_R135();
void in_L135();
void out_L135();
void v_R90();
void v_L90();
void long_turn_R90();
void long_turn_L90();
void long_turn_R180();
void long_turn_L180();
void diagonal(float acc,float max_sp,float end_sp);

//search
void init_maze(void);
void make_map(int *x, int *y,int size,int mask);
void search_lefthand();
//void search_adachi(int gx,int gy,int goal_size);
void search_adachi(int *gx,int *gy,int goal_size);
void search_adachi_zenmen(int *gx,int *gy,int goal_size);
void search_adachi_zenmen2(int *gx,int *gy,int goal_size);
void search_adachi_2(int *gx,int *gy,int goal_size,int mask);
void init_maze();
void saitan(int *gx,int *gy,int goal_size);
void saitan2(int *gx,int *gy,int goal_size);
void saitan3(int *gx,int *gy,int goal_size);
void saitan4(int *gx,int *gy,int goal_size);
void saitan5(int *gx,int *gy,int goal_size);

//adjustment
void adjustment_wall();
void adjustment_PID_speed();
void adjustment_PID_omega();
void adjustment_slalom_R();
void adjustment_slalom_L();
void adjustment_slalom_R1();
void adjustment_slalom_L1();
void adjustment_slalom_R2();
void adjustment_slalom_L2();
void adjustment_slalom_R3();
void adjustment_slalom_L3();
void adjustment_slalom_R4();
void adjustment_slalom_L4();
void adjustment_slalom_R5();
void adjustment_slalom_L5();

#endif /* INC_INDEX_H_ */
