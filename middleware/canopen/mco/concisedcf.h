/**************************************************************************
MODULE:    ConciseDCF
CONTAINS:  MicroCANopen Plus, concise DCF handling
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS MANAGER
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#ifndef _CONCISEDCF_H
#define _CONCISEDCF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mco.h"
#include "mcohw.h"
#include "comgr.h"

#if USE_CONCISEDCF

#define CDCF_INPROGRESS   0x01
#define CDCF_WAITRESPONSE 0x02
#define CDCF_COMPLETE     0x10
#define CDCF_ERROR        0x80

// Definitions for CDCF list
#define CDCF_8BITS  1
#define CDCF_16BITS 2
#define CDCF_24BITS 3
#define CDCF_32BITS 4
#define CDCF_ENTRY(index, subindex, length)                                                                          \
    (uint8_t)((index)&0xFF), (uint8_t)(((index) >> 8) & 0xFF), (uint8_t)((subindex)&0xFF), (uint8_t)((length)&0xFF), \
        (uint8_t)(((length) >> 8) & 0xFF), (uint8_t)(((length) >> 16) & 0xFF), (uint8_t)(((length) >> 24) & 0xFF)
#define CDCF_DATA8(data)  (uint8_t)((uint32_t)(data)&0xFF)
#define CDCF_DATA16(data) (uint8_t)((uint32_t)(data)&0xFF), (uint8_t)(((uint32_t)(data) >> 8) & 0xFF)
#define CDCF_DATA24(data)                                                        \
    (uint8_t)((uint32_t)(data)&0xFF), (uint8_t)(((uint32_t)(data) >> 8) & 0xFF), \
        (uint8_t)(((uint32_t)(data) >> 16) & 0xFF)
#define CDCF_DATA32(data)                                                        \
    (uint8_t)((uint32_t)(data)&0xFF), (uint8_t)(((uint32_t)(data) >> 8) & 0xFF), \
        (uint8_t)(((uint32_t)(data) >> 16) & 0xFF), (uint8_t)(((uint32_t)(data) >> 24) & 0xFF)
#define CDCF_NROFENTRIES(num) CDCF_DATA32(num)

/**************************************************************************
CONCISE DCF FORMAT as specified by CANopen

uint32_t Number of entries in File

For each entry:

uint16_t  Index
uint8_t   Subindex
uint32_t  Size of data in Bytes
DOMAIN      data

**************************************************************************/

/**************************************************************************
DOES:    Initiates all the SDO write accesses specified in a concise DCF
         file, first 4 bytes is number of entires. In application, call
         CDCF_ProcessCDCF until transfer completed.
RETURNS: Pointer to status variable. If bit CDCF_COMPLETE is set in this
         status variable, then the transfer completed. If bit CDCF_ERROR
         is set the transfer was aborted
**************************************************************************/
MEM_FAR uint8_t *CDCF_Write(uint8_t SDOchannel, uint8_t NodeID, MEM_FAR uint8_t *pCDCF, uint16_t Timeout);

/**************************************************************************
DOES:    Processes the next step of the DCF write sequence
RETURNS: nothing
**************************************************************************/
void CDCF_Process(void);

#endif // USE_CONCISEDCF

#ifdef __cplusplus
}
#endif

#endif // _CONCISEDCF_H
/**************************************************************************
END OF FILE
**************************************************************************/
