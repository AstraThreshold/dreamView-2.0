//
// Created by Fir on 2023/2/8.
//
#include "oled_key_fir.h"
#include "gpio.h"
#include "main.h"

int8_t encoder_status;
int16_t encoder_cnt;

int16_t encoder_last_cnt;

KEY key[4] = {0};
KEY_MSG key_msg = {0};

int get_key_val(uint8_t ch)
{
  switch (ch)
  {
    case 0:
      //return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);   //KEY0 up
      break;
    case 1:
      //return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);   //KEY1 down
      break;
    case 2:
      return HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin);   //KEY2 middle
      break;
    case 3:
      return HAL_GPIO_ReadPin(SHUTTERKEY_GPIO_Port, SHUTTERKEY_Pin);   //shutter key
    default:
      break;
  }
}

void key_init()
{
  for (uint8_t i = 0; i < (sizeof(key) / sizeof(KEY)); ++i)
  {
    key[i].val = key[i].last_val = get_key_val(i);
  }
}

void key_scan()
{
  for (uint8_t i = 2; i < (sizeof(key) / sizeof(KEY)); ++i)
  {
    key[i].val = get_key_val(i);        //获取键值
    if (key[i].last_val != key[i].val)      //发生改变
    {
      key[i].last_val = key[i].val;         //更新状态
      if (key[i].val == 0)
      {
        key_msg.id = i;
        key_msg.pressed = 1;
        //encoder_status = 2;
      }
    }
  }
}

