#ifndef __BTN__H__
#define __BTN__H__

#include "lwbtn/lwbtn.h"
// #include "lwbtn_opts.h"
#include <Arduino.h>

#define LWBTN_IGNORE_USER_OPTS

#define BUTTON_USER_PIN 23

enum btnsIdx_e
{
  BUTTON_USER = 0,
  BUTTONS_TOTAL
}; 

extern uint8_t btn_states[BUTTONS_TOTAL];
extern lwbtn_btn_t btns[];

void btn_process();
void btn_init();

extern const int buttons_id[BUTTONS_TOTAL];

#endif  //!__BTN__H__
