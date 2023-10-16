//
// Created by Fir on 2023/2/8.
//
#ifndef OLED_MENU_SHOW_FIR_H
#define OLED_MENU_SHOW_FIR_H

#include "u8g2.h"
#include "stdio.h"
#include "u8g2_wqy.h"
#include "oled_menu_fir.h"

#define MAX_OUT 20    //PID最大输出限位
#define MAX_OUT_SCREEN 20    //屏幕移动最大输出限位
#define MAX_OUT_WIDTH 15    //PID宽度最大输出限位

#ifdef __cplusplus
extern "C" {
#endif

extern double PID_MAX;      //PID值限位

//TODO!!!!! 重构下面这一坨 用结构体数组代替

//选择界面变量
extern uint8_t x;
extern int16_t y, y_trg;                  //目标和当前值
extern uint8_t line_y, line_y_trg;        //线的位置
extern uint8_t box_width, box_width_trg;  //框的宽度
extern int16_t box_y, box_y_trg;          //框的当前值和目标值
extern int8_t ui_select;                  //当前选中那一栏
extern uint8_t single_line_length;
extern uint8_t total_line_length;

//pid界面变量
extern uint8_t pid_line_y, pid_line_y_trg;          //线的位置
extern uint8_t pid_box_width, pid_box_width_trg;    //框的宽度
extern int16_t pid_box_y, pid_box_y_trg;            //框的当前值和目标值
extern int8_t pid_select;                           //当前选中那一栏
extern uint8_t motion_is_on;
extern int16_t motion_switch_pos, motion_switch_pos_trg;
extern int16_t y_pid, y_trg_pid;
extern uint8_t single_line_length_pid;
extern uint8_t total_line_length_pid;

extern double err_y;    //pid误差y
extern double pre_err_y;   //pid误差y历史 用于积分控制
extern double sum_err_y;   //pid误差y累加 用于积分控制
extern double del_err_y;   //pid误差y近两次之差
extern double sum_out_y;

extern double err_y_switch;    //pid误差y
extern double sum_err_y_switch;   //pid误差y累加 用于积分控制
extern double sum_out_y_switch;

extern double err_bar;
extern double out_bar;

extern double err_screen;

extern double err_str;
extern double out_str;

//主界面变量
extern int16_t shutter_speed;   //快门速度 默认200
extern uint8_t shutter_is_slow;   //是否是慢于1/25的快门
extern uint8_t shutter_is_B;   //是否是B门
extern uint8_t shutter_is_working;   //是否正在动作
extern uint8_t shutter_busy_state;
extern uint8_t shutter_is_ok;    //是否上弦完毕
extern uint8_t position;
extern int16_t shutter_state_box_trg;
extern int16_t shutter_state_box;
extern double shutter_state_box_err;
extern uint8_t shutter_box_is_ok;

extern uint8_t disappear_step;

//指向buffer首地�?????的指�?????
extern uint8_t *buf_ptr;
extern uint16_t buf_len;

extern uint8_t x_setting;
extern int16_t y_setting, y_trg_setting;                  //目标和当前值
extern uint8_t line_y_setting, line_y_trg_setting;        //线的位置
extern uint8_t single_line_length_setting;
extern uint8_t total_line_length_setting;
extern int8_t setting_select;
extern uint8_t setting_box_width, setting_box_width_trg;    //框的宽度
extern int16_t setting_box_y, setting_box_y_trg;            //框的当前值和目标�?????
extern int16_t fps_switch_pos, fps_switch_pos_trg;

extern uint8_t x_developer;
extern int16_t y_developer, y_trg_developer;                  //目标和当前值
extern uint8_t line_y_developer, line_y_trg_developer;        //线的位置
extern uint8_t single_line_length_developer;
extern uint8_t total_line_length_developer;
extern int8_t developer_select;
extern uint8_t developer_box_width, developer_box_width_trg;    //框的宽度
extern int16_t developer_box_y, developer_box_y_trg;            //框的当前值和目标�?????

extern uint8_t x_sound;
extern int16_t y_sound, y_trg_sound;                  //目标和当前值
extern uint8_t line_y_sound, line_y_trg_sound;        //线的位置
extern uint8_t single_line_length_sound;
extern uint8_t total_line_length_sound;
extern int8_t sound_select;
extern uint8_t sound_box_width, sound_box_width_trg;    //框的宽度
extern int16_t sound_box_y, sound_box_y_trg;            //框的当前值和目标�?????
extern int16_t sound_switch_pos, sound_switch_pos_trg;

extern uint8_t x_shutter;
extern int16_t y_shutter, y_trg_shutter;
extern uint8_t line_y_shutter, line_y_trg_shutter;
extern uint8_t single_line_length_shutter;
extern uint8_t total_line_length_shutter;
extern int8_t shutter_select;
extern uint8_t shutter_box_width, shutter_box_width_trg;    //框的宽度
extern int16_t shutter_box_y, shutter_box_y_trg;            //框的当前值和目标值
extern uint8_t shut_nolight_is_on;
extern int16_t shut_nolight_switch_pos, shut_nolight_switch_pos_trg;
extern uint8_t shut_nonext_is_on;
extern int16_t shut_nonext_switch_pos, shut_nonext_switch_pos_trg;
extern uint8_t shut_nolight_switch_need_moving, shut_nonext_switch_need_moving;

extern uint8_t edit_index;
extern int edit_flag;                  //默认不在编辑
extern uint8_t blink_flag;                 //默认高亮
extern const uint8_t BLINK_SPEED;     //2的�?�数

extern uint16_t FPS_Count;    //Fps
extern uint16_t FPS;
static uint16_t ms_Count = 0;
extern uint8_t fps_is_on;

extern uint8_t sound_is_on;

//sprintf函数用 u8g2显示数字
extern char shutter_speed_char[20];
extern char kpid[101];
extern char fps[20];

/**模块显示**/
extern void move_screen(int16_t *a, const int16_t *a_trg);

extern void move(int16_t *a, const int16_t *a_trg);

extern uint8_t move_switch(int16_t *a, const int16_t *a_trg);

extern void move_width(uint8_t *a, const uint8_t *a_trg);

extern void move_bar(uint8_t *a, const uint8_t *a_trg);

extern void move_str(uint8_t *a, const uint8_t *a_trg);

extern void text_edit(int dir, uint8_t index);

extern void disappear();

/**界面显示**/
extern void logo_ui_show();

extern void select_ui_show();

extern void setting_ui_show();

extern void developer_setting_show();

extern void sound_setting_show();

extern void shutter_setting_show();

extern void camera_ui_show();

extern void camera_not_ok_show();

extern void camera_busy_show();

extern void pid_ui_show();

extern void pid_edit_ui_show();

extern void text_edit_ui_show();

extern void about_ui_show();

extern void fps_show();

#ifdef __cplusplus
}
#endif

#endif //OLED_MENU_SHOW_FIR_H
