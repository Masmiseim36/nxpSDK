/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _DNS_UTILS_H
#define _DNS_UTILS_H
/*!=================================================================================================
\file       dns_utils.h
\brief      This is a header file for the dns and mdns modules.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "sockets.h"
/*==================================================================================================
Public macros
==================================================================================================*/

#ifndef DNS_MAX_NAME_LEN
#define DNS_MAX_NAME_LEN            (256U)      /*!< Maximum name length */
#endif

#define DNS_POINTER_VALUE           (192U)
#define DNS_GET_POINTER(x)          ((x) - (49152))

/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum
{
    mDnsTypeA_c         =1U,
    mDnsTypeNS_c        =2U,
    mDnsTypeCNAME_c     =5U,
    mDnsTypeSOA_c       =6U,
    mDnsTypePTR_c       =12U,
    mDnsTypeMX_c        =15U,
    mDnsTypeTXT_c       =16U,
    mDnsTypeAAAA_c      =28U,
    mDnsTypeSRV_c       =33U,
    mDnsTypeA6_c        =38U,
    mDnsTypeANY_c       =255U
} DnsType_t;

typedef enum
{
    mDnsFlagRA_c = 0x80U,
    mDnsFlagRD_c = 0x100U,
    mDnsFlagTC_c = 0x200U,
    mDnsFlagAA_c = 0x400U,
    mDnsFlagQR_c = 0x8000U,
    mDnsFlagOpCode_c = 0x7000U,
    mDnsFlagRCode_c = 0x0FU

} DnsFlags_t; /*!< Flags used for the DNS header */

typedef enum
{
    mDnsRespCodeNoErr_c = 0U,
    mDnsRespCodeFormatErr_c = 1U,
    mDnsRespCodeSrvErr_c = 2U,
    mDnsRespCodeNameErr_c = 3U,
    mDnsRespCodeNotImpl_c = 4U,
    mDnsRespCodeRefused_c = 5U
} DnsResponseCode_t;

typedef enum
{
    mDnsOpCodeStdQuery_c,
    mDnsOpCodeInverseQuery_c,
    mDnsOpCodeStatusRequest_c,
}DnsOpCode_t;

typedef enum
{
    mDnsResolverBusy,
    mDnsResolverCacheEntryFound,
    mDnsResolverRequestSent,
    mDnsResolverMemError,
    mDnsResolverNoDnsServer,
}DnsResponseCode;

typedef enum
{
    mDnsClassIN_c = 1U,
    mDnsClassCS_c = 2U,
    mDnsClassCH_c = 3U,
    mDnsClassHS_c = 4U,
    mDnsClassANY_c = 255U

}DnsClass_t;

/* Header section format */
/**
                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

typedef struct dnsHeader_tag
{
    uint8_t id[2];
    uint8_t flags[2];
    uint8_t qdcount[2];
    uint8_t ancount[2];
    uint8_t nscount[2];
    uint8_t arcount[2];
} dnsHeader_t; /*!< DNS header */

/* Question section format */
/**
                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
typedef struct dnsQuestion_tag
{
    uint8_t qtype[2];
    uint8_t qclass[2];
} dnsQuestion_t; /*!< DNS question */


/* Resource record format */
/**
                                     1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
typedef struct dnsRr_tag
{
    uint8_t type[2];
    uint8_t dnsClass[2];
    uint8_t ttl[4];
    uint8_t rdlength[2];
} dnsRr_t; /*!< DNS RR */

typedef struct dnsCacheEntry_tag
{
    uint16_t type;
    uint16_t dnsClass;
    uint16_t responseLength;
    uint8_t *name, *response;
    uint32_t ttl;
} dnsCacheEntry_t;

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
\fn     uint8_t* DNS_GetNameByOffset(uint8_t *pDnsPacket, uint16_t *pOffset)
\brief  This function transforms a string to DNS string format.

\param  [in]  pDnsPacket  Pointer to the dns packet.
\param  [in]  pOffset     Pointer to the offset in packet. This function also put the offset after
                          the name it is looking for.

\return       uint8_t*    Returns pointer to the string with the name in dns packet.
***************************************************************************************************/
uint8_t* DNS_GetNameByOffset(uint8_t *pDnsPacket, uint16_t *pOffset);

/*!*************************************************************************************************
\fn     uint8_t* DNS_MakeDnsName(uint8_t *nodename)
\brief  This function transforms a string to DNS string format.

\param  [in]  nodename  Pointer to the string .

\return       uint8_t*  Returns pointer to modified string or NULL if something is wrong.
***************************************************************************************************/
uint8_t* DNS_MakeDnsName(uint8_t *nodename);


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _DNS_CLIENT_H */
