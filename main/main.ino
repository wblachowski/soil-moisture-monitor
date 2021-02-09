#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS3231.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// SSD1306 display connected to I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTClib myRTC;

int MOISTURE_SENSOR = A0;
int MOISTURE_POWER = 10;
unsigned long MEASUREMENT_FREQ = 60L*1000; // Every minute
int MEASUREMENT_DUR = 100; // 50ms of power before measuring
unsigned long lastMeasurement = 0L;

void setup() {
  pinMode(MOISTURE_POWER, OUTPUT);
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
}

void loop() {
  displayTime();
  if (lastMeasurement==0L || millis() - lastMeasurement > MEASUREMENT_FREQ){
      analogWrite(MOISTURE_POWER, 255);
      delay(MEASUREMENT_DUR);
      int sensorValue = analogRead(MOISTURE_SENSOR);
      analogWrite(MOISTURE_POWER, 0);
      int percent = convertToPercent(sensorValue);   
      displayValue(percent);
      lastMeasurement = millis();
  }
  delay(1000);
}

int convertToPercent(int value)
{
  return map(value, 1023, 0, 0, 100);
}

void displayValue(int percent)
{
  display.setTextSize(4);
  display.setCursor(0, 0);
  display.print("   ");
  display.setCursor(0, 0);
  display.print(percent);
  display.print("%");
  display.display(); 
}

void displayTime()
{
  DateTime now = myRTC.now();
  display.setTextSize(1);
    
  display.setCursor(80, 0);
      if(now.hour()<10){
    display.print('0');
  }
  display.print(now.hour());
  display.print(':');
  if(now.minute()<10){
    display.print('0');
  }
  display.print(now.minute());
  display.print(':');
   if(now.second()<10){
    display.print('0');
  }
  display.print(now.second());

  display.setCursor(80, 15);
  if(now.day()<10){
    display.print('0');
  }
  display.print(now.day());
  display.print('.');
  if(now.month()<10){
    display.print('0');
  }
  display.print(now.month());
  display.print('.');
  display.print(now.year()%100);
  display.display();
}
