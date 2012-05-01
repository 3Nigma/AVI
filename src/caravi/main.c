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

#include "datatypes.h"
#include "uart.h"
#include "cams.h"
#include "motors.h"
#include "rf12.h"

int main(void) {
  AtomicCommand pcCom;
  uint8_t optCamId = 0;
  uint8_t camRetVal = 0;
  uint8_t comholder[MAX_COMMAND_PAYLOAD];
  uint8_t respholder[MAX_COMMAND_PAYLOAD];
  uint8_t *compt = NULL;
  uint8_t *resppt = NULL;
  uint8_t respLength = 0;

  pcCom.data = comholder;

  /* initialize system modules */
  //initMotors();
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

  /* set STATUS port to output */ 
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
	
  uart_putstr("All system initialized, AviCar online!");

  while(1) {
  rf12_rxdata(&pcCom);

  compt = pcCom.data;
  resppt = respholder;
  respLength = 0;
  while(compt != (pcCom.data + pcCom.length)) {
    switch(compt[0] & CAR_SUB_COMM_MASK) {
    case CAR_CAMS_SUB: /* resolve a cam command */
      switch(compt[0] & CAR_CAMS_ID_MASK) {
      case CAR_CAMS_ID_TOP:
        optCamId = CAM1;
        break;
      case CAR_CAMS_ID_MID:
        optCamId = CAM0;
        break;
      case CAR_CAMS_ID_ALL:
        break;
      default:
        /* it shouldn't reach this point */
        break;
      }
      switch(compt[0] & CAR_CAMS_FUNC_MASK) {
      case CARS_CAMS_FUNC_SOFTRESET:
        softResetCam(optCamId);
        resppt[0] = 0x01;
        respLength += 1;
        resppt += 1;
        compt += 1;
        break;
      case CARS_CAMS_FUNC_PWRDOWN:
       pwrDownCam(optCamId);
       resppt[0] = 0x01;
       respLength += 1;
       resppt += 1;
       compt += 1;
       break;
      case CARS_CAMS_FUNC_FORCEON:
        forceAwakeCam(optCamId);
        resppt[0] = 0x01;
        respLength += 1;
        resppt += 1;
        compt += 1;
        break;
      case CARS_CAMS_FUNC_GETMAXPIXEL:
        camRetVal = getMaxPixel(optCamId);
        resppt[0] = camRetVal;
        respLength += 1;
        resppt += 1;
        compt += 1;
        break;
      case CARS_CAMS_FUNC_GETMINPIXEL:
        camRetVal = getMinPixel(optCamId);
        resppt[0] = camRetVal;
        respLength += 1;
        resppt += 1;
        compt += 1;
        break;
      case CARS_CAMS_FUNC_GETSQUAL:
        camRetVal = getSqual(optCamId);
        resppt[0] = camRetVal;
        resppt += 1;
        compt += 1;
        break;
      case CARS_CAMS_FUNC_GETPIXELSUM:
        camRetVal = getPixelSum(optCamId);
        resppt[0] = camRetVal;
        respLength += 1;
        resppt += 1;
        compt += 1;
        break;
      case CARS_CAMS_FUNC_GETXMOVEMENT:
        camRetVal = getXMovement(optCamId);
        resppt[0] = camRetVal;
        respLength += 1;
        resppt += 1;
        compt += 1;
        break;
      case CARS_CAMS_FUNC_GETYMOVEMENT:
        camRetVal = getYMovement(optCamId);
        resppt[0] = camRetVal;
        respLength += 1;
        resppt += 1;
        compt += 1;
        break;
      case CARS_CAMS_FUNC_SETCAMSTEP:
        setPixelStep(optCamId, compt[1]);
        resppt[0] = 0x01;
        respLength += 1;
        resppt += 1;
        compt += 2;
        break;
      case CARS_CAMS_FUNC_GETPIXELS:
        requirePixels(optCamId);
        resppt = getPixelBuffer(optCamId);
        respLength += CAM_PIX_BYTECNT;
        compt += 1;
        break;
      default:
        /* it shouldn't reach this point */
        break;
      }
      break;
    case CAR_MOTS_SUB:
      break;
    default:
      break;
    }
  }

  /* reply to demander */
  pcCom.data = respholder;
  pcCom.length = respLength;
  rf12_txdata(&pcCom);
  }
}
