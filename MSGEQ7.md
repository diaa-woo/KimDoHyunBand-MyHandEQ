# MSGEQ7

Author: Wooseok Diaa Hwang - 황우석

## Introduce

MSGEQ7 Analog 값을 받아주는 코드이다.
참고: https://github.com/justinb26/MSGEQ7-library/blob/master/MSGEQ7

## Setting
**Nucleo-F103RB**

- 클럭: HSE, 72Mhz
- GPIO
  - PC0: name: RESET; GPIO_OUTPUT; No pull up and pull down
  - PC1: name: STROBE; GPIO_OUTPUT; No pull up and pull down
- ADC setting
  - pin: PA1

## Code

MSGEQ7Library.c

```C
/*
 * MSGEQ7Library.c
 *
 *  Created on: Dec 12, 2022
 *      Author: SW2148
 */

#include "main.h"
#include "stm32f1xx_hal.h"

uint32_t spectrumValues[7];

void poll() {
	HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, SET);
	HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, RESET);

	for(int i = 0; i < 7; i++) {
		HAL_GPIO_WritePin(STROBE_GPIO_Port, STROBE_Pin, RESET);
		delay_us(36);

	}
}
```

MSGEQ7Library.h

```C
/*
 * MSGEQ7Library.h
 *
 *  Created on: Dec 12, 2022
 *      Author: SW2148
 */

#ifndef INC_MSGEQ7LIBRARY_H_
#define INC_MSGEQ7LIBRARY_H_

void poll();

typedef enum {
	HZ_63,
	HZ_160,
	HZ_400,
	HZ_1000,
	HZ_2500,
	HZ_6250,
	HZ_16000
} FREQ;


#endif /* INC_MSGEQ7LIBRARY_H_ */
```
