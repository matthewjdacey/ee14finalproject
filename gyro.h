#ifndef __STM32L476G_DISCOVERY_GYRO_L3GD20_H
#define __STM32L476G_DISCOVERY_GYRO_L3GD20_H

#include "stm32l476xx.h"

extern volatile int steps;

void initializeGyro(void);
int	 readGyroRegister(uint8_t addr, uint8_t *rBuffer);
int16_t readGyroLowHigh(unsigned char registerLower, unsigned char registerHigher);
float getX(void);
float getY(void);
float getZ(void);
void waitForNotBSY(void);
void waitForTXE(void);
void waitForRXEmpty(void);
void writeGyroRegister(uint8_t addr, uint8_t tBuffer);
void initializeSPI2(void);
float scaler(int16_t axis_data, int level);

void countSteps(void);

#endif /* __STM32L476G_DISCOVERY_GYRO_L3GD20_H */