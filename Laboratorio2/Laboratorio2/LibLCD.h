/*
 * LibLCD.h
 *
 * Created: 1/28/2026 11:02:29 PM
 *  Author: rodri
 */ 


#ifndef LIBLCD_H_
#define LIBLCD_H_

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>

extern volatile char rx_char;
extern volatile uint8_t rx_flag;

void setup(void);

void USART_TX_Char(char c);

void USART_TX_String(const char *s);

void USART_TX_Uint(uint16_t num);

char UART_ReceiveChar(void);

void initLCD8bits(void);//Inicializar LCD para usar con 8 bits

uint16_t LecturaADC(uint8_t PIN);

void LCD_Port(char a);//Cargar un valor al puerto del LCD

void LCD_CMD(char a);//Cargar un comando al LCD

void LCD_Write_Char(char c);//Enviarle un caracter al LCD

void LCD_Write_String(char *a);//Enviarle un string al LCD

void LCD_Shift_Right(void);//Desplaza a la derecha

void LCD_Shift_Left(void);//Desplaza a la izquierda

void LCD_Set_Cursor(char c, char f);//Establecer el cursor

#endif /* LIBLCD_H_ */