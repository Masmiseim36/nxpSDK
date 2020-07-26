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
* Simplified API ISO15693.
* $Author: Ashish Pal (nxp79566) $
* $Revision: 4423 $ (v06.01.00)
* $Date: 2016-16-09 11:50:54 +0530 (Fri, 22 Apr 2016) $
*
* History:
*
*
*/

#ifndef EX11_SIMPLIFIEDAPI_15693_H
#define EX11_SIMPLIFIEDAPI_15693_H


/* Read Single Block takes the option and block num to read */
/* bOpt can be #PH_ON or #PH_OFF, bBlockNo can be 0x00 - 0xFF */
#define READ_SINGLEBLOCK(bOpt, bBlockNo) {       \
                                        phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                        phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = bBlockNo; \
                                        phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_ReadSingleBlock; \
                                        }

/* Extended Read Single Block takes the option and block num to read the extended memory card*/
/* bOpt can be #PH_ON or #PH_OFF, wBlockNo can be  0x00 - 0xFFFF */
#define EXTENDEDREAD_SINGLEBLOCK(bOpt, wBlockNo) {   \
                                        phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                        phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = wBlockNo; \
                                        phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_Extended_ReadSingleBlock; \
                                        }

/* Read Multiple Block takes the option, block num and num of Blocks to Read*/
/* Option can be #PH_ON or #PH_OFF, block bBlockNo can be 0x00 - 0xFF, wNumBlock can be 0x01 - 0x100 */
#define READ_MULTIPLEBLOCK(bOpt, bBlockNo, wNumBlock) {    \
                                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = bBlockNo; \
                                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.wNumBlocks = wNumBlock; \
                                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_ReadMultipleBlocks; \
                                                    }

/* Read Multiple Block takes the option, block num and num of Blocks to Read from the extended memory card */
/* Option can be #PH_ON or #PH_OFF, block bBlockNo can be 0x00 - 0xFFFF, wNumBlock can be 0x01 - 0xFFFF */
#define EXTENDEDREAD_MULTIPLEBLOCK(bOpt, wBlockNo, wNumBlock) {    \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = wBlockNo; \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.wNumBlocks = wNumBlock; \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_Extended_ReadMultipleBlocks; \
                                                }

/* Get System Information takes no parameter and gets the system information */
#define GET_SYSTEMINFORMATION() {    \
                              phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_GetSystemInformation; \
                              }

/* Extended Get System Information takes no parameter and gets the system information for extended memory card */
#define EXTENDEDGET_SYSTEMINFORMATION() {    \
                                      phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_ExtendedGetSystemInformation; \
                                      }

/* Get Multiple Block Security status takes the Block no and num of blocks whose security status to be read */
/* bBlockNo can be 0x00 - 0xFF and wNumBlock can be 0x01 - 0x100 */
#define GET_MULTIPLEBLOCKSECURITYSTATUS(bBlockNo, wNumBlock) {    \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = bBlockNo; \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.wNumBlocks = wNumBlock; \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_GetMultipleBlockSecurityStatus; \
                                            }

/* Extended Get Multiple Block Security status takes the Block no and num of blocks for extended memory whose security status to be read */
/* wBlockNo can be 0x00 - 0xFFFF and wNumBlock can be 0x01 - 0xFFFF */
#define EXTENDEDGET_MULTIPLEBLOCKSECURITYSTATUS(wBlockNo, wNumBlock)  {    \
                                        phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = wBlockNo; \
                                        phNfcLib_TransmitInput.phNfcLib_ISO15693.wNumBlocks = wNumBlock; \
                                        phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_Extended_GetMultipleBlockSecurityStatus; \
                                        }


/* Write Single Block takes the option , block number and data to write */
/* bOpt can be #PH_ON or #PH_OFF, bBlockNo can be 0x00 - 0xFF, pData takes the pointer to the start of the data */
#define WRITE_SINGLEBLOCK(bOpt, bBlockNo, pData) {    \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = bBlockNo; \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_WriteSingleBlock; \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.pBuffer = pData;   \
                                                }

/* Extended Write Single Block takes the option , block number and data to write to a extended memory card */
/* bOpt can be #PH_ON or #PH_OFF, wBlockNo can be 0x00 - 0xFFFF, pData takes the pointer to the start of the data */
#define EXTENDEDWRITE_SINGLEBLOCK(bOpt, wBlockNo, pData) {    \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = wBlockNo; \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_Extended_WriteSingleBlock; \
                                                phNfcLib_TransmitInput.phNfcLib_ISO15693.pBuffer = pData;   \
                                                }


/* Write Multiple Block takes the option , block number, number of blocks and data to write */
/* bOpt can be #PH_ON or #PH_OFF, bBlockNo can be 0x00 - 0xFF, wNumBlock can be 0x01 - 0x100, pData takes the pointer to the start of the data */
#define WRITE_MULTIPLEBLOCK(bOpt, bBlockNo, wNumBlock, pData) {    \
                                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = bBlockNo; \
                                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.wNumBlocks = wNumBlock; \
                                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_WriteMultipleBlocks; \
                                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.pBuffer = pData;   \
                                                    }

/* Extended Write Multiple Block takes the option , block number, number of blocks and data to write to a extended memory card */
/* bOpt can be #PH_ON or #PH_OFF, wBlockNo can be 0x00 - 0xFFFF, wNumBlock can be 0x01 - 0xFFFF, pData takes the pointer to the start of the data */
#define EXTENDEDWRITE_MULTIPLEBLOCK(bOpt, wBlockNo, wNumBlock, pData) {    \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = wBlockNo; \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.wNumBlocks = wNumBlock; \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_Extended_WriteMultipleBlocks; \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.pBuffer = pData;   \
                                            }

/* Lock Block takes the option and block number to Lock */
/* bOpt can be #PH_ON or #PH_OFF, bBlockNo can be 0x00 - 0xFF */
#define LOCK_BLOCK(bOpt, bBlockNo) {    \
                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = bBlockNo; \
                                    phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_LockBlock; \
                                    }

/* Extended Lock Block takes the option and block number of Extended Memory Card to Lock */
/* bOpt can be #PH_ON or #PH_OFF, wBlockNo can be 0x00 - 0xFFFF */
#define EXTENDEDLOCK_BLOCK(bOpt, wBlockNo) {    \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.wBlockNumber = wBlockNo; \
                                            phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_Extended_LockBlock; \
                                            }

/* Write AFI takes the option and AFI to write */
/* bOpt can be #PH_ON or #PH_OFF, bAFI can be any valid AFI value */
#define WRITE_AFI(bOpt, bAFI) {    \
                            phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                            phNfcLib_TransmitInput.phNfcLib_ISO15693.bAfi = bAFI; \
                            phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_WriteAFI; \
                            }

/* Lock AFI takes the option with which to lock the AFI */
/* bOpt can be #PH_ON or #PH_OFF */
#define LOCK_AFI(bOpt) {    \
                        phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                        phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_LockAFI; \
                        }

/* Write DSFID takes the option and DSFID to write */
/* bOpt can be #PH_ON or #PH_OFF and bDSFID can be any valid DSFID value */
#define WRITE_DSFID(bOpt, bDSFID) {    \
                                phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                                phNfcLib_TransmitInput.phNfcLib_ISO15693.bDsfid = bDSFID; \
                                phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_WriteDSFID; \
                                }

/* Lock DSFID takes the option with which to lock the DSFID */
/* bOpt can be #PH_ON or #PH_OFF */
#define LOCK_DSFID(bOpt) {    \
                        phNfcLib_TransmitInput.phNfcLib_ISO15693.bOption = bOpt; \
                        phNfcLib_TransmitInput.phNfcLib_ISO15693.bCommand = ISO15693_LockDSFID; \
                        }


#endif /* EX11_SIMPLIFIEDAPI_15693_H */
