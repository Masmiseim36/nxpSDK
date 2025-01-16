/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_CO5300_H_
#define _FSL_CO5300_H_

#include "fsl_display.h"
#include "fsl_mipi_dsi_cmd.h"

/*
 * Change Log:
 *
 * 1.0.0:
 *   - Initial version.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief co5300 resource.
 */
typedef struct _co5300_resource
{
    mipi_dsi_device_t *dsiDevice;      /*!< MIPI DSI device. */
    void (*pullResetPin)(bool pullUp); /*!< Function to pull reset pin high or low. */
    void (*pullPowerPin)(bool pullUp); /*!< Function to pull power pin high or low. */
} co5300_resource_t;

extern const display_operations_t co5300_ops;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

status_t CO5300_Init(display_handle_t *handle, const display_config_t *config);

status_t CO5300_Deinit(display_handle_t *handle);

status_t CO5300_Start(display_handle_t *handle);

status_t CO5300_Stop(display_handle_t *handle);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_CO5300_H_ */
