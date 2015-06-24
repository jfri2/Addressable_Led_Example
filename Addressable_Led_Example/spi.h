/**

spi.h

Basic implementation of hardware SPI using an ATmega328p microcontroller

Functions:

void SPIinit() 	: Sets hardware SPI up as master, SCK = clk/16
SPI_START()		: Sets CS active low for SPI transmission
SPI_END()		: Sets CS high to end SPI transmission
uint8_t SPIxchng(uint8_t send_data) : Exchange one byte of data over the spi bus


----

Copyright (c) 2015 John S Fritz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

**/

#include <avr/io.h>

/// Pin Assignments
#define SCK 	5
#define MOSI 	3
#define MISO 	4
#define CS 		2

/// Port Assignments
#define SPIDDR 		DDRB
#define CSDDR 		DDRB
#define SPIPORT 	PORTB
#define CSPORT 		PORTB

/// Spi enable and disable 'functions'
#define SPI_START();	CSPORT &= ~(1<<CS);
#define SPI_END();		CSPORT |= (1<<CS);


void SPIinit() {
	SPIDDR |= (1<<MOSI) | (1<<SCK);				//sets MOSI and SCK pins as outputs
	SPIDDR &= ~(1<<MISO);						//sets MISO to input
	SPIPORT |= ((1<<SCK) | (1<<MOSI));			//enables pullups on SCK and MOSI pins
	SPIPORT &= ~(1<<MISO);						//disables pullup resistors on MISO
	CSDDR |= (1<<CS);							//sets CS pin to output
	CSPORT |= (1<<CS);							//enables pullups on CS pin
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR1);	//set SPCR regiseter to enable spi, master, prescalar = clk/16
}

uint8_t SPIxchng(uint8_t send_data) {
	uint8_t return_data = 0;

	//load data into register
	SPDR = send_data;
	//Do nothing while transmission is going on
	while(!(SPSR & (1<<SPIF)));			

	return(return_data);
}


