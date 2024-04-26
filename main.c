#include "stm32l476xx.h"
#include "SysTick.h"
#include "UART.h"
#include "gyro.h"
#include "Interface.h"
#include "LCD.h"

#include <string.h>
#include <stdio.h>

#define L3GD20_STATUS_REG_ADDR  0x27 // Status register
#define L3GD20_OUT_X_L_ADDR			0x28 // Output register

int main(void){
	clockInit();
	SysTick_Initialize(10000);
	joystick_Init();
	LCD_Initialization();
	EXTI_Init();
	initializeGyro();
	
 	while (1) {
		drive_LCD();
		countSteps();
	}
}
