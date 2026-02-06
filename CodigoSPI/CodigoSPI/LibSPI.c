/*
 * LibSPI.c
 *
 * Created: 2/4/2026 8:40:16 PM
 *  Author: rodri
 */ 
#include "LibSPI.h"
#include <avr/interrupt.h>

volatile char rx_char;
volatile uint8_t rx_flag = 0;
uint16_t uart_acc = 0;
volatile uint8_t valor_port = 0;

void USART_TX_Char(char c)
{
	while (!(UCSR0A & (1 << UDRE0))); // Espera buffer vacío
	UDR0 = c;
}

void USART_TX_String(const char *s)
{
	while (*s)
	{
		USART_TX_Char(*s++);
	}
}

void USART_TX_Uint(uint16_t num)
{
	char buffer[6];
	itoa(num, buffer, 10);
	USART_TX_String(buffer);
}

ISR(USART_RX_vect)
{
	rx_char = UDR0;   // Lee el dato recibido
	rx_flag = 1;      // Marca que llegó algo
	
	if (rx_flag) {          // ? CLAVE
        rx_flag = 0;        // consumir el carácter

        if (rx_char >= '0' && rx_char <= '9') {

            uart_acc = (uart_acc * 10) + (rx_char - '0');

            if (uart_acc > 255) {
                uart_acc = 255;
            }
        }
        else if (rx_char == '\n' || rx_char == '\r') {

            valor_port = (uint8_t)uart_acc;
            //refreshPORT(valor_port);
            uart_acc = 0;
        }
	
	}
}

void spiInit(Spi_Type sType, Spi_Data_Order sDataOrder, Spi_Clock_Polarity sClockPolarity, Spi_Clock_Phase sClockPhase)
{
	//PB2 -> SS
	//PB3 -> MOSI
	//PB4 -> MISO
	//PB5 -> SCK
	
	if(sType & (1<<MSTR))//Si soy maestro
	{
		DDRB |= (1<<DDB3)|(1<<DDB2)|(1<<DDB5);//MOSI, SS y SCK como salidas
		DDRB &= ~(1<<DDB4); //MISO como entrada
		SPCR |= (1<<MSTR); //Diciendole al registro que soy maestro
		
		uint8_t temp = sType & 0b00000111; //Mascara para solo interesarnos en los ultimos 3 bits que definen clock
		switch(temp){
			case 0: //DIV2
				SPCR &= ~((1<<SPR1)|(1<<SPR0));
				SPSR |= (1<<SPI2X);
			break;
			case 1: //DIV4
				SPCR &= ~((1<<SPR1)|(1<<SPR0));
				SPSR &= ~(1<<SPI2X);
			break;
			case 2: //DIV8
				SPCR &= ~(1<<SPR1);
				SPCR |= (1<<SPR0);
				SPSR |= (1<<SPI2X);
			break;
			case 3: //DIV16
				SPCR &= ~(1<<SPR1);
				SPCR |= (1<<SPR0);
				SPSR &= ~(1<<SPI2X);
			break;
			case 4: //DIV32
				SPCR &= ~(1<<SPR0);
				SPCR |= (1<<SPR1);
				SPSR |= (1<<SPI2X);
			break;
			case 5: //DIV64
				SPCR &= ~(1<<SPR0);
				SPCR |= (1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
			case 6: //DIV128
				SPCR |= ((1<<SPR1)|(1<<SPR0));
				SPSR &= ~(1<<SPI2X);
			break;
		}
		
	}
	else{ //aqui entra seguro al ser un esclavo
		DDRB |= (1<<DDB4); //MISO como salida
		DDRB &= ~((1<<DDB3)|(1<<DDB5)|(1<<DDB2)); //MOSI, SCK, SS como entradas
		SPCR &= ~(1<<MSTR); //Se escribe en el registro que es esclavo 
	}
	
	//Se habilita la comunicacion SPI y se meten los demas datos al registro
	SPCR |= (1<<SPE)|sDataOrder|sClockPolarity|sClockPhase;
}

static void spiReceiveWait(){
	while (!(SPSR & (1<<SPIF))); //Se espera a que los datos esten completos
}

void spiWrite(uint8_t dat){//escribe dat en el spi
	SPDR = dat;
}

unsigned spiDataReady(){//Verificar que los datos estan listos para leer
	if(SPSR &(1<<SPIF))
	return 1;
	else
	return 0;
}

uint8_t spiRead(void){//leer la data recibida
	while (!(SPSR & (1<<SPIF))); //Esperar para completar la recepcion
	return(SPDR); //Leer los datos
}

uint8_t LecturaADC(uint8_t PIN){ //Cambiar a uint16_t cuando se vaya a usar servos
	 ADMUX = (ADMUX & 0xF0) | PIN; // Seleccionar canal
	 ADCSRA |= (1 << ADSC); // Iniciar conversión
	 while (ADCSRA & (1 << ADSC)); // Esperar a que termine
	 uint8_t adc8 = ADC >> 2;
	 return adc8; //cambiar a solo ADC cuando se vaya a usar servos
	 
 }
 
 void refreshPORT(uint8_t valor){
	if(valor & 0b10000000){
		PORTB |= (1<<PORTB1);
	}else{
		PORTB &= ~(1<<PORTB1);
	}
	if(valor & 0b01000000){
		PORTB |= (1<<PORTB0);
		}else{
		PORTB &= ~(1<<PORTB0);
	}
	if(valor & 0b00100000){
		PORTD |= (1<<PORTD7);
		}else{
		PORTD &= ~(1<<PORTD7);
	}
	if(valor & 0b00010000){
		PORTD |= (1<<PORTD6);
		}else{
		PORTD &= ~(1<<PORTD6);
	}
	if(valor & 0b00001000){
		PORTD |= (1<<PORTD5);
		}else{
		PORTD &= ~(1<<PORTD5);
	}
	if(valor & 0b00000100){
		PORTD |= (1<<PORTD4);
		}else{
		PORTD &= ~(1<<PORTD4);
	}
	if(valor & 0b00000010){
		PORTD |= (1<<PORTD3);
		}else{
		PORTD &= ~(1<<PORTD3);
	}
	if(valor & 0b00000001){
		PORTD |= (1<<PORTD2);
		}else{
		PORTD &= ~(1<<PORTD2);
	}
}