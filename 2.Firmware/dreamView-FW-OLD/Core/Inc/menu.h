//
// Created by Fir on 2023/4/18.
//
#ifndef MENU_H
#define MENU_H

#define MAX_OUT 20    //PID最大输出限位
#define MAX_OUT_SCREEN 20    //屏幕移动最大输出限位
#define MAX_OUT_WIDTH 15    //PID宽度最大输出限位

#include "oled_menu_fir.h"
#include "algorithm"

#ifdef __cplusplus
extern "C" {
#endif

extern double kPID[3];

extern uint8_t motionIsOn;

typedef struct
{
  const char *select;
} newSelectList;



class menu
{
public:
  friend class newSwitch;

  newSelectList *selectList;

  uint8_t x;
  int16_t y, yTrg;
  int16_t yLine, yLineTrg;
  uint8_t widthBox, widthBoxTrg;
  int16_t yBox, yBoxTrg;
  int16_t yFont[20] = {0};
  int16_t yFontTrg[20] = {0};

  uint8_t lengthSingleLine;
  uint8_t lengthTotalLine;

  double yBoxErr;
  double yBoxSumErr, yBoxDelErr, yBoxPreErr;
  double yBoxSumOut;

  double yBarErr;
  double yBarOut;

  double yScreenErr;

  double yFontErr;

  double widthErr;
  double widthOut;

  int8_t uiSelect;
  uint8_t listNum;

  //传入选择列表名 而后将类成员变量指针指向列表
  menu(newSelectList _list[], uint8_t _listNum);

  void moveBox(int16_t *_a, const int16_t *_aTrg); //选择框
  void moveBar(int16_t *_a, const int16_t *_aTrg); //进度条
  void moveScreen(int16_t *_a, const int16_t *_aTrg);  //屏幕
  void moveFont(int16_t *_a, const int16_t *_aTrg);   //文字
  void moveWidth(uint8_t *_a, const uint8_t *_a_trg);   //选择框宽度
  void show(uint8_t _motionIsOn);
};

class menuSwitch
{
private:
  uint16_t xSwitch, xSwitchTrg;

  double ySwitchErr;
  double ySwitchSumErr;
  double ySwitchSumOut;

public:
  uint8_t switchIsOn;

  menuSwitch();

  uint8_t move(uint16_t *_a, const uint16_t *_a_trg);

  void show(uint8_t _pos, menu _menu, uint8_t _switchIsOn); //_pos为生成开关的初始位置 0为第一号
};

extern newSelectList mainMenuList[7];

extern void mainMenuProc(menu*);

class ui
{
protected:
  uint8_t uiIndex, uiState;

//  enum
//  {
//    M_LOGO,       //开屏动画
//    M_SELECT,     //主菜单界面
//    M_PID,        //PID界面
//    M_PID_EDIT,   //pid编辑
//    M_TEXT_EDIT,  //文字编辑 胶片选型
//    M_LIGHT,      //测光信息界面
//    //M_SHUTTER,    //快门信息界面
//    M_CAMERA,     //拍摄主界面
//    M_CAMERA_NOT_OK,    //快门未上弦按下快门时的状态
//    M_CAMERA_BUSY,
//    M_ABOUT,      //关于本机
//    M_LOG,        //日志信息界面（包括快门动作信息 上弦动作信息 过片信息 加入时间戳显示）
//    M_SETTING,    //setting界面包括日志 测光信息 快门信息 动画编辑（pid）关于本机 电池信息 屏幕亮度调整 屏幕休眠时间
//    M_BATTERY,    //电池信息
//    M_SHUTTER_SETTING,  //快门释放设置
//    M_DEVELOPER_SETTING,   //开发人员选项
//    M_SOUND     //按键音设置
//  };
//
//  enum            //ui_state
//  {
//    S_NONE,
//    S_DISAPPEAR
//  };

public:
  ui();

  void mainMenuProc(menu *_mainMenu);//TODO:后期删掉这个函数

  //选择菜单类的选择函数 如果不是选择菜单类函数需要单独定义  
  uint8_t uiSelect(menu *_menu);

  //ui调度器 使用选择函数 内部包含选择哪个要去到哪个位置
  void uiProcess();
};

#ifdef __cplusplus
}
#endif

#endif //MENU_H
