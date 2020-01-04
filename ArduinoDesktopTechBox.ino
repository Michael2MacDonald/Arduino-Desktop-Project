#include <RotaryEncoder.h>
#include <LiquidCrystal.h>
#include <Bounce2.h>
#include <Wire.h>
#include "MainMenu.h"
#include "Clock.h"
#include "SubscriberCount.h"
#include "notes.h"

//LCD Pins//
const int rs = 22, en = 23, d4 = 24, d5 = 25, d6 = 26, d7 = 27;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int lcdBackLight = A2;
//Menu Stuff//
int modeState = 0;
int displayModeState = 0;
const int modeArrayItems = 2;
String modes[3]{"Menu", "Clock", "Sub Count"};
const int select = 4;
//Global Vars//
int MQ2Reading;
int LDRreading;
int Temp;
int Hunitity;
int Hour;
int Min;
bool recordedEmergencyTime = false;
//Rotary Encoder Stuff//
RotaryEncoder encoder(2, 3);
int pos = 0;
int newPos;
//Button Debounce//
Bounce debouncerS = Bounce();

void clearlcd(){
    lcd.setCursor(0,0);
    lcd.print(F("                "));
    lcd.setCursor(0,1);
    lcd.print(F("                "));
    lcd.setCursor(0,0);
}

bool hasRecordedStartTime = false;
bool hasRecordedPauseTime = false;
unsigned long timerStartTime;
unsigned long timeMillis;
unsigned long timerTimeMillis;
bool paused = false;
void timer(){
  timeMillis = millis();
  if(TimerOn == true){
    if(!hasRecordedStartTime){timerStartTime = timeMillis; hasRecordedStartTime = true;}
    timerTimeMillis = (clockFunctionsVars[4] * 360000)+(clockFunctionsVars[5] * 6000)+(clockFunctionsVars[6] * 100);
    if(timeMillis - timerStartTime == timerTimeMillis){
      
    }
  }
  else if(TimerOn == true && paused){
    if(!hasRecordedPauseTime){
      timerTimeMillis = timerTimeMillis - (timeMillis - timerStartTime);
      hasRecordedPauseTime = true;
    }
  }
}

void CO_Alert(){
  while(1){
    DateTime now = rtc.now();
    if(recordedEmergencyTime == false){
      Hour = now.hour();
      Min = now.minute();
      recordedEmergencyTime = true;
    }
    clearlcd();
    lcd.setCursor(0,0);
    lcd.print(F("CO LEVEL ALERT!!"));
    lcd.setCursor(0,1);
    if (Hour > 12){
      lcd.print(F("PM "));
      lcd.print(Hour-12, DEC);
    }
    else{
      lcd.print(F("AM "));
      lcd.print(Hour);
    }
    lcd.print(':');
    lcd.print(Min, DEC);
  }
}

int encoderP(){
  newPos = encoder.getPosition();
  if (pos < newPos) {
    pos = newPos;
    return true;
  } 
  else{
    return false;
  }
}

int encoderM(){
  newPos = encoder.getPosition();
  if (pos > newPos) {
    //Serial.print(newPos);
    //Serial.println();
    pos = newPos;
    return true;
  } 
  else{
    return false;
  }
}

void encoderRead() {
  encoder.tick();
}

int alwaysRun(){
  MQ2();
  radioFunctions();
  timer();
}

void setup() {
  analogWrite(lcdBackLight,255);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  Serial.begin(9600);
  clockSetup();
  MainMenuTechBoxSetup();
  //MQ2setup();
  radioSetup();
  debouncerS.attach(select,INPUT_PULLUP);
  debouncerS.interval(25);
  attachInterrupt(digitalPinToInterrupt(2), encoderRead, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), encoderRead, CHANGE);
  //PCICR |= (1 << PCIE0);
  //PCMSK0 |= (1 << PCINT4) | (1 << PCINT5);
  }

void loop() {
  debouncerS.update();
  lcd.print(F("                "));
  lcd.setCursor(0,0);
  lcd.print(F("Select A Mode:"));
  lcd.setCursor(0,1);
  lcd.print(modes[displayModeState]);
  if(debouncerS.fell() && displayModeState == 2){
    subCount();
  }
  if(debouncerS.fell() && displayModeState == 0){
    menu();
  }
  if(debouncerS.fell() && displayModeState == 1){
    Clock();
  }
  if(encoderP()){
    displayModeState++;
  }
  if(encoderM()){
    displayModeState--;
  }
  if(displayModeState < modeArrayItems - modeArrayItems){
    displayModeState = modeArrayItems;
  }
  if(displayModeState > 2){
    displayModeState = 0;
  }
  if(MQ2() == true){
      CO_Alert();
    }
  alwaysRun();
}
