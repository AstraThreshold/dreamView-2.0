//
// Created by Fir on 2023/4/16.
//

#ifndef BUZZER_FIR_H
#define BUZZER_FIR_H

#include "u8g2.h"

#ifdef __cplusplus
extern "C" {
#endif

static uint16_t counter = 0;
extern uint32_t beep_time;

extern void buzzer_on();
extern void buzzer_off();

extern void buzzer_beep(uint8_t freq);

extern void buzzer_proc(uint8_t freq, uint8_t time);

#ifdef __cplusplus
}
#endif

#endif //BUZZER_FIR_H
