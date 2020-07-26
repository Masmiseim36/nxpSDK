/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* NXPNFCRDLIB Configuration File, User can change configuration based on use cases and system needs.
* Changes in these configuration leads to change in flash size and behavior of NxpNfcRdLib.
* $Author: Purnank G (ing05193) $
* $Revision: 5076 $ (v06.01.00)
* $Date: 2016-10-05 17:29:09 +0530 (Mon, 13 Jun 2016) $
*
*/

#ifndef PH_NXPNFCRDLIB_CONFIG_H
#define PH_NXPNFCRDLIB_CONFIG_H

#include "ph_NxpBuild.h"

/* Used in NFCLIB (Simplified API). */

/**< Maximum HAL Tx buffer size by allocated by Simplified API Layer. */
#if !defined(PH_NXPNFCRDLIB_CONFIG_HAL_TX_BUFFSIZE) && !defined(NXPBUILD__PHHAL_HW_RC663) && !defined(NXPBUILD__PHHAL_HW_PN5190)
    #define PH_NXPNFCRDLIB_CONFIG_HAL_TX_BUFFSIZE                    300U
#endif
#if !defined(PH_NXPNFCRDLIB_CONFIG_HAL_TX_BUFFSIZE) && (defined(NXPBUILD__PHHAL_HW_RC663) || defined(NXPBUILD__PHHAL_HW_PN5190))
    #define PH_NXPNFCRDLIB_CONFIG_HAL_TX_BUFFSIZE                    1030U
#endif

/**< Maximum HAL Rx buffer size by allocated by Simplified API Layer. */
#if !defined(PH_NXPNFCRDLIB_CONFIG_HAL_RX_BUFFSIZE) && !defined(NXPBUILD__PHHAL_HW_RC663) && !defined(NXPBUILD__PHHAL_HW_PN5190)
    #define PH_NXPNFCRDLIB_CONFIG_HAL_RX_BUFFSIZE                    600U
#endif
#if !defined(PH_NXPNFCRDLIB_CONFIG_HAL_RX_BUFFSIZE) && (defined(NXPBUILD__PHHAL_HW_RC663) || defined(NXPBUILD__PHHAL_HW_PN5190))
    #define PH_NXPNFCRDLIB_CONFIG_HAL_RX_BUFFSIZE                    2060U
#endif

#define PH_NXPNFCRDLIB_CONFIG_ATS_BUFF_LENGTH                    64U       /**< Maximum ATS response buffer length. */
#define PH_NXPNFCRDLIB_CONFIG_HCE_BUFF_LENGTH                    300U      /**< Buffer length for HCE used when UpdateBinary or Custom commands are supported. */

/* Used in Discovery Loop. */
#define PH_NXPNFCRDLIB_CONFIG_MAX_PROXIMITY_CARDS_SUPPORTED      0x01U    /**< Maximum proximity cards (Type A, Type B and Type F) detected by Discovery Loop. */
#define PH_NXPNFCRDLIB_CONFIG_MAX_VICINITY_CARDS_SUPPORTED       0x01U    /**< Maximum vicinity cards (ISO15693 and ISO18000p3m3) detected by Discovery Loop. */

/* Default guard times used in Discovery Loop and NFCLIB (Simplified API).
 * As per Nfc Forum Activity and EMVCo Specification. */
#define PH_NXPNFCRDLIB_CONFIG_TYPEA_GT                           5100U    /**< Guard time configuration for Type A poll in Micro seconds.*/
#define PH_NXPNFCRDLIB_CONFIG_TYPEB_GT                           5100U    /**< Guard time configuration for Type B poll in Micro seconds.*/
#define PH_NXPNFCRDLIB_CONFIG_TYPEF_GT                           20400U   /**< Guard time configuration for Type F poll in Micro seconds.*/
#define PH_NXPNFCRDLIB_CONFIG_B_TO_F_GT                          15300U   /**< Guard time configuration for Type F poll preceded by Type B poll. */
#define PH_NXPNFCRDLIB_CONFIG_TYPEV_GT                           5200U    /**< Guard time configuration for Type V poll in Micro seconds.*/
#define PH_NXPNFCRDLIB_CONFIG_I18000P3M3_GT                      10000U   /**< Guard time configuration for 18000p3m3 poll in Micro seconds.*/

/* Used in Discovery Loop and NFCLIB (Simplified API) as per EMVCo Specification. */
#define PH_NXPNFCRDLIB_CONFIG_EMVCO_FIELD_OFF_DELAY_US           5100U    /**< Wait time tP as per EMVCo specification. Simplified API EMVCo profile uses                                                                                            this value for Card removal procedure. */
#define PH_NXPNFCRDLIB_CONFIG_EMVCO_REMOVAL_RETRY_COUNT          3U       /**< Poll command retry count used in removal procedure of EMVCo profile. */

#define PH_NXPNFCRDLIB_CONFIG_DEFAULT_FSCI_VALUE                 8U       /**< Default FSDI value. */

#if (defined(NXPBUILD__PHHAL_HW_RC663) || defined(NXPBUILD__PHHAL_HW_PN5190)) && defined(PH_NXPNFCRDLIB_CONFIG_HIGHER_FSDI)
    #define PH_NXPNFCRDLIB_CONFIG_FSDI_VALUE                     10U      /**< With RC663 and PN5190 we can configure the required FSDI value. But as frame error detection with CRC16 is not good beyond 1KBytes Limiting the value to 10. */
    #define PH_NXPNFCRDLIB_CONFIG_FSCI_VALUE                     10U      /**< With RC663 and PN5190 we can configure the required FSCI value. But as frame error detection with CRC16 is not good beyond 1KBytes Limiting the value to 10. */
#endif

#ifndef PH_NXPNFCRDLIB_CONFIG_FSDI_VALUE
    #define PH_NXPNFCRDLIB_CONFIG_FSDI_VALUE                     8U       /**< Default FSDI Configuration. This value is fixed for PN5180 and PN7462AU as these Readers doesn't support frame size. */
#endif /* PH_NXPNFCRDLIB_CONFIG_FSDI_VALUE */

#ifndef PH_NXPNFCRDLIB_CONFIG_FSCI_VALUE
    #define PH_NXPNFCRDLIB_CONFIG_FSCI_VALUE                     PH_NXPNFCRDLIB_CONFIG_DEFAULT_FSCI_VALUE   /**< Default FSCI Configuration. This value is fixed for PN5180 and PN7462AU as these Readers doesn't support frame size. */
#endif /* PH_NXPNFCRDLIB_CONFIG_FSCI_VALUE */

/* TODO : Max retry of RATS and Attrib can be 2 in case of EMVCo. Need to check. */

/* EMVCo values used in PAL 14443-3a, PAL 14443-4a, PAL 14443-3b and Discovery Loop. */
#define PH_NXPNFCRDLIB_CONFIG_EMVCO_TMIN_RETRANSMISSION          3000U    /**< Tmin retransmission used to retransmit command in case of timeout error as per EMVCo 2.6 req 9.6.1.3.*/
#define PH_NXPNFCRDLIB_CONFIG_EMVCO_DEFAULT_RETRANSMISSION       PH_NXPNFCRDLIB_CONFIG_EMVCO_TMIN_RETRANSMISSION    /**< Default retransmission time to retransmit command in case of timeout error.*/
#define PH_NXPNFCRDLIB_CONFIG_EMVCO_RETRYCOUNT                   2U       /**< Retry count used to retransmit command in case of timeout error as per EMVCo 2.6 req 9.6.1.3.*/

/* User should update this macro with expected NDEF length that can be read from the tag.
 * For which buffer of this length needs to be allocated properly by User and pass this buffer to phalTop_ReadNdef() API.
 * */
#define PH_NXPNFCRDLIB_CONFIG_MAX_NDEF_DATA                      0x100U    /**< Default value is 256 bytes, Maximum NDEF DATA that can be exchanged at a time, Application Buffer for NDEF Exchange should equal this */


/** \name HAL Rc663 macros
 */

/** \name Memory related constants
*/
/*@{*/
#define PH_NXPNFCRDLIB_CONFIG_RC663_PRECACHED_255BYTES           0xFFU    /**< Maximum number of bytes to precache into FIFO before triggering a command for 255 FIFO. */
#define PH_NXPNFCRDLIB_CONFIG_RC663_PRECACHED_512BYTES           0x200U   /**< Maximum number of bytes to precache into FIFO before triggering a command for 512 FIFO. */
#define PH_NXPNFCRDLIB_CONFIG_RC663_DEFAULT_PRECACHED_BYTES      PH_NXPNFCRDLIB_CONFIG_RC663_PRECACHED_255BYTES /**< Default number of bytes to precache into FIFO. */
/*@}*/

/**
* \name FIFO configuration values
*/
/*@{*/
#define PH_NXPNFCRDLIB_CONFIG_RC663_VALUE_FIFOSIZE_255           0x0000U  /**< Configure FIFO-Size to 256 bytes */
#define PH_NXPNFCRDLIB_CONFIG_RC663_VALUE_FIFOSIZE_512           0x0001U  /**< Configure FIFO-Size to 512 bytes */
#define PH_NXPNFCRDLIB_CONFIG_RC663_DEFAULT_FIFOSIZE             PH_NXPNFCRDLIB_CONFIG_RC663_VALUE_FIFOSIZE_255 /**< Default FIFO-Size */
/*@}*/

#endif /* PH_NXPNFCRDLIB_CONFIG_H */
