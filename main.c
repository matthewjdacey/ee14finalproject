#include "stm32l476xx.h"
#include "SysTick.h"
#include "LCD.h"
#include <stdlib.h>

extern volatile int ms;
void System_Clock_Init(void);
void joystick_Init(void);

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
	
	while(1) {
		LCD_Display_Timer(ms);
	}
	

}
