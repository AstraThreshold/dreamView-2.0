//
// Created by Fir on 2023/4/18.
//
#include "core.h"
#include "oled.h"
#include "menu.h"
#include "key.h"
#include "tim.h"

void dreamViewKernelStart(void)
{
  OLED_Init();
  OLED_Display_On();
  OLED_Clear();
  u8g2Init(&u8g2);

  //Key_Init();

  for (;;)
  {
    OLED_ClearBuffer();

    //btn_proc_poll();
    //drawTestLogo(&u8g2);
    Key_Proc();

    u8g2_SendBuffer(&u8g2);
  }
}