/*
 * LibSPI.h
 *
 * Created: 2/4/2026 8:40:40 PM
 *  Author: rodri
 */ 


#ifndef LIBSPI_H_
#define LIBSPI_H_

#include <avr/io.h>
#include <stdint.h>

extern volatile char rx_char;
extern volatile uint8_t rx_flag;
extern volatile uint8_t valor_port;
typedef enum
{
	SPI_MASTER_OSC_DIV2		=0b01010000,
	SPI_MASTER_OSC_DIV4		=0b01010001,
	SPI_MASTER_OSC_DIV8		=0b01010010,
	SPI_MASTER_OSC_DIV16	=0b01010011,
	SPI_MASTER_OSC_DIV32	=0b01010100,
	SPI_MASTER_OSC_DIV64	=0b01010101,
	SPI_MASTER_OSC_DIV128	=0b01010110,
	SPI_SLAVE_SS			=0b01000000
	}Spi_Type;
	
typedef enum
{
	SPI_DATA_ORDER_MSB		=0b00000000,
	SPI_DATA_ORDER_LSB		=0b00100000
	}Spi_Data_Order;

typedef enum
{
	SPI_CLOCK_IDLE_HIGH		=0b00001000,
	SPI_CLOCK_IDLE_LOW		=0b00000000
	}Spi_Clock_Polarity;

typedef enum
{
	SPI_CLOCK_FIRST_EDGE	=0b00000000,
	SPI_CLOCK_LAST_EDGE	=0b00000000,
	}Spi_Clock_Phase;

void USART_TX_Char(char c);

void USART_TX_String(const char *s);

void USART_TX_Uint(uint16_t num);

char UART_ReceiveChar(void);

void spiInit(Spi_Type, Spi_Data_Order, Spi_Clock_Polarity, Spi_Clock_Phase);
void spiWrite(uint8_t dat);
unsigned spiDataReady();
uint8_t spiRead(void);
uint8_t LecturaADC(uint8_t PIN);
void refreshPORT(uint8_t valor);
#endif /* LIBSPI_H_ */