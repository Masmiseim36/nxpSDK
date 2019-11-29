/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _PURGER_H
#define _PURGER_H

#include "EmbeddedTypes.h"

#ifdef __cplusplus
    extern "C" {
#endif

#define mPurgerUsedSlot_c         1
#define mPurgerUnusedSlot_c       0
#define mPurgerNumPackets_c       4

#define mPurgerVerboseMode_c      0

/* Type: appMsgTrack_t */
typedef struct appMsgTrack_tag
{
  uint8_t  msduHandle;
  uint8_t  destAddressHigh;
  uint8_t  destAddressLow;
  uint8_t  expirationTime;
  uint8_t  slotStatus;
} appMsgTrack_t;

/* Error codes for purger*/
enum
{
  purgerNoError,
  purgerNoMessage,
  purgerNoSlot,
  purgerMaxError
};

/* Declarations of the purger functions */
void    Purger_Init(uint8_t expireInterval, void (*appFn)(uint8_t, uint8_t));
uint8_t Purger_Track(uint8_t msdu, uint8_t destHigh, uint8_t destLow, uint8_t expire);
uint8_t Purger_Remove(uint8_t msdu);
uint8_t Purger_Check(uint8_t time, instanceId_t macInstance, uint8_t interfaceId);

#ifdef __cplusplus
}
#endif

#endif //_PURGER_H