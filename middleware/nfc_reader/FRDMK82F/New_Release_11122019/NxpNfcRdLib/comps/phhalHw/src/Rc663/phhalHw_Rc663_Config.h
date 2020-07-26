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
* Internal definitions for Rc663 specific HAL-Component of Reader Library Framework.
* $Author: Purnank G (ing05193) $
* $Revision: 5076 $ (v06.01.00)
* $Date: 2016-06-13 17:29:09 +0530 (Mon, 13 Jun 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHHALHW_RC663_CONFIG_H
#define PHHALHW_RC663_CONFIG_H

#include <ph_Status.h>
#include <phhalHw.h>

/** \defgroup phhalHw_Rc663_Config Configuration
* \brief Custom configuration related to NFC IC Hardware settings.
* @{
*/

/** \name Modulation indices
*/
/*@{*/
#define PHHAL_HW_RC663_MODINDEX_I14443B     0xCCU   /**< Default modulation index for ISO14443B. */
#define PHHAL_HW_RC663_MODINDEX_FELICA      0x17U   /**< Default modulation index for FeliCa. */
#define PHHAL_HW_RC663_MODINDEX_I15693      0x10U   /**< Default modulation index for ISO15693.  */
#define PHHAL_HW_RC663_MODINDEX_EPCUID      0x10U   /**< Default modulation index for EPC/UID. */
#define PHHAL_HW_RC663_MODINDEX_I18000P3M3  0x10U   /**< Default modulation index for ISO18000-3 Mode3. */
/*@}*/

/** \name RxThreshold values
*/
/*@{*/
#define PHHAL_HW_RC663_RXTHRESHOLD_I14443A_106          0x3FU   /**< RxThreshold for ISO14443A @ 106kbit/s. */
#define PHHAL_HW_RC663_RXTHRESHOLD_I14443A_212          0x3FU   /**< RxThreshold for ISO14443A @ 212kbit/s. */
#define PHHAL_HW_RC663_RXTHRESHOLD_I14443A_424          0x3FU   /**< RxThreshold for ISO14443A @ 424kbit/s. */
#define PHHAL_HW_RC663_RXTHRESHOLD_I14443A_848          0x3FU   /**< RxThreshold for ISO14443A @ 848kbit/s. */
#define PHHAL_HW_RC663_RXTHRESHOLD_I14443B_106          0x3FU   /**< RxThreshold for ISO14443B @ 106kbit/s. */
#define PHHAL_HW_RC663_RXTHRESHOLD_I14443B_212          0x3FU   /**< RxThreshold for ISO14443B @ 212kbit/s. */
#define PHHAL_HW_RC663_RXTHRESHOLD_I14443B_424          0x3FU   /**< RxThreshold for ISO14443B @ 424kbit/s. */
#define PHHAL_HW_RC663_RXTHRESHOLD_I14443B_848          0x3FU   /**< RxThreshold for ISO14443B @ 848kbit/s. */
#define PHHAL_HW_RC663_RXTHRESHOLD_FELICA_212           0x3FU   /**< RxThreshold for FeliCa @ 212kbit/s. */
#define PHHAL_HW_RC663_RXTHRESHOLD_FELICA_424           0x3FU   /**< RxThreshold for FeliCa @ 424kbit/s. */
#define PHHAL_HW_RC663_RXTHRESHOLD_I15693_FAST          0x44U   /**< RxThreshold for ISO15693 @ High. */
#define PHHAL_HW_RC663_RXTHRESHOLD_I15693_FAST_HIGH     0x44U   /**< RxThreshold for ISO15693 @ Fast_High. */
#define PHHAL_HW_RC663_RXTHRESHOLD_EPCUID               0x4EU   /**< RxThreshold for EPC/UID. */
#define PHHAL_HW_RC663_RXTHRESHOLD_I18000P3M3           0x36U   /**< RxThreshold for ISO18000-3 Mode3. */
/*@}*/

/** \name Macros used in LPCD.
*/
/*@{*/
#define PHHAL_HW_RC663_LPCD_RECEIVER_GAIN               0x03U    /**< Receiver Gain when LPCD CMD is executed. */
#define PHHAL_HW_RC663_FILTER_MASK                      0x00FFU  /**< Mask to get if Filter is Enable (#PH_ON) or disable (#PH_OFF). */
#define PHHAL_HW_RC663_FILTER_OPTION_MASK               0xFF00U  /**< Mask to get Filter option. */
#define PHHAL_HW_RC663_FILTER_BIT_OPTION1               0x10U    /**< Bit 4 of dataparams 'bLpcdOption' is used to store Filter option1. */
#define PHHAL_HW_RC663_FILTER_BIT_OPTION2               0x20U    /**< Bit 5 of dataparams 'bLpcdOption' is used to store Filter option2. */
/*@}*/

/** \name LPCD calibration constants
* Below values of Qmin, Qmax and Imin register contents signify below values for Q and I during calibration.
* Imin = Qmin = 0x00 and Imax = Qmax = 0x3F which are least and highest values that can be configured for I and Q.
*/
/*@{*/
#define PHHAL_HW_RC663_LPCD_CALIBRATE_QMIN_REG          0xC0U     /**< Load Qmin Reg with 0xC0 during calibration. */
#define PHHAL_HW_RC663_LPCD_CALIBRATE_QMAX_REG          0xFFU     /**< Load Qmax Reg with 0xFF during calibration. */
#define PHHAL_HW_RC663_LPCD_CALIBRATE_IMIN_REG          0xC0U     /**< Load Imin Reg with 0xC0 during calibration. */
/*@}*/


/** \name Feature selection
*/
/*@{*/
#define PHHAL_HW_RC663_FEATURE_FIFO_UNDERFLOW_CHECK /**< Checks for FIFO underflow during data transmission if defined. */
/*@}*/

/** @}
* end of phhalHw_Rc663_Config group
*/

#endif /* PHHALHW_RC663_CONFIG_H */
