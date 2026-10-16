#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
const int sensorPin = A0;

// Least Squares Quadratic Model directly on ADC
const float a = -0.0038928;
const float b = 2.51821;
const float c = -225.5925;

float smoothedADC = 500.0;

void setup() {
  lcd.begin(16, 2);
  smoothedADC = analogRead(sensorPin);
}

void loop() {
  long sum = 0;
  for (int i = 0; i < 64; i++) {
    sum += analogRead(sensorPin);
    delayMicroseconds(250);
  }
  float currentAvg = (float)sum / 64.0;

  // Smoothing filter
  smoothedADC = (smoothedADC * 0.85) + (currentAvg * 0.15);

  // Quadratic equation: T = a*x^2 + b*x + c
  float x = smoothedADC;
  float temperature = (a * x * x) + (b * x) + c;

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);
  lcd.write(223);
  lcd.print("C    ");

  lcd.setCursor(0, 1);
  lcd.print("ADC: ");
  lcd.print((int)smoothedADC);
  lcd.print("        ");

  delay(200);
}
