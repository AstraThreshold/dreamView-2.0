//
// Created by Fir on 2023/4/18.
//
#include "core.h"
#include "menu.h"
#include "oled_setup_u8g2_fir.h"
#include "oled_menu_show_fir.h"
#include "oled_menu_init_fir.h"
#include "oled_encoder_fir.h"
#include "oled_key_fir.h"
#include "buzzer_fir.h"
#include "oled_menu_proc_fir.h"

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
  u8g2_DrawStr(u8g2, 51,30,"g");
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



  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0)
  {
    //如果上电时的状态是等待上弦
    shutter_state_box = shutter_state_box_trg = u8g2_GetStrWidth(&u8g2, "Waiting") * 2 + 20;
  } else
  {
    shutter_state_box = shutter_state_box_trg = u8g2_GetStrWidth(&u8g2, "Ready!") * 2 + 6;
  }

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8 & GPIO_PIN_9, GPIO_PIN_SET);

  u8g2_SetFont(&u8g2, u8g2_font_myfont);
  OLED_menu_init();
  Encoder_Init(0);

  ui dreamViewUI = ui();
  menu mainMenu = menu(mainMenuList, 7);

  for (;;)
  {
    key_scan();
    Encoder_Analyze(Encoder_Scan());
    buzzer_proc(10, 45);
    ui_proc();

    OLED_ClearBuffer();
    //dreamViewUI.mainMenuProc(&mainMenu);
    drawTestLogo(&u8g2);
    u8g2_SendBuffer(&u8g2);
  }
}