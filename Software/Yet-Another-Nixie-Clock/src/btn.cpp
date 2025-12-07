#include "btn.h"

uint8_t btn_states[3] = {0, 0, 0};


const int buttons_id[BUTTONS_TOTAL] = {BUTTON_USER};



lwbtn_btn_t btns[] = {

    {.arg = (void*)&buttons_id[BUTTON_USER]}

};


void btn_button_event(struct lwbtn* lw, struct lwbtn_btn* btn, lwbtn_evt_t evt) {
    if (evt == LWBTN_EVT_ONPRESS) {
        int btn_id = *(int*)btn->arg;
        // ui_process_button_event(btn_id); BTN HANDLING HERE
    }
}

uint8_t prv_btn_get_state(struct lwbtn* lw, struct lwbtn_btn* btn) {
  switch (*(int*)btn->arg) {
    case BUTTON_RIGHT:
      return !digitalRead(BUTTON_USER_PIN);
    default:
      return 0; // Button not pressed
  };
}

void btn_process(){
  lwbtn_process_ex(NULL, HAL_GetTick());
}

void btn_init(){
  pinMode(BUTTON_USER_PIN, INPUT);
  lwbtn_init_ex(NULL, btns, sizeof(btns) / sizeof(btns[0]), prv_btn_get_state, btn_button_event);
}