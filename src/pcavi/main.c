/* main.c - The main module of the PC link RF connection
 *
 * Copyright (c) 2012-*, Victor ADASCALITEI <adascalitei dot victor at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of AVI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "rf12.h"
#include "uart.h"

#define RX_LED PD5
#define TX_LED PD6

int main(void)
{
	uint8_t msg[] = "Test";
	
	uart_init();
	uart_putstr ("Hello");
	
	DDRD |= _BV(RX_LED) | _BV(TX_LED);
	
	/* set nRES pin of RFM12 HIGH */
	DDRD |= _BV(PD3);
	PORTD &= ~_BV(PD3);
	_delay_ms(10);
	PORTD |= _BV(PD3);
	_delay_ms(100);
	
	initRF();
	
    while(1)
    {
		rf12_txdata(msg, sizeof(msg));
		PORTD ^= _BV(TX_LED);	
		_delay_ms(1000);	
	}
}

/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "global.h"
#include "rf12.h"     

#define F_CPU 10000000UL
#include <util/delay.h>

void send(void);
void receive(void);

int main(void)
{
	rf12_init();					// ein paar Register setzen (z.B. CLK auf 10MHz)
	rf12_setfreq(RF12FREQ(433.92));	// Sende/Empfangsfrequenz auf 433,92MHz einstellen
	rf12_setbandwidth(4, 1, 4);		// 200kHz Bandbreite, -6dB Verstärkung, DRSSI threshold: -79dBm 
	rf12_setbaud(19200);			// 19200 baud
	rf12_setpower(0, 6);			// 1mW Ausgangangsleistung, 120kHz Frequenzshift

	for (;;)
	{
		send();
		for (unsigned char i=0; i<100; i++)
			_delay_ms(10);

	//	receive();
	}
}

void receive(void)
{	unsigned char test[32];	
	rf12_rxdata(test,32);	
	// daten verarbeiten
}

void send(void)
{	unsigned char test[]="Dies ist ein 433MHz Test !!!\n   ";	
	rf12_txdata(test,32);
}

*/
