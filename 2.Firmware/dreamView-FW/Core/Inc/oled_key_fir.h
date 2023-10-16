//
// Created by Fir on 2023/2/8.
//

#ifndef OLED_KEY_FIR_H
#define OLED_KEY_FIR_H

#include "u8g2.h"

#ifdef __cplusplus
extern "C" {
#endif

//按键变量
typedef struct
{
  int val;
  int last_val;
} KEY;

extern KEY key[4];

//按键信息
typedef struct
{
  uint8_t id;
  int pressed;
} KEY_MSG;

extern KEY_MSG key_msg;

extern int get_key_val(uint8_t ch);

extern void key_init();

extern void key_scan();

#ifdef __cplusplus
}
#endif

#endif //OLED_KEY_FIR_H
