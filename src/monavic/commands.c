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

static int linkPresent = 0;
static int comPort = 0;

int initLink(int port, int baud) {
  if(OpenComport(port, baud) != 1) {
    comPort = port;
    linkPresent = 1;
    return 1;
  }

  return 0;
}

void closeLink(int port) {
  linkPresent = 0;
  CloseComport(port);
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
  uint8_t buff[1];
  uint8_t txAttemptCnt = 0;
  uint8_t elapsedTime = 0;
  int i = 0;

  /* send command */
  com->crc = computeCrc(com->data, 1);
  do {
    SendByte(comPort, com->length);
    for(i = 0; i < META_PAYLOAD_LENGTH; ++i) {
      SendByte(comPort, com->data[i]);
    }
    SendByte(comPort, com->crc);

    elapsedTime = 0;
    while(PollComport(comPort, buff, 1) == 0 && elapsedTime != 25) { usleep(1); elapsedTime++; }
  } while(buff[0] != ALL_OK && (txAttemptCnt++) < 5);

  /* receive response */
  elapsedTime = 0;
  while(PollComport(comPort, buff, 1) == 0 && elapsedTime != 25) { usleep(1); elapsedTime++; }
  resp = buff[0];

  return resp;
}

static AtomicCommand procCommand(AtomicCommand *com) {
  AtomicCommand resp;
  uint8_t buff[MAX_COMMAND_PAYLOAD + 2];
  uint8_t buffIndex = 0;
  uint8_t recvByteCnt = 0;
  uint8_t localCrc = 0;
  uint8_t txAttemptCnt = 0;
  uint8_t rxAttemptCnt = 0;
  int i = 0;

  /* send command */
  com->crc = computeCrc(com->data, com->length);
  do {
    SendByte(comPort, com->length);
    for(i = 0; i < com->length; ++i) {
      SendByte(comPort, com->data[i]);
    }
    SendByte(comPort, com->crc);
    while(PollComport(comPort, buff, 1)) { }
  } while(buff[0] != ALL_OK && (txAttemptCnt++) < 5);

  /* receive response */
  do {
    /* get PCAVI -> CARAVI Tx response */
    while(PollComport(comPort, buff, 1)) { }
    if(buff[0] != ALL_OK) {
      fprintf(stderr, "PCAVI -> CARAVI Tx response not OK : %d!\n", buff[0]);
    }
    /* get CARAVI -> PCAVI Rx response */
    while(PollComport(comPort, buff, 1)) { }
    if(buff[0] != ALL_OK) {
      fprintf(stderr, "CARAVI -> PCAVI Tx response not OK : %d!\n", buff[0]);
    }
    /* get complete packet */
    while((recvByteCnt = PollComport(comPort, buff, sizeof(buff))) == 0) { }
    localCrc = 0;
    buffIndex = 1;
    resp.length = buff[0];
    resp.data = (uint8_t *)malloc(resp.length * sizeof(uint8_t));
    for(i = 0; i < resp.length; ++i) {
      resp.data[i] = buff[buffIndex++];
      localCrc += resp.data[i];
    }
    resp.crc = buff[buffIndex];

    if(resp.crc != localCrc) {
      SendByte(comPort, INVALID_CRC);
      free(resp.data);
    } else {
      SendByte(comPort, ALL_OK);
    } 
  } while(resp.crc != localCrc && (rxAttemptCnt++) < 5);

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
  g_assert(linkPresent == 1);

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
  g_assert(linkPresent == 1);

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
  g_assert(linkPresent == 1);

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

gboolean getPixelData(uint8_t camId, uint8_t **holder, uint8_t holderSz) {
  g_assert(linkPresent == 1);

  AtomicCommand reqCom = getPixelCamDataCommand(camId);
  AtomicCommand result = procCommand(&reqCom);
  g_assert(result.length <= holderSz);
  g_assert(NULL != holder);
  g_memmove((*holder), reqCom.data, holderSz);

  return TRUE;
}
