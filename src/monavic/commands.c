/*
***************************************************************************
*
* Author: Victor ADASCALITEI
*
* Copyright (C) 2012 Victor ADASCALITEI
*
* adascalitei.victor@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/

#include "commands.h"

static cssl_t *serialLink;

int initLink(int port, int baud) {
  cssl_start();
  serialLink = cssl_open("/dev/ttyUSB0", NULL, 0,
		     baud,8,0,1);

  return serialLink != NULL;
}

void closeLink(int port) {
  cssl_close(serialLink);
  cssl_stop();
}

static uint8_t computeCrc(uint8_t *data, uint8_t len) {
  uint8_t crc = 0;
  int i = 0;

  for(i = 0; i < len; ++i) {
    crc += data[i];
  }

  return crc;
}

static uint8_t procMetaCommand(AtomicCommand *com) {
  uint8_t resp;
  uint8_t txAttemptCnt = 0;
  int i = 0;

  /* send command */
  com->crc = computeCrc(com->data, 1);
  do {
    cssl_putchar(serialLink, com->length);
    for(i = 0; i < META_PAYLOAD_LENGTH; ++i) {
      cssl_putchar(serialLink, com->data[i]);
    }
    cssl_putchar(serialLink, com->crc);

  } while(cssl_getchar(serialLink) != ALL_OK && (txAttemptCnt++) < 5);

  /* receive response */
  resp = cssl_getchar(serialLink);

  return resp;
}

static AtomicCommand procCommand(AtomicCommand *com) {
  AtomicCommand resp;
  uint8_t respByte;
  uint8_t localCrc = 0;
  uint8_t txAttemptCnt = 0;
  uint8_t rxAttemptCnt = 0;
  int i = 0;

  /* send command */
  com->crc = computeCrc(com->data, com->length);
  do {
    cssl_putchar(serialLink, com->length);
    for(i = 0; i < com->length; ++i) {
      cssl_putchar(serialLink, com->data[i]);
    }
    cssl_putchar(serialLink, com->crc);
   
    respByte = cssl_getchar(serialLink);
  } while(respByte != ALL_OK && (txAttemptCnt++) < 5);
  if(respByte != ALL_OK) {
    fprintf(stderr, "TX feedback missed!\n");
  }

  /* get PCAVI -> CARAVI Tx response */
  respByte = cssl_getchar(serialLink);
  if(respByte != ALL_OK) {
    fprintf(stderr, "PCAVI -> CARAVI Tx response not OK : %d!\n", respByte);
  }

  /* get CARAVI -> PCAVI Rx response */
  respByte = cssl_getchar(serialLink);
  if(respByte != ALL_OK) {
    fprintf(stderr, "CARAVI -> PCAVI Tx response not OK : %d!\n", respByte);
  }

  /* receive response */
  do {
    /* get complete packet */
    localCrc = 0;
    resp.length = cssl_getchar(serialLink);
    resp.data = (uint8_t *)malloc(resp.length * sizeof(uint8_t));
    for(i = 0; i < resp.length; ++i) {
      resp.data[i] = cssl_getchar(serialLink);
      localCrc += resp.data[i];
    }
    resp.crc = cssl_getchar(serialLink);

    if(resp.crc != localCrc) {
      cssl_putchar(serialLink, INVALID_CRC);
      free(resp.data);
    } else {
      cssl_putchar(serialLink, ALL_OK);
    } 
  } while(resp.crc != localCrc && (rxAttemptCnt++) < 5);
  if(resp.crc != localCrc) {
    fprintf(stderr, "Response window missed, CRC mismatched!\n");
  }

  return resp;
}

static AtomicCommand getIsOnlineMetaCommand() {
  AtomicCommand resp;

  resp.length = 0;
  resp.data = (uint8_t *)calloc(1, sizeof(uint8_t));
  resp.data[0] = META_ONLINE_REQUEST;

  return resp;
}

gboolean isPCAviOnline() {
  g_assert(serialLink != NULL);

  AtomicCommand reqCom = getIsOnlineMetaCommand();
  uint8_t result = procMetaCommand(&reqCom);

  free(reqCom.data);

  return result == META_ONLINE_RESPONSE;
}

static AtomicCommand getResetCamCommand(uint8_t camId) {
  AtomicCommand resp;

  resp.length = 1;
  resp.data = (uint8_t *)calloc(1, sizeof(uint8_t));
  resp.data[0] = CAR_CAMS_SUB | camId | CARS_CAMS_FUNC_SOFTRESET;

  return resp;
}

gboolean softResetCam(uint8_t camId) {
  g_assert(serialLink != NULL);

  AtomicCommand reqCom = getResetCamCommand(camId);
  AtomicCommand result = procCommand(&reqCom);

  free(reqCom.data);

  return result.data[0] == ALL_OK;
}

static AtomicCommand getPwrDownCamCommand(uint8_t camId) {
  AtomicCommand resp;

  resp.length = 1;
  resp.data = (uint8_t *)calloc(1, sizeof(uint8_t));
  resp.data[0] = CAR_CAMS_SUB | camId | CARS_CAMS_FUNC_PWRDOWN;

  return resp;
}

gboolean powerDownCam(uint8_t camId) {
  g_assert(serialLink != NULL);

  AtomicCommand reqCom = getPwrDownCamCommand(camId);
  AtomicCommand result = procCommand(&reqCom);

  free(reqCom.data);

  return result.data[0] == ALL_OK;
}

static AtomicCommand getPixelCamDataCommand(uint8_t camId) {
  AtomicCommand resp;

  resp.length = 1;
  resp.data = (uint8_t *)calloc(1, sizeof(uint8_t));
  resp.data[0] = CAR_CAMS_SUB | camId | CARS_CAMS_FUNC_GETPIXELS;

  return resp;
}

gboolean getPixelData(uint8_t camId, uint8_t *holder, uint8_t holderSz) {
  g_assert(serialLink != NULL);

  AtomicCommand reqCom = getPixelCamDataCommand(camId);
  AtomicCommand result = procCommand(&reqCom);
  g_assert(result.length <= holderSz);
  g_assert(NULL != holder);

  int i = 0;
  for(i = 0; i < holderSz; ++i) {
    holder[i] = reqCom.data[i];
  }

  free(reqCom.data);

  return TRUE;
}
