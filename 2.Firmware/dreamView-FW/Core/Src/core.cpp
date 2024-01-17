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

  ui_param_init();
  ui_init();
  oled_init();

  for (;;)
  {
    //OLED_ClearBuffer();

    //btn_proc_poll();
    //drawTestLogo(&u8g2);
    //Key_Proc();

    ui_proc();

//    u8g2_DrawRFrame(&u8g2, 64-17, 23-17, 34, 34, 4);
//
//    u8g2_DrawRFrame(&u8g2, 64-17-10-34, 23-17, 34, 34, 4);
//
//    u8g2_DrawRFrame(&u8g2, 64+17+10, 23-17, 34, 34, 4);

    //u8g2_SetDrawColor(&u8g2, 1);

    // u8g2_SendBuffer(&u8g2);
  }
}