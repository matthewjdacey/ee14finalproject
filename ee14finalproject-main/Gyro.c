#include "stm32l476xx.h"
#include "SysTick.h"
#include "Gyro.h"


#define L3GD20_CS_LOW							GPIOD->ODR &= ~(1U << 7)
#define L3GD20_CS_HIGH						GPIOD->ODR |= (1U << 7)


void GYRO_IO_WRITE(uint8_t *pBuffer, uint8_t WriteAddr, uint8_t size) {
	uint8_t rxBuffer[32];
	
	// if (NumByteToWrite > 0x01) // What?? what is this variable
		WriteAddr = 1U << 6; // Select the mode of writing multiple-byte

	// Set SPI interface
	L3GD20_CS_LOW; // 0 = SPI, 1 = I2C
	delay(10); // Short delay
	
	// Send the address of the indexed register
	SPI_Write(&WriteAddr, rxBuffer, 1);
	
	// Send the data that will be written into the device
	// Bit transfer order: Most significant bit first
	SPI_Write(pBuffer, rxBuffer, size);
	
	// Set chip select High at the end of the transmission
	delay (10); // Short deLay
	L3GD20_CS_HIGH; // 0 = SPI, 1 = I2C
}


void GYRO_IO_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint8_t size) { 
	uint8_t rxBuffer [32];
		
	// Select read & multiple-byte mode
	uint8_t AddrByte = ReadAddr | 1U << 7 | 1U << 6;
	
	// Set chip select low at the start of the transmission
	L3GD20_CS_LOW; // 0 = SPI, 1 = I2C
	delay(10); // Short deLay
	
	// Send the address of the indexed register 
	SPI_Write(&AddrByte, rxBuffer, 1);
	
	// Receive the data that will be read from the device (MSB First)
	SPI_Read(pBuffer, size);
	
	// Set chip select High at the end of the transmission
	delay(10); // Short delay
	L3GD20_CS_HIGH; // 0 = SPI, 1 = I2C

}
void SPI_Init(void) {

	// Enable SPI's clock
	//if (SPI2 == SPI1){
	//	RCC->AHB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SPIl Clock
	//	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST; // Reset SPI1
	//	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST; // Clear the reset of SPIl
	//}
	
	// else if(SPI2 == SPI2){
		RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN; // Enable SPI2 CLock
		RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST; // Reset SPIZ
		RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI2RST; // Clear the reset of SPI2	
	// } 
	
	// else if(SPI2 == SPI3){
	// 	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN; // Enable SPI3 Clock
	// 	RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI3RST; // Reset SPIS
	// 	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI3RST; // CLear the reset of SPI
	// }
	
	SPI2->CR1 &= ~SPI_CR1_SPE; // Disable SPI
	
	// Configure duplex or receive-only
	SPI2->CR1 &= ~SPI_CR1_RXONLY; // 0 = Full dupLex, 1 = Receive-onLy
	
	// Bidirectional data mode enable: This bit enables half-duplex
	// communication using a common single bidirectional data Line.
	// 0 = 2-Line unidirectional data mode selected
	// 1 = 1-Line bidirectional data mode selected
	SPI2->CR1 &= ~SPI_CR1_BIDIMODE;
	
	// Output enable in bidirectional mode
	// 0 = Output disabled (receive-only mode)
	// 1 = Output enabled (transmit-only mode)
	SPI2->CR1 &= ~SPI_CR1_BIDIOE;
	
	// Data Frame Format
	SPI2->CR2 &= ~SPI_CR2_DS;
	SPI2->CR2 = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2; // 0111: 8-bit
	
	// Bit order
	// 0 = MSB transmitted/received first
	// 1 = LSB transmitted/received first
	SPI2->CR1 &= ~SPI_CR1_LSBFIRST; // Most significant bit first
	
	// Clock phase
	// 0 = The first clock transition is the first data capture edge
	// 1 = The second clock transition is the first data capture edge
	SPI2->CR1 &= ~SPI_CR1_CPHA; // 1st edge
	
	// Clock polarity
	// 0 = Set CK to 0 when idle
	// 1 = Set CK to 1 when idle
	SPI2->CR1 &= ~SPI_CR1_CPOL; // Polarity Low
	
	// Baud rate control:
	// SPI baud rate is set to 5 MHz
	SPI2->CR1 |= 3U<<3; // Set SPI cLock to 80MHz/16 = 5 MHz\
	
	// CRC Polynomial
	SPI2->CRCPR = 10;
	
	// Hardware CRC calculation disabled
	SPI2->CR1 &= ~SPI_CR1_CRCEN;
	
	// Frame format: 0 = SPI Motorola mode, 1 = SPI TI mode
	SPI2->CR2 &= ~SPI_CR2_FRF;
	
	// NSSGPIO: The value of SSI is forced onto the NSS pin, and the IO value
	// of the NSS pin is ignored.
	// 1 = Software slave management enabled
	// 0 = Hardware NSS management enabled
	SPI2->CR1 = SPI_CR1_SSM;
	
	// Set as Master: 0 = slave, 1 = master
	SPI2->CR1 |= SPI_CR1_MSTR;
	
	// Manage NSS (slave selection) using Software
	SPI2->CR1 |= SPI_CR1_SSI;
	
	// Enable NSS pulse management
	SPI2->CR2 |= SPI_CR2_NSSP;
	
	// Receive buffer not empty (RXNE)
	// The RNE flag is set depending on the FRXTH bit value in the SPI2 CR2 register:
	// (1) If FRXTH is set, RNE goes high and stays high until the RXFIFO Level is
	// 		greater or equal to 1/4 (8-bit).
	// (2) If FRXTH is cleared, RXNE goes high and stays high until the RXIFo Level is
	//		higher than or equal to 1/2 (16-bit).
	SPI2->CR2 |= SPI_CR2_FRXTH;
	
	// Enable SPI
	SPI2->CR1 |= SPI_CR1_SPE;
}


void SPI_Write(uint8_t *txBuffer, uint8_t * rxBuffer, int size) {
	int i = 0;
	for (i = 0; i < size; i++) {
		// wait for TXE (Transmit buffer empty)
		while( (SPI2->SR & SPI_SR_TXE ) != SPI_SR_TXE);
		
		// Type-cast the pointer to ensure a single byte is written to DR
		*((volatile uint8_t*)&SPI2->DR) = txBuffer [i];
		
		// Wait for RXNE (Receive buffer not empty)
		while( (SPI2->SR & SPI_SR_RXNE ) != SPI_SR_RXNE );
		
		// Type-cast the pointer to ensure a single byte is read from DR
		rxBuffer[1] = *((volatile uint8_t*)&SPI2->DR);
	}
	// Wait for the busy flag cleared
	while( (SPI2->SR & SPI_SR_BSY) == SPI_SR_BSY );
}

void SPI_Read(uint8_t *rxBuffer, int size) {
	int i = 0;
	for (i = 0; i < size; i++) {
		// Wait for TXE (Transmit buffer empty)
		while( (SPI2->SR & SPI_SR_TXE ) != SPI_SR_TXE);
		
		// The clock is controlled by the master. Thus, the master must send a
		// byte data to the slave to start the clock.
		// Type-cast the pointer to ensure a single byte is written to DR
		// volatile: preventing compilers from false optimizations
		*((volatile uint8_t*)&SPI2->DR) = 0xFF; // A dummy byte
		
		// Wait for RXNE (Receive buffer not empty)
		while( (SPI2->SR & SPI_SR_RXNE ) != SPI_SR_RXNE );
		
		// Type-cast the pointer to ensure a single byte is read from DR
		rxBuffer[i] = *((volatile uint8_t*)&SPI2->DR);
	}
	// Wait for the busy flag cleared 
	while( (SPI2->SR & SPI_SR_BSY) == SPI_SR_BSY );
}
