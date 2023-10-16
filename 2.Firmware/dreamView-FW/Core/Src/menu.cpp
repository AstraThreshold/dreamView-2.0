//
// Created by Fir on 2023/4/18.
//
#include "menu.h"
#include "oled_setup_u8g2_fir.h"
#include "oled_key_fir.h"
#include "oled_menu_show_fir.h"

double kPID[3] = {0.45, 0.10, 0.00};

uint8_t motionIsOn = 1;

menu::menu(newSelectList _list[], uint8_t _listNum)
{
  listNum = _listNum;
  selectList = new newSelectList[listNum];
  for (int i = 0; i < listNum; i++) selectList[i] = _list[i];

  uiSelect = 0;

  lengthSingleLine = 63 / listNum;
  lengthTotalLine = lengthSingleLine * listNum + 1;

  x = 4;
  y = yTrg = 0;
  yLine = yLineTrg = 1;

  yBox = yBoxTrg = 0;   //0在第一个 步进+16

  widthBox = widthBoxTrg = u8g2_GetUTF8Width(&u8g2, selectList[uiSelect].select) + x * 2;

  yBoxErr = 0;
  yBoxSumErr = yBoxDelErr = yBoxPreErr = 0;
  yBoxSumOut = 0;

  yBarErr = 0;
  yBarOut = 0;

  yScreenErr = 0;

  yFontErr = 0;

  std::fill(yFont, yFont+20, 12);
  std::fill(yFontTrg, yFontTrg+20, 12);

  widthErr = 0;
  widthOut = 0;
}

void menu::moveBox(int16_t *_a, const int16_t *_aTrg)
{
  yBoxErr = *_aTrg - *_a;    //得到当前误差
  yBoxSumErr += yBoxErr;   //累加历史误差
  yBoxDelErr = yBoxErr - yBoxPreErr;    //计算近两次误差之差
  yBoxSumOut = Kpid[0] * yBoxErr + Kpid[1] * yBoxSumErr + Kpid[2] * yBoxDelErr;
  if (yBoxSumOut >= MAX_OUT)
  {
    yBoxSumOut = MAX_OUT;
  }
  *_a += yBoxSumOut;    //输出
  yBoxPreErr = yBoxErr;    //pre_err_y在err_y更新前存储当前err_y
}

void menu::moveBar(int16_t *_a, const int16_t *_aTrg)
{
  yBarErr = *_aTrg - *_a;
  if (yBarErr <= 1 && yBarErr >= -1)
  {
    *_a = *_aTrg;
    return;
  } else
  {
    if ((0.25 * yBarErr) > MAX_OUT)
    {
      yBarOut = MAX_OUT;
    } else if ((0.25 * yBarErr) > 0 && (0.25 * yBarErr) <= 1)
    {
      yBarOut = 1;
    } else if ((0.25 * yBarErr) < 0 && (0.25 * yBarErr) >= -1)
    {
      yBarOut = -1;
    } else
    {
      yBarOut = 0.25 * yBarErr;
    }
    *_a += yBarOut;
  }
}

void menu::moveScreen(int16_t *_a, const int16_t *_aTrg)
{
  yScreenErr = *_aTrg - *_a;    //得到当前误差
  if (yScreenErr <= 1 && yScreenErr >= -1)
  {
    *_a = *_aTrg;
    return;
  } else
  {
    if (*_a < *_aTrg)
    {
      if (0.4 * yScreenErr >= MAX_OUT_SCREEN)
      {
        *_a += MAX_OUT_SCREEN;
      } else
      {
        *_a += 0.4 * yScreenErr;
      }
      if (*_a > *_aTrg) *_a = *_aTrg;     //加完超过
    } else if (*_a > *_aTrg)
    {
      if (0.4 * yScreenErr >= MAX_OUT_SCREEN)
      {
        *_a += MAX_OUT_SCREEN;
      } else
      {
        *_a += 0.4 * yScreenErr;
      }
      if (*_a < *_aTrg) *_a = *_aTrg;     //减完不足
    }
  }
}

void menu::moveFont(int16_t *_a, const int16_t *_aTrg)
{
  yFontErr = *_aTrg - *_a;
  if (yFontErr <= 1 && yFontErr >= -1)
  {
    *_a = *_aTrg;
    return;
  } else
  {
    if (yFontErr * 0.2 >= 20) { *_a += 20; return; }
    else if (yFontErr * 0.2 <= -20) { *_a -= 20; return; }
    else if (yFontErr * 0.2 > 0 && yFontErr * 0.2 <= 1) *_a += 1;
    else if (yFontErr * 0.2 < 0 && yFontErr * 0.2 >= -1) *_a -= 1;
    else *_a += yFontErr * 0.2;
    return;
  }
}

void menu::moveWidth(uint8_t *_a, const uint8_t *_aTrg)
{
  widthErr = *_aTrg - *_a;
  if (widthErr <= 1 && widthErr >= -1)
  {
    *_a = *_aTrg;
    return;
  } else
  {
    if ((0.25 * widthErr) > MAX_OUT_WIDTH)   //因为宽度变化较大 所以增加pid限位
    {
      widthOut = MAX_OUT_WIDTH;
    } else if((0.25 * widthErr) > 0 && (0.25 * widthErr) <= 1)
    {
      widthOut = 1;
    } else if((0.25 * widthErr) < 0 && (0.25 * widthErr) >= -1)
    {
      widthOut = -1;
    } else
    {
      widthOut = 0.25 * widthErr;   //Kp = 0.60
    }
    *_a += widthOut;
  }
}

void menu::show(uint8_t _motionIsOn)
{
  for (uint8_t i = 0; i < list_num; ++i)
  {
    u8g2_DrawUTF8(&u8g2, x, yFont[i], list[i].select);
  }

  if (!_motionIsOn)
  {
    yLine = yLineTrg;
    y = yTrg;
    yBox = yBoxTrg;
    widthBox = widthBoxTrg;
    //yFont = yFontTrg;
  } else
  {
    moveBar(&yLine, &yLineTrg);
    moveScreen(&y, &yTrg);
    moveBox(&yBox, &yBoxTrg);
    for (int i = 0; i < listNum; i++)
    {
      moveFont(&yFont[i], &yFontTrg[i]);
    }
    moveWidth(&widthBox, &widthBoxTrg);
  }
  u8g2_DrawVLine(&u8g2, 126, 0, lengthTotalLine);
  u8g2_DrawPixel(&u8g2, 125, 0);
  u8g2_DrawPixel(&u8g2, 127, 0);

  for (uint8_t i = 0; i < listNum; ++i)
  {
    //u8g2_DrawUTF8(&u8g2, x, 16 * i + y + 12, list[i].select);
    u8g2_DrawPixel(&u8g2, 125, lengthSingleLine * (i + 1));
    u8g2_DrawPixel(&u8g2, 127, lengthSingleLine * (i + 1));
  }
  u8g2_DrawVLine(&u8g2, 125, yLine, lengthSingleLine);
  u8g2_DrawVLine(&u8g2, 127, yLine, lengthSingleLine);

  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawRBox(&u8g2, 0, yBox, widthBox, 16, 1);
  u8g2_SetDrawColor(&u8g2, 1);
}

menuSwitch::menuSwitch()
{
  switchIsOn = 1;
  xSwitch = xSwitchTrg = 106;

  ySwitchErr = ySwitchSumErr = ySwitchSumOut = 0;
}

uint8_t menuSwitch::move(uint16_t *_a, const uint16_t *_aTrg)
{
  ySwitchErr = *_aTrg - *_a;    //得到当前误差
  ySwitchSumErr += ySwitchErr;   //累加历史误差
  ySwitchSumOut = 0.45 * ySwitchErr + 0.10 * ySwitchSumErr;
  if (ySwitchSumOut >= MAX_OUT)
  {
    ySwitchSumOut = MAX_OUT;
  }

  *_a += ySwitchSumOut;

  if (ySwitchSumOut == 0) return 1;
}

void menuSwitch::show(uint8_t _pos, menu _menu, uint8_t _switchIsOn)
{
  u8g2_DrawRBox(&u8g2, 106, 16 * _pos + _menu.y + 3, 16, 10, 3);
  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawBox(&u8g2, 107, 16 * _pos + _menu.y + 5, 14, 6);
  u8g2_DrawBox(&u8g2, 109, 16 * _pos + _menu.y + 4, 10, 1);   //去掉上面的多余部分
  u8g2_DrawBox(&u8g2, 109, 16 * _pos + _menu.y + 11, 10, 1);    //去掉下面的多余部分
  u8g2_SetDrawColor(&u8g2, 1);

  if (!_switchIsOn) move(&xSwitch, &xSwitchTrg);
  else xSwitch = xSwitchTrg;
  u8g2_DrawRBox(&u8g2, xSwitch, 16 * _pos + _menu.y + 5, 6, 6, 1);   //绘制圆点
}

newSelectList mainMenuList[7] =
{
  {"<-返回主界面"},     //返回主界面
  {"-设置"},
  {"-"},
  {"-"},
  {"-胶片型号编辑"},   //胶片型号编辑
  {"-"},     //最近的日志信息 仿命令行设计
  {"-"},
};

//menu mainMenu(mainMenuList, 7);

//此部分需要改变值 所以需要传引用
//void mainMenuProc(menu *_mainMenu)
//{
//  if (key_msg.pressed)
//  {
//    key_msg.pressed = 0;
//    switch (key_msg.id)
//    {
//      case 0:
//        if (_mainMenu->uiSelect < 1) //如果在第一个还继续向上
//        {
//          _mainMenu->uiSelect = _mainMenu->listNum - 1;   //直接把select赋给最后一个
//          _mainMenu->yBoxTrg = 48;
//          _mainMenu->yLineTrg = _mainMenu->lengthTotalLine - _mainMenu->lengthSingleLine;    //右侧进度条赋为最后一个
//          _mainMenu->yTrg = -48;   //屏幕坐标赋为最后一个
//          break;
//        }
//        _mainMenu->uiSelect -= 1;
//        _mainMenu->yLineTrg -= _mainMenu->lengthSingleLine;   //目标值后移一个格
//        //当前选择方块在第1个位置时
//        if (_mainMenu->yBoxTrg <= 0) _mainMenu->yTrg += 16;   //屏幕向上移动
//        else _mainMenu->yBoxTrg -= 16;                       //选择方块向上移动
//        break;
//      case 1:
//        if (_mainMenu->uiSelect > _mainMenu->listNum - 2) //如果在最后一个还继续向下
//        {
//          _mainMenu->uiSelect = 0;   //直接把select赋给第一个
//          _mainMenu->yBoxTrg = 0;    //选择方块坐标赋为第一个
//          _mainMenu->yLineTrg = 1;   //右侧进度条赋为第一个
//          _mainMenu->yTrg = 0;       //屏幕坐标赋为最后一个
//          break;
//        }
//        _mainMenu->uiSelect += 1;
//        _mainMenu->yLineTrg += _mainMenu->lengthSingleLine;   //目标值前进一个格
//        if (_mainMenu->yBoxTrg >= 48)  _mainMenu->yTrg -= 16;
//        else _mainMenu->yBoxTrg += 16;
//        break;
//      case 2:
//      case 3:
//        switch (_mainMenu->uiSelect)
//        {
//          case 0:     //回到主界面
//            //TODO: 此部分待重构
//             = S_DISAPPEAR;
//            ui_index = M_CAMERA;
//            break;
//          case 1:     //设置界面
//            ui_state = S_DISAPPEAR;
//            ui_index = M_SETTING;
//            break;
//          case 2:   //icon
//            ui_state = S_DISAPPEAR;
//            ui_index = M_ICON;
//            break;
//          case 3:   //chart
//            ui_state = S_DISAPPEAR;
//            ui_index = M_CHART;
//            break;
//          case 4:   //textedit
//            ui_state = S_DISAPPEAR;
//            ui_index = M_TEXT_EDIT;
//            break;
//          case 6:   //about
//            ui_state = S_DISAPPEAR;
//            ui_index = M_ABOUT;
//            break;
//          default:
//            break;
//        }
//      default:
//        break;
//    }
//    _mainMenu->widthBoxTrg = u8g2_GetUTF8Width(&u8g2, _mainMenu->selectList[_mainMenu->uiSelect].select) + _mainMenu->x * 2;
//  }
//  _mainMenu->show(motionIsOn);
//}

ui::ui()
{

}

uint8_t isFontPosInit = 0;

void ui::mainMenuProc(menu *_mainMenu)
{
  //初始化字符位置以及展开动画 只进行一次
  if (isFontPosInit == 0)
  {
    if (_mainMenu->uiSelect > 3)  std::fill(_mainMenu->yFont, _mainMenu->yFont+20, 60);
    else std::fill(_mainMenu->yFont, _mainMenu->yFont+20, 16 * (_mainMenu->uiSelect) + y + 12);

    isFontPosInit = 1;
  }

  for (uint8_t i = 0; i < _mainMenu->listNum; i++)
  {
    _mainMenu->yFontTrg[i] = 16 * i + _mainMenu->y + 12;
  }

  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (_mainMenu->uiSelect < 1) //如果在第一个还继续向上
        {
          _mainMenu->uiSelect = _mainMenu->listNum - 1;   //直接把select赋给最后一个
          _mainMenu->yBoxTrg = 48;
          _mainMenu->yLineTrg = _mainMenu->lengthTotalLine - _mainMenu->lengthSingleLine;    //右侧进度条赋为最后一个
          _mainMenu->yTrg = -48;   //屏幕坐标赋为最后一个
          break;
        }
        _mainMenu->uiSelect -= 1;
        _mainMenu->yLineTrg -= _mainMenu->lengthSingleLine;   //目标值后移一个格
        //当前选择方块在第1个位置时
        if (_mainMenu->yBoxTrg <= 0) _mainMenu->yTrg += 16;   //屏幕向上移动
        else _mainMenu->yBoxTrg -= 16;                       //选择方块向上移动
        break;
      case 1:
        if (_mainMenu->uiSelect > _mainMenu->listNum - 2) //如果在最后一个还继续向下
        {
          _mainMenu->uiSelect = 0;   //直接把select赋给第一个
          _mainMenu->yBoxTrg = 0;    //选择方块坐标赋为第一个
          _mainMenu->yLineTrg = 1;   //右侧进度条赋为第一个
          _mainMenu->yTrg = 0;       //屏幕坐标赋为最后一个
          break;
        }
        _mainMenu->uiSelect += 1;
        _mainMenu->yLineTrg += _mainMenu->lengthSingleLine;   //目标值前进一个格
        if (_mainMenu->yBoxTrg >= 48)  _mainMenu->yTrg -= 16;
        else _mainMenu->yBoxTrg += 16;
        break;
      case 2:
      case 3:
        switch (_mainMenu->uiSelect)
        {
          case 0:     //回到主界面
            uiState = S_DISAPPEAR;
            uiIndex = M_CAMERA;
            break;
          case 1:     //设置界面
            uiState = S_DISAPPEAR;
            uiIndex = M_SETTING;
            break;
//          case 2:   //icon
//            ui_state = S_DISAPPEAR;
//            ui_index = M_ICON;
//            break;
//          case 3:   //chart
//            ui_state = S_DISAPPEAR;
//            ui_index = M_CHART;
//            break;
          case 4:   //textedit
            uiState = S_DISAPPEAR;
            uiIndex = M_TEXT_EDIT;
            break;
          case 6:   //about
            uiState = S_DISAPPEAR;
            uiIndex = M_ABOUT;
            break;
          default:
            break;
        }
      default:
        break;
    }
    _mainMenu->widthBoxTrg = u8g2_GetUTF8Width(&u8g2, _mainMenu->selectList[_mainMenu->uiSelect].select) + _mainMenu->x * 2;
  }
  fps_show();   //显示fps 函数内部集成了开关状态变量
  _mainMenu->show(motionIsOn);
}

uint8_t ui::uiSelect(menu *_menu)
{
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (_menu->uiSelect < 1) //如果在第一个还继续向上
        {
          _menu->uiSelect = _menu->listNum - 1;   //直接把select赋给最后一个
          _menu->yBoxTrg = 48;
          _menu->yLineTrg = _menu->lengthTotalLine - _menu->lengthSingleLine;    //右侧进度条赋为最后一个
          _menu->yTrg = -48;   //屏幕坐标赋为最后一个
          break;
        }
        _menu->uiSelect -= 1;
        _menu->yLineTrg -= _menu->lengthSingleLine;   //目标值后移一个格
        //当前选择方块在第1个位置时
        if (_menu->yBoxTrg <= 0) _menu->yTrg += 16;   //屏幕向上移动
        else _menu->yBoxTrg -= 16;                       //选择方块向上移动
        break;
      case 1:
        if (_menu->uiSelect > _menu->listNum - 2) //如果在最后一个还继续向下
        {
          _menu->uiSelect = 0;   //直接把select赋给第一个
          _menu->yBoxTrg = 0;    //选择方块坐标赋为第一个
          _menu->yLineTrg = 1;   //右侧进度条赋为第一个
          _menu->yTrg = 0;       //屏幕坐标赋为最后一个
          break;
        }
        _menu->uiSelect += 1;
        _menu->yLineTrg += _menu->lengthSingleLine;   //目标值前进一个格
        if (_menu->yBoxTrg >= 48) _menu->yTrg -= 16;
        else _menu->yBoxTrg += 16;
        break;
      case 2:
      case 3:
        return _menu->uiSelect;
    }
  }
}
