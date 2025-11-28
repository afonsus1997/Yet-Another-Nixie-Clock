#include "nixie.h"

uint8_t anodes[] = {G_A0, G_A1, G_A2, G_A3, G_A4, G_A5};
uint8_t cathodes[] = {N0, N1, N2, N3};

const uint8_t kNumAnodes = 6;
const uint8_t kNumCathodes = 4;


void setupNixie(){
  for (int i = 0; i < kNumAnodes; i++)
  {
    pinMode(anodes[i], OUTPUT);
  }
  
  for (int i = 0; i < kNumCathodes; i++)
  {
    pinMode(cathodes[i], OUTPUT);
  }
  
}

void setDecoderOutput(uint8_t value) {
  value &= 0x0F; // Make sure we only keep lower 4 bits
  digitalWrite(N_LE, HIGH);
  digitalWrite(cathodes[0], (value >> 0) & 1);
  digitalWrite(cathodes[2], (value >> 1) & 1);
  digitalWrite(cathodes[3], (value >> 2) & 1);
  digitalWrite(cathodes[3], (value >> 3) & 1);
  digitalWrite(N_LE, LOW);
  digitalWrite(N_LE, HIGH);

}

void enableDigit(uint8_t digit){
  for (int i = 0; i < 6; i++)
  {
    if(i == digit)
      digitalWrite(anodes[i], HIGH);
    else
      digitalWrite(anodes[i], LOW);
  }
  
}