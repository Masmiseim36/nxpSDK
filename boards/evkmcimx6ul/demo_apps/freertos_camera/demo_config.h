/*
 * Copyright 2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __DEMO_CONFIG_H__
#define __DEMO_CONFIG_H__

/*================= Task ==================== */
#define INIT_TASK_STACK_SIZE 0x200
#define INIT_TASK_PRIORITY 3
#define DISPLAY_TASK_STACK_SIZE 0x200
#define DISPLAY_TASK_PRIORITY 3
#define SDMMC_TASK_STACK_SIZE 0x400
#define SDMMC_TASK_PRIORITY 2

/*============== Interrupt ========= */
#define INIT_PXP_INT()                 \
    do                                 \
    {                                  \
        GIC_SetPriority(PXP_IRQn, 21); \
        GIC_EnableIRQ(PXP_IRQn);       \
    } while (0)

#define INIT_LCDIF_INT()                 \
    do                                   \
    {                                    \
        GIC_SetPriority(LCDIF_IRQn, 21); \
        GIC_EnableIRQ(LCDIF_IRQn);       \
    } while (0)

#define INIT_CAMERA_INT()              \
    do                                 \
    {                                  \
        GIC_SetPriority(CSI_IRQn, 21); \
        GIC_EnableIRQ(CSI_IRQn);       \
    } while (0)

#define INIT_SDMMC_INT()                  \
    do                                    \
    {                                     \
        GIC_SetPriority(USDHC1_IRQn, 21); \
        GIC_EnableIRQ(USDHC1_IRQn);       \
    } while (0)

/*================= Touch ==================== */
#define TOUCH_TSC TSC
#define TOUCH_ADC ADC_5HC

/*================= Display ==================== */
#define DISPLAY_LCDIF LCDIF
#define DISPLAY_PXP PXP
#define LCD_DATA_BUS kELCDIF_DataBus24Bit
#define LCD_HEIGHT 272
#define LCD_WIDTH 480
#define LCD_HSW 41
#define LCD_HFP 4
#define LCD_HBP 8
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 2
#define LCD_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

/*================= Camera ==================== */
#define CAMERA_WIDTH 480
#define CAMERA_HEIGHT 272
#define CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)

/* Frame buffer data alignment. */
#define FRAME_BUFFER_ALIGN 64

#endif /* __DEMO_CONFIG_H__ */
