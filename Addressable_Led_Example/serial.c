/*******************************

serial.c

Basic implementation of hardware UART using an ATmega328p microcontroller

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

********************************/

#include <stdio.h>

void UARTinit(void);
static int UARTPutChar(char ch, FILE *stream);
static int UARTGetChar(FILE *stream);
void UARTSendByte(uint8_t);

static FILE mystdout = FDEV_SETUP_STREAM(UARTPutChar, UARTGetChar, _FDEV_SETUP_RW);	/* Assign I/O stream to UART */


void UARTinit(void) {

	UCSR0A = 0x00;			
	UCSR0B = 0x18;			// enable UART TX and RX
	UCSR0C = 0x06;			// set the UART for N, 8, 1
	UBRR0L = 8;				// set BAUD Rate for 115200 with 16MHz clock
	UBRR0H = 0;

	stdout = &mystdout;		// define the output stream
	stdin = &mystdout;		// define the input stream	

}


// the following function sends a single character out of the serial port
static int UARTPutChar(char ch, FILE *stream) {

    while ( (UCSR0A & (1 << UDRE0)) == 0 )		// wait until there is room in the transmit buffer
	{
	}

    UDR0 = ch;									// load the character into the UART data register

    return 0;
}

void UARTSendByte(uint8_t send_byte) {
	while ((UCSR0A & (1 << UDRE0)) == 0)
	{		
	}
	
	UDR0 = send_byte;
}

// the following function waits for a serial character to be received
static int UARTGetChar(FILE *stream) {
   unsigned char ch;   							// create a variable to hold the received value
   
   while ( (UCSR0A & (1<<RXC0)) == 0 )			// wait until the received character flag is set
   {
   }
   
   ch=UDR0;  									// load the received character into the local variable

   return ch;									// return the value
}

