/*
 * Copyright 2023-2024 NXP.
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
 * @brief display dev HAL driver implementation for Ili9341 using SPI 8-bit interface.
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

#if (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_SpiIli9341 == 1)

#include <FreeRTOS.h>
#include "semphr.h"
#include <queue.h>

#include "fsl_common.h"
#include "fsl_ili9341.h"
#include "fsl_lpspi_cmsis.h"

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
/* constant values */
#define HAL_DISPLAY_NAME                               "SpiIli9341"
#define HAL_DISPLAY_DEV_SpiIli9341_ROTATE              ROTATE_0
#define HAL_DISPLAY_DEV_SpiIli9341_BUFFER_COUNT        1
/* configurable default values */
#define HAL_DISPLAY_DEV_SpiIli9341_HEIGHT              240
#define HAL_DISPLAY_DEV_SpiIli9341_WIDTH               320
#define HAL_DISPLAY_DEV_SpiIli9341_LEFT                0
#define HAL_DISPLAY_DEV_SpiIli9341_TOP                 0
#define HAL_DISPLAY_DEV_SpiIli9341_RIGHT               (HAL_DISPLAY_DEV_SpiIli9341_WIDTH - 1)
#define HAL_DISPLAY_DEV_SpiIli9341_BOTTOM              (HAL_DISPLAY_DEV_SpiIli9341_HEIGHT - 1)
#define HAL_DISPLAY_DEV_SpiIli9341_FORMAT              MPP_PIXEL_RGB565
/* The maximum height transfered is 1/10 of the full screen height(240/10) */
#define HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT 24
#define HAL_DISPLAY_DEV_SpiIli9341_BPP                 2     /* Only supports RGB565 */
#define HAL_DISPLAY_DEV_SpiIli9341_MEM_ACCESS_CTL_DATA 0x28  /* bit fields: D7(MY):0 D6(MX):0 D5(MV):1
                                                                D4(ML):0 D3(BGR):1 D2(MH):0 D1:0 D0:0 */
/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint32_t spi_event;
static volatile bool spi_event_received;
static SemaphoreHandle_t s_transfer_done;

/**** declarations ****/
static void HAL_InitLcd();
static void HAL_FlushDisplay(const display_dev_t *dev, uint8_t *pdata);

hal_display_status_t HAL_DisplayDev_SpiIli9341_Init(
    display_dev_t *dev, mpp_display_params_t *config, mpp_callback_t callback, void *param);
hal_display_status_t HAL_DisplayDev_SpiIli9341_Deinit(const display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_SpiIli9341_Start(display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_SpiIli9341_Stop(display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_SpiIli9341_Blit(const display_dev_t *dev, void *frame,
        int stripe);
hal_display_status_t HAL_DisplayDev_SpiIli9341_Getbufdesc(const display_dev_t *dev,
        hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy);

/**** static variables ****/
const static display_dev_operator_t s_DisplayDev_SpiOps = {
    .init        = HAL_DisplayDev_SpiIli9341_Init,
    .deinit      = HAL_DisplayDev_SpiIli9341_Deinit,
    .start       = HAL_DisplayDev_SpiIli9341_Start,
    .stop        = HAL_DisplayDev_SpiIli9341_Stop,
    .blit        = HAL_DisplayDev_SpiIli9341_Blit,
    .get_buf_desc    = HAL_DisplayDev_SpiIli9341_Getbufdesc,
};

static display_dev_t s_DisplayDev_spi = {.id   = 0,
                                           .name = HAL_DISPLAY_NAME,
                                           .ops  = &s_DisplayDev_SpiOps,
                                           .cap  = {.width       = HAL_DISPLAY_DEV_SpiIli9341_WIDTH,
                                                   .height      = HAL_DISPLAY_DEV_SpiIli9341_HEIGHT,
                                                   .pitch       = HAL_DISPLAY_DEV_SpiIli9341_WIDTH * HAL_DISPLAY_DEV_SpiIli9341_BPP,
                                                   .left        = HAL_DISPLAY_DEV_SpiIli9341_LEFT,
                                                   .top         = HAL_DISPLAY_DEV_SpiIli9341_TOP,
                                                   .right       = HAL_DISPLAY_DEV_SpiIli9341_RIGHT,
                                                   .bottom      = HAL_DISPLAY_DEV_SpiIli9341_BOTTOM,
                                                   .rotate      = HAL_DISPLAY_DEV_SpiIli9341_ROTATE,
                                                   .format      = HAL_DISPLAY_DEV_SpiIli9341_FORMAT,
                                                   .nbFrameBuffer = HAL_DISPLAY_DEV_SpiIli9341_BUFFER_COUNT,
                                                   .frameBuffers = NULL,
                                                   .callback    = NULL,
                                                   .user_data   = NULL}};


uint32_t BOARD_LCD_SPI_FREQ_FUNC(void)
{
    return BOARD_LCD_SPI_FREQ;
}

static void SPI_MasterSignalEvent(uint32_t event)
{
    int task_awake = 0; /* no higher prio task to wake */
    hal_sema_give_isr(s_transfer_done, &task_awake);
    portYIELD_FROM_ISR(task_awake);
}

static void SPI_WaitEvent(void)
{
    if (hal_sema_take(s_transfer_done, HAL_MAX_TIMEOUT) != pdTRUE) {
    	HAL_LOGE("HAL_DisplayDev_SpiIli9341: LCD SPI transfer error\r\n");
    }
}

static void HAL_SPI_LCD_WriteCmd(uint8_t data)
{
    GPIO_PortClear(BOARD_LCD_DC_GPIO, 1u << BOARD_LCD_DC_GPIO_PIN);
    BOARD_LCD_SPI.Send(&data, 1);
    SPI_WaitEvent();
}

static void HAL_SPI_LCD_WriteCmdParams(uint8_t data)
{
    GPIO_PortSet(BOARD_LCD_DC_GPIO, 1u << BOARD_LCD_DC_GPIO_PIN);
    BOARD_LCD_SPI.Send(&data, 1);
    SPI_WaitEvent();
}

static void HAL_SPI_LCD_WriteMultiData(const uint8_t *pdata, int NumItems)
{
    GPIO_PortSet(BOARD_LCD_DC_GPIO, 1u << BOARD_LCD_DC_GPIO_PIN);
    BOARD_LCD_SPI.Send(pdata, NumItems);
    SPI_WaitEvent();
}

static void HAL_InitLcd()
{
    /* Init board hardware. */

    /* attach FRO 12MHz(Free Running Oscillator 12MHz internal clock) to FLEXCOMM1(SPI) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom1Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);

    /* Enable Dma clock */
    CLOCK_EnableClock(kCLOCK_Dma0);

    /* Enables the clock for GPIO0: Enables clock */
    CLOCK_EnableClock(kCLOCK_Gpio0);

    /* Define the init structure for the data/command output pin */
    gpio_pin_config_t dc_config = {
        kGPIO_DigitalOutput,
        1,
    };

    /* Init data/command GPIO output . */
    GPIO_PinInit(BOARD_LCD_DC_GPIO, BOARD_LCD_DC_GPIO_PIN, &dc_config);

    s_transfer_done = xSemaphoreCreateBinary();
    if (NULL == s_transfer_done) {
        HAL_LOGE("HAL_DisplayDev_SpiIli9341: Semaphore create failed\r\n");
        assert(0);
    }

    /* FreeRTOS kernel API is used in SPI ISR, so need to set proper IRQ priority. */
    NVIC_SetPriority(BOARD_LCD_SPI_IRQ, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);

    /* SPI master init */
    BOARD_LCD_SPI.Initialize(SPI_MasterSignalEvent);
    BOARD_LCD_SPI.PowerControl(ARM_POWER_FULL);
    BOARD_LCD_SPI.Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_DATA_BITS(8),
            BOARD_LCD_SPI_BAUDRATE);

    FT9341_Init(HAL_SPI_LCD_WriteCmdParams, HAL_SPI_LCD_WriteCmd);

    /* Change to landscape view using memory acces control command. */
    HAL_SPI_LCD_WriteCmd(ILI9341_CMD_MAC);
    HAL_SPI_LCD_WriteCmdParams(HAL_DISPLAY_DEV_SpiIli9341_MEM_ACCESS_CTL_DATA);
}

/*
 * This function is used to transfer frame data from MCU to the display.
 * The frame buffer is broken down into multiple sub-buffers(the number of
 * sub buffers is calculated based on the image frame size and the maximum
 * frame lines supported by the display for each sub-buffer which is defined by
 * HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT).
 * The sub-buffers are then sent one after the other to the display.
 */
static void HAL_FlushDisplay(const display_dev_t *dev, uint8_t *pdata)
{
    uint8_t data[4];

    /* display private capability */
    display_dev_private_capability_t display_cap;
    display_cap = dev->cap;

    /* get number of bits per pixel per format*/
    int pixel_depth = get_bitpp(display_cap.format)/8;
    uint32_t buffer_size = display_cap.width * display_cap.height * pixel_depth;

    /*
     * The display is "Big endian"(MSB first) and mpp supports only
     * "little endian"(LSB first), hence a swap byte is required to
     * display image correctly.
     */
    swap_2_bytes(pdata, buffer_size);

    /*
     * Define frame area where MCU can access by setting
     * the column left/right addresses.
     * */
    HAL_SPI_LCD_WriteCmd(ILI9341_CMD_COLADDR);
    data[0] = (display_cap.left >> 8) & 0xFF;
    data[1] = display_cap.left & 0xFF;
    data[2] = (display_cap.right >> 8) & 0xFF;
    data[3] = display_cap.right & 0xFF;
    HAL_SPI_LCD_WriteMultiData(data, 4);

    /*
     * get the number of sub buffers:
     * if (image_height / HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT)
     * is not euclidean another sub buffer should be added.
     */
    int nb_sub_buffers =  display_cap.height / HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT;
    if ((display_cap.height % HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT) != 0) {
        nb_sub_buffers += 1;
    }

    /* first sub-frame buffer top/bottom */
    int curr_buff_top = display_cap.top;
    int curr_buff_bottom = display_cap.top + HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT;

    /*
     * The sub-buffer size is calculated using the sub-frame height
     * defined by HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT
     * */
    uint32_t sub_buff_size = display_cap.width * HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT * pixel_depth;

    int remaining_buff_height = display_cap.height;

    const uint8_t *curr_sub_buff = pdata;

    for (int i = 1; i <= nb_sub_buffers; i++) {
        /*
         * Define frame area where MCU can access by updating
         * the page(line) top/bottom  addresses.
         * */
        HAL_SPI_LCD_WriteCmd(ILI9341_CMD_PAGEADDR);
        data[0] = (curr_buff_top >> 8) & 0xFF;
        data[1] = curr_buff_top & 0xFF;
        data[2] = (curr_buff_bottom >> 8) & 0xFF;
        data[3] = curr_buff_bottom & 0xFF;
        HAL_SPI_LCD_WriteMultiData(data, 4);

        /* Transfer data from MCU to display frame memory. */
        HAL_SPI_LCD_WriteCmd(ILI9341_CMD_GRAM);
        HAL_SPI_LCD_WriteMultiData(curr_sub_buff, sub_buff_size);

        /* update current sub-buffer */
        curr_sub_buff = curr_sub_buff + sub_buff_size;

        /* get the remaining buffer height */
        remaining_buff_height -= HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT;

        /* get next addr page top/bottom index */
        curr_buff_top = curr_buff_bottom;

        if (remaining_buff_height < HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT) {
            /* if the remaining buffer height is lower than the defined
             * sub-buffer height, the last sub-frame contains only the
             * remaining pixel height */
            sub_buff_size = display_cap.width * remaining_buff_height * pixel_depth;
            curr_buff_bottom += remaining_buff_height;
        } else {
            curr_buff_bottom += HAL_DISPLAY_DEV_SpiIli9341_MAX_SUB_BUFF_HEIGHT;
        }
    }
}

hal_display_status_t HAL_DisplayDev_SpiIli9341_Init(
    display_dev_t *dev, mpp_display_params_t *config, mpp_callback_t callback, void *user_data)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_SpiIli9341_Init\n");

    display_dev_private_capability_t *display_cap = NULL;
    display_cap = &dev->cap;
    /* set default config */
    memcpy(display_cap, &s_DisplayDev_spi.cap, sizeof(display_dev_private_capability_t) );

    /* check input pixel depth versus static config */
    int pixel_depth = get_bitpp(config->format)/8;
    if (pixel_depth > HAL_DISPLAY_DEV_SpiIli9341_BPP) {
        HAL_LOGE("Pixel depth = %d higher than max =%d (defined in mpp_config.h.\n",
                pixel_depth, HAL_DISPLAY_DEV_SpiIli9341_BPP);
        return kStatus_HAL_DisplayError;
    } else {
        display_cap->format = config->format;
    }

    /* check user resolution does not exceed the screen resolution */
    if ((config->width < 0) || (config->width > HAL_DISPLAY_DEV_SpiIli9341_WIDTH)) {
        HAL_LOGE("HAL_DisplayDev_SpiIli9341_Init: invalid display width: %d.\n", config->width);
        return kStatus_HAL_DisplayError;
    }

    if ((config->height < 0) || (config->height > HAL_DISPLAY_DEV_SpiIli9341_HEIGHT)) {
        HAL_LOGE("HAL_DisplayDev_SpiIli9341_Init: invalid display height: %d.\n", config->height);
        return kStatus_HAL_DisplayError;
    }

    /* get user's display resolution */
    if ((config->width != 0) && (config->height != 0)) {
        display_cap->width = config->width;
        display_cap->height = config->height;
    }

    /* get user's provided display offsets */
    if ((config->top != 0) || (config->left != 0)) {
        display_cap->top  =  config->top;
        display_cap->left = config->left;
    }

    /* right and bottom should be updated depending on width/height/top/left */
    display_cap->right = display_cap->left + display_cap->width - 1;
    display_cap->bottom = display_cap->top + display_cap->height - 1;

    /* check that display area does not exceed the screen resolution */
    if (((display_cap->right - display_cap->left) > HAL_DISPLAY_DEV_SpiIli9341_WIDTH)
            || ((display_cap->bottom - display_cap->top) > HAL_DISPLAY_DEV_SpiIli9341_HEIGHT)) {
        HAL_LOGE("HAL_DisplayDev_SpiIli9341_Init: invalid area parameter.\n");
        return kStatus_HAL_DisplayError;
    }

    display_cap->frameBuffers = NULL;
    display_cap->callback = callback;
    display_cap->user_data = user_data;

    HAL_LOGD("--HAL_DisplayDev_SpiIli9341_Init\n");
    return ret;
}

hal_display_status_t HAL_DisplayDev_SpiIli9341_Deinit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    return ret;
}

hal_display_status_t HAL_DisplayDev_SpiIli9341_Start(display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_SpiIli9341_Start\n");
    /*-------------------------
     * Initialize the display
     * -----------------------*/
    HAL_InitLcd();
    HAL_LOGD("--HAL_DisplayDev_SpiIli9341_Start\n");
    return ret;
}

hal_display_status_t HAL_DisplayDev_SpiIli9341_Stop(display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    return ret;
}

hal_display_status_t HAL_DisplayDev_SpiIli9341_Blit(const display_dev_t *dev, void *frame,
        int stripe)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_SpiIli9341_Blit\n");
    if (frame == NULL) {
        HAL_LOGE("Invalid frame buffer\n");
        return kStatus_HAL_DisplayError;
    }
    HAL_FlushDisplay(dev, frame);
    HAL_LOGD("--HAL_DisplayDev_SpiIli9341_Blit\n");
#if (ENABLE_PISANO_CHECKSUM == 1)
    checksum_data_t checksum;
    checksum.type = CHECKSUM_TYPE_PISANO;
    checksum.value = calc_checksum((dev->cap.width * get_bitpp(dev->cap.format)/8) * dev->cap.height, frame);
    HAL_LOGD("CHECKSUM=0x%X\n", checksum.value);
    if (dev->cap.callback != NULL)
    	dev->cap.callback(NULL, MPP_EVENT_INTERNAL_TEST_RESERVED,
                      (void *) &checksum, dev->cap.user_data);
#endif
    return ret;
}

hal_display_status_t HAL_DisplayDev_SpiIli9341_Getbufdesc(const display_dev_t *dev, hw_buf_desc_t *in_buf,
        mpp_memory_policy_t *policy)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_SpiIli9341_GetInput\n");

    do {
            if ((in_buf == NULL) || (policy == NULL)) {
            HAL_LOGE("\nNULL pointer to buffer descriptor\n");
            ret = kStatus_HAL_DisplayError;
            break;
        }
        /* set memory policy */
        *policy = HAL_MEM_ALLOC_NONE;
        in_buf->alignment = 0;
        in_buf->nb_lines = 0;  /* display adafruit is configurable */
        in_buf->cacheable = false;
        in_buf->stride = 0;
        in_buf->addr = NULL;
    } while (false);

    HAL_LOGD("--HAL_DisplayDev_SpiIli9341_GetInput\n");
    return ret;
}

int HAL_DisplayDev_SpiIli9341_setup(display_dev_t *dev)
{
    dev->ops = &s_DisplayDev_SpiOps;
    return 0;
}
#else /* !((defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_SpiIli9341 == 1)) */
int HAL_DisplayDev_SpiIli9341_setup(display_dev_t *dev)
{
    HAL_LOGE("Display SpiIli9341 not enabled\n");
    return -1;
}
#endif /* (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_SpiIli9341 == 1) */
