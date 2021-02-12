#include "Display.h"
#include <DS3231.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

Display::Display()
{
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
}

void Display::initialize()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        for (;;)
            ;
    }
    display.clearDisplay();
    display.setTextColor(WHITE, BLACK);
}

void Display::displayMoisture(int percentage)
{
    display.setTextSize(4);
    display.setCursor(0, 0);
    display.print("   ");
    display.setCursor(0, 0);
    display.print(min(percentage, 99));
    display.print("%");
    display.display();
}

void Display::displayTime(DateTime time)
{
    display.setTextSize(1);

    display.setCursor(80, 0);
    if (time.hour() < 10)
    {
        display.print('0');
    }
    display.print(time.hour());
    display.print(':');
    if (time.minute() < 10)
    {
        display.print('0');
    }
    display.print(time.minute());
    display.print(':');
    if (time.second() < 10)
    {
        display.print('0');
    }
    display.print(time.second());
    display.display();
}

void Display::displayLastWatering(uint32_t now, uint32_t wateringTime)
{
    display.setTextSize(1);
    display.setCursor(80, 11);
    display.print("Last wat:");
    display.setCursor(80, 22);

    uint32_t diff = now - wateringTime;
    int days = diff / 86400;
    int hours = (diff % 86400) / 3600;
    if (wateringTime == 0)
    {
        display.print("--------");
    }
    else if (days > 99)
    {
        display.print("99+ d");
    }
    else
    {
        display.print(days);
        display.print("d ");
        display.print(hours);
        display.print("h     ");
    }
    display.display();
}

void Display::displayPressTime(double percentage)
{
    display.fillRect(0, 31, SCREEN_WIDTH, 1, WHITE);
    display.fillRect(int(SCREEN_WIDTH * percentage), 31, SCREEN_WIDTH, 1, BLACK);
    display.display();
}