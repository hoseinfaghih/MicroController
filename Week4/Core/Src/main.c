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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBOUNCE_TIME 200
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
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
static void MX_TIM6_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t data[100];
uint8_t d;
uint8_t i = 0;
int digit[4] = {0};
int password[4];
int temp_digit = 0;
int current_digit = -1;
int log_on = 1;
int alert_on = 1;
int volume = 5;
int frequency = 5000;
int password_correct = 0;
int password_wrong = 0;
int password_timer = 0;
int wrong_password_count = 0;
int lock = 0;
int unlock = 0;
int lock_timer;
int alert_event = 0;// 1 correct password - 2 wrong password - 3 set password
int alert_event_timer = 0;
int alert_on_alert = 0;
int alert_on_alert_timer = 0;

TIM_HandleTypeDef *pwm_timer  = &htim2;
uint32_t pwm_channel = TIM_CHANNEL_2;

void PWM_Start(){
	HAL_TIM_PWM_Start(pwm_timer,pwm_channel);
}

void PWM_Change_Tone(uint16_t pwm_freq,uint16_t volume){
	if (pwm_freq == 0 || pwm_freq > 20000){
		__HAL_TIM_SET_COMPARE(pwm_timer,pwm_channel,0);
	}
	else{
		const uint32_t internal_clock_freq = HAL_RCC_GetSysClockFreq();
		const uint16_t prescaler = 1 + internal_clock_freq / pwm_freq / 60000;
		const uint32_t timer_clock = internal_clock_freq / prescaler;
		const uint32_t period_cycles = timer_clock / pwm_freq;
		const uint32_t pulse_width = volume * period_cycles / 1000 / 2;

		pwm_timer->Instance->PSC = prescaler - 1;
		pwm_timer->Instance->ARR = period_cycles - 1;
		pwm_timer->Instance->EGR = TIM_EGR_UG;
		__HAL_TIM_SET_COMPARE(pwm_timer,pwm_channel,pulse_width);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_12){ // pin 0 triggered
		static uint32_t last_enter_time = 0;
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12) && !lock){ // rising edge
			if (HAL_GetTick() - last_enter_time < DEBOUNCE_TIME){
				return;
			}
			else{
				last_enter_time = HAL_GetTick();
			}
		}
		else if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12) && !lock){ // falling edge
			if (HAL_GetTick() - last_enter_time < DEBOUNCE_TIME){// debounce
				return;
			}
			else { //short press
				last_enter_time = HAL_GetTick();
				digit[current_digit] = (digit[current_digit]+1)%10;
			}
		}

	}
	else if(GPIO_Pin == GPIO_PIN_13){
		static uint32_t last_enter_time_2 = 0;
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13) && !lock){ // rising edge
			if (HAL_GetTick() - last_enter_time_2 < DEBOUNCE_TIME){
				return;
			}
			else{
				last_enter_time_2 = HAL_GetTick();
			}
		}
		else if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13) && !lock){// falling edge
			if (HAL_GetTick() - last_enter_time_2 < DEBOUNCE_TIME){// debounce
				return;
			}
			else { //short press
				last_enter_time_2 = HAL_GetTick();
				if (current_digit == -1){
					current_digit = 0;
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
				}
				else if (current_digit <= 2 && current_digit >= 0){
					current_digit++;
				}
				else {
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
					current_digit = -1;
					check_password(digit[0] , digit[1] , digit[2] , digit[3]);
					digit[0] = 0;
					digit[1] = 0;
					digit[2] = 0;
					digit[3] = 0;
				}

			}
		}
	}
	else if (GPIO_Pin == GPIO_PIN_0){
		static uint32_t last_enter_time3 = 0;
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){ // rising edge
			if (HAL_GetTick() - last_enter_time3 < DEBOUNCE_TIME){
				return;
			}
			else{
				last_enter_time3 = HAL_GetTick();
			}
		}
		else { // falling edge
			if (HAL_GetTick() - last_enter_time3 < DEBOUNCE_TIME){// debounce
				return;
			}
			else { //short press
				last_enter_time3 = HAL_GetTick();
				lock = 0;
				unlock = 1;
			}
		}
	}
}
void check_password(int a1,int a2,int a3,int a4){
	if (a1==password[0] && a2 == password[1] && a3 == password[2] && a4 == password[3]){
		if (log_on){
			char log_str[] = "LOG (XXXX, CORRECT)\n";
			log_str[5] = (a1 + '0');
			log_str[6] = (a2 + '0');
			log_str[7] = (a3 + '0');
			log_str[8] = (a4 + '0');
			HAL_UART_Transmit(&huart1, log_str, strlen(log_str),HAL_MAX_DELAY);
		}
		password_correct = 1;
		wrong_password_count = 0;
		alert_event = 1;
	}
	else{
		if (log_on){
			char log_str[] = "LOG (XXXX, FAILED)\n";
			log_str[5] = (a1 + '0');
			log_str[6] = (a2 + '0');
			log_str[7] = (a3 + '0');
			log_str[8] = (a4 + '0');
			HAL_UART_Transmit(&huart1, log_str, strlen(log_str),HAL_MAX_DELAY);
		}
		password_wrong = 1;
		alert_event = 2;
		wrong_password_count++;
		if (wrong_password_count >= 3){
			lock = 1;
		}
	}
}
void shownumber (int x){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, x%2);
	x/=2;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, x%2);
	x/=2;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, x%2);
	x/=2;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, x%2);
}
void active_digits (){
	if (current_digit == temp_digit){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
	}
	else{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
	}
	if (temp_digit == 0){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0);
		shownumber(digit[0]);
		temp_digit = 1;
	}
	else if (temp_digit == 1){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0);
		shownumber(digit[1]);
		temp_digit = 2;
	}
	else if (temp_digit == 2){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0);
		shownumber(digit[2]);
		temp_digit = 3;
	}
	else if (temp_digit == 3){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1);
		shownumber(digit[3]);
		temp_digit = 0;
	}
}
void check_setpassword_command(int x){
	char error1[] = "ERROR(LENGTH DOESN'T MATCH)\n";
	char error2[] = "ERROR(DID YOU MEAN SET_PASS ?)\n";
	char error3[] = "ERROR(FORGET PARANTHESIS)\n";
	char error4[] = "ERROR(NON-NUMERIC-PASSWORD)\n";
	char ok1[] = "PASS_CHANGED(XXXX)\n";
	char check[] = "SET_PASS\n";
	if (x != 15){
		HAL_UART_Transmit(&huart1, error1, strlen(error1), HAL_MAX_DELAY);
	}
	else {
		if (strncmp(data,check,7)!= 0){
			HAL_UART_Transmit(&huart1, error2, strlen(error2), HAL_MAX_DELAY);
		}
		else{
			if (data[8] != '(' || data[13] != ')'){
				HAL_UART_Transmit(&huart1, error3, strlen(error3), HAL_MAX_DELAY);
			}
			else if (data[9]>='0' && data[9]<= '9' && data[10]>='0' && data[10]<= '9' && data[11]>='0' && data[11]<= '9' && data[12]>='0' && data[12]<= '9'){
				ok1[13] = data[9];
				ok1[14] = data[10];
				ok1[15] = data[11];
				ok1[16] = data[12];
				password[0] = (data[9] - '0');
				password[1] = (data[10] - '0');
				password[2] = (data[11] - '0');
				password[3] = (data[12] - '0');
				alert_event = 3;
				HAL_UART_Transmit(&huart1, ok1, strlen(ok1), HAL_MAX_DELAY);
			}
			else{
				HAL_UART_Transmit(&huart1, error4, strlen(error4), HAL_MAX_DELAY);
			}
		}
	}
}
void check_log_command (int x){
	char error1[] = "ERROR(LENGTH DOESN'T MATCH)\n";
	char error2[] = "ERROR(DID YOU MEAN LOG_ON ?)\n";
	char error3[] = "ERROR(DID YOU MEAN LOG_OFF?)\n";
	char check[] = "LOG_O";
	if (x == 7){
		if (strncmp (data,check,5) == 0 && data[5] == 'N'){
			char response[] = "LOG IS NOW ON \n";
			log_on = 1;
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else{
			HAL_UART_Transmit(&huart1, error2, strlen(error2), HAL_MAX_DELAY);
		}

	}
	else if ( x == 8){
		if (strncmp (data,check,5) == 0 && data[5] == 'F' && data[6] == 'F'){
			char response[] = "LOG IS NOW OFF \n";
			log_on = 0;
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else{
			HAL_UART_Transmit(&huart1, error3, strlen(error3), HAL_MAX_DELAY);
		}
	}
	else{
		HAL_UART_Transmit(&huart1, error1, strlen(error1), HAL_MAX_DELAY);
	}
}
void check_setvolume_command(int x){
	char error1[] = "ERROR(LENGTH DOESN'T MATCH)\n";
	char error2[] = "ERROR(VOLUME MUST BE IN RANGE(1,5))\n";
	char error3[] = "ERROR(NON NUMERIC VOLUME !)\n";
	char error4[] = "ERROR(YOU FORGET PARANTHESIS)\n";
	char error5[] = "ERROR(DID YOU MEAN SETVOLUME ?)\n";
	char check[] = "SET_VOLUME";
	if (x!=14){
		HAL_UART_Transmit(&huart1, error1, strlen(error1), HAL_MAX_DELAY);
	}
	else{
		if (strncmp(data,check,10) == 0){
			if (data[10] == '(' && data[12] ==')' && data[11] >='1' && data[11] <= '5'){
				int newvolume = data[11] - '0';
				char response[] = "VOLUME CHANGED TO X\n";
				response[18] = data[11];
				volume = newvolume;
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			}
			else if (data[10] == '(' && data[12] == ')' && data[11]>='6' && data[11] <= '9'){
				HAL_UART_Transmit(&huart1, error2, strlen(error2), HAL_MAX_DELAY);
			}
			else if (data[10] == '(' && data[12] == ')'){
				HAL_UART_Transmit(&huart1, error3, strlen(error3), HAL_MAX_DELAY);
			}
			else{
				HAL_UART_Transmit(&huart1, error4, strlen(error4), HAL_MAX_DELAY);
			}
		}
		else{
			HAL_UART_Transmit(&huart1, error5, strlen(error5), HAL_MAX_DELAY);
		}
	}
}
void check_setalert_command(int x){
	char error1[] = "ERROR(LENGTH DOESN'T MATCH)\n";
	char error2[] = "ERROR(NON NUMERIC VALUE OR FORGET PARANTHESIS!)\n";
	char error3[] = "ERROR(DID YOU MEAN SET_ALERT?)\n";
	char check[] ="SET_ALERT";
	if (x != 16){
		HAL_UART_Transmit(&huart1, error1, strlen(error1), HAL_MAX_DELAY);
	}
	else {
		if (strncmp(data,check,9) == 0){
			if (data[9] == '(' && data[14] == ')' && isdigit(data[10]) && isdigit(data[11]) && isdigit(data[12]) && isdigit(data[13]) ){
				int newfrequency = (data[10] - '0')*1000;
				newfrequency += (data[11] - '0')*100;
				newfrequency += (data[12] - '0')*10;
				newfrequency += (data[13] - '0');
				char response[] = "FREQUENCY SET TO XXXX\n";
				response[17] = data[10];
				response[18] = data[11];
				response[19] = data[12];
				response[20] = data[13];
				frequency = newfrequency;
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);

			}
			else{
				HAL_UART_Transmit(&huart1, error2, strlen(error2), HAL_MAX_DELAY);
			}
		}
		else{
			HAL_UART_Transmit(&huart1, error3, strlen(error3), HAL_MAX_DELAY);
		}
	}
}
void check_alert_command(int x){
	char error1[] = "ERROR(LENGTH DOESN'T MATCH)\n";
	char error2[] = "ERROR(DID YOU MEAN ALERT_ON ?)\n";
	char error3[] = "ERROR(DID YOU MEAN ALERT_OFF?)\n";
	char check[] = "ALERT_O";
	if (x == 9){
		if (strncmp (data,check,7) == 0 && data[7] == 'N'){
			char response[] = "ALERT IS NOW ON \n";
			alert_on = 1;
			alert_on_alert = 1;
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else{
			HAL_UART_Transmit(&huart1, error2, strlen(error2), HAL_MAX_DELAY);
		}

	}
	else if ( x == 10){
		if (strncmp (data,check,7) == 0 && data[7] == 'F' && data[8] == 'F'){
			char response[] = "ALERT IS NOW OFF \n";
			alert_on = 0;
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else{
			HAL_UART_Transmit(&huart1, error3, strlen(error3), HAL_MAX_DELAY);
		}
	}
	else{
		HAL_UART_Transmit(&huart1, error1, strlen(error1), HAL_MAX_DELAY);
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		HAL_UART_Receive_IT(&huart1, &d, 1);
		data[i++] = d;
		if (d == '\n') {
			data[i] = '\0';
			if (data[0] == 'S'){
				char check1[] = "SET_P";
				char check2[] = "SET_V";
				char check3[] = "SET_A";
				if (i < 6){
					char response[] = "ERROR(UNKNOWN-COMMAND1)\n";
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				}
				else if (strncmp(data,check1,5) == 0){
					check_setpassword_command(i);
				}
				else if (strncmp(data,check2,5) == 0){
					check_setvolume_command(i);
				}
				else if (strncmp(data,check3,5) == 0){
					check_setalert_command(i);
				}
				else{
					char response[] = "ERROR(UNKNOWN-COMMAND)\n";
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				}

			}
			else if(data[0] == 'L'){
				check_log_command(i);
			}
			else if (data[0] == 'A'){
				check_alert_command(i);
			}
			else{
				char response[] = "ERROR(UNKNOWN-COMMAND)\n";
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			}
			i = 0;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM6) {
		active_digits();

	}
	if (htim->Instance == TIM7){
		if (password_correct == 1){
			if (password_timer == 0 ){
				password_timer = HAL_GetTick();
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 1);
			}
			else if (HAL_GetTick() - password_timer >= 5000){
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,0);
				password_timer = 0;
				password_correct = 0;
			}
		}
		else if (password_wrong == 1){
			if (password_timer == 0 ){
				password_timer = HAL_GetTick();
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 1);
			}
			else if (HAL_GetTick() - password_timer >= 5000){
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,0);
				password_timer = 0;
				password_wrong = 0;
			}
			else{
				int idk = (HAL_GetTick()-password_timer);
				idk/=500;
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, idk%2);
			}
		}
	}
	if (htim->Instance == TIM16){
		if (lock){
			if (lock_timer == 0){
				lock_timer = HAL_GetTick();
				PWM_Change_Tone(frequency, volume*10);
			}
			else {
				int checksquare = (HAL_GetTick()-lock_timer)/1000;
				if (checksquare % 2 == 0){
					PWM_Change_Tone(frequency, volume*10);
				}
				else{
					PWM_Change_Tone(frequency*2, volume*10);
				}
			}
		}
		else if(unlock){
			PWM_Change_Tone(0, 0);
			unlock = 0;
		}
		else if (alert_on_alert){
			if (alert_on_alert_timer == 0){
				alert_on_alert_timer = HAL_GetTick();
				PWM_Change_Tone(3000, volume*10);
			}
			else if (HAL_GetTick() - alert_on_alert_timer >= 20000){
				alert_on_alert = 0;
				alert_on_alert_timer = 0;
				PWM_Change_Tone(0, 0);
			}
		}
		if (alert_on == 1){
			if (alert_event != 0 && alert_event_timer  == 0){
//				char res[] = "IM HERE\n";
//				HAL_UART_Transmit(&huart1, res, strlen(res), HAL_MAX_DELAY);
				alert_event_timer = HAL_GetTick();
				PWM_Change_Tone(500*alert_event, volume*10);
			}
			else if (alert_event != 0 && HAL_GetTick() - alert_event_timer >= 1000){
				PWM_Change_Tone(0, 0);
				alert_event = 0;
				alert_event_timer = 0;
			}
		}
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
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  MX_TIM2_Init();
  MX_TIM16_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start_IT(&htim16);
  HAL_UART_Receive_IT(&huart1, &d, 1);
  PWM_Start();
//  PWM_Change_Tone(500, 50);
//  HAL_Delay(1000);
//  PWM_Change_Tone(0, 0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	PWM_Change_Tone(440, 30);
//	HAL_Delay(1000);
//	PWM_Change_Tone(2440, 50);
//	HAL_Delay(1000);

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
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
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
  htim6.Init.Prescaler = 48-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000-1;
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
  htim7.Init.Prescaler = 48-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1000-1;
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
  htim16.Init.Prescaler = 48-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 1000-1;
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
  huart1.Init.BaudRate = 9600;
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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

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

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD1 PD2 PD3 PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
