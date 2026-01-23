/*
 * LibreriaLab1.h
 *
 * Created: 1/22/2026 2:04:25 PM
 *  Author: rodri
 */ 


#ifndef LIBRERIALAB1_H_
#define LIBRERIALAB1_H_
#include <avr/io.h>

int segundos();

void setup();

void conteoacinco();

void bloquearbotones(uint8_t activar1);

int MultiplexDos(uint8_t salida1,uint8_t  salida2);

int ledscarrera();




#endif /* LIBRERIALAB1_H_ */