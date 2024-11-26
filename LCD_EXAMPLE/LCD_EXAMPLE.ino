#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // I2C 주소를 정확히 확인 후 변경하세요

void setup() {
  lcd.init();          // LCD 초기화
  lcd.backlight();     // 백라이트 켜기
  lcd.setCursor(0, 0);
  lcd.print("Hello, BTS");
  lcd.setCursor(0, 1);
  lcd.print("Super STAR!!!");
}

void loop() {
  // 빈 루프
}
