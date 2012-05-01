/* cams.c - A ADNS Mouse Cam Controller Interface
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

#include "cams.h"

static uint8_t camPixBuf[2][CAM_PIX_BYTECNT];
static uint8_t camSteps[2];

void initCams(void){
  /* configure the serial com-cam pins */
  SET_PIN_OUTPUT(CAM_PDIR, CAM0);
  SET_PIN_OUTPUT(CAM_PDIR, CAM1);
  
  SET_PIN_DATA(CAM_RPORT, CAM0);
  SET_PIN_DATA(CAM_RPORT, CAM1);
  _delay_us(5);
  RESET_PIN_DATA(CAM_RPORT, CAM0);
  RESET_PIN_DATA(CAM_RPORT, CAM1);
  _delay_us(5);
  SET_PIN_DATA(CAM_RPORT, CAM0);
  SET_PIN_DATA(CAM_RPORT, CAM1);
  
  _delay_ms(90); /* t_{SPTT} = minim 90ms @ page 7, 17 */
  _delay_ms(4);	 /* "t_{compute} = minim 3.1ms Data Delay after PD deactivated" @ page 6 */

  /* set default pixel steps */
  camSteps[0] = 64;
  camSteps[1] = 64;
}

uint8_t readCamReg(uint8_t camId, uint8_t address) {
  uint8_t response = 0;
  uint8_t cnt = 0;
  
  SET_PIN_OUTPUT(CAM_PDIR, CAM_DATAPIN);
  
  for (cnt = 128; cnt > 0 ; cnt >>= 1) {
    RESET_PIN_DATA(CAM_RPORT, camId);
    if((address & cnt) != 0)
      SET_PIN_DATA(CAM_RPORT, CAM_DATAPIN);
    else
      RESET_PIN_DATA(CAM_RPORT, CAM_DATAPIN);
    SET_PIN_DATA(CAM_RPORT, camId);
  }
  
  SET_PIN_INPUT(CAM_PDIR, CAM_DATAPIN);
  //_delay_us(0); /* TODO: Watch this! Tweaked by ear! Data sheet says it should be [tHOLD = 100us] min. */
  /* 100 - ok value
     90  - ok value
     75  - ok value
     50  - ok value
     10  - ok value
     5   - ok value
     0   - ok value */
  for (cnt = 128; cnt > 0 ; cnt >>= 1) {
    RESET_PIN_DATA(CAM_RPORT, camId);
    SET_PIN_DATA(CAM_RPORT, camId);
    if(bit_is_set(CAM_WPORT, CAM_DATAPIN))
      response |= cnt;
  }
  
  return response;
}

void writeCamReg(uint8_t camId, uint8_t address, uint8_t data){
  uint8_t cnt = 0;
  
  SET_PIN_OUTPUT(CAM_PDIR, CAM_DATAPIN);
  
  for (cnt = 128; cnt > 0 ; cnt >>= 1) {
    RESET_PIN_DATA(CAM_RPORT, camId);
    if((address & cnt) != 0)
      SET_PIN_DATA(CAM_RPORT, CAM_DATAPIN);
    else
      RESET_PIN_DATA(CAM_RPORT, CAM_DATAPIN);
    SET_PIN_DATA(CAM_RPORT, camId);
  }
  
  for (cnt = 128; cnt > 0 ; cnt >>= 1) {
    RESET_PIN_DATA(CAM_RPORT, camId);
    if((data & cnt) != 0)
      SET_PIN_DATA(CAM_RPORT, CAM_DATAPIN);
    else
      RESET_PIN_DATA(CAM_RPORT, CAM_DATAPIN);
    SET_PIN_DATA(CAM_RPORT, camId);
  }
  
  //_delay_us(0); /* TODO: Watch this! tSWW, tSWR = 100us min. */
  /* 100 - ok value
     50  - ok value
     10  - ok value
     5   - ok value
     0   - ok value */
}

inline void softResetCam(uint8_t camId){
  uint8_t currentConf = readCamReg(camId, RCONFIG) | CONFIG_RESET;
  writeCamReg(camId, WCONFIG, currentConf);
}
inline void pwrDownCam(uint8_t camId){
  uint8_t currentConf = readCamReg(camId, RCONFIG) | CONFIG_PWRDOWN;
  writeCamReg(camId, WCONFIG, currentConf);
}
inline void forceAwakeCam(uint8_t camId){
  uint8_t currentConf = readCamReg(camId, RCONFIG) | CONFIG_FORCEDAWAKE;
  writeCamReg(camId, WCONFIG, currentConf);
}
inline uint8_t isCamOnline(uint8_t camId){
  return (readCamReg(camId, RSTATUS) | STATUS_PRODID) == 0x01;
}
inline uint8_t isCamAsleep(uint8_t camId){
  return (readCamReg(camId, RSTATUS) | STATUS_AWAKE) == 0x01;
}
inline uint8_t getMaxPixel(uint8_t camId){
  return readCamReg(camId, RMAX_PIX);
}
inline uint8_t getMinPixel(uint8_t camId){
  return readCamReg(camId, RMIN_PIX);
}
inline uint8_t getSqual(uint8_t camId){
  return readCamReg(camId, RSQUAL);
}
inline uint8_t getPixelSum(uint8_t camId){
  return readCamReg(camId, RPIX_SUM);
}
inline uint8_t getXMovement(uint8_t camId){
  return readCamReg(camId, RDELTA_X);
}
inline uint8_t getYMovement(uint8_t camId){
  return readCamReg(camId, RDELTA_Y);
}
inline void requirePixels(uint8_t camId){
  uint8_t rowIndex = 0;
  uint8_t colIndex = 0;
  uint8_t pixVal = 0;
  uint8_t bitIndex = 0;
  uint8_t byteIndex = 0;
  uint8_t offsetCamId = (camId == CAM0 ? 0 : 1);

  writeCamReg(camId, WPIX_DATA, 0x01); /* initialize a pixel dump command with a dummy write */
  do{
    pixVal = readCamReg(camId, RPIX_DATA);
  }while((pixVal & PIXDATA_SOF) == 0); /* wait for start of frame bit */
  pixVal -= PIXDATA_SOF; /* normalize pixel value to discard SOF bit*/
  
  /* acquire actual pixels (18x18 = 324)*/
  for(rowIndex = 0; rowIndex < CAM_PIX_HEIGHT; ++rowIndex){
    for(colIndex = 0; colIndex < CAM_PIX_WIDTH ; ++colIndex){
	if((pixVal & PIXDATA_IVALID) != 0) {						/* make sure that the pixel is valid */
	  if((pixVal - PIXDATA_IVALID) > camSteps[camId]) {
            camPixBuf[offsetCamId][byteIndex] <<= 1;
            camPixBuf[offsetCamId][byteIndex] |= 1;
          }
          if(bitIndex++ == 8) {
            bitIndex = 0;
            byteIndex++;
          }
	} else --colIndex;
        
	pixVal = readCamReg(camId, RPIX_DATA);					/* read next pixel */
    }
  }
}

inline uint8_t *getPixelBuffer(uint8_t camId) {
  switch(camId) {
  case CAM0:
    return camPixBuf[0];
    break;
  case CAM1:
    return camPixBuf[1];
    break;
  default:
    break;
  }
}

void setPixelStep(uint8_t camId, uint8_t newVal) {
  switch(camId) {
  case CAM0:
    camSteps[0] = newVal;
    break;
  case CAM1:
    camSteps[1] = newVal;
    break;
  default:
    break;
  }
}
