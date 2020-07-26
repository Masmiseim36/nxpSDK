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

#ifndef INTFS_NFCRDLIB_SIMPLIFIEDAPI_ISO_H_
#define INTFS_NFCRDLIB_SIMPLIFIEDAPI_ISO_H_
#include <phApp_Init.h>
#include <Nfcrdlib_SimplifiedApi_ISO_15693.h>
#include <Nfcrdlib_SimplifiedApi_ISO_18000.h>
#include <Nfcrdlib_SimplifiedApi_ISO_MFUL.h>
#include <Nfcrdlib_SimplifiedApi_ISO_MFC.h>

uint32_t NfcLib_MifareClassic_1k_Reference_app(void);
uint32_t NfcLib_ISO18000p3m3_Reference_app(void);
uint32_t NfcLib_ISO15693_Reference_app(void);
uint32_t NfcLib_MifareDesfire_Reference_app(void);
uint32_t NfcLib_MifareUltralight_Reference_app(void);
uint32_t NfcLib_TypeB_Reference_app(void);
uint32_t NfcLib_Layer4TypeA_Reference_app(void);


#ifdef PH_OSAL_FREERTOS
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define SIMPLIFIED_ISO_STACK    (2000/4)
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#ifdef __PN74XXXX__
#define SIMPLIFIED_ISO_STACK    (2000/4)
#else /*  __PN74XXXX__*/
#define SIMPLIFIED_ISO_STACK    (2000)
#endif /*  __PN74XXXX__*/
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION*/
#define SIMPLIFIED_ISO_PRIO     4
#endif /* PH_OSAL_FREERTOS */

#ifdef PH_OSAL_LINUX
#define SIMPLIFIED_ISO_PRIO     0
#define SIMPLIFIED_ISO_STACK    0x20000
#endif /* PH_OSAL_LINUX*/


#endif /* INTFS_NFCRDLIB_SIMPLIFIEDAPI_ISO_H_ */
