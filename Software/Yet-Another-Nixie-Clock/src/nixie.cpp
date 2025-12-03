#include "nixie.h"

// ------------------------------
// NIXIE PIN CONFIGURATION
// ------------------------------
uint8_t anodes[] = {G_A0, G_A1, G_A2, G_A3, G_A4, G_A5};
const uint8_t kNumAnodes = 6;

uint8_t cathodes[] = {N0, N1, N2, N3};
const uint8_t kNumCathodes = 4;


uint8_t currentTime[] = {2,4,4,6,7,9};

// ------------------------------
// MULTIPLEX TIMING
// ------------------------------
const uint32_t coldOnTime  = 15000; // 15 ms first strike
const uint32_t coldOffTime = 1000;  // 1 ms

const uint32_t normalOnTime  = 500; // 1.2 ms
const uint32_t normalOffTime = 200;  // 0.2 ms
const uint8_t  pulsesPerDigit = 2;   // double pulse per digit

// ------------------------------
// MULTIPLEX STATE
// ------------------------------
uint8_t currentDigit = 0;
uint8_t pulseCount = 0;
bool digitOn = false;
uint32_t lastUpdateTime = 0;
bool coldStartDone = false;

// ------------------------------
// SET TIME
// ------------------------------
void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  currentTime[0] = seconds % 10;
  currentTime[1] = seconds / 10;
  currentTime[2] = minutes % 10;
  currentTime[3] = minutes / 10;
  currentTime[4] = hours % 10;
  currentTime[5] = hours / 10;
}

// ------------------------------
// SETUP
// ------------------------------
void setupNixie() {
  for (int i=0;i<kNumAnodes;i++) pinMode(anodes[i],OUTPUT);
  for (int i=0;i<kNumCathodes;i++) pinMode(cathodes[i],OUTPUT);

  pinMode(N_LE,OUTPUT);
  digitalWrite(N_LE,HIGH);

  disableDigits();
}

// ------------------------------
// DIGIT CONTROL
// ------------------------------
void disableDigits() { for(int i=0;i<kNumAnodes;i++) digitalWrite(anodes[i],LOW);}
void disableDigit(uint8_t d) { digitalWrite(anodes[d],LOW);}
void enableDigit(uint8_t d){ for(int i=0;i<kNumAnodes;i++) digitalWrite(anodes[i], (i==d)?HIGH:LOW);}

// ------------------------------
// DECODER OUTPUT
// ------------------------------
void setDecoderOutput(uint8_t value){
  value &= 0x0F;
  digitalWrite(cathodes[0],(value>>0)&1);
  digitalWrite(cathodes[1],(value>>1)&1);
  digitalWrite(cathodes[2],(value>>2)&1);
  digitalWrite(cathodes[3],(value>>3)&1);

  // Pulse latch
  // digitalWrite(N_LE,LOW);
  // digitalWrite(N_LE,HIGH);
}

// ------------------------------
// MULTIPLEXING
// ------------------------------
void multiplexNixie() {
  uint32_t now = micros();

  uint32_t onTime  = coldStartDone ? normalOnTime : coldOnTime;
  uint32_t offTime = coldStartDone ? normalOffTime : coldOffTime;

  if(!digitOn && now - lastUpdateTime >= offTime){
    setDecoderOutput(currentTime[currentDigit]);
    enableDigit(currentDigit);
    lastUpdateTime = now;
    digitOn = true;
  }
  else if(digitOn && now - lastUpdateTime >= onTime){
    disableDigit(currentDigit);
    lastUpdateTime = now;
    digitOn = false;
    pulseCount++;

    if(pulseCount >= pulsesPerDigit){
      pulseCount = 0;
      currentDigit = (currentDigit+1)%kNumAnodes;
      if(!coldStartDone && currentDigit==0) coldStartDone = true;
    }
  }
}

// ------------------------------
// NIXIE LOOP
// ------------------------------
void nixieLoop(){
  multiplexNixie();
}