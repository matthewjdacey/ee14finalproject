#ifndef __STM32L476G_DISCOVERY_SYSTICK_H
#define __STM32L476G_DISCOVERY_SYSTICK_H

#include "stm32l476xx.h"

void SysTick_Initialize(uint32_t ticks);
void SysTick_Handler(void);
void delay (uint32_t T);
void EXTI_Init(void);
void EXTI0_IRQHandler(void);

#endif /* __STM32L476G_DISCOVERY_SYSTICK_H */
