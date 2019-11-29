/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_OTA_CLIENT_H
#define APP_OTA_CLIENT_H

/*!
\file       app_ota_client.h
\brief      OTA Client Implementation
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifndef OTA_DEMO_TIMINGS
#define OTA_SERVER_QUERY_TIME_IN_SEC    ( 60*20  /*60*60*24*/)      /*To find the server*/
#define OTA_IMAGE_QUERY_TIME_IN_SEC     ( 60*20  /*60*60*24*/)      /*To query new image available*/
#define OTA_INITIAL_TIME_IN_SEC		RND_u32GetRand(1, 1150 /*60*60*/);    /* spread out 2 to 60 minutes */
#define OTA_IEEE_LOOKUP_TIME_IN_SEC     5           /* how longto delay the ieee look up after the match response */
#define OTA_IMAGE_QUERY_TIMEOUT_IN_SEC  15              /* To wait for the image response */
#define OTA_DISCOVERY_TIMEOUT_IN_SEC    15              /* To wait for the server discovery */
#define OTA_DL_IN_PROGRESS_TIME_IN_SEC  (15*60)             /* Time out period without block notifications */
#else
#define OTA_SERVER_QUERY_TIME_IN_SEC     60         /*To find the server*/
#define OTA_IMAGE_QUERY_TIME_IN_SEC     60         /*To query new image available*/
#define OTA_INITIAL_TIME_IN_SEC		RND_u32GetRand(0, 44);    /* spread out 10 to 59 seconds */
#define OTA_IEEE_LOOKUP_TIME_IN_SEC     5           /* how longto delay the ieee look up after the match response */
#define OTA_IMAGE_QUERY_TIMEOUT_IN_SEC  15         /* To wait for the image response */
#define OTA_DISCOVERY_TIMEOUT_IN_SEC    15         /* To wait for the server discovery */
#define OTA_DL_IN_PROGRESS_TIME_IN_SEC  (15*60)        /* Time out period without block notifications */
#endif

#define MAX_SERVER_EPs 2
#define MAX_SERVER_NODES 2
#define IGNORE_COORDINATOR_AS_OTA_SERVER
#define APP_OTA_OFFSET_WRITEN_BEFORE_LINKKEY_VERIFICATION 1100
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
	bool bValid;
    uint64 u64IeeeAddrOfServer;
    uint16 u16NwkAddrOfServer;
    uint8 u8MatchLength;
    uint8 u8MatchList[MAX_SERVER_EPs];
}tsDiscovedOTAServers;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void vAppInitOTA(void);
PUBLIC void vRunAppOTAStateMachine(void);
PUBLIC void vHandleMatchDescriptor( ZPS_tsAfEvent  * psStackEvent);
PUBLIC void vHandleIeeeAddressRsp( ZPS_tsAfEvent  * psStackEvent);
PUBLIC void vHandleAppOtaClient(tsOTA_CallBackMessage *psCallBackMessage);
PUBLIC void vLoadOTAPersistedData(void);
PUBLIC void vOTAResetPersist(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern PUBLIC	uint8 s_au8LnkKeyArray[16];
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_OTA_CLIENT_H*/
