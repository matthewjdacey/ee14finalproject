#include "Interface.h"

// '0' == time of day; '1' == step counter; '2' == stopwatch
volatile uint8_t screen_select = 0;

void joystick_Init(void) {
	// Turn on clock for GPIO port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Set ports to input mode (00)
	GPIOA->MODER &= ~0xFF;				// ports 0-3
	GPIOA->MODER &= ~(3 << 10);		// ports 5
	
	// Enable pull down mode on the side buttons (10)
	GPIOA->PUPDR &= ~0xCFC;
	GPIOA->PUPDR |= 0x8A8;
}

// Setting up interrupts for A[0, 1, 2, 3, 5]
void EXTI_Init(void) {
	// Enable clock for port A
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// Configure output lines 0-3, 5
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5;
	
	// Setup port A
	SYSCFG->EXTICR[0] = 0;
	SYSCFG->EXTICR[1] = 0;
	
	// Disabling rising edge triggers
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT0;
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT1;
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT2;
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT3;
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT5;

	// Enable falling edge
	EXTI->FTSR1 |= EXTI_FTSR1_FT0;
	EXTI->FTSR1 |= EXTI_FTSR1_FT1;
	EXTI->FTSR1 |= EXTI_FTSR1_FT2;
	EXTI->FTSR1 |= EXTI_FTSR1_FT3;
	EXTI->FTSR1 |= EXTI_FTSR1_FT5;
	
	// Enable EXTI interrupt
	EXTI->IMR1 |= EXTI_IMR1_IM0;
	EXTI->IMR1 |= EXTI_IMR1_IM1;
	EXTI->IMR1 |= EXTI_IMR1_IM2;
	EXTI->IMR1 |= EXTI_IMR1_IM3;
	EXTI->IMR1 |= EXTI_IMR1_IM5;
	
	// Set priorities all to the same value (treating all buttons equally)
	NVIC_SetPriority(EXTI0_IRQn, 39);
	NVIC_SetPriority(EXTI1_IRQn, 39);
	NVIC_SetPriority(EXTI2_IRQn, 39);
	NVIC_SetPriority(EXTI3_IRQn, 39);
	NVIC_SetPriority(EXTI9_5_IRQn, 39);
	
	// Enable interrupt requests
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	
}

void EXTI0_IRQHandler(void) {
	//Clear pending status
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	
	// Timer mode
	if (screen_select == 2) {
		if (joy_pressed == 0)
			joy_pressed = 1;
		else
			joy_pressed = 0;
	}
	
	// Set configure mode
	if (screen_select == 0)
		configuring = !configuring;
	
	//Clear interrupt pending request
	EXTI->PR1 |= EXTI_PR1_PIF0;
}

void EXTI1_IRQHandler(void) {
	//Clear pending status
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	
	// Toggle through the screens right to left
	if (configuring == 0) {
		if (screen_select == 0)
			screen_select = 2;
		else
			screen_select--;
	}
	// In configure mode
	else {
		if (pos == 0)
			pos = 2;
		else
			pos--;
	}
	
	// Clear interrupt pending request
	EXTI->PR1 |= EXTI_PR1_PIF1;
}

void EXTI2_IRQHandler(void) {
	//Clear pending status
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	
	// Toggle through the screens left to right
	if (configuring == 0) {
		if (screen_select == 2)
			screen_select = 0;
		else
			screen_select++;
	}
	// In configure mode
	else {
		if (pos == 2)
			pos = 0;
		else
			pos++;
	}
	//Clear interrupt pending request
	EXTI->PR1 |= EXTI_PR1_PIF2;
}

void EXTI3_IRQHandler(void) {
	//Clear pending status
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	
	if (configuring)
		timeUp();
		
	//Clear interrupt pending request
	EXTI->PR1 |= EXTI_PR1_PIF3;
}

void EXTI9_5_IRQHandler(void) {
	//Clear pending status
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	
	// Reset timer
	if (screen_select == 2) {
		joy_pressed = 1;
		ms = 0;
	}
	
	// Reset pedometer
	if (screen_select == 1) 
		steps = 0;
	
	if (configuring)
		timeDown();
		
	//Clear interrupt pending request
	EXTI->PR1 |= EXTI_PR1_PIF5;
}

void drive_LCD(void) {
	if(screen_select == 0)
		LCD_Display_TOD(min);
	else if(screen_select == 1)
		LCD_Display_Steps(steps);
	else
		LCD_Display_Timer(ms);
}
