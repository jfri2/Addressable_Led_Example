/*
 * Addressable_Led_Example.c
 *
 * Created: 6/24/2015 5:36:15 PM
 *  Author: John
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