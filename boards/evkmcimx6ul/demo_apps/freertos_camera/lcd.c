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

#include "fsl_elcdif.h"
#include "lcd.h"
#include "demo_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Only support RGB565. */
#define LCD_BPP 2
#define ELCDIF_PIXEL_FORMAT kELCDIF_PixelFormatRGB565
#define LCD_PIXEL_FORMAT_T uint16_t

typedef struct _lcd_handle
{
    volatile uint32_t activeBuf;   /*!< Address of the active frame buffer. */
    volatile uint32_t inactiveBuf; /*!< Address of the inactive frame buffer. */
    volatile bool isBufPending;    /*!< The inactive buffer has been set to LCD, but still not actived. */
    SemaphoreHandle_t sema;
} lcd_handle_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static lcd_handle_t s_lcdHandle;

AT_NONCACHEABLE_SECTION_ALIGN(static LCD_PIXEL_FORMAT_T s_frameBuffer[2][LCD_HEIGHT][LCD_WIDTH], FRAME_BUFFER_ALIGN);

/*******************************************************************************
 * Code
 ******************************************************************************/
void LCD_IRQHandler(void)
{
    uint32_t bufAddr;
    BaseType_t xHigherPriorityTaskWoken;

    if (kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(DISPLAY_LCDIF))
    {
        ELCDIF_ClearInterruptStatus(DISPLAY_LCDIF, kELCDIF_CurFrameDone);

        if (s_lcdHandle.isBufPending)
        {
            /* The inactive is actived when frame done interrupt occurs. */
            bufAddr = s_lcdHandle.inactiveBuf;
            s_lcdHandle.inactiveBuf = s_lcdHandle.activeBuf;
            s_lcdHandle.activeBuf = bufAddr;
            s_lcdHandle.isBufPending = false;

            xHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(s_lcdHandle.sema, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

void LCD_Init(void)
{
    memset(&s_lcdHandle, 0, sizeof(s_lcdHandle));
    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));

    s_lcdHandle.activeBuf = (uint32_t)s_frameBuffer[0];
    s_lcdHandle.inactiveBuf = (uint32_t)s_frameBuffer[1];
    s_lcdHandle.isBufPending = false;
    s_lcdHandle.sema = NULL;

    const elcdif_rgb_mode_config_t config = {
        .panelWidth = LCD_WIDTH,
        .panelHeight = LCD_HEIGHT,
        .hsw = LCD_HSW,
        .hfp = LCD_HFP,
        .hbp = LCD_HBP,
        .vsw = LCD_VSW,
        .vfp = LCD_VFP,
        .vbp = LCD_VBP,
        .polarityFlags = LCD_POL_FLAGS,
        .bufferAddr = (uint32_t)s_lcdHandle.activeBuf,
        .pixelFormat = ELCDIF_PIXEL_FORMAT,
        .dataBus = LCD_DATA_BUS,
    };

    vSemaphoreCreateBinary(s_lcdHandle.sema);

    configASSERT(NULL != s_lcdHandle.sema);

    ELCDIF_RgbModeInit(DISPLAY_LCDIF, &config);
    ELCDIF_EnableInterrupts(DISPLAY_LCDIF, kELCDIF_CurFrameDoneInterruptEnable);

    INIT_LCDIF_INT();

    ELCDIF_RgbModeStart(DISPLAY_LCDIF);
}

uint32_t LCD_BeginDraw(bool copyActiveBuf)
{
    xSemaphoreTake(s_lcdHandle.sema, portMAX_DELAY);

    if (copyActiveBuf)
    {
        memcpy((void *)s_lcdHandle.inactiveBuf, (void *)s_lcdHandle.activeBuf, LCD_BPP * LCD_WIDTH * LCD_HEIGHT);
    }

    return s_lcdHandle.inactiveBuf;
}

void LCD_EndDraw(void)
{
    /*
     * Draw end, pass the buffer to LCDIF.
     */
    ELCDIF_SetNextBufferAddr(DISPLAY_LCDIF, s_lcdHandle.inactiveBuf);
    s_lcdHandle.isBufPending = true;
}
