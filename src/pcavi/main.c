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
#include "datatypes.h"

AtomicResponse aquireUARTCommand(AtomicCommand *muc) {
  uint8_t localCrc = 0;
  uint8_t id;

  /* aquire packet length */
  muc->length = uart_getbyte();
  if(muc->length > (sizeof(muc->data)/sizeof(muc->data[0]))) return INVALID_LENGTH;

  if(0 == muc->length) {
    /* PC wants a meta command */
    muc->type = META_COMMAND; 
    muc->length = 1;
  } else {
    /* PC wants an ordinary car command */
    muc->type = CAR_COMMAND;
  }

  /* get payload + CRC */
  for(id = 0; id < muc->length; ++id) {
    muc->data[id] = uart_getbyte();
    localCrc += muc->data[id];
  }
  muc->crc = uart_getbyte();

  if(localCrc != muc->crc) return INVALID_CRC;

  return ALL_OK;
}

void sendUARTResponse(AtomicResponse comStatus) {
  uart_sendbyte(comStatus);
}

void sendUARTCarResponse(AtomicCommand *muc) {
  uint8_t id;
  uint8_t uartSendAttempt = 0;
  AtomicResponse clientResponse;

  do {
    uart_sendbyte(muc->length);
    for(id = 0; id < muc->length; ++id) {
      uart_sendbyte(muc->data[id]);
    }
    uart_sendbyte(muc->crc);

    clientResponse = uart_getbyte();
  } while(clientResponse != ALL_OK && uartSendAttempt++ < UART_MAXTX_ATTEMPTS);
  
  return clientResponse;
}

int main(void) {
  AtomicResponse comStatus;
  AtomicCommand com;

  initRF();
  uart_init();

  while(1) {
    comStatus = aquireUARTCommand(&com);
    sendUARTResponse(comStatus);
    if(comStatus == ALL_OK) {
      /* the received uart command was valid, handle it */
      switch(com.type) {
      case META_COMMAND:
        switch(com.data[0]) {
        case META_ONLINE_REQUEST:
          uart_sendbyte(META_ONLINE_RESPONSE);
          break;
        default:
          /* it shouldn't reach this point */ 
          break;
        }
        break;
      case CAR_COMMAND:
        sendUARTResponse(rf12_txdata(&com));
        sendUARTResponse(rf12_rxdata(&com));
        sendUARTCarResponse(&com);
        break; 
      default:
        /* it shouldn't reach this point */
        break;
      }
    }	
  }
}
