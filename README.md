# Soil Moisture Monitor 🌱
> There's no reason to stick your finger in the dirt ever again.

This is an Arduino project for monitoring soil moisture to keep your plants lush and healthy. It comes with a standard moisture reading as well as automatic watering detection to let you know how long it's been since the last watering.

## Components
Originally, the project uses the following components (albeit many of these could be substituted with their counterparts without much effort):
* Arduino Nano
* LM393 Soil moisture sensor
* DS3231 RTC
* SSD1306 128x32 OLED Display

## Usage
The main aim of the monitor is to display the soil moisture to the user. However, there are a few add-ons to make the experience even better.

### Moisture sensing
The moisture is measured every minute by supplying the power to the sensor for 50ms and taking the measurement. The measurement in 0-100% format is then displayed in large font in the left part fo the display.

The reason for taking measurements at intervals instead of continuously is to slow down the sensor rusting (some rusting can't be prevented, sadly).

### Watering detection
The system measures the Exponential Moving Average of the sensor measurements and keeps track of the latest 30 values.
Based on some thresholds, the monitor detects whether a significant increase in the moisture has occurred.
Once watering is detected, the system saves the watering time in its non-volatile memory and, henceforth, displays the time that has passed since the last watering on the right-hand side of the display.
A new watering can't be detected until at least 3 hours since the last one has passed.
This behaviour aims to prevent multiple watering events from being detected if the moisture level keeps raising for an extended period of time after the actual watering.

### Watering triggering
If the user feels uncertain about the automatic watering detection mechanism, they can let the system know that the watering has happened by pressing a special button and holding it for 2 seconds.
This event is treated in the same way as the automatic detection.
