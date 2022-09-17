#include <Adafruit_NeoPixel.h> // Neopixel을 사용하기 위해서 라이브러리를 불러옵니다.
#include <avr/power.h>

#define PIN 2                             // 디지털 입력 핀 설정
#define NUMPIXELS 2             // Neopixel LED 소자 수 (LED가 24개라면 , 24로 작성)
#define BRIGHTNESS 180       // 밝기 설정 0(어둡게) ~ 255(밝게) 까지 임의로 설정 가능

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.setBrightness(BRIGHTNESS);    //  BRIGHTNESS 만큼 밝기 설정
  strip.begin();                                        //  Neopixel 제어를 시작
  strip.show();                                        //  Neopixel 동작 초기화 합니다
}

void loop() {

  strip.begin();

  strip.setPixelColor(0, 255, 0, 0);          //  Neopixel 색상 설정 ( 첫번째 소자위치 , 색상설정(Red) , 0 , 0 )
  strip.setPixelColor(1, 0, 255, 0);          //  ( 두번째 소자위치 , 0 , 색상설정(Green) , 0 )
  //strip.setPixelColor(2, 0, 0, 255);          //  ( 세번째 소자위치 , 0 , 0 , 색상설정(Blue) )
  //strip.setPixelColor(3, 255, 255, 255);  //  ( 네번째 소자위치 , (Red) , (Green) , (Blue) ) 3가지 색을 다 킨다면 White가 켜짐
  strip.show();                                         //  LED가 켜지는 동작을 하게 합니다

  delay(2000);
  //  colorWipe 함수를 적용하면 모든 LED를 동일한 색상으로 적용합니다.

  colorWipe(strip.Color(255, 0, 0), 100); //  strip 색상을 ( 255 (Red) , 0 (Green) , 0 (Blue) , 마지막 100은 딜레이 타임으로 숫자만큼 픽셀이 하나씩 색상이 바뀜
  delay(1500);                         
  colorWipe(strip.Color(0, 255, 0), 100); //  strip 색상을 ( 0 (Red) , 255 (Green) , 0 (Blue) , 100)
  delay(1500);
  colorWipe(strip.Color(0, 0, 255), 100); //  strip 색상을 ( 0 (Red) , 0 (Green) , 255 (Blue) , 100)
  delay(1500);
  colorWipe(strip.Color(255, 255, 255), 100); //  strip 색상을 ( 255 (Red) , 255 (Green) , 255 (Blue) , 100)  " White "
  delay(1500);
}

void colorWipe(uint32_t c, uint8_t wait) {   //  loop에서 사용할 colorWipe 함수를 만든 공식
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
