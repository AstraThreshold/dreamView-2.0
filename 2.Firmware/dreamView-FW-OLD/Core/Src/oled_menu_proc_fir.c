//
// Created by Fir on 2023/2/8.
//
#include "oled_menu_proc_fir.h"
#include "oled_menu_show_fir.h"
#include "oled_setup_u8g2_fir.h"
#include "stm32f1xx_it.h"
#include "shutter_timer_fir.h"

void logo_proc()
{
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    ui_state = S_DISAPPEAR;
    ui_index = M_SELECT;
  }
  logo_ui_show();
}

//TODO:第二个重构这个 在proc里处理switch
void setting_proc()
{
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (setting_select < 1)
        {
          //如果在第一个还继续向上
          setting_select = sizeof(setting) / sizeof(SELECT_LIST) - 1;   //直接把select赋给最后一个
          setting_box_y_trg = 48;    //选择方块坐标赋为最后一个
          line_y_trg_setting = total_line_length_setting - single_line_length_setting;    //右侧进度条赋为最后一个
          y_trg_setting = -64 - 16;   //屏幕坐标赋为最后一个
          break;
        }
        setting_select -= 1;
        line_y_trg_setting -= single_line_length_setting;
        if (setting_box_y_trg == 0)    //当前选择方块在第1个位置时
        {
          y_trg_setting += 16;    //屏幕向上移动
        } else
        {
          setting_box_y_trg -= 16;    //选择方块向上移动
        }
        break;
      case 1:
        if ((setting_select + 2) > (sizeof(setting) / sizeof(SELECT_LIST)))
        {
          //如果最后一个还继续向下
          setting_select = 0;   //直接把select赋给第一个
          setting_box_y_trg = 0;    //选择方块坐标赋为第一个
          line_y_trg_setting = 1;    //右侧进度条赋为第一个
          y_trg_setting = 0;   //屏幕坐标赋为最后一个
          break;
        }
        setting_select += 1;
        line_y_trg_setting += single_line_length_setting;   //目标值前进一个格
        if (setting_box_y_trg == 48)    //当前选择方块在最后一个位置时
        {
          y_trg_setting -= 16;    //屏幕向下移动
        } else
        {
          setting_box_y_trg += 16;    //选择方块向上移动
        }
        break;
      case 2:
      case 3:
        switch (setting_select)
        {
          case 0:     //回到菜单界面
            ui_state = S_DISAPPEAR;
            ui_index = M_SELECT;
            break;
          case 1:   //快门释放设置
            ui_state = S_DISAPPEAR;
            ui_index = M_SHUTTER_SETTING;
            break;
          case 2:     //电池信息
            ui_state = S_DISAPPEAR;
            ui_index = M_BATTERY;
            break;
          case 3:   //动画效果
            ui_state = S_DISAPPEAR;
            ui_index = M_PID;
            break;
          case 4:   //快门当前状态
            ui_state = S_DISAPPEAR;
            ui_index = M_SOUND;
            break;
          case 5:   //测光表数据
            ui_state = S_DISAPPEAR;
            ui_index = M_LIGHT;
            break;
          case 6:   //日志信息
            ui_state = S_DISAPPEAR;
            ui_index = M_LOG;
            break;
          case 7:   //开发人员选项
            ui_state = S_DISAPPEAR;
            ui_index = M_DEVELOPER_SETTING;
            break;
          case 8:   //关于本机
            ui_state = S_DISAPPEAR;
            ui_index = M_ABOUT;
            break;
          default:
            break;
        }
      default:
        break;
    }
    setting_box_width_trg = u8g2_GetUTF8Width(&u8g2, setting[setting_select].select) + x_setting * 2;
  }
  setting_ui_show();
}

void shutter_setting_proc()
{
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (shutter_select < 1)
        {
          //如果在第一个还继续向上
          shutter_select = sizeof(shutter_setting) / sizeof(SELECT_LIST) - 1;   //直接把select赋给最后一个
          shutter_box_y_trg = 48;    //选择方块坐标赋为最后一个
          line_y_trg_shutter = total_line_length_shutter - single_line_length_shutter;    //右侧进度条赋为最后一个
          y_trg_shutter = -16;   //屏幕坐标赋为最后一个
          break;
        }
        shutter_select -= 1;
        line_y_trg_shutter -= single_line_length_shutter;
        if (shutter_box_y_trg == 0)    //当前选择方块在第1个位置时
        {
          y_trg_shutter += 16;    //屏幕向上移动
        } else
        {
          shutter_box_y_trg -= 16;    //选择方块向上移动
        }
        break;
      case 1:
        if ((shutter_select + 2) > (sizeof(shutter_setting) / sizeof(SELECT_LIST)))
        {
          //如果最后一个还继续向下
          shutter_select = 0;   //直接把select赋给第一个
          shutter_box_y_trg = 0;    //选择方块坐标赋为第一个
          line_y_trg_shutter = 1;    //右侧进度条赋为第一个
          y_trg_shutter = 0;   //屏幕坐标赋为最后一个
          break;
        }
        shutter_select += 1;
        line_y_trg_shutter += single_line_length_shutter;   //目标值前进一个格
        if (shutter_box_y_trg == 48)    //当前选择方块在最后一个位置时
        {
          y_trg_shutter -= 16;    //屏幕向下移动
        } else
        {
          shutter_box_y_trg += 16;    //选择方块向上移动
        }
        break;
      case 2:
      case 3:
        switch (shutter_select)
        {
          case 0:     //回到设置界面
            ui_state = S_DISAPPEAR;
            ui_index = M_SETTING;
            break;
          case 1:   //无视曝光释放快门
            shut_nolight_switch_need_moving = 1;
            shut_nonext_switch_need_moving = 0;
            shut_nolight_is_on += 1;
            if (shut_nolight_is_on > 1) shut_nolight_is_on = 0;
            if (shut_nolight_is_on == 1) shut_nolight_switch_pos_trg = 114; //112
            if (shut_nolight_is_on == 0) shut_nolight_switch_pos_trg = 108; //106
            break;
          case 2:     //未过片允许释放快门
            shut_nonext_switch_need_moving = 1;
            shut_nolight_switch_need_moving = 0;
            shut_nonext_is_on += 1;
            if (shut_nonext_is_on > 1) shut_nonext_is_on = 0;
            if (shut_nonext_is_on == 1) shut_nonext_switch_pos_trg = 114; //112
            if (shut_nonext_is_on == 0) shut_nonext_switch_pos_trg = 108; //106
            break;
          case 3:   //长曝光快门触发方式
            ui_state = S_DISAPPEAR;
            ui_index = M_PID;
            break;
          case 4:   //长曝光时屏幕行为
            ui_state = S_DISAPPEAR;
            //ui_index = M_SHUTTER;
            break;
          default:
            break;
        }
      default:
        break;
    }
    shutter_box_width_trg = u8g2_GetUTF8Width(&u8g2, shutter_setting[shutter_select].select) + x_shutter * 2;
  }
  shutter_setting_show();
}

void camera_proc()
{
  camera_ui_show();
//  if (!HAL_GPIO_ReadPin(SHUTTERKEY_GPIO_Port, SHUTTERKEY_Pin))
//  {
//  }
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (shutter_is_B) break;    //如果在慢门还减慢 就滚蛋
        if (shutter_speed > 50)
        {
          shutter_speed -= 50;    //减慢快门速度（快门大于50的时候 栅格为50)
          break;
        }
        if (shutter_speed <= 50 && shutter_speed >= -50)
        {
          shutter_speed -= 25;    //快门小于等于50的时候 栅格为25
          if (shutter_speed <= 0 && shutter_speed > -50)
          {
            shutter_is_slow = 1;   //如果25的时候还选择减慢 则开启慢门
            break;
          }
          if (shutter_speed == -50)
          {
            shutter_is_B = 1;    //快门速度-50对应B门
            break;
          }
          break;
        }
      case 1:
        if (shutter_is_B == 1)
        {
          shutter_is_B = 0;   //若处在B门 解除B门
          shutter_is_slow = 1;
          shutter_speed = -25;
          break;
        }
        if (shutter_speed == 0)
        {
          //若处在慢门1
          shutter_is_slow = 0;
          shutter_speed = 25;
          break;
        }
        if (shutter_speed == -25)
        {
          //若处在慢门2
          shutter_speed = 0;
          break;
          //慢门有两档 分别是一秒和两秒 对应快门速度0和-25 -50是B门
        }
        if (shutter_speed < 50 && shutter_speed >= 0)
        {
          shutter_speed += 25;
          break;
        }
        if (shutter_speed >= 50)
        {
          shutter_speed += 50;
          break;
        }
      case 2:
        ui_state = S_DISAPPEAR;
        ui_index = M_SELECT;
        break;
      case 3:
        //如果按下快门键
        //快门触发部分
        //注意 快门上电时默认是打开电磁铁的（两个pin被快门小板上拉）
        if (shutter_is_ok != 1 && shutter_is_working == 0)
        {
          //快门等待上弦
          ui_index = M_CAMERA_NOT_OK;
        }
        if (shutter_is_ok == 1 && shutter_is_working == 0)   //如果快门已经上弦且空闲
        {
          shutter_is_working = 1;

          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);

          if (shutter_speed > 0)
          {
            while (shutter_box_is_ok == 0)
            {
              camera_ui_show();
              OLED_Refresh();
            }//直到pid绘制完成
            HAL_Delay((uint32_t) (1000 / shutter_speed));
            //快门速度 强制类型转换 舍去小数部分
            //注意快门速度要取倒数 同时delay的单位是毫秒
          } else if (shutter_speed <= 0 && shutter_speed > -50)
          {
            if (shutter_speed == 0)
            {
              for (int i = 0; i < 35; ++i)
              {
                //相当于延时1s
                key_scan();
                if (key_msg.pressed)
                {
                  key_msg.pressed = 0;
                  shutter_busy_state++;
                  if (shutter_busy_state > 1) shutter_busy_state = 0;
                  if (shutter_busy_state == 1)
                  {
                    ui_index = M_CAMERA_BUSY;
                  } else
                  {
                    ui_index = M_CAMERA;
                  }
                }
                ui_proc();
                OLED_Refresh();
              }
            }
            if (shutter_speed == -25)
            {
              for (int i = 0; i < 70; ++i)
              {
                //TODO: 推送git后修改此处的延时处理 改成系统滴答定时器 类比蜂鸣器
                //相当于延时2s
                key_scan();
                if (key_msg.pressed)
                {
                  key_msg.pressed = 0;
                  shutter_busy_state++;
                  if (shutter_busy_state > 1) shutter_busy_state = 0;
                  if (shutter_busy_state == 1)
                  {
                    ui_index = M_CAMERA_BUSY;
                  } else
                  {
                    ui_index = M_CAMERA;
                  }
                }
                ui_proc();
                OLED_Refresh();
              }
            }
          } else if (shutter_speed == -50)
          {
            //如果为B门
            while (1)
            {
              camera_ui_show();
              OLED_Refresh();
              if (HAL_GPIO_ReadPin(SHUTTERKEY_GPIO_Port, SHUTTERKEY_Pin) == 1) break;
            }   //按键松开之前一直打开快门
          }
          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
          HAL_Delay(20);   //留出快门做动作的时间 避免快门还没动作就重新给电磁铁上�?????
          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
          //给电磁铁上电 等待上弦
          shutter_is_ok = 0;    //快门动作结束 快门需要重新上弦
          shutter_is_working = 0;   //线程阻塞结束 恢复快门状态为空闲
          ui_index = M_CAMERA;
          //todo: 此处记得将快门计数-1 而且当胶片用尽时 不允许释放快门 写到释放快门的条件里
        }
        break;
      default:
        break;
    }
  }
}

void camera_not_ok_proc()
{
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
//    switch (key_msg.id)
//    {
//      case 0:
//        ui_index = M_CAMERA;
//        break;
//      case 1:
//        ui_index = M_CAMERA;
//        break;
//      case 2:
//        ui_state = S_DISAPPEAR;
//        ui_index = M_SELECT;
//        break;
//      case 3:
//        ui_index = M_CAMERA;
//        break;
//      default:
//        break;
//    }
    ui_index = M_CAMERA;
  }
  camera_ui_show();
  for (uint16_t i = 0; i < buf_len; ++i)
  {
    buf_ptr[i] = buf_ptr[i] & (i % 2 == 0 ? 0x55 : 0xAA);
  }
  camera_not_ok_show();
}

void camera_busy_proc()
{
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 2:
        ui_state = S_DISAPPEAR;
        ui_index = M_SELECT;
        break;
      case 3:
        ui_index = M_CAMERA;
        break;
      default:
        break;
    }
  }
  camera_ui_show();
  for (uint16_t i = 0; i < buf_len; ++i)
  {
    buf_ptr[i] = buf_ptr[i] & (i % 2 == 0 ? 0x55 : 0xAA);
  }
  camera_busy_show();
}

void pid_edit_proc(void)
{
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (Kpid[pid_select - 2] > 0) Kpid[pid_select - 2] -= 0.01;
        break;
      case 1:
        if (Kpid[pid_select - 2] < PID_MAX) Kpid[pid_select - 2] += 0.01;
        break;
      case 2:
      case 3:
        ui_index = M_PID;
        break;
      default:
        break;
    }
  }
  pid_ui_show();
  for (uint16_t i = 0; i < buf_len; ++i)
  {
    buf_ptr[i] = buf_ptr[i] & (i % 2 == 0 ? 0x55 : 0xAA);
  }
  pid_edit_ui_show();
}

//todo: 不给用户那么多选择空间 只允许选择三种动画模式 做成像长曝光快门触发方式那样的单选框
void pid_proc()
{
  pid_ui_show();
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (pid_select < 1) break;
        pid_select -= 1;
        pid_line_y_trg -= single_line_length_pid;
        if (pid_box_y_trg == 0)
        {
          y_trg_pid += 16;
        } else
        {
          pid_box_y_trg -= 16;
        }
        break;
      case 1:
        if ((pid_select + 2) > sizeof(pid) / sizeof(SELECT_LIST)) break;
        pid_select += 1;
        pid_line_y_trg += single_line_length_pid;
        if (pid_box_y_trg == 48)
        {
          y_trg_pid -= 16;
        } else
        {
          pid_box_y_trg += 16;
        }
        break;
      case 2:
      case 3:
        switch (pid_select)
        {
          case 0:   //返回
            ui_state = S_DISAPPEAR;
            ui_index = M_SETTING;
            pid_select = 0;
            pid_line_y = pid_line_y_trg = 1;
            pid_box_y = pid_box_y_trg = 0;
            pid_box_width = pid_box_width_trg = u8g2_GetUTF8Width(&u8g2, pid[pid_select].select) + x * 2;
            break;
          case 1:   //动画总开关
            motion_is_on +=1;
            if (motion_is_on > 1) motion_is_on = 0;
            if (motion_is_on == 1) motion_switch_pos_trg = 114; //112
            if (motion_is_on == 0) motion_switch_pos_trg = 108; //106
            break;
          case 2:   //kp
          case 3:   //ki
          case 4:   //kd
            ui_index = M_PID_EDIT;
            break;
          default:
            break;
        }

//        if (pid_select == 3)
//        {
//          ui_index = M_SETTING;   //如果按下返回 就返回到设置界面
//          ui_state = S_DISAPPEAR;
//          pid_select = 0;
//          pid_line_y = pid_line_y_trg = 1;
//          pid_box_y = pid_box_y_trg = 0;
//          pid_box_width = pid_box_width_trg = u8g2_GetUTF8Width(&u8g2, pid[pid_select].select) + x * 2;
//        } else
//        {
//          ui_index = M_PID_EDIT;
//        }
//        break;
//      default:
//        break;
    }
    pid_box_width_trg = u8g2_GetUTF8Width(&u8g2, pid[pid_select].select) + x * 2;
  }
}

//TODO: 首先开始重构这部分
void select_proc(void)
{
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (ui_select < 1)
        {
          //如果在第一个还继续向上
          ui_select = sizeof(list) / sizeof(SELECT_LIST) - 1;   //直接把select赋给最后一个
          box_y_trg = 48;    //选择方块坐标赋为最后一个
          line_y_trg = total_line_length - single_line_length;    //右侧进度条赋为最后一个
          y_trg = -48 ;   //屏幕坐标赋为最后一个
          break;
        }
        ui_select -= 1;
        line_y_trg -= single_line_length;   //目标值后移一个格
        if (box_y_trg == 0)    //当前选择方块在第1个位置时
        {
          y_trg += 16;    //屏幕向上移动
        } else
        {
          box_y_trg -= 16;    //选择方块向上移动
        }
        break;
      case 1:
        if ((ui_select + 2) > (sizeof(list) / sizeof(SELECT_LIST)))
        {
          //如果最后一个还继续向下
          ui_select = 0;   //直接把select赋给第一个
          box_y_trg = 0;    //选择方块坐标赋为第一个
          line_y_trg = 1;    //右侧进度条赋为第一个
          y_trg = 0;   //屏幕坐标赋为最后一个
          break;
        }
        ui_select += 1;
        line_y_trg += single_line_length;   //目标值前进一个格
        //if ((ui_select + 1) > (4 - y / 16))    //原版
        if (box_y_trg == 48)    //当前选择方块在最后一个位置时
        {
          y_trg -= 16;    //屏幕向下移动
        } else
        {
          box_y_trg += 16;    //选择方块向上移动
        }
        break;
      case 2:
      case 3:
        switch (ui_select)
        {
          case 0:     //回到主界面
            ui_state = S_DISAPPEAR;
            ui_index = M_CAMERA;
            break;
          case 1:     //设置界面
            ui_state = S_DISAPPEAR;
            ui_index = M_SETTING;
            break;
          case 2:   //icon
            ui_state = S_DISAPPEAR;
            ui_index = M_ICON;
            break;
          case 3:   //chart
            ui_state = S_DISAPPEAR;
            ui_index = M_CHART;
            break;
          case 4:   //textedit
            ui_state = S_DISAPPEAR;
            ui_index = M_TEXT_EDIT;
            break;
          case 6:   //about
            ui_state = S_DISAPPEAR;
            ui_index = M_ABOUT;
            break;
          default:
            break;
        }
      default:
        break;
    }
    box_width_trg = u8g2_GetUTF8Width(&u8g2, list[ui_select].select) + x * 2;
  }
  select_ui_show();
}

void icon_proc(void)
{
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    ui_state = S_DISAPPEAR;
    ui_index = M_SELECT;
  }
}

void text_edit_proc()
{
  text_edit_ui_show();
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (edit_flag)
        {
          text_edit(0, edit_index);
        } else
        {
          if (edit_index == 0)
          {
            edit_index = name_len;
          } else
          {
            edit_index -= 1;
          }
        }
        break;
      case 1:
        if (edit_flag)
        {
          text_edit(1, edit_index);
        } else
        {
          if (edit_index == name_len)
          {
            edit_index = 0;
          } else
          {
            edit_index += 1;
          }
        }
        break;
      case 2:
      case 3:
        if (edit_index == name_len)
        {
          ui_state = S_DISAPPEAR;
          ui_index = M_SELECT;
          edit_index = 0;
        } else
        {
          edit_flag = !edit_flag;
        }
        break;
      default:
        break;
    }
  }
}

void about_proc()
{
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    ui_state = S_DISAPPEAR;
    ui_index = M_SETTING;
  }
  about_ui_show();
}

void developer_setting_proc()
{
  developer_setting_show();
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (developer_select < 1)
        {
          //如果在第一个还继续向上
          developer_select = sizeof(developer_setting) / sizeof(SELECT_LIST) - 1;   //直接把select赋给最后一个
          developer_box_y_trg = 32;    //选择方块坐标赋为最后一个
          line_y_trg_developer = total_line_length_developer - single_line_length_developer;    //右侧进度条赋为最后一个
          break;
        }
        developer_select -= 1;
        line_y_trg_developer -= single_line_length_developer;
        if (developer_box_y_trg == 0)
        {
          y_trg_developer += 16;
        } else
        {
          developer_box_y_trg -= 16;
        }
        break;
      case 1:
        if ((developer_select + 2) > (sizeof(developer_setting) / sizeof(SELECT_LIST)))
        {
          //如果最后一个还继续向下
          developer_select = 0;   //直接把select赋给第一个
          developer_box_y_trg = 0;    //选择方块坐标赋为第一个
          line_y_trg_developer = 1;    //右侧进度条赋为第一个
          break;
        }
        developer_select += 1;
        line_y_trg_developer += single_line_length_developer;
        if (developer_box_y_trg == 48)
        {
          y_trg_developer -= 16;
        } else
        {
          developer_box_y_trg += 16;
        }
        break;
      case 2:
      case 3:
        switch (developer_select)
        {
          case 0:   //返回
            ui_state = S_DISAPPEAR;
            ui_index = M_SETTING;   //返回到设置界面
            break;
          case 1:   //帧率显示
            fps_is_on += 1;
            if (fps_is_on > 1) fps_is_on = 0;
            if (fps_is_on == 1) fps_switch_pos_trg = 114; //112
            if (fps_is_on == 0) fps_switch_pos_trg = 108; //106
            break;
          case 2:   //物理引脚链接图表
            ui_state = S_DISAPPEAR;
            //todo: 物理引脚show
            break;
          default:
            break;
        }
      default:
        break;
    }
    developer_box_width_trg = u8g2_GetUTF8Width(&u8g2, developer_setting[developer_select].select)  + x_developer * 2;
  }
}

void sound_setting_proc()
{
  sound_setting_show();
  if (key_msg.pressed)
  {
    key_msg.pressed = 0;
    switch (key_msg.id)
    {
      case 0:
        if (sound_select < 1)
        {
          //如果在第一个还继续向上
          sound_select = sizeof(sound_setting) / sizeof(SELECT_LIST) - 1;   //直接把select赋给最后一个
          sound_box_y_trg = 32;    //选择方块坐标赋为最后一个
          line_y_trg_sound = total_line_length_sound - single_line_length_sound;    //右侧进度条赋为最后一个
          break;
        }
        sound_select -= 1;
        line_y_trg_sound -= single_line_length_sound;
        if (sound_box_y_trg == 0)
        {
          y_trg_sound += 16;
        } else
        {
          sound_box_y_trg -= 16;
        }
        break;
      case 1:
        if ((sound_select + 2) > (sizeof(sound_setting) / sizeof(SELECT_LIST)))
        {
          //如果最后一个还继续向下
          sound_select = 0;   //直接把select赋给第一个
          sound_box_y_trg = 0;    //选择方块坐标赋为第一个
          line_y_trg_sound = 1;    //右侧进度条赋为第一个
          break;
        }
        sound_select += 1;
        line_y_trg_sound += single_line_length_sound;
        if (sound_box_y_trg == 48)
        {
          y_trg_sound -= 16;
        } else
        {
          sound_box_y_trg += 16;
        }
        break;
      case 2:
      case 3:
        switch (sound_select)
        {
          case 0:   //返回
            ui_state = S_DISAPPEAR;
            ui_index = M_SETTING;   //返回到设置界面
            break;
          case 1:   //按键音开关
            sound_is_on += 1;
            if (sound_is_on > 1) sound_is_on = 0;
            if (sound_is_on == 1) sound_switch_pos_trg = 114; //112
            if (sound_is_on == 0) sound_switch_pos_trg = 108; //106
            break;
          case 2:   //按键音频率
            ui_state = S_DISAPPEAR;
            //todo: 按键音频率
            break;
          default:
            break;
        }
      default:
        break;
    }
    sound_box_width_trg = u8g2_GetUTF8Width(&u8g2, sound_setting[sound_select].select)  + x_sound * 2;
  }
}

/**/
void ui_proc()
{
  switch (ui_state)
  {
    case S_NONE:
      if (ui_index != M_CAMERA) OLED_ClearBuffer();
      //if (ui_index != M_CAMERA && ui_index != M_CAMERA_BUSY) OLED_ClearBuffer();
      //相机主界面涉及到B门快门等的刷新 避免频闪 所以不刷新
      switch (ui_index)
      {
        case M_LOGO:
          logo_proc();
          break;
        case M_CAMERA:
          camera_proc();
          break;
        case M_CAMERA_NOT_OK:
          camera_not_ok_proc();
          break;
        case M_CAMERA_BUSY:
          camera_busy_proc();
          break;
        case M_SETTING:
          setting_proc();
          break;
        case M_SHUTTER_SETTING:
          shutter_setting_proc();
          break;
        case M_SELECT:
          select_proc();
          break;
        case M_PID:
          pid_proc();
          break;
        case M_ICON:
          icon_proc();
          break;
        case M_TEXT_EDIT:
          text_edit_proc();
          break;
        case M_PID_EDIT:
          pid_edit_proc();
          break;
        case M_ABOUT:
          about_proc();
          break;
        case M_DEVELOPER_SETTING:
          developer_setting_proc();
          break;
        case M_SOUND:
          sound_setting_proc();
          break;
        default:
          break;
      }
      break;
    case S_DISAPPEAR:
      disappear();
      break;
    default:
      break;
  }
  //++FPS_Count;  //oled_setup_u8g2_fir.c内集成了fps++
  fps_show();   //显示fps 函数内部集成了开关状态变量
  u8g2_SendBuffer(&u8g2);
}