# neoPixel Code

Author: Wooseok Diaa Hwang - 황우석

## Introduce

네오픽셀 WS2812B를 DMA PWM을 사용하여 색상을 출력해내는 코드이다. 

## Setting
**Nucleo-F103RB**

- 클럭: HSE, 72Mhz
- 사용 핀
  - PA8: TIM1 Channel1 PWM Generator
- TIM1 setting
  - clock: Interal Clock
  - channel: Channel1 PWM generator
    - prescaler: 0, ARR: 72
  - DMA setting
    - DMA Request: TIM1_CH1
    - Channel: DMA1 Channel 2
    - Direction: Memory To Peripheral
    - DMA Request Settings
      - Mode: normal
      - data width - peripheral: Half Word

## Code
```C
/* USER CODE BEGIN 0 */
#define MAX_LED 10
#define USE_BRIGHTNESS 1

uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4];

void setLED(int ledNum, int red, int green, int blue) {
	LED_Data[ledNum][0] = ledNum;
	LED_Data[ledNum][1] = green;
	LED_Data[ledNum][2] = red;
	LED_Data[ledNum][3] = blue;
}

#define PI 3.14159265

void setBrightness(int brightness) // 0-45
{
#if USE_BRIGHTNESS
	if(brightness >45) brightness = 45;
	for(int i = 0; i<MAX_LED; i++) {
		LED_Mod[i][0] = LED_Data[i][0];
		for(int j = 1; j<4; j++)
		{
			float angle = 90-brightness;
			angle = angle*PI/180;
			LED_Mod[i][j] = (LED_Data[i][j]/(tan(angle)));
		}
	}
#endif
}

uint16_t pwmData[(24*MAX_LED)+50];
uint8_t datasentflag = 0;

void ws2812B_send() {
	uint32_t index = 0;
	uint32_t color;

	for(int i = 0; i<MAX_LED; i++)
	{
#if USE_BRIGHTNESS
		color = ((LED_Mod[i][1]<<16) | (LED_Mod[i][2]<<8) | (LED_Mod[i][3]));
#else
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));
#endif
		for(int i =23; i>=0; i--) {
			if(color&(1<<i))
			{
				pwmData[index] = 60;
			}
			else pwmData[index] = 30;
			index++;
		}
	}

	for(int i = 0; i<50; i++)
	{
		pwmData[index] = 0;
		index++;
	}

	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, index);
	while(!datasentflag){};
	datasentflag = 0;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	datasentflag=1;
}
/* USER CODE END 0 */
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();

  setLED(0, 255, 0, 0);
  setLED(1, 0, 255, 0);
  setLED(2, 0, 0, 255);
  setLED(3, 46, 89, 128);
  setLED(4, 156, 233, 100);
  setLED(5, 102, 0, 235);
  setLED(6, 47, 38, 77);
  setLED(7, 255, 200, 0);
  setLED(8, 255, 100, 52);
  setLED(9, 134, 34, 231);
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  for (int i=0; i<46; i++)
	  {
		  setBrightness(i);
		  ws2812B_send();
		  HAL_Delay (50);
	  }

	  for (int i=45; i>=0; i--)
	  {
		  setBrightness(i);
		  ws2812B_send();
		  HAL_Delay (50);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
```