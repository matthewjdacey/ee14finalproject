#include "stm32l476xx.h"
#include "SysTick.h"
#include "UART.h"
#include "SysClock.h"
#include "gyro.h"

#include <string.h>
#include <stdio.h>

#define L3GD20_STATUS_REG_ADDR  0x27 // Status register
#define L3GD20_OUT_X_L_ADDR			0x28 // Output register

// char RxComByte = 0;
// uint8_t buffer[BufferSize];
// char str[] = "Give Red LED control input (Y = On, N = off):\r\n";
// 
// int main(void){
// 	char rxByte;
// 	
// 	System_Clock_Init(); // Switch System Clock = 80 MHz
// 	UART2_Init();
// 		
// 	printUART(str);
// }

int n;
uint8_t buffer[BufferSize];

int rightTurn(void) {
	int turn = 0;
	if(getZ() <= -500) {
		printUART("rightTurn!");
		turn = 1;
	}
	return turn;
}

int leftTurn(void) {
	int turn = 0;
	if(getZ() >= 500) {
		printUART("leftTurn!");
		turn = 1;
	}
	return turn;
}

int main(void){
	float LRTest = 0;
	float UDTest = 0;
	float Z = 0;
	float Y = 0;
	const int CENTER_SENS = 2000;
	const int PEDAL_SENS = 1500;
	// System Clock Initialization
	System_Clock_Init();
	// UART Driver Initialization
	UART2_Init();
												 
	printUART("~~~~ Hello! ~~~~");
	printUART("=== Pre-Gyro Initialization ====");
	// Gyro Initialization
	verboseInitializeGyro();
	gyrodefault_tester();
	printUART("~~~~ Goodbye! ~~~~");
	
 	while (1) {
		printXYZ();
	}
}