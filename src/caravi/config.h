/* config.h - Configuration file used by the system
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

/* general MCU configs */
#define UART_BAUD	19200
#define F_CPU		8000000

/* cams region */
#define CAM_RPORT	PORTD
#define CAM_WPORT	PIND
#define CAM_PDIR	DDRD
#define CAM_DATAPIN PD3

/* cams definition used as individual clock inputs and common data line.
   Do not modify unless you know what you are doing! */
#define CAM0 PD4
#define CAM1 PD5

/* macro definitions for independent MCU actions */
#define SET_PIN_DATA(port, pin) ((port) |= (1 << pin))
#define RESET_PIN_DATA(port, pin) ((port) &= ~(1 << pin))
#define SET_PIN_OUTPUT(port, pin) SET_PIN_DATA(port, pin)
#define SET_PIN_INPUT(port, pin) RESET_PIN_DATA(port, pin)

#define PIXCOLOR_STEP 20	/* anything below this value will be treated as black and
							   anything above this value will be treated as white when interpreting the pixels
							   acquired from the cams */

#endif
