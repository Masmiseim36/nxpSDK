/*
*
* Copyright 2016,2020 NXP
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef _CM_CMD_
#define _CM_CMD_

#include "sm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief CM_InvokeGarbageCollection.
 *  The function will trigger the garbage collection in SE05X.
 *  Invoking this api will close the existing session to SE05X.
 *  To use SE05X, open session again.
 *  Warning: Excessive calls to this API will impact the device durability
 *
 * @param conn_ctx Connection context.
 *
 * @returns Status of the operation
 * @retval #SW_OK The operation has completed successfully.
 * @retval #SM_NOT_OK The operation has failed.
 */
U16 CM_InvokeGarbageCollection (void *conn_ctx);

#ifdef __cplusplus
}
#endif
#endif
