/* cams.h (header for the camera system used by AVI)
 *
 * The module is built around AVAGO's ADNS 2610 Mouse Optical Sensor for which the data sheet is freely available (AV02-1184EN).
 * Sensor summary : 25Mhz max clock @ 1500fps with 12ips, 5V power, serial communication @ clock/12 rate
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

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "config.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

/* define masks used by the addresses */
#define WMASK		0x80
#define RMASK		0x00

/* define generic addresses */
#define CONFIG		0x00
#define STATUS		0x01
#define DELTA_Y		0x02
#define DELTA_X		0x03
#define SQUAL		0x04
#define MAX_PIX		0x05
#define MIN_PIX		0x06
#define PIX_SUM		0x07
#define PIX_DATA	0x08
#define SHUT_UP		0x09
#define SHUT_LO		0x0A
#define INV_PROD	0x11

/* define specific addresses */
#define RCONFIG (CONFIG | RMASK)
#define WCONFIG (CONFIG | WMASK)
#define RSTATUS (STATUS | RMASK)
#define RDELTA_Y (DELTA_Y | RMASK)
#define RDELTA_X (DELTA_X | RMASK)
#define RSQUAL (SQUAL | RMASK)
#define RMAX_PIX (MAX_PIX | RMASK)
#define RMIN_PIX (MIN_PIX | RMASK)
#define RPIX_SUM (PIX_SUM | RMASK)
#define RPIX_DATA (PIX_DATA | RMASK)
#define WPIX_DATA (PIX_DATA | WMASK)
#define RSHUT_UP (SHUT_UP | RMASK)
#define RSHUT_LO (SHUT_LO | RMASK)
#define RINV_PROD (SHUT_LO | RMASK)

/* define inner data bit masks */
#define CONFIG_RESET		0x80
#define CONFIG_PWRDOWN		0x40
#define CONFIG_FORCEDAWAKE	0x01
#define STATUS_AWAKE		0x01
#define STATUS_PRODID		0xE0
#define PIX_DATA_SOF		0x80
#define PIX_DATA_DVALID		0x40
#define PIX_DATA_VALUE		0x3F

/* additional defines (eg. bit masks) */
#define PIXDATA_SOF	       	0x80
#define PIXDATA_IVALID		0x40

/* declare initialization function */
void initCams(void);

/* declare interrogation and configuration functions */
extern uint8_t isCamOnline(uint8_t);
extern uint8_t isCamAsleep(uint8_t);
extern void softResetCam(uint8_t);
extern void pwrDownCam(uint8_t);
extern void forceAwakeCam(uint8_t);
extern void requirePixels(uint8_t);

/* declare data acquisition functions */
extern uint8_t getMaxPixel(uint8_t);
extern uint8_t getMinPixel(uint8_t);
extern uint8_t getSqual(uint8_t);
extern uint8_t getPixelSum(uint8_t);
extern uint8_t getXMovement(uint8_t);
extern uint8_t getYMovement(uint8_t);

#endif
