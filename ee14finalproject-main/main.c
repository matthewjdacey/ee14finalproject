#include "stm32l476xx.h"
#include "SysTick.h"
#include "LCD.h"
#include "Gyro.h"
#include <stdlib.h>
#include <stdint.h>

#define L3GD20_STATUS_REG_ADDR		0x27
#define L3GD20_OUT_X_L_ADDR				0x28

extern volatile int ms;
void System_Clock_Init(void);
void joystick_Init(void);
struct gyro{
	float x; 
	float y;
	float z;
};

void System_Clock_Init(){
	
	RCC->CR |= RCC_CR_MSION; 
	
	// Select MSI as the clock source of System Clock
	RCC->CFGR &= ~RCC_CFGR_SW; 
	
	// Wait until MSI is ready
	while ((RCC->CR & RCC_CR_MSIRDY) == 0); 	
	
	// MSIRANGE can be modified when MSI is OFF (MSION=0) or when MSI is ready (MSIRDY=1). 
	RCC->CR &= ~RCC_CR_MSIRANGE; 
	RCC->CR |= RCC_CR_MSIRANGE_7;  // Select MSI 8 MHz	
 
	// The MSIRGSEL bit in RCC-CR select which MSIRANGE is used. 
	// If MSIRGSEL is 0, the MSIRANGE in RCC_CSR is used to select the MSI clock range.  (This is the default)
	// If MSIRGSEL is 1, the MSIRANGE in RCC_CR is used. 
	RCC->CR |= RCC_CR_MSIRGSEL; 
	
	// Enable MSI and wait until it's ready	
	while ((RCC->CR & RCC_CR_MSIRDY) == 0); 		
}

void joystick_Init() {
	// Turn on clock for GPIO port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Set A0 to input mode
	GPIOA->MODER &= ~0x3;
}

int main(void){
	//System Clock Initialization
	System_Clock_Init();
	//LED Initialization
	LCD_Initialization();
	joystick_Init();
	EXTI_Init();
	
	


	//SysTick Initialization
	//SysTick_Initialize(1000); // 1000 ticks
	//delay of 1Sec
	
	
	// while(1) {
	// 	delay(1000); // 1000 calls of the handler
	// 	//LED Toggle
	// 	Red_LED_Toggle();
	// }
	
	// ---- STOP WATCH ----
	
	SysTick_Initialize(10000); // SysTick reloads every 10ms
		

	struct gyro myGyro;
	int16_t gyro_x, gyro_y, gyro_z;
	uint8_t gyr[6], status;
	
	GYRO_IO_Read(&status, L3GD20_STATUS_REG_ADDR, (uint8_t) 1);
	if( (status & 0x08) == 0x08) {
		GYRO_IO_Read(gyr, L3GD20_OUT_X_L_ADDR, 6);
		
		gyro_x = (int16_t) ((uint16_t) (gyr[1]<<8) + gyr[0]);
		gyro_y = (int16_t) ((uint16_t) (gyr[3]<<8) + gyr[2]);
		gyro_z = (int16_t) ((uint16_t) (gyr[4]<<8) + gyr[4]);
		
		myGyro.x = (float) gyro_x * .07f;
		myGyro.y = (float) gyro_y * .07f;
		myGyro.z = (float) gyro_z * .07f;	
	}	
	
	while(1) {
		LCD_Display_Timer(ms);
	}
	
}
