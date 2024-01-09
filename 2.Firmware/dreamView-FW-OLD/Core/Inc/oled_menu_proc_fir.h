//
// Created by Fir on 2023/2/8.
//

#ifndef OLED_MENU_PROC_FIR_H
#define OLED_MENU_PROC_FIR_H

#include "oled_menu_show_fir.h"
#include "oled_key_fir.h"
#include "oled_menu_fir.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void logo_proc();

extern void setting_proc();

extern void shutter_setting_proc();

extern void camera_proc();

extern void camera_not_ok_proc();

extern void camera_busy_proc();

extern void pid_edit_proc(void);

extern void pid_proc();

extern void select_proc(void);

extern void text_edit_proc();

extern void about_proc();

extern void developer_setting_proc();

extern void sound_setting_proc();

//总UI处理
extern void ui_proc();

#ifdef __cplusplus
}
#endif

#endif //OLED_MENU_PROC_FIR_H
