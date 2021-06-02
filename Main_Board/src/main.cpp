#include <Arduino.h>
#include <LiquidCrystal.h>

#define TEMPERATURE_START_CHAR '@'
#define HUMIDITY_START_CHAR '$'
enum Bluetooth_state {idle = 0, Temperature = 1, Humidity = 2};
const int rs = 13, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int motor_en = 9;
const int motor_pin1 = 8;
const int motor_pin2 = 7;

void Handle_Bluetooth();
void print_LCD();
void main_func();

float temperature, humidity;
int bluetooth_state = 0;
bool change_sensors_state = false;
int watering_amount = 0;
enum Watering_state {Null = 0, CC = 1};
int watering_state = Null;

void setup() {
  lcd.begin(20, 4);
  Serial.begin(9600);
  pinMode(motor_en, OUTPUT);
	pinMode(motor_pin1, OUTPUT);
	pinMode(motor_pin2, OUTPUT);

  digitalWrite(motor_pin1, LOW);
	digitalWrite(motor_pin2, LOW);
}

void loop() {
  if (Serial.available() >= 5) {
    Handle_Bluetooth();
  }

  if(change_sensors_state){
    main_func();
    print_LCD();
    change_sensors_state = false;
  }
}

void main_func(){
  if(humidity > 50){
    watering_amount = 0;
    watering_state = Null;
    digitalWrite(motor_pin1, LOW);
	  digitalWrite(motor_pin2, LOW);
  }else if(humidity < 20){
    watering_amount = 20;
    watering_state = CC;
    analogWrite(motor_en,64);
    digitalWrite(motor_pin1, HIGH);
	  digitalWrite(motor_pin2, LOW);
  }else{ // 20 <= humidity <= 50
    if(temperature < 25){
      watering_amount = 0;
      watering_state = Null;
      digitalWrite(motor_pin1, LOW);
	    digitalWrite(motor_pin2, LOW);
    }
    if(temperature >= 25){
      watering_amount = 10;
      watering_state = CC;
      analogWrite(motor_en,26);
      digitalWrite(motor_pin1, HIGH);
	    digitalWrite(motor_pin2, LOW);
    }
  }
}

void Handle_Bluetooth(){
  if(bluetooth_state == idle){
    char incomingByteChar = Serial.read();
    if(incomingByteChar == TEMPERATURE_START_CHAR)
      bluetooth_state = Temperature;
    else if(incomingByteChar == HUMIDITY_START_CHAR)
      bluetooth_state = Humidity;
    else
      Serial.read();
  }
  if(bluetooth_state == Temperature){
    float old_temperature = temperature;
    temperature = Serial.parseFloat();
    bluetooth_state = idle;
    if(temperature != old_temperature)
      change_sensors_state = true;
  }
  if(bluetooth_state == Humidity){
    float old_humidity = humidity;
    humidity = Serial.parseFloat();
    bluetooth_state = idle;
    if(humidity != old_humidity)
      change_sensors_state = true;
  }
}

void print_LCD(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println(String(temperature).c_str());
  lcd.println(String(humidity).c_str());
  lcd.setCursor(0, 1);
  lcd.setCursor(0, 2);
  if(watering_state == Null){
    lcd.println("No Watering Is Needed!");
  }else{
    lcd.println(String(watering_amount));
    if(watering_state == CC)
      lcd.println("CC");
  }
}