#include <Arduino.h>
#include <Wire.h>

#define Addr 0x40

#define TEMPERATURE_START_CHAR '@'
#define HUMIDITY_START_CHAR '$'

float get_temperature();
float get_humidity();


void setup() {
	Wire.begin();
	Serial.begin(9600);
}

void loop() {
	float temperature = get_temperature();
	float humidity = get_humidity();
	Serial.print(TEMPERATURE_START_CHAR);
	Serial.println(temperature);
	Serial.print(HUMIDITY_START_CHAR);
	Serial.println(humidity);
	delay(5000);
}

float get_temperature(){
	unsigned int data[2];  
	Wire.beginTransmission(Addr);   
	Wire.write(0xF3);    
	Wire.endTransmission();  
	delay(500);  
	Wire.requestFrom(Addr, 2);  
	if(Wire.available() == 2)  
	{    
		data[0] = Wire.read();    
		data[1] = Wire.read(); 
		float temperature = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
		return temperature;
	}  
}

float get_humidity(){
	unsigned int data[2]; 
	Wire.beginTransmission(Addr);
	Wire.write(0xF5);  
	Wire.endTransmission();  
	delay(500); 
	Wire.requestFrom(Addr, 2);
	if(Wire.available() == 2)
	{
		data[0] = Wire.read();  
		data[1] = Wire.read();   
		float humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
		return humidity;
	}
}