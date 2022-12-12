# Delay_us

Author: Wooseok Diaa Hwang - 황우석

## Introduce

TIM을 이용해 us 딜레이를 주는 함수이다.. 
참고: https://huroint.tistory.com/16

## Setting

**Nucleo-F103RB**

- 클럭: HSE, 72Mhz
- TIM2
  - Clock: Internel Clock
  - prescaler: 72 - 1
  - Counter Period: 65535

## Code

Delay_us.h

```C
/*
 * Delay_us.h
 *
 *  Created on: Dec 12, 2022
 *      Author: SW2148
 */

#ifndef INC_DELAY_US_H_
#define INC_DELAY_US_H_

void delay_us(uint16_t us);

#endif /* INC_DELAY_US_H_ */
```

Delay_us.c

```C
/*
 * Delay_us.c
 *
 *  Created on: Dec 12, 2022
 *      Author: SW2148
 */

#include "stm32f1xx_hal.h"

void delay_us(uint16_t us) {
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while(__HAL_TIM_GET_COUNTER(&htim2) < us);
}
```