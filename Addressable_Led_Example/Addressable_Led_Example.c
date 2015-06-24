/*
 * Addressable_Led_Example.c
 *
 * Created: 6/24/2015 5:36:15 PM
 *  Author: John
 */ 

#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include "serial.c"
#include "spi.h"

int main(void) {
	
	SPIinit();
	UARTinit();
	
    while(1) {
        //TODO:: Please write your application code 
    }
}