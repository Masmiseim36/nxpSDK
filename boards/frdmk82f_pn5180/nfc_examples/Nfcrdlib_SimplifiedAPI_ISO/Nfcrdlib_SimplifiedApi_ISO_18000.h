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

#ifndef EX11_SIMPLIFIEDAPI_ISO_18000_H
#define EX11_SIMPLIFIEDAPI_ISO_18000_H


/* ACK does not take any parameter and uses the handle present in context */
#define I18000_ACK() {       \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_Ack; \
                }

/* REQRN takes the option with which to perform the REQRn Command */
/* bOpt can be only PHAL_I18000P3M3_REQRN_USE_HANDLE here */
#define I18000_REQRN(bOpt) { \
                phNfcLib_TransmitInput.phNfcLib_ISO18000.bOption = bOpt; \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_ReqRn; \
                }

/* READ takes the Memory Bank to read, address of the word to read, length of the word ptr and No of words to read */
/* bMembank can be 0x00 - 0x03 , pWordptr is user choice limited to tag memory size */
/* bWordPtrLength can be 0x00 - 0x03 and depends on the TAG memory size , bWordcount is the number of words to read */
#define I18000_READ(bMembank, pWordptr, bWordPtrlength, bWordcount) { \
		    phNfcLib_TransmitInput.phNfcLib_ISO18000.bMemBank = bMembank; \
		    phNfcLib_TransmitInput.phNfcLib_ISO18000.pWordPtr = pWordptr; \
		    phNfcLib_TransmitInput.phNfcLib_ISO18000.bWordPtrLength = bWordPtrlength; \
		    phNfcLib_TransmitInput.phNfcLib_ISO18000.bWordCount = bWordcount; \
		    phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_Read; \
            }

/* WRITE takes the option,  Memory Bank to write, address of the word to write, length of the word ptr and Data to write */
/* bOpt can be  #PH_NFCLIB_18000P3M3_AC_NO_COVER_CODING or #PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING, bMembank can be 0x00 - 0x03 */
/* pWordptr is user choice limited to tag memory size, bWordPtrLength can be 0x00 - 0x03 and depends on the TAG memory size */
/* pData is a 2 byte word to write */
#define I18000_WRITE(bOpt, bMembank, pWordptr, bWordPtrlength, pData) { \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bOption = bOpt; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bMemBank = bMembank; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.pWordPtr = pWordptr; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bWordPtrLength = bWordPtrlength; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.pBuffer = pData; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_Write; \
    }

/* KILL takes the option , the password for the kill and recommissioning option */
/* bOpt can be  #PH_NFCLIB_18000P3M3_AC_NO_COVER_CODING or #PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING, pPassWord is a 4 byte password */
/* bReCom can be any optional or mandatory support Recom option, value 0x04 is a mandatory support */
#define I18000_KILL(bOpt, pPassWord, bReCom) { \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bOption = bOpt; \
     phNfcLib_TransmitInput.phNfcLib_ISO18000.pPassword = pPassWord; \
     phNfcLib_TransmitInput.phNfcLib_ISO18000.bRecom = bReCom; \
     phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_Kill; \
    }

/* LOCK takes the Mask on which and the action with which to perform the Lock */
/* pLockMask and pLockAction both are a 10-bit field please refer ISO 18000-3M3 for detailed value */
#define I18000_LOCK(pLockMask, pLockAction) { \
        phNfcLib_TransmitInput.phNfcLib_ISO18000.pMask = pLockMask; \
        phNfcLib_TransmitInput.phNfcLib_ISO18000.pAction = pLockAction; \
        phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_Lock; \
        }

/* ACCESS takes the option and password for the Access */
/* bOpt can be  #PH_NFCLIB_18000P3M3_AC_NO_COVER_CODING or #PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING, pPassWord is a 4 byte password */
#define I18000_ACCESS(bOpt, pPassWord) { \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bOption = bOpt; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.pPassword = pPassWord; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_Access; \
}

/* BLOCKWRITE takes the Memory Bank to write ,address of the word,length of the word ptr, No of words and Data to write */
/* bMembank can be 0x00 - 0x03, pWordptr is user choice limited to tag memory size, bWordPtrLen can be 0x00 - 0x03 and depends on the TAG memory size */
/* bWordcount is the no of words to write and is card support specific, pData takes the data to write */
#define I18000_BLOCKWRITE(bMembank, pWordptr, bWordPtrLen, bWordcount, pData)  { \
     phNfcLib_TransmitInput.phNfcLib_ISO18000.bMemBank = bMembank; \
     phNfcLib_TransmitInput.phNfcLib_ISO18000.pWordPtr = pWordptr; \
     phNfcLib_TransmitInput.phNfcLib_ISO18000.bWordPtrLength = bWordPtrLen; \
     phNfcLib_TransmitInput.phNfcLib_ISO18000.bWordCount = bWordcount; \
     phNfcLib_TransmitInput.phNfcLib_ISO18000.pBuffer = pData; \
     phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_BlockWrite; \
    }

/* BLOCKERASE takes the Memory Bank to write ,address of the word,length of the word ptr and  No of words to erase */
/* bMembank can be 0x00 - 0x03, pWordptr is user choice limited to tag memory size */
/* bWordPtrLen can be 0x00 - 0x03 and depends on the TAG memory size and No of words to erase is card support specific*/
#define I18000_BLOCKERASE(bMembank, pWordptr, bWordPtrLen, bWordcount)  { \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bMemBank = bMembank; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.pWordPtr = pWordptr; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bWordPtrLength = bWordPtrLen; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bWordCount = bWordcount; \
    phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_BlockErase; \
    }

/* BLOCKPERMALOCK takes the RFU, READ/LOCK choice, Memory Bank to PermaLock, starting address of the block and length of the block ptr, range of the block mask, and MemMask */
/* bRFU is to be kept 0, bReadlock can be 0/1, bMembank can be 0x00 - 0x03,  pBlockptr is user choice limited to tag memory size */
/* bBlockPtrLen can be 0x00 - 0x03, bBlockrange depends on Tag Memory and pMemMask along with ReadLock depend upon 0/1 */
#define I18000_BLOCKPERMALOCK(bRfu, bReadlock, bMembank, pBlockptr, bBlockPtrLen, bBlockrange, pMemMask) { \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.bRFU = bRfu; \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.bReadLock = bReadlock; \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.bMemBank = bMembank; \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.pBlockPtr = pBlockptr; \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.bBlockPtrLength = bBlockPtrLen; \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.bBlockRange = bBlockrange; \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.pMask = pMemMask; \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_BlockPermaLock; \
                }

/* SETHANDLE takes the pointer to the handle */
/* pHandlePtr is a 2 byte Handle return by the REQRN command */
#define I18000_SETHANDLE(pHandlePtr) { \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.pHandle = pHandlePtr; \
		        phNfcLib_TransmitInput.phNfcLib_ISO18000.bCommand = ISO18000_SetHandle; \
		                }

#endif /* EX11_SIMPLIFIEDAPI_ISO_18000_H */
