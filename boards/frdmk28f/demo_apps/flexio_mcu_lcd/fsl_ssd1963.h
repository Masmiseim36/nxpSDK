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

#ifndef _FSL_SSD1963_H_
#define _FSL_SSD1963_H_

#include "fsl_flexio_mculcd.h"
#include "fsl_flexio_mculcd_edma.h"

/*!
 * @addtogroup ssd1963
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Data width between host and SSD1963 controller. */
#define SSD1963_DATA_WITDH (16)

/*! @brief SSD1963 command. */
#define SSD1963_NOP 0x00
#define SSD1963_SOFT_RESET 0x01
#define SSD1963_GET_POWER_MODE 0x0A
#define SSD1963_GET_ADDRESS_MODE 0x0B
#define SSD1963_GET_DISPLAY_MODE 0x0D
#define SSD1963_GET_TEAR_EFFECT_STATUS 0x0E
#define SSD1963_ENTER_SLEEP_MODE 0x10
#define SSD1963_EXIT_SLEEP_MODE 0x11
#define SSD1963_ENTER_PARTIAL_MODE 0x12
#define SSD1963_ENTER_NORMAL_MODE 0x13
#define SSD1963_EXIT_INVERT_MODE 0x20
#define SSD1963_ENTER_INVERT_MODE 0x21
#define SSD1963_SET_GAMMA_CURVE 0x26
#define SSD1963_SET_DISPLAY_OFF 0x28
#define SSD1963_SET_DISPLAY_ON 0x29
#define SSD1963_SET_COLUMN_ADDRESS 0x2A
#define SSD1963_SET_PAGE_ADDRESS 0x2B
#define SSD1963_WRITE_MEMORY_START 0x2C
#define SSD1963_READ_MEMORY_START 0x2E
#define SSD1963_SET_PARTIAL_AREA 0x30
#define SSD1963_SET_SCROLL_AREA 0x33
#define SSD1963_SET_TEAR_OFF 0x34
#define SSD1963_SET_TEAR_ON 0x35
#define SSD1963_SET_ADDRESS_MODE 0x36
#define SSD1963_SET_SCROLL_START 0x37
#define SSD1963_EXIT_IDLE_MODE 0x38
#define SSD1963_ENTER_IDLE_MODE 0x39
#define SSD1963_WRITE_MEMORY_CONTINUE 0x3C
#define SSD1963_READ_MEMORY_CONTINUE 0x3E
#define SSD1963_SET_TEAR_SCANLINE 0x44
#define SSD1963_GET_SCANLINE 0x45
#define SSD1963_READ_DDB 0xA1
#define SSD1963_SET_LCD_MODE 0xB0
#define SSD1963_GET_LCD_MODE 0xB1
#define SSD1963_SET_HORI_PERIOD 0xB4
#define SSD1963_GET_HORI_PERIOD 0xB5
#define SSD1963_SET_VERT_PERIOD 0xB6
#define SSD1963_GET_VERT_PERIOD 0xB7
#define SSD1963_SET_GPIO_CONF 0xB8
#define SSD1963_GET_GPIO_CONF 0xB9
#define SSD1963_SET_GPIO_VALUE 0xBA
#define SSD1963_GET_GPIO_STATUS 0xBB
#define SSD1963_SET_POST_PROC 0xBC
#define SSD1963_GET_POST_PROC 0xBD
#define SSD1963_SET_PWM_CONF 0xBE
#define SSD1963_GET_PWM_CONF 0xBF
#define SSD1963_GET_LCD_GEN0 0xC0
#define SSD1963_SET_LCD_GEN0 0xC1
#define SSD1963_GET_LCD_GEN1 0xC2
#define SSD1963_SET_LCD_GEN1 0xC3
#define SSD1963_GET_LCD_GEN2 0xC4
#define SSD1963_SET_LCD_GEN2 0xC5
#define SSD1963_GET_LCD_GEN3 0xC6
#define SSD1963_SET_LCD_GEN3 0xC7
#define SSD1963_SET_GPIO0_ROP 0xC8
#define SSD1963_GET_GPIO0_ROP 0xC9
#define SSD1963_SET_GPIO1_ROP 0xCA
#define SSD1963_GET_GPIO1_ROP 0xCB
#define SSD1963_SET_GPIO2_ROP 0xCC
#define SSD1963_GET_GPIO2_ROP 0xCD
#define SSD1963_SET_GPIO3_ROP 0xCE
#define SSD1963_GET_GPIO3_ROP 0xCF
#define SSD1963_SET_DBC_CONF 0xD0
#define SSD1963_GET_DBC_CONF 0xD1
#define SSD1963_SET_DBC_TH 0xD4
#define SSD1963_GET_DBC_TH 0xD5
#define SSD1963_SET_PLL 0xE0
#define SSD1963_SET_PLL_MN 0xE2
#define SSD1963_GET_PLL_MN 0xE3
#define SSD1963_GET_PLL_STATUS 0xE4
#define SSD1963_SET_DEEP_SLEEP 0xE5
#define SSD1963_SET_LSHIFT_FREQ 0xE6
#define SSD1963_GET_LSHIFT_FREQ 0xE7
#define SSD1963_SET_PIXEL_DATA_INTERFACE 0xF0
#define SSD1963_GET_PIXEL_DATA_INTERFACE 0xF1

#define SSD1963_ADDR_MODE_FLIP_VERT (1 << 0)
#define SSD1963_ADDR_MODE_FLIP_HORZ (1 << 1)
#define SSD1963_ADDR_MODE_LATCH_RIGHT_TO_LEFT (1 << 2)
#define SSD1963_ADDR_MODE_BGR (1 << 3)
#define SSD1963_ADDR_MODE_REFRESH_BOTTOM_UP (1 << 4)
#define SSD1963_ADDR_MODE_PAG_COL_ADDR_ORDER (1 << 5)
#define SSD1963_ADDR_MODE_COL_ADDR_ORDER (1 << 6)
#define SSD1963_ADDR_MODE_PAGE_ADDR_ORDER (1 << 7)

/*! @brief ssd1963 handle. */
typedef struct _ssd1963_handle
{
    FLEXIO_MCULCD_Type *base;                    /*!< FLEXIO LCD device structure. */
    flexio_mculcd_edma_handle_t flexioLcdHandle; /*!< FLEXIO LCD EDMA driver handle. */
    uint8_t addrMode;                            /*!< The parameter of set_address_mode and get_address_mode. */
    uint16_t panelWidth;                         /*!< Width of the panel. */
    uint16_t panelHeight;                        /*!< Height of the panel. */
    volatile bool transferCompletedFlag;         /*!< EDMA transfer completed. */
} ssd1963_handle_t;

/*! @brief SSD1963 TFT interface timing polarity flags. */
enum _ssd1963_polarity_flags
{
    kSSD1963_HsyncActiveHigh = (1U << 1U),            /*!< Set then the HSYNC will be active high. */
    kSSD1963_VsyncActiveHigh = (1U << 0U),            /*!< Set then the VSYNC will be active high. */
    kSSD1963_LatchDataInClockRisingEdge = (1U << 2U), /*!< Set then the data latched in clock rising edge. */
};

/*! @brief The data width between SSD1963 and TFT panel. */
typedef enum _ssd1963_panel_data_width
{
    kSSD1963_PanelData18Bit = 0 << 5, /*!< 18-bit panel data. */
    kSSD1963_PanelData24Bit = 1 << 5, /*!< 24-bit panel data. */
} ssd1963_panel_data_width_t;

/*!
 * @brief The pixel data interface format.
 * Currently only supports such formats
 *  - 16-bits interface, pixel format RGB565, every pixel is sent by one cycle.
 */
typedef enum _ssd1963_pixel_interface
{
    kSSD1963_PixelInterface16Bit565 = 3, /*!< 16-bits interface, pixel format RGB565. */
} ssd1963_pixel_interface_t;

/*! @brief Initailize structure of ssd1963 */
typedef struct _ssd1963_config
{
    uint32_t pclkFreq_Hz;                      /*!< Pixel clock (LSHIFT clock) frequency in Hz. */
    ssd1963_pixel_interface_t pixelInterface;  /*!< Pixel interface format. */
    ssd1963_panel_data_width_t panelDataWidth; /*!< Panel data width. */
    uint8_t polarityFlags;                     /*!< OR'ed value of @ref _ssd1963_polarity_flags. */
    uint16_t panelWidth;                       /*!< How many pixels per line. */
    uint16_t panelHeight;                      /*!< How many lines per panel. */
    uint16_t hsw;                              /*!< HSYNC pulse width. */
    uint16_t hfp;                              /*!< Horizontal front porch. */
    uint16_t hbp;                              /*!< Horizontal back porch. */
    uint16_t vsw;                              /*!< VSYNC pulse width. */
    uint16_t vfp;                              /*!< Vrtical front porch. */
    uint16_t vbp;                              /*!< Vertical back porch. */
} ssd1963_config_t;

/*! @brief SSD1963 flip mode. */
typedef enum _ssd1963_flip_mode
{
    kSSD1963_FlipNone = 0U,                                /*!< No flip. */
    kSSD1963_FlipVertical = SSD1963_ADDR_MODE_FLIP_VERT,   /*!< Flip vertical, set_address_mode A[0] */
    kSSD1963_FlipHorizontal = SSD1963_ADDR_MODE_FLIP_HORZ, /*!< Flip horizontal, set_address_mode A[1] */
    kSSD1963_FlipBoth =
        SSD1963_ADDR_MODE_FLIP_VERT | SSD1963_ADDR_MODE_FLIP_HORZ, /*!< Flip both vertical and horizontal. */
} ssd1963_flip_mode_t;

/*!
 * @brief SSD1963 orientation mode.
 * After setting the orientation mode, the accress order from host processor to
 * frame buffer will be changed.
 */
typedef enum _ssd1963_orientation_mode_t
{
    kSSD1963_Orientation0 = 0U, /*!< Rotate 0 degree. */
    kSSD1963_Orientation90 =
        SSD1963_ADDR_MODE_PAGE_ADDR_ORDER | SSD1963_ADDR_MODE_PAG_COL_ADDR_ORDER, /*!< Rotate 90 degree. */
    kSSD1963_Orientation180 =
        SSD1963_ADDR_MODE_PAGE_ADDR_ORDER | SSD1963_ADDR_MODE_COL_ADDR_ORDER, /*!< Rotate 180 degree. */
    kSSD1963_Orientation270 =
        SSD1963_ADDR_MODE_COL_ADDR_ORDER | SSD1963_ADDR_MODE_PAG_COL_ADDR_ORDER, /*!< Rotate 270 degree. */
} ssd1963_orientation_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initailize the SSD1963.
 *
 * This function configures the SSD1963 contoller, and setups the interface
 * between LCD panel. This function does not turn on the display, application
 * could turn on the display after filling the frame buffer.
 *
 * @param handle SSD1963 handle structure.
 * @param config Pointer to the SSD1963 configuration structure.
 * @param base Pointer to the FLEXIO LCD device.
 * @param txEdmaHandle eDMA handle for FlexIO MCULCD eDMA TX.
 * @param rxEdmaHandle eDMA handle for FlexIO MCULCD eDMA RX.
 * @param srcClock_Hz The external reference clock(XTAL or CLK) frequency in Hz.
 *
 * @retval kStatus_Success Initailize successfully.
 * @retval kStatus_InvalidArgument Initailize failed because of invalid argument.
 */
status_t SSD1963_Init(ssd1963_handle_t *handle,
                      const ssd1963_config_t *config,
                      FLEXIO_MCULCD_Type *base,
                      edma_handle_t *txEdmaHandle,
                      edma_handle_t *rxEdmaHandle,
                      uint32_t srcClock_Hz);

/*!
 * @brief Deinitailize the SSD1963.
 *
 * @param handle SSD1963 handle structure.
 */
void SSD1963_Deinit(ssd1963_handle_t *handle);

/*!
 * @brief Set display on.
 *
 * @param handle SSD1963 handle structure.
 */
void SSD1963_StartDisplay(ssd1963_handle_t *handle);

/*!
 * @brief Set display off.
 *
 * @param handle SSD1963 handle structure.
 */
void SSD1963_StopDisplay(ssd1963_handle_t *handle);

/*!
 * @brief Set display flip mode.
 *
 * @param handle SSD1963 handle structure.
 * @param mode The flip mode.
 */
void SSD1963_SetFlipMode(ssd1963_handle_t *handle, ssd1963_flip_mode_t mode);

/*!
 * @brief Set display orientation mode.
 *
 * @param handle SSD1963 handle structure.
 * @param mode The orientation mode.
 */
void SSD1963_SetOrientationMode(ssd1963_handle_t *handle, ssd1963_orientation_mode_t mode);

/*!
 * @brief Set the backlight value.
 *
 * @param handle SSD1963 handle structure.
 * @param value Backlight value, must be in the range of 0 to 255.
 */
void SSD1963_SetBackLight(ssd1963_handle_t *handle, uint8_t value);

/*!
 * @brief Select area to read or write.
 *
 * @param handle SSD1963 handle structure.
 * @param startX Start point X coordination.
 * @param startY Start point Y coordination.
 * @param endX End point X coordination.
 * @param endY End point Y coordination.
 */
void SSD1963_SelectArea(ssd1963_handle_t *handle, uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY);

/*!
 * @brief Read pixel data from the selected area.
 *
 * This function reads pixel data to the area selected by the function
 * @ref SSD1963_SelectArea. The pixel data will be read from the start of
 * the area, it could not read the pixels out of the selected area.
 *
 * @param handle SSD1963 handle structure.
 * @param pixels Pointer to the memory to save the read pixels.
 * @param length Length of the pixel array @p pixels.
 */
void SSD1963_ReadPixels(ssd1963_handle_t *handle, uint16_t *pixels, uint32_t length);

/*!
 * @brief Write pixel data to the selected area.
 *
 * This function writes pixel data to the area selected by the function
 * @ref SSD1963_SelectArea. The pixel data will be written from the start of
 * the area, if the pixel data written is larger than the area size, the part
 * out of range will be ignored.
 *
 * @param handle SSD1963 handle structure.
 * @param pixels Pointer to the pixels to write.
 * @param length Length of the pixel array @p pixels.
 */
void SSD1963_WritePixels(ssd1963_handle_t *handle, const uint16_t *pixels, uint32_t length);

/*!
 * @brief Write same pixel data to the selected area.
 *
 * This function could be used to clear the window easily. It sends the same pixel
 * color many times.
 *
 * @param handle SSD1963 handle structure.
 * @param color The color to send.
 * @param pixelCount How many pixels to set.
 */
void SSD1963_WriteSamePixels(ssd1963_handle_t *handle, const uint32_t color, uint32_t pixelCount);

/*!
 * @brief Enable or disable tear effect signal.
 *
 * @param handle SSD1963 handle structure.
 * @param enable Pass in true to enable, false to disable.
 */
void SSD1963_EnableTearEffect(ssd1963_handle_t *handle, bool enable);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _FSL_SSD1963_H_ */
