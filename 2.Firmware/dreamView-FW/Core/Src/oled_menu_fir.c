//
// Created by Fir on 2023/2/8.
//
#include "oled_menu_fir.h"

const uint8_t LOGO[] = {};

SELECT_LIST pid[5] =
  {
    //todo: 把这里的返回挪到第一个
    {"<-返回"},
    {"动画总开关"},
    {"-力度 值↑力度↑"},
    {"-阻尼 值↑阻尼↓"},
    {"-稳态误差"},
  };

SELECT_LIST list[7] =
  {
    //取消单独的MainUI选项 开屏动画仅在开屏时显示
    {"<-返回主界面"},     //返回主界面
    {"-设置"},
    {"-"},
    {"-"},
    {"-胶片型号编辑"},   //胶片型号编辑
    {"-"},     //最近的日志信息 仿命令行设计
    {"-"},
  };

SELECT_LIST setting[9] =
  {
    {"<-返回"},
    {"-快门释放设置"},    //此处里面是很多复选框
    {"-电池信息"},
    {"-动画效果"},
    {"-按键音"},
    {"-测光表数据"},
    {"-日志信息"},
    {"-开发人员选项"},
    {"-关于本机"},
  };

SELECT_LIST shutter_setting[5] =
  {
    {"<-返回"},
    {"无视曝光释放快门"},
    {"未过片释放快门"},
    {"+长曝光快门触发方式"},    //按住和按下再按下
    {"+长曝光时屏幕行为"},   //显示完整版 显示精简版 关闭
  };

SELECT_LIST developer_setting[3] =
  {
    {"<-返回"},
    {"帧率显示开关"},
    {"+物理引脚链接图表"},
  };

SELECT_LIST sound_setting[3] =
  {
    {"<-返回"},
    {"按键音开关"},
    {"+按键音频率"},
  };

char name[] = "Kodak Porta400 ";

double Kpid[3] = {0.06, 0.02, 0.07};

uint8_t pid_num = sizeof(pid) / sizeof(SELECT_LIST);      //pid选项数量

uint8_t list_num;       //选择界面数量

uint8_t setting_num = sizeof(setting) / sizeof(SELECT_LIST);

uint8_t developer_num = sizeof(developer_setting) / sizeof(SELECT_LIST);

uint8_t sound_num = sizeof(sound_setting) / sizeof(SELECT_LIST);

uint8_t shutter_num = sizeof(shutter_setting) / sizeof(SELECT_LIST);

//允许名字的最大长度
const uint8_t name_len = 12;        //0-11 for name 12 for return
