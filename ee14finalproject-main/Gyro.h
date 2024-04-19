#ifndef __STM32L476G_DISCOVERY_GYRO_H
#define __STM32L476G_DISCOVERY_GYRO_H
#include <stdint.h>

void GYRO_IO_WRITE(uint8_t *pBuffer, uint8_t WriteAddr, uint8_t size);
void GYRO_IO_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint8_t size);
void SPI_Init(void);
void SPI_Write(uint8_t *txBuffer, uint8_t * rxBuffer, int size);
void SPI_Read(uint8_t *rxBuffer, int size);

#endif
