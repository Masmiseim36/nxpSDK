/*! *********************************************************************************
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * \file
 *
 * Connectivity Framework Module Information
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#include "ModuleInfo.h"
#include "FunctionLib.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

/* FWK build version  */
/*! \cond DOXY_SKIP_TAG */
#define gFwkVerMajor_c 6
#define gFwkVerMinor_c 1
#define gFwkVerPatch_c 0
#define gFwkBuildNo_c  0
#define gFwkModuleId_c 0xA3
/*! \endcond */

#define gFwkVerString_c "CONNECTIVITY FRAMEWORK v" QUH(gFwkVerMajor_c) "." QUH(gFwkVerMinor_c) "." QUH(gFwkVerPatch_c)

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
/*!
 * \brief The version string of the Connectivity Framework
 */
static const char *mFwkVersionString = gFwkVerString_c;

/*!
 * \brief Registers the Connectivity Framework module info
 */
RegisterModuleInfo(FWK,                 /* DO NOT MODIFY */
                   (mFwkVersionString), /* DO NOT MODIFY */
                   gFwkModuleId_c,      /* DO NOT MODIFY */
                   gFwkVerMajor_c,      /* DO NOT MODIFY */
                   gFwkVerMinor_c,      /* DO NOT MODIFY */
                   gFwkVerPatch_c,      /* DO NOT MODIFY */
                   gFwkBuildNo_c);      /* DO NOT MODIFY */

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
/*! *********************************************************************************
 * \brief  This function will retrieve the number of registered modules on the secondary
 *         core on a multicore platform.
 *
 * \return  uint8_t  Number of registered modules
 *
 ********************************************************************************** */
#if defined(CPU_K32W042S1M2VPJ_cm0plus) && (CPU_K32W042S1M2VPJ_cm0plus == 1)
uint8_t ModVer_GetNoOfEntries_Multicore(void)
{
    return gVERSION_TAGS_entries_d;
}
/*! *********************************************************************************
 * \brief  This function will retrieve a string containing information about the
 *         registered modules on the secondary core on a multicore platform in a format
 *         required by FSCI.
 *
 * \param[out]  pString pointer to a string
 *
 * \return  uint8_t  Length of the out string
 *
 ********************************************************************************** */
uint8_t ModVer_GetInfoFSCIFormat_Multicore(uint8_t *pString)
{
    uint8_t       totalLen = 0;
    moduleInfo_t *pInfo    = gVERSION_TAGS_startAddr_d;

    while (pInfo < gVERSION_TAGS_endAddr_d)
    {
        if ((totalLen + gVERSION_TAGS_entrySizeNoPaddingNoModuleString_d) > MAX_REGISTERED_MODULES_STRLEN)
        {
            break;
        }

        FLib_MemCpy(pString + totalLen, &pInfo->moduleId, gVERSION_TAGS_entrySizeNoPaddingNoModuleString_d);
        totalLen += gVERSION_TAGS_entrySizeNoPaddingNoModuleString_d;
        pInfo++;
    }

    return totalLen;
}

/*! *********************************************************************************
 * \brief  This function will retrieve a string containing information about the
 *         registered modules on the secondary core on a multicore platform in a format
 *         required by shell.
 *
 * \param[out]  pString pointer to a string
 *
 * \return  bool_t  TRUE if modules found, FALSE otherwise
 *
 ********************************************************************************** */
bool_t ModVer_GetInfoShellFormat_Multicore(uint8_t *pString)
{
    moduleInfo_t *pInfo        = gVERSION_TAGS_startAddr_d;
    uint32_t      moduleStrLen = 0, totalLen = 0;

    if (!gVERSION_TAGS_entries_d)
    {
        return FALSE;
    }

    while (pInfo < gVERSION_TAGS_endAddr_d)
    {
        moduleStrLen = FLib_StrLen(*pInfo->moduleString);
        if ((totalLen + moduleStrLen + 2) > MAX_REGISTERED_MODULES_STRLEN)
        {
            break;
        }
        FLib_MemCpy(pString + totalLen, *pInfo->moduleString, moduleStrLen);
        totalLen += moduleStrLen;
        pString[totalLen++] = '\n';
        pString[totalLen++] = '\r';
        pInfo++;
    }

    pString[totalLen] = 0;
    return TRUE;
}

#endif /* defined(CPU_K32W042S1M2VPJ_cm0plus) && (CPU_K32W042S1M2VPJ_cm0plus == 1) */
