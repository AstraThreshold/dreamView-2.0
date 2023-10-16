#include "stm32f1xx_hal.h"
#include "shutter_timer_fir.h"

//
// Created by Fir on 2023/2/13.
//

uint8_t fac_us=0;
uint16_t fac_ms=0;

//void delay_Init()
//{
//  //只可以选择不分频或者8分频，这里选择系统时钟8分频，最后频率为9MHZ
//  SysTick->CTRL &= ~(1<<2);
//  //SystemCoreClock为72000000，最终fac_us为9，也就是记录震动9次。因为频率为9MHZ所以为1us
//  fac_us  = SystemCoreClock  / 8000000;
//  fac_ms  = fac_us*1000;  //1000us=1ms
//}
//
//uint32_t GXT_SYSTICK_IsActiveCounterFlag(void)
//{
//  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
//}

//static uint32_t getCurrentMicros(void)
//{
//  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
//  GXT_SYSTICK_IsActiveCounterFlag();
//  uint32_t m = HAL_GetTick();
//  const uint32_t tms = SysTick->LOAD + 1;
//  __IO uint32_t u = tms - SysTick->VAL;
//  if (GXT_SYSTICK_IsActiveCounterFlag())
//  {
//    m = HAL_GetTick();
//    u = tms - SysTick->VAL;
//  }
//  return (m * 1000 + (u * 1000) / tms);
//}
//
////获取系统时间，单位us
//uint32_t micros(void)
//{
//  return getCurrentMicros();
//}


