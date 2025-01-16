/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_RM692c9_H_
#define _FSL_RM692c9_H_

#include "fsl_display.h"
#include "fsl_mipi_dsi_cmd.h"

/*
 * Change log:
 *
 *   1.0.0
 *     - Initial version
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RM692c9_NOP        (0x00)
#define RM692c9_SWRESET    (0x01)
#define RM692c9_RDDID      (0x04)
#define RM692c9_RDNUMED    (0x05)
#define RM692c9_RDDPM      (0x0A)
#define RM692c9_RDDMADCTR  (0x0B)
#define RM692c9_RDDCOLMOD  (0x0C)
#define RM692c9_RDDIM      (0x0D)
#define RM692c9_RDDSM      (0x0E)
#define RM692c9_RDDSDR     (0x0F)
#define RM692c9_SLPIN      (0x10)
#define RM692c9_SLPOUT     (0x11)
#define RM692c9_INVOFF     (0x20)
#define RM692c9_INVON      (0x21)
#define RM692c9_ALLPOFF    (0x22)
#define RM692c9_ALLPON     (0x23)
#define RM692c9_DISPOFF    (0x28)
#define RM692c9_DISPON     (0x29)
#define RM692c9_RAMWR      (0x2C)
#define RM692c9_TEOFF      (0x34)
#define RM692c9_TEON       (0x35)
#define RM692c9_MADCTR     (0x36)
#define RM692c9_IDMOFF     (0x38)
#define RM692c9_IDMON      (0x39)
#define RM692c9_COLMOD     (0x3A)
#define RM692c9_RAMWRC     (0x3C)
#define RM692c9_STES       (0x44)
#define RM692c9_GSL        (0x45)
#define RM692c9_DSTBON     (0x4F)
#define RM692c9_WRDISBV    (0x51)
#define RM692c9_RDDISBV    (0x52)
#define RM692c9_WRCTRLD    (0x53)
#define RM692c9_RDCTRLD    (0x54)
#define RM692c9_WRRADACL   (0x55)
#define RM692c9_RDRADACL   (0x56)
#define RM692c9_WRCE       (0x58)
#define RM692c9_RDCE       (0x59)
#define RM692c9_WRCE1      (0x5A)
#define RM692c9_RDCE1      (0x5B)
#define RM692c9_WRCE2      (0x5C)
#define RM692c9_RDCE2      (0x5D)
#define RM692c9_WRTMR      (0x62)
#define RM692c9_RDTMR      (0x63)
#define RM692c9_WRPA       (0x64)
#define RM692c9_RDPA       (0x65)
#define RM692c9_WRWB       (0x66)
#define RM692c9_RDWB       (0x67)
#define RM692c9_WRCEMODE   (0x68)
#define RM692c9_RDCEMODE   (0x69)
#define RM692c9_RDDDBS     (0xA1)
#define RM692c9_RDDDBC     (0xA8)
#define RM692c9_RDFC       (0xAA)
#define RM692c9_RDCC       (0xAF)
#define RM692c9_SETDSIMODE (0xC2)
#define RM692c9_RDCTRLD1   (0xDA)
#define RM692c9_RDCTRLD2   (0xDB)
#define RM692c9_RDCTRLD3   (0xDC)
#define RM692c9_WRMAUCCTR  (0xFE)
#define RM692c9_RDMAUCCTR  (0xFF)

/*!
 * @brief RM692c9 resource.
 */
typedef struct _rm692c9_resource
{
    mipi_dsi_device_t *dsiDevice;      /*!< MIPI DSI device. */
    void (*pullResetPin)(bool pullUp); /*!< Function to pull reset pin high or low. */
} rm692c9_resource_t;

extern const display_operations_t rm692c9_ops;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

status_t RM692c9_Init(display_handle_t *handle, const display_config_t *config);

status_t RM692c9_Deinit(display_handle_t *handle);

status_t RM692c9_Start(display_handle_t *handle);

status_t RM692c9_Stop(display_handle_t *handle);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_RM692c9_H_ */
