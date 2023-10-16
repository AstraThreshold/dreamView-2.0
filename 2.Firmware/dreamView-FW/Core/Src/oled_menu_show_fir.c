//
// Created by Fir on 2023/2/8.
//
#include "oled_menu_show_fir.h"
#include "oled_setup_u8g2_fir.h"

//选择界面变量
uint8_t x;
int16_t y, y_trg;                  //目标和当前值
uint8_t line_y, line_y_trg;        //线的位置
uint8_t box_width, box_width_trg;  //框的宽度
int16_t box_y, box_y_trg;          //框的当前值和目标值
int8_t ui_select;                  //当前选中那一栏
uint8_t single_line_length;        //滚动条每一段的长度
uint8_t total_line_length;         //滚动条总长度

//pid界面变量
uint8_t pid_line_y, pid_line_y_trg;
uint8_t pid_box_width, pid_box_width_trg;
int16_t pid_box_y, pid_box_y_trg;
int8_t pid_select;
uint8_t motion_is_on = 1;
int16_t motion_switch_pos, motion_switch_pos_trg;
int16_t y_pid, y_trg_pid;
uint8_t single_line_length_pid;
uint8_t total_line_length_pid;

uint8_t blink_flag;                 //默认高亮

//指向buffer首地址的指针
uint8_t *buf_ptr;
uint16_t buf_len;

uint8_t x_setting;
int16_t y_setting, y_trg_setting;
uint8_t line_y_setting, line_y_trg_setting;
uint8_t single_line_length_setting;
uint8_t total_line_length_setting;
int8_t setting_select;
uint8_t setting_box_width, setting_box_width_trg;    //框的宽度
int16_t setting_box_y, setting_box_y_trg;            //框的当前值和目标值
int16_t fps_switch_pos, fps_switch_pos_trg;         //开关小圆点的当前值和目标值

uint8_t x_developer;
int16_t y_developer, y_trg_developer;
uint8_t line_y_developer, line_y_trg_developer;
uint8_t single_line_length_developer;
uint8_t total_line_length_developer;
int8_t developer_select;
uint8_t developer_box_width, developer_box_width_trg;    //框的宽度
int16_t developer_box_y, developer_box_y_trg;            //框的当前值和目标值

uint8_t x_sound;
int16_t y_sound, y_trg_sound;                  //目标和当前值
uint8_t line_y_sound, line_y_trg_sound;        //线的位置
uint8_t single_line_length_sound;
uint8_t total_line_length_sound;
int8_t sound_select;
uint8_t sound_box_width, sound_box_width_trg;    //框的宽度
int16_t sound_box_y, sound_box_y_trg;            //框的当前值和目标�?????
int16_t sound_switch_pos, sound_switch_pos_trg;

uint8_t x_shutter;
int16_t y_shutter, y_trg_shutter;
uint8_t line_y_shutter, line_y_trg_shutter;
uint8_t single_line_length_shutter;
uint8_t total_line_length_shutter;
int8_t shutter_select;
uint8_t shutter_box_width, shutter_box_width_trg;    //框的宽度
int16_t shutter_box_y, shutter_box_y_trg;            //框的当前值和目标值
uint8_t shut_nolight_is_on;
int16_t shut_nolight_switch_pos, shut_nolight_switch_pos_trg;
uint8_t shut_nonext_is_on;
int16_t shut_nonext_switch_pos, shut_nonext_switch_pos_trg;
uint8_t shut_nolight_switch_need_moving, shut_nonext_switch_need_moving;

//todo: 考虑把这部分都变成float
double err_y;    //pid误差y
double pre_err_y;   //pid误差y历史 用于积分控制
double sum_err_y;   //pid误差y累加 用于积分控制
double del_err_y;   //pid误差y近两次之差
double sum_out_y;

double err_y_switch;    //pid误差y
double sum_err_y_switch;   //pid误差y累加 用于积分控制
double sum_out_y_switch;

double err_bar;
double out_bar;

double err_screen;

double err_str;
double out_str;

double PID_MAX = 0.50;      //PID值限位

char kpid[101];

char shutter_speed_char[20];

char fps[20];

uint8_t ui_index, ui_state;

uint16_t FPS_Count;    //Fps
uint16_t FPS;
uint8_t fps_is_on = 1;

uint8_t sound_is_on = 1;

//todo: 做最开始的那个文字从头上滚下来的动画 尝试融入进每个show函数里
//todo: 记住 文字进入指定位置后 就要给flag置1
void move_screen(int16_t *a, const int16_t *a_trg)
{
  if (!motion_is_on)
  {
    *a = *a_trg;
    return;
  }
  err_screen = *a_trg - *a;    //得到当前误差
  if (*a < *a_trg)
  {
    if (0.5 * err_screen >= MAX_OUT_SCREEN)
    {
      *a += MAX_OUT_SCREEN;
    } else
    {
      *a += 0.5 * err_screen;
    }
    if (*a > *a_trg) *a = *a_trg;     //加完超过
  } else if (*a > *a_trg)
  {
    if (0.5 * err_screen >= MAX_OUT_SCREEN)
    {
      *a += MAX_OUT_SCREEN;
    } else
    {
      *a += 0.5 * err_screen;
    }
    if (*a < *a_trg) *a = *a_trg;     //减完不足
  }
}

void move(int16_t *a, const int16_t *a_trg)
{
  if (!motion_is_on)
  {
    *a = *a_trg;
    return;
  }
  err_y = *a_trg - *a;    //得到当前误差
  sum_err_y += err_y;   //累加历史误差
  del_err_y = err_y - pre_err_y;    //计算近两次误差之差
  sum_out_y = Kpid[0] * err_y + Kpid[1] * sum_err_y + Kpid[2] * del_err_y;
  if (sum_out_y >= MAX_OUT)
  {
    sum_out_y = MAX_OUT;
  }
  *a += sum_out_y;    //输出
  pre_err_y = err_y;    //pre_err_y在err_y更新前存储当前err_y
}

uint8_t move_switch(int16_t *a, const int16_t *a_trg)
{
  if (!motion_is_on)
  {
    *a = *a_trg;
    return 1;
  }
  err_y_switch = *a_trg - *a;    //得到当前误差
  sum_err_y_switch += err_y_switch;   //累加历史误差
  sum_out_y_switch = 0.45 * err_y_switch + 0.10 * sum_err_y_switch;
  if (sum_out_y_switch >= MAX_OUT)
  {
    sum_out_y_switch = MAX_OUT;
  }

  *a += sum_out_y_switch;

  if (sum_out_y_switch == 0) return 1;
}

//宽度移动函数
void move_width(uint8_t *a, const uint8_t *a_trg)
{
  if (!motion_is_on)
  {
    *a = *a_trg;
    return;
  }
  err_bar = *a_trg - *a;
  if ((0.50 * err_bar) > MAX_OUT_WIDTH)   //因为宽度变化较大 所以增加pid限位
  {
    out_bar = MAX_OUT_WIDTH;
  } else
  {
    out_bar = 0.60 * err_bar;   //Kp = 0.60
  }
  *a += out_bar;
}

void move_bar(uint8_t *a, const uint8_t *a_trg)
{
  if (!motion_is_on)
  {
    *a = *a_trg;
    return;
  }
  err_bar = *a_trg - *a;
  if ((0.50 * err_bar) > MAX_OUT)
  {
    out_bar = MAX_OUT;
  } else
  {
    out_bar = 0.50 * err_bar;
  }
  *a += out_bar;
}

//todo: 注意此函数的进入条件 动画只能运行一遍 运行过后flag置1
void move_str(uint8_t *a, const uint8_t *a_trg)
{
  if (!motion_is_on)
  {
    *a = *a_trg;
    return;
  }
  if (*a == *a_trg) return;   //如果字符已经到位了 直接return
  err_str = *a_trg - *a;
  if ((0.50 * err_str) > MAX_OUT)
  {
    out_str = MAX_OUT;
  } else
  {
    out_str = 0.50 * err_str;
  }
  *a += out_str;
}

void text_edit(int dir, uint8_t index)
{
  if (!dir)
  {
    if (name[index] >= 'A' && name[index] <= 'Z')
    {
      if (name[index] == 'A')
      {
        name[index] = 'z';
      } else
      {
        name[index] -= 1;
      }
    } else if (name[index] >= 'a' && name[index] <= 'z')
    {
      if (name[index] == 'a')
      {
        name[index] = ' ';
      } else
      {
        name[index] -= 1;
      }
    } else
    {
      name[index] = 'Z';
    }
  } else
  {
    if (name[index] >= 'A' && name[index] <= 'Z')
    {
      if (name[index] == 'Z')
      {
        name[index] = ' ';
      } else
      {
        name[index] += 1;
      }
    } else if (name[index] >= 'a' && name[index] <= 'z')
    {
      if (name[index] == 'z')
      {
        name[index] = 'A';
      } else
      {
        name[index] += 1;
      }
    } else
    {
      name[index] = 'a';
    }
  }
}

void disappear()
{
  switch (disappear_step)
  {
    case 1:
      for (uint16_t i = 0; i < buf_len; ++i)
      {
        if (i % 2 == 0) buf_ptr[i] = buf_ptr[i] & 0x55;
      }
      break;
    case 2:
      for (uint16_t i = 0; i < buf_len; ++i)
      {
        if (i % 2 != 0) buf_ptr[i] = buf_ptr[i] & 0xAA;
      }
      break;
    case 3:
      for (uint16_t i = 0; i < buf_len; ++i)
      {
        if (i % 2 == 0) buf_ptr[i] = buf_ptr[i] & 0x00;
      }
      break;
    case 4:
      for (uint16_t i = 0; i < buf_len; ++i)
      {
        if (i % 2 != 0) buf_ptr[i] = buf_ptr[i] & 0x00;
      }
      break;
    default:
      ui_state = S_NONE;
      disappear_step = 0;
      break;
  }
  disappear_step++;
}

void logo_ui_show()
{
  u8g2_DrawXBMP(&u8g2, 0, 0, 128, 64, LOGO);
//  u8g2_DrawUTF8(&u8g2, 30, 50, "chinese1,你好");
//   for(uint16_t i=0;i<buf_len;++i)
//   {
//     if(i%4==0||i%4==1)
//     {
//     buf_ptr[i]=buf_ptr[i] & 0x33;
//     }
//     else
//     {
//     buf_ptr[i]=buf_ptr[i] & 0xCC;
//     }
//   }
}

void select_ui_show()
{
  move_bar(&line_y, &line_y_trg);
  move_screen(&y, &y_trg);
  move(&box_y, &box_y_trg);
  move_width(&box_width, &box_width_trg);
  u8g2_DrawVLine(&u8g2, 126, 0, total_line_length);
  u8g2_DrawPixel(&u8g2, 125, 0);
  u8g2_DrawPixel(&u8g2, 127, 0);
  for (uint8_t i = 0; i < list_num; ++i)
  {
    u8g2_DrawUTF8(&u8g2, x, 16 * i + y + 12, list[i].select);
    u8g2_DrawPixel(&u8g2, 125, single_line_length * (i + 1));
    u8g2_DrawPixel(&u8g2, 127, single_line_length * (i + 1));
  }
  u8g2_DrawVLine(&u8g2, 125, line_y, single_line_length);
  u8g2_DrawVLine(&u8g2, 127, line_y, single_line_length);

//  u8g2_DrawVLine(&u8g2, 125, line_y, single_line_length - 1);
//  u8g2_DrawVLine(&u8g2, 127, line_y, single_line_length - 1);

  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawRBox(&u8g2, 0, box_y, box_width, 16, 1);
  u8g2_SetDrawColor(&u8g2, 1);
}

void setting_ui_show()
{
  move_bar(&line_y_setting, &line_y_trg_setting);
  move_screen(&y_setting, &y_trg_setting);
  move(&setting_box_y, &setting_box_y_trg);
  move_width(&setting_box_width, &setting_box_width_trg);
  u8g2_DrawVLine(&u8g2, 126, 0, total_line_length_setting);
  u8g2_DrawPixel(&u8g2, 125, 0);
  u8g2_DrawPixel(&u8g2, 127, 0);
  for (uint8_t i = 0; i < setting_num; ++i)
  {
    u8g2_DrawUTF8(&u8g2, x_setting, 16 * i + y_setting + 12, setting[i].select);
    u8g2_DrawPixel(&u8g2, 125, single_line_length_setting * (i + 1));
    u8g2_DrawPixel(&u8g2, 127, single_line_length_setting * (i + 1));
  }
  u8g2_DrawVLine(&u8g2, 125, line_y_setting, single_line_length_setting);
  u8g2_DrawVLine(&u8g2, 127, line_y_setting, single_line_length_setting);

//  u8g2_DrawVLine(&u8g2, 125, line_y_setting, single_line_length_setting - 1);
//  u8g2_DrawVLine(&u8g2, 127, line_y_setting, single_line_length_setting - 1);

  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawRBox(&u8g2, 0, setting_box_y, setting_box_width, 16, 1);
  u8g2_SetDrawColor(&u8g2, 1);
}

void developer_setting_show()
{
  move_bar(&line_y_developer, &line_y_trg_developer);
  move_screen(&y_developer, &y_trg_developer);
  move(&developer_box_y, &developer_box_y_trg);
  move_width(&developer_box_width, &developer_box_width_trg);
  u8g2_DrawVLine(&u8g2, 126, 0, total_line_length_developer);
  u8g2_DrawPixel(&u8g2, 125, 0);
  u8g2_DrawPixel(&u8g2, 127, 0);

  for (uint8_t i = 0; i < developer_num; ++i)
  {
    u8g2_DrawUTF8(&u8g2, x_developer, 16 * i + y_developer + 12, developer_setting[i].select);
    u8g2_DrawPixel(&u8g2, 125, single_line_length_developer * (i + 1));
    u8g2_DrawPixel(&u8g2, 127, single_line_length_developer * (i + 1));
    if (i == 1)   //如果是fps显示
    {
      //绘制开关
      u8g2_DrawRBox(&u8g2, 106, 16 * i + y_developer + 3, 16, 10, 3);
      u8g2_SetDrawColor(&u8g2, 2);
      u8g2_DrawBox(&u8g2, 107, 16 * i + y_developer + 5, 14, 6);
      u8g2_DrawBox(&u8g2, 109, 16 * i + y_developer + 4, 10, 1);   //去掉上面的多余部分
      u8g2_DrawBox(&u8g2, 109, 16 * i + y_developer + 11, 10, 1);    //去掉下面的多余部分
      u8g2_SetDrawColor(&u8g2, 1);

//      u8g2_DrawRBox(&u8g2, 104, 16 * i + y_shutter + 3, 16, 10, 3);
//      u8g2_SetDrawColor(&u8g2, 2);
//      u8g2_DrawBox(&u8g2, 105, 16 * i + y_shutter + 5, 14, 6);
//      u8g2_DrawBox(&u8g2, 107, 16 * i + y_shutter + 4, 10, 1);   //去掉上面的多余部分
//      u8g2_DrawBox(&u8g2, 107, 16 * i + y_shutter + 11, 10, 1);    //去掉下面的多余部分
//      u8g2_SetDrawColor(&u8g2, 1);

      move_switch(&fps_switch_pos, &fps_switch_pos_trg);
      u8g2_DrawRBox(&u8g2, fps_switch_pos, 16 * i + y_developer + 5, 6, 6, 1);   //绘制圆点
    }
  }
//  u8g2_DrawVLine(&u8g2, 125, line_y_developer, single_line_length_developer - 1);
//  u8g2_DrawVLine(&u8g2, 127, line_y_developer, single_line_length_developer - 1);

  u8g2_DrawVLine(&u8g2, 125, line_y_developer, single_line_length_developer);
  u8g2_DrawVLine(&u8g2, 127, line_y_developer, single_line_length_developer);

  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawRBox(&u8g2, 0, developer_box_y, developer_box_width, 16, 1);
  u8g2_SetDrawColor(&u8g2, 1);
}

void sound_setting_show()
{
  move_bar(&line_y_sound, &line_y_trg_sound);
  //move_screen(&y_sound, &y_trg_sound);
  move(&sound_box_y, &sound_box_y_trg);
  move_width(&sound_box_width, &sound_box_width_trg);
  u8g2_DrawVLine(&u8g2, 126, 0, total_line_length_sound);
  u8g2_DrawPixel(&u8g2, 125, 0);
  u8g2_DrawPixel(&u8g2, 127, 0);

  for (uint8_t i = 0; i < sound_num; ++i)
  {
    u8g2_DrawUTF8(&u8g2, x_sound, 16 * i + y_sound + 12, sound_setting[i].select);
    u8g2_DrawPixel(&u8g2, 125, single_line_length_sound * (i + 1));
    u8g2_DrawPixel(&u8g2, 127, single_line_length_sound * (i + 1));
    if (i == 1)   //如果是按键音开关
    {
      //绘制开关
      u8g2_DrawRBox(&u8g2, 106, 16 * i + y_sound + 3, 16, 10, 3);
      u8g2_SetDrawColor(&u8g2, 2);
      u8g2_DrawBox(&u8g2, 107, 16 * i + y_sound + 5, 14, 6);
      u8g2_DrawBox(&u8g2, 109, 16 * i + y_sound + 4, 10, 1);   //去掉上面的多余部分
      u8g2_DrawBox(&u8g2, 109, 16 * i + y_sound + 11, 10, 1);    //去掉下面的多余部分
      u8g2_SetDrawColor(&u8g2, 1);

//      u8g2_DrawRBox(&u8g2, 104, 16 * i + y_shutter + 3, 16, 10, 3);
//      u8g2_SetDrawColor(&u8g2, 2);
//      u8g2_DrawBox(&u8g2, 105, 16 * i + y_shutter + 5, 14, 6);
//      u8g2_DrawBox(&u8g2, 107, 16 * i + y_shutter + 4, 10, 1);   //去掉上面的多余部分
//      u8g2_DrawBox(&u8g2, 107, 16 * i + y_shutter + 11, 10, 1);    //去掉下面的多余部分
//      u8g2_SetDrawColor(&u8g2, 1);

      move_switch(&sound_switch_pos, &sound_switch_pos_trg);
      u8g2_DrawRBox(&u8g2, sound_switch_pos, 16 * i + y_sound + 5, 6, 6, 1);   //绘制圆点
    }
  }

  u8g2_DrawVLine(&u8g2, 125, line_y_sound, single_line_length_sound);
  u8g2_DrawVLine(&u8g2, 127, line_y_sound, single_line_length_sound);

  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawRBox(&u8g2, 0, sound_box_y, sound_box_width, 16, 1);
  u8g2_SetDrawColor(&u8g2, 1);
}

void shutter_setting_show()
{
  move_bar(&line_y_shutter, &line_y_trg_shutter);
  move_screen(&y_shutter, &y_trg_shutter);
  move(&shutter_box_y, &shutter_box_y_trg);
  move_width(&shutter_box_width, &shutter_box_width_trg);
  u8g2_DrawVLine(&u8g2, 126, 0, total_line_length_shutter);
  u8g2_DrawPixel(&u8g2, 125, 0);
  u8g2_DrawPixel(&u8g2, 127, 0);

  for (uint8_t i = 0; i < shutter_num; ++i)
  {
    u8g2_DrawUTF8(&u8g2, x_shutter, 16 * i + y_shutter + 12, shutter_setting[i].select);
    u8g2_DrawPixel(&u8g2, 125, single_line_length_shutter * (i + 1));
    u8g2_DrawPixel(&u8g2, 127, single_line_length_shutter * (i + 1));
    if (i == 1 || i == 2)   //如果是无视曝光释放快门或未过片允许释放快门
    {
      //绘制开关
      u8g2_DrawRBox(&u8g2, 106, 16 * i + y_shutter + 3, 16, 10, 3);
      u8g2_SetDrawColor(&u8g2, 2);
      u8g2_DrawBox(&u8g2, 107, 16 * i + y_shutter + 5, 14, 6);
      u8g2_DrawBox(&u8g2, 109, 16 * i + y_shutter + 4, 10, 1);   //去掉上面的多余部分
      u8g2_DrawBox(&u8g2, 109, 16 * i + y_shutter + 11, 10, 1);    //去掉下面的多余部分
      u8g2_SetDrawColor(&u8g2, 1);

//      u8g2_DrawRBox(&u8g2, 104, 16 * i + y_shutter + 3, 16, 10, 3);
//      u8g2_SetDrawColor(&u8g2, 2);
//      u8g2_DrawBox(&u8g2, 105, 16 * i + y_shutter + 5, 14, 6);
//      u8g2_DrawBox(&u8g2, 107, 16 * i + y_shutter + 4, 10, 1);   //去掉上面的多余部分
//      u8g2_DrawBox(&u8g2, 107, 16 * i + y_shutter + 11, 10, 1);    //去掉下面的多余部分
//      u8g2_SetDrawColor(&u8g2, 1);

      if (i == 1)
      {
        u8g2_DrawRBox(&u8g2, shut_nolight_switch_pos, 16 * i + y_shutter + 5, 6, 6, 1);   //绘制圆点

        if (shut_nolight_switch_need_moving)
        {
          move_switch(&shut_nolight_switch_pos, &shut_nolight_switch_pos_trg);
        }
      }
      if (i == 2)
      {
        u8g2_DrawRBox(&u8g2, shut_nonext_switch_pos, 16 * i + y_shutter + 5, 6, 6, 1);   //绘制圆点

        if (shut_nonext_switch_need_moving)
        {
          move_switch(&shut_nonext_switch_pos, &shut_nonext_switch_pos_trg);
        }
      }
    }
  }
//  u8g2_DrawVLine(&u8g2, 125, line_y_shutter, single_line_length_shutter - 1);
//  u8g2_DrawVLine(&u8g2, 127, line_y_shutter, single_line_length_shutter - 1);

  u8g2_DrawVLine(&u8g2, 125, line_y_shutter, single_line_length_shutter);
  u8g2_DrawVLine(&u8g2, 127, line_y_shutter, single_line_length_shutter);

  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawRBox(&u8g2, 0, shutter_box_y, shutter_box_width, 16, 1);
  u8g2_SetDrawColor(&u8g2, 1);
}

//todo: 返回到相机界面时才上弦并给快门上电
void camera_ui_show()
{
  shutter_box_is_ok = 0;
  OLED_ClearBuffer();

  u8g2_DrawUTF8(&u8g2, 0, 10, "快门当前状态");

  if (shutter_is_working == 1)
  {
    u8g2_SetFont(&u8g2, u8g2_font_chargen_92_tr);
    u8g2_DrawStr(&u8g2, 2, 34, "Busy!");    //工作�????? 因为快门工作时线程阻�????? �?????以要有提�?????
    u8g2_SetFont(&u8g2, u8g2_font_myfont);

    //画XOR圆角填充
    u8g2_SetDrawColor(&u8g2, 2);

    //pid的小过渡动画
    shutter_state_box_trg = u8g2_GetStrWidth(&u8g2, "Busy!") * 2 + 6;
    shutter_state_box_err = shutter_state_box_trg - shutter_state_box;

    if (0.5 * shutter_state_box_err > MAX_OUT_WIDTH)
    {
      shutter_state_box += MAX_OUT_WIDTH;
    } else
    {
      shutter_state_box += 0.50 * shutter_state_box_err;
    }

    u8g2_DrawRBox(&u8g2, 0, 16, shutter_state_box, 22, 4);
    u8g2_SetDrawColor(&u8g2, 1);
    if (shutter_state_box == shutter_state_box_trg) shutter_box_is_ok = 1;
    //绘制完成pid后标记

  } else if (shutter_is_working == 0)    //如果快门空闲
  {
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0)
    {
      shutter_is_ok = 0;
      u8g2_SetFont(&u8g2, u8g2_font_chargen_92_tr);
      u8g2_DrawStr(&u8g2, 2, 34, "Waiting");    //等待上弦
      u8g2_SetFont(&u8g2, u8g2_font_myfont);
      //画XOR圆角填充
      u8g2_SetDrawColor(&u8g2, 2);

      //pid的小过渡动画
      shutter_state_box_trg = u8g2_GetStrWidth(&u8g2, "Waiting") * 2 + 20;
      shutter_state_box_err = shutter_state_box_trg - shutter_state_box;

      if (0.5 * shutter_state_box_err > MAX_OUT_WIDTH)
      {
        shutter_state_box += MAX_OUT_WIDTH;
      } else
      {
        shutter_state_box += 0.50 * shutter_state_box_err;
      }

      u8g2_DrawRBox(&u8g2, 0, 16, shutter_state_box, 22, 4);
      u8g2_SetDrawColor(&u8g2, 1);

      if (shutter_state_box == shutter_state_box_trg) shutter_box_is_ok = 1;
      //绘制完成pid后标记

    } else
    {
      shutter_is_ok = 1;
      u8g2_SetFont(&u8g2, u8g2_font_chargen_92_tr);
      u8g2_DrawStr(&u8g2, 2, 34, "Ready!");   //可以拍照
      u8g2_SetFont(&u8g2, u8g2_font_myfont);
      //画XOR圆角填充
      u8g2_SetDrawColor(&u8g2, 2);

      shutter_state_box_trg = u8g2_GetStrWidth(&u8g2, "Ready!") * 2 + 6;
      shutter_state_box_err = shutter_state_box_trg - shutter_state_box;

      shutter_box_is_ok = 0;

      if (0.5 * shutter_state_box_err > MAX_OUT_WIDTH)
      {
        shutter_state_box += MAX_OUT_WIDTH;
      } else
      {
        shutter_state_box += 0.50 * shutter_state_box_err;
      }

      u8g2_DrawRBox(&u8g2, 0, 16, shutter_state_box, 22, 4);
      u8g2_SetDrawColor(&u8g2, 1);

      if (shutter_state_box == shutter_state_box_trg) shutter_box_is_ok = 1;
      //绘制完成pid后标记
    }
  }

  //后期在剩余胶片数字前面加上一个胶片的小图片
  u8g2_DrawStr(&u8g2, 102, 10, "100%");    //电池剩余电量
  u8g2_DrawStr(&u8g2, 102, 21, "3/36");    //剩余胶片数量
  u8g2_DrawUTF8(&u8g2, 35, 63, "未过片!");    //过片情况

  u8g2_DrawStr(&u8g2, 0, 51, "f1.4");    //推荐光圈
  //todo: 将胶片编辑换为胶片感光度选择选项 数据会传到测光模块用于测光
  u8g2_DrawStr(&u8g2, 0, 64, "200");    //推荐快门速度

  if (shutter_is_B == 0)
  {
    //快门速度
    if (!shutter_is_slow)
    {
      //如果没开启慢门
      sprintf(shutter_speed_char, "%d", shutter_speed);
      if (shutter_speed >= 1000) position = 80 - 10 + 2;
      if (shutter_speed >= 100 && shutter_speed < 1000) position = 92 - 8 + 2;
      if (shutter_speed >= 0 && shutter_speed < 100) position = 104 - 6 + 2;

      u8g2_SetFont(&u8g2, u8g2_font_luBIS14_tn);
      u8g2_DrawStr(&u8g2, position, 62, shutter_speed_char);    //显示当前快门速度
      u8g2_SetFont(&u8g2, u8g2_font_myfont);
    } else if (shutter_is_slow)
    {
      //如果开启了慢门
      u8g2_SetFont(&u8g2, u8g2_font_luBIS14_tn);
      if (shutter_speed == 0) u8g2_DrawStr(&u8g2, 105, 62, "1");    //快门速度=0对应一秒
      if (shutter_speed == -25) u8g2_DrawStr(&u8g2, 103, 62, "2");    //快门速度=-25对应两秒
      u8g2_SetFont(&u8g2, u8g2_font_chargen_92_tr);    //绘制秒符号
      u8g2_DrawUTF8(&u8g2, 118, 60, "\"");
      u8g2_SetFont(&u8g2, u8g2_font_myfont);
    }
  } else if (shutter_is_B == 1)
  {
    u8g2_SetFont(&u8g2, u8g2_font_chargen_92_tr);
    u8g2_DrawStr(&u8g2, 113, 62, "B");    //显示B门
    u8g2_SetFont(&u8g2, u8g2_font_myfont);
  }
}
//todo: 考虑仅在需要的时候才给快门通电 否则会导致严重发热

void camera_busy_show()
{
  u8g2_DrawRBox(&u8g2, 28, 14, 70, 31, 7);
  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawRBox(&u8g2, 30, 16, 66, 27, 1);
  u8g2_SetDrawColor(&u8g2, 1);

  u8g2_DrawUTF8(&u8g2, 44, 35, "快门忙!");
}

void camera_not_ok_show()
{
  u8g2_DrawRBox(&u8g2, 18, 14, 90, 31, 7);
  u8g2_SetDrawColor(&u8g2, 2);
  //u8g2_DrawRBox(&u8g2, 19, 15, 88, 29, 1);
  u8g2_DrawRBox(&u8g2, 20, 16, 86, 27, 1);
  u8g2_SetDrawColor(&u8g2, 1);

  u8g2_DrawUTF8(&u8g2, 31, 35, "快门未就绪!");
}

void pid_ui_show()
{
  move_bar(&pid_line_y, &pid_line_y_trg);
  move_screen(&y_pid, &y_trg_pid);
  move(&pid_box_y, &pid_box_y_trg);
  move_width(&pid_box_width, &pid_box_width_trg);
  u8g2_DrawVLine(&u8g2, 126, 0, total_line_length_pid);
  u8g2_DrawPixel(&u8g2, 125, 0);
  u8g2_DrawPixel(&u8g2, 127, 0);

  for (uint8_t i = 0; i < pid_num; ++i)
  {
    u8g2_DrawUTF8(&u8g2, x, 16 * i + y_pid + 12, pid[i].select);
    u8g2_DrawPixel(&u8g2, 125, single_line_length_pid * (i + 1));
    u8g2_DrawPixel(&u8g2, 127, single_line_length_pid * (i + 1));
    if (i == 1)   //如果是动画总开关
    {
      //绘制开关
      u8g2_DrawRBox(&u8g2, 106, 16 * i + y_pid + 3, 16, 10, 3);
      u8g2_SetDrawColor(&u8g2, 2);
      u8g2_DrawBox(&u8g2, 107, 16 * i + y_pid + 5, 14, 6);
      u8g2_DrawBox(&u8g2, 109, 16 * i + y_pid + 4, 10, 1);   //去掉上面的多余部分
      u8g2_DrawBox(&u8g2, 109, 16 * i + y_pid + 11, 10, 1);    //去掉下面的多余部分
      u8g2_SetDrawColor(&u8g2, 1);

      move_switch(&motion_switch_pos, &motion_switch_pos_trg);
      u8g2_DrawRBox(&u8g2, motion_switch_pos, 16 * i + y_pid + 5, 6, 6, 1);   //绘制圆点
    }
  }
  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawRBox(&u8g2, 0, pid_box_y, pid_box_width, 16, 1);
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_DrawVLine(&u8g2, 125, pid_line_y, single_line_length_pid);
  u8g2_DrawVLine(&u8g2, 127, pid_line_y, single_line_length_pid);
}

void pid_edit_ui_show()
{
  u8g2_DrawBox(&u8g2, 16, 16, 96, 31);
  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawBox(&u8g2, 17, 17, 94, 29);
  u8g2_SetDrawColor(&u8g2, 1);

  //u8g2.drawBox(17,17,(int)(Kpid[pid_select]/PID_MAX)),30);
  u8g2_DrawFrame(&u8g2, 18, 36, 60, 8);
  u8g2_DrawBox(&u8g2, 20, 38, (uint8_t) (Kpid[pid_select - 2] / PID_MAX * 56), 4);

  switch (pid_select)
  {
    case 2:
      u8g2_DrawStr(&u8g2, 22, 30, "Editing Kp");
      break;
    case 3:
      u8g2_DrawStr(&u8g2, 22, 30, "Editing Ki");
      break;
    case 4:
      u8g2_DrawStr(&u8g2, 22, 30, "Editing Kd");
      break;
    default:
      break;
  }
  sprintf(kpid, "%.2f", Kpid[pid_select - 2]);
  u8g2_DrawStr(&u8g2, 81, 44, kpid);
}

void text_edit_ui_show()
{
  u8g2_DrawRFrame(&u8g2, 4, 6, 120, 52, 8);
  u8g2_DrawUTF8(&u8g2, (128 - u8g2_GetUTF8Width(&u8g2, "胶片型号编辑")) / 2, 20, "胶片型号编辑");
  u8g2_DrawStr(&u8g2, 10, 38, name);
  u8g2_DrawUTF8(&u8g2, 95, 50, "确定");

  uint8_t box_x = 9;

  if (edit_index < name_len)
  {
    if (blink_flag < BLINK_SPEED / 2)
    {
      for (uint8_t i = 0; i < edit_index; ++i)
      {
        char temp[2] = {name[i], '\0'};
        box_x += u8g2_GetStrWidth(&u8g2, temp);
        if (name[i] != ' ')
        {
          box_x++;
        }
      }
      char temp[2] = {name[edit_index], '\0'};
      u8g2_SetDrawColor(&u8g2, 2);
      u8g2_DrawBox(&u8g2, box_x, 26, u8g2_GetStrWidth(&u8g2, temp) + 2, 16);
      u8g2_SetDrawColor(&u8g2, 1);
    }
  } else
  {
    u8g2_SetDrawColor(&u8g2, 2);
    u8g2_DrawRBox(&u8g2, 93, 38, u8g2_GetUTF8Width(&u8g2, "确定") + 4, 16, 1);
    u8g2_SetDrawColor(&u8g2, 1);
  }

  if (edit_flag)
  {
    if (blink_flag < BLINK_SPEED)
    {
      blink_flag += 1;
    } else
    {
      blink_flag = 0;
    }
  } else
  {
    blink_flag = 0;
  }
}

void about_ui_show()
{
  u8g2_DrawStr(&u8g2, 2, 12, "MCU : STM32");
  u8g2_DrawStr(&u8g2, 2, 28, "FLASH : 64KB");
  u8g2_DrawStr(&u8g2, 2, 44, "SRAM : 20KB");
  u8g2_DrawStr(&u8g2, 2, 60, "SPEED : 72MHz");
}

void fps_show()
{
  if (fps_is_on)
  {
    sprintf(fps, "%hu", FPS);
    u8g2_DrawStr(&u8g2, 105, 10, fps);
  }
}