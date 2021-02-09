#include <Wire.h>
#include "Display.h"

Display display;
RTClib myRTC;

#define MOISTURE_SENSOR A0
#define MOISTURE_POWER 10
#define MEASUREMENT_FREQ 60L*1000 // Every minute
#define MEASUREMENT_DUR 100 // 50ms of power before measuring

unsigned long lastMeasurement = 0L;

void setup() {
  pinMode(MOISTURE_POWER, OUTPUT);
  display.initialize();
  Serial.begin(9600);
}

void loop() {
  display.displayTime(myRTC.now());
  if (lastMeasurement==0L || millis() - lastMeasurement > MEASUREMENT_FREQ){
      int moisture = measureMoisture();   
      display.displayMoisture(moisture);
      lastMeasurement = millis();
  }
  delay(1000);
}

int measureMoisture(){
  analogWrite(MOISTURE_POWER, 255);
  delay(MEASUREMENT_DUR);
  int sensorValue = analogRead(MOISTURE_SENSOR);
  analogWrite(MOISTURE_POWER, 0);
  return convertToPercentage(sensorValue); 
}

int convertToPercentage(int value)
{
  return map(value, 1023, 0, 0, 100);
}
