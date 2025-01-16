/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CAMERA_CONFIG_H_
#define CAMERA_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Define whether to store camera video data in PSRAM .*/
#ifndef CAMERA_PSRAM
#define CAMERA_PSRAM 0
#endif

/*! @brief XCLK's frequency */
#define DEMO_FLEXIO_XCLK_FREQ_HZ            (12000000U)
/*! @brief FLEXIO TIMER1 used to generate PWM */
#define DEMO_FLEXIO_XCLK_TIMER              (1U)
/*! @brief FLEXIO time0 for shifter to receive data */
#define DEMO_FLEXIO_PCLK_TIMER              (0U)

/*! @brief  Select FXIO2_D1 as XCLK */
#define DEMO_FLEXIO_XCLK_IDX                (1U)
#define DEMO_FELXIO_HREF_IDX                (2U)
#define DEMO_FLEXIO_DATA0_IDX               (6U)
#define DEMO_FLEXIO_DATA_WIDTH              (8U)
#define DEMO_FELXIO_PCLK_IDX                (15U)
#define DEMO_FLEXIO_SHIFTER0_IDX            (0U)
#define DEMO_FLEXIO_SHIFTER_NUM             (8U)

/*! @brief definition related RESET and XSYNC signals */
#define DEMO_XSYNC_IRQ        GPIO20_IRQn
#define DEMO_XSYNC_PORT       (GPIO2)
#define DEMO_XSYNC_PIN        (5U)
#define DEMO_RESET_PORT       (GPIO2)
#define DEMO_RESET_PIN        (0U)

/*! @brief defintions for ov7670 camera sensor */
#define CAMERA_BPP            (2U)
#define CAMERA_FRAME_RATE     (30U)
#define CAMERA_FRAMEBUF_NUM   (2U)
#define CAMERA_WIDTH          (320U)
#define CAMERA_HEIGHT         (240U)
#define CAMERA_STRIDE    (CAMERA_WIDTH *CAMERA_BPP)
#define CAMERA_CTRL_FLAG (kCAMERA_HrefActiveHigh|kCAMERA_DataLatchOnRisingEdge)

/*! @brief video buffer pointer and length */
typedef volatile struct tag_CameraData
{
    volatile uint32_t len; /* pixel bytes */
    volatile void *pBuf;
} CameraBuffer_t;

#define QUEUE_SIZE    (2U)
typedef volatile struct tag_CameraTransfer
{
    volatile CameraBuffer_t queue[QUEUE_SIZE];
    volatile uint8_t driverIdx;
    volatile uint8_t userIdx;
    /*!< Semaphore for the camera. */
    //SemaphoreHandle_t semaNewFrame;
} CameraDvpTransfer;

#endif
