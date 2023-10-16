//
// Created by Fir on 2023/2/8.
//
#include "oled_menu_init_fir.h"
#include "oled_setup_u8g2_fir.h"

int16_t shutter_speed = 200;
uint8_t shutter_is_B = 0;   //是否是B�?????
uint8_t shutter_is_working = 0;   //是否正在动作
uint8_t shutter_busy_state = 0;
uint8_t shutter_is_slow = 0;   //是否是慢于1/25的快门
uint8_t shutter_is_ok = 0;    //是否上弦完毕
uint8_t position = 0;
int16_t shutter_state_box_trg = 0;
int16_t shutter_state_box = 0;
double shutter_state_box_err = 0;
uint8_t shutter_box_is_ok = 0;

uint8_t disappear_step = 1;

uint8_t edit_index = 0;
int edit_flag = 0;                  //默认不在编辑 //默认高亮
const uint8_t BLINK_SPEED = 16;     //2的�?�数

void OLED_menu_init()
{
  setting_num = sizeof(setting) / sizeof(SELECT_LIST);
  single_line_length_setting = 63 / setting_num;
  total_line_length_setting = single_line_length_setting * setting_num + 1;

  developer_num = sizeof(developer_setting) / sizeof(SELECT_LIST);
  single_line_length_developer = 63 / developer_num;
  total_line_length_developer = single_line_length_developer * developer_num + 1;

  sound_num = sizeof(sound_setting) / sizeof(SELECT_LIST);
  single_line_length_sound = 63 / sound_num;
  total_line_length_sound = single_line_length_sound * sound_num + 1;

  list_num = sizeof(list) / sizeof(SELECT_LIST);
  single_line_length = 63 / list_num;   //12864屏幕纵向63像素 此句使屏幕纵向分成了listnum�??????
  total_line_length = single_line_length * list_num + 1;

  shutter_num = sizeof(shutter_setting) / sizeof(SELECT_LIST);
  single_line_length_shutter = 63 / shutter_num;
  total_line_length_shutter = single_line_length_shutter * shutter_num + 1;

  pid_num = sizeof(pid) / sizeof(SELECT_LIST);
  single_line_length_pid = 63 / pid_num;
  total_line_length_pid = single_line_length_pid * pid_num + 1;

  buf_ptr = u8g2_GetBufferPtr(&u8g2);
  buf_len = 8 * u8g2_GetBufferTileHeight(&u8g2) * u8g2_GetBufferTileWidth(&u8g2);

  x = x_setting = x_developer = x_shutter = x_sound = 4;
  y = y_trg = 0;
  y_developer = y_trg_developer = 0;
  y_setting = y_trg_setting = 0;
  y_shutter = y_trg_shutter = 0;
  y_pid = y_trg_pid = 0;
  y_sound = y_trg_sound = 0;

//  fps_switch_pos = fps_switch_pos_trg = 112;
//  shut_nolight_switch_pos = shut_nolight_switch_pos_trg = 112;
//  shut_nonext_switch_pos = shut_nonext_switch_pos_trg = 112;

  //开关默认全部打开
  fps_switch_pos = fps_switch_pos_trg = 114;
  shut_nolight_switch_pos = shut_nolight_switch_pos_trg = 114;
  shut_nonext_switch_pos = shut_nonext_switch_pos_trg = 114;
  motion_switch_pos = motion_switch_pos_trg = 114;
  sound_switch_pos = sound_switch_pos_trg = 114;

  //快门默认未准备好
  shutter_is_working = 0;
  shutter_is_ok = 0;

  line_y = line_y_trg = 1;
  line_y_setting = line_y_trg_setting = 1;
  line_y_developer = line_y_trg_developer = 1;
  line_y_shutter = line_y_trg_shutter = 1;
  line_y_sound = line_y_trg_sound = 1;
  pid_line_y = pid_line_y_trg = 1;

  ui_select = pid_select = setting_select = developer_select = shutter_select = sound_select = 0;

  box_width = box_width_trg = u8g2_GetUTF8Width(&u8g2, list[ui_select].select) + x * 2;
  pid_box_width = pid_box_width_trg = u8g2_GetUTF8Width(&u8g2, pid[pid_select].select) + x * 2;
  setting_box_width = setting_box_width_trg = u8g2_GetUTF8Width(&u8g2, setting[setting_select].select) + x * 2;
  developer_box_width = developer_box_width_trg = u8g2_GetUTF8Width(&u8g2, developer_setting[developer_select].select) + x * 2;
  sound_box_width = sound_box_width_trg = u8g2_GetUTF8Width(&u8g2, sound_setting[sound_select].select) + x * 2;
  shutter_box_width = shutter_box_width_trg = u8g2_GetUTF8Width(&u8g2, shutter_setting[shutter_select].select) + x * 2;

  ui_index = M_LOGO;
  ui_state = S_NONE;
}
