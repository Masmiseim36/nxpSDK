/*! *********************************************************************************
* \defgroup PhyTypes PHY Data Types
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _PHY_TYPES_H
#define _PHY_TYPES_H


/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

/* WARNING!!! Only one frequency can be declared at a time! */
#ifdef gPHY_802_15_4g_d

  /* 802.15.4g Frequency Bands */
  #ifndef gFreqBand_169_400__169_475MHz_d
  #define gFreqBand_169_400__169_475MHz_d 0
  #endif

  #ifndef gFreqBand_470__510MHz_d
  #define gFreqBand_470__510MHz_d 0
  #endif

  #ifndef gFreqBand_779__787MHz_d
  #define gFreqBand_779__787MHz_d 0
  #endif

  #ifndef gFreqBand_863__870MHz_d
  #define gFreqBand_863__870MHz_d 0
  #endif

  #ifndef gFreqBand_902__928MHz_d
  #define gFreqBand_902__928MHz_d 1
  #endif

  #ifndef gFreqBand_920__928MHz_d
  #define gFreqBand_920__928MHz_d 0       
  #endif

  /* Non-standard Frequency Bands */
  #ifndef gFreqBand_865__867MHz_d
  #define gFreqBand_865__867MHz_d 0
  #endif

  /*! The following defines are used to set the specific frequency band ID for the selected frequency band. */
  #if gFreqBand_169_400__169_475MHz_d
    #define gFreqBandId_d           gFreq169_400__169_475MHz_c  
  #elif gFreqBand_470__510MHz_d
    #define gFreqBandId_d           gFreq470__510MHz_c
  #elif gFreqBand_779__787MHz_d
    #define gFreqBandId_d           gFreq779__787MHz_c
  #elif gFreqBand_863__870MHz_d
    #define gFreqBandId_d           gFreq863__870MHz_c
  #elif gFreqBand_902__928MHz_d
    #define gFreqBandId_d           gFreq902__928MHz_c
  #elif gFreqBand_920__928MHz_d
    #define gFreqBandId_d           gFreq920__928MHz_c
  #elif gFreqBand_865__867MHz_d
    #define gFreqBandId_d           gFreq865__867MHz_c
  #else
    #error "No frequency band declared!"
  #endif
#endif  /* gPHY_802_15_4g_d */

/* \note MUST REMAIN UNCHANGED: */
#ifdef gPHY_802_15_4g_d
  /*! This define is used to set the number of symbols per octet for the current PHY. */
  #define gPhySymbolsPerOctet_c     8u
  /*! This define is used to set the length of the PHR in octets for the MRFSK PHY.  */
  #define gPhyMRFSKPHRLength_c      2u    /* [bytes] */
  /*! This define is used to set the length of the Preamble in octets for the MRFSK PHY.  */
  #define gPhyFSKPreambleLength_c   16u   /* [bytes] */
  /*! This define is used to set the length of the SFD in octets for the MRFSK PHY.  */
  #define gPhyMRFSKSFDLength_c      2u    /* [bytes] */
  /*! This define is used to limit the minimum number of octets for a packet to be considered valid. */
  #define gMinPHYPacketSize_c       5u
  /*! Maximum number of bytes that the PHY can transmit or receive */
  #define gMaxPHYPacketSize_c       254u
  /*! Number of bytes forming the CRC field */
  #define gPhyFCSSize_c             2u
  /*! The duration of the Clear Channel Assessment (in symbols) */
  #define gCCADurationDefault_c     13u
  /*! This define is used to set the duration of the synchronization header (in symbols) */
  #define gPhySHRDuration_c        (gPhySymbolsPerOctet_c * (gPhyFSKPreambleLength_c + gPhyMRFSKSFDLength_c))
  /*! The maximum over-the-air duration of a frame (in symbols) */
  #define gPhyMaxFrameDuration_c   (gPhySHRDuration_c + (gPhyMRFSKPHRLength_c + gMaxPHYPacketSize_c) * gPhySymbolsPerOctet_c)  /* [symbols] 802.15.4g page 48 formula  */
#else
  /*! The duration of the Clear Channel Assessment (in symbols) */
  #define gCCATime_c                8u
  /*! RX-to-TX or TX-to-RX maximum turnaround time (in symbols) */
  #define gPhyTurnaroundTime_c      12u
  /*! Minimum number of bytes forming a PHY packet */
  #define gMinPHYPacketSize_c       5u
  /*! Maximum number of bytes that the PHY can transmit or receive */
  #define gMaxPHYPacketSize_c       (127u)
  /*! This define is used to set the duration of the synchronization header (in symbols) */
  #define gPhySHRDuration_c         (10u)
  /*! This define is used to set the number of symbols per octet for the current PHY. */
  #define gPhySymbolsPerOctet_c     (2u)
  /*! Number of bytes forming the CRC field */
  #define gPhyFCSSize_c             (2u)
  /*! The maximum over-the-air duration of a frame (in symbols) */
  #define gPhyMaxFrameDuration_c    (gPhySHRDuration_c + (gMaxPHYPacketSize_c + 1) * gPhySymbolsPerOctet_c)
  /*! The duration of a Back-off period (in symbols) */
  #define gUnitBackoffPeriod_c      (20u)
#endif  /* gPHY_802_15_4g_d */

/*! PHY flag that reflects the state of the RxOnWhenIdle function */
#define gPhyFlagRxOnWhenIdle_c  (1u << 0)
/*! PHY flag that indicate that an automatic RX sequence is ongoing */
#define gPhyFlagIdleRx_c        (1u << 1)
/*! PHY flag that reflects the state of the FP bit of the last ACK frame received */
#define gPhyFlagRxFP_c          (1u << 2)
/*! PHY flag that reflects the state of the FP bit of the last ACK frame transmitted */
#define gPhyFlagTxAckFP_c       (1u << 3)
/*! If set, a TX operation may be postponed if a packet reception is in progress */
#define gPhyFlagDeferTx_c       (1u << 4)
/*! PHY flag that indicate that the requested TX has been postponed */
#define gPhyFlaqReqPostponed_c  (1u << 5)


#ifdef gPHY_802_15_4g_d
/*! The following define is used to configure the receiver timeout when AFC is enabled, and no SFD is received (in symbols) */
#ifndef gAfcRxTimeout_c
#define gAfcRxTimeout_c             (gPhySHRDuration_c)
#endif

/*! The following define is used to configure the default receiver sensitivity threshold. */
#ifndef gRssiThreshold_c
#define gRssiThreshold_c        0xB4 /* -90 dBm */
#endif

/*! Default 802.15.4g PHY mode */
#ifndef gPhyModeDefault_d
#define gPhyModeDefault_d gPhyMode1_c
#endif
#endif /* #ifdef gPHY_802_15_4g_d */

/*! This constant defines the identification value of an invalid timer. */
#define gInvalidTimerId_c (0xFF)

/*! \cond DOXY_SKIP_TAG */
#ifdef gPHY_802_15_4g_d
#define gPhyTimeShift_c   (16)          /* 16bit hw timer */
#define gPhyTimeMask_c    (0x000000000000FFFF)
#else
#define gPhyTimeShift_c   (24)          /* 24bit hw timer */
#define gPhyTimeMask_c    (0x00FFFFFF)
#endif
/*! \endcond */

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! This type enumerates all the possible statuses of primitives that require passing a status to the MAC Layer. */
typedef enum
{
    gPhyChannelBusy_c          = 0x00,    /*!< The CCA attempt has detected a busy channel.*/
    gPhyBusyRx_c               = 0x01,    /*!< The transceiver is asked to change its state while receiving.*/
    gPhyBusyTx_c               = 0x02,    /*!< The transceiver is asked to change its state while transmitting.*/
    gPhyChannelIdle_c          = 0x04,    /*!< The CCA attempt has detected an idle channel.*/
    gPhyInvalidParameter_c     = 0x05,    /*!< A SET request was issued with a parameter in the primitive that is out of the valid range.*/
    gPhyRxOn_c                 = 0x06,    /*!< The transceiver is in the receiver enabled state.*/
    gPhySuccess_c              = 0x07,    /*!< A SET/GET, an ED operation, a data request, an indirect queue insert, or a transceiver state change was successful.*/
    gPhyTRxOff_c               = 0x08,    /*!< The transceiver is in the transceiver disabled state.*/
    gPhyTxOn_c                 = 0x09,    /*!< The transceiver is in the transmitter enabled state.*/
    gPhyUnsupportedAttribute_c = 0x0a,    /*!< A SET/GET request was issued with the identifier of an attribute that is not supported.*/
    gPhyReadOnly_c             = 0x0b,    /*!< A SET request was issued with the identifier of an attribute that is read-only.*/
    gPhyIndexUsed_c            = 0x11,    /*!< The indirect queue insert operation has detected an used index.*/
    gPhyNoAck_c                = 0x14,    /*!< No ACK was received for the last transmission.*/
    gPhyFramePending_c         = 0x15,	  /*!< The ACK of a Data Request frame indicates a pending frame in the coordinator’s indirect TX queue.*/
    gPhyBusy_c                 = 0xF1,    /*!< The current request cannot be handled, because the PHY is busy. */
    gPhyInvalidPrimitive_c     = 0xF2     /*!< The set was not completed, because the primitive is not in the valid range. */
}phyStatus_t;

/*! This type enumerates the possible transmission modes in respect to slotted or unslotted modes. */
typedef enum
{
    gPhySlottedMode_c      = 0x0c,
    gPhyUnslottedMode_c    = 0x0d
}phySlottedMode_t;

/*! This type is used to indicate, whether the CCA operations are required before the transmissions, and together with phySlottedTx_t to determine, whether there are more than one needed. */
typedef enum
{
    gPhyEnergyDetectMode_c = 0x00,
    gPhyCCAMode1_c         = 0x01, /*!< Energy above a threshold */
    gPhyCCAMode2_c         = 0x02, /*!< Carrier sense only */
    gPhyCCAMode3_c         = 0x03, /*!< CCA mode 1 + CCA mode 2 */
    gPhyNoCCABeforeTx_c    = 0x04
}phyCCAType_t;

typedef enum
{
    gPhyContCcaEnabled     = 0x00,
    gPhyContCcaDisabled
}phyContCCAMode_t;

/*! This type is used to enumerate the possible states to set the transceiver to. \
    Setting the transceiver into any Tx state is done by issuing pdDataReq_t that does not use this type for any member. */
typedef enum
{
    gPhyForceTRxOff_c = 0x03,              /*!< The transceiver is to be switched off immediately.*/
    gPhySetRxOn_c     = 0x12,              /*!< The transceiver is to be configured into the receiver enabled state.*/
    gPhySetTRxOff_c   = 0x13,              /*!< The transceiver is to be configured into the transceiver disabled state.*/
}phyState_t;

/*! This type is used to filter the next received frames, and to accept only Ack frames. */
typedef enum
{
    gPhyRxAckRqd_c     = 0x00,             /*!< A receive Ack frame is expected to follow the transmit frame (non-Ack frames are rejected)*/    
    gPhyNoAckRqd_c     = 0x01,             /*!< An ordinary receive frame (any type of frame) follows the transmit frame*/
    gPhyEnhancedAckReq = 0x02
}phyAckRequired_t;

/*! This type enumerates all PHY PIB IDs. 
    The PIBs can be read and written to by the upper layer, and are used to configure certain parameters and modes of operation for the PHY Layer. 
    The PIBs that refer to physical parameters, such as carrier frequency or transmission power, are usually mirrored in hardware, but in case the 
    IEEE 802.15.4 hardware acceleration is used, there are protocol-oriented PIBs also stored in hardware, such as addresses, PAN ID, promiscuous mode setting, and so on. */
typedef enum
{
    gPhyPibCurrentChannel_c  = 0x00,       /*!< The channel currently used.*/
    gPhyPibCurrentPage_c     = 0x01,       /*!< The channel page currently used.*/
    gPhyPibTransmitPower_c   = 0x02,       /*!< The power used for TX operations.*/
    gPhyPibLongAddress_c     = 0x03,       /*!< The MAC long address to be used by the PHY’s source address matching feature.*/
    gPhyPibShortAddress_c    = 0x04,       /*!< The MAC short address to be used by the PHY’s source address matching feature.*/
    gPhyPibPanId_c           = 0x05,       /*!< The MAC PAN ID to be used by the PHY’s source address matching feature.*/
    gPhyPibPanCoordinator_c  = 0x06,       /*!< Indicates if the device is a PAN coordinator or not.*/
    gPhyPibSrcAddrEnable_c   = 0x07,       /*!< Enables or disables the PHY’s source address matching feature.*/
    gPhyPibPromiscuousMode_c = 0x08,       /*!< Selects between normal, promiscuous and active promiscuous mode.*/
    gPhyPibAutoAckEnable_c   = 0x09,       /*!< Enables or disables automatic transmission of ACK frames.*/
    gPhyPibFrameVersion_c    = 0x0A,       /*!< Used in checking for allowed frame versions (0x00 - any version accepted, 0x01 - accept Frame Version 0 packets (2003 compliant), 0x02 - accept Frame Version 1 packets (2006 compliant), 0x03 - accept Frame Version 0 and 1 packets).*/
    gPhyPibFrameEnable_c     = 0x0B,       /*!< Used for enabling or disabling reception of MAC frames.*/
    gPhyPibAckFramePending_c = 0x0C,       /*!< Used to copy its contents to the outgoing ACK frame's Frame Pending field as a response to a received Data Request frame with Source Address Matching disabled.*/
    gPhyPibRxOnWhenIdle      = 0x0D,       /*!< Enable RX when the radio is IDLE*/  
    gPhyPibFrameWaitTime_c   = 0x0E,       /*!< The number of symbols the RX should be on after receiving an ACK with FP=1 */
    gPhyPibDeferTxIfRxBusy_c = 0x0F,       /*!< If a packet is being received, transmission is deferred */
    gPhyPibLastTxAckFP_c     = 0x10,       /*!< Indicates, whether the last ACK sent to the MAC Data Request had the frame pending flag set. */
#ifdef gPHY_802_15_4g_d      
    gPhyPibCurrentMode_c            = 0x20, /*!< Used to set or get the current operating PHY mode. Only for Sub-1 GHz PHY. */
    gPhyPibFSKPreambleRepetitions_c = 0x21, /*!< Used to set or get the number of 1 octet patterns in the preamble. Only for Sub-1 GHz PHY. */
    gPhyPibFSKScramblePSDU_c        = 0x22, /*!< Enables or disables the data whitening feature. Only for Sub-1 GHz PHY. */
    gPhyPibCCADuration_c            = 0x23, /*!< Set or get the CCA duration specified in symbols. The valid range is 8 – 1000. Only for Sub-1 GHz PHY. */
    gPhyPibCSLRxEnabled_c           = 0x24, /*!< Enable or disable the CSL mode for the RX sequences.  */
    gPhyPibFreqBandId_c             = 0x25, /*!< Sub-1 GHz Frequency Band Identifier. */
    gPhyPibAckWaitDuration_c        = 0x26, /*!< ACK receiver on period. */
    gPhyPibTschEnabled_c            = 0x27, /*!< Enable or disable TSCH. */
    gPhyPibTschTimeslotInfo_c       = 0x28  /*!< The TSCH timings inside a timeslot. */
#endif  /* gPHY_802_15_4g_d */
}phyPibId_t;

typedef union phyFlags_tag
{
    uint32_t mask;
    struct{
        uint32_t     rxOnWhenIdle            :1;
        uint32_t     rxFramePending          :1;
        uint32_t     idleRx                  :1;
#ifdef  gPHY_802_15_4g_d
        uint32_t     ccaBfrTX                :1;
        uint32_t     rxAckRqd                :1;
        uint32_t     autoAck                 :1;
        uint32_t     panCordntr              :1;
        uint32_t     promiscuous             :1;
        uint32_t     activePromiscuous       :1;
        uint32_t     cslRxEnabled            :1;
        uint32_t     rxEnhAckRqd             :1;
        uint32_t     ccaComplete             :1;
        uint32_t     tschEnabled             :1;
        uint32_t     filterFail              :1;
        uint32_t     rxIsListen              :1;
        uint32_t     reserved                :17;            
#else
        uint32_t     phyState                :3;
        uint32_t     reserved                :26;
#endif  /* gPHY_802_15_4g_d */
    };
}phyFlags_t;

/*!< PHY Time event Id */
typedef uint8_t  phyTimeTimerId_t;
/*!< PHY Time time-stamp */
typedef uint64_t phyTime_t;
/*!< PHY Time event callback type */
typedef void (*phyTimeCallback_t) ( uint32_t param );

typedef enum
{
    gPhyTimeOk_c               = 0x00, /*!< The request was performed successfully */
    gPhyTimeAlreadyPassed_c    = 0x01, /*!< Requested time for event trigger has already passed */
    gPhyTimeTooClose_c         = 0x02, /*!< Requested time for event trigger is too close to the current time to be processed */
    gPhyTimeTooMany_c          = 0x03, /*!< Too many requests have been addressed */
    gPhyTimeInvalidParameter_c = 0x04, /*!< The request was performed with an invalid parameter */
    gPhyTimeNotFound_c         = 0x05, /*!< The requested timer was not found */
    gPhyTimeError_c            = 0x06  /*!< The request encountered a hardware error */
}phyTimeStatus_t;

#ifdef gPHY_802_15_4g_d
/*! Used internally by the Sub-1 GHz PHY to store the PHY Header before sending it to the transceiver. */
typedef union phyPHR_tag
{
    uint16_t mask;
    uint8_t  byteAccess[2];
    struct{
        uint8_t     modeSwitch          :1; /*!< Set to 1 to indicate that a mode switch shall occur. */
        uint8_t     reserved            :2;
        uint8_t     fcsType             :1; /*!< Set to 0 corresponds to a 4-octet FCS. */
        uint8_t     dataWhitening       :1; /*!< Set to 1 when data whitening is used. */
        uint8_t     frameLengthRsvd     :3; /*!< Max psdu 254 */
        uint8_t     frameLength;
    };
}phyPHR_t;

/*! This type is used to enumerate all the PHY modes available. 
    The available PHY modes depend on the selected Frequency Band (See 0 – IEEE Standard for Local and metropolitan area networks). */
typedef enum{
    gPhyMode1_c = 0x00, 
    gPhyMode2_c = 0x01, 
    gPhyMode3_c = 0x02,
    gPhyMode4_c = 0x03,
    /* Non-standard Phy Modes */
    gPhyMode1ARIB_c = 0x04,
    gPhyMode2ARIB_c = 0x05,
    gPhyMode3ARIB_c = 0x06, 
}phyMode_t;

/*! 802.15.4g MAC sub-GHz frequency bands */
typedef enum{
    gFreq169_400__169_475MHz_c = 0x00,    /*!<  169.400-169.475   (Europe) */
    gFreq470__510MHz_c = 0x02,    /*!<  470-510   (China)  */
    gFreq779__787MHz_c = 0x03,    /*!<  779-787   (China)  */
    gFreq863__870MHz_c = 0x04,    /*!<  863-870   (Europe) */
    gFreq902__928MHz_c = 0x07,    /*!<  902-928   (U.S.)   */
    gFreq920__928MHz_c = 0x09,    /*!<  920-928   (Japan) - Includes ARIB modes */
    gFreq865__867MHz_c = 0x0E,    /*!<  865-867   (India) - Non-standard */
}phyFreqBand_t;

typedef struct phyTschTsInfo_tag{
    uint16_t tsCCA;
    uint16_t tsRxTx;
    uint16_t tsRxAckDelay;
    uint16_t tsAckWait;
}phyTschTsInfo_t;
#endif  /* gPHY_802_15_4g_d */


#endif  /* _PHY_TYPES_H */
/*! *********************************************************************************
* @}
********************************************************************************** */