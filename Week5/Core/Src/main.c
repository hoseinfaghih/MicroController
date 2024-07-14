/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBOUNCE_TIME 100
#define PI 3.14159265
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart1;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM6_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC3_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM16_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t data[100];
uint8_t d;
uint8_t i = 0;
int state = 0;
char real_username[5];
char real_password[100];
char userinput_password[100];
int real_password_length = 0;
int real_password_ptr = 0;
int userinput_pass_ptr = 0;
int ldr_threshold = 500;
int is_day = 1;
int volume  = 500;
int wrong_input = 0;
TIM_HandleTypeDef *pwm_timer2  = &htim2;
TIM_HandleTypeDef *pwm_timer1 = &htim1;
uint32_t pwm_channel3 = TIM_CHANNEL_3;
uint32_t pwm_channel1 = TIM_CHANNEL_1;

int led3_last_enter_time  = 0;
int led3_intensity = 0;
int buz_last_enter_time = 0;
int freq = 0;
int buzzer_mode = 1;
int cnt_buzzer = 0;

void PWM_Start(){
	HAL_TIM_PWM_Start(pwm_timer2,pwm_channel3);
	HAL_TIM_PWM_Start(pwm_timer1,pwm_channel1);
}

void PWM_Change_Tone(uint16_t pwm_freq,uint16_t volume){
	if (pwm_freq == 0 || pwm_freq > 20000){
		__HAL_TIM_SET_COMPARE(pwm_timer2,pwm_channel3,0);
	}
	else{
		const uint32_t internal_clock_freq = HAL_RCC_GetSysClockFreq();
		const uint16_t prescaler = 1 + internal_clock_freq / pwm_freq / 60000;
		const uint32_t timer_clock = internal_clock_freq / prescaler;
		const uint32_t period_cycles = timer_clock / pwm_freq;
		const uint32_t pulse_width = volume * period_cycles / 1000 / 2;

		pwm_timer2->Instance->PSC = prescaler - 1;
		pwm_timer2->Instance->ARR = period_cycles - 1;
		pwm_timer2->Instance->EGR = TIM_EGR_UG;
		__HAL_TIM_SET_COMPARE(pwm_timer2,pwm_channel3,pulse_width);
	}
}

void PWM_Change_LED (uint16_t intensity){ // 0 to 1000
	if (intensity < 0 || intensity > 1000){
		return;
	}
	else{
	__HAL_TIM_SET_COMPARE(pwm_timer1,pwm_channel1,intensity);
	}
}

void state1_uart (int i){ // waiting for entering username for the first time
	if (i == 5){
		if (isalpha(data[0]) && isalpha(data[1]) && isalpha(data[2]) && isalpha(data[3])){
			real_username[0] = data[0];
			real_username[1] = data[1];
			real_username[2] = data[2];
			real_username[3] = data[3];
			real_username[4] = '\0';
			state = 2;
			char response[] = "NOW ENTER YOUR PASSWORD !\n";
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else{
			char error[] = "INVALID USERNAME! MUST BE ALPHABETIC \n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		char error[] = "INVALID USERNAME! MUST BE SIZE 4\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void state2_uart (int i){ // waiting for entering password for the first time
	if (i == 5){
		if (data[0] == 'd' && data[1] == 'o' && data[2] == 'n'  && data[3] == 'e'){
			if (real_password_ptr < 3){
				char error[] = "INVALID PASSWORD! MUST BE AT LEAST SIZE 3 - PLEASE TRY AGAIN\n";
				HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
				real_password_ptr = 0;
			}
			else{
				real_password_length = real_password_ptr;
				real_password[real_password_ptr] = '\0';
				char response[200];
				sprintf(response,"PASSWORD SET TO : %s\n\0", real_password);
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				state = 3;
				char response2[] = "MENU :\n1 - ON\n2 - OFF\n";
				HAL_UART_Transmit(&huart1, response2, strlen(response2), HAL_MAX_DELAY);
			}
		}
		else{
			char error[] = "INVALID COMMAND\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		char error[] = "INVALID COMMAND\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void state3_uart (int i){ // menu ; can on or off system
	if (i == 3){
		if (data[0] = 'O' && data[1] == 'N'){
			char response[] = "PLEASE ENTER USERNAME :\n";
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			state = 5;
		}
		else{
			char error[] = "INVALID COMMAND\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else if (i == 4){
		if (data[0] == 'O' && data[1] == 'F' && data[2] == 'F'){
			char response[] = "SYSTEM IS NOW OFF!\nFOR TURNING ON SYSTEM,ENTER COMMAND -ON\n";
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			state = 4;
		}
		else{
			char error[] = "INVALID COMMAND\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		char error[] = "INVALID COMMAND\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void state4_uart (int i){ // in off mode ; waiting for on mode
	if (i == 3){
		if (data[0] == 'O'  && data[1] == 'N'){
			state = 5;
			char response[] = "PLEASE ENTER USERNAME : \n";
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else{
			char error[] = "INVALID COMMAND\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		char error[] = "INVALID COMMAND\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void state5_uart (int i){ // entered on ; waiting for enter username
	if (i == 5){
		if (data[0] == real_username[0] && data[1] == real_username[1] && data[2] == real_username[2] && data[3] == real_username[3] ){
			char response[] = "PLEASE ENTER PASSWORD WITH BUTTONS THEN ENTER done :\n";
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			state = 6;
		}
		else{
			char error[] = "INVALID USERNAME\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		char error[] = "INVALID USERNAME\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void state6_uart (int i){ // entered on then entered correct username , now waiting for enter correct password
	if (i == 5){
		char check[] = "done";
		if ( strncmp (data,check,4) == 0){
			userinput_password[userinput_pass_ptr] = '\0';
			if (userinput_pass_ptr == real_password_length){
				if (strncmp (userinput_password,real_password,real_password_length) == 0){
					char response [] = "SYSTEM IS NOW ON ! ENJOY SECURITY !\n";
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
					state = 7;
					userinput_pass_ptr = 0;
				}
				else{
					userinput_password[userinput_pass_ptr] = '\0';
					userinput_pass_ptr = 0;
					char error [100] ;
					sprintf (error,"%s PASSWORD IS NOT CORRECT ! TRY AGAIN \n",userinput_password);
					HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
				}
			}
			else{
				userinput_password[userinput_pass_ptr] = '\0';
				userinput_pass_ptr = 0;
				char error [100] ;
				sprintf (error,"%s PASSWORD IS NOT CORRECT ! TRY AGAIN \n",userinput_password);
				HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
			}
		}
		else{
			char error[] = "INVALID COMMAND\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		char error[] = "INVALID COMMAND\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void state7_uart (int i) { // system is on ; not alert mode ;
	if (i == 4){
		if (data[0] == 'O' && data[1] == 'F' && data[2] == 'F'){
			char response[] = "SYSTEM IS NOW OFF \n";
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			state = 4;
		}
		else{
			char error[] = "INVALID-COMMAN\nD";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else if (i >= 17 && i <= 19){
		char check[] = "SET-THRESHOLD";
		if (strncmp (data,check,13) == 0){
			if (data[13] == '(' && data[i-2] == ')'){
				int flag = 0 ;
				for (int ii = 14 ; ii < i-2 ; ii++){
					if (!isdigit(data[ii])){
						flag = 1;
					}
				}
				if (flag){
					char error[] = "VALUE MUST BE NUMERIC !\n";
					HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
				}
				else{
					if (i == 17){
						int result = (data[14]-'0');
						ldr_threshold = result*10;
						char response[100];
						sprintf(response,"THRESHOLD SET TO %lu\n",result);
						HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
					}
					else if (i == 18){
						int result  = (data[14]-'0')*10;
						result += (data[15]-'0');
						ldr_threshold = result*10;
						char response[100];
						sprintf(response,"THRESHOLD SET TO %lu\n",result);
						HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
					}
					else{
						int result  = (data[14]-'0')*100;
						result += (data[15]-'0')*10;
						result += (data[16]-'0');
						if (result > 100){
							char error[] = "VALUE MUST BE IN RANGE (0,100)\n";
							HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
						}
						else{
							char response[100];
							sprintf(response,"THRESHOLD SET TO %lu\n",result);
							HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
							ldr_threshold = result * 10;
						}
					}
				}
			}
			else{
				char error[] = "FORGOT PARANTHESIS\n";
				HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
			}
		}
		else{
			char error[] = "INVALID-COMMAND\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		char error[] = "INVALID-COMMAND\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void state8_uart(int i){ // alert mode ; day ; waiting for username enter
	if (i == 5){
		if (data[0] == real_username[0] && data[1] == real_username[1] && data[2] == real_username[2] && data[3] == real_username[3] ){
			char response[] = "PLEASE ENTER PASSWORD WITH BUTTONS THEN ENTER done :\n";
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			state = 9;
		}
		else{
			wrong_input++;
			char error[] = "INVALID USERNAME\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		wrong_input++;
		char error[] = "INVALID USERNAME\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void state9_uart(int i){ // alert mode ; day  waiting for password
	if (i == 5){
		char check[] = "done";
		if ( strncmp (data,check,4) == 0){
			userinput_password[userinput_pass_ptr] = '\0';
			if (userinput_pass_ptr == real_password_length){
				if (strncmp (userinput_password,real_password,real_password_length) == 0){
					char response [] = "SYSTEM IS NOW ON ! ENJOY SECURITY !\n";
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
					state = 7;
					userinput_pass_ptr = 0;
					wrong_input = 0;
				}
				else{
					userinput_password[userinput_pass_ptr] = '\0';
					userinput_pass_ptr = 0;
					char error [100] ;
					sprintf (error,"%s PASSWORD IS NOT CORRECT ! TRY AGAIN \n",userinput_password);
					HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
					wrong_input++;
				}
			}
			else{
				userinput_password[userinput_pass_ptr] = '\0';
				userinput_pass_ptr = 0;
				char error [100] ;
				sprintf (error,"%s PASSWORD IS NOT CORRECT ! TRY AGAIN \n",userinput_password);
				HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
				wrong_input++;
			}
		}
		else{
			char error[] = "INVALID COMMAND\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		char error[] = "INVALID COMMAND\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void state10_uart (int i){
	if (i == 5){
		if (data[0] == real_username[0] && data[1] == real_username[1] && data[2] == real_username[2] && data[3] == real_username[3] ){
			char response[] = "PLEASE ENTER PASSWORD WITH BUTTONS THEN ENTER done :\n";
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			state = 11;
		}
		else{
			wrong_input++;
			char error[] = "INVALID USERNAME\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		wrong_input++;
		char error[] = "INVALID USERNAME\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void state11_uart (int i){
	if (i == 5){
		char check[] = "done";
		if ( strncmp (data,check,4) == 0){
			userinput_password[userinput_pass_ptr] = '\0';
			if (userinput_pass_ptr == real_password_length){
				if (strncmp (userinput_password,real_password,real_password_length) == 0){
					char response [] = "SYSTEM IS NOW ON ! ENJOY SECURITY !\n";
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
					state = 7;
					userinput_pass_ptr = 0;
					wrong_input = 0;
				}
				else{
					userinput_password[userinput_pass_ptr] = '\0';
					userinput_pass_ptr = 0;
					char error [100] ;
					sprintf (error,"%s PASSWORD IS NOT CORRECT ! TRY AGAIN \n",userinput_password);
					HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
					wrong_input++;
				}
			}
			else{
				userinput_password[userinput_pass_ptr] = '\0';
				userinput_pass_ptr = 0;
				char error [100] ;
				sprintf (error,"%s PASSWORD IS NOT CORRECT ! TRY AGAIN \n",userinput_password);
				HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
				wrong_input++;
			}
		}
		else{
			char error[] = "INVALID COMMAND\n";
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
	}
	else{
		char error[] = "INVALID COMMAND\n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		HAL_UART_Receive_IT(&huart1, &d, 1);
		data[i++] = d;
		if (d == '\n') {
			data[i] = '\0';
			if (state == 1){
				state1_uart(i);
			}
			else if (state == 2){
				state2_uart(i);
			}
			else if (state == 3){
				state3_uart(i);
			}
			else if (state == 4){
				state4_uart(i);
			}
			else if (state == 5){
				state5_uart(i);
			}
			else if (state == 6){
				state6_uart(i);
			}
			else if (state == 7){
				state7_uart(i);
			}
			else if (state == 8){
				state8_uart(i);
			}
			else if (state == 9){
				state9_uart(i);
			}
			else if (state == 10){
				state10_uart(i);
			}
			else if (state == 11){
				state11_uart(i);
			}
			i = 0;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_1){ // pin 1 triggered
		static uint32_t last_enter_time = 0;
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1)){ // rising edge
			if (HAL_GetTick() - last_enter_time < DEBOUNCE_TIME){
				return;
			}
			else{
				last_enter_time = HAL_GetTick();
			}
		}
		else { // falling edge
			if (HAL_GetTick() - last_enter_time < DEBOUNCE_TIME){// debounce
				return;
			}
			else { //short press
				last_enter_time = HAL_GetTick();
				if (state == 2){
					real_password[real_password_ptr] = '1';
					real_password_ptr++;
				}
				if (state == 6){
					userinput_password[userinput_pass_ptr] = '1';
					userinput_pass_ptr++;
				}

			}
		}

	}
	if(GPIO_Pin == GPIO_PIN_2){ // pin 2 triggered
		static uint32_t last_enter_time2 = 0;
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2)){ // rising edge
			if (HAL_GetTick() - last_enter_time2 < DEBOUNCE_TIME){
				return;
			}
			else{
				last_enter_time2 = HAL_GetTick();
			}
		}
		else{ // falling edge
			if (HAL_GetTick() - last_enter_time2 < DEBOUNCE_TIME){// debounce
				return;
			}
			else { //short press
				last_enter_time2 = HAL_GetTick();
				if (state == 2){
					real_password[real_password_ptr] = '2';
					real_password_ptr++;
				}
				if (state == 6){
					userinput_password[userinput_pass_ptr] = '2';
					userinput_pass_ptr++;
				}
			}
		}

	}
	if(GPIO_Pin == GPIO_PIN_3){ // pin 3 triggered
		static uint32_t last_enter_time3 = 0;
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3)){ // rising edge
			if (HAL_GetTick() - last_enter_time3 < DEBOUNCE_TIME){
				return;
			}
			else{
				last_enter_time3 = HAL_GetTick();
			}
		}
		else{ // falling edge
			if (HAL_GetTick() - last_enter_time3 < DEBOUNCE_TIME){// debounce
				return;
			}
			else { //short press
				last_enter_time3 = HAL_GetTick();
				if (state == 2){
					real_password[real_password_ptr] = '3';
					real_password_ptr++;
				}
				if (state == 6){
					userinput_password[userinput_pass_ptr] = '3';
					userinput_pass_ptr++;
				}
			}
		}

	}
	if (GPIO_Pin == GPIO_PIN_4){ // pin 4 triggered (PIR)
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)){
			if (state == 7 && is_day == 1){
				state = 8;
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
				char response [] = "SUSPICIOUS MOVEMENT !\nPLEASE ENTER USERNAME : \n";
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			}
			else if (state == 7 && is_day == 0){
				state = 10;
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
				char response [] = "SUSPICIOUS MOVEMENT !\nPLEASE ENTER USERNAME : \n";
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			}
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6){ // LED 1 -2
		if (state <= 5){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, 0);
		}
		else if (state == 6){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 0);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, 1);
		}
		else{
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 0);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, 0);
		}
	}
	if (htim->Instance == TIM7){ // LED3
		if (state == 7){
			if (HAL_GetTick() - led3_last_enter_time >= 500){
				if (led3_intensity == 1000){
					led3_intensity = 0;
					PWM_Change_LED(0);
					led3_last_enter_time = HAL_GetTick();
				}
				else{
					led3_last_enter_time = HAL_GetTick();
					led3_intensity = 1000;
					PWM_Change_LED(0);
				}
			}
		}
		if (state == 8 || state == 9){ // LED3
			if (HAL_GetTick() - led3_last_enter_time >= 300){
				if (led3_intensity < 1000){
					led3_intensity+=200;
					PWM_Change_LED(led3_intensity);
					led3_last_enter_time = HAL_GetTick();
				}
				else if ( led3_intensity < 2000){
					led3_intensity+=200;
					PWM_Change_LED(2000 - led3_intensity);
					led3_last_enter_time = HAL_GetTick();
				}
				else{
					PWM_Change_LED(0);
					led3_last_enter_time = HAL_GetTick();
				}
			}
		}
		else if (state == 10 || state == 11){
			if (led3_intensity < 1000){
				led3_intensity+=200;
				PWM_Change_LED(led3_intensity);
				led3_last_enter_time = HAL_GetTick();
			}
			else if ( led3_intensity < 2000){
				led3_intensity+=200;
				PWM_Change_LED(2000 - led3_intensity);
				led3_last_enter_time = HAL_GetTick();
			}
			else{
				PWM_Change_LED(0);
				led3_last_enter_time = HAL_GetTick();
			}

		}
		else{
			PWM_Change_LED(0);
		}
	}
	if (htim->Instance == TIM16){
//		if (state == 10 || state == 11){ // night-alert
//			if (buz_last_enter_time == 0){
//				buz_last_enter_time = HAL_GetTick();
//			}
//			if (buzzer_mode == 1){
//				if (HAL_GetTick() - buz_last_enter_time >= 5000){
//					buzzer_mode = 2;
//					buz_last_enter_time = HAL_GetTick();
//				}
//				int temp = (HAL_GetTick() - buz_last_enter_time)/500;
//				if (temp % 2 == 1){
//					PWM_Change_Tone(500, volume);
//				}
//				else{
//					PWM_Change_Tone(1000, volume);
//				}
//			}
//			else if (buzzer_mode == 2){
//				if (HAL_GetTick() - buz_last_enter_time >= 6000){
//					buzzer_mode = 3;
//					buz_last_enter_time = HAL_GetTick();
//					cnt_buzzer =0;
//				}
//				cnt_buzzer++;
//				if (cnt_buzzer >= 200){
//					cnt_buzzer = 0;
//				}
//				PWM_Change_Tone(cnt_buzzer*10, volume);
//			}
//			else if (buzzer_mode == 3){
//				if (HAL_GetTick() - buz_last_enter_time >= 6000){
//					buzzer_mode = 1;
//					buz_last_enter_time = HAL_GetTick();
//					cnt_buzzer = 0;
//				}
//				cnt_buzzer++;
//				PWM_Change_Tone(sin(cnt_buzzer*PI/180), volume);
//
//			}
//
//		}
		PWM_Change_Tone(1000, volume);
	}
}


void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef* hadc){
	if (hadc->Instance == ADC1){
		uint32_t value;
		value = HAL_ADC_GetValue(hadc);
		if (value < ldr_threshold){
			is_day = 0;
		}
		else{
			is_day = 1;
		}

	}
	if (hadc->Instance == ADC3){
//		uint32_t value2;
//		value2 = HAL_ADC_GetValue(hadc);
//		int temp = (value2/10)-2;
//		volume = (temp*10);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_TIM7_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */

  HAL_ADC_Start_IT(&hadc3);


  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start(&htim16);

  HAL_ADC_Start_IT(&hadc1);

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
  char setuser[] = "PLEASE SET YOUR USERNAME :\n";
  HAL_UART_Transmit(&huart1, setuser, strlen(setuser), HAL_MAX_DELAY);
  state = 1;
  HAL_UART_Receive_IT(&huart1, &d, 1);
  PWM_Start();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_TIM1
                              |RCC_PERIPHCLK_ADC12|RCC_PERIPHCLK_ADC34;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.Adc34ClockSelection = RCC_ADC34PLLCLK_DIV1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 4800-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 100-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 4800-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 100-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 4800-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 100-1;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 14400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : MEMS_INT4_Pin MEMS_INT1_Pin */
  GPIO_InitStruct.Pin = MEMS_INT4_Pin|MEMS_INT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD1 PD2 PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
