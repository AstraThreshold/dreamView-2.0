/**
 * Code by Fir
 * 声明OLED菜单的相关变量和函数
 */

#ifndef MENU_FIR_H
#define MENU_FIR_H

#include "u8g2.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t LOGO[];

//PID变量 分别对应kp ki kd
extern double Kpid[3];

extern uint8_t ui_index, ui_state;

enum            //ui_index
{
  M_LOGO,       //开屏动画
  M_SELECT,     //主菜单界面
  M_PID,        //PID界面
  M_PID_EDIT,   //pid编辑
  M_ICON,       //icon界面 弃用
  M_CHART,      //图表 弃用
  M_TEXT_EDIT,  //文字编辑 胶片选型
  M_LIGHT,      //测光信息界面
  //M_SHUTTER,    //快门信息界面
  M_CAMERA,     //拍摄主界面
  M_CAMERA_NOT_OK,    //快门未上弦按下快门时的状态
  M_CAMERA_BUSY,
  M_ABOUT,      //关于本机
  M_LOG,        //日志信息界面（包括快门动作信息 上弦动作信息 过片信息 加入时间戳显示）
  M_SETTING,    //setting界面包括日志 测光信息 快门信息 动画编辑（pid）关于本机 电池信息 屏幕亮度调整 屏幕休眠时间
  M_BATTERY,    //电池信息
  M_SHUTTER_SETTING,  //快门释放设置
  M_DEVELOPER_SETTING,   //开发人员选项
  M_SOUND     //按键音设置
};

enum            //ui_state
{
  S_NONE,
  S_DISAPPEAR
};

//菜单结构
typedef struct
{
  char *select;
} SELECT_LIST;

extern SELECT_LIST pid[5];

extern SELECT_LIST list[7];

extern SELECT_LIST setting[9];

extern SELECT_LIST shutter_setting[5];

extern SELECT_LIST developer_setting[3];

extern SELECT_LIST sound_setting[3];

//设备名称
extern char name[];

extern uint8_t pid_num;      //pid选项数量

extern uint8_t list_num;       //选择界面数量

extern uint8_t setting_num;

extern uint8_t developer_num;

extern uint8_t sound_num;

extern uint8_t shutter_num;

//允许名字的最大长度
extern const uint8_t name_len;        //0-11 for name 12 for return

#ifdef __cplusplus
}
#endif

#endif //MENU_FIR_H
