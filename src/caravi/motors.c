/* motors.c - Module that offers a interface to the motors of the car via the L298 driver
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

#include "motors.h"

void initMotors(void){
  /* configure L298 controller lines */
  CONTROLLER_PORT_DIR |= _BV(IN1B_C_PIN) | _BV(IN2B_D_PIN) | _BV(IN3A_C_PIN) | _BV(IN4A_D_PIN);
  CONTROLLER_PORT_DATA &= ~(_BV(IN1B_C_PIN) | _BV(IN2B_D_PIN) | _BV(IN3A_C_PIN) | _BV(IN4A_D_PIN));
  
  /* LEFT_MOTOR init area
     - initialize counter 0 in phase correct PWM mode with OC0(PB3)
     - initialize in cruising gear
     - the motor is initialized in the stop position
  */
  DDRB |= _BV(PB3);
  OCR0 = 0;
  TCCR0 |= _BV(WGM00) | _BV(COM01) | _BV(COM00);
  TCCR0 |= CRUISING_GEAR;
  
  /* RIGHT_MOTOR init area
     - initialize counter 2 in phase correct PWM mode with OC2(PD7)
     - initialize in cruising gear
     - the motor is initialized in the stop position
  */
  DDRD |= _BV(PD7);
  OCR2 = 0;
  TCCR2 |= _BV(WGM20) | _BV(COM21) | _BV(COM20);
  TCCR2 |= CRUISING_GEAR;
}

void applyDirectionShift(uint8_t CPin, uint8_t DPin, uint8_t directionCode){
  CONTROLLER_PORT_DATA &= ~(_BV(CPin) | _BV(DPin));
  CONTROLLER_PORT_DATA |=  directionCode;
}

void setMotorDirection(uint8_t motorId, uint8_t newDirection){
  /* Truth table (Figure 6/page 6 of ST L298 'Jenuary' 2000 DataSheet)
     C = H, D = L -> Forward direction
     C = L, D = H -> Reverse direction
     C = D        -> Fast Motor Stop
     V_{en} assumed H
     where H = High (V_{cc}), L = Low (GND)
  */
  uint8_t MotCPin = 0;
  uint8_t MotDPin = 0;
  
  switch(motorId){
  case LEFT_MOTOR:
    MotCPin = IN3A_C_PIN;
    MotDPin = IN4A_D_PIN;
    break;
  case RIGHT_MOTOR:
    MotCPin = IN1B_C_PIN;
    MotDPin = IN2B_D_PIN;
    break;
  case ALL_MOTORS:
    break;
  default:
    /* it should never reach this point! */
    break;
  }
  
  switch(newDirection){
  case FORWARD:
    if(motorId == ALL_MOTORS){
      /* set FORWARD direction for BOTH motors at once */
      applyDirectionShift(IN3A_C_PIN, IN4A_D_PIN, _BV(IN3A_C_PIN));
      applyDirectionShift(IN1B_C_PIN, IN2B_D_PIN, _BV(IN1B_C_PIN));
    } else {
      /* set FORWARD direction for selected motor */
      applyDirectionShift(MotCPin, MotDPin, _BV(MotCPin));
    }
    break;
  case REVERSE:
    if(motorId == ALL_MOTORS){
      /* set FORWARD direction for BOTH motors at once */
      applyDirectionShift(IN3A_C_PIN, IN4A_D_PIN, _BV(IN4A_D_PIN));
      applyDirectionShift(IN1B_C_PIN, IN2B_D_PIN, _BV(IN2B_D_PIN));
    } else {
      /* set FORWARD direction for selected motor */
      applyDirectionShift(MotCPin, MotDPin, _BV(MotDPin));
    }
    break;
  }
}

void setMotorSpeed(uint8_t motorId, uint8_t newSpeed) {
  switch(motorId){
  case LEFT_MOTOR:
    OCR0 = newSpeed;
    break;
  case RIGHT_MOTOR:
    OCR2 = newSpeed;
    break;
  case ALL_MOTORS:
    OCR0 = newSpeed;
    OCR2 = newSpeed;
    break;
  default:
    /* it should never reach this point! */
    break;
  }
}

void setMotorGear(uint8_t motorId, uint8_t newGear) {	
  switch(motorId) {
  case LEFT_MOTOR:
    TCCR0 = (TCCR0 & 0xF8) | newGear;
    break;
  case RIGHT_MOTOR:
    TCCR2 = (TCCR2 & 0xF8) | newGear;
    break;
  case ALL_MOTORS:
    TCCR0 = (TCCR0 & 0xF8) | newGear;
    TCCR2 = (TCCR2 & 0xF8) | newGear;
    break;
  default:
    /* it should never reach this point! */
    break;
  }
}

inline void stopAllMotors(void){
  /* full system immediate HALT */
  OCR0 = 255;
  OCR2 = 255;
}
