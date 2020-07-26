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
* Simplified API ISO 18000.
* $Author: Ashish Pal (nxp79566) $
* $Revision: 4423 $ (v06.01.00)
* $Date: 2016-16-09 11:50:54 +0530 (Fri, 22 Apr 2016) $
*
* History:
*
*
*/

#ifndef EX11_SIMPLIFIEDAPI_MFC_H
#define EX11_SIMPLIFIEDAPI_MFC_H

/* READ takes the block number to Read */
/* bBlockNo can take any value between 0x00-0xFF depending upon card size */
#define MFC_READ(bBlockNo) { \
            phNfcLib_TransmitInput.phNfcLib_MifareClassic.bBlockNumber = bBlockNo; \
            phNfcLib_TransmitInput.phNfcLib_MifareClassic.bCommand = Read; \
                        }

/* WRITE takes the block number and data to Write */
/* bBlockNo can take any value between 0x00-0xFF depending upon card size and the pBlockData is the pointer to 16 bytes data */
#define MFC_WRITE(bBlockNo, pBlockData) {\
		phNfcLib_TransmitInput.phNfcLib_MifareClassic.bBlockNumber = bBlockNo; \
		phNfcLib_TransmitInput.phNfcLib_MifareClassic.pBuffer = pBlockData; \
		phNfcLib_TransmitInput.phNfcLib_MifareClassic.bCommand = Write; \
		                }

/* INCREMENT takes the block number of the value block and the value with which to increment the block */
/* bBlockNo can take any value between 0x00-0xFF depending upon card size and should be value block*/
/* pValue takes pointer to a 4 byte value data */
#define MFC_INCREMENT(bBlockNo, pValue) { \
		    phNfcLib_TransmitInput.phNfcLib_MifareClassic.bBlockNumber = bBlockNo; \
		    phNfcLib_TransmitInput.phNfcLib_MifareClassic.pBuffer = pValue; \
		     phNfcLib_TransmitInput.phNfcLib_MifareClassic.bCommand = Increment; \
		    }

/* DECREMENT takes the block number of the value block and the value with which to decrement the block */
/* bBlockNo can take any value between 0x00-0xFF depending upon card size and should be value block*/
/* pValue takes pointer to a 4 byte value data */
#define MFC_DECREMENT(bBlockNo, pValue) { \
    phNfcLib_TransmitInput.phNfcLib_MifareClassic.bBlockNumber = bBlockNo; \
    phNfcLib_TransmitInput.phNfcLib_MifareClassic.pBuffer = pValue; \
    phNfcLib_TransmitInput.phNfcLib_MifareClassic.bCommand = Decrement; \
		    }

/* TRANSFER takes the block number of the value block where the transfer buffer should be transferred */
/* bBlockNo can take any value between 0x00-0xFF depending upon card size and should be value block*/
#define MFC_TRANSFER(bBlockNo)  { \
            phNfcLib_TransmitInput.phNfcLib_MifareClassic.bBlockNumber = bBlockNo; \
            phNfcLib_TransmitInput.phNfcLib_MifareClassic.bCommand = Transfer; \
		    }

/* RESTORE takes the block number of the value block  the transfer buffer shall be restored from*/
/* bBlockNo can take any value between 0x00-0xFF depending upon card size and should be value block*/
#define MFC_RESTORE(bBlockNo) {\
            phNfcLib_TransmitInput.phNfcLib_MifareClassic.bBlockNumber = bBlockNo; \
            phNfcLib_TransmitInput.phNfcLib_MifareClassic.bCommand = Restore; \
		    }

/* AUTHENTICATE takes the block number, key type, key number and  key version of the Key with which to authenticate */
/* bBlockNo can take any value between 0x00-0xFF constraint to card memory, bKeyType can be #PHAL_MFC_KEYA or #PHAL_MFC_KEYB */
/* bKeyNumber and wKeyVersion of the Keystore depends upon the size of keystore and size is 2 for current settings */
#define MFC_AUTHENTICATE(bBlockNo, bKeytype, bKeyNo, wKeyVersion) { \
    phNfcLib_TransmitInput.phNfcLib_MifareClassic.bBlockNumber = bBlockNo; \
    phNfcLib_TransmitInput.phNfcLib_MifareClassic.bKeyType = bKeytype; \
    phNfcLib_TransmitInput.phNfcLib_MifareClassic.bKeyNumber = bKeyNo; \
    phNfcLib_TransmitInput.phNfcLib_MifareClassic.bKeyVersion = wKeyVersion; \
    phNfcLib_TransmitInput.phNfcLib_MifareClassic.bCommand = Authenticate; \
        }

/* PERSONALIZEUID takes the UID type with which to personalize the presented card */
/* bUidType can be PHAL_MFC_UID_TYPE_UIDF0 or PHAL_MFC_UID_TYPE_UIDF1 or PHAL_MFC_UID_TYPE_UIDF2 or PHAL_MFC_UID_TYPE_UIDF3 */
#define MFC_PERSONALIZEUID(bUidType)   { \
		        }

#endif /* EX11_SIMPLIFIEDAPI_MFC_H */
