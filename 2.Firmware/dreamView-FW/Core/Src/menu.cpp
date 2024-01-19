//
// Created by Fir on 2023/4/18.
//
#include <cstring>
#include <cmath>
#include <string>
#include "menu.h"
#include "oled.h"
#include "key.h"

//todo: 关于修改数值和多选框 核心思想: 都是对value数组进行改动 同时显示的也是value数组 最后再统一将value同步到参数数组中

//todo: 拍摄界面如果有需要报警的参数 如曝光不正确等 需要闪烁叹号来示意
//todo: 把所有带选项框的页面都重构 否则用不了（因为n-1都变成了n）
//todo: 现在在菜单无法修改参数的真实值 因为param还未与value进行同步 菜单修改的是value的值 并不是param的值
/************************************* 定义页面 *************************************/

//总目录，缩进表示页面层级
enum {
  M_WINDOW,
  M_SLEEP,
  M_MAIN,
  M_CAM,
  M_CAMSETTING,
  M_VOLT,
  M_SETTING,
  M_ABOUT,
};

//状态，初始化标签
enum {
  S_FADE,       //转场动画
  S_WINDOW,     //弹窗初始化
  S_LAYER_IN,   //层级初始化
  S_LAYER_OUT,  //层级初始化
  S_NONE,       //直接选择页面
};

//菜单结构体
typedef struct MENU {
  const char *m_select;
} M_SELECT;

/************************************* 定义内容 *************************************/

/************************************* 文字内容 *************************************/

M_SELECT main_menu[]
  {
    {"休眠模式"},
    {"拍摄模式"},
    {"相机设置"},
    {"系统设置"},
  };

M_SELECT cam_setting_menu[]
  {
    {"[ 相机设置 ]"},
    {"~ 曝光许用误差"},
    {"+ 强制释放快门"},
    //todo: 此处的'#'代表单选列表 仔细研究一下
    {"# B门触发方式"},
    {"# B门屏幕行为"},
  };

M_SELECT knob_menu[]
  {
    {"[ Knob ]"},
    {"# Rotate Func"},
    {"$ Press Func"},
  };

M_SELECT krf_menu[]
  {
    {"[ Rotate Function ]"},
    {"--------------------------"},
    {"= Disable"},
    {"--------------------------"},
    {"= Volume"},
    {"= Brightness"},
    {"--------------------------"},
  };

M_SELECT kpf_menu[]
  {
    {"[ Press Function ]"},
    {"--------------------------"},
    {"= Disable"},
    {"--------------------------"},
    {"= A"},
    {"= B"},
    {"= C"},
    {"= D"},
    {"= E"},
    {"= F"},
    {"= G"},
    {"= H"},
    {"= I"},
    {"= J"},
    {"= K"},
    {"= L"},
    {"= M"},
    {"= N"},
    {"= O"},
    {"= P"},
    {"= Q"},
    {"= R"},
    {"= S"},
    {"= T"},
    {"= U"},
    {"= V"},
    {"= W"},
    {"= X"},
    {"= Y"},
    {"= Z"},
    {"--------------------------"},
    {"= 0"},
    {"= 1"},
    {"= 2"},
    {"= 3"},
    {"= 4"},
    {"= 5"},
    {"= 6"},
    {"= 7"},
    {"= 8"},
    {"= 9"},
    {"--------------------------"},
    {"= Esc"},
    {"= F1"},
    {"= F2"},
    {"= F3"},
    {"= F4"},
    {"= F5"},
    {"= F6"},
    {"= F7"},
    {"= F8"},
    {"= F9"},
    {"= F10"},
    {"= F11"},
    {"= F12"},
    {"--------------------------"},
    {"= Left Ctrl"},
    {"= Left Shift"},
    {"= Left Alt"},
    {"= Left Win"},
    {"= Right Ctrl"},
    {"= Right Shift"},
    {"= Right Alt"},
    {"= Right Win"},
    {"--------------------------"},
    {"= Caps Lock"},
    {"= Backspace"},
    {"= Return"},
    {"= Insert"},
    {"= Delete"},
    {"= Tab"},
    {"--------------------------"},
    {"= Home"},
    {"= End"},
    {"= Page Up"},
    {"= Page Down"},
    {"--------------------------"},
    {"= Up Arrow"},
    {"= Down Arrow"},
    {"= Left Arrow"},
    {"= Right Arrow"},
    {"--------------------------"},
  };

M_SELECT volt_menu[]
  {
    {"A0"},
    {"A1"},
    {"A2"},
    {"A3"},
    {"A4"},
    {"A5"},
    {"A6"},
    {"A7"},
    {"B0"},
    {"B1"},
  };

//todo: 把所有需要复选框或者弹窗的界面都这么搞
struct {
  M_SELECT menu[6];
  uint8_t value[6];
} setting_menu = {{
                    {"[ 系统设置 ]"},
                    {"~ 屏幕亮度"},
                    {"+ 按键音"},
                    {"+ 暗黑模式"},
                    {"- [ 测试 ]"},
                    {"- [ 关于本机 ]"},
                  }, {'\0', 255, 0, 1, '\0', '\0'}};

M_SELECT about_menu[]
  {
    {"[ dreamView ]"},
    {"- HW Version: v2.3"},
    {"- FW Version: v2.0"},
    {"- 由@dreamUI驱动"},
    {"- UI Version: v1.0"},
    {"- Based on WouoUI"},
    {"- by 无理造物"},
    {"- Bili UID: 43483079"},
  };

/************************************* 文字内容 *************************************/

/************************************* 图片内容 *************************************/

const uint8_t main_icon_pic[][120]
  {
    {
      0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xF1, 0x3F,
      0xFF, 0xFF, 0xC3, 0x3F, 0xFF, 0xFF, 0x87, 0x3F, 0xFF, 0xFF, 0x07, 0x3F, 0xFF, 0xFF, 0x0F, 0x3E,
      0xFF, 0xFF, 0x0F, 0x3E, 0xFF, 0xFF, 0x0F, 0x3C, 0xFF, 0xFF, 0x0F, 0x3C, 0xFF, 0xFF, 0x0F, 0x38,
      0xFF, 0xFF, 0x0F, 0x38, 0xFF, 0xFF, 0x0F, 0x38, 0xFF, 0xFF, 0x07, 0x38, 0xFF, 0xFF, 0x07, 0x38,
      0xFF, 0xFF, 0x03, 0x38, 0xF7, 0xFF, 0x01, 0x38, 0xE7, 0xFF, 0x00, 0x3C, 0x87, 0x3F, 0x00, 0x3C,
      0x0F, 0x00, 0x00, 0x3E, 0x0F, 0x00, 0x00, 0x3E, 0x1F, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x80, 0x3F,
      0x7F, 0x00, 0xC0, 0x3F, 0xFF, 0x01, 0xF0, 0x3F, 0xFF, 0x07, 0xFC, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F,
      0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F
    },
    {
      0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xF9, 0xE7, 0x3F,
      0xFF, 0xF9, 0xE7, 0x3F, 0xFF, 0xF9, 0xE7, 0x3F, 0xFF, 0xF0, 0xE7, 0x3F, 0x7F, 0xE0, 0xE7, 0x3F,
      0x7F, 0xE0, 0xC3, 0x3F, 0x7F, 0xE0, 0xC3, 0x3F, 0x7F, 0xE0, 0xC3, 0x3F, 0x7F, 0xE0, 0xE7, 0x3F,
      0xFF, 0xF0, 0xE7, 0x3F, 0xFF, 0xF9, 0xE7, 0x3F, 0xFF, 0xF9, 0xE7, 0x3F, 0xFF, 0xF9, 0xE7, 0x3F,
      0xFF, 0xF9, 0xE7, 0x3F, 0xFF, 0xF9, 0xC3, 0x3F, 0xFF, 0xF9, 0x81, 0x3F, 0xFF, 0xF0, 0x81, 0x3F,
      0xFF, 0xF0, 0x81, 0x3F, 0xFF, 0xF0, 0x81, 0x3F, 0xFF, 0xF9, 0x81, 0x3F, 0xFF, 0xF9, 0xC3, 0x3F,
      0xFF, 0xF9, 0xE7, 0x3F, 0xFF, 0xF9, 0xE7, 0x3F, 0xFF, 0xF9, 0xE7, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F,
      0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F
    },
    {
      0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xEF, 0xFF, 0xFF, 0x3F, 0xC7, 0xFF, 0xFF, 0x3F,
      0xC7, 0xF3, 0xFF, 0x3F, 0x83, 0xC0, 0xFF, 0x3F, 0xEF, 0xCC, 0xFF, 0x3F, 0x6F, 0x9E, 0xFF, 0x3F,
      0x6F, 0x9E, 0xFF, 0x3F, 0x2F, 0x3F, 0xFF, 0x3F, 0x2F, 0x3F, 0xFF, 0x3F, 0x8F, 0x7F, 0xFE, 0x3F,
      0x8F, 0x7F, 0xFE, 0x39, 0x8F, 0x7F, 0xFE, 0x39, 0xCF, 0xFF, 0xFC, 0x3C, 0xCF, 0xFF, 0xFC, 0x3C,
      0xEF, 0xFF, 0xFC, 0x3C, 0xEF, 0xFF, 0x79, 0x3E, 0xEF, 0xFF, 0x79, 0x3E, 0xEF, 0xFF, 0x33, 0x3F,
      0xEF, 0xFF, 0x33, 0x3F, 0xEF, 0xFF, 0x87, 0x3F, 0xEF, 0xFF, 0xCF, 0x3F, 0xEF, 0xFF, 0x7F, 0x3E,
      0xEF, 0xFF, 0x7F, 0x38, 0x0F, 0x00, 0x00, 0x30, 0xFF, 0xFF, 0x7F, 0x38, 0xFF, 0xFF, 0x7F, 0x3E,
      0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F,
    },
    {
      0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F,
      0xFF, 0x1F, 0xFE, 0x3F, 0xFF, 0x1F, 0xFE, 0x3F, 0xFF, 0x0C, 0xCC, 0x3F, 0x7F, 0x00, 0x80, 0x3F,
      0x3F, 0x00, 0x00, 0x3F, 0x3F, 0xE0, 0x01, 0x3F, 0x7F, 0xF8, 0x87, 0x3F, 0x7F, 0xFC, 0x8F, 0x3F,
      0x3F, 0xFC, 0x0F, 0x3F, 0x0F, 0x3E, 0x1F, 0x3C, 0x0F, 0x1E, 0x1E, 0x3C, 0x0F, 0x1E, 0x1E, 0x3C,
      0x0F, 0x3E, 0x1F, 0x3C, 0x3F, 0xFC, 0x0F, 0x3F, 0x7F, 0xFC, 0x8F, 0x3F, 0x7F, 0xF8, 0x87, 0x3F,
      0x3F, 0xE0, 0x01, 0x3F, 0x3F, 0x00, 0x00, 0x3F, 0x7F, 0x00, 0x80, 0x3F, 0xFF, 0x0C, 0xCC, 0x3F,
      0xFF, 0x1F, 0xFE, 0x3F, 0xFF, 0x1F, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F,
      0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F
    },
  };

/************************************* 图片内容 *************************************/

/************************************* 页面变量 *************************************/

//OLED变量
#define   DISP_H              64    //屏幕高度
#define   DISP_W              128   //屏幕宽度
uint8_t *buf_ptr;                 //指向屏幕缓冲的指针
uint16_t buf_len;                  //缓冲长度

//UI变量
#define   UI_DEPTH            20    //最深层级数
#define   UI_MNUMB            100   //菜单数量
#define   UI_PARAM            16    //参数数量

enum {
  DISP_BRI,     //屏幕亮度
  TILE_ANI,     //磁贴动画速度
  LIST_ANI,     //列表动画速度
  WIN_ANI,      //弹窗动画速度
  SPOT_ANI,     //聚光动画速度
  TAG_ANI,      //标签动画速度
  FADE_ANI,     //消失动画速度
  BTN_SPT,      //按键短按时长
  BTN_LPT,      //按键长按时长
  TILE_UFD,     //磁贴图标从头展开开关
  LIST_UFD,     //菜单列表从头展开开关
  TILE_LOOP,    //磁贴图标循环模式开关
  LIST_LOOP,    //菜单列表循环模式开关
  WIN_BOK,      //弹窗背景虚化开关
  KNOB_DIR,     //旋钮方向切换开关
  DARK_MODE,    //黑暗模式开关
};

struct {
  bool init;
  uint8_t num[UI_MNUMB];
  uint8_t select[UI_DEPTH];
  uint8_t layer;
  uint8_t index = M_SLEEP;
  uint8_t state = S_NONE;
  bool sleep = true;
  uint8_t fade = 1;
  uint8_t param[UI_PARAM];
} ui;

//磁贴变量
//所有磁贴页面都使用同一套参数
#define   TILE_B_FONT         u8g2_font_myfont        //磁贴大标题字体
#define   TILE_B_TITLE_H      18                          //磁贴大标题字体高度
#define   TILE_ICON_H         30                          //磁贴图标高度
#define   TILE_ICON_W         30                          //磁贴图标宽度
#define   TILE_ICON_S         38                          //磁贴图标间距
#define   TILE_ICON_L         7                           //磁贴图标上边距
#define   TILE_INDI_H         27                          //磁贴标题指示器高度
#define   TILE_INDI_S         38                          //磁贴标题上边距
#define   TILE_LINE_S         46                          //磁贴标题虚线上边距
#define   TILE_ARROW_S        4                           //磁贴标题箭头侧边距
#define   TILE_ARROW_L        56                          //磁贴标题箭头上边距
#define   TILE_ARROW_W        6                           //磁贴标题箭头长度
#define   TILE_BTN_S          16                          //磁贴按钮图标侧边距
struct {
  float title_y_calc = TILE_INDI_S + (TILE_INDI_H - TILE_B_TITLE_H) / 2.0 + TILE_B_TITLE_H * 2;
  float title_y_trg_calc = TILE_INDI_S + (TILE_INDI_H - TILE_B_TITLE_H) / 2.0 + TILE_B_TITLE_H;
  int16_t temp{};
  bool select_flag{};
  float arrow_y{};
  float arrow_y_trg{};
  float line_y{};
  float line_y_trg{};
  float icon_x{};
  float icon_x_trg{};
  float icon_y{};
  float icon_y_trg{};
  float title_y{};
  float title_y_trg{};
} tile;

//列表变量
//默认参数

#define   LIST_FONT           u8g2_font_myfont   //列表字体
#define   LIST_TEXT_H         8                           //列表每行文字字体的高度
#define   LIST_LINE_H         16                          //列表单行高度
#define   LIST_TEXT_S         4                           //列表每行文字的上边距，左边距和右边距，下边距由它和字体高度和行高度决定
#define   LIST_BAR_W          5                           //列表进度条宽度，需要是奇数，因为正中间有1像素宽度的线
#define   LIST_BOX_R          0.5f                        //列表选择框圆角

struct {
  uint8_t line_n = DISP_H / LIST_LINE_H;
  int16_t temp;
  bool loop;
  float y;
  float y_trg;
  float box_x;
  float box_x_trg;
  float box_y;
  float box_y_trg[UI_DEPTH];
  float bar_y;
  float bar_y_trg;
} list;

//曲线相关
#define   WAVE_SAMPLE         20                          //采集倍数
#define   WAVE_W              94                          //波形宽度
#define   WAVE_L              24                          //波形左边距
#define   WAVE_U              0                           //波形上边距
#define   WAVE_MAX            27                          //最大值
#define   WAVE_MIN            4                           //最小值
#define   WAVE_BOX_H          32                          //波形边框高度
#define   WAVE_BOX_W          94                          //波形边框宽度
#define   WAVE_BOX_L_S        24                          //波形边框左边距
//列表和文字背景框相关
#define   VOLT_FONT           u8g2_font_helvB18_tr        //电压数字字体
#define   VOLT_TEXT_BG_L_S    24                          //文字背景框左边距
#define   VOLT_TEXT_BG_W      94                          //文字背景框宽度
#define   VOLT_TEXT_BG_H      29                          //文字背景框高度
struct {
  int ch0_adc[WAVE_SAMPLE * WAVE_W];
  int ch0_wave[WAVE_W];
  int val;
  float text_bg_r;
  float text_bg_r_trg;
} volt;

//选择框变量

//默认参数
#define   CHECK_BOX_L_S       95                          //选择框在每行的左边距
#define   CHECK_BOX_U_S       2                           //选择框在每行的上边距
#define   CHECK_BOX_F_W       12                          //选择框外框宽度
#define   CHECK_BOX_F_H       12                          //选择框外框高度
#define   CHECK_BOX_D_S       2                           //选择框里面的点距离外框的边距

struct {
  uint8_t *v;
  uint8_t *m;
  uint8_t *s;
  uint8_t *s_p;
} check_box;

//弹窗变量
#define   WIN_FONT            u8g2_font_myfont   //弹窗字体
#define   WIN_H               32                          //弹窗高度
#define   WIN_W               102                         //弹窗宽度
#define   WIN_BAR_W           92                          //弹窗进度条宽度
#define   WIN_BAR_H           7                           //弹窗进度条高度
#define   WIN_Y               - WIN_H - 2                 //弹窗竖直方向出场起始位置
#define   WIN_Y_TRG           - WIN_H - 2                 //弹窗竖直方向退场终止位置
struct {
  //uint8_t
  uint8_t *value;
  uint8_t max;
  uint8_t min;
  uint8_t step;

  MENU *bg;
  uint8_t index;
  char title[20];
  uint8_t select;
  uint8_t l = (DISP_W - WIN_W) / 2;
  uint8_t u = (DISP_H - WIN_H) / 2;
  float bar;
  float bar_trg;
  float y;
  float y_trg;
} win;

//聚光灯变量
struct {
  float l;
  float l_trg;
  float r;
  float r_trg;
  float u;
  float u_trg;
  float d;
  float d_trg;
} spot;

/************************************* 页面变量 *************************************/

/********************************** 自定义功能变量 **********************************/

//旋钮功能变量
#define   KNOB_PARAM          4
#define   KNOB_DISABLE        0
#define   KNOB_ROT_VOL        1
#define   KNOB_ROT_BRI        2
enum {
  KNOB_ROT,       //睡眠下旋转旋钮的功能，0 禁用，1 音量，2 亮度
  KNOB_COD,       //睡眠下短按旋钮输入的字符码，0 禁用
  KNOB_ROT_P,     //旋转旋钮功能在单选框中选择的位置
  KNOB_COD_P,     //字符码在单选框中选择的位置
};
struct {
  uint8_t param[KNOB_PARAM] = {KNOB_DISABLE, KNOB_DISABLE, 2, 2}; //禁用在列表的第2个选项，第0个是标题，第1个是分界线
} knob;

/********************************** 自定义功能变量 **********************************/

/************************************ 初始化函数 ***********************************/

/********************************* 初始化数据处理函数 *******************************/

//显示数值的初始化
void check_box_v_init(uint8_t *param)
{
  check_box.v = param;
}

//多选框的初始化
void check_box_m_init(uint8_t *param)
{
  check_box.m = param;
}

//单选框时的初始化
void check_box_s_init(uint8_t *param, uint8_t *param_p)
{
  check_box.s = param;
  check_box.s_p = param_p;
}

//多选框处理函数
void check_box_m_select(uint8_t param)
{
  check_box.m[param] = !check_box.m[param];
}

//单选框处理函数
void check_box_s_select(uint8_t val, uint8_t pos)
{
  *check_box.s = val;
  *check_box.s_p = pos;
}

//弹窗数值初始化
void window_value_init(char title[], uint8_t select, uint8_t *value, uint8_t max, uint8_t min, uint8_t step, MENU *bg,
                       uint8_t index)
{
  strcpy(win.title, title);
  win.select = select;
  win.value = value;
  win.max = max;
  win.min = min;
  win.step = step;
  win.bg = bg;
  win.index = index;
  ui.index = M_WINDOW;
  ui.state = S_WINDOW;
}

/*********************************** UI 初始化函数 *********************************/

//在初始化EEPROM时，选择性初始化的默认设置
void ui_param_init()
{
  ui.param[DISP_BRI] = 255;      //屏幕亮度
  ui.param[TILE_ANI] = 50;       //磁贴动画速度
  ui.param[LIST_ANI] = 60;       //列表动画速度
  ui.param[WIN_ANI] = 25;       //弹窗动画速度
  ui.param[SPOT_ANI] = 50;       //聚光动画速度
  ui.param[TAG_ANI] = 60;       //标签动画速度
  ui.param[FADE_ANI] = 30;       //消失动画速度
  ui.param[BTN_SPT] = 25;       //按键短按时长
  ui.param[BTN_LPT] = 150;      //按键长按时长
  ui.param[TILE_UFD] = 1;        //磁贴图标从头展开开关
  ui.param[LIST_UFD] = 1;        //菜单列表从头展开开关
  ui.param[TILE_LOOP] = 1;        //磁贴图标循环模式开关
  ui.param[LIST_LOOP] = 1;        //菜单列表循环模式开关
  ui.param[WIN_BOK] = 1;        //弹窗背景虚化开关
  ui.param[KNOB_DIR] = 0;        //旋钮方向切换开关
  ui.param[DARK_MODE] = 1;        //黑暗模式开关
}

//列表类页面列表行数初始化，必须初始化的参数
void ui_init()
{
  ui.num[M_MAIN] = sizeof(main_menu) / sizeof(M_SELECT);
  ui.num[M_CAMSETTING] = sizeof(cam_setting_menu) / sizeof(M_SELECT);
  ui.num[M_VOLT] = sizeof(volt_menu) / sizeof(M_SELECT);
  ui.num[M_SETTING] = sizeof(setting_menu.menu) / sizeof(M_SELECT);
  ui.num[M_ABOUT] = sizeof(about_menu) / sizeof(M_SELECT);
}

/********************************* 分页面初始化函数 ********************************/

//进入磁贴类时的初始化
void tile_param_init()
{
  ui.init = false;
  tile.icon_x = 0;
  tile.icon_x_trg = TILE_ICON_S;
  tile.arrow_y = 76;    //为了让其到位的速度慢一点
  tile.arrow_y_trg = TILE_ARROW_L;
  tile.line_y = 65;
  tile.line_y_trg = TILE_LINE_S;
  tile.icon_y = -TILE_ICON_H * 2;
  tile.icon_y_trg = TILE_ICON_L;
  tile.title_y = tile.title_y_calc;
  tile.title_y_trg = tile.title_y_trg_calc;
}

//进入睡眠时的初始化
void sleep_param_init()
{
  u8g2_SetDrawColor(&u8g2, 0);
  u8g2_DrawBox(&u8g2, 0, 0, DISP_W, DISP_H);
  //u8g2_SetPowerSave(&u8g2, 1);
  ui.state = S_NONE;
  ui.sleep = true;
}

//电压测量页初始化
void volt_param_init()
{
  volt.text_bg_r = 0;
  volt.text_bg_r_trg = VOLT_TEXT_BG_W;
}

//设置页初始化
void setting_param_init()
{
  check_box_v_init(setting_menu.value);
  check_box_m_init(setting_menu.value);
}

/********************************** 通用初始化函数 *********************************/

/*
  页面层级管理逻辑是，把所有页面都先当作列表类初始化，不是列表类按需求再初始化对应函数
  这样做会浪费一些资源，但跳转页面时只需要考虑页面层级，逻辑上更清晰，减少出错
*/

//弹窗动画初始化
void window_param_init()
{
  win.bar = 0;
  win.y = WIN_Y;
  win.y_trg = win.u;
  ui.state = S_NONE;
}

//进入更深层级时的初始化
void layer_init_in()
{
  ui.layer++;
  ui.init = 0;
  list.y = 0;
  list.y_trg = LIST_LINE_H;
  list.box_x = 0;
  list.box_y = 0;
  list.bar_y = 0;
  ui.state = S_FADE;
  switch (ui.index)
  {
    case M_MAIN:
      tile_param_init();
      break;  //睡眠进入主菜单，动画初始化
    case M_VOLT:
      volt_param_init();
      break;  //主菜单进入电压测量页，动画初始化
    case M_SETTING:
      setting_param_init();
      break;  //主菜单进入设置页，单选框初始化
  }
}

//进入更浅层级时的初始化
void layer_init_out()
{
  ui.select[ui.layer] = 0;
  list.box_y_trg[ui.layer] = 0;
  ui.layer--;
  ui.init = false;
  list.y = 0;
  list.y_trg = LIST_LINE_H;
  list.bar_y = 0;
  ui.state = S_FADE;
  switch (ui.index)
  {
    case M_SLEEP:
      sleep_param_init();
      break;    //主菜单进入睡眠页，检查是否需要写EEPROM
    case M_MAIN:
      tile_param_init();
      break;    //不管什么页面进入主菜单时，动画初始化
  }
}

/************************************* 动画函数 *************************************/

//动画函数
void animation(float *a, float *a_trg, uint8_t n)
{
  if (*a != *a_trg)
  {
    if (fabs(*a - *a_trg) < 0.15f) *a = *a_trg;
    else *a += (*a_trg - *a) / (ui.param[n] / 10.0f);
  }
}

//消失函数
void fade()
{
  HAL_Delay(ui.param[FADE_ANI]);
  if (ui.param[DARK_MODE])
  {
    switch (ui.fade)
    {
      case 1:
        for (uint16_t i = 0; i < buf_len; ++i) if (i % 2 != 0) buf_ptr[i] = buf_ptr[i] & 0xAA;
        break;
      case 2:
        for (uint16_t i = 0; i < buf_len; ++i) if (i % 2 != 0) buf_ptr[i] = buf_ptr[i] & 0x00;
        break;
      case 3:
        for (uint16_t i = 0; i < buf_len; ++i) if (i % 2 == 0) buf_ptr[i] = buf_ptr[i] & 0x55;
        break;
      case 4:
        for (uint16_t i = 0; i < buf_len; ++i) if (i % 2 == 0) buf_ptr[i] = buf_ptr[i] & 0x00;
        break;
      default:
        ui.state = S_NONE;
        ui.fade = 0;
        break;
    }
  } else
  {
    switch (ui.fade)
    {
      case 1:
        for (uint16_t i = 0; i < buf_len; ++i) if (i % 2 != 0) buf_ptr[i] = buf_ptr[i] | 0xAA;
        break;
      case 2:
        for (uint16_t i = 0; i < buf_len; ++i) if (i % 2 != 0) buf_ptr[i] = buf_ptr[i] | 0x00;
        break;
      case 3:
        for (uint16_t i = 0; i < buf_len; ++i) if (i % 2 == 0) buf_ptr[i] = buf_ptr[i] | 0x55;
        break;
      case 4:
        for (uint16_t i = 0; i < buf_len; ++i) if (i % 2 == 0) buf_ptr[i] = buf_ptr[i] | 0x00;
        break;
      default:
        ui.state = S_NONE;
        ui.fade = 0;
        break;
    }
  }
  ui.fade++;
}

/************************************* 显示函数 *************************************/

//磁贴类页面通用显示函数
void tile_show(struct MENU arr_1[], const uint8_t icon_pic[][120])
{
  //计算动画过渡值
  animation(&tile.icon_x, &tile.icon_x_trg, TILE_ANI);
  animation(&tile.icon_y, &tile.icon_y_trg, TILE_ANI);
  animation(&tile.title_y, &tile.title_y_trg, TILE_ANI);
  animation(&tile.line_y, &tile.line_y_trg, TILE_ANI);
  animation(&tile.arrow_y, &tile.arrow_y_trg, TILE_ANI);

  //设置大标题的颜色，0透显，1实显，2反色，这里用实显
  u8g2_SetDrawColor(&u8g2, 1);

  //绘制标题
  u8g2_SetFont(&u8g2, TILE_B_FONT);
  u8g2_DrawUTF8(&u8g2,
                ((DISP_W) - u8g2_GetUTF8Width(&u8g2, arr_1[ui.select[ui.layer]].m_select)) / 2,
                tile.title_y, arr_1[ui.select[ui.layer]].m_select);

  //绘制图标
  if (!ui.init)
  {
    //按钮和箭头从下向上缓动
    /**画左箭头**/
    u8g2_DrawHLine(&u8g2, TILE_ARROW_S, tile.arrow_y, TILE_ARROW_W);
    u8g2_DrawPixel(&u8g2, TILE_ARROW_S + 1, tile.arrow_y + 1);
    u8g2_DrawPixel(&u8g2, TILE_ARROW_S + 2, tile.arrow_y + 2);
    u8g2_DrawPixel(&u8g2, TILE_ARROW_S + 1, tile.arrow_y - 1);
    u8g2_DrawPixel(&u8g2, TILE_ARROW_S + 2, tile.arrow_y - 2);
    /**画左箭头**/

    /**画右箭头**/
    u8g2_DrawHLine(&u8g2, DISP_W - TILE_ARROW_W - TILE_ARROW_S, tile.arrow_y, TILE_ARROW_W);
    u8g2_DrawPixel(&u8g2, DISP_W - TILE_ARROW_W, tile.arrow_y + 1);
    u8g2_DrawPixel(&u8g2, DISP_W - TILE_ARROW_W - 1, tile.arrow_y + 2);
    u8g2_DrawPixel(&u8g2, DISP_W - TILE_ARROW_W, tile.arrow_y - 1);
    u8g2_DrawPixel(&u8g2, DISP_W - TILE_ARROW_W - 1, tile.arrow_y - 2);
    /**画右箭头**/

    /**画按钮**/
    u8g2_DrawHLine(&u8g2, TILE_BTN_S, tile.arrow_y + 2, 9);
    u8g2_DrawBox(&u8g2, TILE_BTN_S + 2, tile.arrow_y + 2 - 4, 5, 4);

    u8g2_DrawHLine(&u8g2, 128 - TILE_BTN_S - 9, tile.arrow_y + 2, 9);
    u8g2_DrawBox(&u8g2, 128 - TILE_BTN_S - 9 + 2, tile.arrow_y + 2 - 4, 5, 4);
    /**画按钮**/

    //虚线从下向上缓动
    for (int index = 0; index <= 128; index += 1)
    {
      if (index % 8 == 0 | (index - 1) % 8 == 0 | (index - 2) % 8 == 0) continue;
      u8g2_DrawPixel(&u8g2, index, (int16_t) tile.line_y);  // 绘制一条由像素点组成的虚线
    }

    //图标从左上到右下展开
    for (uint8_t i = 0; i < ui.num[ui.index]; ++i)
    {
      if (ui.param[TILE_UFD])
      {
        tile.temp = (DISP_W - TILE_ICON_W) / 2 + i * tile.icon_x - TILE_ICON_S * ui.select[ui.layer];
      } else tile.temp = (DISP_W - TILE_ICON_W) / 2 + (i - ui.select[ui.layer]) * tile.icon_x;
      u8g2_DrawXBMP(&u8g2, tile.temp, (int16_t) tile.icon_y, TILE_ICON_W, TILE_ICON_H, icon_pic[i]);
    }
    if (tile.icon_x == tile.icon_x_trg)
    {
      ui.init = true;
      tile.icon_x = tile.icon_x_trg = -ui.select[ui.layer] * TILE_ICON_S;
    }
  } else
  {
    /**画左箭头**/
    u8g2_DrawHLine(&u8g2, TILE_ARROW_S, TILE_ARROW_L, TILE_ARROW_W);
    u8g2_DrawPixel(&u8g2, TILE_ARROW_S + 1, TILE_ARROW_L + 1);
    u8g2_DrawPixel(&u8g2, TILE_ARROW_S + 2, TILE_ARROW_L + 2);
    u8g2_DrawPixel(&u8g2, TILE_ARROW_S + 1, TILE_ARROW_L - 1);
    u8g2_DrawPixel(&u8g2, TILE_ARROW_S + 2, TILE_ARROW_L - 2);
    /**画左箭头**/

    /**画右箭头**/
    u8g2_DrawHLine(&u8g2, DISP_W - TILE_ARROW_W - TILE_ARROW_S, TILE_ARROW_L, TILE_ARROW_W);
    u8g2_DrawPixel(&u8g2, DISP_W - TILE_ARROW_W, TILE_ARROW_L + 1);
    u8g2_DrawPixel(&u8g2, DISP_W - TILE_ARROW_W - 1, TILE_ARROW_L + 2);
    u8g2_DrawPixel(&u8g2, DISP_W - TILE_ARROW_W, TILE_ARROW_L - 1);
    u8g2_DrawPixel(&u8g2, DISP_W - TILE_ARROW_W - 1, TILE_ARROW_L - 2);
    /**画右箭头**/

    /**画按钮**/
    u8g2_DrawHLine(&u8g2, TILE_BTN_S, TILE_ARROW_L + 2, 9);
    u8g2_DrawBox(&u8g2, TILE_BTN_S + 2, TILE_ARROW_L + 2 - 4, 5, 4);

    u8g2_DrawHLine(&u8g2, 128 - TILE_BTN_S - 9, TILE_ARROW_L + 2, 9);
    u8g2_DrawBox(&u8g2, 128 - TILE_BTN_S - 9 + 2, TILE_ARROW_L + 2 - 4, 5, 4);
    /**画按钮**/

    for (int index = 0; index <= 128; index += 1)
    {
      if (index % 8 == 0 | (index - 1) % 8 == 0 | (index - 2) % 8 == 0) continue;
      u8g2_DrawPixel(&u8g2, index, TILE_LINE_S);  // 绘制一条由像素点组成的虚线
    }

    for (uint8_t i = 0; i < ui.num[ui.index]; ++i)
    {
      u8g2_DrawXBMP(&u8g2, (DISP_W - TILE_ICON_W) / 2 + (int16_t) tile.icon_x + i * TILE_ICON_S, TILE_ICON_L,
                    TILE_ICON_W,
                    TILE_ICON_H,
                    icon_pic[i]);
    }
  }

  //反转屏幕内元素颜色，白天模式遮罩
  u8g2_SetDrawColor(&u8g2, 2);
  if (!ui.param[DARK_MODE]) u8g2_DrawBox(&u8g2, 0, 0, DISP_W, DISP_H);
}

/*************** 根据列表每行开头符号，判断每行尾部是否绘制以及绘制什么内容 *************/

//列表显示数值
void list_draw_value(int n)
{
  auto str = std::to_string(check_box.v[n]); //因为值和字符已经一一对应 所以无需-1
  //std::string str = std::to_string(check_box.v[n - 1]);
  auto *c_str = str.c_str();
  u8g2_DrawStr(&u8g2, CHECK_BOX_L_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, c_str);
}

//绘制外框
void list_draw_check_box_frame()
{ u8g2_DrawRFrame(&u8g2, CHECK_BOX_L_S, list.temp + CHECK_BOX_U_S, CHECK_BOX_F_W, CHECK_BOX_F_H, 1); }

//绘制框里面的点
void list_draw_check_box_dot()
{
  u8g2_DrawBox(&u8g2, CHECK_BOX_L_S + CHECK_BOX_D_S + 1, list.temp + CHECK_BOX_U_S + CHECK_BOX_D_S + 1,
               CHECK_BOX_F_W - (CHECK_BOX_D_S + 1) * 2, CHECK_BOX_F_H - (CHECK_BOX_D_S + 1) * 2);
}

//列表显示旋钮功能
void list_draw_krf(int n)
{
  //switch (check_box.v[n - 1]) //因为值和字符已经一一对应 所以无需-1
  switch (check_box.v[n])
  {
    case 0:
      u8g2_DrawUTF8(&u8g2, CHECK_BOX_L_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, "OFF");
      break;
    case 1:
      u8g2_DrawUTF8(&u8g2, CHECK_BOX_L_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, "VOL");
      break;
    case 2:
      u8g2_DrawUTF8(&u8g2, CHECK_BOX_L_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, "BRI");
      break;
    default:
      u8g2_DrawUTF8(&u8g2, CHECK_BOX_L_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, "..");
      break;
  }
}

//列表显示 单选框
void list_draw_kpf(int n)
{
  //因为值和字符已经一一对应 所以无需-1
  if (check_box.v[n] == 0) u8g2_DrawUTF8(&u8g2, CHECK_BOX_L_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, "OFF");
  else if (check_box.v[n] <= 90)
  {
    auto str = std::to_string((char) check_box.v[n]);
    auto *c_str = str.c_str();
    u8g2_DrawUTF8(&u8g2, CHECK_BOX_L_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, c_str);
  } else u8g2_DrawUTF8(&u8g2, CHECK_BOX_L_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, "?");
}

//判断列表尾部内容
void list_draw_text_and_check_box(struct MENU arr[], int i)
{
  u8g2_DrawUTF8(&u8g2, LIST_TEXT_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, arr[i].m_select);
  //u8g2_SetCursor(&u8g2,CHECK_BOX_L_S, list.temp + LIST_TEXT_H + LIST_TEXT_S);
  switch (arr[i].m_select[0])
  {
    case '~':
      list_draw_value(i);
      break;
    case '+':
      list_draw_check_box_frame();
      //因为值和字符已经一一对应 所以无需-1
      if (check_box.m[i] == 1) list_draw_check_box_dot();
      //if (check_box.m[i - 1] == 1) list_draw_check_box_dot();
      break;
    case '=':
      list_draw_check_box_frame();
      if (*check_box.s_p == i) list_draw_check_box_dot();
      break;
    case '#':
      list_draw_krf(i);
      break;
    case '$':
      list_draw_kpf(i);
      break;
  }
}

/******************************** 列表显示函数 **************************************/

//列表类页面通用显示函数
void list_show(struct MENU arr[], uint8_t ui_index)
{
  //更新动画目标值
  u8g2_SetFont(&u8g2, LIST_FONT);
  list.box_x_trg = u8g2_GetUTF8Width(&u8g2, arr[ui.select[ui.layer]].m_select) + LIST_TEXT_S * 2;
  list.bar_y_trg = ceil((ui.select[ui.layer]) * ((float) DISP_H / (ui.num[ui_index] - 1)));

  //计算动画过渡值
  animation(&list.y, &list.y_trg, LIST_ANI);
  animation(&list.box_x, &list.box_x_trg, LIST_ANI);
  animation(&list.box_y, &list.box_y_trg[ui.layer], LIST_ANI);
  animation(&list.bar_y, &list.bar_y_trg, LIST_ANI);

  //检查循环动画是否结束
  if (list.loop && list.box_y == list.box_y_trg[ui.layer]) list.loop = false;

  //设置文字和进度条颜色，0透显，1实显，2反色，这里都用实显
  u8g2_SetDrawColor(&u8g2, 1);

  //绘制进度条
  u8g2_DrawHLine(&u8g2, DISP_W - LIST_BAR_W, 0, LIST_BAR_W);
  u8g2_DrawHLine(&u8g2, DISP_W - LIST_BAR_W, DISP_H - 1, LIST_BAR_W);
  u8g2_DrawVLine(&u8g2, DISP_W - ceil((float) LIST_BAR_W / 2), 0, DISP_H);
  u8g2_DrawBox(&u8g2, DISP_W - LIST_BAR_W, 0, LIST_BAR_W, list.bar_y);

  //绘制列表文字
  if (!ui.init)
  {
    for (int i = 0; i < ui.num[ui_index]; ++i)
    {
      if (ui.param[LIST_UFD]) list.temp = i * list.y - LIST_LINE_H * ui.select[ui.layer] + list.box_y_trg[ui.layer];
      else list.temp = (i - ui.select[ui.layer]) * list.y + list.box_y_trg[ui.layer];
      list_draw_text_and_check_box(arr, i);
    }
    if (list.y == list.y_trg)
    {
      ui.init = true;
      list.y = list.y_trg = -LIST_LINE_H * ui.select[ui.layer] + list.box_y_trg[ui.layer];
    }
  } else
    for (int i = 0; i < ui.num[ui_index]; ++i)
    {
      list.temp = LIST_LINE_H * i + list.y;
      list_draw_text_and_check_box(arr, i);
    }

  //绘制文字选择框，0透显，1实显，2反色，这里用反色
  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawRBox(&u8g2, 0, list.box_y, list.box_x, LIST_LINE_H, LIST_BOX_R);

  //反转屏幕内元素颜色，白天模式遮罩，在这里屏蔽有列表参与的页面，使遮罩作用在那个页面上
  if (!ui.param[DARK_MODE])
  {
    u8g2_DrawBox(&u8g2, 0, 0, DISP_W, DISP_H);
    switch (ui.index)
    {
      case M_WINDOW:
      case M_VOLT:
        u8g2_DrawBox(&u8g2, 0, 0, DISP_W, DISP_H);
    }
  }
}

//弹窗通用显示函数
void window_show()
{
  //绘制背景列表，根据开关判断背景是否要虚化
  list_show(win.bg, win.index);
  if (ui.param[WIN_BOK]) for (uint16_t i = 0; i < buf_len; ++i) buf_ptr[i] = buf_ptr[i] & (i % 2 == 0 ? 0x55 : 0xAA);

  //更新动画目标值
  u8g2_SetFont(&u8g2, WIN_FONT);
  win.bar_trg = (float) (*win.value - win.min) / (float) (win.max - win.min) * (WIN_BAR_W - 4);

  //计算动画过渡值
  animation(&win.bar, &win.bar_trg, WIN_ANI);
  animation(&win.y, &win.y_trg, WIN_ANI);

  //绘制窗口
  u8g2_SetDrawColor(&u8g2, 0);
  u8g2_DrawRBox(&u8g2, win.l, (int16_t) win.y, WIN_W, WIN_H, 2);    //绘制外框背景
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_DrawRFrame(&u8g2, win.l, (int16_t) win.y, WIN_W, WIN_H, 2);  //绘制外框描边
  u8g2_DrawRFrame(&u8g2, win.l + 5, (int16_t) win.y + 20, WIN_BAR_W, WIN_BAR_H, 1);       //绘制进度条外框
  u8g2_DrawBox(&u8g2, win.l + 7, (int16_t) win.y + 22, win.bar, WIN_BAR_H - 4);           //绘制进度条

  u8g2_DrawUTF8(&u8g2, win.l + 5, (int16_t) win.y + 14, win.title);   //绘制标题

  auto str = std::to_string(*win.value);
  auto *c_str = str.c_str();
  u8g2_DrawUTF8(&u8g2, win.l + 78, (int16_t) win.y + 14, c_str);   //绘制当前值

  //需要在窗口修改参数时立即见效的函数
  if (!strcmp(win.title, "屏幕亮度")) u8g2_SetContrast(&u8g2, ui.param[DISP_BRI]);

  //反转屏幕内元素颜色，白天模式遮罩
  u8g2_SetDrawColor(&u8g2, 2);
  if (!ui.param[DARK_MODE]) u8g2_DrawBox(&u8g2, 0, 0, DISP_W, DISP_H);
}

/************************************* 处理函数 *************************************/

/*********************************** 通用处理函数 ***********************************/

//磁贴类页面旋转时判断通用函数
void tile_rotate_switch()
{
  switch (g_KeyValue)
  {
    case KEY_1_CLICK:
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
      if (ui.init)
      {
        if (ui.select[ui.layer] > 0)
        {
          ui.select[ui.layer] -= 1;
          tile.icon_x_trg += TILE_ICON_S;
          tile.select_flag = false;
        } else
        {
          if (ui.param[TILE_LOOP])
          {
            ui.select[ui.layer] = ui.num[ui.index] - 1;
            tile.icon_x_trg = -TILE_ICON_S * (ui.num[ui.index] - 1);
            break;
          } else tile.select_flag = true;
        }
      }
      break;

    case KEY_0_CLICK:
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
      if (ui.init)
      {
        if (ui.select[ui.layer] < (ui.num[ui.index] - 1))
        {
          ui.select[ui.layer] += 1;
          tile.icon_x_trg -= TILE_ICON_S;
          tile.select_flag = false;
        } else
        {
          if (ui.param[TILE_LOOP])
          {
            ui.select[ui.layer] = 0;
            tile.icon_x_trg = 0;
            break;
          } else tile.select_flag = true;
        }
      }
      break;
  }
}

//列表类页面旋转时判断通用函数
void list_rotate_switch()
{
  if (!list.loop)
  {
    switch (g_KeyValue)
    {
      case KEY_1_CLICK:
        if (ui.select[ui.layer] == 0)
        {
          if (ui.param[LIST_LOOP] && ui.init)
          {
            list.loop = true;
            ui.select[ui.layer] = ui.num[ui.index] - 1;
            if (ui.num[ui.index] > list.line_n)
            {
              list.box_y_trg[ui.layer] = DISP_H - LIST_LINE_H;
              list.y_trg = DISP_H - ui.num[ui.index] * LIST_LINE_H;
            } else list.box_y_trg[ui.layer] = (ui.num[ui.index] - 1) * LIST_LINE_H;
            break;
          } else break;
        }
        if (ui.init)
        {
          ui.select[ui.layer] -= 1;
          if (ui.select[ui.layer] < -(list.y_trg / LIST_LINE_H))
          {
            if (!(DISP_H % LIST_LINE_H)) list.y_trg += LIST_LINE_H;
            else
            {
              if (list.box_y_trg[ui.layer] == DISP_H - LIST_LINE_H * list.line_n)
              {
                list.y_trg += (list.line_n + 1) * LIST_LINE_H - DISP_H;
                list.box_y_trg[ui.layer] = 0;
              } else if (list.box_y_trg[ui.layer] == LIST_LINE_H)
              {
                list.box_y_trg[ui.layer] = 0;
              } else list.y_trg += LIST_LINE_H;
            }
          } else list.box_y_trg[ui.layer] -= LIST_LINE_H;
          break;
        }

      case KEY_0_CLICK:
        if (ui.select[ui.layer] == (ui.num[ui.index] - 1))
        {
          if (ui.param[LIST_LOOP] && ui.init)
          {
            list.loop = true;
            ui.select[ui.layer] = 0;
            list.y_trg = 0;
            list.box_y_trg[ui.layer] = 0;
            break;
          } else break;
        }
        if (ui.init)
        {
          ui.select[ui.layer] += 1;
          if ((ui.select[ui.layer] + 1) > (list.line_n - list.y_trg / LIST_LINE_H))
          {
            if (!(DISP_H % LIST_LINE_H)) list.y_trg -= LIST_LINE_H;
            else
            {
              if (list.box_y_trg[ui.layer] == LIST_LINE_H * (list.line_n - 1))
              {
                list.y_trg -= (list.line_n + 1) * LIST_LINE_H - DISP_H;
                list.box_y_trg[ui.layer] = DISP_H - LIST_LINE_H;
              } else if (list.box_y_trg[ui.layer] == DISP_H - LIST_LINE_H * 2)
              {
                list.box_y_trg[ui.layer] = DISP_H - LIST_LINE_H;
              } else list.y_trg -= LIST_LINE_H;
            }
          } else list.box_y_trg[ui.layer] += LIST_LINE_H;
          break;
        }
        break;
    }
  }
}

//弹窗通用处理函数
void window_proc()
{
  window_show();
  if (win.y == WIN_Y_TRG) ui.index = win.index;
  if (g_KeyActionFlag == KEY_PRESSED && win.y == win.y_trg && win.y != WIN_Y_TRG)
  {
    g_KeyActionFlag = KEY_NOT_PRESSED;
    switch (g_KeyValue)
    {
      case KEY_0_CLICK:
        if (*win.value < win.max) *win.value += win.step;
        break;
      case KEY_1_CLICK:
        if (*win.value > win.min) *win.value -= win.step;
        break;
      case KEY_0_PRESS:
      case KEY_1_PPESS:
        win.y_trg = WIN_Y_TRG;
        break;
    }
  }
}

/********************************** 分页面处理函数 **********************************/

//睡眠页面处理函数
void sleep_proc()
{
  while (ui.sleep)
  {
    //睡眠时循环执行的函数

    //睡眠时需要扫描旋钮才能退出睡眠
    Key_Scan();

    //当旋钮有动作时
    if (g_KeyActionFlag == KEY_PRESSED)
    {
      g_KeyActionFlag = KEY_NOT_PRESSED;

      //u8g2_SetPowerSave(&u8g2, 0);

      ui.sleep = false;
      ui.index = M_MAIN;
      ui.state = S_LAYER_IN;

      break;
    }
  }
}

//主菜单处理函数，磁贴类模板
void main_proc()
{
  tile_show(main_menu, main_icon_pic);
  if (g_KeyActionFlag == KEY_PRESSED)
  {
    g_KeyActionFlag = KEY_NOT_PRESSED;
    switch (g_KeyValue)
    {
      case KEY_0_CLICK:
      case KEY_1_CLICK:
        tile_rotate_switch();
        break;
      case KEY_1_PPESS:
        switch (ui.select[ui.layer])
        {
          case 0:
            ui.index = M_SLEEP;
            ui.state = S_LAYER_OUT;
            break;
          case 1:
            ui.index = M_CAM;
            ui.state = S_LAYER_IN;
            break;
          case 2:
            ui.index = M_CAMSETTING;
            ui.state = S_LAYER_IN;
            break;
          case 3:
            ui.index = M_SETTING;
            ui.state = S_LAYER_IN;
            break;
        }
    }
    if (!tile.select_flag && ui.init)
    {
      tile.title_y = tile.title_y_calc;
    }
  }
}

//拍摄设置函数
void cam_setting_proc()
{
  list_show(cam_setting_menu, M_CAMSETTING);
  if (g_KeyActionFlag == KEY_PRESSED)
  {
    g_KeyActionFlag = KEY_NOT_PRESSED;
    switch (g_KeyValue)
    {
      case KEY_0_CLICK:
      case KEY_1_CLICK:
        list_rotate_switch();
        break;
      case KEY_0_PRESS:
        ui.select[ui.layer] = 0;
      case KEY_1_PPESS:
        switch (ui.select[ui.layer])
        {
          case 0:
            ui.index = M_MAIN;
            ui.state = S_LAYER_OUT;
            break;
          case 11:
            //ui.index = M_KNOB;
            ui.state = S_LAYER_IN;
            break;
        }
    }
  }
}

//电压测量页处理函数
void volt_proc()
{

}

//设置菜单处理函数，多选框列表类模板，弹窗模板
void setting_proc()
{
  ui.param[DARK_MODE] = setting_menu.value[3];
  //ui.param[BUZ_MODE] = setting_menu.value[2];  //按键音还未写好

  list_show(setting_menu.menu, M_SETTING);

  if (g_KeyActionFlag == KEY_PRESSED)
  {
    g_KeyActionFlag = KEY_NOT_PRESSED;
    switch (g_KeyValue)
    {
      case KEY_0_CLICK:
      case KEY_1_CLICK:
        list_rotate_switch();
        break;
      case KEY_0_PRESS:
        ui.select[ui.layer] = 0;
      case KEY_1_PPESS:
        switch (ui.select[ui.layer])
        {
          //返回更浅层级，长按被当作选择这一项，也是执行这一行
          case 0:
            ui.index = M_MAIN;
            ui.state = S_LAYER_OUT;
            break;
            //弹出窗口，参数初始化：标题，参数名，参数值，最大值，最小值，步长，背景列表名，背景列表标签
          case 1:
            //todo: 看一下设置界面的弹窗和复选框初始化函数 处理一下 新的这些变量要初始化 旧的不用的那些初始化要裁剪掉
            //window_value_init("屏幕亮度", DISP_BRI, &ui.param[DISP_BRI], 255, 0, 15, setting_menu.menu, M_SETTING);
            window_value_init("屏幕亮度", DISP_BRI, &setting_menu.value[1], 255, 0, 15, setting_menu.menu, M_SETTING);
            break;
          case 2:
            //按键音
            //check_box_m_select(DARK_MODE);
            check_box_m_select(3);
            break;
          case 3:
            //暗黑模式
            //check_box_m_select(DARK_MODE);
            check_box_m_select(3);
            break;
          case 4:
            //测试界面
            ui.index = M_ABOUT;
            ui.state = S_LAYER_IN;
            break;
          case 5:
            //关于本机
            ui.index = M_ABOUT;
            ui.state = S_LAYER_IN;
            break;
        }
    }
  }
}

//关于本机页
void about_proc()
{
  list_show(about_menu, M_ABOUT);
  if (g_KeyActionFlag == KEY_PRESSED)
  {
    g_KeyActionFlag = KEY_NOT_PRESSED;
    switch (g_KeyValue)
    {
      case KEY_0_CLICK:
      case KEY_1_CLICK:
        list_rotate_switch();
        break;
      case KEY_0_PRESS:
        ui.select[ui.layer] = 0;
      case KEY_1_PPESS:
        switch (ui.select[ui.layer])
        {
          case 0:
            ui.index = M_SETTING;
            ui.state = S_LAYER_OUT;
            break;
        }
    }
  }
}

//总的UI进程
void ui_proc()
{
  u8g2_SendBuffer(&u8g2);
  switch (ui.state)
  {
    case S_FADE:
      fade();
      break;  //转场动画
    case S_WINDOW:
      window_param_init();
      break;  //弹窗初始化
    case S_LAYER_IN:
      layer_init_in();
      break;  //层级初始化
    case S_LAYER_OUT:
      layer_init_out();
      break;  //层级初始化

    case S_NONE:
      u8g2_ClearBuffer(&u8g2);
      switch (ui.index)      //直接选择页面
      {
        case M_WINDOW:
          window_proc();

          //todo: 所有的参数在弹窗中修改时 显示的参数也要进行同步（不在弹窗中的不需要写在这里）
          ui.param[DISP_BRI] = setting_menu.value[1];
          break;
        case M_SLEEP:
          sleep_proc();
          break;
        case M_MAIN:
          main_proc();
          break;
        case M_CAMSETTING:
          cam_setting_proc();
          break;
//        case M_KNOB:
//          knob_proc();
//          break;
//        case M_KRF:
//          krf_proc();
//          break;
//        case M_KPF:
//          kpf_proc();
//          break;
        case M_VOLT:
          volt_proc();
          break;
        case M_SETTING:
          setting_proc();
          break;
        case M_ABOUT:
          about_proc();
          break;
      }
  }
}

//OLED初始化函数
void oled_init()
{
  u8g2_SetContrast(&u8g2, ui.param[DISP_BRI]);
  buf_ptr = u8g2_GetBufferPtr(&u8g2);
  buf_len = 8 * u8g2_GetBufferTileHeight(&u8g2) * u8g2_GetBufferTileWidth(&u8g2);
}