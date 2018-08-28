/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "fsl_ssd1963.h"

/*******************************************************************************
 * Definitations
 ******************************************************************************/
#define SSD1963_ORIENTATION_MODE_MASK \
    (SSD1963_ADDR_MODE_PAGE_ADDR_ORDER | SSD1963_ADDR_MODE_PAG_COL_ADDR_ORDER | SSD1963_ADDR_MODE_COL_ADDR_ORDER)

#define SSD1963_FLIP_MODE_MASK (SSD1963_ADDR_MODE_FLIP_VERT | SSD1963_ADDR_MODE_FLIP_HORZ)

/* The PLL VCO clock must be in the range of (250MHz, 800MHz). */
#define SSD1963_VCO_MIN_HZ 250000000U
#define SSD1963_VCO_MAX_HZ 800000000U
#define SSD1963_PLL_MULTI_MIN 0x0U
#define SSD1963_PLL_MULTI_MAX 0xFFU
#define SSD1963_PLL_DIV_MIN 0x0U
#define SSD1963_PLL_DIV_MAX 0x1FU

/* The PLL output frequency will be configured to about 100MHz. */
#define SSD1963_PLL_FREQ_HZ 100000000U

/* The max value of LCDC_FPR to generate the lshift clock (pixel clock). */
#define SSD1963_LCDC_FPR_MAX 0xFFFFFU

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Use loop to delay.
 *
 * @param loops Number of the loops.
 */
static void SSD1963_Delay(uint32_t loops);

/*!
 * @brief Get the multiplier and divider setting for PLL.
 *
 * This function gets the multiplier and divider to generate PLL frequency at
 * about 100MHz. The actually PLL frequency is returned.
 *
 * @param multi The multiplier value.
 * @param div The divider value.
 * @param srcClock_Hz The external reference clock(XTAL or CLK) frequency in Hz.
 * @return Generated PLL frequency with the @p multi and @p div. If could not get
 * the desired PLL frequency, this function returns 0.
 */
static uint32_t SSD1963_GetPllDivider(uint8_t *multi, uint8_t *div, uint32_t srcClock_Hz);

/*!
 * @brief SSD1963 EDMA transfer complete callback function.
 */
void SSD1963_TransferCompletedCallback(FLEXIO_MCULCD_Type *base,
                                       flexio_mculcd_edma_handle_t *handle,
                                       status_t status,
                                       void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void SSD1963_Delay(uint32_t loops)
{
    while (loops--)
    {
        __NOP();
    }
}

void SSD1963_TransferCompletedCallback(FLEXIO_MCULCD_Type *base,
                                       flexio_mculcd_edma_handle_t *handle,
                                       status_t status,
                                       void *userData)
{
    if (kStatus_FLEXIO_MCULCD_Idle == status)
    {
        ((ssd1963_handle_t *)userData)->transferCompletedFlag = true;
    }
}

static uint32_t SSD1963_GetPllDivider(uint8_t *multi, uint8_t *div, uint32_t srcClock_Hz)
{
    uint32_t multiCur, divCur, pllFreqCur, vcoCur, diffCur;
    uint32_t multiCandidate = 0U;
    uint32_t divCandidate = 0U;
    uint32_t pllFreqCandidate = 0U;
    uint32_t diff = 0xFFFFFFFFU;

    for (multiCur = SSD1963_PLL_MULTI_MIN; multiCur <= SSD1963_PLL_MULTI_MAX; multiCur++)
    {
        vcoCur = srcClock_Hz * (multiCur + 1U);

        /* VCO must be larger than SSD1963_VCO_MIN_HZ. */
        if (vcoCur <= SSD1963_VCO_MIN_HZ)
        {
            continue;
        }

        /* VCO must be smaller than SSD1963_VCO_MAX_HZ. */
        if (vcoCur >= SSD1963_VCO_MAX_HZ)
        {
            break;
        }

        divCur = ((vcoCur + (SSD1963_PLL_FREQ_HZ / 2U)) / SSD1963_PLL_FREQ_HZ) - 1U;

        /*
         * VCO frequency must be in the range of (250MHz, 800MHz). The desired
         * PLL output frequency is 100MHz, then the divCur here must be in the
         * range of (1, 8). In this case, it is not necessary to check whether
         * divCur is in the range of (0, 31). But for safty when the desired
         * PLL frequency is changed, here check the upper range.
         */
#if ((((SSD1963_VCO_MAX_HZ + (SSD1963_PLL_FREQ_HZ / 2U)) / SSD1963_PLL_FREQ_HZ) - 1U) > SSD1963_PLL_DIV_MAX)
        if (divCur > SSD1963_PLL_DIV_MAX)
        {
            divCur = SSD1963_PLL_DIV_MAX;
        }
#endif

        pllFreqCur = vcoCur / (divCur + 1);

        if (SSD1963_PLL_FREQ_HZ > pllFreqCur)
        {
            diffCur = SSD1963_PLL_FREQ_HZ - pllFreqCur;
        }
        else
        {
            diffCur = pllFreqCur - SSD1963_PLL_FREQ_HZ;
        }

        /* Find better multi and divider. */
        if (diff > diffCur)
        {
            diff = diffCur;
            multiCandidate = multiCur;
            divCandidate = divCur;
            pllFreqCandidate = pllFreqCur;
        }
    }

    *multi = (uint8_t)multiCandidate;
    *div = (uint8_t)divCandidate;

    return pllFreqCandidate;
}

status_t SSD1963_Init(ssd1963_handle_t *handle,
                      const ssd1963_config_t *config,
                      FLEXIO_MCULCD_Type *base,
                      edma_handle_t *txEdmaHandle,
                      edma_handle_t *rxEdmaHandle,
                      uint32_t srcClock_Hz)
{
    assert(handle);
    assert(config);

    uint8_t multi, div;
    uint32_t pllFreq_Hz;
    uint32_t fpr; /* Pixel clock = PLL clock * ((fpr + 1) / 2^20) */
    uint16_t commandParam[8];
    uint16_t vt, vps, ht, hps;
    status_t status;

    pllFreq_Hz = SSD1963_GetPllDivider(&multi, &div, srcClock_Hz);

    /* Could not set the PLL to desired frequency. */
    if (!pllFreq_Hz)
    {
        return kStatus_InvalidArgument;
    }

    fpr = (uint32_t)(((float)config->pclkFreq_Hz / (float)pllFreq_Hz) * (float)(1U << 20U));

    if ((fpr < 1) || (fpr > (SSD1963_LCDC_FPR_MAX + 1)))
    {
        return kStatus_InvalidArgument;
    }

    fpr--;

    /* Initialize the handle. */
    memset(handle, 0, sizeof(ssd1963_handle_t));

    status = FLEXIO_MCULCD_TransferCreateHandleEDMA(base, &handle->flexioLcdHandle, SSD1963_TransferCompletedCallback,
                                                    handle, txEdmaHandle, rxEdmaHandle);

    if (kStatus_Success != status)
    {
        return status;
    }

    handle->base = base;
    handle->panelWidth = config->panelWidth;
    handle->panelHeight = config->panelHeight;

    FLEXIO_MCULCD_StartTransfer(base);

    /* Soft reset. */
    FLEXIO_MCULCD_WriteCommandBlocking(base, SSD1963_SOFT_RESET);
    SSD1963_Delay(50000);

    /* Setup the PLL. */
    /* Set the multiplier and divider. */
    commandParam[0] = multi;
    commandParam[1] = div | (1U << 5U);
    commandParam[2] = 1U << 2U;
    FLEXIO_MCULCD_WriteCommandBlocking(base, SSD1963_SET_PLL_MN);
    FLEXIO_MCULCD_WriteDataArrayBlocking(base, commandParam, 3U * 2U);

    /* Enable PLL. */
    commandParam[0] = 0x01U;
    FLEXIO_MCULCD_WriteCommandBlocking(base, SSD1963_SET_PLL);
    FLEXIO_MCULCD_WriteDataArrayBlocking(base, commandParam, 1U * 2U);

    /* Delay at least 100us, to wait for the PLL stable. */
    SSD1963_Delay(500);

    /* Use the PLL. */
    commandParam[0] = 0x03U;
    FLEXIO_MCULCD_WriteCommandBlocking(base, SSD1963_SET_PLL);
    FLEXIO_MCULCD_WriteDataArrayBlocking(base, commandParam, 1U * 2U);

    /* Configure the pixel clock. */
    commandParam[0] = (fpr & 0xFF0000U) >> 16U;
    commandParam[1] = (fpr & 0xFF00U) >> 8U;
    commandParam[2] = (fpr & 0xFFU);
    FLEXIO_MCULCD_WriteCommandBlocking(base, SSD1963_SET_LSHIFT_FREQ);
    FLEXIO_MCULCD_WriteDataArrayBlocking(base, commandParam, 3U * 2U);

    /* Configure LCD panel. */
    commandParam[0] = config->panelDataWidth | config->polarityFlags; /* Not enable FRC, dithering. */
    commandParam[1] = 0x20U;                                          /* TFT mode. */
    commandParam[2] = (config->panelWidth - 1) >> 8;
    commandParam[3] = (config->panelWidth - 1) & 0xFFU;
    commandParam[4] = (config->panelHeight - 1) >> 8;
    commandParam[5] = (config->panelHeight - 1) & 0xFFU;
    commandParam[6] = 0;
    FLEXIO_MCULCD_WriteCommandBlocking(base, SSD1963_SET_LCD_MODE);
    FLEXIO_MCULCD_WriteDataArrayBlocking(base, commandParam, 7 * 2U);

    /* Horizontal period setting. */
    ht = config->panelWidth + config->hsw + config->hfp + config->hbp;
    hps = config->hsw + config->hbp;
    commandParam[0] = (ht - 1U) >> 8U;
    commandParam[1] = (ht - 1U) & 0xFFU;
    commandParam[2] = hps >> 8U;
    commandParam[3] = hps & 0xFFU;
    commandParam[4] = config->hsw - 1U;
    commandParam[5] = 0U;
    commandParam[6] = 0U;
    commandParam[7] = 0U;
    FLEXIO_MCULCD_WriteCommandBlocking(base, SSD1963_SET_HORI_PERIOD);
    FLEXIO_MCULCD_WriteDataArrayBlocking(base, commandParam, 8 * 2U);

    /* Vertical period setting. */
    vt = config->panelHeight + config->vsw + config->vfp + config->vbp;
    vps = config->vsw + config->vbp;
    commandParam[0] = (vt - 1U) >> 8U;
    commandParam[1] = (vt - 1U) & 0xFFU;
    commandParam[2] = vps >> 8U;
    commandParam[3] = vps & 0xFFU;
    commandParam[4] = config->vsw - 1U;
    commandParam[5] = 0U;
    commandParam[6] = 0U;
    FLEXIO_MCULCD_WriteCommandBlocking(base, SSD1963_SET_VERT_PERIOD);
    FLEXIO_MCULCD_WriteDataArrayBlocking(base, commandParam, 7 * 2U);

    /* Data interface. */
    commandParam[0] = config->pixelInterface;
    FLEXIO_MCULCD_WriteCommandBlocking(base, SSD1963_SET_PIXEL_DATA_INTERFACE);
    FLEXIO_MCULCD_WriteDataArrayBlocking(base, commandParam, 1 * 2U);

    /* Address mode. */
    commandParam[0] = 0U;
    FLEXIO_MCULCD_WriteCommandBlocking(base, SSD1963_SET_ADDRESS_MODE);
    FLEXIO_MCULCD_WriteDataArrayBlocking(base, commandParam, 1 * 2U);

    FLEXIO_MCULCD_StopTransfer(base);

    return kStatus_Success;
}

void SSD1963_Deinit(ssd1963_handle_t *handle)
{
    assert(handle);

    memset(handle, 0, sizeof(ssd1963_handle_t));
}

void SSD1963_StartDisplay(ssd1963_handle_t *handle)
{
    FLEXIO_MCULCD_StartTransfer(handle->base);
    FLEXIO_MCULCD_WriteCommandBlocking(handle->base, SSD1963_SET_DISPLAY_ON);
    FLEXIO_MCULCD_StopTransfer(handle->base);
}

void SSD1963_StopDisplay(ssd1963_handle_t *handle)
{
    FLEXIO_MCULCD_StartTransfer(handle->base);
    FLEXIO_MCULCD_WriteCommandBlocking(handle->base, SSD1963_SET_DISPLAY_OFF);
    FLEXIO_MCULCD_StopTransfer(handle->base);
}

void SSD1963_SetFlipMode(ssd1963_handle_t *handle, ssd1963_flip_mode_t mode)
{
    uint8_t newAddrMode = (handle->addrMode & ~SSD1963_FLIP_MODE_MASK) | mode;

    FLEXIO_MCULCD_StartTransfer(handle->base);
    FLEXIO_MCULCD_WriteCommandBlocking(handle->base, SSD1963_SET_ADDRESS_MODE);
    FLEXIO_MCULCD_WriteSameValueBlocking(handle->base, newAddrMode, 2);
    FLEXIO_MCULCD_StopTransfer(handle->base);

    handle->addrMode = newAddrMode;
}

void SSD1963_SetOrientationMode(ssd1963_handle_t *handle, ssd1963_orientation_mode_t mode)
{
    uint8_t newAddrMode = (handle->addrMode & ~SSD1963_ORIENTATION_MODE_MASK) | mode;

    FLEXIO_MCULCD_StartTransfer(handle->base);
    FLEXIO_MCULCD_WriteCommandBlocking(handle->base, SSD1963_SET_ADDRESS_MODE);
    FLEXIO_MCULCD_WriteSameValueBlocking(handle->base, newAddrMode, 2);
    FLEXIO_MCULCD_StopTransfer(handle->base);

    handle->addrMode = newAddrMode;
}

void SSD1963_SelectArea(ssd1963_handle_t *handle, uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY)
{
    uint16_t sc; /* Start of column number. */
    uint16_t ec; /* End of column number. */
    uint16_t sp; /* Start of page number. */
    uint16_t ep; /* End of page number. */
    ssd1963_orientation_mode_t mode;
    uint16_t commandParam[4]; /* Command parameters for set_page_address and set_column_address. */

    mode = (ssd1963_orientation_mode_t)(handle->addrMode & SSD1963_ORIENTATION_MODE_MASK);

    switch (mode)
    {
        default:
        case kSSD1963_Orientation0:
            sp = startY;
            ep = endY;
            sc = startX;
            ec = endX;
            break;

        case kSSD1963_Orientation90:
            sp = handle->panelHeight - 1 - endX;
            ep = handle->panelHeight - 1 - startX;
            sc = startY;
            ec = endY;
            break;

        case kSSD1963_Orientation180:
            sp = handle->panelHeight - 1 - endY;
            ep = handle->panelHeight - 1 - startY;
            sc = handle->panelWidth - 1 - endX;
            ec = handle->panelWidth - 1 - startX;
            break;

        case kSSD1963_Orientation270:
            sp = startX;
            ep = endX;
            sc = handle->panelWidth - 1 - endY;
            ec = handle->panelWidth - 1 - startY;
            break;
    }

    /* Send the set_page_address command. */
    commandParam[0] = (sp & 0xFF00U) >> 8U;
    commandParam[1] = sp & 0xFFU;
    commandParam[2] = (ep & 0xFF00U) >> 8U;
    commandParam[3] = ep & 0xFFU;

    FLEXIO_MCULCD_StartTransfer(handle->base);
    FLEXIO_MCULCD_WriteCommandBlocking(handle->base, SSD1963_SET_PAGE_ADDRESS);
    FLEXIO_MCULCD_WriteDataArrayBlocking(handle->base, commandParam, 4 * 2);

    /* Send the set_column_address command. */
    commandParam[0] = (sc & 0xFF00U) >> 8U;
    commandParam[1] = sc & 0xFFU;
    commandParam[2] = (ec & 0xFF00U) >> 8U;
    commandParam[3] = ec & 0xFFU;

    FLEXIO_MCULCD_WriteCommandBlocking(handle->base, SSD1963_SET_COLUMN_ADDRESS);
    FLEXIO_MCULCD_WriteDataArrayBlocking(handle->base, commandParam, 4 * 2);
    FLEXIO_MCULCD_StopTransfer(handle->base);
}

void SSD1963_WritePixels(ssd1963_handle_t *handle, const uint16_t *pixels, uint32_t length)
{
    flexio_mculcd_transfer_t xfer;

    xfer.command = SSD1963_WRITE_MEMORY_START;
    xfer.mode = kFLEXIO_MCULCD_WriteArray;
    xfer.dataAddrOrSameValue = (uint32_t)pixels;
    xfer.dataSize = length * 2;

    handle->transferCompletedFlag = false;
    FLEXIO_MCULCD_TransferEDMA(handle->base, &handle->flexioLcdHandle, &xfer);
    while (!handle->transferCompletedFlag)
    {
    }
}

void SSD1963_ReadPixels(ssd1963_handle_t *handle, uint16_t *pixels, uint32_t length)
{
    flexio_mculcd_transfer_t xfer;

    xfer.command = SSD1963_READ_MEMORY_START;
    xfer.mode = kFLEXIO_MCULCD_ReadArray;
    xfer.dataAddrOrSameValue = (uint32_t)pixels;
    xfer.dataSize = length * 2;

    handle->transferCompletedFlag = false;
    FLEXIO_MCULCD_TransferEDMA(handle->base, &handle->flexioLcdHandle, &xfer);
    while (!handle->transferCompletedFlag)
    {
    }
}

void SSD1963_SetBackLight(ssd1963_handle_t *handle, uint8_t value)
{
    uint16_t commandParam[] = {0x06U, value, 0x01U, 0xFFU, 0x00U, 0x01U};

    FLEXIO_MCULCD_StartTransfer(handle->base);
    FLEXIO_MCULCD_WriteCommandBlocking(handle->base, SSD1963_SET_PWM_CONF);
    FLEXIO_MCULCD_WriteDataArrayBlocking(handle->base, commandParam, sizeof(commandParam));
    FLEXIO_MCULCD_StopTransfer(handle->base);
}

void SSD1963_WriteSamePixels(ssd1963_handle_t *handle, const uint32_t color, uint32_t pixelCount)
{
    flexio_mculcd_transfer_t xfer;

    xfer.command = SSD1963_WRITE_MEMORY_START;
    xfer.mode = kFLEXIO_MCULCD_WriteSameValue;
    xfer.dataAddrOrSameValue = color;
    xfer.dataSize = pixelCount * 2;

    handle->transferCompletedFlag = false;
    FLEXIO_MCULCD_TransferEDMA(handle->base, &handle->flexioLcdHandle, &xfer);
    while (!handle->transferCompletedFlag)
    {
    }
}

void SSD1963_EnableTearEffect(ssd1963_handle_t *handle, bool enable)
{
    FLEXIO_MCULCD_StartTransfer(handle->base);
    if (enable)
    {
        FLEXIO_MCULCD_WriteCommandBlocking(handle->base, SSD1963_SET_TEAR_ON);
        FLEXIO_MCULCD_WriteSameValueBlocking(handle->base, 0U, 2U);
    }
    else
    {
        FLEXIO_MCULCD_WriteCommandBlocking(handle->base, SSD1963_SET_TEAR_OFF);
    }
    FLEXIO_MCULCD_StopTransfer(handle->base);
}
