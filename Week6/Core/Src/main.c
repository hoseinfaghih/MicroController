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
#define DEBOUNCE_TIME 100

typedef unsigned char byte;
byte dinasour[] = {
  0x07,
  0x05,
  0x07,
  0x13,
  0x1F,
  0x0F,
  0x07,
  0x05
};

byte cactus[] = {
  0x04,
  0x14,
  0x15,
  0x1D,
  0x05,
  0x07,
  0x04,
  0x04
};

byte zamin[] = {
  0x1F,
  0x00,
  0x00,
  0x09,
  0x10,
  0x02,
  0x08,
  0x00
};
byte khali[] = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00
};

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
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

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
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t data[100];
uint8_t d;
uint8_t i = 0;
int time_got = 0;
int game_start = 0;
int game_start_time = 0;
int game_over = 0;
int cactus_position = 0;
int dinasour_jump = 0;
int dinasour_jump_time = 0;
int game_over_set = 0;
int is_pause = 0;
int current_music = 1;
int volume = 50;
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
		PWM_Change_Tone(active_tone.frequency, volume);
		current_tone_end = HAL_GetTick() + active_tone.duration;
		current_tone_number++;
	}
	else if (current_tone_number >= melody_tone_count && melody_tone_count != 0){
		current_music++;
		if (current_music>8){
			current_music = 1;
		}
		Play_Music();
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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1) {
			HAL_UART_Receive_IT(&huart1, &d, 1);
			data[i++] = d;
			if (d == '\n') {
				data[i] = '\0';
				check_time(i);
				i = 0;
			}
		}
}
void check_time (int i){
	if (time_got){
		char error[] = "TIME ALREADY RECIEVED THANKS \n";
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
	else if (i != 9){
		char error [] = "PLEASE ENTER TIME IN HH:MM:SS FORMAT\n" ;
		HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
	}
	else {
		if (!isdigit(data[0]) || !isdigit(data[1]) || !isdigit(data[3]) || !isdigit(data[4]) || !isdigit(data[6]) || !isdigit(data[7])){
			char error [] = "TIME SHOULD BE NUMERIC !!\n" ;
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
		int hour = (data[0]-'0')*10 + (data[1] - '0');
		int minute = (data[3]-'0')*10 + (data[4] - '0');
		int seconds = (data[6] - '0')*10 + (data[7] - '0');
		if (hour > 23){
			char error [] = "HOUR MUST BE LESS THAN 24\n" ;
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
		else if (minute > 59){
			char error [] = "MINUTES MUST BE LESS THAN 60\n" ;
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
		else if (seconds > 59){
			char error [] = "SECONDS MUST BE LESS THAN 60\n" ;
			HAL_UART_Transmit(&huart1, error, strlen(error), HAL_MAX_DELAY);
		}
		else{
			char response[] = "WE GOT YOUR TIME ! THANKS !\n";
			HAL_UART_Transmit(&huart1, response, strlen(response), HAL_MAX_DELAY);
			RTC_TimeTypeDef mytime;
			mytime.Hours = hour;
			mytime.Minutes = minute;
			mytime.Seconds = seconds;
			HAL_RTC_SetTime(&hrtc, &mytime, RTC_FORMAT_BIN);
			time_got = 1;
		}
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == GPIO_PIN_12){
		static uint32_t last_enter_time = 0;
		if (HAL_GetTick() - last_enter_time < DEBOUNCE_TIME){
			return;
		}
		else {
			last_enter_time = HAL_GetTick();
			for (int ii=0;ii<4;ii++){
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 0);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, 0);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 0);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, 0);
				if (ii == 0){
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
				  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)){ // S1
					  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
				  }
				}
				if (ii == 1){
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, 1);
				  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)){ // S5
					  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9);
				  }
				}
				if (ii == 2){
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 1);
				  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)){ // S9
					  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
				  }
				}
				if (ii == 3){
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, 1);
				  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)){ // S13
					  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
				  }
				}
			}
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, 1);
			while(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12));
		}
	}
	if (GPIO_Pin == GPIO_PIN_13){
		static uint32_t last_enter_time = 0;
		if (HAL_GetTick() - last_enter_time < DEBOUNCE_TIME){
			return;
		}
		else {
			last_enter_time = HAL_GetTick();
			for (int ii=0;ii<4;ii++){
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 0);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, 0);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 0);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, 0);
				if (ii == 0){
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
				  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13)){ // S2
					  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_12);
					  if (time_got){
						  game_start_time = HAL_GetTick();
						  game_start = 1;
						  Play_Music();
					  }
				  }
				}
				if (ii == 1){
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, 1);
				  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13)){ // S6
					  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
					  if (game_start == 1 && game_over == 0 && !is_pause){
						  dinasour_jump = 1;
						  dinasour_jump_time = HAL_GetTick();
					  }
				  }
				}
				if (ii == 2){
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 1);
				  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13)){ // S10
					  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_14);
					  if (game_start == 1 && game_over == 0){
						  is_pause = 1 - is_pause ;
					  }
				  }
				}
				if (ii == 3){
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, 1);
				  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13)){ // S14
					  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15);
					  if (game_start == 1 && game_over == 0){
						  current_music++;
						  if (current_music > 8){
							  current_music = 1;
						  }
						  Play_Music();
					  }
				  }

				}
			}
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, 1);
			while(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13));
		}
	}
	if (GPIO_Pin == GPIO_PIN_14){

	}
	if (GPIO_Pin == GPIO_PIN_15){

	}
}
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//	if (htim->Instance == TIM6){
//		if (dinasour_jump && HAL_GetTick() - dinasour_jump_time >= 500){
//			dinasour_jump = 0;
//
//		}
//		if (game_start){
//		  cactus_moves();
//		}
//
//	}
//}
void cactus_moves(){
	static uint32_t last_move_time = 0;
	if (HAL_GetTick() - last_move_time >= 200){
		last_move_time = HAL_GetTick();
		if (cactus_position == -1){
			setCursor(19, 2);
			write(1);
			cactus_position=19;
		}
		else if (cactus_position > 1){
			setCursor(cactus_position, 2);
			print(" ");
			setCursor(cactus_position-1, 2);
			write(1);
			cactus_position--;
		}
		else if (cactus_position == 1){
			if (!dinasour_jump){
				game_over = 1;
				game_start = 0;
			}
			else{
				setCursor(cactus_position, 2);
				print(" ");
				setCursor(cactus_position-1, 2);
				write(1);
				cactus_position--;
			}
		}
		else if (cactus_position == 0){
			if (!dinasour_jump){
				game_over = 1;
				game_start = 0;
			}
			else{
				setCursor(cactus_position, 2);
				print(" ");
				cactus_position--;
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
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  PWM_Start();
  LiquidCrystal(GPIOD, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6);
  begin(20,4);
  createChar(0, dinasour);
  createChar(1, cactus);
  createChar(2, zamin);
  createChar(3, khali);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, 1);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 1);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, 1);
  char hello [] = "HELLO ! PLEASE ENTER TIME :\n";
  HAL_UART_Transmit(&huart1, hello, strlen(hello), HAL_MAX_DELAY);
  HAL_UART_Receive_IT(&huart1, &d, 1);

  for (int index = 0; index <20 ;index++){
	  setCursor(index, 3);
	  write(2);
  }
  setCursor (0,2);
  write(0);
  setCursor(16,2);
  write(1);
  cactus_position = 16;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (game_over && !game_over_set){
		  clear();
		  int haltime = HAL_GetTick();
		  haltime -= game_start_time;
		  haltime/=1000;
		  RTC_TimeTypeDef endtime;
		  HAL_RTC_GetTime(&hrtc, &endtime, RTC_FORMAT_BIN);
		  char struart [30];
		  sprintf (struart,"GAME FINISHED AT %02d:%02d:%02d\n",endtime.Hours , endtime.Minutes,endtime.Seconds);
		  char strled[10];
//		  int randomnumebr = endtime.SubSeconds;
		  sprintf(strled,"SCORE : %d" , haltime);
		  setCursor(5, 1);
		  print("GAME OVER !");
		  setCursor(5, 2);
		  print(strled);
		  HAL_UART_Transmit(&huart1, struart, strlen(struart), HAL_MAX_DELAY);
		  game_over_set = 1 ;
		  PWM_Change_Tone(0, 0);
	  }
	  else if (game_start == 1 && game_over == 0){
		  if (!is_pause){
			  if (dinasour_jump){
				  if (HAL_GetTick() - dinasour_jump_time > 800){
					  setCursor(0, 2);
					  write(0);
					  setCursor(0, 1);
					  print(" ");
					  dinasour_jump = 0;
				  }else{
					  setCursor(0, 2);
					  write(3);
					  setCursor(0, 1);
					  write(0);
				  }
			  }
			  cactus_moves();
			  Update_Melody();
		  }
		  else{
			  PWM_Change_Tone(0, 0);
		  }
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
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
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
  hrtc.Init.AsynchPrediv = 40-1;
  hrtc.Init.SynchPrediv = 1000-1;
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
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

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

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD1 PD2 PD3 PD4
                           PD5 PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
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
