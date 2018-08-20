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



#ifndef BDB_OPTIONS_INCLUDED
#define BDB_OPTIONS_INCLUDED

/*!
\file       bdb_options.h
\brief      Options header file for Base Device Behaviour (BDB) module
*/
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "jendefs.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Channel definition. If it's an IEEE 802.15.4 valid channel (11-26), it will be used as a fixed channel,
   otherwise a primary and a secondary channel mask will be used. */
#ifndef SINGLE_CHANNEL   
    #define SINGLE_CHANNEL 0
#endif
    
/* Channel Definitions */
#if (SINGLE_CHANNEL < 11 || SINGLE_CHANNEL > 26)
#ifndef BDB_PRIMARY_CHANNEL_SET
    #define BDB_PRIMARY_CHANNEL_SET                 (0x02108800)                               /* bdbPrimaryChannelSet */
#endif
#ifndef BDB_SECONDARY_CHANNEL_SET
    #define BDB_SECONDARY_CHANNEL_SET               (0x07FFF800 ^ BDB_PRIMARY_CHANNEL_SET)     /* bdbSecondaryChannelSet */
#endif
#ifndef BDBC_TL_PRIMARY_CHANNEL_SET
    #define BDBC_TL_PRIMARY_CHANNEL_SET             (0x02108800)                               /* bdbcTLPrimaryChannelSet */
#endif
#ifndef BDBC_TL_SECONDARY_CHANNEL_SET
    #define BDBC_TL_SECONDARY_CHANNEL_SET           (0x07fff800 ^ BDBC_TL_PRIMARY_CHANNEL_SET) /* bdbcTLSecondaryChannelSet */
#endif 

#else

//#warning Single channel only!
#ifndef BDB_PRIMARY_CHANNEL_SET
    #define BDB_PRIMARY_CHANNEL_SET                 (1<<SINGLE_CHANNEL)
#endif
#ifndef BDB_SECONDARY_CHANNEL_SET
    #define BDB_SECONDARY_CHANNEL_SET               (0)
#endif
#ifndef BDBC_TL_PRIMARY_CHANNEL_SET
    #define BDBC_TL_PRIMARY_CHANNEL_SET             (0x02108800)    /* bdbcTLPrimaryChannelSet */
#endif
#ifndef BDBC_TL_SECONDARY_CHANNEL_SET
    #define BDBC_TL_SECONDARY_CHANNEL_SET           (0x07fff800 ^ BDBC_TL_PRIMARY_CHANNEL_SET) /* bdbcTLSecondaryChannelSet */
#endif
#endif

/* BDB Constants used by all nodes
   Note - Must not change for final app */
#define BDBC_MAX_SAME_NETWORK_RETRY_ATTEMPTS   (10)     /* bdbcMaxSameNetworkRetryAttempts */
#define BDBC_MIN_COMMISSIONING_TIME            (180)    /* bdbcMinCommissioningTime */
#define BDBC_REC_SAME_NETWORK_RETRY_ATTEMPTS   (3)      /* bdbcRecSameNetworkRetryAttempts */
#define BDBC_TC_LINK_KEY_EXCHANGE_TIMEOUT      (5)      /* bdbcTCLinkKeyExchangeTimeout */

/* BDB Constants used by nodes supporting touchlink
   Note - Must not change for final app */
#define BDBC_TL_INTERPAN_TRANS_ID_LIFETIME      (8)     /* bdbcTLInterPANTransIdLifetime */
#define BDBC_TL_MIN_STARTUP_DELAY_TIME          (2)     /* bdbcTLMinStartupDelayTime */
#define BDBC_TL_RX_WINDOW_DURATION              (5)     /* bdbcTLRxWindowDuration */
#define BDBC_TL_SCAN_TIME_BASE_DURATION_MS      (250)   /* bdbcTLScanTimeBaseDuration */
//#define BDB_APPLICATION_DEFINED_TL_MASTER_KEY         /* Uncomment it if application wants to overrides BDB certification TL key with master key */


/* BDB Attribute values */
#ifndef BDB_COMMISSIONING_GROUP_ID
    #define BDB_COMMISSIONING_GROUP_ID              (0xFFFF)            /* bdbCommissioningGroupID */
#endif
#ifndef BDB_COMMISSIONING_MODE
    #define BDB_COMMISSIONING_MODE                  (0x0E)              /* bdbCommissioningMode */
#endif    
#ifndef BDB_COMMISSIONING_STATUS
    #define BDB_COMMISSIONING_STATUS                (0x00)                  /* bdbCommissioningStatus */
#endif
#ifndef BDB_JOINING_NODE_EUI64
    #define BDB_JOINING_NODE_EUI64                  (0x0000000000000000)    /* bdbJoiningNodeEui64 */                                                                       /* bdbJoiningNodeNewTCLinkKey */
#endif
#ifndef BDB_JOIN_USES_INSTALL_CODE_KEY
    #define BDB_JOIN_USES_INSTALL_CODE_KEY          (FALSE)                 /* bdbJoinUsesInstallCodeKey */
#endif                                                                        /* bdbNodeCommissioningCapability - This will be set according to BDB features in Makefile */
                                                                        /* bdbNodeIsOnANetwork - Application is responsible to set this before BDB initialisation */
#ifndef BDB_NODE_JOIN_LINK_KEY_TYPE
    #define BDB_NODE_JOIN_LINK_KEY_TYPE             (0x00)                  /* bdbNodeJoinLinkKeyType */
#endif

#ifndef BDB_SCAN_DURATION
    #define BDB_SCAN_DURATION                       (0x04)                  /* bdbScanDuration */
#endif
#ifndef BDB_TC_LINK_KEY_EXCHANGE_ATTEMPTS
    #define BDB_TC_LINK_KEY_EXCHANGE_ATTEMPTS       (0x00)                  /* bdbTCLinkKeyExchangeAttempts */
#endif    
#ifndef BDB_TC_LINK_KEY_EXCHANGE_ATTEMPTS_MAX
    #define BDB_TC_LINK_KEY_EXCHANGE_ATTEMPTS_MAX   (0x03)                  /* bdbTCLinkKeyExchangeAttemptsMax */
#endif
#ifndef BDB_TC_LINK_KEY_EXCHANGE_METHOD
    #define BDB_TC_LINK_KEY_EXCHANGE_METHOD         (0x00)                  /* bdbTCLinkKeyExchangeMethod */
#endif
#ifndef BDB_TRUST_CENTER_NODE_JOIN_TIMEOUT
    #define BDB_TRUST_CENTER_NODE_JOIN_TIMEOUT      (0x0F)                  /* bdbTrustCenterNodeJoinTimeout */
#endif
#ifndef BDB_TRUST_CENTER_REQUIRE_KEYEXCHANGE
    #define BDB_TRUST_CENTER_REQUIRE_KEYEXCHANGE    (TRUE)                  /* bdbTrustCenterRequireKeyExchange */
#endif
    
#ifndef BDB_SET_DEFAULT_TC_POLICY
    #define BDB_SET_DEFAULT_TC_POLICY               (FALSE)
#endif

#ifndef BDBC_IMP_MAX_REJOIN_CYCLES    
    #define BDBC_IMP_MAX_REJOIN_CYCLES              (0x03)                   
#endif    
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* BDB_OPTIONS_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
