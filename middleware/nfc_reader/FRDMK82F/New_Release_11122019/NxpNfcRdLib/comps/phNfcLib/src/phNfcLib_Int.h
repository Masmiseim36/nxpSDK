/*
*                     Copyright 2016-2019, NXP
*
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
* Internal functions of NFC Library Top Level API of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHNFCLIB_INT_H
#define PHNFCLIB_INT_H

#include <ph_Status.h>

#ifdef NXPBUILD__PHNFCLIB_PROFILES

typedef phStatus_t (*pphNfcLib_Activate_Profile)(uint16_t wTechnologyMask);
typedef phStatus_t (*pphNfcLib_Deactivate_Profile)(void);
typedef phStatus_t (*pphNfcLib_Transmit)(void * const  pTxBuffer, uint16_t wTxBufferLength);

/*
 * ATQ codes
 */
#define PH_NFCLIB_ATQA_UL        0x4400U
#define PH_NFCLIB_ATQA_ULC       0x4400U
#define PH_NFCLIB_ATQA_MFC       0x0200U
#define PH_NFCLIB_ATQA_MFP_S     0x0400U
#define PH_NFCLIB_ATQA_MFP_X     0x4200U
#define PH_NFCLIB_ATQA_DESFIRE   0x4403U
#define PH_NFCLIB_ATQA_DESFIRE1  0x0403U
#define PH_NFCLIB_ATQA_JCOP      0x0400U
#define PH_NFCLIB_ATQA_MINI      0x0400U
#define PH_NFCLIB_ATQA_NPA       0x0800U

/*
 * SAK codes
 */
#define PH_NFCLIB_SAK_UL         0x00U
#define PH_NFCLIB_SAK_ULC        0x00U //ATQA and SAK for UL and ULC are same.
#define PH_NFCLIB_SAK_MINI       0x09U
#define PH_NFCLIB_SAK_MFC_1K     0x08U
#define PH_NFCLIB_SAK_MFC_4K     0x18U
#define PH_NFCLIB_SAK_MFP_2K_SL1 0x08U
#define PH_NFCLIB_SAK_MFP_4K_SL1 0x18U
#define PH_NFCLIB_SAK_MFP_2K_SL2 0x10U
#define PH_NFCLIB_SAK_MFP_4K_SL2 0x11U
#define PH_NFCLIB_SAK_MFP_2K_SL3 0x20U
#define PH_NFCLIB_SAK_MFP_4K_SL3 0x20U
#define PH_NFCLIB_SAK_DESFIRE    0x20U //0x24U
#define PH_NFCLIB_SAK_JCOP       0x28U
#define PH_NFCLIB_SAK_LAYER4     0x20U
#define PH_NFCLIB_SAK_P2P        0x40U
#define PH_NFCLIB_SAK_MERGED     0x60U

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFC
phStatus_t phNfcLib_MFC_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFC */

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFUL
phStatus_t phNfcLib_MFUL_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFUL */

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFDF
phStatus_t phNfcLib_MFDF_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFDF */

#ifdef NXPBUILD__PH_NFCLIB_ISO_15693
phStatus_t phNfcLib_ISO15693_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
#endif /* NXPBUILD__PH_NFCLIB_ISO_15693 */

#ifdef NXPBUILD__PH_NFCLIB_ISO_18000
phStatus_t phNfcLib_ISO18000_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
#endif /* NXPBUILD__PH_NFCLIB_ISO_18000*/

#if (defined NXPBUILD__PH_NFCLIB_ISO_MFC || defined NXPBUILD__PH_NFCLIB_ISO_MFUL)
/* Utility API */
phStatus_t phNfcLib_Mifare_Transmit(uint8_t * pTxBuffer, uint16_t wTxLength);
#endif

#endif /* NXPBUILD__PHNFCLIB_PROFILES*/
#endif /* PHNFCLIB_INT_H */
