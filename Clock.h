#include <LiquidCrystal.h>
#include <RTClib.h> //include Adafruit RTC library
#include <Wire.h>
extern int alwaysRun();
extern LiquidCrystal lcd;
extern Bounce debouncerS;
extern void CO_Alert();

RTC_DS3231 rtc;

//Set the names of days
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

int clockSetup(){  
 if (! rtc.begin()) {
   lcd.clear();
   lcd.print(F("Couldn't find RTC"));
   while (1);
 }
 //Setup of time if RTC lost power or time is not set
 if (rtc.lostPower()) {
   //Sets the code compilation time to RTC DS3231
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
 }
}

void Clock(){
 while(1){
   debouncerS.update();
   //Set now as RTC time
   DateTime now = rtc.now();
   lcd.setCursor(0,0);
   if (now.hour() > 12){
    lcd.print(F("PM "));
    lcd.print(now.hour()-12, DEC);
   }
   else{
    lcd.print(F("AM "));
    lcd.print(now.hour());
   }
   lcd.print(':');
   lcd.print(now.minute(), DEC);
   lcd.print(':');
   lcd.print(now.second(), DEC);
   lcd.print(F("       "));
   lcd.setCursor(0,1);
   //Print RTC time to LCD
   lcd.print(now.year(), DEC);
   lcd.print('/');
   lcd.print(now.month(), DEC);
   lcd.print('/');
   lcd.print(now.day(), DEC);
   lcd.print('(');
   lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
   lcd.print(") ");
   if(debouncerS.fell()){
      return;
   }
   if(MQ2() == true){
      CO_Alert();
      return;
    }
  alwaysRun();
 }
}
