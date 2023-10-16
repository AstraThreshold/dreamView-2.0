//
// Created by Fir on 2023/2/24.
//
#ifndef OLED_ENCODER_FIR_H
#define OLED_ENCODER_FIR_H

#include "u8g2.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 在研究Encoder的时序之前首先要了解一点，Encoder按旋转的输出动作可以分为两种
 * 一种是转两格，A、B对C端输出一个完整脉冲
 * （转一格就只是由低电平->高电平或由高电平->低电平）
 * 另一种就是转一格，A、B对C端输出一个完整脉冲
 * 一定位一脉冲的Encoder按测试电路图的接法，
 * 在静止的时候AB两线输出都是高电平
 * 转动一格，AB两线各自输出一个低电平脉冲，然后又回到高电平状态
 * 对应于Encoder内部AB两个触点开关的动作为断开-->闭合-->断开。
 *
 * 两定位一脉冲的Encoder稍微复杂一些，转动一格只会输出半个脉冲
 * 若初始状态时AB都是高电平，转动一格就输出从高电平到低电平的下降沿，随后一直输出低电平，
 * 对应于Encoder内部AB两个触电开关的动作为断开-->闭合
 * 若初始状态时AB都是低电平，转动一格就输出从低电平到高电平的上升沿，随后一直输出低电平，
 * 对应于Encoder内部AB两个触点开关的动作为闭合-->断开
 *
 * 因此对于无特殊要求的人机输入应用来说，我都推荐使用一定位一脉冲的Encoder
 */

extern uint8_t encoder_num_sw;

#define Encoder_A_Now HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)
#define Encoder_B_Now HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)
#define Encoder_Key HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin)

#define Encoder_SCAN_PERIOD_MS          1                              //Encoder编码器扫描周期
#define KEY_COUNT_DESHAKING         (150/Encoder_SCAN_PERIOD_MS)        //按键消抖时间
#define KEY_COUNT_LONGTIME          (600/Encoder_SCAN_PERIOD_MS)       //长按按键判断时间
#define KEY_COUNT_DUALCLICKTIME     (150/Encoder_SCAN_PERIOD_MS)       //双击按键判断时间
#define KEY_LONG_REPEAT_TIME        (200/Encoder_SCAN_PERIOD_MS)       //长按按键的回报率的倒数，即一直长按按键时响应的时间间隔

extern int32_t Encoder_KEY_COUNT;                      //Encoder按键动作计数器
extern int32_t Encoder_KEY_DoubleClick_Count;          //Encoder按键双击动作计数器
extern int8_t FLAG_Encoder_KEY_ShotClick;            //Encoder按键短按动作标志
extern int8_t FLAG_Encoder_KEY_LongClick;            //Encoder按键长按动作标志
extern int8_t FLAG_Encoder_KEY_DoubleClick;          //Encoder按键双击动作标志

/**
 * @brief 对Encoder旋转编码器的连接IO口做IO口模式设置 以及将相关的变量进行初始化
 * @param Set_Encoder_TYPE Encoder旋转编码器的类型
 */
extern void Encoder_Init(uint8_t Set_Encoder_TYPE);

/**
 * @brief 扫描Encoder旋转编码器的动作并将参数返回给动作分析函数使用
 * @return 返回Encoder旋转编码器的扫描结果
 * @warning 只扫描Encoder旋转编码器有没有动作，不关心是第几次按下按键或长按或双击
 */
extern int8_t Encoder_Scan(void);

/**
 * @brief 对Encoder旋转编码器的动作进行分析 并作出相应的动作处理
 * @brief 判断是单击还是双击还是长按松手还是一直按下
 * @param Encoder_Value
 * @return 目前无用
 */
extern int8_t Encoder_Analyze(int8_t Encoder_Value);

//extern void EX_Init(void);

#ifdef __cplusplus
}
#endif

#endif //OLED_ENCODER_FIR_H
