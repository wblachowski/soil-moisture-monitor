#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// SSD1306 display connected to I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int MOISTURE_SENSOR = A0;
int MOISTURE_POWER = 10;
long MEASUREMENT_FREQ = 60L*1000; // Every minute
int MEASUREMENT_DUR = 100; // 50ms of power before measuring

void setup() {
  pinMode(MOISTURE_POWER, OUTPUT);
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
}

void loop() {
  analogWrite(MOISTURE_POWER, 255);
  delay(MEASUREMENT_DUR);
  int sensorValue = analogRead(MOISTURE_SENSOR);
  analogWrite(MOISTURE_POWER, 0);
  
  Serial.println(sensorValue);
  int percent = convertToPercent(sensorValue);
  displayValue(percent);
  Serial.println(percent);
  delay(MEASUREMENT_FREQ);
}

int convertToPercent(int value)
{
  return map(value, 1023, 0, 0, 100);
}

void displayValue(int percent)
{
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print(percent);
  display.print("%");
  display.display(); 
}
