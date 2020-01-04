#include <LiquidCrystal.h>
#include <Wire.h>
extern LiquidCrystal lcd;
extern const int MQ2pin;
int RL_VALUE=5;                                     //define the load resistance on the board, in kilo ohms
float RO_CLEAN_AIR_FACTOR=9.83;
int CALIBARAION_SAMPLE_TIMES=50;                    //define how many samples you are going to take in the calibration phase
int CALIBRATION_SAMPLE_INTERVAL=500;                //define the time interal(in milisecond) between each samples in the cablibration phase
int READ_SAMPLE_INTERVAL=0;                        //define how many samples you are going to take in normal operation
int READ_SAMPLE_TIMES=5;

unsigned long previousMillis1 = 0;
float COCurve[3] = {2.3,0.72,-0.34};
float Ro = 3.75;
long iPPM_CO;

//functions
float MQResistanceCalculation(int raw_adc){
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}

float MQCalibration(int mq_pin){
  int i;
  float val=0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBARAION_SAMPLE_TIMES;                   //calculate the average value
  val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro                                        
  return val;                                                      //according to the chart in the datasheet 
}

float MQRead(int mq_pin){
  int i;
  float rs=0;
  unsigned long currentMillis = millis();
    for (i=0;i<READ_SAMPLE_TIMES;i++) {
      rs += MQResistanceCalculation(analogRead(mq_pin));
    }
  rs = rs/READ_SAMPLE_TIMES;
  return rs;  
}


long  MQGetPercentage(float rs_ro_ratio, float *pcurve){
  return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}

void MQ2setup(){
  lcd.clear();
  lcd.print(F("Calibrating"));
  lcd.setCursor(0,1);
  lcd.print(F("MQ-2..."));
  Ro = MQCalibration(MQ2pin);
  lcd.clear();
  lcd.print(Ro);
  delay(5000);
  lcd.clear();
}

int MQ2(){
  int Alarm = false;
  iPPM_CO = MQGetPercentage(MQRead(MQ2pin)/Ro, COCurve);
  if(iPPM_CO >= 50){
    Alarm = true;
  }
  if(Alarm == true){
    return true;
  }
  else{
    return false;
  }
}
