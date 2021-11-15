/*
 * mymacro.h
 *
 *  Created on: May 1, 2021
 *      Author: sato1
 */

#ifndef INC_MYMACRO_H_
#define INC_MYMACRO_H_

#include "math.h"

//math
#define ABS(x)	((x < 0) ? -(x) : (x))
#define SIGN(x)	((x < 0) ? -1	:  1 )
#define PI		(3.141592)

//MAZE macro
#define MAZE_SIZE_X	16
#define MAZE_SIZE_Y 16

//GOAL Position Lower Left
#define MAZE_GOAL_X 7
#define MAZE_GOAL_Y 7

//GOAL SIZE NxN
#define MAZE_GOAL_SIZE 2

#define UNKNOWN	2
#define NOWALL	0
#define WALL	1
#define VWALL	3

#define RIGHT	0
#define LEFT	1
#define FRONT	2
#define REAR	3

#define MASK_SEARCH	0x01
#define MASK_SECOND	0x03

#define CONV_SEN2WALL(w)	((w)?WALL:NOWALL)

#define MODE_ENABLE	0x20


//wall parameter_1
#define TH_SENS_SIDE		500
#define TH_SENS_FRONT		300

#define REF_SEN_FR	717
#define REF_SEN_R	1325
#define REF_SEN_FL	656
#define REF_SEN_L	1650

#define TH_SEN_FR	450		//before 90mm+10mmぐらいのあたい
#define TH_SEN_R	700		//　左はじのとき
#define TH_SEN_FL	400		//before 90mm+10mmぐらいのあたい
#define TH_SEN_L	700		//右右はじのとき

#define SET_SEN_FR 	3495
#define SET_SEN_FL 	3550

#define STOP_SEN_FR 1650		//before 60mmの時のあたい
#define STOP_SEN_FL 2000		//before 60mmの時のあたい

#define STOP_SEN_V90_FL 1500
#define STOP_SEN_V90_FR 1500

#define STOP_SLA_FR	1000		//before 73mm位のあたい
#define STOP_SLA_FL 1150		//before 73mm位のあたい

#define WALL_CONTROL_CNT 30

#define SLA_R90_ERR 0.0
#define SLA_L90_ERR 0.0

#define V90_STOP_SS 15.0



//run parameter
#define SEARCH_ACCEL	(4.0)
#define SEARCH_SPEED	(0.30)	// (m/s)
#define HALF_SECTION	(45.0)	// (mm)
#define SECTION			(90.0)	// (mm)
#define DIAG_SECTION	(63.64)

#define NON_CON_MODE	0
#define STRAIGHT_MODE	1
#define TURN_MODE		2
#define DIALOG_MODE		3
#define ADJUST_MODE		4

//turn parameter
#define SEARCH_TURN_ACCEL	(80.0*PI)
#define SEARCH_TURN_SPEED	(4.0*PI)

#define V90_TURN_ACCEL		(100.0*PI)
#define V90_TURN_SPEED		(5.0*PI)

#define D45_TURN_ACCEL		(100.0*PI)

#define SLA_SEARCH_TURN_ACCEL	(80.0*PI)
#define SLA_SEARCH_TURN_SPEED	(4.0*PI)
#define LONG_TURN_SPEED			(3.6*PI)

#define SLA_SEARCH_SPEED	0.30
#define SLA_SEARCH_ACCEL    0.0

//#define SEARCH_TURN_ACCEL	(50.0*PI)
//#define SEARCH_TURN_SPEED	(4.0*PI)


//mouse parameter
#define ENC_RES_MAX     (4096)
#define ENC_RES_HALF    (2048)
#define ENC_CNT_MAX     (4096)
#define ENC_CNT_HALF    (2048)

#define TIRE_DIAMETER	14.0							// mm 13.9
#define TIRE_RADIUS		(TIRE_DIAMETER/2)				// mm
#define MMPP			(TIRE_DIAMETER*PI/ENC_RES_MAX)	// mm
#define TREAD_WIDTH		(18.0-2.0)						// mm

#define SPEED_P_GAIN 	15.0
#define SPEED_I_GAIN 	0.2//0.1
#define SPEED_D_GAIN 	0.0 //0.0

#define OMEGA_P_GAIN	0.6//0.8 //0.2
#define OMEGA_I_GAIN 	0.01//0.01
#define OMEGA_D_GAIN 	0.0

#define CON_WALL_P_GAIN 0.01
#define CON_WALL_I_GAIN 0.025
#define CON_WALL_D_GAIN 0.01
//#define CON_WALL_D_GAIN 0.0
//log
#define LOG_COUNT	1000


#define G_ACCEL		9.8

#endif /* INC_MYMACRO_H_ */
