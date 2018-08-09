/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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