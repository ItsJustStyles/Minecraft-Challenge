#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define RELE 8
#define PIR_PIN 7

DHT dht(DHTPIN, DHTTYPE);
int SensorPin = A0;
int SensorPin2 = A1;

SoftwareSerial bluetooth(13, 12);

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  pinMode(RELE, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  dht.begin();
}

void loop() {
  int valor1 = analogRead(SensorPin);
  int valor2 = analogRead(SensorPin2);
  int humedad_suelo = map(valor1, 1023, 0, 0, 100);

  float humedad_aire = dht.readHumidity();
  float temperatura = dht.readTemperature();

  Serial.print("Humedad del suelo: ");
  Serial.print(humedad_suelo);
  Serial.print("% ");

  if (isnan(humedad_aire) || isnan(temperatura)) {
    Serial.println("Error al leer el sensor DHT22");
  } else {
    Serial.print("Humedad del aire: ");
    Serial.print(humedad_aire);
    Serial.print("% ");

    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print("°C ");

    if (valor2 < 550) {
      Serial.print("Esta lloviendo ");
    } else {
      Serial.print("No esta lloviendo ");
    }
    // Verificar el sensor PIR
    if (digitalRead(PIR_PIN) == HIGH) {  // Detecta movimiento
      Serial.print("Movimiento detectado! ");
    } else {
      Serial.print("No hay movimiento ");
    }


    // Enviar los datos por Bluetooth

    bluetooth.print(humedad_suelo);
    bluetooth.print(";");

    bluetooth.print(humedad_aire);
    bluetooth.print(";");


    bluetooth.print(temperatura);
    bluetooth.print(";");

    if (valor2 < 550) {
      bluetooth.print("Esta lloviendo");
      bluetooth.print(";");
    } else {
      bluetooth.print("No esta lloviendo");
      bluetooth.print(";");
    }
    // Verificar el sensor PIR
    if (digitalRead(PIR_PIN) == HIGH) {  // Detecta movimiento
      bluetooth.print("Movimiento detectado! ");
      bluetooth.print(";");
    } else {
      bluetooth.print("No hay movimiento ");
      bluetooth.print(";");
    }
  }



  // Verificar si se ha recibido un dato por el Serial Monitor
  if (Serial.available()) {
    char dato = Serial.read();
    if (dato == 'S') {
      digitalWrite(RELE, HIGH);  // Enciende la bomba
      Serial.print("Bomba activada por comando desde el Serial Monitor ");
      delay(1500);
      digitalWrite(RELE, LOW);  // Apagar la bomba
    }
  } else {
    Serial.print("  ");
  }

  // Verificar si se ha recibido un dato por Bluetooth
  if (bluetooth.available()) {
    char dato = bluetooth.read();
    if (dato == 'S') {
      digitalWrite(RELE, HIGH);  // Enciende la bomba
      bluetooth.println("Bomba prendida");
      bluetooth.print(";");
      delay(1500);
      digitalWrite(RELE, LOW);  // Enciende la bomba
    }
  } else {
    bluetooth.println(" ");
    bluetooth.print(";");
  }



  // Lógica de control para activar la bomba de agua
  if (humedad_suelo < 30 && valor2 >= 550) {
    if (humedad_aire < 70 && temperatura > 10) {
      digitalWrite(RELE, HIGH);  // Encender la bomba
      Serial.println("Bomba encendida (riego activado)");
      bluetooth.println("Bomba encendida (riego activado)");
      bluetooth.print(";");
      delay(1500);
      digitalWrite(RELE, LOW);
      Serial.println("Bomba apagada (riego finalizado)");
      bluetooth.println("Bomba apagada (riego finalizado)");
      bluetooth.print(";");
    } else if (humedad_aire >= 65) {
      Serial.println("Humedad del aire alta, no se necesita riego");
      bluetooth.println("Humedad del aire alta, no se necesita riego");
      bluetooth.print(";");
    } else if (temperatura <= 10) {
      Serial.println("Temperatura baja, riego no necesario");
      bluetooth.println("Temperatura baja, riego no necesario");
      bluetooth.print(";");
    }
  } else if (valor2 < 550) {
    digitalWrite(RELE, LOW);  // Apagar la bomba
    Serial.println("No se activa la bomba: está lloviendo");
    bluetooth.println("No se activa la bomba: está lloviendo");
    bluetooth.print(";");

  } else if (temperatura > 30 && humedad_suelo < 30) {
    digitalWrite(RELE, HIGH);  // Encender la bomba
    Serial.println("Bomba encendida: temperatura alta y humedad del suelo baja");
    bluetooth.println("Bomba encendida: temperatura alta y humedad del suelo baja");
    bluetooth.print(";");
    delay(1500);
    digitalWrite(RELE, LOW);
    Serial.println("Bomba apagada (riego finalizado)");
    bluetooth.println("Bomba apagada (riego finalizado)");
    bluetooth.print(";");
  } else {
    digitalWrite(RELE, LOW);  // Asegurar que la bomba esté apagada
    Serial.println("Condiciones normales, bomba apagada");
    bluetooth.println("Condiciones normales, bomba apagada");
    bluetooth.print(";");
  }



  delay(2400);
}
