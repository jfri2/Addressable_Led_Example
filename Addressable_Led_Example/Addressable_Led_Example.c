/*
Addressable_Led_Example.c

Example program for new AdaFruit serial RGB LED strips.

To use:
	- In spi.h, change #define macros to match your ports and pin numbers
	- In Addressable_Led_Example.c, define your F_CPU and desired global brightness anywhere from 0b00000 to 0b11111 (this is a multiplier of the brightness you set for each LED).
	- In Addressable_Led_Example.c, define the total number of LEDS in your strip. 

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
 */ 

#define F_CPU 16000000L
#define LEDS 144

#include <avr/io.h>
#include <util/delay.h>
#include "serial.c"
#include "spi.h"

uint8_t BRIGHTNESS = 0xFF;

void sendStartFrame(void);
void sendStopFrame(void);
void sendLedColorFrame(uint8_t, uint8_t, uint8_t);
void sendLedSelectFrame(uint32_t);
void setLED(uint32_t, uint8_t, uint8_t, uint8_t);

int main(void) {
	
	SPIinit();
	UARTinit();
	BRIGHTNESS |= 0b11100000;		//makes sure first frame is setup correctly
	
    while(1) {
		
		///Loops to display demo white chase sequence
        for(int i = 0; i < LEDS; i++) {
			setLED(i,100,100,100);
			_delay_ms(50);
		}
		for(int i = LEDS; i > 0; i--) {
			setLED(i,100,100,100);
			_delay_ms(50);
		}
    }
}

void sendStartFrame(void) {	
	for(uint8_t i = 0; i < 4; i++) {
		SPIxchng(0);
	}
}

void sendStopFrame(void) {
	for(uint8_t i = 0; i < 4; i++) {
		SPIxchng(0xFF);
	}
}

void sendLedColorFrame(uint8_t red, uint8_t green, uint8_t blue) {
	SPIxchng(BRIGHTNESS);
	SPIxchng(blue);
	SPIxchng(green);
	SPIxchng(red);
}

void sendLedSelectFrame(uint32_t ledNumber) {
	uint8_t selectFrame[4];
	
	selectFrame[0] = (ledNumber >> 24) & 0xFF;
	selectFrame[1] = (ledNumber >> 16) & 0xFF;
	selectFrame[2] = (ledNumber >> 8) & 0xFF;
	selectFrame[3] = ledNumber & 0xFF;
	
	for(uint8_t i = 0; i < 4; i++) {
		SPIxchng(selectFrame[i]);
	}
}

void setLED(uint32_t ledNumber, uint8_t red, uint8_t green, uint8_t blue) {
	SPI_START();	
	sendStartFrame();
	sendLedColorFrame(red, green, blue);
	sendLedSelectFrame(ledNumber);
	sendStopFrame();	
	SPI_END();
}