#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int analogInput = A0;  // Pin para medir voltaje
float vout = 0.0;
float vin = 0.0;
float R1 = 10000.0;
float R2 = 10000.0;
float current = 0.0;
float R_load = 100.0; // Supón una carga de 100 ohmios
int value = 0;

void setup() {
  lcd.begin(16, 2);
}

void loop() {
  // Lectura de voltaje
  value = analogRead(analogInput);
  vout = (value * 5.0) / 1024.0;
  vin = vout / (R2 / (R1 + R2));

  // Calcular la corriente
  if (R_load > 0) {
    current = vin / 6; // Corriente estimada en amperios
  }

  // Mostrar voltaje en la LCD
  lcd.setCursor(0, 0);
  if (vin < 0.01) {
    lcd.print("0.00 Voltios");
  } else {
    lcd.print(vin, 2);
    lcd.print(" Voltios");
  }

  // Mostrar corriente en la LCD
  lcd.setCursor(0, 1);
  lcd.print(current, 2);
  lcd.print(" Amperios");

  delay(500);
}
