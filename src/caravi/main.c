/* main.c - The main application module that drives the AVI car
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
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "cams.h"
#include "motors.h"
#include "rf12.h"

int main(void)
{
	uint8_t *recvData;
	uint8_t optMod = 0;
	uint8_t optCamId = 0;
	uint8_t optFeat = 0;
	uint8_t motorSpeed = 0;
	uint16_t rfWrtResponse = 0;
	char rxMsg[12];
	
	/* initialize system modules */
	//initMotors();
	//initSerial();
	uart_init();
	initCams();
	
	/* set nRES on RFM12 HIGH */
	DDRB |= _BV(PB2);
	PORTB &= ~_BV(PB2);
	_delay_ms(10);
	PORTB |= _BV(PB2);
	_delay_ms(100);
	
	initRF();
	sei();
	
	//DDRB &= ~_BV(PB0);
	//while(PINB & _BV(PB0)) { }
	
	// set STATUS port to output 
	DDRA |= 0xFF;
	PORTA = 0x00;
	
	/*
	_delay_ms(2000);
	setMotorDirection(ALL_MOTORS, FORWARD);
	setMotorSpeed(ALL_MOTORS, 50);
	_delay_ms(1000);
	stopAllMotors();
	
	while(1){
		PORTA = 0b10101010;
		_delay_ms(500);
		PORTA = 0b01010101;
		_delay_ms(500);
	}*/
	
	pwrDownCam(CAM0);
	pwrDownCam(CAM1);
	/* force cams LED on */
	//forceAwakeCam(CAM0);
	//forceAwakeCam(CAM1);
	
	//uart_putstr("All system initialized, MCU online!");
	while(1)
	{
		//rf12_rxdata((uint8_t *)rxMsg, 8);
		//uart_putstr(rxMsg);
		/*if(rfm12_rx_status() == STATUS_COMPLETE)
		{
			PORTA |= 0x01;
			if(rfm12_rx_type() == 0xEE) PORTA |= 0x02;
			if(rfm12_rx_len() == 4)
			{
				PORTA |= 0x04;
				recvData = rfm12_rx_buffer();
				if(recvData[0] == 'T' || recvData[0] == 't') PORTA |= 0x08;
			}			
		}
				
		rfm12_tick();
		*/
		//rfWrtResponse = rf12_wrt_cmd(0x0000);
		//uart_putc( rfWrtResponse >> 8 );
		//uart_putc( rfWrtResponse & 0xFF );
		
		//optMod = getchar();
		//switch(optMod)
		//{
			//case 'M': /* cams module */
				//optCamId = getchar() - '0';
				//optFeat = getchar();
				//switch(optFeat)
				//{
					//case '0':
						//pwrDownCam(optCamId);
						//break;
					//case '1':
						//softResetCam(optCamId);
						//break;
					//case '2':
						//forceAwakeCam(optCamId);
						//break;
					//default:
						//printf("M = %3d, m = %3d, s = %3d\n", getMaxPixel(optCamId), getMinPixel(optCamId), getPixelSum(optCamId));
						//requirePixels(optCamId);
						//break;
				//}
				//break;
			//default:
				///* it shouldn't reach this point */
				//break;
		//}
	}
}
