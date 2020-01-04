#include <LiquidCrystal.h>
#include <Wire.h>
extern int alwaysRun();
extern LiquidCrystal lcd;
extern Bounce debouncerS;
extern void clearlcd();
extern int MQ2();
extern void CO_Alert();
String YouCounter = "";

//YT Logo
void YTLogo(){
  byte C0[8] = {
    0b00011,
    0b00111,
    0b01111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111};
  byte C1[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b01111,
    0b00111,
    0b00011};
  byte C2[8] = {
    0b11111,
    0b11011,
    0b11001,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000};
  byte C3[8] = {
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11001,
    0b11011,
    0b11111};
  byte C4[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b01111,
    0b00111,
    0b00011};
  byte C5[8] = {
    0b00011,
    0b00111,
    0b01111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111};
  byte C6[8] = {
    0b11000,
    0b11100,
    0b11110,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111};
  byte C7[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11110,
    0b11100,
    0b11000};

  lcd.createChar(0 , C0);
  lcd.createChar(1 , C1);
  lcd.createChar(2 , C2);
  lcd.createChar(3 , C3);
  lcd.createChar(4 , C4);
  lcd.createChar(5 , C5);
  lcd.createChar(6 , C6);
  lcd.createChar(7 , C7);
  
  lcd.setCursor(0,0);
  lcd.write(byte(0));
  lcd.setCursor(0,1);
  lcd.write(byte(1));
  lcd.setCursor(1,0);
  lcd.write(byte(2));
  lcd.setCursor(1,1);
  lcd.write(byte(3));
  lcd.setCursor(2,0);
  lcd.write(byte(4));
  lcd.setCursor(2,1);
  lcd.write(byte(5));
  lcd.setCursor(3,0);
  lcd.write(byte(6));
  lcd.setCursor(3,1);
  lcd.write(byte(7));
}

//subcount
void subCount(){
  clearlcd();
  while(1){
    debouncerS.update();
    YTLogo();
    // subscribers text
    lcd.setCursor(5,0);
    lcd.print(F("Michael's"));
    lcd.setCursor(5,1);
    lcd.print(F("Subs:"));
    lcd.setCursor(11,1);
    lcd.print(F("             "));
    lcd.setCursor(11,1);
    lcd.print(YouCounter);
    while(Serial.available()){
     YouCounter = Serial.readString();
    }
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
