/*
 * Laboratorio2.c
 *
 * Created: 1/28/2026 10:53:12 PM
 * Author : rodri
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

uint16_t contadorS3;

#include "LibLCD.h"
int main(void)
{
	setup();
	LCD_Set_Cursor(1,1);
	LCD_Write_Char('H');
	LCD_Write_Char('o');
	LCD_Write_Char('l');
	LCD_Write_Char('a');
	LCD_Set_Cursor(1,2);
	LCD_Write_String("Mundo digital 2");
    /* Replace with your application code */
    while (1) 
    {
	LCD_CMD(0x01);
	
	LCD_Set_Cursor(1,1);
	LCD_Write_String("S1:");
	uint16_t adc = LecturaADC(2);

	// Voltaje * 100 (0–500)
	uint16_t volt = (adc * 500UL) / 1023;

	// Separar parte entera y decimal
	uint8_t v_int = volt / 100;
	uint8_t v_dec = volt % 100;

	char buffer[8];

	LCD_Set_Cursor(4,1);
	itoa(v_int, buffer, 10);
	LCD_Write_String(buffer);
	LCD_Write_Char('.');
	if (v_dec < 10) LCD_Write_Char('0'); // cero a la izquierda
	itoa(v_dec, buffer, 10);
	LCD_Write_String(buffer);
	LCD_Write_Char('V');
	
	LCD_Set_Cursor(1,2);
	LCD_Write_String("S2:");
	LCD_Set_Cursor(4,2);
	uint16_t valor2 = LecturaADC(3);
	char buffer2[6];   // máx 65535 + '\0'
	itoa(valor2, buffer2, 10);
	LCD_Write_String(buffer2);
	

	if(rx_char=='+'){
		contadorS3++;
		rx_char='0';
	}else if(rx_char=='-'){
		contadorS3--;
		rx_char='0';}
	
	LCD_Set_Cursor(9,2);
	LCD_Write_String("S3:");
	LCD_Set_Cursor(12,2);
	char buffer3[6];   // máx 65535 + '\0'
	itoa(contadorS3, buffer3, 10);
	LCD_Write_String(buffer3);
	
	adc = LecturaADC(2);
	volt = (adc * 500UL) / 1023;
	v_int = volt / 100;
	v_dec = volt % 100;
	itoa(v_int, buffer, 10);
	
	USART_TX_String("S1=");
	USART_TX_String(buffer);
	USART_TX_Char('.');
	if (v_dec < 10) USART_TX_Char('0'); 
	itoa(v_dec, buffer, 10);
	USART_TX_String(buffer);
	USART_TX_Char('V');
	
	USART_TX_String("  S2=");
	USART_TX_Uint(valor2);
	USART_TX_String("  S3=");
	USART_TX_Uint(contadorS3);
	USART_TX_String("\r\n");
	//_delay_ms(100);
    }
}

