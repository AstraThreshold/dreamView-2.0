//
// Created by Fir on 2023/2/8.
//
//
// Created by Fir on 2023/2/8.
//
#include "oled_setup_u8g2_fir.h"
#include "main.h"
#include "dma.h"
#include "spi.h"
//#include "oled_menu_show_fir.h"

u8g2_t u8g2;

void OLED_WR_Byte_NOCMD(uint8_t dat)
{
#if HWORSWSPI
//	unsigned char spistate;
//	spistate =HAL_SPI_GetState(&hspi1);
//	while(spistate==HAL_SPI_STATE_BUSY || spistate==HAL_SPI_STATE_BUSY_TX);
  HAL_SPI_Transmit_DMA(&hspi2, &dat, 1);
#else
  uint8_t i;
  for(i=0; i<8; i++) {
    OLED_SCLK_Clr();
    if(dat&0x80)
      OLED_SDIN_Set();
    else
      OLED_SDIN_Clr();
    OLED_SCLK_Set();
    dat<<=1;
  }
#endif
}

void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
#if !HWORSWSPI
  uint8_t i;
#endif
  if (cmd == OLED_DATA)
    OLED_DC_Set();
  else
    OLED_DC_Clr();
  //OLED_CS_Clr();
#if HWORSWSPI
  HAL_SPI_Transmit_DMA(&hspi2, &dat, 1);
#else
  for(i=0; i<8; i++) {
    OLED_SCLK_Clr();
    if(dat&0x80)
      OLED_SDIN_Set();
    else
      OLED_SDIN_Clr();
    OLED_SCLK_Set();
    dat<<=1;
  }
#endif
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
  uint8_t i, n;
  for (i = 0; i < 8; i++)
  {
    OLED_WR_Byte(0xb0 + i, OLED_CMD);    //设置页地址（0~7）
    OLED_WR_Byte(0x02, OLED_CMD);      //设置显示位置—列低地址
    OLED_WR_Byte(0x10, OLED_CMD);      //设置显示位置—列高地址
    for (n = 0; n < 128; n++)OLED_WR_Byte(0, OLED_DATA);
  } //更新显示
}

void OLED_Set_Pos(uint8_t x, uint8_t y)
{
  OLED_WR_Byte(0xb0 + y, OLED_CMD);
  OLED_WR_Byte((((x + 2) & 0xf0) >> 4) | 0x10, OLED_CMD);
  OLED_WR_Byte(((x + 2) & 0x0f), OLED_CMD);
}

//开启OLED显示
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC命令
  OLED_WR_Byte(0X14, OLED_CMD);  //DCDC ON
  OLED_WR_Byte(0XAF, OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC命令
  OLED_WR_Byte(0X10, OLED_CMD);  //DCDC OFF
  OLED_WR_Byte(0XAE, OLED_CMD);  //DISPLAY OFF
}

//初始化SSD1306
void OLED_Init(void)
{
  OLED_RST_Set();
  HAL_Delay(100);
  OLED_RST_Clr();
  HAL_Delay(100);
  OLED_RST_Set();

  OLED_WR_Byte(0xAE, OLED_CMD);   //display off
  OLED_WR_Byte(0x20, OLED_CMD);  //Set Memory Addressing Mode
  OLED_WR_Byte(0x10,
               OLED_CMD);  //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET,OLED_CMD);11,Invalid
  OLED_WR_Byte(0xb0, OLED_CMD);  //Set Page Start Address for Page Addressing Mode,0-7
  OLED_WR_Byte(0xc8, OLED_CMD);  //Set COM Output Scan Direction
  OLED_WR_Byte(0x00, OLED_CMD);//---set low column address
  OLED_WR_Byte(0x10, OLED_CMD);//---set high column address
  OLED_WR_Byte(0x40, OLED_CMD);//--set start line address
  OLED_WR_Byte(0x81, 0xCF);   //--set contrast control register调整亮度 初始为最高亮度
  OLED_WR_Byte(0x7f, OLED_CMD);
  OLED_WR_Byte(0xa1, OLED_CMD);//--set segment re-map 0 to 127
  OLED_WR_Byte(0xa6, OLED_CMD);//--set normal display
  OLED_WR_Byte(0xa8, OLED_CMD);//--set multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3F, OLED_CMD);//
  OLED_WR_Byte(0xa4, OLED_CMD);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  OLED_WR_Byte(0xd3, OLED_CMD);//-set display offset
  OLED_WR_Byte(0x00, OLED_CMD);//-not offset
  OLED_WR_Byte(0xd5, OLED_CMD);//--set display clock divide ratio/oscillator frequency
  OLED_WR_Byte(0xf0, OLED_CMD);//--set divide ratio
  OLED_WR_Byte(0xd9, OLED_CMD);//--set pre-charge period
  OLED_WR_Byte(0x22, OLED_CMD); //
  OLED_WR_Byte(0xda, OLED_CMD);//--set com pins hardware configuration
  OLED_WR_Byte(0x12, OLED_CMD);
  OLED_WR_Byte(0xdb, OLED_CMD);//--set vcomh
  OLED_WR_Byte(0x20, OLED_CMD);//0x20,0.77xVcc
  OLED_WR_Byte(0x8d, OLED_CMD);//--set DC-DC enable
  OLED_WR_Byte(0x14, OLED_CMD);//
  OLED_WR_Byte(0xaf, OLED_CMD);//--turn on oled panel
//	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
//	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
//	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
//	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
//	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
//	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
//	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
//	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
//	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
//	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
//	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
//	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
//	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
//	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
//	OLED_WR_Byte(0x12,OLED_CMD);
//	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
//	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
//	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WR_Byte(0x02,OLED_CMD);//
//	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
//	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
//	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
//	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
//	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
//
//	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
  OLED_Clear();
  OLED_Set_Pos(0, 0);
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch (msg)
  {
    case U8X8_MSG_BYTE_SEND: /*通过SPI发送arg_int个字节数据*/
      HAL_SPI_Transmit_DMA(&hspi2, (uint8_t *)arg_ptr, arg_int);while(hspi2.TxXferCount);//DMA
      break;
    case U8X8_MSG_BYTE_INIT: /*初始化函数*/
      break;
    case U8X8_MSG_BYTE_SET_DC: /*设置DC引脚,表明发送的是数据还是命令*/
      HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    default:
      return 0;
  }
  return 1;
}

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  switch (msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT: /*delay和GPIO的初始化，在main中已经初始化完成了*/
      break;
    case U8X8_MSG_DELAY_MILLI: /*延时函数*/
      HAL_Delay(arg_int);
      break;
    case U8X8_MSG_GPIO_CS: /*片选信号*/
      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, arg_int);
      break;
    case U8X8_MSG_GPIO_DC:
      break;
    case U8X8_MSG_GPIO_RESET:
      break;
  }
  return 1;
}

void OLED_ClearBuffer()
{
  u8g2_ClearBuffer(&u8g2);
}

void OLED_Refresh()
{
  u8g2_SendBuffer(&u8g2);
}

void u8g2Init(u8g2_t *u8g2)
{
  u8g2_Setup_ssd1306_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);  // 初始化 u8g2 结构体
  u8g2_InitDisplay(&u8g2); // 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
  u8g2_SetPowerSave(&u8g2, 0); // 打开显示器

  u8g2_ClearBuffer(&u8g2);
}