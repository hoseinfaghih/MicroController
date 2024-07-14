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
#define DEBOUNCE_TIME 100

const Tone super_mario_bros[] = {
    {2637, 306}, {0, 153}, {2637, 153}, {0, 153}, {2093, 153},
    {2637, 153}, {0, 153}, {3136, 153}, {0, 459}, {1586, 153},
    {0, 459}, {2093, 153}, {0, 306}, {1586, 153}, {0, 306}, {1319, 153},
    {0, 306}, {1760, 153}, {0, 153}, {1976, 153}, {0, 153}, {1865, 153},
    {1760, 153}, {0, 153}, {1586, 204}, {2637, 204}, {3136, 204}, {3520, 153},
    {0, 153}, {2794, 153}, {3136, 153}, {0, 153}, {2637, 153}, {0, 153},
    {2093, 153}, {2349, 153}, {1976, 153}, {0, 306}, {2093, 153}, {0, 306},
    {1586, 153}, {0, 306}, {1319, 153}, {0, 306}, {1760, 153}, {0, 153},
    {1976, 153}, {0, 153}, {1865, 153}, {1760, 153}, {0, 153}, {1586, 204},
    {2637, 204}, {3136, 204}, {3520, 153}, {0, 153}, {2794, 153}, {3136, 153},
    {0, 153}, {2637, 153}, {0, 153}, {2093, 153}, {2349, 153}, {1976, 153},
    {0, 306}, {2093, 153}, {0, 306}, {1586, 153}, {0, 306}, {1319, 153},
    {0, 306}, {1760, 153}, {0, 153}, {1976, 153}, {0, 153}, {1865, 153},
    {1760, 153}, {0, 153}, {1586, 204}, {2637, 204}, {3136, 204}, {3520, 153},
    {0, 153}, {2794, 153}, {3136, 153}, {0, 153}, {2637, 153}, {0, 153},
    {2093, 153}, {2349, 153}, {1976, 153}, {0, 306}, {0, 300},{0,0}
};

const Tone hedwig_theme[] = {
    {REST, 750}, {NOTE_D4, 374}, {NOTE_G4, 561}, {NOTE_AS4, 187},
    {NOTE_A4, 374}, {NOTE_G4, 750}, {NOTE_D5, 374}, {NOTE_C5, 1124},
    {NOTE_A4, 1124}, {NOTE_G4, 561}, {NOTE_AS4, 187}, {NOTE_A4, 374},
    {NOTE_F4, 750}, {NOTE_GS4, 374}, {NOTE_D4, 2249}, {NOTE_D4, 374},
    {NOTE_G4, 561}, {NOTE_AS4, 187}, {NOTE_A4, 374}, {NOTE_G4, 750},
    {NOTE_D5, 374}, {NOTE_F5, 750}, {NOTE_E5, 374}, {NOTE_DS5, 750},
    {NOTE_B4, 374}, {NOTE_DS5, 561}, {NOTE_D5, 187}, {NOTE_CS5, 374},
    {NOTE_CS4, 750}, {NOTE_B4, 374}, {NOTE_G4, 2249}, {NOTE_AS4, 374},
    {NOTE_D5, 750}, {NOTE_AS4, 374}, {NOTE_D5, 750}, {NOTE_AS4, 374},
    {NOTE_DS5, 750}, {NOTE_D5, 374}, {NOTE_CS5, 750}, {NOTE_A4, 374},
    {NOTE_AS4, 561}, {NOTE_D5, 187}, {NOTE_CS5, 374}, {NOTE_CS4, 750},
    {NOTE_D4, 374}, {NOTE_D5, 1700}, {REST, 150}, {NOTE_AS4, 374},
    {NOTE_D5, 750}, {NOTE_AS4, 374}, {NOTE_D5, 750}, {NOTE_AS4, 374},
    {NOTE_F5, 750}, {NOTE_E5, 374},
	{NOTE_DS5, 750}, {NOTE_B4, 374}, {NOTE_DS5, 561}, {NOTE_D5, 187},
	{NOTE_CS5, 374}, {NOTE_CS4, 750}, {NOTE_AS4, 374}, {NOTE_G4, 2249},
	{0, 1000},{0,0}
};

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

const Tone got[] = {
    {392, 225}, {262, 225}, {311, 112}, {349, 112}, {392, 225}, {262, 225}, {311, 112}, {349, 112},
    {392, 225}, {262, 225}, {311, 112}, {349, 112}, {392, 225}, {262, 225}, {311, 112}, {349, 112},
    {392, 225}, {262, 225}, {330, 112}, {349, 112}, {392, 225}, {262, 225}, {330, 112}, {349, 112},
    {392, 225}, {262, 225}, {330, 112}, {349, 112}, {392, 225}, {262, 225}, {330, 112}, {349, 112},
    {392, 450}, {262, 450},

    {311, 112}, {349, 112}, {392, 450}, {262, 450}, {311, 112}, {349, 112},
    {294, 1800},
    {349, 450}, {233, 450},
    {311, 112}, {294, 112}, {349, 450}, {233, 450},
    {311, 112}, {294, 112}, {262, 1800},

    {392, 450}, {262, 450},

    {311, 112}, {349, 112}, {392, 450}, {262, 450}, {311, 112}, {349, 112},
    {294, 1800},
    {349, 450}, {233, 450},
    {311, 112}, {294, 112}, {349, 450}, {233, 450},
    {311, 112}, {294, 112}, {262, 1800},
    {392, 450}, {262, 450},
    {311, 112}, {349, 112}, {392, 450}, {262, 450}, {311, 112}, {349, 112},

    {294, 900},
    {349, 450}, {233, 450},
    {294, 225}, {311, 225}, {294, 225}, {233, 225},
    {262, 1800},
    {523, 900},
    {466, 900},
    {262, 900},
    {392, 900},
    {311, 900},
    {311, 450}, {349, 450},
    {392, 1800},

    {523, 900},
    {466, 900},
    {262, 900},
    {392, 900},
    {311, 900},
    {311, 450}, {294, 450},
    {523, 225}, {392, 225}, {415, 112}, {466, 112}, {523, 225}, {392, 225}, {415, 112}, {466, 112},
    {523, 225}, {392, 225}, {415, 112}, {466, 112}, {523, 225}, {392, 225}, {415, 112}, {466, 112},

    {0, 450}, {415, 112}, {466, 112}, {523, 225}, {392, 225}, {415, 112}, {466, 112},
    {523, 225}, {392, 112}, {415, 112}, {466, 112}, {523, 225}, {392, 225}, {415, 112}, {466, 112},{0,1000},{0,0}
};
const Tone home_alone[] = {
    {330, 450}, {523, 225}, {494, 225}, {494, 900},
    {523, 225}, {494, 225}, {349, 450}, {392, 450},
    {392, 450}, {330, 450}, {392, 450}, {330, 450},
    {523, 900}, {392, 900},
    {294, 450}, {440, 450}, {392, 450}, {262, 225}, {349, 225},
    {330, 900}, {294, 900},
    {392, 450}, {330, 450}, {392, 450}, {330, 450},
    {523, 900}, {392, 900},
    {440, 225}, {523, 225}, {349, 450}, {330, 225}, {392, 225}, {262, 450},
    {330, 900}, {294, 900},
    {0, 450}, {330, 225}, {262, 225}, {349, 450}, {294, 450},
    {0, 450}, {370, 225}, {294, 225}, {440, 450}, {392, 450},
    {0, 450}, {415, 225}, {330, 225}, {494, 450}, {440, 450},
    {349, 450}, {523, 900}, {440, 450}, {494, 450},
    {392, 450}, {330, 450}, {392, 450}, {330, 450},
    {523, 900}, {392, 900},
    {294, 450}, {440, 450}, {392, 450}, {262, 225}, {349, 225},
    {330, 900}, {294, 900},
    {392, 450}, {330, 450}, {392, 450}, {330, 450},
    {523, 900}, {392, 900},
    {0, 450}, {440, 112}, {494, 112}, {523, 225}, {392, 450}, {330, 450},
    {0, 450}, {440, 112}, {494, 112}, {523, 225}, {392, 450}, {330, 450},
    {0, 450}, {440, 112}, {494, 112}, {523, 225}, {392, 450}, {330, 450},
    {262, 450}, {523, 900}, {494, 450},
    {587, 450}, {330, 450}, {392, 450}, {330, 450},
    {523, 900}, {392, 900},
    {523, 1800},{0,1000},{0,0}
};
const Tone harry_poter[] = {
    {0, 900}, {294, 450}, // REST, D4
    {392, 450}, {466, 225}, {440, 450}, // G4, AS4, A4
    {392, 900}, {587, 450}, // G4, D5
    {523, 900}, // C5
    {440, 900}, // A4
    {392, 450}, {466, 225}, {440, 450}, // G4, AS4, A4
    {349, 900}, {415, 450}, // F4, GS4
    {294, 1800}, // D4
    {294, 450}, // D4

    {392, 450}, {466, 225}, {440, 450}, // G4, AS4, A4
    {392, 900}, {587, 450}, // G4, D5
    {698, 900}, {659, 450}, // F5, E5
    {622, 900}, {494, 450}, // DS5, B4
    {622, 450}, {587, 225}, {554, 450}, // DS5, D5, CS5
    {277, 900}, {494, 450}, // CS4, B4
    {392, 1800}, // G4
    {466, 450}, // AS4

    {587, 900}, {466, 450}, // D5, AS4
    {587, 900}, {466, 450}, // D5, AS4
    {622, 900}, {587, 450}, // DS5, D5
    {554, 900}, {440, 450}, // CS5, A4
    {466, 450}, {587, 225}, {554, 450}, // AS4, D5, CS5
    {277, 900}, {294, 450}, // CS4, D4
    {587, 1800}, // D5
    {0, 450}, {466, 450}, // REST, AS4

    {587, 900}, {466, 450}, // D5, AS4
    {587, 900}, {466, 450}, // D5, AS4
    {698, 900}, {659, 450}, // F5, E5
    {622, 900}, {494, 450}, // DS5, B4
    {622, 450}, {587, 225}, {554, 450}, // DS5, D5, CS5
    {277, 900}, {466, 450}, // CS4, AS4
    {392, 1800},{0,1000},{0,0} // G4
};
const Tone subwaysurf[] = {
    {NOTE_C4, 450}, {REST, 225}, {NOTE_G4, 450}, {REST, 225}, {NOTE_AS4, 450}, {NOTE_C5, 225}, {NOTE_AS4, 225}, {REST, 112}, {NOTE_F4, 225}, {NOTE_DS4, 225}, {REST, 112},
    {NOTE_C4, 450}, {REST, 225}, {NOTE_G4, 450}, {REST, 225}, {NOTE_AS4, 450}, {NOTE_C5, 225}, {NOTE_AS4, 225}, {REST, 112}, {NOTE_F4, 225}, {NOTE_DS4, 225}, {REST, 112},
    {NOTE_C4, 450}, {REST, 225}, {NOTE_G4, 450}, {REST, 225}, {NOTE_AS4, 450}, {NOTE_C5, 225}, {NOTE_AS4, 225}, {REST, 112}, {NOTE_F4, 225}, {NOTE_DS4, 225}, {REST, 112},

    {NOTE_C4, 450}, {REST, 225}, {NOTE_E4, 450}, {REST, 225}, {NOTE_G4, 450}, {NOTE_A4, 450}, {NOTE_AS4, 450},
    {NOTE_C5, 225}, {REST, 112}, {NOTE_C5, 225}, {REST, 112}, {NOTE_AS4, 225}, {REST, 112}, {NOTE_A4, 225}, {REST, 112},
    {NOTE_AS4, 225}, {REST, 112}, {NOTE_AS4, 225}, {NOTE_C5, 225}, {REST, 112}, {NOTE_AS4, 225}, {NOTE_A4, 225}, {REST, 112},
    {REST, 450},
    {NOTE_C5, 225}, {REST, 112}, {NOTE_AS4, 225}, {REST, 112}, {NOTE_A4, 225}, {REST, 112}, {NOTE_AS4, 225}, {REST, 450}, {NOTE_E5, 225},
    {REST, 450},

    {NOTE_C5, 225}, {REST, 112}, {NOTE_C5, 225}, {REST, 112}, {NOTE_AS4, 225}, {REST, 112}, {NOTE_A4, 225}, {REST, 112},
    {NOTE_AS4, 225}, {REST, 112}, {NOTE_AS4, 225}, {NOTE_C5, 225}, {REST, 112}, {NOTE_AS4, 225}, {NOTE_A4, 225}, {REST, 112},
    {REST, 450},
    {NOTE_C5, 225}, {REST, 112}, {NOTE_AS4, 225}, {REST, 112}, {NOTE_A4, 225}, {REST, 112}, {NOTE_AS4, 225}, {REST, 450}, {NOTE_E4, 225},
    {REST, 1800},{0,1000},{0,0}
};
const Tone crb[] = {
    {NOTE_E4, 500}, {NOTE_G4, 500}, {NOTE_A4, 250}, {NOTE_A4, 500}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_B4, 500}, {NOTE_C5, 250}, {NOTE_C5, 500}, {REST, 500},
    {NOTE_C5, 500}, {NOTE_D5, 500}, {NOTE_B4, 250}, {NOTE_B4, 500}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_G4, 500}, {NOTE_A4, 250}, {REST, 500},

    {NOTE_E4, 500}, {NOTE_G4, 500}, {NOTE_A4, 250}, {NOTE_A4, 500}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_B4, 500}, {NOTE_C5, 250}, {NOTE_C5, 500}, {REST, 500},
    {NOTE_C5, 500}, {NOTE_D5, 500}, {NOTE_B4, 250}, {NOTE_B4, 500}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_G4, 500}, {NOTE_A4, 250}, {REST, 500},

    {NOTE_E4, 500}, {NOTE_G4, 500}, {NOTE_A4, 250}, {NOTE_A4, 500}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_C5, 500}, {NOTE_D5, 250}, {NOTE_D5, 500}, {REST, 500},
    {NOTE_D5, 500}, {NOTE_E5, 500}, {NOTE_F5, 250}, {NOTE_F5, 500}, {REST, 500},
    {NOTE_E5, 500}, {NOTE_D5, 500}, {NOTE_E5, 250}, {NOTE_A4, 500}, {REST, 500},

    {NOTE_A4, 500}, {NOTE_B4, 500}, {NOTE_C5, 250}, {NOTE_C5, 500}, {REST, 500},
    {NOTE_D5, 500}, {NOTE_E5, 500}, {NOTE_A4, 250}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_C5, 500}, {NOTE_B4, 250}, {NOTE_B4, 500}, {REST, 500},
    {NOTE_C5, 500}, {NOTE_A4, 500}, {NOTE_B4, 250}, {REST, 500},

    {NOTE_A4, 500}, {NOTE_A4, 250},
    // Repeat of the first part
    {NOTE_A4, 500}, {NOTE_B4, 500}, {NOTE_C5, 250}, {NOTE_C5, 500}, {REST, 500},
    {NOTE_C5, 500}, {NOTE_D5, 500}, {NOTE_B4, 250}, {NOTE_B4, 500}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_G4, 500}, {NOTE_A4, 250}, {REST, 500},

    {NOTE_E4, 500}, {NOTE_G4, 500}, {NOTE_A4, 250}, {NOTE_A4, 500}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_B4, 500}, {NOTE_C5, 250}, {NOTE_C5, 500}, {REST, 500},
    {NOTE_C5, 500}, {NOTE_D5, 500}, {NOTE_B4, 250}, {NOTE_B4, 500}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_G4, 500}, {NOTE_A4, 250}, {REST, 500},

    {NOTE_E4, 500}, {NOTE_G4, 500}, {NOTE_A4, 250}, {NOTE_A4, 500}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_C5, 500}, {NOTE_D5, 250}, {NOTE_D5, 500}, {REST, 500},
    {NOTE_D5, 500}, {NOTE_E5, 500}, {NOTE_F5, 250}, {NOTE_F5, 500}, {REST, 500},
    {NOTE_E5, 500}, {NOTE_D5, 500}, {NOTE_E5, 250}, {NOTE_A4, 500}, {REST, 500},

    {NOTE_A4, 500}, {NOTE_B4, 500}, {NOTE_C5, 250}, {NOTE_C5, 500}, {REST, 500},
    {NOTE_D5, 500}, {NOTE_E5, 500}, {NOTE_A4, 250}, {REST, 500},
    {NOTE_A4, 500}, {NOTE_C5, 500}, {NOTE_B4, 250}, {NOTE_B4, 500}, {REST, 500},
    {NOTE_C5, 500}, {NOTE_A4, 500}, {NOTE_B4, 250}, {REST, 500},
    // End of Repeat

    {NOTE_E5, 375}, {REST, 187}, {REST, 187}, {NOTE_F5, 375}, {REST, 187}, {REST, 187},
    {NOTE_E5, 375}, {NOTE_E5, 375}, {REST, 187}, {NOTE_G5, 375}, {REST, 187}, {NOTE_E5, 375}, {NOTE_D5, 375}, {REST, 187}, {REST, 187},
    {NOTE_D5, 375}, {REST, 187}, {REST, 187}, {NOTE_C5, 375}, {REST, 187}, {REST, 187},
    {NOTE_B4, 375}, {NOTE_C5, 375}, {REST, 187}, {NOTE_B4, 375}, {REST, 187}, {NOTE_A4, 750},

    {NOTE_E5, 375}, {REST, 187},
{REST, 187}, {REST, 187}, {NOTE_F5, 375}, {REST, 187}, {REST, 187},
	{NOTE_E5, 375}, {NOTE_E5, 375}, {REST, 187}, {NOTE_G5, 375}, {REST, 187}, {NOTE_E5, 375}, {NOTE_D5, 375}, {REST, 187}, {REST, 187},
	{NOTE_D5, 375}, {REST, 187}, {REST, 187}, {NOTE_C5, 375}, {REST, 187}, {REST, 187},
	{NOTE_B4, 375}, {NOTE_C5, 375}, {REST, 187}, {NOTE_B4, 375}, {REST, 187}, {NOTE_A4, 750},{0,1000},{0,0}
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

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
static void MX_TIM2_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM17_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t data[100];
uint8_t d;
uint8_t i = 0;
int digit[4];
int vlm = 2;
int current_music = 1;
int temp_digit = 0;
int pause = 1;
int first_unpause = 1;
int volume_mode = 0;
int music_mode = 0;
int input = 0;
int input2 = 2;
int is_shuffle = 0;
int is_pause_after = 0;
int pause_after_time = 0;
int pause_after_start = 0;
TIM_HandleTypeDef *pwm_timer  = &htim2;
uint32_t pwm_channel = TIM_CHANNEL_2;
const Tone *volatile melody_ptr;
volatile uint16_t melody_tone_count;
volatile uint16_t current_tone_number;
volatile uint32_t current_tone_end;

void Play_Music (){
	if (current_music == 1){
		Change_Melody(pink_panther,  sizeof(pink_panther) / sizeof(pink_panther[0]));
	}
	else if (current_music == 2){
		Change_Melody(crb,  sizeof(crb) / sizeof(crb[0]));
	}
	else if (current_music == 3){
		Change_Melody(got,  sizeof(got) / sizeof(got[0]));
	}
	else if (current_music == 4){
		Change_Melody(subwaysurf,  sizeof(subwaysurf) / sizeof(subwaysurf[0]));
	}
	else if (current_music == 5){
		Change_Melody(harry_poter,  sizeof(harry_poter) / sizeof(harry_poter[0]));
	}
	else if (current_music == 6){
		Change_Melody(home_alone,  sizeof(home_alone) / sizeof(home_alone[0]));
	}
	else if (current_music == 7){
		Change_Melody(hedwig_theme,  sizeof(hedwig_theme) / sizeof(hedwig_theme[0]));
	}
	else if (current_music == 8){
		Change_Melody(super_mario_bros,  sizeof(super_mario_bros) / sizeof(super_mario_bros[0]));
	}
}
void Change_Melody(const Tone *melody,uint32_t tone_count){
	melody_ptr = melody;
	melody_tone_count = tone_count;
	current_tone_number = 0;
}
void Update_Melody(){
	if ((HAL_GetTick() > current_tone_end) && (current_tone_number < melody_tone_count)){
		const Tone active_tone = *(melody_ptr + current_tone_number);
		PWM_Change_Tone(active_tone.frequency, vlm);
		current_tone_end = HAL_GetTick() + active_tone.duration;
		current_tone_number++;
	}
	else if (current_tone_number >= melody_tone_count && melody_tone_count != 0){
		if (is_shuffle == 0){
			current_music++;
			if (current_music>8){
				current_music = 1;
			}
		}
		else {
			uint32_t time = HAL_GetTick()%7;
			time++;
			current_music += time;
			if (current_music > 8){
				current_music -= 8;
			}
		}
		Play_Music();
		char response[100];
		uint32_t time = HAL_GetTick();
		sprintf(response,"[INFO][Music Changed to %d][%lu]\n",current_music,time);
		HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
	}
}
void tokenize (int input){
	digit[3] = input%10;
	digit[2] = (input/10)%10;
	digit[1] = (input/100)%10;
	digit[0] = (input/1000)%10;
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
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
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
void deactive_digits(){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0);
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

void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef* hadc){
	if (hadc->Instance == ADC1){
		uint32_t value;
		value = HAL_ADC_GetValue(hadc);
		if (music_mode == 1){
			int super_temp = (value/128)+1;
			input = super_temp;
		}
		else if (volume_mode == 1){
			int super_temp = (value/10)-2;
			if (super_temp < 0){
				super_temp = 0;
			}
			input2 = (super_temp);
		}
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM7) {
		if (music_mode == 1){
			if (input == 0){
				input = current_music;
			}
			tokenize(input);
		}
		else if (volume_mode == 1){
			tokenize(input2);
		}
		else{
			tokenize(current_music);
		}
	}
	if (htim->Instance == TIM6){
		if(!pause){
			active_digits();
		}
		else{
			deactive_digits();
		}
	}
	if (htim->Instance == TIM16){
		if(pause == 1 || music_mode == 1){
			PWM_Change_Tone(0, 0);
		}
		else {
			Update_Melody();
		}

	}
	if (htim->Instance == TIM17){
		if (is_pause_after == 1 && HAL_GetTick() - pause_after_start >= pause_after_time * 1000){
			pause = 1;
			is_pause_after = 0;
			pause_after_start = 0;
			pause_after_time = 0;
		}
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_12){ // pin 0 triggered
		static uint32_t last_enter_time = 0;
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)){ // rising edge
			if (HAL_GetTick() - last_enter_time < DEBOUNCE_TIME){
				return;
			}
			else{
				last_enter_time = HAL_GetTick();
			}
		}
		else if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)){ // falling edge
			if (HAL_GetTick() - last_enter_time < DEBOUNCE_TIME){// debounce
				return;
			}
			else { //short press
				last_enter_time = HAL_GetTick();
				if (pause == 1 && first_unpause == 1){
					uint32_t time = HAL_GetTick();
					char response[100];
					sprintf(response,"[INFO][Music Changed to 1][%lu]\n",time);
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
					Play_Music();
					first_unpause = 0;
				}
				pause = 1 - pause;
			}
		}

	}
	if(GPIO_Pin == GPIO_PIN_13){ // pin 0 triggered
			static uint32_t last_enter_time2 = 0;
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13)){ // rising edge
				if (HAL_GetTick() - last_enter_time2 < DEBOUNCE_TIME){
					return;
				}
				else{
					last_enter_time2 = HAL_GetTick();
					music_mode = 1;
				}
			}
			else{ // falling edge
				if (HAL_GetTick() - last_enter_time2 < DEBOUNCE_TIME){// debounce
					return;
				}
				else { //short press
					last_enter_time2 = HAL_GetTick();
					music_mode = 0;
					current_music = input;
					Play_Music();
					uint32_t time = HAL_GetTick();
					char response[100];
					sprintf(response,"[INFO][Music Changed to %d][%lu]\n",current_music,time);
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				}
			}

		}
	if(GPIO_Pin == GPIO_PIN_14){ // pin 0 triggered
			static uint32_t last_enter_time3 = 0;
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14)){ // rising edge
				if (HAL_GetTick() - last_enter_time3 < DEBOUNCE_TIME){
					return;
				}
				else{
					last_enter_time3 = HAL_GetTick();
					volume_mode = 1;
				}
			}
			else{ // falling edge
				if (HAL_GetTick() - last_enter_time3 < DEBOUNCE_TIME){// debounce
					return;
				}
				else { //short press
					last_enter_time3 = HAL_GetTick();
					vlm = input2;
					uint32_t time = HAL_GetTick();
					char response[100];
					sprintf(response,"[INFO][Volume Changed to %d][%lu]\n",vlm,time);
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
					volume_mode = 0;
				}
			}

		}
}

void check_music_set(int x){
	if (x!=13){
		uint32_t time = HAL_GetTick();
		char response[100];
		sprintf(response,"[ERROR][UNKNOWN-COMMAND][%lu]\n",time);
		HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
	}
	else if (data[9]!='(' || data[11]!=')'){
		uint32_t time = HAL_GetTick();
		char response[100];
		sprintf(response,"[ERROR][FORGOT PARANTHESIS ?][%lu]\n",time);
		HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
	}
	else if (!(data[10]<='8' && data[10]>='1')){
		uint32_t time = HAL_GetTick();
		char response3[100];
		sprintf(response3,"[ERROR][MUSIC not found][%lu]\n",time);
		HAL_UART_Transmit(&huart1, response3, strlen(response3), HAL_MAX_DELAY);
	}
	else{
		int temp = data[10]-'0';
		current_music = temp;
		Play_Music();
		uint32_t time = HAL_GetTick();
		char response4[100];
		sprintf(response4,"[INFO][MUSIC CHANGED TO %d][%lu]\n",temp,time);
		HAL_UART_Transmit(&huart1, response4, strlen(response4), HAL_MAX_DELAY);
	}
}

void check_change_volume (int x){
	if (x < 17 || x > 19){
		uint32_t time = HAL_GetTick();
		char response[100];
		sprintf(response,"[ERROR][UNKNOWN-COMMAND][%lu]\n",time);
		HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
	}
	else if (x == 17){
		if (data[13]!='(' || data[15]!=')'){
			uint32_t time = HAL_GetTick();
			char response[100];
			sprintf(response,"[ERROR][FORGOT PARANTHESIS ?][%lu]\n",time);
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else if (!isdigit(data[14])){
			uint32_t time = HAL_GetTick();
			char response[100];
			sprintf(response,"[ERROR][Volume not valid][%lu]\n",time);
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else{
			int temp =  data[14]-'0';
			vlm = temp;
			uint32_t time = HAL_GetTick();
			char response[100];
			sprintf(response,"[INFO][Volume Changed to %d][%lu]\n",temp,time);
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
	}
	else if (x == 18){
		if (data[13]!='(' || data[16]!=')'){
			uint32_t time = HAL_GetTick();
			char response[100];
			sprintf(response,"[ERROR][FORGOT PARANTHESIS ?][%lu]\n",time);
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else if (!isdigit(data[14]) || !isdigit(data[15])){
			uint32_t time = HAL_GetTick();
			char response[100];
			sprintf(response,"[ERROR][Volume not valid][%lu]\n",time);
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else{
			int temp1 =  data[14]-'0';
			int temp2 = data[15]-'0';
			int temp = (temp1*10+temp2);
			vlm = temp;
			uint32_t time = HAL_GetTick();
			char response[100];
			sprintf(response,"[INFO][Volume Changed to %d][%lu]\n",temp,time);
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
	}
	else if (x == 19){
		if (data[13]!='(' || data[17]!=')'){
			uint32_t time = HAL_GetTick();
			char response[100];
			sprintf(response,"[ERROR][FORGOT PARANTHESIS ?][%lu]\n",time);
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else if (!isdigit(data[14]) || !isdigit(data[15]) || !isdigit(data[16])){
			uint32_t time = HAL_GetTick();
			char response[100];
			sprintf(response,"[ERROR][Volume not valid][%lu]\n",time);
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else{
			int temp1 =  data[14]-'0';
			int temp2 = data[15]-'0';
			int temp3 = data[16]-'0';
			int temp = (temp1*100 + temp2*10 + temp3);
			uint32_t time = HAL_GetTick();
			if (temp > 100){
				char error[100];
				sprintf(error,"[ERROR][Volume not valid][%lu]\n",time);
				HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
			}
			else{
				vlm = temp;
				char response[100];
				sprintf(response,"[INFO][Volume Changed to %d][%lu]\n",temp,time);
				HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			}
		}
	}
}

void check_play_mode (int x){
	if (data[9]!='(' || data[17]!=')'){
		uint32_t time = HAL_GetTick();
		char response[100];
		sprintf(response,"[ERROR][FORGOT PARANTHESIS?][%lu]\n",time);
		HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
	}
	else if (data[10]=='S' && data[11] == 'H' && data[12] == 'U' && data[13] == 'F' && data[14] == 'F' && data[15] == 'L' && data[16] == 'E'){
		is_shuffle = 1;
		uint32_t time = HAL_GetTick();
		char response[100];
		sprintf(response,"[INFO][Play Mode Changed to Shuffle][%lu]\n",time);
		HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
	}
	else if (data[10]=='O' && data[11] == 'R' && data[12] == 'D' && data[13] == 'E' && data[14] == 'R' && data[15] == 'E' && data[16] == 'D'){
		is_shuffle = 0;
		uint32_t time = HAL_GetTick();
		char response[100];
		sprintf(response,"[INFO][Play Mode Changed to ORDERED][%lu]\n",time);
		HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
	}
	else{
		uint32_t time = HAL_GetTick();
		char response[100];
		sprintf(response,"[ERROR][INVALID INPUT (SHUFFLE OR ORDERED)][%lu]\n",time);
		HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
	}
}

void check_pause_after (int x){
	if (data[11]!='(' || data[x-2]!=')'){
		uint32_t time = HAL_GetTick();
		char response[100];
		sprintf(response,"[ERROR][FORGOT PARANTHESIS][%lu]\n",time);
		HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
	}
	else {
		int flag = 1 ,j,res = 0;
		for (j = 12 ; j<(x-2);j++){
			if(!isdigit(data[j])){
				flag = 0;
				break;
			}
			else{
				res*=10;
				res+=(data[j]-'0');
			}
		}
		if (flag == 0){
			uint32_t time = HAL_GetTick();
			char response[100];
			sprintf(response,"[ERROR][Pause Time not valid][%lu]\n",time);
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
		else{
			is_pause_after = 1;
			pause_after_time = res;
			uint32_t time = HAL_GetTick();
			pause_after_start = time;
			char response[100];
			sprintf(response,"[INFO][Pause Time Changed to %d][%lu]\n",res,time);
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
		}
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		HAL_UART_Receive_IT(&huart1, &d, 1);
		data[i++] = d;
		if (d == '\n') {
			data[i] = '\0';
			if (data[0] == 'M'){
				char check1[] = "MUSIC_SET";
				if (i < 9){
					uint32_t time = HAL_GetTick();
					char response[100];
					sprintf(response,"[ERROR][UNKNOWN-COMMAND][%lu]\n",time);
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				}
				else if (strncmp(data,check1,9) == 0){
					check_music_set(i);
				}
				else{
					uint32_t time = HAL_GetTick();
					char response[100];
					sprintf(response,"[ERROR][UNKNOWN-COMMAND][%lu]\n",time);
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				}

			}
			if(data[0] == 'C'){
				char check1[] = "Change_Volume";
				if (i<14){
					uint32_t time = HAL_GetTick();
					char response[100];
					sprintf(response,"[ERROR][UNKNOWN-COMMAND][%lu]\n",time);
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				}
				else if (strncmp(data,check1,13) == 0){
					check_change_volume(i);
				}
				else{
					uint32_t time = HAL_GetTick();
					char response[100];
					sprintf(response,"[ERROR][UNKNOWN-COMMAND][%lu]\n",time);
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				}
			}
			if (data[0] == 'P' && data[1] == 'l'){
				if (i!=19){
					uint32_t time = HAL_GetTick();
					char response[100];
					sprintf(response,"[ERROR][UNKNOWN-COMMAND][%lu]\n",time);
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				}
				char check1[] = "Play_Mode";
				if (strncmp(data,check1,9) == 0){
					check_play_mode(i);
				}
				else{
					uint32_t time = HAL_GetTick();
					char response[100];
					sprintf(response,"[ERROR][UNKNOWN-COMMAND][%lu]\n",time);
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				}
			}
			if (data[0] == 'P' && data[1] == 'a'){
				char check1[] = "Pause_After";
				if (strncmp(data,check1,11) == 0){
					check_pause_after(i);
				}
				else{
					uint32_t time = HAL_GetTick();
					char response[100];
					sprintf(response,"[ERROR][UNKNOWN-COMMAND][%lu]\n",time);
					HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
				}
			}
			i = 0;
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
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start_IT(&htim16);
  HAL_TIM_Base_Start_IT(&htim17);

  HAL_ADC_Start_IT(&hadc1);
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
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
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
  hadc1.Init.Resolution = ADC_RESOLUTION_10B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
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
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

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
  htim7.Init.Prescaler = 48-1;
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
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 48-1;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 1000-1;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */

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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT1_Pin
                           MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT1_Pin
                          |MEMS_INT2_Pin;
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

  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD1 PD2 PD3 PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
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
