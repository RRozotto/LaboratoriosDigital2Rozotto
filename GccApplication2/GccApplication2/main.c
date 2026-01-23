/*
 * GccApplication2.c
 *
 * Created: 1/15/2026 7:10:18 PM
 * Author : rodri
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "LibreriaLab1.h"

const uint8_t hex_digits[16] = { 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70,0x7F, 0x73, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47 };
uint8_t Leds;
uint8_t PrevState;
uint8_t Contar;
uint8_t Modo;
uint8_t ConteoSeg;
uint8_t currentState;

int main(void)
{
	setup();

    while (1) {
	switch(Modo){
		
		case 0: //Esperando
		bloquearbotones(0);
		currentState = PINB; // Leer estado actual de los pines

		// Detectar botones en flanco de bajada
		if ((PrevState & (1 << PINB2)) && !(currentState & (1 << PINB2))) {
			Modo=1;
			conteoacinco();
		break;
		
		case 1: //Contando
		ConteoSeg = segundos();
		MultiplexDos( (hex_digits[ConteoSeg]),Leds);
		if(ConteoSeg==0){
			Modo = 2;
		}
		break;
		
		case 2:
		bloquearbotones(1);
		PCICR  = (1 << PCIE0);
		PCMSK0 = (1 << PCINT0) | (1 << PCINT1); //PB0 y PB1
		Leds =  ledscarrera();
		MultiplexDos( (hex_digits[ConteoSeg]),Leds);
		if (Leds & 0x80) {// Aquí entrás si Leds es 1000xxxx
			Modo = 3;
		}
		Leds =  ledscarrera();
		if (Leds & 0x08) {// Aquí entrás si Leds es 1000xxxx
			Modo = 4;
		}
		break;
		
		case 3: //gana 1
		bloquearbotones(0);
		MultiplexDos( (hex_digits[1]),0b11110000);
		currentState = PINB; // Leer estado actual de los pines

		// Detectar botones en flanco de bajada
		if ((PrevState & (1 << PINB2)) && !(currentState & (1 << PINB2))) {
			Modo=1;
			conteoacinco();}
		break;
		
		case 4: //gana 2
		bloquearbotones(0);
		MultiplexDos( (hex_digits[2]),0b00001111);
		currentState = PINB; // Leer estado actual de los pines

		// Detectar botones en flanco de bajada
		if ((PrevState & (1 << PINB2)) && !(currentState & (1 << PINB2))) {
			Modo=1;
			conteoacinco();}
		break;
		
		
	}
	PrevState = currentState;
	}
	/*int ConteoSeg = segundos();
	Leds =  ledscarrera();
	MultiplexDos( (hex_digits[ConteoSeg]),Leds);
	
	if(Contar==0){
		uint8_t salida;
		salida = (ConteoBo1 << 4) | (ConteoBo2 & 0x0F);
	PORTD = salida;}*/
	
    }
}
