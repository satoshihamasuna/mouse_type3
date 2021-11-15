/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <index.h>
#include <glob_var.h>
#include <ICM20648.h>
#include <mymacro.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	setbuf(stdout, NULL);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM5_Init();
  MX_TIM4_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */


  myspi_init();
  gyro_init();
  myspi_start();
  ir_sensor_Initialize();
  Motor_Start();
  interrupt_start();
  init_maze();
  int sx[1] = {0};		int sy[1] = {0};
  int gx[MAZE_GOAL_SIZE];	int gy[MAZE_GOAL_SIZE];
  for(int i = 0;i < MAZE_GOAL_SIZE;i++){
	  gx[i] = MAZE_GOAL_X + i;
	  gy[i] = MAZE_GOAL_Y + i;
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  read_button();
	  switch( ((uint8_t)is_mode_enable) << 5 | mouse_mode ){
	  	  case ( 0x00 | MODE_ENABLE ):
				  //Motor_Set_Duty_Right(300);Motor_Set_Duty_Left(-300);
	  			 printf("%d,%d,%d,%d\n",enc_R.diff_pulse,enc_L.diff_pulse,enc_R.count,enc_L.count);
	  	  	  	 HAL_Delay(10);
	  		  break;
	  	  case ( 0x01 | MODE_ENABLE ):
	  			 printf("%d,%d,%d,%d,%d\n",adc_value[0],adc_value[1],adc_value[2],adc_value[3],adc_value[4]);
	  	  	  	 HAL_Delay(10);
	  	  	  break;
	  	  case ( 0x02 | MODE_ENABLE ):
	  			 printf("%f,%f,%f,%f\n",read_gyro_yaw(),read_gyro_x_axis(),read_gyro_y_axis(),read_accel_z_axis());
	  	  	  	 HAL_Delay(10);
	  		  break;
	  	  case ( 0x03 | MODE_ENABLE ):
				 printf("%f,%f,%f\n",speed,omega,len_mouse);
				 HAL_Delay(10);
	  		  break;
	  	  case ( 0x04 | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value) > 1500*4  ){
					check_LED_toggle(5);
					len_mouse = degree =	0.0;
					mypos.x = mypos.y = 0;
					mypos.dir = north;
					straight(15.0,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
					search_adachi(gx, gy,MAZE_GOAL_SIZE);
					HAL_Delay(100);save_data();
					search_adachi_zenmen2(sx, sy,1);
					HAL_Delay(100);save_data();
					turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
					mypos.dir =(mypos.dir + 6) % 4;
					HAL_Delay(100);
					run_mode = NON_CON_MODE;log_flag = false;
					is_mode_enable = false;
				}

		      break;
	  	  case ( 0x05 | MODE_ENABLE ):
				if(	sen_fr.value + sen_l.value + sen_r.value + sen_fr.value	> 1500*4){
					check_LED_toggle(5);
					len_mouse = degree =	0.0;
					mypos.x = mypos.y = 0;
					mypos.dir = north;

					//straight(15.0,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
					saitan4(gx, gy,MAZE_GOAL_SIZE);
					HAL_Delay(100);	//save_data();
					saitan4(sx, sy,1);
					HAL_Delay(100); //save_data();

					turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
					mypos.dir =(mypos.dir + 6) % 4;
					HAL_Delay(100);

					is_mode_enable = false;
	  		  	}
		  	  break;
	  	  case ( 0x06 | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value)/4 > 2000 ){
					check_LED_toggle(5);
					len_mouse = degree =	0.0;
					mypos.x = mypos.y = 0;
					mypos.dir = north;

					//straight(15.0,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
					saitan5(gx, gy,MAZE_GOAL_SIZE);
					HAL_Delay(100);	//save_data();

					saitan3(sx, sy,1);
					HAL_Delay(100); //save_data();

					turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
					mypos.dir =(mypos.dir + 6) % 4;
					HAL_Delay(100);

					is_mode_enable = false;
				}
		  	  break;
	  	  case ( 0x07 | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value) > 1500*4 ){
					check_LED_toggle(5);
								  //disp_map();
					print_log();		HAL_Delay(10);
					print_irsens_log();	HAL_Delay(10);
					is_mode_enable = false;
				}
		  	  break;
	  	  case ( 0x08 | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value) > 1500*4   ){
					check_LED_toggle(5);
					len_mouse = degree =	0.0;
					mypos.x = mypos.y = 0;
					mypos.dir = north;
					straight(15.0,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
					search_adachi(gx, gy,MAZE_GOAL_SIZE);
					HAL_Delay(100);save_data();
					search_adachi(sx, sy,1);
					HAL_Delay(100);save_data();
					turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
					mypos.dir =(mypos.dir + 6) % 4;
					HAL_Delay(100);
					run_mode = NON_CON_MODE;log_flag = false;
					is_mode_enable = false;
				}
		  	  break;
	  	  case ( 0x09 | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value) > 1500*4  ){
					check_LED_toggle(5);
					len_mouse = degree =	0.0;
					mypos.x = mypos.y = 0;
					mypos.dir = north;
					saitan4(gx, gy,MAZE_GOAL_SIZE);
					HAL_Delay(100);save_data();
					search_adachi(sx, sy,1);
					HAL_Delay(100);save_data();
					turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
					mypos.dir =(mypos.dir + 6) % 4;
					HAL_Delay(100);
					run_mode = NON_CON_MODE;log_flag = false;
					is_mode_enable = false;
				}
		  	  break;
	  	  case ( 0x0A | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value) > 1500*4  ){
					check_LED_toggle(5);
					is_mode_enable = false;
				}
		  	  break;
	  	  case ( 0x0B | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value) > 1500*4  ){
					check_LED_toggle(5);
					len_mouse = degree =	0.0;
					mypos.x = mypos.y = 0;
					mypos.dir = north;
					saitan4(gx, gy,MAZE_GOAL_SIZE);
					HAL_Delay(100);save_data();
					search_adachi(sx, sy,1);
					HAL_Delay(100);save_data();
					turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
					mypos.dir =(mypos.dir + 6) % 4;
					HAL_Delay(100);
					run_mode = NON_CON_MODE;log_flag = false;
					is_mode_enable = false;
				}
		  	  break;
	  	  case ( 0x0C | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value) > 1500*4  ){
					check_LED_toggle(5);
					len_mouse = 0.0; degree = 0.0;
					log_flag = true;log_count = 0;
					STOP_WALL2(10000);
					run_mode = NON_CON_MODE;log_flag = false;
					is_mode_enable = false;
				}
		  	  break;
	  	  case ( 0x0D | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value) > 1500*4  ){
					check_LED_toggle(5);
					disp_map();
					is_mode_enable = false;

				}
		  	  break;
	  	  case ( 0x0E | MODE_ENABLE ):
				  if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value)/4 > 2000 ){
					check_LED_toggle(5);
					write_save_data();
					NVIC_SystemReset();
					HAL_Delay(10);
					is_mode_enable = false;
				  }
		  	  break;
	  	  case ( 0x0F | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value)/4 > 2000 ){
	 				check_LED_toggle(5);
	  				read_save_data();
	  				HAL_Delay(10);
	  				is_mode_enable = false;
				}
		  	  break;

	  	  case ( 0x10 | MODE_ENABLE ):
				if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value)/4 > 2000 ){
					check_LED_toggle(5);
					len_mouse = degree =	0.0;
					mypos.x = mypos.y = 0;
					mypos.dir = north;
					log_flag = true;log_count = 0;
					straight(15.0+SECTION,SEARCH_ACCEL,SEARCH_SPEED,0);
					HAL_Delay(100);

					run_mode = NON_CON_MODE;
					log_flag = false;
					is_mode_enable = false;
				}
		  	  break;
	  	  case ( 0x11 | MODE_ENABLE ):
	  			if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value)/4 > 2000 ){
	  		  		check_LED_toggle(5);
	  		  		adjustment_slalom_L2();
	  		  		is_mode_enable = false;
	  		  	}
		  	  break;
	  	  case ( 0x12 | MODE_ENABLE ):
	  			if( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value)/4 > 2000 ){
	  				check_LED_toggle(5);
	  				adjustment_slalom_R2();
	  		  		is_mode_enable = false;
	  		   	}
		  	  break;
	  	  case ( 0x13 | MODE_ENABLE ):
	  			  	//printf("****************************\n");
	  	  	  	  	//printf("gyro_roll->%4.2f,gyro_pitch->%4.2f,gyro_yaw->%4.2f\n",gyro_2_theta_roll,gyro_2_theta_pitch,gyro_2_theta_yaw);
					//printf("q0->%0.4f,q1->%0.4f,q2->%0.4f,q3->%0.4f\n",q0,q1,q2,q3);
	  		  		printf("roll->,%4.2f,pitch->,%4.2f,yaw->,%4.2f\n",theta_roll* 57.3,theta_pitch* 57.3,theta_yaw* 57.3);
	  		  	  	HAL_Delay(10);
		  	  break;
	  	  case ( 0x14 | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x15 | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x16 | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x17 | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x18 | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x19 | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x1A | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x1B | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x1C | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x1D | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x1E | MODE_ENABLE ):
		  	  break;
	  	  case ( 0x1F | MODE_ENABLE ):
		  	  break;
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int _write(int file, char *ptr, int len)
{
  HAL_UART_Transmit(&huart1,(uint8_t *)ptr,len,10);
  return len;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
