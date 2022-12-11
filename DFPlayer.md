# neoPixel Code

Author: Wooseok Diaa Hwang - 황우석

## Introduce

DFPlayer로 음악을 출력해내는 코드이다. 
참고: https://www.youtube.com/watch?v=FoB_49eAvFA

## Setting
**Nucleo-F103RB**

- 클럭: HSE, 72Mhz
- 사용 핀
  - PA7: name: BTN1; GPIO_EXTI7; No pull up and pull down
  - PA9: name: BTN2; GPIO_EXTI9; No pull up and pull down
  - PA8: name: BTN3; GPIO_EXTI8; No pull up and pull down
- USART1 setting
  - pin: PB10, PB11
  - Async
  - 9600Bit/s

## Code

DFPlayerLibrary.c

```C
/*
 * DFPlayerLibrary.c
 *
 *  Created on: Dec 12, 2022
 *      Author: diadntjr
 */

#include "stm32f1xx_hal.h"
#include "DFPlayerLibrary.h"
#include "stdio.h"

extern UART_HandleTypeDef huart3;
#define DF_UART &huart3

#define SOURCE 0x02  // TF CARD

uint8_t ispause 	= 0;
uint8_t isplaying	= 1;

//Default byte info
#define START_BYTE 	0x7E
#define END_BYTE 	0xEF
#define VERSION 	0xFF
#define CMD_LEN 	0x06
#define FEEDBACK 	0x00  //If need feedback, change 0x01

// check send format in DFPlayer datasheet
void sendCMD(uint8_t cmd, uint8_t parameter1, uint8_t parameter2)
{
	uint16_t checksum = VERSION + CMD_LEN + cmd + FEEDBACK + parameter1 + parameter2;
	checksum = 0 - checksum;

	uint8_t cmdSequence[10] = { START_BYTE , VERSION , CMD_LEN , cmd, FEEDBACK, parameter1, parameter2, (checksum >> 8)&0x00ff, (checksum&0x00ff), END_BYTE};

	HAL_UART_Transmit(DF_UART, cmdSequence, 10, HAL_MAX_DELAY);
}

void playTrack_DF(uint8_t trackNum)
{
	sendCMD(0x03, 0x00, trackNum);
	HAL_Delay(200);
}

void init_DF(uint8_t volume)  // volume: 0 - 30
{
	sendCMD(0x3F, 0x00, SOURCE);
	HAL_Delay(200);
	sendCMD(0x06, 0x00, volume);
	HAL_Delay(500);
}

void next_DF() {
	sendCMD(0x01, 0x00, 0x00);
	HAL_Delay(200);
}

void playback_DF() {
	sendCMD(0x0D, 0, 0);
	HAL_Delay(200);
}

void pause_DF() {
	sendCMD(0x0E, 0, 0);
	HAL_Delay(200);
}

void previous_DF() {
	sendCMD(0x02, 0, 0);
	HAL_Delay(200);
}

void runMode() {
	switch(DF_MODE) {
	case NEXT:
		next_DF();
		DF_MODE = NONE;
		break;
	case TOGGLE:
		if(isplaying)
		{
			ispause = 1;
			isplaying = 0;
			pause_DF();
		}
		else if(ispause)
		{
			isplaying = 1;
			ispause = 0;
			playback_DF();
		}
		DF_MODE = NONE;
		break;
	case PREVIOUS:
		previous_DF();
		DF_MODE = NONE;
		break;
	case NONE:
		break;
	}
}

```

DFPlayerLibrary.h

```C
/*
 * DFPlayerLibrary.h
 *
 *  Created on: Dec 12, 2022
 *      Author: diadntjr
 */

#ifndef INC_DFPLAYERLIBRARY_H_
#define INC_DFPLAYERLIBRARY_H_

void sendCMD(uint8_t cmd, uint8_t parameter1, uint8_t parameter2);
void playTrack_DF(uint8_t trackNum);
void init_DF(uint8_t volume);
void next_DF();
void pause_DF();
void playback_DF();
void previous_DF();
void runMode();

typedef enum mode{
	NONE,
	NEXT,
	TOGGLE,
	PREVIOUS
}MODE;

extern MODE DF_MODE;

#endif /* INC_DFPLAYERLIBRARY_H_ */
```

main.c | Private User Code

```C
MODE DF_MODE;

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
	if(pin == BTN1_Pin) {
		DF_MODE = PREVIOUS;
	}
	if(pin == BTN2_Pin) {
		DF_MODE = TOGGLE;
	}
	if(pin == BTN3_Pin) {
		DF_MODE = NEXT;
	}
}
```

main 함수

```C
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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  init_DF(14);  // control volume
  playTrack_DF(0x02);  // music order
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  runMode();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

```