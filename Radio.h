#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};
typedef struct{
  int X;
  int Y;
}
Values;
Values data;
int test;

void radioSetup() {
  radio.begin();
  radio.openReadingPipe(0, pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening(); 
}

void radioFunctions() {
  if (radio.available(pipes[0])) {
    radio.read(&data, sizeof(data));
    Serial.print(data.X);
    Serial.print(" ");
    Serial.print(data.Y);
  }
  if (radio.available(pipes[1])) {
    radio.read(&test, sizeof(test));
    Serial.print(" ");
    Serial.print(test);
  }
}
