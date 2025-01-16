/*
 * Copyright 2024 NXP
 * Copyright (C) 2020, Intel Corporation
 * Copyright (C) 2023, Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * \file
 *
 * Part of this code was imported from Zephyr project.
 *
 */

/**
 * @file
 * @brief Public API for FLASH drivers
 */

#ifndef _FWK_SETTINGS_PORT_H_
#define _FWK_SETTINGS_PORT_H_

#include <stddef.h>
#include <stdint.h>

#include <zephyr/sys/list_gen.h>
#include <zephyr/sys/slist.h>
#include "fsl_os_abstraction.h"
#include "fwk_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined gAppNvsExternalFlash_c && (gAppNvsExternalFlash_c > 0)
#define SETTINGS_PARTITION NVS_EXTFLASH_AREA_ID
#ifndef CONFIG_SETTINGS_NVS_SECTOR_SIZE_MULT
#define CONFIG_SETTINGS_NVS_SECTOR_SIZE_MULT (gPlatNvsSectorSize_c/PLATFORM_EXTFLASH_SECTOR_SIZE)
#endif /* CONFIG_SETTINGS_NVS_SECTOR_SIZE_MULT */
#elif defined gAppNvsInternalFlash_c && (gAppNvsInternalFlash_c > 0)
#define SETTINGS_PARTITION NVS_INTFLASH_AREA_ID
#ifndef CONFIG_SETTINGS_NVS_SECTOR_SIZE_MULT
#define CONFIG_SETTINGS_NVS_SECTOR_SIZE_MULT (gPlatNvsSectorSize_c/PLATFORM_INTFLASH_SECTOR_SIZE)
#endif /* CONFIG_SETTINGS_NVS_SECTOR_SIZE_MULT */
#else
#error "Must define a non volatile storage NVS partition for settings"
#endif

#ifndef CONFIG_SETTINGS_NVS_SECTOR_COUNT
#define CONFIG_SETTINGS_NVS_SECTOR_COUNT ((uint32_t)NV_STORAGE_SIZE / gPlatNvsSectorSize_c)
#endif

/* Reconfigure the Settings component name end character to zero as the '='
 * (equal) character is a legal character in Matter keys names. */
#define SETTINGS_NAME_END '\0'

/* Iterable sections imported from Zephyr iterable_sections.h*/
/**
 * @brief Defines a new element for an iterable section for a generic type.
 *
 * @details
 * Convenience helper combining __in_section() and Z_DECL_ALIGN().
 * The section name will be '.[SECNAME].static.[SECTION_POSTFIX]'
 *
 * In the linker script, create output sections for these using
 * ITERABLE_SECTION_ROM() or ITERABLE_SECTION_RAM().
 *
 * @note In order to store the element in ROM, a const specifier has to
 * be added to the declaration: const TYPE_SECTION_ITERABLE(...);
 *
 * @param[in]  type data type of variable
 * @param[in]  varname name of variable to place in section
 * @param[in]  secname type name of iterable section.
 * @param[in]  section_postfix postfix to use in section name
 */
#define TYPE_SECTION_ITERABLE(type, varname, secname, section_postfix) \
    DECL_ALIGN(type) varname __in_section(_##secname, static, _CONCAT(section_postfix, _)) __used __noasan

/**
 * @brief iterable section start symbol for a generic type
 *
 * will return '_[OUT_TYPE]_list_start'.
 *
 * @param[in]  secname type name of iterable section.  For 'struct foobar' this
 * would be TYPE_SECTION_START(foobar)
 *
 */
#define TYPE_SECTION_START(secname) _CONCAT(_##secname, _list_start)

/**
 * @brief iterable section end symbol for a generic type
 *
 * will return '_<SECNAME>_list_end'.
 *
 * @param[in]  secname type name of iterable section.  For 'struct foobar' this
 * would be TYPE_SECTION_START(foobar)
 */
#define TYPE_SECTION_END(secname) _CONCAT(_##secname, _list_end)

/**
 * @brief iterable section extern for start symbol for a generic type
 *
 * Helper macro to give extern for start of iterable section.  The macro
 * typically will be called TYPE_SECTION_START_EXTERN(struct foobar, foobar).
 * This allows the macro to hand different types as well as cases where the
 * type and section name may differ.
 *
 * @param[in]  type data type of section
 * @param[in]  secname name of output section
 */
#define TYPE_SECTION_START_EXTERN(type, secname) extern type TYPE_SECTION_START(secname)[]

/**
 * @brief iterable section extern for end symbol for a generic type
 *
 * Helper macro to give extern for end of iterable section.  The macro
 * typically will be called TYPE_SECTION_END_EXTERN(struct foobar, foobar).
 * This allows the macro to hand different types as well as cases where the
 * type and section name may differ.
 *
 * @param[in]  type data type of section
 * @param[in]  secname name of output section
 */
#define TYPE_SECTION_END_EXTERN(type, secname) extern type TYPE_SECTION_END(secname)[]

/**
 * @brief Iterate over a specified iterable section for a generic type
 *
 * @details
 * Iterator for structure instances gathered by TYPE_SECTION_ITERABLE().
 * The linker must provide a _<SECNAME>_list_start symbol and a
 * _<SECNAME>_list_end symbol to mark the start and the end of the
 * list of struct objects to iterate over. This is normally done using
 * ITERABLE_SECTION_ROM() or ITERABLE_SECTION_RAM() in the linker script.
 */
#define TYPE_SECTION_FOREACH(type, secname, iterator) \
    TYPE_SECTION_START_EXTERN(type, secname);         \
    TYPE_SECTION_END_EXTERN(type, secname);           \
    for (type *iterator = TYPE_SECTION_START(secname); iterator < TYPE_SECTION_END(secname); iterator++)

/**
 * @brief Iterate over a specified iterable section (alternate).
 *
 * @details
 * Iterator for structure instances gathered by STRUCT_SECTION_ITERABLE().
 * The linker must provide a _<SECNAME>_list_start symbol and a
 * _<SECNAME>_list_end symbol to mark the start and the end of the
 * list of struct objects to iterate over. This is normally done using
 * ITERABLE_SECTION_ROM() or ITERABLE_SECTION_RAM() in the linker script.
 */
#define STRUCT_SECTION_FOREACH(struct_type, iterator) TYPE_SECTION_FOREACH(struct struct_type, struct_type, iterator)

#ifndef WITH_LOGS

#define LOG_MODULE_REGISTER(x, y)
#define LOG_MODULE_DECLARE(...)

#undef LOG_WRN
#undef LOG_INF
#undef LOG_DBG

#undef LOG_HEXDUMP_ERR
#undef LOG_HEXDUMP_WRN
#undef LOG_HEXDUMP_INF
#undef LOG_HEXDUMP_DBG

#define LOG_ERR(...) (void)0
#define LOG_WRN(...) (void)0
#define LOG_DBG(...) (void)0
#define LOG_INF(...) (void)0

#define LOG_HEXDUMP_ERR(...) (void)0
#define LOG_HEXDUMP_WRN(...) (void)0
#define LOG_HEXDUMP_DBG(...) (void)0
#define LOG_HEXDUMP_INF(...) (void)0
#else
/* TO DO */
#endif
#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* _FWK_NVS_PORT_H_ */
