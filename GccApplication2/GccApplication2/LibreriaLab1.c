#include "LibreriaLab1.h"
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t Multi;
uint8_t ConteoSec1;
uint8_t ConteoSec2;
uint8_t Multi1;
volatile uint8_t ConteoBo1 = 0x00; 
volatile uint8_t ConteoBo2 = 0x00; 
uint8_t PrevState;
uint8_t activar;
volatile uint8_t btnB0 = 1;
volatile uint8_t btnB1 = 1;

void setup(){
	cli(); // Desactivar interrupciones globales
	DDRD  = 0xFF; // Puerto D como salida
	PORTD = 0x00;
	DDRC  = 0xFF; // Puerto C como salida
	PORTC = 0x00;
	DDRB  = 0x00; // Puerto B como entrada
	PORTB = 0xFF; // Pull-ups activados
	
	// Configuración de interrupciones de pines
	/*PCICR  = (1 << PCIE0);
	PCMSK0 = (1 << PCINT0) | (1 << PCINT1); //PB0 y PB1*/
	
	// Configuración del Timer 0
	TCCR0A  = 0x00;
	TCCR0B  = (1 << CS02)| (1 << CS00);
	TCNT0   = 100;
	TIMSK0  = (1 << TOIE0);
	
	sei(); // Habilitar interrupciones globales
}

int ledscarrera(){
	return (ConteoBo1 << 4) | (ConteoBo2 & 0x0F);
}

void bloquearbotones(uint8_t activar1){
	activar = activar1;
	if (activar==0){
		ConteoBo2=0;
		ConteoBo1=0;
	}
}
void conteoacinco(){
	ConteoSec2 = 5;
}
int segundos(){
	return ConteoSec2;
}

int MultiplexDos(uint8_t salida1,uint8_t  salida2){
	switch (Multi) //Switch case para realizar la multiplexacion
	{
		case 0:
		PORTD = 0b00000000;
		PORTC |= (1 << PC0);
		PORTC &= ~(1 << PC1);
		PORTD = salida1;
		break;
		case 1:
		PORTD = 0b00000000;
		PORTC &= ~(1 << PC0);
		PORTC |= (1 << PC1);
		PORTD = salida2;
		break;
	}
}


ISR(PCINT0_vect)
{
	// Nada aquí a propósito
}

// ---------------- ISR TIMER0 ----------------
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 100;

	// -------- MULTIPLEXADO --------
	Multi1++;
	if (Multi1 >= 1) {
		Multi1 = 0;
		Multi++;
		if (Multi >= 2) Multi = 0;
	}

	// -------- SEGUNDOS --------
	ConteoSec1++;
	if (ConteoSec1 >= 100) {
		ConteoSec1 = 0;
		ConteoSec2--;
		if (ConteoSec2 == 255) ConteoSec2 = 5;
	}

	// -------- ANTIRREBOTE --------
	static uint8_t cntB0 = 0;
	static uint8_t cntB1 = 0;

	uint8_t estado = PINB;

	// ---- BOTÓN PB0 (D8) ----
	if (!(estado & (1 << PB0)) && (activar==1)) {   // presionado
		if (cntB0 < 5) cntB0++;
		else if (btnB0) {
			btnB0 = 0;
			if (ConteoBo1 == 0) {
			ConteoBo1 = 0x01;      // primer 1
			} else {
				ConteoBo1 <<= 1;
				if (ConteoBo1 > 0x08) {
					ConteoBo1 = 0x00;
				}
			}
		}
	} else {
		cntB0 = 0;
		btnB0 = 1;
	}

	// ---- BOTÓN PB1 (D9) ----
	if (!(estado & (1 << PB1)) && (activar==1)) {
		if (cntB1 < 5) cntB1++;
		else if (btnB1) {
			btnB1 = 0;
			if (ConteoBo2 == 0) {
			ConteoBo2 = 0x01;      // primer 1
			} else {
				ConteoBo2 <<= 1;
				if (ConteoBo2 > 0x08) {
					ConteoBo2 = 0x00;
				}
			}
		}
	} else {
		cntB1 = 0;
		btnB1 = 1;
	}
}
