/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2020 NXP
 *
 * License: NXP LA_OPT_NXP_Software_License
 *
 * NXP Confidential. This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.  This code may only be used in a microprocessor,
 * microcontroller, sensor or digital signal processor ("NXP Product")
 * supplied directly or indirectly from NXP.  See the full NXP Software
 * License Agreement in license/LA_OPT_NXP_Software_License.pdf
 *
 * FreeMASTER Communication Driver - TSA
 */

#include "freemaster.h"
#include "freemaster_private.h"
#include "freemaster_protocol.h"
#include "freemaster_utils.h"

#if FMSTR_USE_TSA > 0 && FMSTR_DISABLE == 0

/* global variables */
const char FMSTR_TSA_POINTER[] = {
    (char)(0xe0 |
           (sizeof(void *) == 2 ?
                0x01 :
                sizeof(void *) == 4 ?
                0x02 :
                sizeof(void *) == 8 ? 0x03 : 0x00)), /* size of pointer is not 2,4 nor 8 (probably S12 platform) */
    (char)0};                                        /* string-terminating zero */

#if FMSTR_USE_TSA_DYNAMIC > 0
static FMSTR_SIZE fmstr_tsaBuffSize; /* Dynamic TSA buffer size */
static FMSTR_ADDR fmstr_tsaBuffAddr; /* Dynamic TSA buffer address */
static FMSTR_SIZE fmstr_tsaTableIndex;
#endif

/******************************************************************************
 *
 * @brief    TSA Initialization
 *
 ******************************************************************************/
static FMSTR_BOOL _FMSTR_IsMemoryMapped(const char *type, unsigned long info);

FMSTR_BOOL FMSTR_InitTsa(void)
{
#if FMSTR_USE_TSA_DYNAMIC > 0
    fmstr_tsaTableIndex = 0;
    fmstr_tsaBuffSize   = 0;
    fmstr_tsaBuffAddr   = (FMSTR_ADDR)NULL;
#endif

    return FMSTR_TRUE;
}

/******************************************************************************
 *
 * @brief    Assigning memory to dynamic TSA table
 *
 ******************************************************************************/

FMSTR_BOOL FMSTR_SetUpTsaBuff(FMSTR_ADDR buffAddr, FMSTR_SIZE buffSize)
{
#if FMSTR_USE_TSA_DYNAMIC > 0
    /* only allow to clear or set when cleared */
    if (FMSTR_ADDR_VALID(buffAddr) == FMSTR_FALSE || FMSTR_ADDR_VALID(fmstr_tsaBuffAddr) == FMSTR_FALSE)
    {
        /* TSA table must be aligned on pointer size */
        FMSTR_SIZE alignment = FMSTR_GetAlignmentCorrection(buffAddr, sizeof(FMSTR_ADDR));
        fmstr_tsaBuffAddr    = buffAddr + alignment;
        fmstr_tsaBuffSize    = buffSize - alignment;
        return FMSTR_TRUE;
    }
    else
    {
        return FMSTR_FALSE;
    }
#else
    return FMSTR_FALSE;
#endif
}

/******************************************************************************
 *
 * @brief    Retrieving entry from dynamic TSA table
 *
 ******************************************************************************/

#if FMSTR_USE_TSA_DYNAMIC > 0
FMSTR_TSA_FUNC_PROTO(dynamic_tsa)
{
    if (tableSize != NULL)
    {
        *tableSize = (FMSTR_SIZE)(fmstr_tsaTableIndex * sizeof(FMSTR_TSA_ENTRY));
    }
    return fmstr_tsaBuffAddr;
}
#endif

/******************************************************************************
 *
 * @brief    Add entry to a dynamic TSA table
 *
 ******************************************************************************/

FMSTR_BOOL FMSTR_TsaAddVar(FMSTR_TSATBL_STRPTR tsaName,
                           FMSTR_TSATBL_STRPTR tsaType,
                           FMSTR_TSATBL_VOIDPTR varAddr,
                           FMSTR_SIZE32 varSize,
                           FMSTR_SIZE flags)
{
#if FMSTR_USE_TSA_DYNAMIC > 0
    /* the new TSA table entry must fit into the memory buffer */
    if (((fmstr_tsaTableIndex + 1U) * sizeof(FMSTR_TSA_ENTRY)) <= fmstr_tsaBuffSize)
    {
        FMSTR_TSATBL_VOIDPTR info = FMSTR_TSA_INFO2(varSize, flags);
        FMSTR_LP_TSA_ENTRY pItem  = (FMSTR_LP_TSA_ENTRY)FMSTR_CAST_ADDR_TO_PTR(fmstr_tsaBuffAddr);
        FMSTR_BOOL found;
        FMSTR_SIZE i;

        /* Check if this record is already in table */
        for (i = 0; i < fmstr_tsaTableIndex; i++)
        {
            found = FMSTR_TRUE;

            if (FMSTR_StrCmp(pItem->name.p, tsaName) != 0)
            {
                found = FMSTR_FALSE; /* name is different */
            }
            else if (pItem->type.p != tsaType)
            {
                found = FMSTR_FALSE; /* type is different */
            }
            else if (pItem->addr.p != varAddr)
            {
                found = FMSTR_FALSE; /* address is different */
            }
            else if (pItem->info.p != info)
            {
                found = FMSTR_FALSE; /* size or attributes are different */
            }

            if (found != FMSTR_FALSE)
            {
                /* the same entry already exists, consider it added okay */
                return FMSTR_TRUE;
            }

            /* MISRA does not allow multiple increments in for(), let's do it here */
            pItem++;
        }

        /* add the entry to the last-used position */
        pItem->name.p = FMSTR_TSATBL_STRPTR_CAST(tsaName);
        pItem->type.p = FMSTR_TSATBL_STRPTR_CAST(tsaType);
        pItem->addr.p = FMSTR_TSATBL_VOIDPTR_CAST(varAddr);
        pItem->info.p = FMSTR_TSATBL_VOIDPTR_CAST(info);
        fmstr_tsaTableIndex++;
        return FMSTR_TRUE;
    }
    else
    {
        return FMSTR_FALSE;
    }
#else
    return FMSTR_FALSE;
#endif
}

/******************************************************************************
 *
 * @brief    Handling GETTSAINFO and GETTSAINFO_EX command
 *
 * @param    msgBuffIO - original command (in) and response buffer (out)
 * @param    retStatus - response status
 *
 * @return   As all command handlers, the return value should be the buffer
 *           pointer where the response output finished (except checksum)
 *
 ******************************************************************************/

FMSTR_BPTR FMSTR_GetTsaInfo(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus)
{
    volatile FMSTR_BPTR response = msgBuffIO;
    FMSTR_ADDR tsaTbl;
    FMSTR_SIZE tblIndex;
    FMSTR_SIZE tblSize = 0U;
    FMSTR_U8 tblFlags;

    FMSTR_ASSERT(msgBuffIO != NULL);
    FMSTR_ASSERT(retStatus != NULL);

    /* Get ULEB index of table the PC is requesting   */
    msgBuffIO = FMSTR_SizeFromBuffer(&tblIndex, msgBuffIO);
    FMSTR_UNUSED(msgBuffIO);

    /* TSA flags */
    tblFlags = FMSTR_TSA_VERSION | FMSTR_TSA_FLAGS;

    /* sizeof TSA table entry items */
    /*lint -e{506,774} constant value boolean */
    if ((sizeof(FMSTR_ADDR)) == 2U)
    {
        tblFlags |= FMSTR_TSA_INFO_ADRSIZE_16;
    }
    else
    {
        if ((sizeof(FMSTR_ADDR)) <= 4U)
        {
            tblFlags |= FMSTR_TSA_INFO_ADRSIZE_32;
        }
        else
        {
            tblFlags |= FMSTR_TSA_INFO_ADRSIZE_64;
        }
    }

    /* flags */
    response = FMSTR_ValueToBuffer8(response, tblFlags);

    /* get the table (or NULL if no table on given index) */
    tsaTbl = FMSTR_TsaGetTable(tblIndex, &tblSize);

    /* table size in bytes */
    tblSize *= FMSTR_CFG_BUS_WIDTH;
    response = FMSTR_SizeToBuffer(response, tblSize);

    /* table address */
    response = FMSTR_AddressToBuffer(response, tsaTbl);

    /* success  */
    *retStatus = FMSTR_STS_OK | FMSTR_STSF_VARLEN;
    return response;
}

/******************************************************************************
 *
 * @brief    Handling GETSTRLEN and GETSTRLEN_EX commands
 *
 * @param    msgBuffIO - original command (in) and response buffer (out)
 * @param    retStatus - response status
 *
 * @return   As all command handlers, the return value should be the buffer
 *           pointer where the response output finished (except checksum)
 *
 ******************************************************************************/

FMSTR_BPTR FMSTR_GetStringLen(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus)
{
    FMSTR_BPTR response = msgBuffIO;
    FMSTR_ADDR strAddr;
    FMSTR_SIZE len = 0U;

    FMSTR_ASSERT(msgBuffIO != NULL);
    FMSTR_ASSERT(retStatus != NULL);

    msgBuffIO = FMSTR_AddressFromBuffer(&strAddr, msgBuffIO);
    FMSTR_UNUSED(msgBuffIO);

    len = FMSTR_StrLen((FMSTR_CHAR *)strAddr);

    /* return strign size in bytes (even on 16bit DSP) */
    len *= FMSTR_CFG_BUS_WIDTH;

    /* success  */
    *retStatus = FMSTR_STS_OK | FMSTR_STSF_VARLEN;
    return FMSTR_SizeToBuffer(response, len);
}

/******************************************************************************
 *
 * @brief    Helper (inline) function for TSA memory region check
 *
 * @param    addrUser - address of region to be checked
 * @param    sizeUser - size of region to be checked
 * @param    addrSafe - address of known "safe" region
 * @param    sizeSafe - size of safe region
 *
 * @return   This function returns non-zero if given user space is safe
 *           (i.e. it lies in given safe space)
 *
 ******************************************************************************/

/* declare function prototype */
static FMSTR_BOOL FMSTR_CheckMemSpace(FMSTR_ADDR addrUser,
                                      FMSTR_SIZE sizeUser,
                                      FMSTR_ADDR addrSafe,
                                      FMSTR_SIZE sizeSafe);

static FMSTR_BOOL FMSTR_CheckMemSpace(FMSTR_ADDR addrUser,
                                      FMSTR_SIZE sizeUser,
                                      FMSTR_ADDR addrSafe,
                                      FMSTR_SIZE sizeSafe)
{
    FMSTR_BOOL ret = FMSTR_FALSE;

#ifdef __HCS12X__
    /* convert from logical to global if needed */
    addrUser = FMSTR_FixHcs12xAddr(addrUser);
    addrSafe = FMSTR_FixHcs12xAddr(addrSafe);
#endif

    if (addrUser >= addrSafe)
    {
        ret = (FMSTR_BOOL)(((addrUser + sizeUser) <= (addrSafe + sizeSafe)) ? FMSTR_TRUE : FMSTR_FALSE);
    }

    return ret;
}

/******************************************************************************
 *
 * @brief    Check wether given memory region is "safe" (covered by TSA)
 *
 * @param    varAddr - address of the memory to be checked
 * @param    varSize  - size of the memory to be checked
 * @param    writeAccess - write access is required
 *
 * @return   This function returns non-zero if user space is safe
 *
 ******************************************************************************/

FMSTR_BOOL FMSTR_CheckTsaSpace(FMSTR_ADDR varAddr, FMSTR_SIZE varSize, FMSTR_BOOL writeAccess)
{
    FMSTR_LP_TSA_ENTRY pte;
    FMSTR_ADDR pteAddr;
    FMSTR_SIZE tableIndex;
    FMSTR_SIZE i, cnt;
    unsigned long info;
    const char *type;

#if FMSTR_CFG_BUS_WIDTH >= 2U
    /* TSA tables use sizeof() operator which returns size in "bus-widths" (e.g. 56F8xx) */
    varSize = (varSize + 1) / FMSTR_CFG_BUS_WIDTH;
#endif

    /* to be as fast as possible during normal operation,
       check variable entries in all tables first */
    tableIndex = 0U;
    while ((pteAddr = FMSTR_TsaGetTable(tableIndex, &cnt)) != NULL)
    {
        pte = (FMSTR_LP_TSA_ENTRY)FMSTR_CAST_ADDR_TO_PTR(pteAddr);

        /* number of items in a table */
        cnt /= (FMSTR_SIZE)sizeof(FMSTR_TSA_ENTRY);

        /* all table entries */
        for (i = 0U; i < cnt; i++)
        {
            if (sizeof(pte->addr.p) < sizeof(pte->addr.n))
            {
                info = (unsigned long)pte->info.n;
            }
            else
            {
                info = (unsigned long)pte->info.p;
            }

            type = pte->type.p;

            /* variable entry only (also check read-write flag) */
            if (_FMSTR_IsMemoryMapped(type, info) != FMSTR_FALSE &&
                (writeAccess == FMSTR_FALSE || ((info & FMSTR_TSA_INFO_VAR_MASK) == FMSTR_TSA_INFO_RW_VAR)))
            {
                /* need to take the larger of the two in union (will be optimized by compiler anyway) */
                if (sizeof(pte->addr.p) < sizeof(pte->addr.n))
                {
                    if (FMSTR_CheckMemSpace(varAddr, varSize, pte->addr.n, (FMSTR_SIZE)(info >> 2)) != FMSTR_FALSE)
                    {
                        return FMSTR_TRUE; /* access granted! */
                    }
                }
                else
                {
                    if (FMSTR_CheckMemSpace(varAddr, varSize, (FMSTR_ADDR)pte->addr.p, (FMSTR_SIZE)(info >> 2)) !=
                        FMSTR_FALSE)
                    {
                        return FMSTR_TRUE; /* access granted! */
                    }
                }
            }

            pte++;
        }

        tableIndex++;
    }

    /* no more writable memory chunks available */
    if (writeAccess != FMSTR_FALSE)
    {
        return FMSTR_FALSE;
    }

    /* allow reading of recorder buffer */
#if FMSTR_USE_RECORDER > 0
    if (FMSTR_IsInRecBuffer(varAddr, varSize) != FMSTR_FALSE)
    {
        return FMSTR_TRUE;
    }
#endif

    /* allow reading of any C-constant string referenced in TSA tables */
    tableIndex = 0U;
    while ((pteAddr = FMSTR_TsaGetTable(tableIndex, &cnt)) != NULL)
    {
        pte = (FMSTR_LP_TSA_ENTRY)FMSTR_CAST_ADDR_TO_PTR(pteAddr);

        /* allow reading of the TSA table itself */
        if (FMSTR_CheckMemSpace(varAddr, varSize, pteAddr, cnt) != FMSTR_FALSE)
        {
            return FMSTR_TRUE;
        }

        /* number of items in a table */
        cnt /= (FMSTR_SIZE)sizeof(FMSTR_TSA_ENTRY);

        /* all table entries */
        for (i = 0U; i < cnt; i++)
        {
            /* system strings are always accessible as C-pointers */
            if (pte->name.p != NULL)
            {
                if (FMSTR_CheckMemSpace(varAddr, varSize, (FMSTR_ADDR)(pte->name.p), FMSTR_StrLen(pte->name.p)) !=
                    FMSTR_FALSE)
                {
                    return FMSTR_TRUE;
                }
            }

            if (pte->type.p != NULL)
            {
                if (FMSTR_CheckMemSpace(varAddr, varSize, (FMSTR_ADDR)(pte->type.p), FMSTR_StrLen(pte->type.p)) !=
                    FMSTR_FALSE)
                {
                    return FMSTR_TRUE;
                }
            }

            pte++;
        }

        tableIndex++;
    }

    /* no valid TSA entry found => not-safe to access the memory */
    return FMSTR_FALSE;
}

/* Check type of the entry. */
static FMSTR_BOOL _FMSTR_IsMemoryMapped(const char *type, unsigned long info)
{
    FMSTR_ASSERT(type != NULL);

    /* If type is special non-memory type or memeber structure (0b00 in info) */
    if (type[0] == FMSTR_TSA_SPECIAL_NOMEM[0] || (info & FMSTR_TSA_INFO_VAR_MASK) == 0U)
    {
        return FMSTR_FALSE;
    }

    return FMSTR_TRUE;
}

/* Find TSA table row with user resource by resource ID */
FMSTR_ADDR FMSTR_FindUresInTsa(FMSTR_ADDR resourceId)
{
    FMSTR_LP_TSA_ENTRY pte;
    FMSTR_ADDR pteAddr;
    FMSTR_SIZE tableIndex;
    FMSTR_SIZE i, cnt;

    tableIndex = 0U;
    while ((pteAddr = FMSTR_TsaGetTable(tableIndex, &cnt)) != NULL)
    {
        pte = (FMSTR_LP_TSA_ENTRY)FMSTR_CAST_ADDR_TO_PTR(pteAddr);

        /* number of items in a table */
        cnt /= (FMSTR_SIZE)sizeof(FMSTR_TSA_ENTRY);

        /* all table entries */
        for (i = 0U; i < cnt; i++)
        {
            if (pte->addr.n == resourceId)
            {
                return FMSTR_CAST_PTR_TO_ADDR(pte);
            }
        }

        tableIndex++;
    }

    return NULL;
}

#else /* (FMSTR_USE_TSA) && (!(FMSTR_DISABLE)) */

/* use void TSA API functions */
FMSTR_BOOL FMSTR_SetUpTsaBuff(FMSTR_ADDR buffAddr, FMSTR_SIZE buffSize)
{
    FMSTR_UNUSED(buffAddr);
    FMSTR_UNUSED(buffSize);
    return FMSTR_FALSE;
}

FMSTR_BOOL FMSTR_TsaAddVar(FMSTR_TSATBL_STRPTR tsaName,
                           FMSTR_TSATBL_STRPTR tsaType,
                           FMSTR_TSATBL_VOIDPTR varAddr,
                           FMSTR_SIZE32 varSize,
                           FMSTR_SIZE flags)
{
    FMSTR_UNUSED(tsaName);
    FMSTR_UNUSED(tsaType);
    FMSTR_UNUSED(varAddr);
    FMSTR_UNUSED(varSize);
    FMSTR_UNUSED(flags);
    return FMSTR_FALSE;
}

/*lint -efile(766, freemaster_protocol.h) include file is not used in this case */

#endif /* (FMSTR_USE_TSA) && (!(FMSTR_DISABLE)) */
