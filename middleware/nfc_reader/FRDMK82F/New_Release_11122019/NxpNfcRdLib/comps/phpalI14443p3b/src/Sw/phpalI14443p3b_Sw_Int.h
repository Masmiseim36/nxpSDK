/*
 *         Copyright (c), NXP Semiconductors Gratkorn / Austria
 *
 *                     (C)NXP Semiconductors
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/** \file
* Internal definitions for Software ISO14443-3B Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHPALI14443P3B_SW_INT_H
#define PHPALI14443P3B_SW_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

#define PHPAL_I14443P3B_SW_ATQB_TIME_US     600U     /**< TR0 + TR1 (ISO14443-2 9.2.5 75,5 + 94U,4 ~ 170 --> set 200 us). */
#define PHPAL_I14443P3B_SW_DELTA_POLL       17U      /**< T(B,POLL) (Digital Protocol Ver1.1 (Article 7.9.1.5) is 16.4ms = ~17ms). */
#define PHPAL_I14443P3B_SW_ATTRIB_TIME_US   4800U    /**< Answer to ATTRIB FWT (ISO14443-3 7.9.4.3). */

/** Anticollision prefix code */
#define PHPAL_I14443P3B_SW_APF                      0x05U

/** Wakeup bit within PARAM byte */
#define PHPAL_I14443P3B_SW_PARAM_WUP_BIT            0x08U

/** Extended ATQB bit within PARAM byte */
#define PHPAL_I14443P3B_SW_PARAM_EXTATQB_BIT        0x10U

/** HltB Command code */
#define PHPAL_I14443P3B_SW_HALT_CMD                 0x50U

/** HltB Response code */
#define PHPAL_I14443P3B_SW_HALT_RESP                0x00U

/** ATTRIB Command code */
#define PHPAL_I14443P3B_SW_ATTRIB_CMD               0x1DU

/** ATTRIB parameter1 default value */
#define PHPAL_I14443P3B_SW_ATTRIB_PARAM1_DEFAULT    0x00U

/** FWI default value */
#define PHPAL_I14443P3B_SW_FWI_DEFAULT              0x04U

/** SFGI default value */
#define PHPAL_I14443P3B_SW_SFGI_DEFAULT             0x00U

/** FSCI default value */
#define PHPAL_I14443P3B_SW_FSCI_DEFAULT             0x02U

/** FTW min in microseconds */
#define PHPAL_I14443P3B_SW_FWT_MIN_US               ((256*16)/13.56)

/** Time extension in microseconds for all commands.  */
#define PHPAL_I14443P3B_SW_EXT_TIME_US              63U

/** The minimum FSDI value as per EMVCo v3.0 - Requirements 5.9 */
#define PHPAL_I14443P3B_EMVCO_FRAMESIZE_MIN         8U

/* EMVCo:  Delta Time to support EMVCO Req */
#define PHPAL_I14443P3B_SW_FWT_EMVCO_MIN_US         29U

/** Minimum TxWait in case received TR2 value is (00)b. */
#define PHPAL_I14443P3B_SW_TXWAIT_US_TR2_0          133U

/** Minimum TxWait in case received TR2 value is (01)b. */
#define PHPAL_I14443P3B_SW_TXWAIT_US_TR2_1          246U

/** Minimum TxWait in case received TR2 value is (10)b. */
#define PHPAL_I14443P3B_SW_TXWAIT_US_TR2_2          397U

/** Minimum TxWait in case received TR2 value is (11)b. */
#define PHPAL_I14443P3B_SW_TXWAIT_US_TR2_3          699U

/** Default TxWait */
#define PHPAL_I14443P3B_SW_TXWAIT_US_DEFAULT        500U

/** Minimum TR2 Mask to extract TR2 value. */
#define PHPAL_I14443P3B_SW_MINIMUM_TR2_MASK         0x06U

/** TR2 position. */
#define PHPAL_I14443P3B_SW_MINIMUM_TR2_POS          1U

/** TR2 bit value (00)b. */
#define PHPAL_I14443P3B_SW_MINIMUM_TR2_VALUE_0      0U

/** TR2 bit value (01)b. */
#define PHPAL_I14443P3B_SW_MINIMUM_TR2_VALUE_1      1U

/** TR2 bit value (10)b. */
#define PHPAL_I14443P3B_SW_MINIMUM_TR2_VALUE_2      2U

/** TR2 bit value (11)b. */
#define PHPAL_I14443P3B_SW_MINIMUM_TR2_VALUE_3      3U

/** Delta TPCD max in microseconds */
#define PHPAL_I14443P3B_SW_DELTA_TPCD_US_MAX        16400U

/** Delta TPCD minimum in microseconds */
#define PHPAL_I14443P3B_SW_DELTA_TPCD_US_MIN        0U

/** Delta frame wait time (FWT) in microseconds = 49152/fc */
#define PHPAL_I14443P3B_SW_DELTA_FWT_US             3625U

/** Delta TPCD used for exchange (in microseconds) */
#define PHPAL_I14443P3B_SW_DELTA_TPCD_US            1000U

/**
* \brief Perform a ISO14443-3B Request/Wakeup command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalI14443p3b_Sw_RequestBEx(
                                        phpalI14443p3b_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                                        uint8_t bIsWakeUp,                              /**< [In] Performs ReqB if \c bIsWakeUp=0 and WupB if \c bIsWakeUp=1. */
                                        uint8_t bNumSlots,                              /**< [In] Number of slots. */
                                        uint8_t bAfi,                                   /**< [In] AFI; Application Family Indentifier. */
                                        uint8_t bExtAtqb,                               /**< [In] Enable Extended AtqB. */
                                        uint8_t * pAtqb,                                /**< [Out] AtqB; uint8_t[13]. */
                                        uint8_t * pAtqbLen                                /**< [Out] Length of received ATQB (12/13 bytes) */
                                        );

/**
* \brief Check the received answer to Request/Wakeup/SlotMarker command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalI14443p3b_Sw_CheckATQBEx(
    phpalI14443p3b_Sw_DataParams_t * pDataParams,
    uint8_t * pResp,
    uint16_t wRespLength,
    uint8_t * pAtqb,
    uint8_t * pAtqbLen
    );

/**
* \brief Set the baud rate on the reader device.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalI14443p3b_Sw_SetReaderBaudRateEx( phpalI14443p3b_Sw_DataParams_t * pDataParams );

/**
* \brief Decide which baud rate to use depending on the VICCs capabilities and the callers request.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalI14443p3b_Sw_DecideBaudRateEx( uint8_t bBitRateCapability, uint8_t* pbDri, uint8_t* pbDsi );

/** @}
* end of ph_Private
*/

#endif /* PHPALI14443P3A_SW_INT_H */

