/*
 * mytypedef.h
 *
 *  Created on: May 1, 2021
 *      Author: sato1
 */

#ifndef INC_MYTYPEDEF_H_
#define INC_MYTYPEDEF_H_



typedef enum{
	 false = 0,
	 true  = 1,
}t_bool;

typedef struct{
	uint16_t count;
	uint16_t p_count;
	int	 diff_pulse;
	float speed;
}t_encoder;

typedef struct{
	int16_t value;
	int16_t p_value;
	int16_t c_value;
	int16_t error;
	int16_t ref;
	int16_t th_control;
	int16_t cnt;
	t_bool is_wall;
	t_bool is_controll;
}t_sensor;

typedef struct{
	float p_omg;
	float omg;
	float control;
	float error;
	float p_error;
	float diff;
	float sum;
	t_bool enable;
}t_control;

typedef enum{
	front = 0,
	right = 1,
	rear  = 2,
	left  = 3,
}t_local_dir;

typedef struct{
	t_bool front;
	t_bool right;
	t_bool rear;
	t_bool left;
}t_local_wall;

typedef enum{
	north = 0,
	east  = 1,
	south = 2,
	west  = 3,
}t_direction;

typedef enum{
	x_axis = 0,
	y_axis = 1,
	z_axis = 2,
}t_axis;

typedef struct{
	short x;
	short y;
	t_direction dir;
}t_position;

typedef struct{
	unsigned char north : 2;
	unsigned char east  : 2;
	unsigned char south : 2;
	unsigned char west  : 2;
}t_wall;

typedef enum{
	LED_RF_PATTERN  = 3,
	LED_LF_PATTERN  = 2,
	LED_RS_PATTERN  = 1,
	LED_LS_PATTERN  = 0,
	BATTERY_PATTERN = 4,
}t_sens_pattern;

typedef enum{
	straight_HSTEP 	= 1,
	turn_r90		= 2,
	turn_l180		= 3,
	turn_l90		= 4,
	diagonal_r		= 5,
	diagonal_l		= 6,
	in_r45			= 7,
	out_r45			= 8,
	in_l45			= 9,
	out_l45			= 10,
	in_r135			= 11,
	out_r135		= 12,
	in_l135			= 13,
	out_l135		= 14,
	v_r90			= 15,
	v_l90			= 16,
	spin_r90		= 17,
	spin_l90		= 18,
	goal			= 0xff,
}t_run_pattern;


#endif /* INC_MYTYPEDEF_H_ */
