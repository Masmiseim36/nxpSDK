/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_MEMORY_H
#define OSA_MEMORY_H

/*!
 * @file    osa_memory.h
 * @brief   Contains memory allocation function prototypes for Maestro
 *          libosa.
 */

/*
 * CONSTANTS, DEFINES AND MACROS
 */

#include "lib_common.h"

#define MAX_IO_COMPONENTS 0
#define MEM_TYPE_COMPONENT_SHIFT 12

#ifdef MEMUSAGE_DEBUG
#define osa_set_memtype(p, t)   _osa_set_memtype(p, t, #t)
#endif

#define OSA_MEMTYPE_BASE            ((MAX_IO_COMPONENTS + 1) << \
                                     (MEM_TYPE_COMPONENT_SHIFT))
#define OSA_MEMTYPE_SEGMENT_SHIFT   10

/*!
 * The base value of allocating memory type definition for framework
 *
 */
#define OSA_FWMEMTYPE_BASE          (OSA_MEMTYPE_BASE)

/*!
 * The base value of allocating memory type definition for metadata mgr
 */
#define OSA_METADATAMGR_MEMTYPE_BASE (OSA_MEMTYPE_BASE + \
                                      (1 << OSA_MEMTYPE_SEGMENT_SHIFT))

/*!
 * The base value of allocating memory type definition for Device Manager
 */
#define OSA_DEVICEMGR_MEMTYPE_BASE  (OSA_MEMTYPE_BASE + \
                                     (2 << OSA_MEMTYPE_SEGMENT_SHIFT))
/*!
 * The base value of allocating memory type definition for af
 */
#define OSA_INPUTMGR_MEMTYPE_BASE   (OSA_MEMTYPE_BASE + \
                                     (3 << OSA_MEMTYPE_SEGMENT_SHIFT))

/*!
 * The base value of allocating memory type definition for application
 */
#define OSA_PSMMEMTYPE_BASE         (OSA_MEMTYPE_BASE + \
                                     (4 << OSA_MEMTYPE_SEGMENT_SHIFT))

/*!
 * The base value of allocating memory type definition for OSA
 */
#define OSA_OSAMEMTYPE_BASE         (OSA_MEMTYPE_BASE + \
                                     (5 << OSA_MEMTYPE_SEGMENT_SHIFT))

/*!
 * The base value of allocating memory type definition for af
 */
#define OSA_AF_MEMTYPE_BASE    (OSA_MEMTYPE_BASE + \
                                     (6 << OSA_MEMTYPE_SEGMENT_SHIFT))

/*!
 * The base value of allocating memory type definition for test-apps
 */
#define OSA_APPS_MEMTYPE_BASE       (OSA_MEMTYPE_BASE + \
                                     (7 << OSA_MEMTYPE_SEGMENT_SHIFT))

/*!
 * The base value of allocating memory type definition for parsers
 */
#define OSA_PARSERS_MEMTYPE_BASE       (OSA_MEMTYPE_BASE + \
                                     (8 << OSA_MEMTYPE_SEGMENT_SHIFT))

/*!
 * @ingroup libosa
 * @brief   Allocate system memory
 * @param   size The number of bytes to allocate
 *
 * @returns  A pointer to the allocated memory block or NULL on failure
 */
void *osa_malloc(size_t size);

/*!
 * @ingroup libosa
 * @brief   Allocate system memory and zero it
 * @param   nmemb The number of elements to allocate
 * @param   size The size in bytes of each element
 *
 * @returns  A pointer to the allocated memory block or NULL on failure
 */
void *osa_calloc(size_t nmemb, size_t size);

/*!
 * @ingroup libosa
 * @brief   Change the size of a memory block
 * @param   ptr The existing memory block
 * @param   size The size of the new memory block
 *
 * @returns A pointer to the allocated memory block or NULL on failure
 */
void *osa_realloc(void *ptr, size_t size);

/*!
 * @ingroup libosa
 * @brief   Free system memory
 * @param   ptr The existing memory block
 */
void osa_free(void *ptr);

/*!
 * @ingroup libosa
 * @brief   Copy memory area
 * @details Copy from one memory area to another where the areas do not
 *          overlap
 * @param   dest The destination pointer
 * @param   src The source pointer
 * @param   n Number of bytes to copy

 * @returns  A pointer to dest
 */
void *osa_memcpy(void *dest, const void *src, size_t n);

/*!
 * @ingroup libosa
 * @brief   Fill memory with a constant byte
 * @param   src The memory area to fill
 * @param   c The constant byte to fill with
 * @param   n Number of bytes to fill
 *
 * @returns  A pointer to dest
 */
void *osa_memset(void *src, char c, size_t n);

/*!
 * @ingroup libosa
 * @brief   Copy memory area
 * @details Copy from one memory area to another.  Copying takes place as
 *          though the bytes in src are first copied into a temporary array
 *          that does not overlap src or dest, and the bytes are then copied
 *          from the temporary array to dest.
 * @param   dest The destination pointer
 * @param   src The source pointer
 * @param   n Number of bytes to copy
 *
 * @returns  A pointer to dest
 */
void *osa_memmove(void *dest, const void *src, size_t n);

/*!
 * @ingroup libosa
 * @brief   set the owner of the memory block
 * @details Use this API to set the allocating memory block's
 *              owner. This API should be called after succeeding
 *              to allocate memory.
 * @param   ptr The existing memory block
 * @param   type The module type of this memory block's owner
 *
 * @retval TRUE : success
 * @retval FALSE: failure
 */
#ifdef MEMUSAGE_DEBUG
void _osa_set_memtype(void *ptr, uint32_t type, char* name);
#else
void osa_set_memtype(void *ptr, uint32_t type);
#endif

#endif

/*! @} */

