/*
 * LibLCD.c
 *
 * Created: 1/28/2026 11:18:47 PM
 *  Author: rodri
 */ 
#include "LibLCD.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile char rx_char;
volatile uint8_t rx_flag = 0;

void setup(void){
	cli();
	ADMUX = (1 << REFS0); // AVcc como referencia. Quitar la configuracion de ADLAR cuando se usen servos
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
	initLCD8bits();
	//Configuracion de la comunicacion serial
	DDRD |= (1 << DDD1); // Configura TX (D1) como salida
	DDRD &= ~(1 << DDD0); // Configura RX (D0) como entrada
	UCSR0A = 0; // Modo normal, sin doble velocidad ni flags activos
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); // Habilita interrupción por recepción, RX y TX
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Modo asíncrono, sin paridad, 1 bit de stop, 8 bits de datos
	UBRR0 = 103; // Baud rate de 9600 para F_CPU = 16 MHz en modo normal
	sei();
}

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
}

uint16_t LecturaADC(uint8_t PIN){ //Cambiar a uint16_t cuando se vaya a usar servos
	 ADMUX = (ADMUX & 0xF0) | PIN; // Seleccionar canal
	 ADCSRA |= (1 << ADSC); // Iniciar conversión
	 while (ADCSRA & (1 << ADSC)); // Esperar a que termine
	 return ADC; //cambiar a solo ADC cuando se vaya a usar servos
	 
 }

void initLCD8bits(void){
	DDRC |= (1<<DDC0)|(1<<DDC1)/*|(1<<DDC2)|(1<<DDC3)|(1<<DDC4)|(1<<DDC5)*/;
	PORTC = 0;
	DDRB |= (1<<DDB0)|(1<<DDB1);
	PORTB = 0;
	
	DDRD |= (1<<DDD0)|(1<<DDD1)|(1<<DDD2)|(1<<DDD3)|(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);
	PORTD =0;
	
	PORTC &= ~(1<<PORTC0);//se pone en cero el pin que sea RS
	LCD_Port(0x00);
	_delay_ms(15);
	LCD_CMD(0x30);
	_delay_ms(4);
	LCD_CMD(0x30);
	_delay_us(100);
	LCD_CMD(0x30);
	
	LCD_CMD(0x38);
	LCD_CMD(0x08);
	LCD_CMD(0x01);
	LCD_CMD(0x06);
	LCD_CMD(0x01);
	LCD_CMD(0x0C);
}

void LCD_CMD(char a){
	PORTC &= ~(1<<PORTC0);//se pone en cero el pin que sea RS
	LCD_Port(a);
	
	PORTC |= (1<<PORTC1);//Se enciende el pin que tenga el EN
	_delay_ms(4);
	PORTC &= ~(1<<PORTC1);//se pone en cero el pin que sea EN
}

void LCD_Port(char a){
	if (a & 0b00000001)
	PORTB |= (1<<PORTB0);
	else
	PORTB &= ~(1<<PORTB0);
	
	if (a & 0b00000010)
	PORTB |= (1<<PORTB1);
	else
	PORTB &= ~(1<<PORTB1);
	
	if (a & 0b00000100)
	PORTD |= (1<<PORTD2);
	else
	PORTD &= ~(1<<PORTD2);
	
	if (a & 0b00001000)
	PORTD |= (1<<PORTD3);
	else
	PORTD &= ~(1<<PORTD3);
	
	if (a & 0b00010000)
	PORTD |= (1<<PORTD4);
	else
	PORTD &= ~(1<<PORTD4);
	
	if (a & 0b00100000)
	PORTD |= (1<<PORTD5);
	else
	PORTD &= ~(1<<PORTD5);
	
	if (a & 0b01000000)
	PORTD |= (1<<PORTD6);
	else
	PORTD &= ~(1<<PORTD6);
	
	if (a & 0b10000000)
	PORTD |= (1<<PORTD7);
	else
	PORTD &= ~(1<<PORTD7);
	
}

void LCD_Write_Char(char c){
	/*char Cbajo, Calto;
	Cbajo = c & 0x0F;
	Calto = (c & 0xF0)>>4;*/
	
	PORTC |= (1<<PORTC0);//RS se enciende
	//LCD_Port(Calto);
	LCD_Port(c);
	PORTC |= (1<<PORTC1);//EN se enciende
	_delay_ms(4);
	PORTC &= ~(1<<PORTC1);//EN se apaga
	/*LCD_Port(Cbajo);
	PORTC |= (1<<PORTC1);//EN se enciende
	_delay_ms(4);
	PORTC &= ~(1<<PORTC1);//EN se apaga*/
	
}

void LCD_Write_String(char *a){
	int i;
	for (i = 0; a[i] != '\0'; i++)
	LCD_Write_Char(a[i]);
}

void LCD_Shift_Right(void){
	LCD_CMD(0x1C);
}

void LCD_Shift_Left(void){
	LCD_CMD(0x18);
}

void LCD_Set_Cursor(char c, char f){
	char temp, talto, tbajo;
	if(f==1){
		temp = 0x80 + c -1;
		/*talto = (temp & 0xF0)>>4;
		tbajo = temp & 0x0F;*/
		LCD_CMD(temp);
	}else if(f==2){
		temp = 0xC0 + c -1;
		LCD_CMD(temp);
	}
}

