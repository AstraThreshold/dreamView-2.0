//
// Created by Fir on 2023/2/24.
//
#include "oled_encoder_fir.h"
#include "oled_key_fir.h"

static int8_t Encoder_A_Last = 0;                        //Encoder的A引脚上一次的状态
static int8_t Encoder_B_Last = 0;                        //Encoder的B引脚上一次的状态
static int8_t Encoder_Type = 1;                          //定义变量暂存Encoder的类型 0: 一定位对应一脉冲  1: 两定位对应一脉冲

int32_t Encoder_KEY_COUNT = 0;                    //Encoder按键动作计数器
int32_t Encoder_KEY_DoubleClick_Count = 0;        //Encoder按键双击动作计数器
int8_t FLAG_Encoder_KEY_ShotClick = 0;            //Encoder按键短按动作标志
int8_t FLAG_Encoder_KEY_LongClick = 0;            //Encoder按键长按动作标志
int8_t FLAG_Encoder_KEY_DoubleClick = 0;          //Encoder按键双击动作标志

void Encoder_Init(uint8_t Set_Encoder_TYPE)
{
  //Encoder类型选择：0-一定位一脉冲；1-两定位一脉冲
  if (Set_Encoder_TYPE == 0)
  {
    Encoder_Type = 0;
  } else
  {
    Encoder_Type = 1;
  }

  //避免上电时Encoder旋钮位置不确定导致一次动作误判
  Encoder_A_Last = Encoder_A_Now;
  Encoder_A_Last = Encoder_A_Now;
  Encoder_B_Last = Encoder_B_Now;

  //--------清除按键计数器和标志位--------//
  Encoder_KEY_COUNT = 0;                     //Encoder按键动作计数器
  Encoder_KEY_DoubleClick_Count = 0;         //Encoder按键双击动作计数器
  FLAG_Encoder_KEY_ShotClick = 0;            //Encoder按键短按动作标志
  FLAG_Encoder_KEY_LongClick = 0;            //Encoder按键长按动作标志
  FLAG_Encoder_KEY_DoubleClick = 0;          //Encoder按键双击动作标志
}

int8_t Encoder_Scan()
{
  //以下储存A、B上一次值的变量声明为静态全局变量，方便对Encoder对应的IO口做初始化
  static int8_t Encoder_A_Last = 0;
  static int8_t Encoder_B_Last = 0;
  int8_t ScanResult = 0;    //返回编码器扫描结果，用于分析编码器的动作
  //返回值的取值：   0：无动作；      1：正转；           -1：反转；
  //                  2：只按下按键；    3：按着按键正转；   -3：按着按键反转

  //======================================================//
  if (Encoder_Type == 0)      //================一定位对应一脉冲的Encoder================//
  {
    //======================================================//
    if (Encoder_A_Now != Encoder_A_Last)   //以A为时钟，B为数据。正转时AB反相，反转时AB同相
    {
      if (Encoder_A_Now == 0)
      {
        if (Encoder_B_Now == 1)      //只需要采集A的上升沿或下降沿的任意一个状态，若A下降沿时B为1，正转
        {
          ScanResult = 1;     //正转
          key_msg.id = 1;
          key_msg.pressed = 1;
        } else                    //反转
        {
          ScanResult = 4;
          key_msg.id = 0;
          key_msg.pressed = 1;
        }
      }
      Encoder_A_Last = Encoder_A_Now;   //更新编码器上一个状态暂存变量
      Encoder_B_Last = Encoder_B_Now;   //更新编码器上一个状态暂存变量
    }
  }
    //======================================================//
  else                    //================两定位对应一脉冲的Encoder================//
  {
    //======================================================//
    if (Encoder_A_Now != Encoder_A_Last)        //当A发生跳变时采集B当前的状态，并将B与上一次的状态进行对比。
    {
      //若A 0->1 时，B 1->0 正转；若A 1->0 时，B 0->1 正转；
      //若A 0->1 时，B 0->1 反转；若A 1->0 时，B 1->0 反转
      if (Encoder_A_Now == 1)     //Encoder_A和上一次状态相比，为上升沿
      {
        if ((Encoder_B_Last == 1) && (Encoder_B_Now == 0))   //Encoder_B和上一次状态相比，为下降沿
          ScanResult = 1;                         //正转

        if ((Encoder_B_Last == 0) && (Encoder_B_Now == 1))   //Encoder_B和上一次状态相比，为上升沿
          ScanResult = 4;                        //反转

        //>>>>>>>>>>>>>>>>下面为正转一次再反转或反转一次再正转处理<<<<<<<<<<<<<<<<//
        if ((Encoder_B_Last == Encoder_B_Now) && (Encoder_B_Now == 0))  //A上升沿时，采集的B不变且为0
          ScanResult = 1;                                 //正转

        if ((Encoder_B_Last == Encoder_B_Now) && (Encoder_B_Now == 1))  //A上升沿时，采集的B不变且为1
          ScanResult = 4;                                //反转
      } else                    //Encoder_A和上一次状态相比，为下降沿
      {
        if ((Encoder_B_Last == 1) && (Encoder_B_Now == 0))   //Encoder_B和上一次状态相比，为下降沿
          ScanResult = 4;                        //反转

        if ((Encoder_B_Last == 0) && (Encoder_B_Now == 1))   //Encoder_B和上一次状态相比，为上升沿
          ScanResult = 1;                         //正转

        //>>>>>>>>>>>>>>>>下面为正转一次再反转或反转一次再正转处理<<<<<<<<<<<<<<<<//
        if ((Encoder_B_Last == Encoder_B_Now) && (Encoder_B_Now == 0))  //A上升沿时，采集的B不变且为0
          ScanResult = 4;                                //反转

        if ((Encoder_B_Last == Encoder_B_Now) && (Encoder_B_Now == 1))  //A上升沿时，采集的B不变且为1
          ScanResult = 1;                                 //正转

      }
      Encoder_A_Last = Encoder_A_Now;   //更新编码器上一个状态暂存变量
      Encoder_B_Last = Encoder_B_Now;   //更新编码器上一个状态暂存变量
    }
  }

  if (Encoder_Key == 0)   //如果Encoder的按键按下，并且没有Encoder没有转动，
  {
    //if (Encoder_Key == 0)
    //{
//      if (ScanResult == 0)         //按下按键时未转动
    ScanResult = 2;         //返回值为2
//      else
//      {
//          if (ScanResult == 1)     //按下按键时候正转
//              ScanResult = 3;     //返回值为3
//          if (ScanResult == -1)    //按下按键时候反转
//              ScanResult = -3;    //返回值为-3
//      }
    //}
  }

  return ScanResult;      //返回值的取值：   0：无动作；      1：正转；           -1：反转；
}                           //              2：只按下按键；    3：按着按键正转；   -3：按着按键反转    经修改反转为 4

int8_t Encoder_Analyze(int8_t Encoder_Value)
{
  int8_t AnalyzeResult = 0;
//  static unsigned int TMP_Value = 0;  //中间计数值，用于连续长按按键的动作延时间隔
  //>>>>>>>>>>>>>>>>编码器正转处理程序<<<<<<<<<<<<<<<<//
  if (Encoder_Value == 1) //正转
  {
    //--------编码器正转动作代码--------//
    key_msg.id = 1;
    key_msg.pressed = 1;
  }

  //      switch (Encoder_NUM_SW)
  //      {
  //      case 1: G_PWM_NUM1 += 10; if (G_PWM_NUM1>255)G_PWM_NUM1 = 0;   break;
  //      case 2: G_PWM_NUM2 += 10; if (G_PWM_NUM1>255)G_PWM_NUM2 = 0;   break;
  //      case 3: G_PWM_NUM3 += 10; if (G_PWM_NUM1>255)G_PWM_NUM3 = 0;   break;
  //      case 4: G_PWM_NUM1 += 10;     if (G_PWM_NUM1>255)G_PWM_NUM1 = 0;   G_PWM_NUM3 = G_PWM_NUM2 = G_PWM_NUM1;   break;
  //      case 5: G_PWM_NUM1 += 20; if (G_PWM_NUM1 >= 255) { G_PWM_NUM1 = 0; G_PWM_NUM3 += 20; if (G_PWM_NUM3 >= 255) { G_PWM_NUM3 = 0; G_PWM_NUM2 += 20; if (G_PWM_NUM2 >= 255)G_PWM_NUM2 = 0; } }
  //      default:break;
  //      }

  //>>>>>>>>>>>>>>>>编码器反转处理程序<<<<<<<<<<<<<<<<//
  if (Encoder_Value == 4)    //反转
  {
    //--------编码器反转动作代码--------//
    key_msg.id = 0;
    key_msg.pressed = 1;

  }

  //>>>>>>>>>>>>>>>>编码器按键按下并正转处理程序<<<<<<<<<<<<<<<<//
  //              if (Encoder_Value == 3)
  //              {
  //                  //--------编码器按键按下并正转动作代码--------//


  //  }

  //>>>>>>>>>>>>>>>>编码器按键按下并反转处理程序<<<<<<<<<<<<<<<<//
  //          if (Encoder_Value == -3)
  //          {
  //              //--------编码器按键按下并反转动作代码--------//


  //          }
  //>>>>>>>>>>>>>>>>编码器按键按下处理程序<<<<<<<<<<<<<<<<//
  if (Encoder_Value == 2)     //====检测到按键按下====//
  {
//    sum = Menu_Page * 10 + Level;                        //计算二级菜单所对应的一级菜单位置
//    switch (sum)
//    {
//      case 10:
//      {
//        /***************************************
//        在此添加选项的功能，以显示姓名作为示例
//        ***************************************/
//        //二级菜单
//        break;
//      }
//      case 11:                                         //   数值根据sum进行修改
//      {
//        break;
//      }
//      case 12:
//      {
//        break;
//      }
//      case 13:
//      {
//        break;
//      }
//      case 20:
//      {
//        break;
//      }
//      case 22:
//      {
//        break;
//      }
//      case 24:
//      {
//        break;
//      }
//      case 26:
//      {
//        break;
//      }
//      case 30:
//      {
//        break;
//      }
//      case 32:
//      {
//        break;
//      }
//      case 34:
//      {
//        break;
//      }
//      case 36:
//      {
//        break;
//      }
          if (Encoder_KEY_COUNT<10000)    //打开按键按下时间定时器
              Encoder_KEY_COUNT++;
          if (Encoder_KEY_COUNT == KEY_COUNT_DESHAKING)   //按下按键时间到达消抖时间时
          {                                           //置位短按按键标志
              FLAG_Encoder_KEY_ShotClick = 1;
          }

          if ((Encoder_KEY_DoubleClick_Count > 0) && (Encoder_KEY_DoubleClick_Count <= KEY_COUNT_DUALCLICKTIME))   //松开按键后，又在定时器在双击时间内按下按键
          {                                                                                               //置位双击按键标志
              FLAG_Encoder_KEY_DoubleClick = 1;
          }

          if (Encoder_KEY_COUNT == KEY_COUNT_LONGTIME)    //按下按键时间到达长按时间
          {                                           //置位长按按键标志并复位短按按键标志
              FLAG_Encoder_KEY_LongClick = 1;
              FLAG_Encoder_KEY_ShotClick = 0;
          }
//      default:
//        break;
//    }
  }
//    if (QX_Key == 0)//QX_Key未接按键
//    {
//        Menu_Display(Menu_Page, Level);
//    }

//      if(Encoder_Value!=1||Encoder_Value!=4||QX_Key==1)
//      {
//         Menu_Display(Menu_Page,Level);
//      }

  //}
  else                    //====检测到按键松开====//
  {

      if (Encoder_KEY_COUNT < KEY_COUNT_DESHAKING)    //没到消抖时长就松开按键，复位所有定时器和按键标志
      {
          Encoder_KEY_COUNT = 0;
          FLAG_Encoder_KEY_ShotClick = 0;
          FLAG_Encoder_KEY_LongClick = 0;
          FLAG_Encoder_KEY_DoubleClick = 0;
          Encoder_KEY_DoubleClick_Count = 0;
      }
      else
      {

          if (FLAG_Encoder_KEY_ShotClick == 1)        //短按按键定时有效期间
          {
              if ((FLAG_Encoder_KEY_DoubleClick == 0) && (Encoder_KEY_DoubleClick_Count >= 0))
                  Encoder_KEY_DoubleClick_Count++;
              if ((FLAG_Encoder_KEY_DoubleClick == 1) && (Encoder_KEY_DoubleClick_Count <= KEY_COUNT_DUALCLICKTIME))   //如果在规定双击时间内再次按下按键
              {                                                                                               //认为按键是双击动作
                  FLAG_Encoder_KEY_DoubleClick = 2;
              }

              if ((FLAG_Encoder_KEY_DoubleClick == 0) && (Encoder_KEY_DoubleClick_Count > KEY_COUNT_DUALCLICKTIME))    //如果没有在规定双击时间内再次按下按键
                  FLAG_Encoder_KEY_ShotClick = 0;                                                                //认为按键是单击动作
          }

//          if (FLAG_Encoder_KEY_LongClick == 1)        //检测到长按按键松开
//              FLAG_Encoder_KEY_LongClick = 0;
      }

   }
  //>>>>>>>>>>>>>>>>编码器按键分析处理程序<<<<<<<<<<<<<<<<//
//  if (Encoder_KEY_COUNT > KEY_COUNT_DESHAKING)    //短按按键延时到了时间
//  {

//      //短按按键动作结束代码
//      if ((FLAG_Encoder_KEY_ShotClick == 0) && (Encoder_KEY_DoubleClick_Count > KEY_COUNT_DUALCLICKTIME) && (Encoder_KEY_COUNT < KEY_COUNT_LONGTIME))   //短按按键动作结束代码
//      {
//          //--------短按按键动作结束代码--------//
//          Encoder_NUM_SW++;
//          if (Encoder_NUM_SW >= 4)
//              Encoder_NUM_SW = 1;
//          AnalyzeResult = 1;
//          //--------清除标志位--------//
//          Encoder_KEY_COUNT = 0;
//          Encoder_KEY_DoubleClick_Count = 0;
//          FLAG_Encoder_KEY_DoubleClick = 0;
//      }

//      //双击按键动作结束代码
//      if ((FLAG_Encoder_KEY_DoubleClick == 2) && (Encoder_KEY_DoubleClick_Count > 0) && (Encoder_KEY_DoubleClick_Count <= KEY_COUNT_DUALCLICKTIME)) //双击按键动作结束代码
//      {
//          //--------双击按键动作结束代码--------//
//          if (Encoder_NUM_SW == 5)
//              Encoder_NUM_SW = 0;
//          if (Encoder_NUM_SW == 4)
//              Encoder_NUM_SW = 5;

//          if (Encoder_NUM_SW <4)
//          {
//              Encoder_NUM_SW = 4;
//          }
//          AnalyzeResult = 2;
//          //--------清除标志位--------//
//          Encoder_KEY_COUNT = 0;
//          Encoder_KEY_DoubleClick_Count = 0;
//          FLAG_Encoder_KEY_ShotClick = 0;
//          FLAG_Encoder_KEY_DoubleClick = 0;

//      }

//      //连续长按按键按下代码
//      if ((FLAG_Encoder_KEY_LongClick == 1) && (Encoder_KEY_COUNT >= KEY_COUNT_LONGTIME))  //连续长按按键按下代码
//      {
//          TMP_Value++;
//          if (TMP_Value % KEY_LONG_REPEAT_TIME == 0)
//          {
//              TMP_Value = 0;
//              //-------连续长按按键按下代码--------//
//              AnalyzeResult = 4;
//          }
//      }

//      //长按按键动作结束代码
//      if ((FLAG_Encoder_KEY_LongClick == 0) && (Encoder_KEY_COUNT >= KEY_COUNT_LONGTIME))  //长按按键动作结束代码
//      {
//          //--------长按按键按下动作结束代码--------//
//          Encoder_NUM_SW = 0;
//          G_PWM_NUM1 = 0x20;
//          G_PWM_NUM2 = 0x20;
//          G_PWM_NUM3 = 0x20;
//          AnalyzeResult = 3;
//          //--------清除标志位--------//
//          Encoder_KEY_COUNT = 0;
//      }
//  }
  return AnalyzeResult;
}