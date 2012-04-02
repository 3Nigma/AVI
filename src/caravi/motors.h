/*  motors.h - Header for the motor(s) control on AVI
 *
 * The module handles the twin motor H-bridge(L298) controller interface giving the posibility to alter parameters such as :
 * - individual/full motor(s) speed (0 - 255)
 * - individual/full motor(s) direction (forward/reverse)
 * - individual/full motor(s) gear (slow, cruising, fast)
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

#ifndef _MOTORS_H_
#define _MOTORS_H_

#include <avr/io.h>

/* define motor identifiers */
#define LEFT_MOTOR	0
#define RIGHT_MOTOR 1
#define ALL_MOTORS	2

/* gears used by the motors modeled through their respective clock prescaler values */
#define SLOW_GEAR		0b00000101		/* <timer clock> = clk/1024 */
#define CRUISING_GEAR	0b00000011		/* <timer clock> = clk/64 */
#define FAST_GEAR		0b00000001		/* <timer clock> = clk/1 */

/* available motor directions */
#define FORWARD 0
#define REVERSE 1

/* L298 interface definition */
#define CONTROLLER_PORT_DIR		DDRC
#define CONTROLLER_PORT_DATA	PORTC
/* Motor B(right side) - Bridge control pins */
#define IN1B_C_PIN				PC7
#define IN2B_D_PIN				PC6
/* Motor A(left side) - Bridge control pins */
#define IN3A_C_PIN				PC5
#define IN4A_D_PIN				PC4

void initMotors(void);

void setMotorDirection(uint8_t, uint8_t);
void setMotorSpeed(uint8_t, uint8_t);
void setMotorGear(uint8_t, uint8_t);
extern void stopAllMotors(void);

#endif /* MOTOARE_H_ */
