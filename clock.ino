// C++ code
//
#include <Adafruit_LiquidCrystal.h>

int seconds = 0;

Adafruit_LiquidCrystal lcd_1(0);

unsigned long startMillis; // some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;

void MainMenu()
{
    currentMillis = millis();
    if ((currentMillis - startMillis) >= period)
    {
        Serial.println(seconds);
        startMillis = currentMillis;
        lcd_1.print(seconds);
        seconds++;
    }
}

    void setup()
    {
        startMillis = millis();
        lcd_1.setBacklight(1);
        lcd_1.begin(16, 2);
        lcd_1.print("hello world");
        serial.begin(9600);
    }

    void loop()
    {
        lcd_1.print(startMillis);
        lcd_1.setCursor(0, 1);
        MainMenu();
        lcd_1.print(currentMillis);
        
  
    }