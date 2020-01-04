#include <Wire.h>

extern int alwaysRun();
extern void CO_Alert();
extern Bounce debouncerS;
extern int encoderP();
extern int encoderM();
extern void clearlcd();

void GameOne(){
  while(1){
    return;
    if(MQ2() == true){
      CO_Alert();
      return;
    }
    alwaysRun();
  }
}

//==============================//
//===== UNDER CONSTRUCTION =====//
//==============================//
