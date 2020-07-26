/*
 *         Copyright (c), NXP Semiconductors
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
 * Application specific selection of Reader Library Components
 *
 * $Author: Purnank G (ing05193) $
 * $Revision: 5993 $ (v06.01.00)
 * $Date: 2016-09-19 13:50:28 +0530 (Mon, 19 Sep 2016) $
 *
 */

#ifndef INTFS_NFCRDLIB_SIMPLIFIEDAPI_EMVCO_H_
#define INTFS_NFCRDLIB_SIMPLIFIEDAPI_EMVCO_H_
#include <ph_Status.h>

#define PRETTY_PRINTING                             /**< Enable pretty printing */
#define MIN_VALID_DATA_SIZE                   6
#if defined(NXPBUILD__PHHAL_HW_RC663) || defined(NXPBUILD__PHHAL_HW_PN5190)
  #define PHAC_EMVCO_MAX_BUFFSIZE             1030  /**< Maximum buffer size for EMVCo as FSD is configured to 10 in case of RC663 and PN5190. */
#else
  #define PHAC_EMVCO_MAX_BUFFSIZE             600   /**< Maximum buffer size for EMVCo as FSD is configured to 8 in case of PN5180 and PN7462AU. */
#endif /* NXPBUILD__PHHAL_HW_RC663 */

typedef enum{
    eEmdRes_EOT = 0x70,
    eEmdRes_EOT_AnalogTests = 0x72,
    eEmdRes_SW_0 = 0x90,
    eEmdRes_SW_1 = 0x00,
}eEmvcoRespByte;

#ifdef PH_OSAL_FREERTOS
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define EMVCO_LOOPBACK_TASK_STACK              (2000/4)
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#ifdef __PN74XXXX__
#define EMVCO_LOOPBACK_TASK_STACK              (2000/4)
#else /*  __PN74XXXX__*/
#define EMVCO_LOOPBACK_TASK_STACK              (2000)
#endif /*  __PN74XXXX__*/
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define EMVCO_LOOPBACK_TASK_PRIO                4
#endif /* PH_OSAL_FREERTOS */

#ifdef PH_OSAL_LINUX
#define EMVCO_LOOPBACK_TASK_STACK              0x20000
#define EMVCO_LOOPBACK_TASK_PRIO               0
#endif /* PH_OSAL_LINUX */



#endif /* INTFS_NFCRDLIB_SIMPLIFIEDAPI_EMVCO_H_ */
