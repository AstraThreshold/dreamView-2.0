//
// Created by Fir on 2023/2/8.
//

#ifndef OLED_KEY_FIR_H
#define OLED_KEY_FIR_H

#include "../../3rdParty/u8g2/u8g2.h"
#include "stm32f1xx.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * 使用定时器来轮询Key_Scan()函数，定时节拍为2ms,
  * 状态转换时间为10ms,即每次进入switch case语句的时间差为10ms
  * 利用该10ms的间隙跳过按键抖动
  */

//Key: 1:高电平，按键未按下， 0：低电平，按键按下
#define Key HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)

typedef enum
{
  KEY_CHECK = 0,
  KEY_COMFIRM = 1,
  KEY_RELEASE = 2
}KEY_STATE;

typedef enum
{
  NULL_KEY = 0,
  SHORT_KEY =1,
  LONG_KEY
}KEY_TYPE;

//extern u8 g_KeyFlag;
//extern KEY_TYPE g_KeyActionFlag;

//单个按键事件
//#define SingleKeyEvent

//单个按键实现长按和短按
#define SingleKey_LongShort_Event	1
extern void Key_Init(void);
extern void Key_Scan(void);

extern void Key_Proc(void);

extern uint8_t g_KeyFlag;                // 按键有效标志，0： 按键值无效； 1：按键值有效




#ifdef __cplusplus
}
#endif

#endif //OLED_KEY_FIR_H
