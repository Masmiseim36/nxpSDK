/*

Copyright 2016-2019 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#ifndef __GEN_FSK_TESTS_H__
#define __GEN_FSK_TESTS_H__

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef enum app_status_tag
{
  gAppStatusSuccess,
  gAppStatusMemoryAllocFailure
} app_status_t;

typedef enum ct_event_tag
{
  gCtEvtRxDone_c       = 0x00000001U,
  gCtEvtTxDone_c       = 0x00000002U,
  gCtEvtSeqTimeout_c   = 0x00000004U,
  gCtEvtRxFailed_c     = 0x00000008U,
  
  gCtEvtTimerExpired_c = 0x00000010U,
  gCtEvtUart_c         = 0x00000020U,
  gCtEvtKBD_c          = 0x00000040U,
  gCtEvtSelfEvent_c    = 0x00000080U,
  
  gCtEvtWakeUp_c       = 0x00000100U,
  
  gCtEvtMaxEvent_c     = 0x00000200U,
  gCtEvtEventsAll_c    = 0x000003FFU
}ct_event_t;

typedef enum ct_param_type_tag{
    gParamTypeNumber_c = 0,
    gParamTypeString_c,
    gParamTypeBool_c,
    gParamTypeMixed_c,
    gParamTypeMaxType_c
}ct_param_type_t;

typedef struct ct_config_params_tag
{
    ct_param_type_t paramType;
    uint8_t paramName[20];
    uint8_t id;
    char**  ptr;
    union
    {
        uint32_t decValue;
        uint8_t stringValue[4];
        bool_t  boolValue;
    }
    paramValue;
}ct_config_params_t;

typedef struct ct_rx_indication_tag
{
    uint64_t timestamp;
    uint8_t *pBuffer;
    uint16_t bufferLength; 
    int8_t rssi;
    uint8_t crcValid;
}ct_rx_indication_t;

typedef void (* pHookAppNotification) ( void );
typedef void (* pTmrHookNotification) (void*);

typedef enum ct_conf_param_idx_tag
{
    gConfParamMode      = 0,
    gConfParamRateMode  = 1,
    gConfParamWhitenMode= 2,
    gConfParamChannel   = 3,
    gConfParamPower     = 4,
    gConfParamPayload   = 5,
    gConfParamXtalTrim  = 6,
#if defined(gBoard_ExtPaSupport_d) && (gBoard_ExtPaSupport_d > 0)
    gConfParamExPaGain  = 7,
    gConfParamExPaAnt   = 8,
    gConfParamTypeMax   = 9,
#else
    gConfParamTypeMax   = 7,
#endif
    gConfParamIdxMax
}ct_conf_param_idx;
/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define gModeRx_c (1)
#define gModeTx_c (2)
#define gDefaultMode_c gModeRx_c

#ifdef RADIO_IS_GEN_3P5
#define gRateGenfsk250Kbps_c    (0)
#define gRateGenfsk500Kbps_c    (1)
#define gRateGenfsk1Mbps_c      (2)
#define gRateGenfsk2Mbps_c      (3)
#define gRateBle125Kbps_c       (4)
#define gRateBle500Kbps_c       (5)
#define gRateBle1Mbps_c         (6)
#define gRateBle2Mbps_c         (7)

#define gMaxRate_c              gRateBle2Mbps_c
#define gMaxRateGenfsk_c        gRateGenfsk2Mbps_c
#define gRateMaxNbr             (4)

#define gWhitenFixed_c          (0)
#define gWhitenChanging_c       (1)
#define gMaxWhitenMode_c        (2)
#define gDefaultWhitenMode_c    (gWhitenFixed_c)

#elif defined(RADIO_IS_GEN_3P0)
#define gRateGenfsk250Kbps_c    (0)
#define gRateGenfsk500Kbps_c    (1)
#define gRateGenfsk1Mbps_c      (2)
#define gRateGenfsk2Mbps_c      (3)
#define gRateBle1Mbps_c         (4)
#define gRateBle2Mbps_c         (5)

#define gMaxRate_c              gRateBle2Mbps_c
#define gMaxRateGenfsk_c        gRateGenfsk2Mbps_c
#define gRateMaxNbr             (4)
#else
#define gRateGenfsk250Kbps_c    (0)
#define gRateGenfsk500Kbps_c    (1)
#define gRateGenfsk1Mbps_c      (2)
#define gRateBle1Mbps_c         (3)

#define gMaxRate_c              gRateBle1Mbps_c
#define gMaxRateGenfsk_c        gRateGenfsk1Mbps_c
#define gRateMaxNbr             (3)
#endif

#define gMinRate_c     gRateGenfsk250Kbps_c
#define gDefaultRate_c gRateGenfsk1Mbps_c

/*tx power*/
#ifdef RADIO_IS_GEN_3P5
#define gGenFskLastLowPowerIdx_c     (31)
#define gGenFskMaxTxPowerLevel_c     ((gGenFskLastLowPowerIdx_c*2)+1)   /* High power table is used from index 32 to index 63. */
#define gGenFskMinTxPowerLevel_c     (0x00)                             /* Low power table is used from index 0 to index 31. */
#else /* RADIO_IS_GEN_3P5 */
#define gGenFskMaxTxPowerLevel_c     (0x20)
#define gGenFskMinTxPowerLevel_c     (0x00)
#endif /* RADIO_IS_GEN_3P5 */
#define gGenFskDefaultTxPowerLevel_c (0x08)

/*channel*/
#define gGenFskMaxChannel_c     (0x7F)
#define gGenFskMinChannel_c     (0x00)
#define gGenFskDefaultChannel_c (0x2A)
      
#if defined(gBoard_ExtPaSupport_d) && (gBoard_ExtPaSupport_d > 0)
/* these setting should match the default pin config */
#define gDefaultExPaMode_c      (0)
#define gDefaultExPaAnt_c       (1)
#endif

/*network address*/
#ifdef TEST_DTM_LIKE
#define gGenFskDefaultSyncAddress_c  (0x71764129) //DTM sync address
#else
#define gGenFskDefaultSyncAddress_c  (0x8E89BED6)
#endif /* TEST_DTM_LIKE */

#define gGenFskDefaultSyncAddrSize_c (0x03) /*bytes = size + 1*/

/*the following field sizes must be multiple of 8 bit*/
#define gGenFskDefaultH0FieldSize_c     (8)
#if  (defined(TEST_GENFSK_ADV) || defined(TEST_DTM_LIKE))
#define gGenFskDefaultLengthFieldSize_c (8)
#define gGenFskDefaultH1FieldSize_c     (0)
#else
#define gGenFskDefaultLengthFieldSize_c (6)
#define gGenFskDefaultH1FieldSize_c     (2)
#endif /* (defined(TEST_GENFSK_ADV) || defined(TEST_DTM_LIKE)) */
#define gGenFskDefaultHeaderSizeBytes_c ((gGenFskDefaultH0FieldSize_c + \
                                          gGenFskDefaultLengthFieldSize_c + \
                                          gGenFskDefaultH1FieldSize_c) >> 3)                             
#if gGenFskDefaultLengthFieldSize_c < 3
#error "For this application the length field size should not be less than 3"
#endif

#define gGenFskDefaultCrcSize_c         3
#define gGenFskDefaultCrcSeed_c         0x00555555
#define gGenFskDefaultCrccPoly_c        0x0000065B

/*payload length is limited to 2047 bytes including CRC*/
#define gGenFskMaxPayloadLen_c ((1 << gGenFskDefaultLengthFieldSize_c) - 1)
#if gGenFskMaxPayloadLen_c > (2047 - gGenFskDefaultCrcSize_c)
    #undef gGenFskMaxPayloadLen_c
    #define gGenFskMaxPayloadLen_c (2047 -gGenFskDefaultCrcSize_c)
#endif

/*test opcode + 2byte packet index + 2byte number of packets for PER test*/
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
#define gGenFskMinPayloadLen_c (10) 
#else
#define gGenFskMinPayloadLen_c (6) 
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
#define gGenFskDefaultPayloadLen_c (gGenFskMinPayloadLen_c)

#define gMaxTrimValue_c                 0x7F
#define gMinTrimValue_c                 0x00

#define gGenFskDefaultMaxBufferSize_c (gGenFskDefaultSyncAddrSize_c + 1 + \
                                       gGenFskDefaultHeaderSizeBytes_c  + \
                                           gGenFskMaxPayloadLen_c)

/*H0 and H1 config*/
#define gGenFskDefaultH0Value_c        (0x0000)
#if  (defined(TEST_GENFSK_ADV) || defined(TEST_DTM_LIKE))
#define gGenFskDefaultH0Mask_c         0
#else
#define gGenFskDefaultH0Mask_c         ((1 << gGenFskDefaultH0FieldSize_c) - 1)
#endif /* (defined(TEST_GENFSK_ADV) || defined(TEST_DTM_LIKE)) */

#define gGenFskDefaultH1Value_c        (0x0000)
#define gGenFskDefaultH1Mask_c         ((1 << gGenFskDefaultH1FieldSize_c) - 1)
/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern ct_config_params_t gaConfigParams[];
extern uint8_t mAppSerId;
extern uint8_t mAppTmrId;
/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
extern app_status_t CT_GenFskInit(pHookAppNotification pFunc, pTmrHookNotification pTmrFunc);

extern bool_t CT_PacketErrorRate(ct_event_t evType, void* pAssociatedValue);
extern bool_t CT_RangeTest(ct_event_t evType, void* pAssociatedValue);
extern bool_t CT_ContinuousTests(ct_event_t evType, void* pAssociatedValue);
extern bool_t CT_TrimAdjust(ct_event_t evType, void* pAssociatedValue);

extern bool_t CT_UpdateShortcutKeyParam(uint8_t u8PressedKey);
extern bool_t CT_IsShortcutMenuEnabled(void);
#endif