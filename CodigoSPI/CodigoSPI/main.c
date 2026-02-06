/*
 * CodigoSPI.c
 *
 * Created: 2/3/2026 11:29:36 PM
 * Author : rodri
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "LibSPI.h"

uint8_t valorSPI = 0;
uint8_t POT1ADC;
uint8_t POT2ADC;
uint8_t dummy;


void refreshPORT(uint8_t valor);



int main(void)
{
	cli();
	DDRC |= (1<<DDC5);//Se configura como salida para SS, si es esclavo se pone como entrada &= ~ +
	DDRD |= (1<<DDD2)|(1<<DDD3)|(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);
	DDRB |= (1<<DDB0)|(1<<DDB1); //Bits conectados a LEDS
	
	PORTC |= (1<<PORTC5);//MSS, si es esclavo se comenta esto +
	
	//LEDS a cero
	PORTB &= ~((1<<PORTB0)|(1<<PORTB1));
	PORTD &= ~((1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7));
	
	spiInit(SPI_MASTER_OSC_DIV16,SPI_DATA_ORDER_MSB,SPI_CLOCK_IDLE_LOW,SPI_CLOCK_FIRST_EDGE);//Maestro +
	//spiInit(SPI_SLAVE_SS,SPI_DATA_ORDER_MSB,SPI_CLOCK_IDLE_LOW,SPI_CLOCK_FIRST_EDGE);//Esclavo +
	
	//SPCR |= (1<<SPIE); //se activa interrupcion solo para esclavo? o tambien como maestro? +
	
	//Configuracion de ADC
	ADMUX |= (1 << ADLAR); 
	ADMUX = (1 << REFS0); // AVcc como referencia. Quitar la configuracion de ADLAR cuando se usen servos
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
	
	//Configuracion de la comunicacion serial
	DDRD |= (1 << DDD1); // Configura TX (D1) como salida
	DDRD &= ~(1 << DDD0); // Configura RX (D0) como entrada
	UCSR0A = 0; // Modo normal, sin doble velocidad ni flags activos
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); // Habilita interrupción por recepción, RX y TX
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Modo asíncrono, sin paridad, 1 bit de stop, 8 bits de datos
	UBRR0 = 103; // Baud rate de 9600 para F_CPU = 16 MHz en modo normal
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {	
		//Codigo de maestro +
        // cualquier otro carácter se ignora
    
	
		//PORTC &= ~(1<<PORTC5); //Se activa el SS
		
		spiWrite('c');
		spiWrite('x');
		POT1ADC = spiRead();
		spiWrite('d');
		spiWrite('x');
		POT2ADC = spiRead();
		refreshPORT(POT2ADC);
		//spiWrite('x');
		spiWrite(valor_port);
		//spiRead(); 
		
		
		//PORTC |= (1<<PORTC5); //Se desactiva el SS
		
		//uint16_t valor2 = LecturaADC(3);
		char buffer2[6];   // máx 65535 + '\0'
		itoa(POT1ADC, buffer2, 10);
		
		USART_TX_String("POT1=");
		USART_TX_String(buffer2);
		USART_TX_Char(' ');
	
		USART_TX_String("POT2=");
		USART_TX_Uint(POT2ADC);
		USART_TX_String("\r\n");
		
		_delay_ms(250);
		
		//Codigo de esclavo+
		//POT1ADC = LecturaADC(2);
		//POT2ADC = LecturaADC(3);
		
		
	
    }
}

/*ISR(SPI_STC_vect){//interrupcion para recibir info como esclavo? o tambien como maestro?
	uint8_t spiValor = SPDR;
	if(spiValor == 'c'){
		SPDR = POT1ADC;//El valor que se mande debe ser de 8 bits
	}else if(spiValor == 'd'){
		SPDR = POT2ADC;
	}else if((spiValor != 'x')&(spiValor != 'c')&(spiValor != 'd')) {
		refreshPORT(spiValor);
	}
	
}*/
