#ifndef __STM32L476G_DISCOVERY_SYSTICK_H
#define __STM32L476G_DISCOVERY_SYSTICK_H

#include "stm32l476xx.h"

extern volatile int joy_pressed;
extern volatile int ms;
extern volatile int min;
extern volatile int configuring;
extern volatile int pos;

void SysTick_Initialize(uint32_t ticks);
void SysTick_Handler(void);
void clockInit(void);

#endif /* __STM32L476G_DISCOVERY_SYSTICK_H */
