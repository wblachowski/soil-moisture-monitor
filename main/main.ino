#include <DS3231.h>
#include <CircularBuffer.h>
#include "Display.h"
#include "Memory.h"
#include "TimeGuard.h"
#include "ButtonHandler.h"

// Pinout:
#define MOISTURE_SENSOR A0
#define MOISTURE_POWER 10
#define BUZZER 9
#define BUTTON 0

// Constants:
#define BUTTON_PRESS_DUR 2L * 1000      // Button has to be pressed for 2s
#define BUTTON_DEBOUNCE 30L             // Button has to be held for at least 30ms
#define MEASUREMENT_INTERVAL 60L * 1000 // Moisture measured every minute
#define MEASUREMENT_DUR 50L             // Moisture measured after powering the sensor for 50ms
#define BUZZER_DUR 50L                  // Buzz for 30ms after watering detection
#define CLOCK_INTERVAL 250L             // Displayed time updated every 250ms
#define HISTORY_EMA_BETA 0.9            // EMA with averaging over 10 measurements
#define WATERING_INTERVAL 3L * 60 * 60  // At least 3h since last watering to detect a new one
#define WATERING_INCREASE_THRESHOLD 5   // At least 5 percent increase to detect watering
#define WATERING_WINDOW 30              // Increase detection window

Display display;
Memory memory;
RTClib myRTC;

TimeGuard measurementGuard(MEASUREMENT_INTERVAL);
TimeGuard clockGuard(CLOCK_INTERVAL);

uint32_t lastWatering = 0L;
CircularBuffer<int, WATERING_WINDOW> history;
ButtonHandler buttonHandler(BUTTON_DEBOUNCE);

void setup()
{
  pinMode(MOISTURE_POWER, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT);
  display.initialize();
  lastWatering = memory.readLastWatering();
  buzz();
  Serial.begin(9600);
}

void loop()
{
  unsigned long pressTime = buttonHandler.pressTime(digitalRead(BUTTON), millis());
  Serial.println(pressTime);
  display.displayPressTime(((double)pressTime) / (BUTTON_PRESS_DUR));
  if (pressTime >= BUTTON_PRESS_DUR)
  {
    reactToWatering();
    buttonHandler.disableUntilChange();
  }

  if (clockGuard.execute(millis()))
  {
    display.displayTime(myRTC.now());
  }
  if (measurementGuard.execute(millis()))
  {
    display.displayLastWatering(myRTC.now().unixtime(), lastWatering);
    int moisture = measureMoisture();
    display.displayMoisture(moisture);
    int historyInput = calculateHistoryInput(moisture);
    history.push(historyInput);
    if (wateringDetected())
    {
      reactToWatering();
    }
  }
}

int measureMoisture()
{
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
  if (history.isEmpty())
  {
    return moisture;
  }
  return history.last() * HISTORY_EMA_BETA + (1 - HISTORY_EMA_BETA) * moisture;
}

int wateringDetected()
{
  bool enoughData = history.isFull();
  bool wateringIntervalPassed = myRTC.now().unixtime() - lastWatering > WATERING_INTERVAL;
  bool moistureIncrease = history.last() - history.first() > WATERING_INCREASE_THRESHOLD;
  return enoughData && wateringIntervalPassed && moistureIncrease;
}

void reactToWatering()
{
  lastWatering = myRTC.now().unixtime();
  memory.saveLastWatering(lastWatering);
  display.displayLastWatering(myRTC.now().unixtime(), lastWatering);
  buzz();
}

void buzz()
{
  analogWrite(BUZZER, 255);
  delay(BUZZER_DUR);
  analogWrite(BUZZER, 0);
}
