#include <LiquidCrystal.h>
#include <Wire.h>
#include "SubMenus.h"
//pins
extern int alwaysRun();
extern LiquidCrystal lcd;
extern void CO_Alert();
extern Bounce debouncerS;
extern int encoderP();
extern int encoderM();
//menu
extern void clearlcd();
int activeMenuState = 0;
int menuState = 0;
const int menuArrayItems = 7;
String menuStates[8] = {"Toggle LED", "Sensor Data", "Settings", "About Creator", "Calibrate MQ-2", "Games", "Clock Functions", "Exit"};

int MainMenuTechBoxSetup(){
  subMenuSetup();
}

void menu(){
  while(1){
    debouncerS.update();
    lcd.print(F("                "));
    lcd.setCursor(0,0);
    lcd.print(F("                "));
    lcd.setCursor(0,0);
    lcd.print(F("Menu:"));
    lcd.setCursor(0,1);
    lcd.print(menuStates[menuState]);
      
    if(debouncerS.fell() && menuState == 0){
      activeMenuState = 1;
    }
    if(activeMenuState == 1){
      toggleLed();
    }
    if(debouncerS.fell() && menuState == 1){
      activeMenuState = 2;
    }
    if(activeMenuState == 2){
      SensorData();
    }
    if(debouncerS.fell() && menuState == 2){
      activeMenuState = 3;
    }
    if(activeMenuState == 3){
      Settings();
    }
    if(debouncerS.fell() && menuState == 3){
      activeMenuState = 4;
    }
    if(activeMenuState == 4){
      aboutCreator();
    }
    if(debouncerS.fell() && menuState == 4){
      activeMenuState = 5;
    }
    if(activeMenuState == 5){
      MQ2setup();
      activeMenuState = 0;
    }
    if(debouncerS.fell() && menuState == 5){
      activeMenuState = 6;
    }
    if(activeMenuState == 6){
      SubMenu();
    }
    if(debouncerS.fell() && menuState == 6){
      activeMenuState = 7;
    }
    if(activeMenuState == 7){
      clockFunctions();
    }
    if(encoderP()){
      menuState++;
    }
    if(encoderM()){
      menuState--;
    }
    if(menuState > menuArrayItems){
      menuState = 0;
    }
    if(menuState < menuArrayItems - menuArrayItems){
      menuState = menuArrayItems;
    }
    if(debouncerS.fell() && menuState == 7){
      menuState = 0;
      return;
    }
    if(MQ2() == true){
      CO_Alert();
      menuState = 0;
      activeMenuState = 0;
      return;
    }
    alwaysRun();
  }
}
