//
// Created by Fir on 2023/2/8.
// OLED初始化 包含清屏 u8g2初始化等函数
//

#ifndef OLED_SETUP_U8G2_FIR_H
#define OLED_SETUP_U8G2_FIR_H

#include "main.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"
#include "../../3rdParty/u8g2/u8x8.h"
#include "../../3rdParty/u8g2/u8g2.h"

#ifdef __cplusplus
extern "C" {
#endif

extern u8g2_t u8g2;

#define  HWSPI       1
#define  SWSPI       0
#define  HWORSWSPI   HWSPI

#define OLED_CMD  0  //写命令
#define OLED_DATA 1  //写数据

#define OLED_DC_Set()  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 1)
#define OLED_DC_Clr()  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 0)

#define OLED_RST_Set()  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, 1)
#define OLED_RST_Clr()  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, 0)

#define OLED_SCLK_Set()  HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, 1)
#define OLED_SCLK_Clr()  HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, 0)

#define OLED_SDIN_Set()  HAL_GPIO_WritePin(MOSI_GPIO_Port, MOSI_Pin, 1)
#define OLED_SDIN_Clr()  HAL_GPIO_WritePin(MOSI_GPIO_Port, MOSI_Pin, 0)

extern void OLED_WR_Byte_NOCMD(uint8_t dat);

extern void OLED_WR_Byte(uint8_t dat, uint8_t cmd);

extern void OLED_Set_Pos(uint8_t x, uint8_t y);

extern void OLED_Display_On(void);

extern void OLED_Display_Off(void);

//todo: 修改亮度函数（淘宝聊天记录 寄存器）
extern void OLED_Lighter(void);

extern void OLED_Darker(void);

extern void OLED_Clear(void);

extern void OLED_Init(void);

extern void delay_us(uint32_t time);

extern uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

extern uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                         U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                         U8X8_UNUSED void *arg_ptr);

extern void OLED_ClearBuffer();

extern void OLED_Refresh();

extern void u8g2Init(u8g2_t *u8g2);

#ifdef __cplusplus
}
#endif

#endif //OLED_SETUP_U8G2_FIR_H
