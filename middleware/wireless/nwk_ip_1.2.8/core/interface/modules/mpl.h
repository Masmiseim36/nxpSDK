/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _MPL_H_
#define _MPL_H_

/*!
\file       mpl.h
\brief      This is a header file for the MPL module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
/* Network Includes */
#include "network_utils.h"

/*! Description of the MPL_ENABLED configuration option: set to 1 to enable the MPL
    functionality */
#ifndef MPL_ENABLED
    #define MPL_ENABLED                             0
#endif

/*! Description of the MPL_MULTIPLE_SEED_ID_LENGTH_ENABLED configuration option: set to 1 to permit different
    seed identifier length */
#ifndef MPL_MULTIPLE_SEED_ID_LENGTH_ENABLED
    #define MPL_MULTIPLE_SEED_ID_LENGTH_ENABLED     0
#endif

#if MPL_ENABLED

/*==================================================================================================
Public macros
==================================================================================================*/
/*! Maximum supported length of seed identifier. */
#define MPL_MAX_SEED_ID_LENGTH                  2
/*! Macro for checking if an address is non-linklocal multicast */
#define MPL_IsMulticastAddr(addr)               (((addr)->addr8[1] & 0x0F) >= 0x03)

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! MPL seed identifier length */
typedef enum
{
    gMplSeddIdLengthNoAddr_c          = 0x00,           /*!< The seed-id is the IPv6 Source Address and not included in the MPL Option */
    gMplSeddIdLengthShortAddr_c       = 0x02,           /*!< The seed-id is a 16-bit unsigned integer */
    gMplSeddIdLengthExtendedAddr_c    = 0x08,           /*!< The seed-id is a 64-bit unsigned integer */
    gMplSeddIdLengthIpAddr_c          = 0x10            /*!< The seed-id is a 128-bit unsigned integer */
}mplSeedIdLength_t;

/*! MPL Instance configuration structure */
typedef struct mplInstanceConfig_tag
{
    uint32_t            seedLifetime;                   /*!< Minimum lifetime of a seed */
    uint16_t            Imin;                           /*!< Minimum Trickle timer interval */
    uint16_t            Imax;                           /*!< Maximum Trickle timer interval */
    uint8_t             k;                              /*!< Redundancy constant */
    uint8_t             nbOfTimerExpirations;           /*!< Number of Trickle timer expirations */
    bool_t              useFullInterval;                /*!< use interval [0, I) or [I/2, I) as defined by RFC 6206*/
#if MPL_MULTIPLE_SEED_ID_LENGTH_ENABLED
    uint8_t             seedIdLength;                   /*!< Length of the seed identifier (2, 8, 16 bytes) */
#endif /* MPL_MULTIPLE_SEED_ID_LENGTH_ENABLED */
}mplInstanceCfg_t;

/*! MPL instance parameters */
typedef struct mplInstanceSetEntry_tag
{
    ipIfUniqueId_t      ifUniqueId;                     /*!< ID of the interface associated with this instance */
    uint32_t            seedLifetime;                   /*!< Maximum lifetime of a mplSeedSet entry from this instance */
    uint8_t             nbOfTimerExpirations;           /*!< Maximum number of retransmission for a packet processed in this instance */
#if MPL_MULTIPLE_SEED_ID_LENGTH_ENABLED
    uint8_t             seedIdLength;                   /*!< Seed identifier length for this instance */
#endif /* MPL_MULTIPLE_SEED_ID_LENGTH_ENABLED */
}mplInstanceSetEntry_t;

/*! SeedSet table entry parameters */
typedef struct mplSeedSetEntry_tag
{
    ipAddr_t            domainAddr;                     /*!< MPL Domain address */
    uint64_t            expirationTimestamp;            /*!< Timestamp when mplSeedSet entry expires (in ms) */
    uint32_t            instanceSetEntryIdx;            /*!< Index in mMplInstanceSet table (identifies MPL instance) */
    uint8_t             seedId[MPL_MAX_SEED_ID_LENGTH]; /*!< Seed identifier */
    uint8_t             minSequenceNumber;              /*!< Minimum sequence number of the packets received from seedId in MPL domain identified by domainAddr (packets kept in mplBufferedMessageSet) */
}mplSeedSetEntry_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void MPL_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief  Public interface function for MPL module. This function initializes the MPL module.

\param  [in]    pTaskMsgQueue           Pointer to the task message queue

\retval         TRUE if the MPL module was successfully initialized; otherwise FALSE
***************************************************************************************************/
bool_t MPL_Init(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     void MPL_CreateInstance(ipIfUniqueId_t ifId, mplInstanceCfg_t *pMplInstanceConfig)
\brief  This function creates a new MPL instance.

\param  [in]    ifId                    Interface unique ID
\param  [in]    pMplInstanceConfig      Pointer to the configuration information.
***************************************************************************************************/
void MPL_CreateInstance(ipIfUniqueId_t ifId, mplInstanceCfg_t *pMplInstanceConfig);

/*!*************************************************************************************************
\fn     uint32_t MPL_Forward(ipPktInfo_t *pIpPktInfo, uint8_t direction)
\brief  Public interface function for MPL module. This function processes a multicast packet
        (received or transmitted).

\param  [in]    pIpPktInfo              Pointer to the multicast packet
\param  [in]    direction               Direction of the packet (reception or transmission)

\return         uint32_t                Status of the packet processing
***************************************************************************************************/
uint32_t MPL_Forward(ipPktInfo_t *pIpPktInfo, uint8_t direction);

/*!*************************************************************************************************
\fn     MPL_ResetMPLTxSequenceNumber
\brief  Reset MPL transmission sequence number.
***************************************************************************************************/
void MPL_ResetMPLTxSequenceNumber(void);

#ifdef __cplusplus
}
#endif


#endif /* MPL_ENABLED */

/*================================================================================================*/

#endif /* _MPL_H_ */
