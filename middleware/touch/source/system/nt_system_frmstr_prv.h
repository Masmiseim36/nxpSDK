/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _NT_FRMSTR_PRV_H_
#define _NT_FRMSTR_PRV_H_

#include "nt_types.h"

/**
 * \defgroup freemaster_private FreeMASTER support
 * \ingroup ntapi_private
 *
 * \{
 */

/**
 * \defgroup freemaster_api_private API functions
 * \ingroup freemaster_private
 * General Private Function definition of the FreeMASTER support.
 *
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialized the NXP touch FreeMASTER support system.
 * \return The result of operation.
 */
int32_t _nt_freemaster_init(void);

/**
 * \brief This function adds a dynamic variable into the FreeMASTER TSA table.
 * \param name - pointer to the string with the name of the variable
 * \param type_name - pointer to the string with the name of the variable type
 * \param address - address of the variable
 * \param size - size of the variable
 * \return The result of the operation.
 */
int32_t _nt_freemaster_add_variable(const char *name, const char *type_name, const void *address, uint32_t size);

#ifdef __cplusplus
}
#endif

/** \} */ /* end of freemaster_api_private group */
/** \} */ /* end of freemaster_private group */

#endif /* NT_FRMSTR_PRV_H_ */
