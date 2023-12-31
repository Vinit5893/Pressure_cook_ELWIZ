// C++ code
//
#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>

// These 5 arrays paint the bars that go across the screen and print the degree symbol.
byte zero[] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
byte one[] = {B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000};
byte two[] = {B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000};
byte three[] = {B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100};
byte four[] = {B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110};
byte five[] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte degree[] = {B11100, B10100, B11100, B00000, B00000, B00000, B00000, B00000};

int seconds = 0;
int a = 2048;
int b = 2048;
int pos = 0;
int mode_select = 2048;
int cup_select = 2048;
char rice_type[] = " ";
char Veg_type[] = " ";
int cups_value = 0;
int cook_done = 0;
int heat_avg = 0;

bool isStoring = 0;

const int pot = A0;
const int temp_in = A1;
const int temp_out = A2;
const int buzzer = 4;
const int btn = 2;
const int heater = 5;
const int rst = 6;
Servo servo_3;

Adafruit_LiquidCrystal lcd(0);

// ---------------drain water-------------

void drain_servo(int drain_time)
{
    for (pos = 0; pos <= 180; pos += 1)
    {
        servo_3.write(pos);
        delay(15);
    }
    delay(drain_time);
    for (pos = 180; pos >= 0; pos -= 1)
    {
        servo_3.write(pos);
        delay(15);
    }
}

int gettemperature()
{
    // int temp1 = analogRead(temp_out);    // Reading data from the sensor.This voltage is stored as a 10bit number.
    // int voltage1 = (tmp1 * 5.0)/1024;    //(5*temp)/1024 is to convert the 10 bit number to a voltage reading.
    // int milliVolt1 = voltage1 * 1000;    //This is multiplied by 1000 to convert it to millivolt.
    // int temp1 =  int((milliVolt1-500)/10) ;    //This is then subtracted by 500 to get the actual temperature in millivolt and divided by 10 to get the temperature in degree celsius.

    // int tmp2 = analogRead(temp_out);
    // int voltage2 = (tmp2 * 5.0)/1024;
    // int milliVolt2 = voltage2 * 1000;
    // int temp2 =  int((milliVolt2-500)/10) ;

    int temp1 = int(((analogRead(temp_out)) * 0.48828125) - 50);
    int temp2 = int(((analogRead(temp_in)) * 0.48828125) - 50);
    heat_avg = int((temp1 + temp2) / 2.0);
    lcd.setCursor(6, 0);
    lcd.print("Temp:");
    lcd.setCursor(11, 0);
    lcd.print(heat_avg < 100 ? " " : ""); // Print a space if heat_avg is less than 100
    lcd.print(heat_avg < 10 ? " " : "");  // Print a space if heat_avg is less than 10
    lcd.print(heat_avg);
    lcd.setCursor(14, 0);
    lcd.write(6);
    lcd.print("C");
    return heat_avg;
}

void clock(int selecttime)
{
    int seconds = 59;         // Start with 59 seconds for a minute-like countdown
    int min = selecttime - 1; // Start with selecttime - 1 minutes
    int i = 0;
    int totalsec1 = selecttime * 60;

    while (min >= 0)
    {
        if (seconds > 0)
        {
            lcd.setCursor(0, 0);
            lcd.print(min < 10 ? "0" : "");
            lcd.print(min);
            lcd.setCursor(2, 0);
            lcd.print(":");
            lcd.setCursor(3, 0);
            lcd.print(seconds < 10 ? "0" : "");
            lcd.print(seconds);
            seconds -= 1;
            delay(1000);
        }

        if (seconds == 0)
        {
            seconds = 59;
            lcd.setCursor(0, 0);
            lcd.print(min < 10 ? "0" : "");
            lcd.setCursor(3, 0);
            lcd.print("00");
            min -= 1;
            delay(1000);
        }
        gettemperature();
        updateProgressBar(i, totalsec1, 1);
        i += 1;
    }
}

void dummyclock()
{
    while (1)
    {
        gettemperature();
          if (heat_avg >= 90)
          {
              break; // Exit the loop if heat_avg is greater than 90
          }
        delay(1000);
    }
}

void cooking_done()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cooking Done");
  	lcd.setCursor(0, 1);
  	lcd.print("Press rst to OFF");
    mode_select = 2048;
    cup_select = 2048;
    a = 2048;
    b = 2048;
    
  
  	analogWrite(heater, 11); // heater turned on at 45 degree celcius until push rst is pressed
    drain_servo(5000);
  
  
    digitalWrite(buzzer, HIGH); // Buzzer and Led Indication
    delay(2000);	
  	digitalWrite(buzzer, LOW);
  
    while (digitalRead(rst) == 0)
    {}   // stay here untill the rst btn is pressed.
 
}

void rice_select(char rice_type[], int cups_value, int time)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(rice_type);
    lcd.setCursor(0, 1);
    lcd.print("Cups : ");
    lcd.setCursor(7, 1);
    lcd.print(cups_value);
    analogWrite(heater, 255); // heater on
    lcd.clear();
    clock(time); // Timer for 35 mins

    cooking_done();
}

void vegetable_select(char veg_type[], int preheattime, int time)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(veg_type);
    lcd.setCursor(0, 1);
    lcd.print("Preating 5-9 mins");
    analogWrite(heater, 255); // heater on
    lcd.clear();
    dummyclock();       // proceed to next step when temp reaches 90 degree celcius
   
  
    clock(preheattime); // Timer for 5-9 mins
  
    digitalWrite(buzzer, HIGH); //preheating done buzzer
    delay(5000);
    digitalWrite(buzzer, LOW);
  
   //---------------------------------
  
   //loading time
     while(digitalRead(btn) == 0)
     {
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("LOAD & PRESS");
     }
  
   //---------------------------------

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cooking");
    analogWrite(heater, 255); // Active cooking
    lcd.clear();
    clock(time); // Timer for 12 mins

    cooking_done();
}

void updateProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn)
{
    // map(count,0,totalCount,0,100);
    // map(totalCount,0,totalCount,0,100);
    float factor = totalCount / 80.0;
    int percent = (count + 1) / factor;
    int number = percent / 5;
    int remainder = percent % 5;
    if (number > 0)
    {
        lcd.setCursor(number - 1, lineToPrintOn);
        lcd.write(5);
    }
    lcd.setCursor(number, lineToPrintOn);
    lcd.write(remainder);
}

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

void setup()
{
    lcd.begin(16, 2);
    lcd.print("Smart Cooker");
    delay(1000);
    lcd.createChar(0, zero);
    lcd.createChar(1, one);
    lcd.createChar(2, two);
    lcd.createChar(3, three);
    lcd.createChar(4, four);
    lcd.createChar(5, five);
    lcd.createChar(6, degree);
    lcd.clear();

    pinMode(btn, INPUT_PULLUP); // selection button
    pinMode(rst, INPUT_PULLUP); // reset button
    pinMode(heater, OUTPUT);    // heater
    pinMode(buzzer, OUTPUT);    // buzzer

    servo_3.attach(3, 500, 2500);
    servo_3.write(0);

    Serial.begin(9600);
}

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

void loop()
{
    if (!isStoring)
    {
        char modes[14][20] = {"White Rice", "Black Rice", "Brown Rice", "Red Rice", "Quinoa Rice",
                              "Basmati Rice", "Single B Rice", "Double B Rice", "Raw Rice",
                              "Carrot", "Cabbage", "Green Peas", "Spinach", "Potato"};

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(modes[map(analogRead(pot), 0, 1024, 0, 14)]);
    }

    else
    {
        char cups[4][9] = {"Cups : 1", "Cups : 2", "Cups : 3", "Cups : 4"};

        lcd.setCursor(0, 1);
        lcd.print(cups[map(analogRead(pot), 0, 1024, 0, 4)]);
    }
    // delay(500);

    //-------------------------------------------------------------------------------------

    int rst_btn = digitalRead(rst);
  
    if (rst_btn == 1)
    {
        analogWrite(heater, 0);
        isStoring = 0;
        mode_select = 2048;
        cup_select = 2048;
        a = 2048;
        b = 2048;
        digitalWrite(buzzer, LOW);
    }

    else
    {
        int buttonState = digitalRead(btn);

        if (buttonState == HIGH)
        {
            isStoring = !isStoring;
            delay(1000); // Debounce delay

            if (isStoring)
            {
                a = analogRead(pot); // mode
            }

            else
            {
                b = analogRead(pot); // cup
            }
        }

        mode_select = a;
        cup_select = b;
        // Serial.print("Mode : ");
        // Serial.println(mode_select);
        // Serial.print("Cups : ");
        // Serial.println(cup_select);

        delay(100);

        // |||||||||||||||||||||||||||||||||RICE||||||||||||||||||||||||||||||||||

        // ------------------------------Mode1--------------------------------

        // (Name_mode, No. of cup, Time)

        // ---------------Cup1----------------
        if (mode_select >= 0 && mode_select <= 73 && cup_select >= 0 && cup_select <= 255)
        {
            rice_select("White Rice", 1, 1); // (Name_mode, No. of cup, Time) 32 min
        }
        // ---------------Cup2----------------
        else if (mode_select >= 0 && mode_select <= 73 && cup_select >= 256 && cup_select <= 511)
        {
            rice_select("White Rice", 2, 2); // time 35 min
        }
        // ---------------Cup3----------------
        else if (mode_select >= 0 && mode_select <= 73 && cup_select >= 512 && cup_select <= 767)
        {
            rice_select("White Rice", 3, 3); // time 38 min
        }
        // ---------------Cup4----------------
        else if (mode_select >= 0 && mode_select <= 73 && cup_select >= 768 && cup_select <= 1023)
        {
            rice_select("White Rice", 4, 4); // time 42 min
        }

        // ---------------------------------Mode2--------------------------------

        // ---------------Cup1----------------
        else if (mode_select >= 74 && mode_select <= 146 && cup_select >= 0 && cup_select <= 255)
        {
            rice_select("Black Rice", 1, 55); // time 55 min
        }
        // ---------------Cup2----------------
        else if (mode_select >= 74 && mode_select <= 146 && cup_select >= 256 && cup_select <= 511)
        {
            rice_select("Black Rice", 2, 57); // time 57 min
        }
        // ---------------Cup3----------------
        else if (mode_select >= 74 && mode_select <= 146 && cup_select >= 512 && cup_select <= 767)
        {
            rice_select("Black Rice", 3, 61); // time 61 min
        }
        // ---------------Cup4----------------
        else if (mode_select >= 74 && mode_select <= 146 && cup_select >= 768 && cup_select <= 1023)
        {
            rice_select("Black Rice", 4, 0); // time 0 min
            lcd.clear();
            lcd.print("Cannot cook");
        }

        // ---------------------------------Mode3--------------------------------

        // ---------------Cup1----------------
        else if (mode_select >= 147 && mode_select <= 219 && cup_select >= 0 && cup_select <= 255)
        {
            rice_select("Brown Rice", 1, 55); // time 55 min
        }
        // ---------------Cup2----------------
        else if (mode_select >= 147 && mode_select <= 219 && cup_select >= 256 && cup_select <= 511)
        {
            rice_select("Brown Rice", 2, 57); // time 57 min
        }
        // ---------------Cup3----------------
        else if (mode_select >= 147 && mode_select <= 219 && cup_select >= 512 && cup_select <= 767)
        {
            rice_select("Brown Rice", 3, 61); // time 61 min
        }
        // ---------------Cup4----------------
        else if (mode_select >= 147 && mode_select <= 219 && cup_select >= 768 && cup_select <= 1023)
        {
            rice_select("Brown Rice", 4, 0); // time 0 min
            lcd.clear();
            lcd.print("Cannot cook");
        }

        // ---------------------------------Mode4--------------------------------

        // ---------------Cup1----------------
        else if (mode_select >= 220 && mode_select <= 292 && cup_select >= 0 && cup_select <= 255)
        {
            rice_select("Red Rice", 1, 55); // time 55 min
        }
        // ---------------Cup2----------------
        else if (mode_select >= 220 && mode_select <= 292 && cup_select >= 256 && cup_select <= 511)
        {
            rice_select("Red Rice", 2, 57); // time 57 min
        }
        // ---------------Cup3----------------
        else if (mode_select >= 220 && mode_select <= 292 && cup_select >= 512 && cup_select <= 767)
        {
            rice_select("Red Rice", 3, 63); // time 63 min
        }
        // ---------------Cup4----------------
        else if (mode_select >= 220 && mode_select <= 292 && cup_select >= 768 && cup_select <= 1023)
        {
            rice_select("Red Rice", 4, 0); // time 0 min
            lcd.clear();
            lcd.print("Cannot cook");
        }

        // ---------------------------------Mode5--------------------------------

        // ---------------Cup1----------------
        else if (mode_select >= 293 && mode_select <= 365 && cup_select >= 0 && cup_select <= 255)
        {
            rice_select("Quinoa Rice", 1, 42); // time 42 min
        }
        // ---------------Cup2----------------
        else if (mode_select >= 293 && mode_select <= 365 && cup_select >= 256 && cup_select <= 511)
        {
            rice_select("Quinoa Rice", 2, 44); // time 44 min
        }
        // ---------------Cup3----------------
        else if (mode_select >= 293 && mode_select <= 365 && cup_select >= 512 && cup_select <= 767)
        {
            rice_select("Quinoa Rice", 3, 44); // time 44 min
        }
        // ---------------Cup4----------------
        else if (mode_select >= 293 && mode_select <= 365 && cup_select >= 768 && cup_select <= 1023)
        {
            rice_select("Quinoa Rice", 4, 47); // time 47 min
        }

        // ---------------------------------Mode6--------------------------------

        // ---------------Cup1----------------
        else if (mode_select >= 366 && mode_select <= 438 && cup_select >= 0 && cup_select <= 255)
        {
            rice_select("Basmati Rice", 1, 15); // time 15 min
        }
        // ---------------Cup2----------------
        else if (mode_select >= 366 && mode_select <= 438 && cup_select >= 256 && cup_select <= 511)
        {
            rice_select("Basmati Rice", 2, 18); // time 18 min
        }
        // ---------------Cup3----------------
        else if (mode_select >= 366 && mode_select <= 438 && cup_select >= 512 && cup_select <= 767)
        {
            rice_select("Basmati Rice", 3, 22); // time 22 min
        }
        // ---------------Cup4----------------
        else if (mode_select >= 366 && mode_select <= 438 && cup_select >= 768 && cup_select <= 1023)
        {
            rice_select("Basmati Rice", 4, 26); // time 26 min
        }

        // ---------------------------------Mode7--------------------------------

        // ---------------Cup1----------------
        else if (mode_select >= 439 && mode_select <= 511 && cup_select >= 0 && cup_select <= 255)
        {
            rice_select("Single B Rice", 1, 32); // time 32 min
        }
        // ---------------Cup2----------------
        else if (mode_select >= 439 && mode_select <= 511 && cup_select >= 256 && cup_select <= 511)
        {
            rice_select("Single B Rice", 2, 35); // time 35 min
        }
        // ---------------Cup3----------------
        else if (mode_select >= 439 && mode_select <= 511 && cup_select >= 512 && cup_select <= 767)
        {
            rice_select("Single B Rice", 3, 38); // time 38 min
        }
        // ---------------Cup4----------------
        else if (mode_select >= 439 && mode_select <= 511 && cup_select >= 768 && cup_select <= 1023)
        {
            rice_select("Single B Rice", 4, 42); // time 42 min
        }

        // ---------------------------------Mode8--------------------------------

        // ---------------Cup1----------------
        else if (mode_select >= 512 && mode_select <= 584 && cup_select >= 0 && cup_select <= 255)
        {
            rice_select("Double B Rice", 1, 35); // time 35 min
        }
        // ---------------Cup2----------------
        else if (mode_select >= 512 && mode_select <= 584 && cup_select >= 256 && cup_select <= 511)
        {
            rice_select("Double B Rice", 2, 38); // time 38 min
        }
        // ---------------Cup3----------------
        else if (mode_select >= 512 && mode_select <= 584 && cup_select >= 512 && cup_select <= 767)
        {
            rice_select("Double B Rice", 3, 42); // time 42 min
        }
        // ---------------Cup4----------------
        else if (mode_select >= 512 && mode_select <= 584 && cup_select >= 768 && cup_select <= 1023)
        {
            rice_select("Double B Rice", 4, 46); // time 46 min
        }

        // ---------------------------------Mode9--------------------------------

        // ---------------Cup1----------------
        else if (mode_select >= 585 && mode_select <= 657 && cup_select >= 0 && cup_select <= 255)
        {
            rice_select("Raw Rice", 1, 20); // time 20 min
        }
        // ---------------Cup2----------------
        else if (mode_select >= 585 && mode_select <= 657 && cup_select >= 256 && cup_select <= 511)
        {
            rice_select("Raw Rice", 2, 23); // time 23 min
        }
        // ---------------Cup3----------------
        else if (mode_select >= 585 && mode_select <= 657 && cup_select >= 512 && cup_select <= 767)
        {
            rice_select("Raw Rice", 3, 27); // time 27 min
        }
        // ---------------Cup4----------------
        else if (mode_select >= 585 && mode_select <= 657 && cup_select >= 768 && cup_select <= 1023)
        {
            rice_select("Raw Rice", 4, 31); // time 31 min
        }

        // |||||||||||||||||||||||||||||||||VEGETABLE||||||||||||||||||||||||||||||||||

        // (Name, Pre Heating Time (5-9 mins), Steaming Time)

        // ---------------------------------Mode10--------------------------------

        else if (mode_select >= 658 && mode_select <= 730)
        {
            vegetable_select("Carrot", 1, 1); // (Name, Pre Heating Time (5-9 mins), Steaming Time) 12 min
        }

        // ---------------------------------Mode11--------------------------------
        else if (mode_select >= 731 && mode_select <= 803)
        {
            vegetable_select("Cabbage", 6, 11); // time 11 min
        }

        // ---------------------------------Mode12--------------------------------
        else if (mode_select >= 804 && mode_select <= 876)
        {
            vegetable_select("Green Peas", 6, 8); // time 8 min
        }

        // ---------------------------------Mode13--------------------------------
        else if (mode_select >= 877 && mode_select <= 949)
        {
            vegetable_select("Spinach", 6, 3); // time 3 min
        }

        // ---------------------------------Mode14--------------------------------
        else if (mode_select >= 950 && mode_select <= 1022)
        {
            vegetable_select("Potato", 6, 12); // time 12 min
        }
    }
}
