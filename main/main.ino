#include <Wire.h>
#include "Display.h"
#include <CircularBuffer.h>

Display display;
RTClib myRTC;

#define MOISTURE_SENSOR A0
#define MOISTURE_POWER 10
#define MEASUREMENT_FREQ 60L*1000 // Every minute
#define MEASUREMENT_DUR 100 // 50ms of power before measuring
#define HISTORY_EMA_BETA 0.9 //averaging over 10 measurements
#define WATERING_INTERVAL 60L*3*60*1000 //at least three hours between waterings
#define WATERING_INCREASE_THRESHOLD 5 //at least 5 percent increase to detect watering

unsigned long lastMeasurement = 0L;
unsigned long lastWatering = 0L;
CircularBuffer<int, 30> history;

void setup() {
  pinMode(MOISTURE_POWER, OUTPUT);
  display.initialize();
  Serial.begin(9600);
}

void loop() {
  display.displayTime(myRTC.now());
  if (shouldMeasureMoisture()){
      int moisture = measureMoisture();   
      display.displayMoisture(moisture);
      int historyInput = calculateHistoryInput(moisture);
      history.push(historyInput);
      if(wateringDetected()){
        lastWatering = millis();
      }
      lastMeasurement = millis();
  }
  delay(1000);
}

bool shouldMeasureMoisture(){
  bool firstMeasurement = lastMeasurement==0L;
  bool timePassed = millis() - lastMeasurement > MEASUREMENT_FREQ;
  return firstMeasurement || timePassed;
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

int calculateHistoryInput(int moisture)
{
  if(history.isEmpty()){
    return moisture;
  }
  return history.last()*HISTORY_EMA_BETA+(1-HISTORY_EMA_BETA)*moisture;
}

int wateringDetected(){
  bool enoughData = history.isFull();
  bool wateringIntervalPassed = millis() - lastWatering > WATERING_INTERVAL;
  bool moistureIncrease = history.last() - history.first() > WATERING_INCREASE_THRESHOLD;
  return enoughData && wateringIntervalPassed && moistureIncrease;
}
