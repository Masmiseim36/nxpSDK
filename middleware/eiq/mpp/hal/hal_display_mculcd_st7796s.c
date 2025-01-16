/*
 * Copyright 2024 NXP.
 * All rights reserved.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * @brief display dev HAL driver implementation for ST7796S using flexio mcu-lcd 8080 interface.
 */

/* driver includes */
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"

/* mpp includes */
#include "mpp_config.h"
#include "mpp_api_types.h"
/* hal includes */
#include "hal_debug.h"
#include "hal_display_dev.h"
#include "hal_utils.h"
#include "hal_os.h"
/* board configuration includes */

#if (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_McuLcdST7796S == 1)

#include "fsl_common.h"
#include "fsl_st7796s.h"
#include "fsl_edma.h"
#include "fsl_flexio_mculcd.h"
#include "fsl_flexio_mculcd_edma.h"

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/******** constant values ********/
#define HAL_DISPLAY_NAME                                  "McuLcdST7796S"
#define HAL_DISPLAY_DEV_McuLcdST7796S_ROTATE              ROTATE_0
#define HAL_DISPLAY_DEV_McuLcdST7796S_BUFFER_COUNT        1
/******** configurable default values ********/
#define HAL_DISPLAY_DEV_McuLcdST7796S_HEIGHT              320U
#define HAL_DISPLAY_DEV_McuLcdST7796S_WIDTH               480U
#define HAL_DISPLAY_DEV_McuLcdST7796S_LEFT                0
#define HAL_DISPLAY_DEV_McuLcdST7796S_TOP                 0
#define HAL_DISPLAY_DEV_McuLcdST7796S_RIGHT               (HAL_DISPLAY_DEV_McuLcdST7796S_WIDTH - 1)
#define HAL_DISPLAY_DEV_McuLcdST7796S_BOTTOM              (HAL_DISPLAY_DEV_McuLcdST7796S_HEIGHT - 1)
#define HAL_DISPLAY_DEV_McuLcdST7796S_FORMAT              MPP_PIXEL_RGB565
/* only RGB565 is supported */
#define HAL_DISPLAY_DEV_McuLcdST7796S_BPP                 2
/* default stripe height */
#define HAL_SMARTDMA_STRIPES                              16
/* these macros represent the number of small area of the screen in
 * order to set the LCD background.
 */
#define HAL_DISPLAY_DEV_McuLcdST7796S_NB_SUB_IMG_LINES    20
#define HAL_DISPLAY_DEV_McuLcdST7796S_NB_SUB_IMG_COLUMNS  20

#if ((HAL_DISPLAY_DEV_McuLcdST7796S_HEIGHT % HAL_DISPLAY_DEV_McuLcdST7796S_NB_SUB_IMG_LINES !=0) || \
(HAL_DISPLAY_DEV_McuLcdST7796S_WIDTH % HAL_DISPLAY_DEV_McuLcdST7796S_NB_SUB_IMG_COLUMNS !=0))
#error "HAL_DisplayDev_McuLcdST7796S: Number of lines and columns should be a divider of respectively panel height and width."
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static st7796s_handle_t lcd_handle;
static edma_handle_t edma_tx_handle;
static flexio_mculcd_edma_handle_t flexio_lcd_handle;
static hal_sema_t s_transfer_done;
static hal_mutex_t s_mutex; /* prevent stopping controller during transfer */

/**** declarations ****/
static hal_display_status_t hal_init_flexio_mcu_lcd(void);
static hal_display_status_t hal_flush_display(const display_dev_t *dev, const uint8_t *pdata, int stripenum);
static hal_display_status_t hal_set_background_to_black(void);

static void board_set_cs_pin(bool set);
static void board_set_rs_pin(bool set);

static status_t hal_lcd_write_command(void *dbi_xfer_handle,
        uint32_t command);
static status_t hal_lcd_write_command_params(void *dbi_xfer_handle,
        void *data, uint32_t len_byte);
static status_t hal_lcd_write_memory(void *dbi_xfer_handle,
        uint32_t command, const void *data, uint32_t len_byte);

hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Init(display_dev_t *dev,
        mpp_display_params_t *config, mpp_callback_t callback, void *param);
hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Deinit(const display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Start(display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Stop(display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Blit(const display_dev_t *dev,
        void *frame, int stripe);
hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Getbufdesc(const display_dev_t *dev,
        hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy);

/**** static variables ****/

/* set the FlexIO MCU LCD device. */
static FLEXIO_MCULCD_Type s_display_dev_flexio_lcd = {
        .flexioBase          = BOARD_FLEXIO,
        .busType             = kFLEXIO_MCULCD_8080,
        .dataPinStartIndex   = BOARD_FLEXIO_DATA_PIN_START,
        .ENWRPinIndex        = BOARD_FLEXIO_WR_PIN,
        .RDPinIndex          = BOARD_FLEXIO_RD_PIN,
        .txShifterStartIndex = BOARD_FLEXIO_TX_START_SHIFTER,
        .txShifterEndIndex   = BOARD_FLEXIO_TX_END_SHIFTER,
        .rxShifterStartIndex = BOARD_FLEXIO_RX_START_SHIFTER,
        .rxShifterEndIndex   = BOARD_FLEXIO_RX_END_SHIFTER,
        .timerIndex          = BOARD_FLEXIO_TIMER,
        .setCSPin            = board_set_cs_pin,
        .setRSPin            = board_set_rs_pin,
        .setRDWRPin          = NULL /* Not used in 8080 mode. */
};

/* The functions used to drive the panel. */
static dbi_xfer_ops_t flexio_dbi_ops = {
        .writeCommand          = hal_lcd_write_command,
        .writeData             = hal_lcd_write_command_params,
        .writeMemory           = hal_lcd_write_memory,
        .readMemory            = NULL, /* memory read is not needed for this project. */
        .setMemoryDoneCallback = NULL, /* This used for upper layer to register callback.
                                      If the writeMemory is a non-blocking(or async)
                                      function, when write finished, the registered
                                      callback function will be called.
                                      In this integration, hal_lcd_write_memory
                                      blocks to wait the write done, it is not async
                                      implementation, so don't need to set callback. */
};

/* set the ST7796S default configuration. */
static st7796s_config_t st7796sConfig = {.driverPreset    = kST7796S_DriverPresetLCDPARS035,
                                        .pixelFormat     = kST7796S_PixelFormatRGB565,
                                        .orientationMode = kST7796S_Orientation270,
                                        .teConfig        = kST7796S_TEDisabled,
                                        .invertDisplay   = true,
                                        .flipDisplay     = true,
                                        .bgrFilter       = true};

const static display_dev_operator_t s_display_dev_mcu_lcd_ops = {
        .init        = HAL_DisplayDev_McuLcdST7796S_Init,
        .deinit      = HAL_DisplayDev_McuLcdST7796S_Deinit,
        .start       = HAL_DisplayDev_McuLcdST7796S_Start,
        .stop        = HAL_DisplayDev_McuLcdST7796S_Stop,
        .blit        = HAL_DisplayDev_McuLcdST7796S_Blit,
        .get_buf_desc    = HAL_DisplayDev_McuLcdST7796S_Getbufdesc,
};

static display_dev_t s_display_dev_mcu_lcd = {.id   = 0,
        .name = HAL_DISPLAY_NAME,
        .ops  = &s_display_dev_mcu_lcd_ops,
        .cap  = {.width       = HAL_DISPLAY_DEV_McuLcdST7796S_WIDTH,
                .height      = HAL_DISPLAY_DEV_McuLcdST7796S_HEIGHT,
                .pitch       = HAL_DISPLAY_DEV_McuLcdST7796S_WIDTH * HAL_DISPLAY_DEV_McuLcdST7796S_BPP,
                .left        = HAL_DISPLAY_DEV_McuLcdST7796S_LEFT,
                .top         = HAL_DISPLAY_DEV_McuLcdST7796S_TOP,
                .right       = HAL_DISPLAY_DEV_McuLcdST7796S_RIGHT,
                .bottom      = HAL_DISPLAY_DEV_McuLcdST7796S_BOTTOM,
                .rotate      = HAL_DISPLAY_DEV_McuLcdST7796S_ROTATE,
                .format      = HAL_DISPLAY_DEV_McuLcdST7796S_FORMAT,
                .nbFrameBuffer = HAL_DISPLAY_DEV_McuLcdST7796S_BUFFER_COUNT,
                .frameBuffers = NULL,
                .callback    = NULL,
                .user_data   = NULL}};

static void board_set_cs_pin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, (uint8_t)set);
}

static void board_set_rs_pin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, (uint8_t)set);
}

static void board_flexio_transfer_callback(FLEXIO_MCULCD_Type *base,
        flexio_mculcd_edma_handle_t *handle,
        status_t status,
        void *user_data)
{
    long task_awake = false;
    hal_sema_give_isr(s_transfer_done, &task_awake);
    hal_sched_yield(task_awake);
}

/* This function is used to write command to display */
static status_t hal_lcd_write_command(void *dbi_xfer_handle,
        uint32_t command)
{
    /* dbi_xfer_handle is actually flexio_lcd_handle,
     * set by ST7796S_Init 4th parameter. */
     flexio_mculcd_edma_handle_t *flexio_lcd_handle =
             (flexio_mculcd_edma_handle_t *)dbi_xfer_handle;

    /* Here use blocking way to send command, because command is short. */
    FLEXIO_MCULCD_Type *flexio_lcd = flexio_lcd_handle->base; /* flexio_lcdDev */

    FLEXIO_MCULCD_StartTransfer(flexio_lcd);
    FLEXIO_MCULCD_WriteCommandBlocking(flexio_lcd, command);
    FLEXIO_MCULCD_StopTransfer(flexio_lcd);

    return kStatus_Success;
}

/* This function is used to write command parameters to display */
static status_t hal_lcd_write_command_params(void *dbi_xfer_handle,
        void *data, uint32_t len_byte)
{
    /* dbi_xfer_handle is actually flexio_lcd_handle,
     * set by ST7796S_Init 4th parameter. */
    flexio_mculcd_edma_handle_t *flexio_lcd_handle =
            (flexio_mculcd_edma_handle_t *)dbi_xfer_handle;

    /*
     * Here use blocking way to write data, this function is generally
     * used to send command parameter, the data length is short.
     */
    FLEXIO_MCULCD_Type *flexio_lcd = flexio_lcd_handle->base; /* flexioLcdDev */

    FLEXIO_MCULCD_StartTransfer(flexio_lcd);
    FLEXIO_MCULCD_WriteDataArrayBlocking(flexio_lcd, data, len_byte);
    FLEXIO_MCULCD_StopTransfer(flexio_lcd);

    return kStatus_Success;
}

/* This function is used to transfer mcu data to display */
static status_t hal_lcd_write_memory(void *dbi_xfer_handle,
        uint32_t command, const void *data, uint32_t len_byte)
{
    status_t ret = kStatus_Success;

    /* dbi_xfer_handle is actually s_flexioLcdHandle,
     * set by ST7796S_Init 4th parameter. */
    flexio_mculcd_edma_handle_t *flexio_lcd_handle =
            (flexio_mculcd_edma_handle_t *)dbi_xfer_handle;

    FLEXIO_MCULCD_Type *flexio_lcd = flexio_lcd_handle->base; /* flexioLcdDev */

    flexio_mculcd_transfer_t xfer = {
            .command             = command,
            .mode                = kFLEXIO_MCULCD_WriteArray,
            .dataAddrOrSameValue = (uint32_t)data,
            .dataSize            = len_byte,
    };

    ret = FLEXIO_MCULCD_TransferEDMA(flexio_lcd, flexio_lcd_handle, &xfer);

    if (ret != kStatus_Success) {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S: Failed to transfer data to LCD\n");
        return kStatus_HAL_DisplayError;
    } else {
        /* Wait for transfer done. */
        if (hal_sema_take(s_transfer_done, HAL_MAX_TIMEOUT) != true) {
            HAL_LOGE("HAL_DisplayDev_McuLcdST7796S: LCD FLEXIO transfer error\r\n");
            return kStatus_HAL_DisplayError;
        }
    }

    return ret;
}

/* initialize flexio-MCULCD */
static hal_display_status_t hal_init_flexio_mcu_lcd(void)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    status_t status = kStatus_Success;

    flexio_mculcd_config_t flexio_mcu_lcd_config;
    edma_config_t edma_config;

    BOARD_InitLcdPins();

    const gpio_pin_config_t reset_pin_config = {
            .pinDirection = kGPIO_DigitalOutput,
            .outputLogic  = 1,
    };

    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
    CLOCK_AttachClk(kPLL0_to_FLEXIO);

    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio2);
    CLOCK_EnableClock(kCLOCK_Gpio4);

    /* Set SSD1963 CS, RS, and reset pin to output. */
    GPIO_PinInit(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, &reset_pin_config);
    GPIO_PinInit(BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, &reset_pin_config);
    GPIO_PinInit(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, &reset_pin_config);

    s_transfer_done = hal_sema_create_binary();

    if (NULL == s_transfer_done) {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S: Semaphore create failed\r\n");
        return kStatus_HAL_DisplayError;
    }

    /* FreeRTOS kernel API is used in EDMA ISR, so need to set proper IRQ priority. */
    NVIC_SetPriority(BOARD_EDMA_IRQ, hal_get_max_syscall_prio() + 1);

    FLEXIO_MCULCD_GetDefaultConfig(&flexio_mcu_lcd_config);
    flexio_mcu_lcd_config.baudRate_Bps = BOARD_FLEXIO_BAUDRATE_BPS;

    /* Configure FlexIO MCULCD */
    status = FLEXIO_MCULCD_Init(&s_display_dev_flexio_lcd, &flexio_mcu_lcd_config,
            BOARD_FLEXIO_CLOCK_FREQ);

    if (status != kStatus_Success) {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S: Failed to initialize FlexIO MCULCD\n");
        return kStatus_HAL_DisplayError;
    }

    /* Initialize and create EDMA handles. */
    EDMA_GetDefaultConfig(&edma_config);
    EDMA_Init(BOARD_EDMA, &edma_config);

    /* Initialize the internal state of eDMA handle */
    EDMA_CreateHandle(&edma_tx_handle, BOARD_EDMA, BOARD_FLEXIO_TX_DMA_CHANNEL);

#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(BOARD_EDMA, BOARD_FLEXIO_TX_DMA_CHANNEL, BOARD_FLEXIO_TX_DMA_REQUEST);
#endif

    /* Create FLEXIO_MCULCD eDMA handle. */
    status = FLEXIO_MCULCD_TransferCreateHandleEDMA(&s_display_dev_flexio_lcd, &flexio_lcd_handle,
            board_flexio_transfer_callback, NULL, &edma_tx_handle, NULL /* RX not used. */);

    if (status != kStatus_Success) {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S: Failed to create FlexIO_MCULCD eDMA handle\n");
        return kStatus_HAL_DisplayError;
    }

    return ret;
}

/* This function is used to transfer frame data from MCU to the display. */
static hal_display_status_t hal_flush_display(const display_dev_t *dev, const uint8_t *pdata, int stripenum)
{
    status_t status = kStatus_Success;
    int top, bottom;

    /* Define frame area where MCU can access. */
    /* Get buffer size */
    uint32_t pixel_count;
    if (dev->cap.stripe)
    {
        top = dev->cap.stripe_height * (stripenum - 1) + dev->cap.top;
        bottom = top + dev->cap.stripe_height - 1;
        pixel_count = dev->cap.stripe_height * dev->cap.width;
    }
    else
    {
        top = dev->cap.top;
        bottom = dev->cap.bottom;
        pixel_count = dev->cap.height * dev->cap.width;
    }

    status = ST7796S_SelectArea(&lcd_handle, dev->cap.left,
            top, dev->cap.right, bottom);

    if (status != kStatus_Success)  {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S: Failed to set display output area\n");
        return kStatus_HAL_DisplayError;
    }

    /* Transfer data from MCU to display frame memory. */
    status = ST7796S_WritePixels(&lcd_handle, (uint16_t *) pdata, pixel_count);

    if (status != kStatus_Success)  {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S: Failed to write data to display\n");
        return kStatus_HAL_DisplayError;
    }

    return kStatus_HAL_DisplaySuccess;
}

/*
 * this function is used to set display background to black,
 * otherwise, non-initialized data will be displayed.
 * Display mikro-e has a large frame resolution(320*480) that can not
 * be provided by MCUs that have limited memory, thus a smaller frame
 * buffer will be defined and written several times until the entire
 * screen is filled with black.
 * This operation is performed once at the start of the
 * pipeline.
 */
static hal_display_status_t hal_set_background_to_black(void)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    status_t status = kStatus_Success;

    int left = 0;
    int top = 0;

    int backgr_buff_height = HAL_DISPLAY_DEV_McuLcdST7796S_HEIGHT / HAL_DISPLAY_DEV_McuLcdST7796S_NB_SUB_IMG_LINES;
    int backgr_buff_width = HAL_DISPLAY_DEV_McuLcdST7796S_WIDTH / HAL_DISPLAY_DEV_McuLcdST7796S_NB_SUB_IMG_COLUMNS;

    /* use black pixels as background */
    int backgr_pix_count = backgr_buff_height * backgr_buff_width;

    uint8_t *backgr_buff = (uint8_t *)hal_malloc(backgr_pix_count * HAL_DISPLAY_DEV_McuLcdST7796S_BPP);

    if (backgr_buff == NULL) {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S: Failed to allocate "
                "display background buffer.\n");
        return kStatus_HAL_DisplayError;
    }

    /* set frame buffer pixels to black */
    memset(backgr_buff, 0x00, backgr_pix_count * HAL_DISPLAY_DEV_McuLcdST7796S_BPP);

    while (top != HAL_DISPLAY_DEV_McuLcdST7796S_HEIGHT) {
        while (left != HAL_DISPLAY_DEV_McuLcdST7796S_WIDTH) {

            /* Define frame area where MCU can access */
            status = ST7796S_SelectArea(&lcd_handle, left,
                    top, left + backgr_buff_width - 1,
                    top + backgr_buff_height - 1);
            if (status !=  kStatus_Success) {
                free(backgr_buff);
                return kStatus_HAL_DisplayError;
            }

            status = ST7796S_WritePixels(&lcd_handle, (uint16_t *) backgr_buff, backgr_pix_count);
            if (status !=  kStatus_Success) {
                free(backgr_buff);
                return kStatus_HAL_DisplayError;
            }

            left += backgr_buff_width;
        }

        top += backgr_buff_height;
        left = 0;
    }

    free(backgr_buff);

    return ret;

}

hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Init(
        display_dev_t *dev, mpp_display_params_t *config, mpp_callback_t callback, void *user_data)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_McuLcdST7796S_Init\n");

    if (hal_mutex_create(&s_mutex) != MPP_SUCCESS)
    {
        HAL_LOGE("Failed to create mutex\n");
        return kStatus_HAL_DisplayError;
    }

    ret = hal_init_flexio_mcu_lcd();

    if (ret != kStatus_HAL_DisplaySuccess) {
        HAL_LOGE("Failed to initialize flexio mcu-lcd\n");
        return kStatus_HAL_DisplayError;
    }

    display_dev_private_capability_t *display_cap = &dev->cap;

    /* set default config */
    memcpy(display_cap, &s_display_dev_mcu_lcd.cap, sizeof(display_dev_private_capability_t) );

    /* check input pixel depth versus static config */
    int pixel_depth = get_bitpp(config->format)/8;

    if (pixel_depth > HAL_DISPLAY_DEV_McuLcdST7796S_BPP) {
        HAL_LOGE("Pixel depth = %d higher than max =%d (defined in mpp_config.h.\n",
                pixel_depth, HAL_DISPLAY_DEV_McuLcdST7796S_BPP);
        return kStatus_HAL_DisplayError;
    }

    display_cap->format = config->format;

    switch(config->format) {
        case MPP_PIXEL_RGB565:
            display_cap->pitch = display_cap->width * 2;
            break;
        default:
            HAL_LOGE("HAL_DisplayDev_McuLcdST7796S_Init: invalid pixel format parameter.\n");
            return kStatus_HAL_DisplayError;
    }

    /* check user resolution does not exceed the screen resolution */
    if (((config->width < 0) || (config->width > HAL_DISPLAY_DEV_McuLcdST7796S_WIDTH))
            || ((config->height < 0) || (config->height > HAL_DISPLAY_DEV_McuLcdST7796S_HEIGHT))
            || ((config->pitch < 0) || (config->pitch > display_cap->pitch))) {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S_Init: invalid resolution parameter.\n");
        return kStatus_HAL_DisplayError;
    }

    /* get user's display resolution */
    if ((config->width != 0) && (config->height != 0)) {
        display_cap->width = config->width;
        display_cap->height = config->height;
        display_cap->pitch = config->width * pixel_depth;
    }

    if (config->right != 0)
        display_cap->right = config->right;
    else
        display_cap->right = config->left + config->width - 1;

    if (config->bottom != 0)
        display_cap->bottom = config->bottom;
    else
        display_cap->bottom = config->top + config->height - 1;

    /* get user's provided display position */
    if ((config->top != 0) || (config->left != 0)) {
        display_cap->top = config->top;
        display_cap->left = config->left;
    }

    /* check that display area does not exceed the screen resolution */
    if ((display_cap->bottom > HAL_DISPLAY_DEV_McuLcdST7796S_BOTTOM)
            || (display_cap->right > HAL_DISPLAY_DEV_McuLcdST7796S_RIGHT)
            || (((display_cap->right - display_cap->left) != display_cap->width - 1))
            || ((display_cap->bottom - display_cap->top) != display_cap->height - 1)) {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S_Init: invalid area parameter.\n");
        return kStatus_HAL_DisplayError;
    }

    display_cap->rotate = config->rotate;
    display_cap->stripe = config->stripe;
    if(config->stripe)
        display_cap->stripe_height = config->height / HAL_SMARTDMA_STRIPES;
    else
        display_cap->stripe_height = 0;
    display_cap->frameBuffers = NULL;
    display_cap->callback = callback;
    display_cap->user_data = user_data;

    HAL_LOGD("--HAL_DisplayDev_McuLcdST7796S_Init\n");

    return ret;
}

hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Deinit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;

    EDMA_Deinit(BOARD_EDMA);

    hal_mutex_remove(s_mutex);

    return ret;
}

hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Start(display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    status_t status = kStatus_Success;
    HAL_LOGD("++HAL_DisplayDev_McuLcdST7796S_Start\n");
    display_dev_private_capability_t *display_cap = &dev->cap;

    /*------------------------------
     * Initialize the LCD controller
     * -----------------------------*/

    /* first init with no rotation to set the whole screen black */
    st7796sConfig.orientationMode = kST7796S_Orientation0;
    status = ST7796S_Init(&lcd_handle, &st7796sConfig, &flexio_dbi_ops,
            &flexio_lcd_handle);

    if (status != kStatus_Success) {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S: Failed to initialize panel.\n");
        return kStatus_HAL_DisplayError;
    }

    /* set background to black otherwise the panel memory
     * that is not filled is random value */
    ret = hal_set_background_to_black();
    if (ret != kStatus_HAL_DisplaySuccess) {
        HAL_LOGI("Unable to set background image, "
                "non display area may show random values.\n");
        ret = kStatus_HAL_DisplaySuccess;
    }

    /* change orientation for video frame transfer */
    switch(display_cap->rotate)
    {
    case ROTATE_90:
        st7796sConfig.orientationMode = kST7796S_Orientation90;
        break;
    case ROTATE_180:
        st7796sConfig.orientationMode = kST7796S_Orientation180;
        break;
    case ROTATE_270:
        st7796sConfig.orientationMode = kST7796S_Orientation270;
        break;
    case ROTATE_0:
    default:
        st7796sConfig.orientationMode = kST7796S_Orientation0;
        break;
    }
    /* re-init with rotation */
    status = ST7796S_Init(&lcd_handle, &st7796sConfig, &flexio_dbi_ops,
            &flexio_lcd_handle);

    if (status != kStatus_Success) {
        HAL_LOGE("HAL_DisplayDev_McuLcdST7796S: Failed to initialize panel.\n");
        return kStatus_HAL_DisplayError;
    }

    /*-------------------------
     * Start the display
     * -----------------------*/

    ST7796S_EnableDisplay(&lcd_handle, true);

    HAL_LOGD("--HAL_DisplayDev_McuLcdST7796S_Start\n");

    return ret;
}

hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Stop(display_dev_t *dev)
{
    status_t status = kStatus_Success;

    HAL_LOGD("++HAL_DisplayDev_McuLcdST7796S_Stop\n");

    if (hal_mutex_lock(s_mutex) != MPP_SUCCESS)
    {
        HAL_LOGE("Failed to lock mutex\n");
        return kStatus_HAL_DisplayError;
    }

    /* disable display */
    status = ST7796S_EnableDisplay(&lcd_handle, false);

    if (status != kStatus_Success) {
        HAL_LOGE("Failed to stop display\n");
        return kStatus_HAL_DisplayError;
    }

    if (hal_mutex_unlock(s_mutex) != MPP_SUCCESS)
    {
        HAL_LOGE("Failed to unlock mutex\n");
        return kStatus_HAL_DisplayError;
    }

    HAL_LOGD("--HAL_DisplayDev_McuLcdST7796S_Stop\n");

    return kStatus_HAL_DisplaySuccess;
}

/* Send pixel data to screen
 * @param [in] dev device handle
 * @param [in] frame pointer to pixel data
 * @param [in] stripe stripe number, 0 means full-frame
 * @return \ref return_codes
 */
hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Blit(const display_dev_t *dev, void *frame,
        int stripe)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;

    HAL_LOGD("++HAL_DisplayDev_McuLcdST7796S_Blit\n");

    if (frame == NULL) {
        HAL_LOGE("Invalid frame buffer\n");
        return kStatus_HAL_DisplayError;
    }

    if (hal_mutex_lock(s_mutex) != MPP_SUCCESS)
    {
        HAL_LOGE("Failed to lock mutex\n");
        return kStatus_HAL_DisplayError;
    }

    /* flush data to display */
    ret = hal_flush_display(dev, frame, stripe);

    if (ret != kStatus_HAL_DisplaySuccess) {
        HAL_LOGE("Failed to flush image to display\n");
        return kStatus_HAL_DisplayError;
    }

    if (hal_mutex_unlock(s_mutex) != MPP_SUCCESS)
    {
        HAL_LOGE("Failed to unlock mutex\n");
        return kStatus_HAL_DisplayError;
    }

#if (ENABLE_PISANO_CHECKSUM == 1)
    checksum_data_t checksum;
    checksum.type = CHECKSUM_TYPE_PISANO;
    if (stripe == 0) {
        checksum.value = calc_checksum((dev->cap.width * get_bitpp(dev->cap.format)/8) * dev->cap.height, frame);
        HAL_LOGD("CHECKSUM=0x%X\n", checksum.value);
    } else {
        checksum.value = 0;
        HAL_LOGD("Checksum not supported with stripes\n");
    }
    if (dev->cap.callback != NULL)
        dev->cap.callback(NULL, MPP_EVENT_INTERNAL_TEST_RESERVED,
                (void *) &checksum, dev->cap.user_data);
#endif

    HAL_LOGD("--HAL_DisplayDev_McuLcdST7796S_Blit\n");

    return ret;
}

hal_display_status_t HAL_DisplayDev_McuLcdST7796S_Getbufdesc(const display_dev_t *dev, hw_buf_desc_t *in_buf,
        mpp_memory_policy_t *policy)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_McuLcdST7796S_GetInput\n");

    do {
        if ((in_buf == NULL) || (policy == NULL)) {
            HAL_LOGE("\nNULL pointer to buffer descriptor\n");
            ret = kStatus_HAL_DisplayError;
            break;
        }

        /* set memory policy */
        *policy = HAL_MEM_ALLOC_NONE;
        in_buf->alignment = 0;
        in_buf->cacheable = false;
        in_buf->stride = 0;
        in_buf->addr = NULL;
    } while (false);

    HAL_LOGD("--HAL_DisplayDev_McuLcdST7796S_GetInput\n");

    return ret;
}

int HAL_DisplayDev_McuLcdST7796S_setup(display_dev_t *dev)
{
    dev->ops = &s_display_dev_mcu_lcd_ops;
    return 0;
}
#else /* !((defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_McuLcdST7796S == 1)) */
int HAL_DisplayDev_McuLcdST7796S_setup(display_dev_t *dev)
{
    HAL_LOGE("Display McuLcdST7796S not enabled\n");
    return -1;
}
#endif /* (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_McuLcdST7796S == 1) */
