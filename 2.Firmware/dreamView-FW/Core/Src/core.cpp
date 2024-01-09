//
// Created by Fir on 2023/4/18.
//
#include "core.h"
#include "oled.h"
#include "menu.h"

void drawTestLogo(u8g2_t *u8g2)
{
  u8g2_SetFontMode(u8g2, 1); /*字体模式选择*/
  u8g2_SetFontDirection(u8g2, 0); /*字体方向选择*/
  u8g2_SetFont(u8g2, u8g2_font_myfont); /*字库选择*/
  u8g2_DrawStr(u8g2, 0, 20, "U");

  u8g2_SetFontDirection(u8g2, 1);
  u8g2_SetFont(u8g2, u8g2_font_myfont);
  u8g2_DrawStr(u8g2, 21,8,"8");

  u8g2_SetFontDirection(u8g2, 0);
  u8g2_SetFont(u8g2, u8g2_font_myfont);
  u8g2_DrawStr(u8g2, 51,30,"f");
  u8g2_DrawStr(u8g2, 67,30,"\xb2");

  u8g2_DrawHLine(u8g2, 2, 35, 47);
  u8g2_DrawHLine(u8g2, 3, 36, 47);
  u8g2_DrawVLine(u8g2, 45, 32, 12);
  u8g2_DrawVLine(u8g2, 46, 33, 12);

  u8g2_SetFont(u8g2, u8g2_font_myfont);
  u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");
}

void dreamViewKernelStart(void)
{
  OLED_Init();
  OLED_Display_On();
  OLED_Clear();
  u8g2Init(&u8g2);

  list_init();
  oled_init();

  for (;;)
  {
    //drawTestLogo(&u8g2);
    btn_scan();
    ui_proc();
  }
}