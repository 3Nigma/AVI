/* rf12.c - The implementation of the RF link module
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

#include "rf12.h"

uint16_t rf12_wrt_cmd(uint16_t cmd) {
  uint8_t cnt;
  uint16_t toRet = 0;
  
  RF_PORT &= ~_BV(SCK);
  RF_PORT &= ~_BV(CS);
  
  for(cnt = 0; cnt < 16; ++ cnt){
    if(cmd & 0x8000) RF_PORT |= _BV(SDI);
    else RF_PORT &= ~_BV(SDI);
    RF_PORT |= _BV(SCK);
    toRet <<= 1;
    if(RF_PIN & _BV(SDO)) toRet |= 0x0001;
    cmd <<= 1;
    RF_PORT &= ~_BV(SCK);
  }
  
  RF_PORT &= ~_BV(SCK);
  RF_PORT |= _BV(CS);
  
  return toRet;
}

void initRF(void) {
  /* initialize rf port connections */
  RF_PORT |= _BV(CS) | _BV(SDI);
  RF_PORT &= ~_BV(SCK);
  RF_DDR |=_BV(SDI) | _BV(SCK) | _BV(CS);
  RF_DDR &= ~_BV(SDO);
  RF_INT_DDR &= ~_BV(NIRQ);
  
  _delay_ms(100);			/* wait until POR done */
  
  rf12_wrt_cmd(0x0000);	 /* fix nIRQ generating bug */  
  rf12_wrt_cmd(0x80E7);  /* EL,EF,12.0pF */
  rf12_wrt_cmd(0xA640);  /* 868MHz */
  rf12_wrt_cmd(0xC647);  /* 4.8Kbps (38.4: 8, 19.2: 11, 9.6: 23, 4.8: 47) */
  rf12_wrt_cmd(0x94A0);  /* VDI,FAST,134kHz,0dBm,-103dBm */
  rf12_wrt_cmd(0xC2AC);  /* AL,!ml,DIG,DQD4 */
  rf12_wrt_cmd(0xCA83);  /* FIFO8,SYNC,!ff,DR */
  rf12_wrt_cmd(0xCED4);  /* SYNC=2DXX? */
  rf12_wrt_cmd(0xC483);  /* @PWR,NO RSTRIC,!st,!fi,OE,EN */
  rf12_wrt_cmd(0x9850);  /* !mp,90kHz,MAX OUT */ 
  rf12_wrt_cmd(0xCC77);  /* OB1?OB0, LPX,?ddy?DDIT?BW0 */
  rf12_wrt_cmd(0xE000);  /* NOT USE */
  rf12_wrt_cmd(0xC800);  /* NOT USE */
  rf12_wrt_cmd(0xC040);  /* 1.66MHz,2.2V */
  rf12_wrt_cmd(0x8209);  /* enable xtal, disable clk pin */
	
  /* configure RF device interrupt */
  //MCUCR |= _BV(ISC01);  /* raise an interrupt on falling edge of INT0 */
  //MCUCR &= ~_BV(ISC00);
  //GIMSK |= _BV(INT0);  /* enable external interrupt on INT0 */
}

ISR(INT0_vect) {
  uint16_t rfStatus = rf12_wrt_cmd(0x0000);  /* read the status of the RF device */
  uint16_t recvDblChr;
	
  if(rfStatus & 0x1000) {
    rf12_wrt_cmd(0x8201);
    rf12_wrt_cmd(0x8203);
  }
  if(rfStatus & 0x8000)	{  /* TX register ready to receive the next byte OR receiver got to the preprogrammed byte count */
    PORTD ^= _BV(PD6);
    rf12_wrt_cmd(0xB800 + 0x55);
  }
}

void rf12_send_char(unsigned char toSend) {
  while(RF_INT_PIN & _BV(NIRQ));  /* wait for previous TX to finish */
  rf12_wrt_cmd(0xB800 + toSend);
}

uint8_t rf12_recv_char(void) {
  uint16_t recvDblChr;
  
  while(RF_INT_PIN & _BV(NIRQ));
  rf12_wrt_cmd(0x0000);
  recvDblChr = rf12_wrt_cmd(0xB000);
  return (recvDblChr & 0x00FF);
}

void rf12_txdata(uint8_t *data, uint8_t length) {		
  uint8_t cnt;
  uint8_t checksum = 0;
	
  rf12_wrt_cmd(0x0000);  /* read status register */
  //rf12_wrt_cmd(0xB8AA);
  rf12_wrt_cmd(0xB8AA);
  rf12_wrt_cmd(0x8239);  /* !ER, !EBB, ET, ES, EX, !EB, EW, DC */
  /* send preamble */
  rf12_send_char(0xAA);
  rf12_send_char(0xAA);
  rf12_send_char(0x2D);  /* send SYNC High byte */
  rf12_send_char(0xD4);  /* send SYNC Low byte */
	
  /* send useful data */
  rf12_send_char('G');  /* send data length byte */
  rf12_send_char('A');
  rf12_send_char('G');
  rf12_send_char('U');
  rf12_send_char('T');
  rf12_send_char('A');
  rf12_send_char('T');
  rf12_send_char('I');
  /*for(cnt = 0; cnt < length; ++ cnt) {
    rf12_send_char(data[cnt]);
    checksum += data[cnt];
    }*/
  //rf12_send_char(checksum);  /* send raw checksum byte */
	
  /* finish TX session */
  rf12_send_char(0xAA);
  rf12_send_char(0xAA);
  rf12_wrt_cmd(0x8209);  /* !ER, !EBB, ET, ES, EX, !EB, EW, DC */
	
  _delay_ms(10);
}

void rf12_rxdata(uint8_t *data, uint8_t length)  {	
  uint8_t i;
	
  rf12_wrt_cmd(0x82C9);  /* RX on */
  rf12_wrt_cmd(0xCA81);  /* set FIFO mode */
  rf12_wrt_cmd(0xCA83);	 /* enable FIFO */
  for (i = 0; i < length; ++i) {	
    *data++ = rf12_recv_char();
  }
  
  rf12_wrt_cmd(0x8209);  /* RX off */
}

