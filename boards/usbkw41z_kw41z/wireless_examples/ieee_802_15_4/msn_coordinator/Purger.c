/*
* Copyright (c) 2012, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "Purger.h"
#include "MacInterface.h"
#include "MemManager.h"
#include "Messaging.h"
#include "SerialManager.h"

/* Array with enough space to track messages in the indirect queue. */
appMsgTrack_t msgTrackArray[mPurgerNumPackets_c];

/* App provided function to deal with devices which failed to request
   their packet from the indirect queue in due time. */
void (*ptAppProcessAddress)(uint8_t, uint8_t);

/* Expiration interval for the packets added - specified at module
   initialization. */
uint8_t gPurgerExpireInterval;

/******************************************************************************
* This is the initialization function for the module. It sets all slots of the 
* tracking list to unused, get expire time and the process function from the 
* application.
******************************************************************************/
void  Purger_Init(uint8_t expireInterval, void (*appFn)(uint8_t, uint8_t))
{
  uint8_t i;

  gPurgerExpireInterval = expireInterval;
  ptAppProcessAddress = appFn;

  for(i = 0; i < mPurgerNumPackets_c; i++)
  {
    msgTrackArray[i].slotStatus = mPurgerUnusedSlot_c;
  }
}

/******************************************************************************
* This function adds a packet descriptor to the tracking list, in the first 
* free slot. The application specifies the current internal time, and the 
* packet will expire after the number of intervals specified at initialization.
* The descriptor contains the msdu, destination address and expiration time.
* If no slots are free, purgerNoSlot is returned. If this happen, check that 
* mPurgerNumPackets_c is equal with the number of possible packets in the 
* indirect queue.  
******************************************************************************/
uint8_t Purger_Track(uint8_t msdu, uint8_t destHigh, uint8_t destLow, uint8_t time)
{
  uint8_t i;
  uint8_t result = purgerNoSlot;

  for(i = 0; i < mPurgerNumPackets_c; i++)
  {
    if(mPurgerUnusedSlot_c == msgTrackArray[i].slotStatus)
    {
      msgTrackArray[i].msduHandle = msdu;
      msgTrackArray[i].destAddressHigh = destHigh;
      msgTrackArray[i].destAddressLow = destLow;
      msgTrackArray[i].expirationTime = time + gPurgerExpireInterval;
      msgTrackArray[i].slotStatus = mPurgerUsedSlot_c;

#if mPurgerVerboseMode_c == 1        
      Serial_Print(interfaceId,"Track: ", gAllowToBlock_d);
      Serial_PrintHex(interfaceId,&msgTrackArray[i].msduHandle, 1, 0);
      Serial_Print(interfaceId," at: ", gAllowToBlock_d);
      Serial_PrintHex(interfaceId,&time, 1, 0);
      Serial_Print(interfaceId," to: ", gAllowToBlock_d);
      Serial_PrintHex(interfaceId,&msgTrackArray[i].expirationTime, 1, gPrtHexNewLine_c);
#endif //mPurgerVerboseMode_c      

      result = purgerNoError;
      break;
    }
  }
  return result;
}

/******************************************************************************
* This function removes a packet descriptor from the tracking list. It must
* be called by the application every time it receives the gMcpsDataCnf_c and 
* use as packet identified the msdu. If the packet is not found, purgerNoMessage
* is returned. 
******************************************************************************/
uint8_t Purger_Remove(uint8_t msdu)
{
  uint8_t i;
  uint8_t result = purgerNoMessage;

  for(i = 0; i < mPurgerNumPackets_c; i++)
  {
    if(msgTrackArray[i].msduHandle == msdu)
    {
      msgTrackArray[i].slotStatus = mPurgerUnusedSlot_c;

#if mPurgerVerboseMode_c == 1        
      Serial_Print(interfaceId,"Untracked: ", gAllowToBlock_d);
      Serial_PrintHex(interfaceId,&msgTrackArray[i].msduHandle, 1, gPrtHexNewLine_c);
#endif //mPurgerVerboseMode_c      

      result = purgerNoError;
      break;
    }
  }

  return result;
}

/******************************************************************************
* Walk the list of tracked packets and purge expired packets. For every purge 
* request we also call the app provided function to deal with the event.
* It is recommended that this function be called once per time interval. If one
* or more time intervals are skipped, the packets will be purged if the current
* time passed their expiration time, but they will stay in the indirect queue 
* more than was specified.
******************************************************************************/
uint8_t Purger_Check(uint8_t time, instanceId_t macInstance, uint8_t interfaceId)
{
  uint8_t i;
  uint8_t result = 0;
  uint8_t ret = 0;
  nwkToMcpsMessage_t *purgeRequestPacket = NULL;

  (void) ret;   // remove compiler warning
  for(i = 0; i < mPurgerNumPackets_c; i++)
  {
    if((msgTrackArray[i].slotStatus == mPurgerUsedSlot_c) && 
      (time >= msgTrackArray[i].expirationTime) && 
      ((uint8_t)(msgTrackArray[i].expirationTime - time) > gPurgerExpireInterval))
    {
      purgeRequestPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t));
      if(purgeRequestPacket != NULL)
      {
        /* Create an MCPS-Purge Request message containing the msdu. */
        purgeRequestPacket->msgType = gMcpsPurgeReq_c;
        /* Specify the message to purge. */
        purgeRequestPacket->msgData.purgeReq.msduHandle = msgTrackArray[i].msduHandle;
        /* Send the Data Request to the MCPS */
        (void)NWK_MCPS_SapHandler(purgeRequestPacket, macInstance);

#if mPurgerVerboseMode_c == 1        
        Serial_Print(interfaceId,"Sent purge request for ", gAllowToBlock_d);
        Serial_PrintHex(interfaceId,&purgeRequestPacket->msgData.purgeReq.msduHandle, 1, gPrtHexNewLine_c);
#endif //mPurgerVerboseMode_c        

        ptAppProcessAddress(msgTrackArray[i].destAddressHigh, msgTrackArray[i].destAddressLow); 
        /* Remove it from the tracking list. */
        ret = Purger_Remove(msgTrackArray[i].msduHandle);
        result++;  
      }
      else
      {
        /* If this happens, this function will get called until the memory manager
        has memory to allocate the packet. */
        Serial_Print(interfaceId,"Can't allocate purge request packet.\n\r", gAllowToBlock_d);
      }
    }
  }

  return result;
}
  
