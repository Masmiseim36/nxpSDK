/*
 * Copyright 2023 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @brief display dev HAL driver implementation for SSD1963 using flexio mcu-lcd 8080 interface.
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
/* board configuration includes */

#if (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_McuLcdSsd1963 == 1)

#include <FreeRTOS.h>
#include "semphr.h"
#include <queue.h>

#include "fsl_common.h"
#include "fsl_ssd1963.h"
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
#define HAL_DISPLAY_NAME                                  "McuLcdSsd1963"
#define HAL_DISPLAY_DEV_McuLcdSsd1963_ROTATE              ROTATE_0
#define HAL_DISPLAY_DEV_McuLcdSsd1963_BUFFER_COUNT        1
/******** configurable default values ********/
#define HAL_DISPLAY_DEV_McuLcdSsd1963_HEIGHT              480U
#define HAL_DISPLAY_DEV_McuLcdSsd1963_WIDTH               800U
#define HAL_DISPLAY_DEV_McuLcdSsd1963_LEFT                0
#define HAL_DISPLAY_DEV_McuLcdSsd1963_TOP                 0
#define HAL_DISPLAY_DEV_McuLcdSsd1963_RIGHT               (HAL_DISPLAY_DEV_McuLcdSsd1963_WIDTH - 1)
#define HAL_DISPLAY_DEV_McuLcdSsd1963_BOTTOM              (HAL_DISPLAY_DEV_McuLcdSsd1963_HEIGHT - 1)
#define HAL_DISPLAY_DEV_McuLcdSsd1963_FORMAT              MPP_PIXEL_RGB565
/* only RGB565 is supported */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_BPP                 2
/* these macros represent the number of are used to define a small area of the screen in
 * order to set the LCD background.
 */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_NB_SUB_IMG_LINES    10
#define HAL_DISPLAY_DEV_McuLcdSsd1963_NB_SUB_IMG_COLUMNS  100
/******** macros for LCD controller ********/
/* pixel clock(LSHIFT clock) frequency(Hz) */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_PCLK_FREQ           30000000U
/* horizontal synchronization width(horizontal retrace) */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_HSW                 48U
/* horizontal front porch */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_HFP                 40U
/* horizontal back porch */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_HBP                 0U
/* vertical synchronization width(vertical retrace) */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_VSW                 3U
/* vertical front porch */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_VFP                 13U
/* vertical back porch */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_VBP                 18U
/* SSD1963 TFT interface timing polarity flag. */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_POLARITY_FLAG       0U
/* default panel data width */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_Panel_Data_WIDTH    kSSD1963_PanelData24Bit
/* reference clock XTAL frequency in (Hz) */
#define HAL_DISPLAY_DEV_McuLcdSsd1963_XTAL_FREQ           10000000U

#if ((HAL_DISPLAY_DEV_McuLcdSsd1963_HEIGHT % HAL_DISPLAY_DEV_McuLcdSsd1963_NB_SUB_IMG_LINES !=0) || \
(HAL_DISPLAY_DEV_McuLcdSsd1963_WIDTH % HAL_DISPLAY_DEV_McuLcdSsd1963_NB_SUB_IMG_COLUMNS !=0))
#error "HAL_DisplayDev_McuLcdSsd1963: Number of lines and columns should be a divider of respectively panel height and width."
#endif

#if (SSD1963_DATA_WITDH != 16)
#error "HAL_DisplayDev_McuLcdSsd1963: Data width between MCU and SSD1963 controller supported is 16"
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static ssd1963_handle_t lcd_handle;
static edma_handle_t edma_tx_handle;
static flexio_mculcd_edma_handle_t flexio_lcd_handle;
static SemaphoreHandle_t s_transfer_done;

/**** declarations ****/
static hal_display_status_t hal_init_flexio_mcu_lcd(void);
static hal_display_status_t hal_flush_display(const display_dev_t *dev, const uint8_t *pdata);
static hal_display_status_t hal_set_background_to_black(void);

static void board_set_cs_pin(bool set);
static void board_set_rs_pin(bool set);

static status_t hal_lcd_write_command(void *dbi_xfer_handle,
        uint32_t command);
static status_t hal_lcd_write_command_params(void *dbi_xfer_handle,
        void *data, uint32_t len_byte);
static status_t hal_lcd_write_memory(void *dbi_xfer_handle,
        uint32_t command, const void *data, uint32_t len_byte);

hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Init(display_dev_t *dev,
        mpp_display_params_t *config, mpp_callback_t callback, void *param);
hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Deinit(const display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Start(display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Stop(display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Blit(const display_dev_t *dev,
        void *frame, int width, int height);
hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Getbufdesc(const display_dev_t *dev,
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

/* set the SSD1963 default configuration. */
static ssd1963_config_t ssd1963_config = {
        .pclkFreq_Hz    = HAL_DISPLAY_DEV_McuLcdSsd1963_PCLK_FREQ,
        .polarityFlags  = HAL_DISPLAY_DEV_McuLcdSsd1963_POLARITY_FLAG,
        .panelDataWidth = HAL_DISPLAY_DEV_McuLcdSsd1963_Panel_Data_WIDTH,
#if ((SSD1963_DATA_WITDH == 16))
        .pixelInterface = kSSD1963_RGB565, /* only this format is supported. */
#else
        .pixelInterface = kSSD1963_RGB888,
#endif
        .panelWidth     = HAL_DISPLAY_DEV_McuLcdSsd1963_WIDTH,
        .panelHeight    = HAL_DISPLAY_DEV_McuLcdSsd1963_HEIGHT,
        .hsw            = HAL_DISPLAY_DEV_McuLcdSsd1963_HSW,
        .hfp            = HAL_DISPLAY_DEV_McuLcdSsd1963_HFP,
        .hbp            = HAL_DISPLAY_DEV_McuLcdSsd1963_HBP,
        .vsw            = HAL_DISPLAY_DEV_McuLcdSsd1963_VSW,
        .vfp            = HAL_DISPLAY_DEV_McuLcdSsd1963_VFP,
        .vbp            = HAL_DISPLAY_DEV_McuLcdSsd1963_VBP
};

const static display_dev_operator_t s_display_dev_mcu_lcd_ops = {
        .init        = HAL_DisplayDev_McuLcdSsd1963_Init,
        .deinit      = HAL_DisplayDev_McuLcdSsd1963_Deinit,
        .start       = HAL_DisplayDev_McuLcdSsd1963_Start,
        .stop        = HAL_DisplayDev_McuLcdSsd1963_Stop,
        .blit        = HAL_DisplayDev_McuLcdSsd1963_Blit,
        .get_buf_desc    = HAL_DisplayDev_McuLcdSsd1963_Getbufdesc,
};

static display_dev_t s_display_dev_mcu_lcd = {.id   = 0,
        .name = HAL_DISPLAY_NAME,
        .ops  = &s_display_dev_mcu_lcd_ops,
        .cap  = {.width       = HAL_DISPLAY_DEV_McuLcdSsd1963_WIDTH,
                .height      = HAL_DISPLAY_DEV_McuLcdSsd1963_HEIGHT,
                .pitch       = HAL_DISPLAY_DEV_McuLcdSsd1963_WIDTH * HAL_DISPLAY_DEV_McuLcdSsd1963_BPP,
                .left        = HAL_DISPLAY_DEV_McuLcdSsd1963_LEFT,
                .top         = HAL_DISPLAY_DEV_McuLcdSsd1963_TOP,
                .right       = HAL_DISPLAY_DEV_McuLcdSsd1963_RIGHT,
                .bottom      = HAL_DISPLAY_DEV_McuLcdSsd1963_BOTTOM,
                .rotate      = HAL_DISPLAY_DEV_McuLcdSsd1963_ROTATE,
                .format      = HAL_DISPLAY_DEV_McuLcdSsd1963_FORMAT,
                .nbFrameBuffer = HAL_DISPLAY_DEV_McuLcdSsd1963_BUFFER_COUNT,
                .frameBuffers = NULL,
                .callback    = NULL,
                .user_data   = NULL}};

static void board_set_cs_pin(bool set)
{
    GPIO_PinWrite(BOARD_SSD1963_CS_GPIO, BOARD_SSD1963_CS_PIN, (uint8_t)set);
}

static void board_set_rs_pin(bool set)
{
    GPIO_PinWrite(BOARD_SSD1963_RS_GPIO, BOARD_SSD1963_RS_PIN, (uint8_t)set);
}

static void board_flexio_transfer_callback(FLEXIO_MCULCD_Type *base,
        flexio_mculcd_edma_handle_t *handle,
        status_t status,
        void *user_data)
{
    BaseType_t task_awake = pdFALSE;
    xSemaphoreGiveFromISR(s_transfer_done, &task_awake);
    portYIELD_FROM_ISR(task_awake);
}

/* This function is used to write command to display */
static status_t hal_lcd_write_command(void *dbi_xfer_handle,
        uint32_t command)
{
    /* dbi_xfer_handle is actually flexio_lcd_handle,
     * set by SSD1963_Init 4th parameter. */
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
     * set by SSD1963_Init 4th parameter. */
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
     * set by SSD1963_Init 4th parameter. */
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
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963: Failed to transfer data to LCD\n");
        return kStatus_HAL_DisplayError;
    } else {
        /* Wait for transfer done. */
        if (xSemaphoreTake(s_transfer_done, portMAX_DELAY) != pdTRUE) {
            HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963: LCD FLEXIO transfer error\r\n");
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
    GPIO_PinInit(BOARD_SSD1963_RST_GPIO, BOARD_SSD1963_RST_PIN, &reset_pin_config);
    GPIO_PinInit(BOARD_SSD1963_CS_GPIO, BOARD_SSD1963_CS_PIN, &reset_pin_config);
    GPIO_PinInit(BOARD_SSD1963_RS_GPIO, BOARD_SSD1963_RS_PIN, &reset_pin_config);

    s_transfer_done = xSemaphoreCreateBinary();

    if (NULL == s_transfer_done) {
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963: Semaphore create failed\r\n");
        return kStatus_HAL_DisplayError;
    }

    /* FreeRTOS kernel API is used in FLEXIO ISR, so need to set proper IRQ priority. */
    NVIC_SetPriority(BOARD_FLEXIO_MCU_LCD_IRQ, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);

    FLEXIO_MCULCD_GetDefaultConfig(&flexio_mcu_lcd_config);
    flexio_mcu_lcd_config.baudRate_Bps = BOARD_FLEXIO_BAUDRATE_BPS;

    /* Configure FlexIO MCULCD */
    status = FLEXIO_MCULCD_Init(&s_display_dev_flexio_lcd, &flexio_mcu_lcd_config,
            BOARD_FLEXIO_CLOCK_FREQ);

    if (status != kStatus_Success) {
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963: Failed to initialize FlexIO MCULCD\n");
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
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963: Failed to create FlexIO_MCULCD eDMA handle\n");
        return kStatus_HAL_DisplayError;
    }

    return ret;
}

/* This function is used to transfer frame data from MCU to the display. */
static hal_display_status_t hal_flush_display(const display_dev_t *dev, const uint8_t *pdata)
{
    status_t status = kStatus_Success;

    /* get buffer size */
    int pixel_depth = get_bitpp(dev->cap.format)/8;
    uint32_t buffer_size = dev->cap.width * dev->cap.height * pixel_depth;

    /* Define frame area where MCU can access. */
    status = SSD1963_SelectArea(&lcd_handle, dev->cap.left,
            dev->cap.top, dev->cap.right, dev->cap.bottom);

    if (status != kStatus_Success)  {
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963: Failed to set display output area\n");
        return kStatus_HAL_DisplayError;
    }

    /* Transfer data from MCU to display frame memory. */
    status = SSD1963_WriteMemory(&lcd_handle, pdata, buffer_size);

    if (status != kStatus_Success)  {
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963: Failed to write data to display\n");
        return kStatus_HAL_DisplayError;
    }

    return kStatus_HAL_DisplaySuccess;
}

/*
 * this function is used to set display background to black,
 * otherwise, non-initialized data will be displayed.
 * Display mikro-e has a large frame resolution(800*480) that can not
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

    int backgr_buff_height = HAL_DISPLAY_DEV_McuLcdSsd1963_HEIGHT / HAL_DISPLAY_DEV_McuLcdSsd1963_NB_SUB_IMG_LINES;
    int backgr_buff_width = HAL_DISPLAY_DEV_McuLcdSsd1963_WIDTH / HAL_DISPLAY_DEV_McuLcdSsd1963_NB_SUB_IMG_COLUMNS;

    /* use black pixels as background */
    int backgr_buff_size = backgr_buff_height * backgr_buff_width *
            HAL_DISPLAY_DEV_McuLcdSsd1963_BPP;

    uint8_t *backgr_buff = (uint8_t *)malloc(backgr_buff_size * sizeof(uint8_t));

    if (backgr_buff == NULL) {
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963: Failed to allocate "
                "display background buffer.\n");
        return kStatus_HAL_DisplayError;
    }

    /* set frame buffer pixels to black */
    memset(backgr_buff, 0x00, backgr_buff_size * sizeof(uint8_t));

    while (top != HAL_DISPLAY_DEV_McuLcdSsd1963_HEIGHT) {
        while (left != HAL_DISPLAY_DEV_McuLcdSsd1963_WIDTH) {

            /* Define frame area where MCU can access */
            status = SSD1963_SelectArea(&lcd_handle, left,
                    top, left + backgr_buff_width - 1,
                    top + backgr_buff_height - 1);
            if (status !=  kStatus_Success) {
                free(backgr_buff);
                return kStatus_HAL_DisplayError;
            }

            status = SSD1963_WriteMemory(&lcd_handle, backgr_buff, backgr_buff_size);
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

hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Init(
        display_dev_t *dev, mpp_display_params_t *config, mpp_callback_t callback, void *user_data)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_McuLcdSsd1963_Init\n");

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

    if (pixel_depth > HAL_DISPLAY_DEV_McuLcdSsd1963_BPP) {
        HAL_LOGE("Pixel depth = %d higher than max =%d (defined in mpp_config.h.\n",
                pixel_depth, HAL_DISPLAY_DEV_McuLcdSsd1963_BPP);
        return kStatus_HAL_DisplayError;
    }

    display_cap->format = config->format;

    switch(config->format) {
        case MPP_PIXEL_RGB565:
            display_cap->pitch = display_cap->width * 2;
            break;
        default:
            HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963_Init: invalid pixel format parameter.\n");
            return kStatus_HAL_DisplayError;
    }

    /* check user resolution does not exceed the screen resolution */
    if (((config->width < 0) || (config->width > HAL_DISPLAY_DEV_McuLcdSsd1963_WIDTH))
            || ((config->height < 0) || (config->height > HAL_DISPLAY_DEV_McuLcdSsd1963_HEIGHT))
            || ((config->pitch < 0) || (config->pitch > display_cap->pitch))) {
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963_Init: invalid resolution parameter.\n");
        return kStatus_HAL_DisplayError;
    }

    /* get user's display resolution */
    if ((config->width != 0) && (config->height != 0)) {
        display_cap->width = config->width;
        display_cap->height = config->height;
        display_cap->pitch = config->width * pixel_depth;
        /* if width and height changes right and bottom should be updated accordingly */
        display_cap->right = config->width - 1;
        display_cap->bottom = config->height - 1;
    }

    /* get user's provided display position */
    if ((config->top != 0) || (config->left != 0)) {
        display_cap->top = config->top;
        display_cap->left = config->left;
    }

    /* check that display area does not exceed the screen resolution */
    if ((display_cap->bottom > HAL_DISPLAY_DEV_McuLcdSsd1963_BOTTOM)
            || (display_cap->right > HAL_DISPLAY_DEV_McuLcdSsd1963_RIGHT)
            || (((display_cap->right - display_cap->left) != display_cap->width - 1))
            || ((display_cap->bottom - display_cap->top) != display_cap->height - 1)) {
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963_Init: invalid area parameter.\n");
        return kStatus_HAL_DisplayError;
    }

    display_cap->rotate = config->rotate;
    display_cap->frameBuffers = NULL;
    display_cap->callback = callback;
    display_cap->user_data = user_data;

    HAL_LOGD("--HAL_DisplayDev_McuLcdSsd1963_Init\n");

    return ret;
}

hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Deinit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;

    EDMA_Deinit(BOARD_EDMA);

    return ret;
}

hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Start(display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    status_t status = kStatus_Success;
    HAL_LOGD("++HAL_DisplayDev_McuLcdSsd1963_Start\n");
    display_dev_private_capability_t *display_cap = &dev->cap;
    ssd1963_orientation_mode_t orient;

    /*------------------------------
     * Initialize the LCD controller
     * -----------------------------*/
    status = SSD1963_Init(&lcd_handle, &ssd1963_config, &flexio_dbi_ops,
            &flexio_lcd_handle, HAL_DISPLAY_DEV_McuLcdSsd1963_XTAL_FREQ);

    if (status != kStatus_Success) {
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963: Failed to initialize panel.\n");
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

    switch(display_cap->rotate)
    {
    case ROTATE_90:
        orient = kSSD1963_Orientation90;
        break;
    case ROTATE_180:
        orient = kSSD1963_Orientation180;
        break;
    case ROTATE_270:
        orient = kSSD1963_Orientation270;
        break;
    case ROTATE_0:
    default:
        orient = kSSD1963_Orientation0;
        break;
    }

    status = SSD1963_SetOrientationMode(&lcd_handle, orient);
    if (status != kStatus_Success) {
        HAL_LOGE("HAL_DisplayDev_McuLcdSsd1963: Failed to change orientation.\n");
        return kStatus_HAL_DisplayError;
    }

    /*-------------------------
     * Start the display
     * -----------------------*/

    SSD1963_StartDisplay(&lcd_handle);
    SSD1963_SetBackLight(&lcd_handle, 255);

    HAL_LOGD("--HAL_DisplayDev_McuLcdSsd1963_Start\n");

    return ret;
}

hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Stop(display_dev_t *dev)
{
    status_t status = kStatus_Success;

    HAL_LOGD("++HAL_DisplayDev_McuLcdSsd1963_Stop\n");

    status = SSD1963_StopDisplay(&lcd_handle);

    if (status != kStatus_Success) {
        HAL_LOGE("Failed to stop display\n");
        return kStatus_HAL_DisplayError;
    }

    SSD1963_Deinit(&lcd_handle);

    HAL_LOGD("--HAL_DisplayDev_McuLcdSsd1963_Stop\n");

    return kStatus_HAL_DisplaySuccess;
}

hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Blit(const display_dev_t *dev, void *frame,
        int width, int height)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;

    HAL_LOGD("++HAL_DisplayDev_McuLcdSsd1963_Blit\n");

    if (frame == NULL) {
        HAL_LOGE("Invalid frame buffer\n");
        return kStatus_HAL_DisplayError;
    }

    /* flush data to display */
    ret = hal_flush_display(dev, frame);

    if (ret != kStatus_HAL_DisplaySuccess) {
        HAL_LOGE("Failed to flush image to display\n");
        return kStatus_HAL_DisplayError;
    }

#if (ENABLE_PISANO_CHECKSUM == 1)
    checksum_data_t checksum;
    checksum.type = CHECKSUM_TYPE_PISANO;
    checksum.value = calc_checksum((dev->cap.width * get_bitpp(dev->cap.format)/8) * dev->cap.height, frame);
    HAL_LOGD("CHECKSUM=0x%X\n", checksum.value);

    if (dev->cap.callback != NULL)
        dev->cap.callback(NULL, MPP_EVENT_INTERNAL_TEST_RESERVED,
                (void *) &checksum, dev->cap.user_data);
#endif

    HAL_LOGD("--HAL_DisplayDev_McuLcdSsd1963_Blit\n");

    return ret;
}

hal_display_status_t HAL_DisplayDev_McuLcdSsd1963_Getbufdesc(const display_dev_t *dev, hw_buf_desc_t *in_buf,
        mpp_memory_policy_t *policy)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_McuLcdSsd1963_GetInput\n");

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

    HAL_LOGD("--HAL_DisplayDev_McuLcdSsd1963_GetInput\n");

    return ret;
}

int HAL_DisplayDev_McuLcdSsd1963_setup(display_dev_t *dev)
{
    dev->ops = &s_display_dev_mcu_lcd_ops;
    return 0;
}
#else /* !((defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_McuLcdSsd1963 == 1)) */
int HAL_DisplayDev_McuLcdSsd1963_setup(display_dev_t *dev)
{
    HAL_LOGE("Display McuLcdSsd1963 not enabled\n");
    return -1;
}
#endif /* (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_McuLcdSsd1963 == 1) */
