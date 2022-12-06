# neoPixel Code

Author: Wooseok Diaa Hwang - 황우석

## Introduce

DFPlayer로 음악을 출력해내는 코드이다. 
참고: https://www.youtube.com/watch?v=FoB_49eAvFA

## Setting
**Nucleo-F103RB**

- 클럭: HSE, 72Mhz
- 사용 핀
  - PA8: TIM1 Channel1 PWM Generator
- USART1 setting
  - pin: PB10, PB11
  - Async
  - 9600Bit/s

## Code

DFPlayerLibrary.c

```C
#include "stm32f1xx_hal.h"
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
```

DFPlayerLibrary.h

```C
#ifndef INC_DFPLAYERLIBRARY_H_
#define INC_DFPLAYERLIBRARY_H_

void sendCMD(uint8_t cmd, uint8_t parameter1, uint8_t parameter2);
void playTrack_DF(uint8_t trackNum);
void init_DF(uint8_t volume);


#endif /* INC_DFPLAYERLIBRARY_H_ */

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
  init_DF(20);  // control volume
  playTrack_DF(0x02);  // music order
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

```