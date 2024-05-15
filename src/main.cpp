#include <Arduino.h>

#define MOTOR_DRIVER1_IN1_PIN 12
#define MOTOR_DRIVER1_IN2_PIN 13

#define MOTOR_DRIVER1_IN3_PIN 11
#define MOTOR_DRIVER1_IN4_PIN 10

#define MOTOR_DRIVER2_IN1_PIN 8
#define MOTOR_DRIVER2_IN2_PIN 9

#define MOTOR_DRIVER2_IN3_PIN 7
#define MOTOR_DRIVER2_IN4_PIN 6

#define LIGHT_RIGHT 5
#define LIGHT_LEFT 4

#define ULTRASONIC_SENSOR_TRIGGER 2
#define ULTRASONIC_SENSOR_ECHO 3

long current_millis = 0;
int prev_millis = -1;
char serial_data[256] = {'\0'};

int prev_millis_ultrasonic = -1;
long current_distance_cm = 0;
#define ULTRASONIC_SENSOR_MEAN_COUNT 5
long ultrasonic_readings[ULTRASONIC_SENSOR_MEAN_COUNT] = {0};
int ultrasonic_idx = 0;

#define MIN_DISTANCE 5 // in cm

void get_sensor_distance_reading_cm(void);

int led_state = 0;
int blink_counter = 0;

void setup() {
  Serial.begin(9600);

  // motor pins
  pinMode(MOTOR_DRIVER1_IN1_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER1_IN2_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER1_IN3_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER1_IN4_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER2_IN1_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER2_IN2_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER2_IN3_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER2_IN4_PIN, OUTPUT);

  pinMode(LIGHT_RIGHT, OUTPUT);
  pinMode(LIGHT_LEFT, OUTPUT);

  // ultrasonic
  pinMode(ULTRASONIC_SENSOR_TRIGGER, OUTPUT);
  pinMode(ULTRASONIC_SENSOR_ECHO, INPUT);

  Serial.println("Comecando testes de cada motor\n");
  digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
  digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);
  digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
  digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);
  digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
  digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);
  digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
  digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
  delay(1000);
  Serial.println("Testando Motor 1\n");
  digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
  digitalWrite(MOTOR_DRIVER1_IN2_PIN, HIGH);
  delay(1000);
  Serial.println("Invertendo Motor 1\n");
  digitalWrite(MOTOR_DRIVER1_IN1_PIN, HIGH);
  digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);
  delay(1000);
  digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
  digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);

  Serial.println("Testando Motor 2\n");
  digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
  digitalWrite(MOTOR_DRIVER1_IN4_PIN, HIGH);
  delay(1000);
  Serial.println("Invertendo Motor 2\n");
  digitalWrite(MOTOR_DRIVER1_IN3_PIN, HIGH);
  digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);
  delay(1000);
  digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
  digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);

  Serial.println("Testando Motor 3\n");
  digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
  digitalWrite(MOTOR_DRIVER2_IN2_PIN, HIGH);
  delay(1000);
  Serial.println("Invertendo Motor 3\n");
  digitalWrite(MOTOR_DRIVER2_IN1_PIN, HIGH);
  digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);
  delay(1000);
  digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
  digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);

  Serial.println("Testando Motor 4\n");
  digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
  digitalWrite(MOTOR_DRIVER2_IN4_PIN, HIGH);
  delay(1000);
  Serial.println("Invertendo Motor 4\n");
  digitalWrite(MOTOR_DRIVER2_IN3_PIN, HIGH);
  digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
  delay(1000);
  digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
  digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
}

void loop() {
  size_t package_size = 0;
  current_millis = millis();

  if (current_millis - prev_millis_ultrasonic >= 150) {
    get_sensor_distance_reading_cm();
    prev_millis_ultrasonic = current_millis;
  }

  if (current_distance_cm <= MIN_DISTANCE) {
    Serial.print("Parando exec ");
    Serial.println(millis());

    digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
    digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);
    digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
    digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);
    digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
    digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);
    digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
    digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
    delay(1000);
    digitalWrite(MOTOR_DRIVER1_IN1_PIN, HIGH);
    digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);
    digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
    digitalWrite(MOTOR_DRIVER1_IN4_PIN, HIGH);
    digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
    digitalWrite(MOTOR_DRIVER2_IN2_PIN, HIGH);
    digitalWrite(MOTOR_DRIVER2_IN3_PIN, HIGH);
    digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
    delay(2000);
  } else if (Serial.available()) {
    package_size = Serial.readBytesUntil('\n', serial_data, 255);
    if (package_size >= 1) {
      prev_millis = current_millis;
      switch (serial_data[0]) {
      case 'F':
        led_state = 0;
        digitalWrite(MOTOR_DRIVER1_IN1_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN3_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN1_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN3_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
        break;
      case 'G':
        led_state = 0;
        digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN2_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN4_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN2_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN4_PIN, HIGH);
        break;
      case 'R':
        digitalWrite(LIGHT_RIGHT,HIGH);
        led_state = 1;

        digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN2_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER1_IN3_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN1_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN4_PIN, HIGH);
        break;
      case 'L':
        digitalWrite(LIGHT_LEFT,HIGH);
        led_state = 2;
        digitalWrite(MOTOR_DRIVER1_IN1_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN4_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN2_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER2_IN3_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
        break;
      case 'S':
        led_state = 0;
        digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
        break;
      case 'X':
        led_state = 0;
        Serial.println("Comecando testes de cada motor\n");
        digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
        delay(1000);
        Serial.println("Testando Motor 1\n");
        digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN2_PIN, HIGH);
        delay(1000);
        Serial.println("Invertendo Motor 1\n");
        digitalWrite(MOTOR_DRIVER1_IN1_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);
        delay(1000);
        digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);

        Serial.println("Testando Motor 2\n");
        digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN4_PIN, HIGH);
        delay(1000);
        Serial.println("Invertendo Motor 2\n");
        digitalWrite(MOTOR_DRIVER1_IN3_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);
        delay(1000);
        digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);

        Serial.println("Testando Motor 3\n");
        digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN2_PIN, HIGH);
        delay(1000);
        Serial.println("Invertendo Motor 3\n");
        digitalWrite(MOTOR_DRIVER2_IN1_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);
        delay(1000);
        digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);

        Serial.println("Testando Motor 4\n");
        digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN4_PIN, HIGH);
        delay(1000);
        Serial.println("Invertendo Motor 4\n");
        digitalWrite(MOTOR_DRIVER2_IN3_PIN, HIGH);
        digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
        delay(1000);
        digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
        digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);
        break;
      }
      Serial.print("Comando Recebido:");
      Serial.println(serial_data);
    }
  }
  switch (led_state)
  {
  case 0:
    digitalWrite(LIGHT_LEFT,LOW);
    digitalWrite(LIGHT_RIGHT,LOW);
    blink_counter = 0;
    break;
  case 1://direita
    if(current_millis - prev_millis > 500){
      blink_counter++;
      digitalWrite(LIGHT_RIGHT,blink_counter & 1); 
    }
    break;
  case 2:
    if(current_millis - prev_millis > 500){
      blink_counter++;
      digitalWrite(LIGHT_LEFT,blink_counter & 1); 
    }
    break;
  default:
    break;
  }
  // if (prev_millis != -1 && current_millis - prev_millis > 10000) {
  //   prev_millis = -1;
  //   Serial.println("Parando carro Timeout\n");
  //   digitalWrite(MOTOR_DRIVER1_IN1_PIN, LOW);
  //   digitalWrite(MOTOR_DRIVER1_IN2_PIN, LOW);
  //   digitalWrite(MOTOR_DRIVER1_IN3_PIN, LOW);
  //   digitalWrite(MOTOR_DRIVER1_IN4_PIN, LOW);
  //   digitalWrite(MOTOR_DRIVER2_IN1_PIN, LOW);
  //   digitalWrite(MOTOR_DRIVER2_IN2_PIN, LOW);
  //   digitalWrite(MOTOR_DRIVER2_IN3_PIN, LOW);
  //   digitalWrite(MOTOR_DRIVER2_IN4_PIN, LOW);  
  // }
  // put your main code here, to run repeatedly:
}

// dps dessa funcao passaram 38 millisegundos + 12 us
void get_sensor_distance_reading_cm() {
  long mean = 0, cnt = 0;
  digitalWrite(ULTRASONIC_SENSOR_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_SENSOR_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_SENSOR_TRIGGER, LOW);
  ultrasonic_readings[ultrasonic_idx] =
      ((pulseIn(ULTRASONIC_SENSOR_ECHO, HIGH) / 29) / 2);
  Serial.print("Instant distance: ");
  Serial.print(ultrasonic_readings[ultrasonic_idx]);
  Serial.print(" ");

  ultrasonic_idx++;
  if (ultrasonic_idx >= ULTRASONIC_SENSOR_MEAN_COUNT - 1) {
    ultrasonic_idx = 0;
  }
  for (int i = 0; i < ULTRASONIC_SENSOR_MEAN_COUNT; i++) {
    mean += ultrasonic_readings[i];
    cnt++;
  }
  current_distance_cm = mean / cnt;
  Serial.print(" Mean distance: ");
  Serial.print(current_distance_cm);
  Serial.print(" ");
  Serial.println(current_millis);
}