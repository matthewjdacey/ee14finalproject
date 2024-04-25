#ifndef __STM32L476G_DISCOVERY_INTERFACE_H
#define __STM32L476G_DISCOVERY_INTERFACE_H

#include "stm32l476xx.h"
#include <string.h>
#include <stdio.h>
#include "LCD.h"
#include "gyro.h"
#include "SysTick.h"

void joystick_Init(void);
void EXTI2_Init(void);
void EXTI2_IRQHandler(void);
void drive_LCD(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI_Init(void);
void EXTI1_IRQHandler(void);

#endif
