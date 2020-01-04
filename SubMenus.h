#include <Wire.h>
#include "Radio.h"
#include "MQ2.h"
#include "Game.h"
//=====GLOBAL VARS AND FUNCTIONS=====//
extern int alwaysRun();
extern void CO_Alert();
extern Bounce debouncerS;
extern int encoderP();
extern int encoderM();
extern void clearlcd();
extern int activeMenuState;

//=====SETUP=====//
void subMenuSetup(){
}

//==============================//
//===== SUB MENU LEVEL ONE =====//
//==============================//

//=====TOGGLE LED=====//
const int led = 8;
int ledState = LOW;
bool ledExitState = false;
//Function//
void toggleLed(){
  while(activeMenuState == 1){
    debouncerS.update();
    lcd.setCursor(0, 0);
    lcd.print(F("Toggle LED"));
    lcd.setCursor(1,1);
    if(ledState == HIGH){
      lcd.print(F("On "));
    }
    else{
      lcd.print(F("Off"));
    }
    lcd.setCursor(5,1);
    lcd.print(F("Exit "));
    if(ledExitState == 0){
      lcd.setCursor(0,1);
      lcd.print(F(">"));
      lcd.setCursor(4,1);
      lcd.print(F(" "));
    }
    if(ledExitState == true){
      lcd.setCursor(0,1);
      lcd.print(F(" "));
      lcd.setCursor(4,1);
      lcd.print(F(">"));
    }
    if(debouncerS.fell() && ledExitState == false){
      ledState = !ledState;
      digitalWrite(led, ledState);
    }
    if(encoderM() || encoderP()){
      ledExitState = !ledExitState;
    }
    if(debouncerS.fell() && ledExitState == true){
      activeMenuState = 0;
      ledExitState = false;
      return;
    }
    if(MQ2() == true){
      CO_Alert();
      ledExitState = false;
      activeMenuState = 0;
      return;
    }
    alwaysRun();
  }
}

//=====SENSOR DATA=====//
//Menu stuff and data values//
int DataState = 0;
const int DataItems = 5;
String DataName[6] = {"MQ-2 Analog", "CO PPM", "LDR", "X", "Y", "Exit"};
int DataValues[6] = {0, 0, 0, 0, 0, 0};
//Pins//
const int MQ2pin = A0;
const int LDR = A1;
const int DHTPIN = A3;
//Vars For Data//
extern int MQ2Reading;
extern int LDRreading;
extern int Hunitity;
extern int Temp;
//Timing So Data Is Printed Slower//
unsigned long previousMillis = 0;
const long interval = 250;
//Function//
void SensorData(){
   while(activeMenuState == 2){
    debouncerS.update();
    MQ2Reading = analogRead(MQ2pin);
    LDRreading = analogRead(LDR);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      DataValues[0] = MQ2Reading;
      DataValues[1] = iPPM_CO;
      DataValues[2] = LDRreading;
    }
    DataValues[3] = data.X;
    DataValues[4] = data.Y;
    
    clearlcd();
    lcd.print(DataName[DataState]);
    lcd.setCursor(0,1);
    if(DataState != 5){lcd.print(DataValues[DataState]);}
    else{lcd.print(F("Exit"));}
    if(encoderP()){
      DataState++;
    }
    if(encoderM()){
      DataState--;
    }
    if(DataState > DataItems){
      DataState = 0;
    }
    if(DataState < DataItems - DataItems){
      DataState = DataItems;
    }
    if(debouncerS.fell() && DataState == 5){
      DataState = 0;
      activeMenuState = 0;
      return;
    }
    if(MQ2() == true){
      CO_Alert();
      DataState = 0;
      activeMenuState = 0;
      return;
    }
    alwaysRun();
  }
}

//=====CLOCK FUNCTIONS=====//
//Menu stuff and clock values//
int clockFunctionsState = 0;
int setAlarmState = 0;
const int clockFunctionsItems = 7;
const int alarmItems = 4;
String setAlarmNames[5] = {"Alarm Days", "Alarm Hour", "Alarm Minute", "Alarm AM / PM", "Done"};
String clockFunctionsNames[8] = {"Toggle Alarm", "Toggle Timer", "Pause Timer", "Set Alarm", "Timer Hour", "Timer Minute", "Timer Second", "Exit"};
int clockFunctionsVars[8] = {0,0,0,0,0,0,0,0};
int setAlarmVars[5] = {0,0,0,0,0};
bool ChangeClockFunctions = false;
bool setAlarm = false;
bool setAlarmDays = false;
bool AlarmOn = false;
bool TimerOn = false;
bool AlarmPM = false;
extern bool paused;
//Function//
void clockFunctions(){
  while(activeMenuState == 7){
    debouncerS.update();
    clearlcd();
    lcd.print(clockFunctionsNames[clockFunctionsState]);
    lcd.setCursor(0,1);
    if(clockFunctionsState != 0 && clockFunctionsState != 1 && clockFunctionsState != 7 && clockFunctionsState != 3 && clockFunctionsState != 2){lcd.print(clockFunctionsVars[clockFunctionsState]);}
    else if(clockFunctionsState == 7){lcd.print(F("Exit"));}
    else if(clockFunctionsState == 0){if(AlarmOn == true){lcd.print(F("Alarm On"));}if(AlarmOn == false){lcd.print(F("Alarm Off"));}}
    else if(clockFunctionsState == 1){if(TimerOn == true){lcd.print(F("Timer On"));}if(TimerOn == false){lcd.print(F("Timer Off"));}}
    else if(clockFunctionsState == 2){if(TimerOn == true && paused == true){lcd.print(F("Timer Paused"));}if(TimerOn == true && paused == false){lcd.print(F("Timer Going"));}if(TimerOn == false){lcd.print(F("Can't Pause!"));}}
    else if(clockFunctionsState == 3){lcd.print(F("Set Alarm"));}
    if(encoderP()){
      clockFunctionsState++;
    }
    if(encoderM()){
      clockFunctionsState--;
    }
    if(debouncerS.fell() && clockFunctionsState == 0){
      AlarmOn = !AlarmOn;
    }
    if(debouncerS.fell() && clockFunctionsState == 1){
      TimerOn = !TimerOn;
    }
    if(debouncerS.fell() && clockFunctionsState == 2){
      paused = !paused;
    }
    if(debouncerS.fell() && (clockFunctionsState == 4 || clockFunctionsState == 5 || clockFunctionsState == 6)){
      ChangeClockFunctions = true;
    }
    while(ChangeClockFunctions == true){
      debouncerS.update();
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(clockFunctionsVars[clockFunctionsState]);
      lcd.print("   ");
      if(encoderP()){
        clockFunctionsVars[clockFunctionsState]++;
      }
      if(encoderM()){
        clockFunctionsVars[clockFunctionsState]--;
      }
      if(clockFunctionsVars[4] < 0){
        clockFunctionsVars[4] = 0;
      }
      if(clockFunctionsVars[5] > 59){
        clockFunctionsVars[5] = 0;
      }
      if(clockFunctionsVars[5] < 0){
        clockFunctionsVars[5] = 59;
      }
      if(clockFunctionsVars[6] > 59){
        clockFunctionsVars[6] = 0;
      }
      if(clockFunctionsVars[6] < 0){
        clockFunctionsVars[6] = 59;
      }
      if(debouncerS.fell()){
        ChangeClockFunctions = false;
        return;
      }
    }
    while(setAlarm == true){
      debouncerS.update();
      lcd.setCursor(0,0);
      lcd.print(setAlarmNames[setAlarmState]);
      lcd.print("                ");
      lcd.setCursor(0,1);
      if(setAlarmState != (0,3,4)){lcd.print(setAlarmVars[setAlarmState]);}
      else if(setAlarmState == 4){lcd.print("Done");}
      else if(setAlarmState == 0){lcd.print("                ");}
      else if(setAlarmState == 3){
        if(AlarmPM == true){
          lcd.print(F("PM"));
        }
        if(AlarmPM == false){
          lcd.print(F("AM"));
        }
      }
      if(debouncerS.fell() && setAlarmState == 3){
        AlarmPM = !AlarmPM;
      }
      if(encoderP()){
        setAlarmVars[setAlarmState]++;
      }
      if(encoderM()){
        setAlarmVars[setAlarmState]--;
      }
      if(setAlarmState > alarmItems){
        setAlarmState = 0;
      }
      if(setAlarmState < alarmItems - alarmItems){
        setAlarmState = alarmItems;
      }
      if(debouncerS.fell() && setAlarmState == 4){
        setAlarm = false;
        return;
      }
    }
    if(clockFunctionsState > clockFunctionsItems){
      clockFunctionsState = 0;
    }
    if(clockFunctionsState < clockFunctionsItems - clockFunctionsItems){
      clockFunctionsState = clockFunctionsItems;
    }
    if(debouncerS.fell() && clockFunctionsState == 7){
      clockFunctionsState = 0;
      activeMenuState = 0;
      return;
    }
    if(MQ2() == true){
      CO_Alert();
      clockFunctionsState = 0;
      activeMenuState = 0;
      return;
    }
    alwaysRun();
  }
}

//=====SETTINGS=====//
//Menu stuff and setting values//
int SettingState = 0;
const int SettingItems = 3;
String SettingName[4] = {"Red", "Blue", "Green", "Exit"};
int SettingVars[4] = {0, 0, 0, 0};
bool ChangeSettings = false;
//Function//
void Settings(){
  while(activeMenuState == 3){
    debouncerS.update();
    clearlcd();
    lcd.print(SettingName[SettingState]);
    lcd.setCursor(0,1);
    if(SettingState != 3){lcd.print(SettingVars[SettingState]);}
    else{lcd.print(F("Exit"));}
    if(encoderP()){
      SettingState++;
    }
    if(encoderM()){
      SettingState--;
    }
    if(debouncerS.fell() && SettingState != 3){
      ChangeSettings = true;
    }
    while(ChangeSettings == true && SettingState != 3){
      debouncerS.update();
      lcd.setCursor(0,1);
      lcd.print(SettingVars[SettingState]);
      lcd.print("   ");
      if(encoderP()){
        SettingVars[SettingState]++;
      }
      if(encoderM()){
        SettingVars[SettingState]--;
      }
      if(debouncerS.fell()){
        ChangeSettings = false;
      }
    }
    if(SettingState > SettingItems){
      SettingState = 0;
    }
    if(SettingState < SettingItems - SettingItems){
      SettingState = SettingItems;
    }
    if(debouncerS.fell() && SettingState == 3){
      SettingState = 0;
      activeMenuState = 0;
      return;
    }
    if(MQ2() == true){
      CO_Alert();
      SettingState = 0;
      activeMenuState = 0;
      return;
    }
    alwaysRun();
  }
}

//=====ABOUT CREATOR=====//
//Function//
void aboutCreator(){
  clearlcd();
  while(activeMenuState == 4){
    debouncerS.update();
    lcd.setCursor(0,0);
    lcd.print(F("My name is"));
    lcd.setCursor(0,1);
    lcd.print(F("Michael"));
    if(debouncerS.fell()){
      activeMenuState = 0;
      return;
    }
    if(MQ2() == true){
      CO_Alert();
      activeMenuState = 0;
      return;
    }
    alwaysRun();
  }
}

//==============================//
//===== SUB MENU LEVEL TWO =====//
//==============================//
int activeSubMenuState = 0;
int subMenuState = 0;
const int subMenuArrayItems = 5;
String subMenuStates[6] = {"blank", "blank2", "blank3", "Test Game", "blank4", "Exit"};

void SubMenu(){
  while(1){
    debouncerS.update();
    lcd.print(F("                "));
    lcd.setCursor(0,0);
    lcd.print(F("                "));
    lcd.setCursor(0,0);
    lcd.print(F("Games:"));
    lcd.setCursor(0,1);
    lcd.print(subMenuStates[subMenuState]);
      
    if(debouncerS.fell() && subMenuState == 0){
      activeSubMenuState = 1;
    }
    if(activeSubMenuState == 1){
      
    }
    if(debouncerS.fell() && subMenuState == 1){
      activeSubMenuState = 2;
    }
    if(activeSubMenuState == 2){
      
    }
    if(debouncerS.fell() && subMenuState == 2){
      activeSubMenuState = 3;
    }
    if(activeSubMenuState == 3){
      
    }
    if(debouncerS.fell() && subMenuState == 3){
      activeSubMenuState = 4;
    }
    if(activeSubMenuState == 4){
      GameOne();
    }
    if(debouncerS.fell() && subMenuState == 4){
      activeSubMenuState = 5;
    }
    if(activeSubMenuState == 5){
      
    }
    if(encoderP()){
      subMenuState++;
    }
    if(encoderM()){
      subMenuState--;
    }
    if(subMenuState > subMenuArrayItems){
      subMenuState = 0;
    }
    if(subMenuState < subMenuArrayItems - subMenuArrayItems){
      subMenuState = subMenuArrayItems;
    }
      
    if(debouncerS.fell() && subMenuState == 5){
      subMenuState = 0;
      activeSubMenuState = 0;
      activeMenuState = 0;
      return;
    }
    if(MQ2() == true){
      CO_Alert();
      subMenuState = 0;
      activeSubMenuState = 0;
      return;
    }
    alwaysRun();
  }
}
