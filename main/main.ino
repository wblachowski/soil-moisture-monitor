#include <Wire.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <CircularBuffer.h>
#include "Display.h"
#include "Memory.h"
#include "TimeGuard.h"
#include "ButtonHandler.h"

Display display;
Memory memory;
RTClib myRTC;

#define MOISTURE_SENSOR A0
#define MOISTURE_POWER 10
#define BUTTON 2
#define BUTTON_INTERVAL 30L // Every 30ms
#define BUTTON_PRESS_DUR 2L*1000 // Two seconds
#define MEASUREMENT_INTERVAL 60L*1000 // Every minute
#define MEASUREMENT_DUR 100 // 50ms of power before measuring
#define CLOCK_INTERVAL 250L // Every 250ms
#define HISTORY_EMA_BETA 0.9 // Averaging over 10 measurements
#define WATERING_INTERVAL 3L*60*60 // At least three hours between waterings
#define WATERING_INCREASE_THRESHOLD 5 // At least 5 percent increase to detect watering

TimeGuard buttonGuard(BUTTON_INTERVAL);
TimeGuard measurementGuard(MEASUREMENT_INTERVAL);
TimeGuard clockGuard(CLOCK_INTERVAL);

uint32_t lastWatering = 0L;
CircularBuffer<int, 30> history;
ButtonHandler buttonHandler;

void setup() {
  pinMode(MOISTURE_POWER, OUTPUT);
  pinMode(BUTTON, INPUT);
  display.initialize();
  lastWatering = memory.readLastWatering();
  Serial.begin(9600);
}

void loop() {
  if(buttonGuard.execute(millis())){
      unsigned long pressTime = buttonHandler.pressTime(digitalRead(BUTTON), millis());
      display.displayPressTime(((double)pressTime)/(BUTTON_PRESS_DUR));
      if(pressTime >= BUTTON_PRESS_DUR){
        reactToWatering();
        buttonHandler.disableUntilChange();
      }
  }
  if(clockGuard.execute(millis())){
      display.displayTime(myRTC.now());
  }
  if (measurementGuard.execute(millis())){
      display.displayLastWatering(myRTC.now().unixtime(), lastWatering);
      int moisture = measureMoisture();   
      display.displayMoisture(moisture);
      int historyInput = calculateHistoryInput(moisture);
      history.push(historyInput);
      if(wateringDetected()){
        reactToWatering();
      }
  }
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
  bool wateringIntervalPassed = myRTC.now().unixtime() - lastWatering > WATERING_INTERVAL;
  bool moistureIncrease = history.last() - history.first() > WATERING_INCREASE_THRESHOLD;
  return enoughData && wateringIntervalPassed && moistureIncrease;
}

void reactToWatering(){
  lastWatering = myRTC.now().unixtime();
  memory.saveLastWatering(lastWatering);
  display.displayLastWatering(myRTC.now().unixtime(), lastWatering);
}
