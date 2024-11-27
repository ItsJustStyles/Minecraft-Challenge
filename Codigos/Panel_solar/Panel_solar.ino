#include <LiquidCrystal.h>
#include <Servo.h>


//LDRs
const int LDR_TOP_LEFT = A1;
const int LDR_TOP_RIGHT = A2;
const int LDR_BOTTOM_LEFT = A3;
const int LDR_BOTTOM_RIGHT = A4;
const int LIGHT_THRESHOLD = 10;

//Servos
Servo servo_horizontal;
Servo servo_vertical;
int pos_sh = 90;
int pos_sv = 90;
const int UPPER_LIMIT_POS = 160;  //Límite superior de los servos
const int LOWER_LIMIT_POS = 20;   //Límite inferior de los servos

//Multimtro
int analogInput = A0;  // Pin para medir voltaje
float vout = 0.0;
float vin = 0.0;
float R1 = 10000.0;
float R2 = 10000.0;
float current = 0.0;
float R_load = 100.0; // Supón una carga de 100 ohmios
int value = 0;

//Pantalla LCD con I2C
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  //LCD de 16 columnas y 2 filas


void setup() {
  Serial.begin(115200);
  lcd.begin(16,2);
  delay(2000);
  servo_vertical.attach(8);
  servo_horizontal.attach(9);
  servo_vertical.write(90);
  servo_horizontal.write(90);
  pos_sv = servo_vertical.read();
  pos_sh = servo_horizontal.read();
}

void loop() {
  //Leemos los 4 LDRs
  int ldr_tl_value = analogRead(LDR_TOP_LEFT);
  int ldr_tr_value = analogRead(LDR_TOP_RIGHT);
  int ldr_bl_value = analogRead(LDR_BOTTOM_LEFT);
  int ldr_br_value = analogRead(LDR_BOTTOM_RIGHT);

  int average_top = (ldr_tl_value + ldr_tr_value) / 2; //Media de los 2 LDR de arriba
  int average_bottom = (ldr_bl_value + ldr_br_value) / 2; //Media de los 2 LDR de abajo
  int average_left = (ldr_tl_value + ldr_bl_value) / 2; //Media de los 2 LDR de la izquierda
  int average_right = (ldr_tr_value + ldr_br_value) / 2; //Media de los 2 LDR de la derecha

  //Movemos el solar tracker
  moveSolarTracker(average_top, average_bottom, average_left, average_right);
  multimetro();

  //Delay de 30 ms para que los servos no se muevan demasiado rápido
  delay(30);
}

void moveSolarTracker(int average_top, int average_bottom, int average_left, int average_right) {
  //Movemos el solar tracker hacia arriba o hacia abajo
  if ((average_top - average_bottom) > LIGHT_THRESHOLD && pos_sv < UPPER_LIMIT_POS) {
    pos_sv++;
    servo_vertical.write(pos_sv);
  }
  else if ((average_bottom - average_top) > LIGHT_THRESHOLD && pos_sv > LOWER_LIMIT_POS) {
    pos_sv--;
    servo_vertical.write(pos_sv);
  }

  //Movemos el solar tracker hacia la derecha o hacia la izquierda
  if ((average_left - average_right) > LIGHT_THRESHOLD && pos_sh < UPPER_LIMIT_POS) {
    pos_sh++;
    servo_horizontal.write(pos_sh);
  }
  else if ((average_right - average_left) > LIGHT_THRESHOLD && pos_sh > LOWER_LIMIT_POS) {
    pos_sh--;
    servo_horizontal.write(pos_sh);
  }
}

void multimetro(){
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
}

