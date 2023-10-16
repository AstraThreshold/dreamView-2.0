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
    //ui_proc();

    OLED_ClearBuffer();
    dreamViewUI.mainMenuProc(&mainMenu);
    u8g2_SendBuffer(&u8g2);
  }
}