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
#include "LiquidCrystal.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
    uint16_t frequency;
    uint16_t duration;
} Tone;

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

const Tone pink_panther[] = {
	{REST, 900}, {REST, 450}, {REST, 225}, {NOTE_DS4, 225},
	{NOTE_E4, 675}, {REST, 225}, {NOTE_FS4, 225}, {NOTE_G4, 675},
	{REST, 225}, {NOTE_DS4, 225}, {NOTE_E4, 337}, {NOTE_FS4, 225},
	{NOTE_G4, 337}, {NOTE_C5, 225}, {NOTE_B4, 337}, {NOTE_B4, 225},
	{NOTE_G4, 337}, {NOTE_B4, 225}, {NOTE_AS4, 900}, {NOTE_A4, 168},
	{NOTE_G4, 168}, {NOTE_E4, 168}, {NOTE_D4, 168}, {NOTE_E4, 900},
	{REST, 450}, {REST, 225}, {NOTE_DS4, 450}, {NOTE_E4, 675},
	{REST, 225}, {NOTE_FS4, 225}, {NOTE_G4, 675}, {REST, 225},
	{NOTE_DS4, 225}, {NOTE_E4, 337}, {NOTE_FS4, 225}, {NOTE_G4, 337},
	{NOTE_C5, 225}, {NOTE_B4, 337}, {NOTE_G4, 225}, {NOTE_B4, 337},
	{NOTE_E5, 225}, {NOTE_DS5, 1800}, {NOTE_D5, 900}, {REST, 450},
	{REST, 225}, {NOTE_DS4, 225}, {NOTE_E4, 750}, {REST, 225},
	{NOTE_FS4, 225}, {NOTE_G4, 750}, {REST, 225}, {NOTE_DS4, 225},
	{NOTE_E4, 337}, {NOTE_FS4, 225}, {NOTE_G4, 337}, {NOTE_C5, 225},
	{NOTE_B4, 337}, {NOTE_E4, 225}, {NOTE_G4, 337}, {NOTE_B4, 225},
	{NOTE_AS4, 900}, {NOTE_A4, 168}, {NOTE_G4, 168}, {NOTE_E4, 168},
	{NOTE_D4, 168}, {NOTE_E4, 675}, {REST, 450}, {REST, 450},
	{NOTE_E5, 337}, {NOTE_D5, 225}, {NOTE_B4, 337}, {NOTE_A4, 225},
	{NOTE_G4, 337}, {NOTE_E4, 337}, {NOTE_AS4, 112}, {NOTE_A4, 337},
	{NOTE_AS4, 112}, {NOTE_A4, 337}, {NOTE_AS4, 112}, {NOTE_A4, 337},
	{NOTE_AS4, 112}, {NOTE_A4, 337}, {NOTE_G4, 168}, {NOTE_E4, 168},
	{NOTE_D4, 168}, {NOTE_E4, 112}, {NOTE_E4, 112}, {NOTE_E4, 900},
	{0, 1000},{0,0}
};
typedef unsigned char byte;
byte snake_right[] = {
  0x00,
  0x00,
  0x1F,
  0x1D,
  0x1F,
  0x1F,
  0x00,
  0x00
};

byte snake_left[] = {
  0x00,
  0x00,
  0x1F,
  0x17,
  0x1F,
  0x1F,
  0x00,
  0x00
};

byte snake_top[] = {
  0x00,
  0x00,
  0x1F,
  0x1B,
  0x1F,
  0x1F,
  0x00,
  0x00
};

byte snake_down[] = {
  0x00,
  0x00,
  0x1F,
  0x1F,
  0x1B,
  0x1F,
  0x00,
  0x00
};

byte snake_body[] = {
  0x00,
  0x00,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x00,
  0x00
};
byte apple[] = {
  0x03,
  0x04,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x00,
  0x00
};
byte med[] = {
  0x1F,
  0x15,
  0x15,
  0x1F,
  0x1F,
  0x15,
  0x15,
  0x1F
};
byte box[] = {
  0x10,
  0x1A,
  0x0C,
  0x1F,
  0x1B,
  0x11,
  0x1B,
  0x1F
};

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

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
static void MX_RTC_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
GPIO_TypeDef *const Row_ports[] = { GPIOD, GPIOD, GPIOD, GPIOD };
const uint16_t Row_pins[] = { GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1, GPIO_PIN_0 };
GPIO_TypeDef *const Column_ports[] = { GPIOD, GPIOD, GPIOD, GPIOD };
const uint16_t Column_pins[] =
    { GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7 };
volatile uint32_t last_gpio_exti;

int temp_digit = 0;
int digit[4];

int program_mode = 0;
int counter = 0;
int first_time_change = 0;
int last_rtc_entertime = 0;
char name[10];
int snake_default_speed = 1;
int default_health = 5;
int obstacle_count = 3;
int sound_effect = 1;
int game_mode = 1;

int map [4][20];
int snake_path [79][2];
int snake_length = 4;
int direction = 1;
int last_enter_game;
int health;
int speed;
int score = 0;
int time_spent = 0;
int last_enter_timer = 0;
int apple_exist = 1;
int apple_exist_timer;
int medical_exist = 1;
int medical_exist_timer;
int box_exist = 1;
int box_exist_timer;
int flag_voice_apple , flag_voice_health , flag_voice_box;
int last_speed_change;

TIM_HandleTypeDef *pwm_timer  = &htim2;
uint32_t pwm_channel = TIM_CHANNEL_2;
const Tone *volatile melody_ptr;
volatile uint16_t melody_tone_count;
volatile uint16_t current_tone_number;
volatile uint32_t current_tone_end;

uint8_t data[100];
uint8_t d;
uint8_t ii = 0;

void remove_spaces(const char* input, char* output) {
    int non_space_count = 0;
    for (int i = 0; input[i] != '\0'; ++i) {
        if (!isspace(input[i])) {
            output[non_space_count++] = input[i];
        }
    }
    output[non_space_count] = '\0';
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		HAL_UART_Receive_IT(&huart1, &d, 1);
		data[ii++] = d;
		if (d == '\n') {
			data[ii] = '\0';
            char set_name_command[] = "name=";
            char speed_command[] = "speed=";// 1-3
            char health_command[] = "health="; // 1-9
            char obstacle_command[] = "obstacles="; // up to 6
            char music_command[] = "music="; //0-1
            char game_mode_command[] = "game_mode="; //1-3

            if (strncmp(data,game_mode_command,10) == 0 && ii == 12 && data[10] <= '3' && data[10] >= '1'){
            	game_mode = (data[10] - '0');
            	char response[] = "GAME MODE CHANGED SUCCESSFULLY !\n";
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
            }
            else if (strncmp(data,music_command,6) == 0 && ii == 8 && data[6] >= '0' && data[6] <= '1'){
            	sound_effect = (data[6] - '0');
            	char response[] = "MUSIC EFFECT CHANGED SUCCESSFULLY !\n";
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
            }
            else if (strncmp (data,obstacle_command,10) == 0 && ii == 12 && data[10] >= '1' && data[10] <= '6'){
            	obstacle_count = (data[10] - '0');
            	char response[] = "OBSTACLE COUNT CHANGED SUCCESSFULLY !\n";
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
            }
            else if (strncmp(data,health_command,7) == 0 && ii == 9 && data[7] >= '1' && data[7] <= '9'){
            	default_health = (data[7] - '0');
            	char response[] = "DEFAULT HEALTH CHANGED SUCCESSFULLY !\n";
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
            }
            else if (strncmp(data,set_name_command,5) == 0 && ii <= 14){
                strncpy(name, data + 5, sizeof(name) - 1);
                // name[sizeof(name) - 1] = '\0';  // Ensure null-termination
                char response[] = "NAME SET SUCCESSFULLY !\n";
                HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
            }
            else if (strncmp(data,speed_command,6) == 0 && ii == 8 && data[6] >= '1' && data[6] <= '3'){
            	snake_default_speed = (data[6] - '0');
            	char response[] = "Default Speed CHANGED SUCCESSFULLY !\n";
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
            }
            else{
            	char response[] = "[ERROR][UNKNOWN-COMMAND]\n";
            	HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
            }
			ii = 0;
		}
	}
}

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
void Play_Music (){
	Change_Melody(pink_panther,  sizeof(pink_panther) / sizeof(pink_panther[0]));
}
void Change_Melody(const Tone *melody,uint32_t tone_count){
	melody_ptr = melody;
	melody_tone_count = tone_count;
	current_tone_number = 0;
}
void Update_Melody(){
	if ((HAL_GetTick() > current_tone_end) && (current_tone_number < melody_tone_count)){
		const Tone active_tone = *(melody_ptr + current_tone_number);
		PWM_Change_Tone(active_tone.frequency, 20);
		current_tone_end = HAL_GetTick() + active_tone.duration;
		current_tone_number++;
	}
	else if (current_tone_number >= melody_tone_count && melody_tone_count != 0){
		Play_Music();
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if (last_gpio_exti + 200 > HAL_GetTick()) // Simple button debouncing
  {
    return;
  }
  last_gpio_exti = HAL_GetTick();

  int8_t row_number = -1;
  int8_t column_number = -1;

  if (GPIO_Pin == GPIO_PIN_0)
  {
    // blue_button_pressed = 1;
    // return;
  }

  for (uint8_t row = 0; row < 4; row++) // Loop through Rows
  {
    if (GPIO_Pin == Row_pins[row])
    {
      row_number = row;
    }
  }

  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 0);
  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 0);
  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 0);
  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 0);

  for (uint8_t col = 0; col < 4; col++) // Loop through Columns
  {
    HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 1);
    if (HAL_GPIO_ReadPin(Row_ports[row_number], Row_pins[row_number]))
    {
      column_number = col;
    }
    HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 0);
  }

  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 1);
  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 1);
  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 1);
  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 1);

  if (row_number == -1 || column_number == -1)
  {
    return; // Reject invalid scan
  }
  //   C0   C1   C2   C3
  // +----+----+----+----+
  // | 1  | 2  | 3  | 4  |  R0
  // +----+----+----+----+
  // | 5  | 6  | 7  | 8  |  R1
  // +----+----+----+----+
  // | 9  | 10 | 11 | 12 |  R2
  // +----+----+----+----+
  // | 13 | 14 | 15 | 16 |  R3
  // +----+----+----+----+
  const uint8_t button_number = row_number * 4 + column_number + 1;
  switch (button_number)
  {
  case 1:
    if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
    	program_mode = 1;
    }
    break;
  case 2:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
	if (program_mode == 4){
		if (counter == 0){
			default_health++;
			if(default_health == 10){
				default_health = 1;
			}
		}
		if (counter == 1){
			snake_default_speed++;
			if (snake_default_speed == 4){
				snake_default_speed = 1;
			}
		}
		if (counter == 2){
			obstacle_count++;
			if (obstacle_count > 6){
				obstacle_count = 1;
			}
		}
		if (counter == 3){
			sound_effect = 1 ;
		}
	}
	if (program_mode == 5){
		if (direction ==1 || direction == 3){
			direction = 4;
		}
	}
    break;
  case 3:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
	else if (program_mode == 1){
		counter--;
		if (counter == -1){
			counter = 3;
		}
	}
	else if (program_mode == 3){
		game_mode--;
		if (game_mode == 0){
			game_mode = 3;
		}
	}
	else if (program_mode == 4){
		counter--;
		if (counter == -1){
			counter = 3;
		}
	}
    break;
  case 4:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
    break;
  case 5:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
	if (program_mode == 5){
		if (direction == 2 || direction == 4){
			direction = 3;
		}
	}
    break;
  case 6:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
	if (program_mode == 4){
		if (counter == 0){
			default_health--;
			if (default_health == 0){
				default_health = 9;
			}
		}
		if (counter == 1){
			snake_default_speed--;
			if (snake_default_speed == 0){
				snake_default_speed = 3;
			}
		}
		if (counter == 2){
			obstacle_count--;
			if (obstacle_count < 1){
				obstacle_count = 6;
			}
		}
		if (counter == 3){
			sound_effect = 0 ;
		}
	}
    break;
  case 7:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
	else if (program_mode == 1){
		counter++;
		if (counter == 4){
			counter = 0;
		}
	}
	else if (program_mode == 3){
		game_mode++;
		if (game_mode == 4){
			game_mode = 1;
		}
	}
	else if (program_mode == 4){
		counter++;
		if (counter == 4){
			counter = 0;
		}
	}
	else if (program_mode == 5){
		if (direction == 2 || direction == 4){
			direction = 1;
		}
	}
    break;
  case 8:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
    break;
  case 9:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
    break;
  case 10:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
	if (program_mode == 5){
		if (direction == 1 || direction == 3){
			direction = 2;
		}
	}
    break;
  case 11:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
	else if (program_mode == 1){
		if (counter == 3){
			program_mode = 2;
			first_time_change = 1;
			counter = 0;
		}
		else if (counter == 2){
			program_mode = 3;
			first_time_change = 1;
			counter = 0 ;
		}
		else if (counter == 1){
			program_mode = 4;
			first_time_change = 1;
			counter = 0;
		}
		else if (counter == 0){
			program_mode = 5;
			first_time_change = 1;
		}
	}
    break;
  case 12:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
    break;
  case 13:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
    break;
  case 14:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
    break;
  case 15:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
	if (program_mode == 2){
		program_mode = 1;
		first_time_change = 1;
		counter = 0;
	}
	if (program_mode == 3){
		program_mode = 1;
		first_time_change = 1;
		counter = 0;
	}
	if (program_mode == 4){
		program_mode = 1;
		first_time_change = 1;
		counter = 0;
	}
    break;
  case 16:
	if (program_mode == 0){
    	counter = 0;
    	first_time_change = 1;
		program_mode = 1;
	}
    break;

  default:
    break;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if (htim->Instance == TIM6){
		if (program_mode == 5 && game_mode == 3 && HAL_GetTick() - last_speed_change >= 15000){
			last_speed_change = HAL_GetTick();
			speed += 0.2;
		}
		if (program_mode == 5 && health == 0){
			program_mode = 6;
			first_time_change = 1;
		}
		if (HAL_GetTick() - last_enter_timer >= 60000 && program_mode == 5){
			time_spent++;
			last_enter_timer = HAL_GetTick();
		}
		tokenize();
		active_digits();
	}

	if (htim->Instance == TIM7){
		if (program_mode == 5){
			if (apple_exist == 0){
				apple_exist_timer = HAL_GetTick();
				apple_exist = 2;
				if (sound_effect){
					PWM_Change_Tone(500, 20);
					flag_voice_apple = 1;
				}
			}
			if (apple_exist == 2){
				if (HAL_GetTick() - apple_exist_timer >= 15000){
					apple_exist = 3;
				}
				else if (HAL_GetTick() - apple_exist_timer >= 800 && flag_voice_apple){
					PWM_Change_Tone(0, 0);
					flag_voice_apple = 0;
				}
			}
			if (medical_exist == 0){
				medical_exist_timer = HAL_GetTick();
				medical_exist = 2;
				if (sound_effect){
					PWM_Change_Tone(1000, 20);
					flag_voice_health = 1;
				}

			}
			if (medical_exist == 2){
				if (HAL_GetTick() - medical_exist_timer >= 15000){
					medical_exist = 3;
				}
				else if (HAL_GetTick() - medical_exist_timer >= 800 && flag_voice_health){
					PWM_Change_Tone(0, 0);
					flag_voice_health = 0;
				}
			}
			if (box_exist == 0){
				box_exist_timer = HAL_GetTick();
				box_exist = 2;
				if (sound_effect){
					PWM_Change_Tone(1500, 20);
					flag_voice_box = 1;
				}
			}
			if (box_exist == 2){
				if (HAL_GetTick() - box_exist_timer >= 15000){
					box_exist = 3;
				}
				else if (HAL_GetTick() - box_exist_timer >= 800 && flag_voice_box){
					PWM_Change_Tone(0, 0);
					flag_voice_box = 0;
				}
			}
		}
		else if (program_mode == 0){
			Update_Melody();
		}
		else {
			PWM_Change_Tone(0, 0);
		}
	}
}

void tokenize(){
	if (program_mode == 6){ // endgame
		digit[0] = 0;
		digit[1] = 0;
		digit[2] = 0;
		digit[3] = 0;
	}
	digit[0] = score/10;
	digit[1] = score%10;
	digit[2] = time_spent/10;
	digit[3] = time_spent%10;
}
void shownumber (int x){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, x%2);
	x/=2;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, x%2);
	x/=2;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, x%2);
	x/=2;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, x%2);
}
void active_digits (){
	if (temp_digit == 0){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 0);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, 1);
		shownumber(digit[0]);
		temp_digit = 1;
	}
	else if (temp_digit == 1){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 0);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, 0);
		shownumber(digit[1]);
		temp_digit = 2;
	}
	else if (temp_digit == 2){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 0);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, 1);
		shownumber(digit[2]);
		temp_digit = 3;
	}
	else if (temp_digit == 3){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 1);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, 1);
		shownumber(digit[3]);
		temp_digit = 0;
	}
}

void init_snake(){
	snake_path[0][0] = 0;
	snake_path[0][1] = 0;
	snake_path[1][0] = 0;
	snake_path[1][1] = 1;
	snake_path[2][0] = 0;
	snake_path[2][1] = 2;
	snake_path[3][0] = 0;
	snake_path[3][1] = 3;
	snake_length = 4;
	health = default_health;
	speed = snake_default_speed;
}
/*
 0 : empty
 1 : snake
 2 : snake_head
 3: apple
 4: health
 5: obstacle
 6: box
 7: number_health
*/
int create_random_number (int start,int end){
	srand(HAL_GetTick());
	return start + rand() % (end-start+1);
}
void init_map (){
	for (int i=0;i<snake_length-1;i++){
		map[snake_path[i][0]][snake_path[i][1]] = 1;
		setCursor(snake_path[i][1], snake_path[i][0]);
		write(4);
	}
	map[snake_path[snake_length-1][0]][snake_path[snake_length-1][1]] = 2;
	setCursor(snake_path[snake_length-1][1], snake_path[snake_length-1][0]);
	write(0);
	map[0][19] = 7; // show number_health
	int obs_current = 0;
	while (obs_current < obstacle_count){
		int x = create_random_number(0, 3);
		int y = create_random_number(0, 19);
		if (map[x][y] == 0){
			map[x][y] = 5;
			obs_current++;
			setCursor(y, x);
			print("I");
		}
	}
	int temp1=0,temp2=0,temp3=0;
	while (temp1 == 0){ // apple
		int x = create_random_number(0, 3);
		int y = create_random_number(0, 19);
		if (map[x][y] == 0){
			map[x][y] = 3;
			setCursor(y, x);
			write(5);
			temp1=1;
		}
	}
	while(temp2 == 0){ // med or health
		int x = create_random_number(0, 3);
		int y = create_random_number(0, 19);
		if (map[x][y] == 0){
			map[x][y] = 4;
			setCursor(y, x);
			write(6);
			temp2=1;
		}
	}
	while(temp3 == 0){ //box
		int x = create_random_number(0, 3);
		int y = create_random_number(0, 19);
		if (map[x][y] == 0){
			map[x][y] = 6;
			setCursor(y, x);
			write(7);
			temp3=1;
		}
	}
}


void change_head (int newhead_x, int newhead_y){
	int flag_increment = 0;
	if (map[newhead_x][newhead_y] == 1){ // body snake
		health--;
	}
	else if (map[newhead_x][newhead_y] == 3){ //apple
		flag_increment = 1;
		score++;
		apple_exist = 0;
	}
	else if (map[newhead_x][newhead_y] == 4){ // health
		if (health+1 > 9){
			health = 9;
		}
		else{
			health++;
		}
		medical_exist = 0;
	}
	else if (map[newhead_x][newhead_y] == 6){ // box
		int rnd_num = create_random_number(1, 3);
		box_exist = 0;
		if (rnd_num == 1){ // Score +2
			score+=2;
			if (score>99){
				score = 99;
			}
		}
		else if (rnd_num == 2){ // health + 2
			if (health + 2 > 9){
				health = 9;
			}
			else{
				health+=2;
			}
		}
		else if (rnd_num == 3){ // speed boost
			speed += 0.5;
		}
	}
	int oldhead_x = snake_path[snake_length-1][0];
	int oldhead_y = snake_path[snake_length-1][1];
	if (flag_increment == 0){
		map[snake_path[0][0]][snake_path[0][1]] = 0;
		setCursor(snake_path[0][1], snake_path[0][0]);
		print(" ");
		map[oldhead_x][oldhead_y] = 1;
		setCursor(oldhead_y, oldhead_x);
		write(4);
		map[newhead_x][newhead_y] = 2;
		setCursor(newhead_y, newhead_x);
		write(direction-1);
		for (int i=0;i<snake_length-1;i++){
			snake_path[i][0] = snake_path[i+1][0];
			snake_path[i][1] = snake_path[i+1][1];
		}
		snake_path[snake_length - 1][0] = newhead_x;
		snake_path[snake_length - 1][1] = newhead_y;
	}
	else if (flag_increment == 1){
		snake_length++;
		map[oldhead_x][oldhead_y] = 1;
		setCursor(oldhead_y, oldhead_x);
		write(4);
		map[newhead_x][newhead_y] = 2;
		setCursor(newhead_y, newhead_x);
		write(direction-1);
		snake_path[snake_length - 1][0] = newhead_x;
		snake_path[snake_length - 1][1] = newhead_y;
	}

}
void move_snake(){
	int old_head_x = snake_path[snake_length-1][0];
	int old_head_y = snake_path[snake_length-1][1];
	if (direction == 1){
		if (old_head_y == 18 && old_head_x == 0){
			if (game_mode == 1 || game_mode == 3){
				if (map[0][0] == 0){
					change_head(0,0);
				}
				else if (map[0][0] == 5){
					health--;
					change_head(1,0);
					direction = 2;
				}
			}
			else if (game_mode == 2){
				if (map[1][18] == 0){
					direction = 2;
					change_head(1, 18);
				}
				else if (map[1][18] == 5){
					direction = 3;
					change_head(1,17);
				}
			}
		}
		else if (old_head_y == 19 && old_head_x > 0 && old_head_x < 3){
			if (game_mode == 1 || game_mode == 3){
				change_head(old_head_x, 0);
			}
			else if (game_mode == 2){
				direction = 2;
				change_head(old_head_x+1, old_head_y);
			}
		}
		else if (old_head_y == 19 && old_head_x == 3){
			if (game_mode == 1 || game_mode == 3){
				change_head(old_head_x, 0);
			}
			else if (game_mode == 2){
				direction = 4;
				change_head(old_head_x-1, old_head_y);
			}
		}
		else{
			if (map[old_head_x][old_head_y + 1] == 5){
				direction = 2;
				health--;
				change_head(old_head_x+1, old_head_y);
			}
			else{
				change_head(old_head_x, old_head_y + 1);
			}

		}
	}

	else if (direction == 2){
		if (old_head_x == 3 && old_head_y > 0){
			if (game_mode == 2){
				direction = 3;
				change_head(old_head_x, old_head_y - 1);
			}
			else if (game_mode == 1 || game_mode == 3){
				change_head(0, old_head_y);
			}
		}
		else if (old_head_x == 3 && old_head_y == 0){
			if (game_mode == 2){
				direction = 1;
				change_head(old_head_x, 1);
			}
			else if (game_mode == 1 || game_mode == 3){
				change_head(0, old_head_y);
			}
		}
		else{
			if (map[old_head_x+1][old_head_y] == 5){
				direction = 3;
				health--;
				change_head(old_head_x , old_head_y - 1 );
			}
			else{
				change_head(old_head_x + 1, old_head_y);
			}

		}
	}
	else if (direction == 3){
		if (old_head_y == 0 && old_head_x > 0){
			if (game_mode == 2){
				direction = 4;
				change_head(old_head_x - 1, old_head_y);
			}
			else if (game_mode == 1 || game_mode == 3){
				change_head(19, old_head_x);
			}
		}
		else if (old_head_y == 0 && old_head_x == 0){
			if (game_mode == 1 || game_mode == 3){
				change_head(19, old_head_x);
			}
			else if (game_mode == 2){
				direction = 2;
				change_head(old_head_x + 1, old_head_y);
			}
		}
		else{
			if (map[old_head_x][old_head_y - 1] == 5){
				direction = 4;
				health--;
				change_head(old_head_x - 1, old_head_y );
			}
			else{
				change_head(old_head_x, old_head_y - 1);
			}

		}
	}
	else if (direction == 4){
		if (old_head_x == 0 && old_head_y >= 0 && old_head_y < 18){
			if (game_mode == 2){
				direction = 1;
				change_head(old_head_x, old_head_y + 1);
			}
			else if (game_mode == 1 || game_mode == 3){
				change_head(3, old_head_y);
			}
		}
		else if (old_head_x == 0 && old_head_y == 18){
			if (game_mode == 2){
				direction = 3;
				change_head(old_head_x, old_head_y - 1);
			}
			else if (game_mode == 1 || game_mode == 3){
				change_head(3, old_head_y);
			}
		}
		else if (old_head_x == 1 && old_head_y == 19){
			if (game_mode == 2){
				direction = 3;
				change_head(old_head_x, 18);
			}
			else if (game_mode == 1 || game_mode == 3){
				change_head(3, old_head_y);
			}
		}
		else{
			if (map[old_head_x - 1][old_head_y] == 5){
				direction = 1;
				health--;
				change_head(old_head_x, old_head_y + 1);
			}
			else{
				change_head(old_head_x - 1, old_head_y);
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
  MX_RTC_Init();
  MX_TIM7_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  PWM_Start();
  LiquidCrystal(GPIOD, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14);
  begin(20,4);
  createChar(0, snake_right);
  createChar(1, snake_down);
  createChar(2, snake_left);
  createChar(3, snake_top);
  createChar(4, snake_body);
  createChar(5, apple);
  createChar(6, med);
  createChar(7, box);
  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 1);
  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 1);
  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 1);
  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 1);
  RTC_TimeTypeDef mytime;
  mytime.Hours = 10;
  mytime.Minutes = 10;
  mytime.Seconds = 10;
  HAL_RTC_SetTime(&hrtc, &mytime, RTC_FORMAT_BIN);
  sprintf (name,"GUEST");
  HAL_UART_Receive_IT(&huart1, &d, 1);
  Play_Music();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if (program_mode == 0){
			health = default_health;
			setCursor(4, 0);
			print ("SNAKE GAME !");
			setCursor(4, 2);
			write(4);
			setCursor(5, 2);
			write(4);
			setCursor(6, 2);
			write(4);
			setCursor(7, 2);
			write(4);
			setCursor(8, 2);
			write(0);
			setCursor(15, 2);
			write(5);
		}
		if (program_mode == 1){ // Menu
			if (first_time_change){
				clear();
				first_time_change = 0;
			}
			for (int i=0;i<4;i++){
				if (i==counter){
					setCursor(0, i);
					print(".");
				}
				else{
					setCursor(0, i);
					print(" ");
				}
			}
			setCursor(1, 0);
			print("Start Game");
			setCursor(1,1);
			print("Settings");
			setCursor(1,2);
			print("Select Mode");
			setCursor(1,3);
			print("About");
		}
		if (program_mode == 2){ // About
			if (first_time_change){
				clear();
				first_time_change = 0;
			}
			RTC_TimeTypeDef endtime;
			HAL_RTC_GetTime(&hrtc, &endtime, RTC_FORMAT_BIN);
			setCursor(7, 0);
			print("Faghih");
			setCursor(5, 1);
			print("& Sharifi");
			setCursor(7, 2);
			char strled[10];
			sprintf(strled,"%02d:%02d:%02d" , endtime.Hours,endtime.Minutes,endtime.Seconds);
			setCursor(7, 3);
			print(strled);

		}
		if (program_mode == 3){ // Game Mode
			if (first_time_change){
				clear();
				first_time_change = 0;
			}
			char strled[20];
			sprintf(strled,"Game Mode : Mode %d" , game_mode);
			setCursor(0, 1);
			print(strled);
		}
		if (program_mode == 4){ // Settings
			if (first_time_change){
				clear();
				first_time_change = 0;
			}
			char strled1[19];
			char strled2[19];
			char strled3[19];
			char strled4[19];
			sprintf(strled1, "Health:%d  Speed:%d" , default_health, snake_default_speed);
			sprintf(strled2, "Obstacle : %d" , obstacle_count);
			if (sound_effect == 1){
				sprintf(strled3 , "Sound : On ");
			}
			else{
				sprintf(strled3 , "Sound : Off");
			}
			sprintf(strled4,"Name : %s" ,name);
			setCursor(1, 0);
			print(strled1);
			setCursor(1, 1);
			print(strled2);
			setCursor(1, 2);
			print(strled3);
			setCursor(1, 3);
			print(strled4);
			if (counter == 0){
				setCursor(0, 0);
				print(".");
				setCursor(10, 0);
				print(" ");
				setCursor(0, 1);
				print(" ");
				setCursor(0, 2);
				print(" ");
			}
			if (counter == 1){
				setCursor(0, 0);
				print(" ");
				setCursor(10, 0);
				print(".");
				setCursor(0, 1);
				print(" ");
				setCursor(0, 2);
				print(" ");
			}
			if (counter == 2){
				setCursor(0, 0);
				print(" ");
				setCursor(10, 0);
				print(" ");
				setCursor(0, 1);
				print(".");
				setCursor(0, 2);
				print(" ");
			}
			if (counter == 3){
				setCursor(0, 0);
				print(" ");
				setCursor(10, 0);
				print(" ");
				setCursor(0, 1);
				print(" ");
				setCursor(0, 2);
				print(".");
			}
		}
		if (program_mode == 5){
			if(first_time_change){
				clear();
				init_snake();
				init_map();
				last_enter_game = HAL_GetTick();
				first_time_change = 0;
			}
			if (HAL_GetTick() - last_enter_game >= 600 / speed){
				move_snake();
				setCursor(19,0);
				char strhealth[1];
				sprintf(strhealth,"%d",health);
				print(strhealth);
				last_enter_game = HAL_GetTick();
			}
			if (apple_exist == 3){
				while (apple_exist == 3){
					int x = create_random_number(0, 3);
					int y = create_random_number(0, 19);
					if (map[x][y] == 0){
						map[x][y] = 3;
						setCursor(y, x);
						write(5);
						apple_exist = 1;
					}
				}
			}
			if (medical_exist == 3) {
				while (medical_exist == 3){
					int x = create_random_number(0, 3);
					int y = create_random_number(0, 19);
					if (map[x][y] == 0){
						map[x][y] = 4;
						setCursor(y, x);
						write(6);
						medical_exist = 1;
					}
				}
			}
			if (box_exist == 3){
				while (box_exist == 3){
					int x = create_random_number(0, 3);
					int y = create_random_number(0, 19);
					if (map[x][y] == 0){
						map[x][y] = 6;
						setCursor(y, x);
						write(7);
						box_exist = 1;
					}
				}
			}
		}
		if (program_mode == 6){
			if (first_time_change == 1){
				clear();
				first_time_change = 0;
			}
			setCursor(1, 0);
			print("GAME OVER!");
			setCursor(1, 1);
			char strres[10];
			sprintf(strres,"SCORE: %d",score);
			print(strres);
			setCursor(1, 2);
			char strres2[19];
			sprintf(strres2,"%Name : %s",name);
			print(strres2);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

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
  htim6.Init.Period = 5000-1;
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
  htim7.Init.Prescaler = 72-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 10000-1;
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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

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

  /*Configure GPIO pins : MEMS_INT3_Pin MEMS_INT4_Pin */
  GPIO_InitStruct.Pin = MEMS_INT3_Pin|MEMS_INT4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PF2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB11 PB12 PB13 PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

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
