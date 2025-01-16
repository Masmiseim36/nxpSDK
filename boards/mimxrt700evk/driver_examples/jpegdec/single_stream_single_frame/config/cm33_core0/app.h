/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_jpegdec.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
JPEG_DECODER_Type g_jpegdec = {
    .core    = JPEGDEC,
    .wrapper = JPGDECWRP,
};

#define APP_JPEGDEC            (&g_jpegdec)
#define APP_JPEGDEC_IRQHandler JPEGDEC_IRQHandler
#define APP_JPEGDEC_IRQn       JPEGDEC_IRQn

#define DEMO_BUFFER2_ADDR 0x60400000U
#define DEMO_BUFFER3_ADDR 0x60600000U

#define DEMO_FB_ADDR  0x60400000U
#define DEMO_FB0_ADDR 0x60800000U
#define DEMO_FB1_ADDR 0x60A00000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
