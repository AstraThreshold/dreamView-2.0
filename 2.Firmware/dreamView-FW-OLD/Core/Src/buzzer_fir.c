//
// Created by Fir on 2023/4/16.
//
#include "buzzer_fir.h"
#include "tim.h"
#include "stm32f1xx_it.h"
#include "oled_menu_show_fir.h"
#include "oled_key_fir.h"

uint32_t beep_time;

void buzzer_on()
{
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
}

void buzzer_off()
{
  HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
}

//freq越小 频率越高 有软件限位 最低为5 最高为60
void buzzer_beep(uint8_t freq)
{
  if (freq < 5) freq = 5;
  if (freq > 60) freq = 60;
  TIM4->ARR = 100 * freq;
  TIM4->CCR3 = TIM4->ARR / 2;
}

void buzzer_proc(uint8_t freq, uint8_t time)
{
  if (sound_is_on)
  {
    if (key_msg.pressed == 1)
    {
      if (key_msg.id == 0)
      {
        buzzer_on();
        buzzer_beep((uint8_t) freq * 2);
        beep_time = time;
      } else if (key_msg.id == 1)
      {
        buzzer_on();
        buzzer_beep(freq);
        beep_time = time;
      } else if (key_msg.id == 2)
      {
        buzzer_on();
        buzzer_beep((uint8_t) freq / 2);
        beep_time = time;
      }
    }
  }
}