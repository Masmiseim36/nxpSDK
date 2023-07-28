/**********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 *
 * This file contains the structure used by every module present to identify itself
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef MODULE_INFO_H
#define MODULE_INFO_H

/*!
 * @addtogroup ModuleInfo
 * The ModuleInfo module
 *
 * The ModuleInfo is a small Connectivity Framework module that provides a mechanism
 * that allows stack components to register information about themselves.
 *
 * The information comprises:
 * - Component or module name (for example: Bootloader and Bluetooth LE Host) and associated version string
 * - Component or module ID
 * - Version number
 * - Build number
 *
 * The information can be retrieved using shell commands or FSCI commands.
 *
 * @{
 */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

/* Used on multicore platforms to retrieve information from the other core
   If this value is redefined it must have the same value on both cores
*/
#ifndef MAX_REGISTERED_MODULES_STRLEN
#define MAX_REGISTERED_MODULES_STRLEN 128
#endif
#if defined(__GNUC__)

extern uint32_t __start_VERSION_TAGS[];
extern uint32_t __stop_VERSION_TAGS[];

#define gVERSION_TAGS_startAddr_d ((moduleInfo_t *)__start_VERSION_TAGS)
#define gVERSION_TAGS_endAddr_d   ((moduleInfo_t *)__stop_VERSION_TAGS)

#elif (defined(__IAR_SYSTEMS_ICC__))
#define gVERSION_TAGS_startAddr_d ((moduleInfo_t *)__section_begin(".VERSION_TAGS"))
#define gVERSION_TAGS_endAddr_d   ((moduleInfo_t *)__section_end(".VERSION_TAGS"))
#elif (defined(__CC_ARM))

extern uint32_t Image$$VERSION_TAGS$$Base[];
extern uint32_t Image$$VERSION_TAGS$$Limit[];

#define gVERSION_TAGS_startAddr_d ((moduleInfo_t *)Image$$VERSION_TAGS$$Base)
#define gVERSION_TAGS_endAddr_d   ((moduleInfo_t *)Image$$VERSION_TAGS$$Limit)

#else
#define gVERSION_TAGS_startAddr_d ((moduleInfo_t *)0)
#define gVERSION_TAGS_endAddr_d   ((moduleInfo_t *)0)
#warning Module informaion will not be stored!
#endif

#define gVERSION_TAGS_entries_d \
    (((uint32_t)gVERSION_TAGS_endAddr_d - (uint32_t)gVERSION_TAGS_startAddr_d) / sizeof(moduleInfo_t))

#define gVERSION_TAGS_entrySizeNoPadding_d (sizeof(moduleInfo_t) - sizeof(((moduleInfo_t *)(void *)0)->padding))

#define gVERSION_TAGS_entrySizeNoPaddingNoModuleString_d \
    (gVERSION_TAGS_entrySizeNoPadding_d - sizeof(((moduleInfo_t *)(void *)0)->moduleString))

/* Concatenate macros */
#define QU(x)  #x
#define QUH(x) QU(x)

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/* VERSION_TAGS is the section where module information will be stored */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma section = ".VERSION_TAGS"
#elif defined(__CC_ARM)
#pragma arm section rodata = "VERSION_TAGS"
#endif

/*! \brief  module informations structure content.
 *
 * Data structure containing module informations
 */
typedef PACKED_STRUCT moduleInfo_tag
{
    const char **moduleString; /*!< module information string that is usually built using concatenation \
                                    macros between module name and module version */
    uint8_t moduleId;          /*!< hexadecimal value that can be used as a unique ID within the system */
    uint8_t versionNumber[3];  /*!< three decimal values that specify the major, minor, and patch version of the
                                  component/module */
    uint16_t buildNumber;      /*!< build number */
    uint8_t  padding[2];
}
moduleInfo_t;

/* MODULE INFO (VERSION_TAGS) REGISTER MACRO, DO NOT EDIT or REMOVE */
#define SET_MODULE_NAME(name) name##_##VERSION

/*! \brief  Registers a new component or module information.\n
 *          This is not actually a function but a macro that creates in FLASH memory a structure
 *          that keeps the component version information in a dedicated section called VERSION_TAGS.
 *
 * \param[in] moduleName Module name
 * \param[in] moduleNameString  Module name as a concatenated string of name and version
 * \param[in] moduleId          Module ID
 * \param[in] versionNoMajor    Major version number
 * \param[in] versionNoMinor    Minor version number
 * \param[in] versionNoPatch    Patch version number
 * \param[in] buildNo           Build number
 */
#if defined(__IAR_SYSTEMS_ICC__)
#define RegisterModuleInfo(moduleName, moduleNameString, moduleId, versionNoMajor, versionNoMinor, versionNoPatch, \
                           buildNo)                                                                                \
    extern const moduleInfo_t                                       SET_MODULE_NAME(moduleName);                   \
    _Pragma("location=\".VERSION_TAGS\"") __root const moduleInfo_t SET_MODULE_NAME(moduleName) = {                \
        &moduleNameString, moduleId, {versionNoMajor, versionNoMinor, versionNoPatch}, buildNo}
#elif defined(__CC_ARM)
#define RegisterModuleInfo(moduleName, moduleNameString, moduleId, versionNoMajor, versionNoMinor, versionNoPatch, \
                           buildNo)                                                                                \
    extern const moduleInfo_t SET_MODULE_NAME(moduleName);                                                         \
    const moduleInfo_t        SET_MODULE_NAME(moduleName) = {                                                      \
        &moduleNameString, moduleId, {versionNoMajor, versionNoMinor, versionNoPatch}, buildNo}
#elif defined(__GNUC__)
#define RegisterModuleInfo(moduleName, moduleNameString, moduleId, versionNoMajor, versionNoMinor, versionNoPatch, \
                           buildNo)                                                                                \
    extern const moduleInfo_t SET_MODULE_NAME(moduleName);                                                         \
    const moduleInfo_t        SET_MODULE_NAME(moduleName) __attribute__((section(".VERSION_TAGS"), used)) = {      \
        (const char **)&moduleNameString,                                                                   \
        moduleId,                                                                                           \
        {versionNoMajor, versionNoMinor, versionNoPatch},                                                   \
        buildNo,                                                                                            \
    }
#else
#define RegisterModuleInfo(moduleName, moduleNameString, moduleId, versionNoMajor, versionNoMinor, versionNoPatch, \
                           buildNo)
extern const moduleInfo_t SET_MODULE_NAME(moduleName);
const moduleInfo_t        SET_MODULE_NAME(moduleName) =
    {&moduleNameString, moduleId, {versionNoMajor, versionNoMinor, versionNoPatch}, buildNo} #warning Unknown
    / undefined toolchain !
#endif

/**********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  This function will retrieve the number of registered modules on the secondary
 *         core on a multicore platform.
 *
 * \return  uint8_t  Number of registered modules
 *
 ********************************************************************************** */
uint8_t ModVer_GetNoOfEntries_Multicore(void);

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
uint8_t ModVer_GetInfoFSCIFormat_Multicore(uint8_t *pString);

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
bool_t ModVer_GetInfoShellFormat_Multicore(uint8_t *pString);

/*!
 * @} end of ModuleInfo addtogroup
 */

#endif /* MODULE_INFO_H */
