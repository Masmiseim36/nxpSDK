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
* Simplified API ISO MFUL.
* $Author: Ashish Pal (nxp79566) $
* $Revision: 4423 $ (v06.01.00)
* $Date: 2016-16-09 11:50:54 +0530 (Fri, 22 Apr 2016) $
*
* History:
*
*
*/

#ifndef EX11_SIMPLIFIEDAPI_ISO_MFUL_H
#define EX11_SIMPLIFIEDAPI_ISO_MFUL_H

/* MFUL_READ takes the page no to read */
/* bPageNo can be 0x00 - 0xFF depending upon card layout */
#define MFUL_READ(bPageNo) { \
    phNfcLib_TransmitInput.phNfcLib_MifareUltraLight.bPageNumber = bPageNo; \
    phNfcLib_TransmitInput.phNfcLib_MifareUltraLight.bCommand = MFUL_Read; \
		                    }

/* MFUL_WRITE takes the page no and data to write */
/* bPageNo can be 0x00 - 0xFF depending upon card layout, pData is a pointer to 4 byte page data */
#define MFUL_WRITE(bPageNo, pData) { \
    phNfcLib_TransmitInput.phNfcLib_MifareUltraLight.bPageNumber = bPageNo; \
    phNfcLib_TransmitInput.phNfcLib_MifareUltraLight.pBuffer = pData; \
    phNfcLib_TransmitInput.phNfcLib_MifareUltraLight.bCommand = MFUL_Write; \
		                           }

/* MFUL_COMPATIBILITY_WRITE takes the page no and data to write */
/* bPageNo can be 0x00 - 0xFF depending upon card layout, pData is a pointer to 16 byte page data */
#define MFUL_COMPATIBILITY_WRITE(bPageNo, pData) { \
    phNfcLib_TransmitInput.phNfcLib_MifareUltraLight.bPageNumber = bPageNo; \
    phNfcLib_TransmitInput.phNfcLib_MifareUltraLight.pBuffer = pData; \
    phNfcLib_TransmitInput.phNfcLib_MifareUltraLight.bCommand = MFUL_Compatibility_Write; \
                                   }


#endif /* EX11_SIMPLIFIEDAPI_ISO_MFUL_H */
