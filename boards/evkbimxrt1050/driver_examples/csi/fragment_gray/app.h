/*
 * Copyright  2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/* LCD definition. */
#define DEMO_ELCDIF LCDIF

#define DEMO_LCD_HEIGHT 272
#define DEMO_LCD_WIDTH  480
#define DEMO_HSW        41
#define DEMO_HFP        4
#define DEMO_HBP        8
#define DEMO_VSW        10
#define DEMO_VFP        4
#define DEMO_VBP        2
#define DEMO_LCD_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

#define DEMO_LCDIF_DATA_BUS_WIDTH 16

/* Display. */
#define LCD_DISP_GPIO     GPIO1
#define LCD_DISP_GPIO_PIN 2
/* Back light. */
#define LCD_BL_GPIO     GPIO2
#define LCD_BL_GPIO_PIN 31

/* Camera definition. */
#define DEMO_CSI           CSI
#define DEMO_CAMERA_HEIGHT 272
#define DEMO_CAMERA_WIDTH  480

#define DEMO_CAMERA_OV7725  0
#define DEMO_CAMERA_MT9M114 1

#define DEMO_CAMERA_TYPE DEMO_CAMERA_MT9M114

/* Frame buffer data alignment. */
#define FRAME_BUFFER_ALIGN 64

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
extern camera_device_handle_t cameraDevice;
/*${prototype:end}*/

#endif /* _APP_H_ */
