/*
 * Copyright 2018-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main_cm33.h"
#include "fsl_debug_console.h"
#include "fsl_sema42.h"


#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "dsp_support.h"
#include "dsp_ipc.h"
#include "cmd.h"
#include "fsl_cs42448.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_power.h"
#include "fsl_pca9420.h"
#include "dsp_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_CODEC_VOLUME 100U
#define APP_SEMA42        SEMA42
#define SEMA_PRINTF_NUM	  0
#define SEMA_STARTUP_NUM  1
#define SEMA_CORE_ID_CM33 1
#define APP_TASK_STACK_SIZE (6 * 1024)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
int BOARD_CODEC_Init(void);
int BOARD_CODEC_Init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
codec_handle_t g_codecHandle;
cs42448_config_t g_cs42448Config = {
    .DACMode   = kCS42448_ModeSlave,
    .ADCMode   = kCS42448_ModeSlave,
    .reset     = NULL,
    .master    = false,
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
#ifdef XA_VIT_PRE_PROC
    .format = {.sampleRate = 16000U, .bitWidth = 16U},
    .bus    = kCS42448_BusI2S,
#else
    .format = {.sampleRate = 16000U, .bitWidth = BOARD_DMIC_NUM > 2 ? 24U : 16U},
    .bus    = BOARD_DMIC_NUM > 2 ? kCS42448_BusTDM : kCS42448_BusI2S,
#endif
    .slaveAddress = CS42448_I2C_ADDR,
};
codec_config_t g_boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &g_cs42448Config};
static app_handle_t app;
/*******************************************************************************
 * Code
 ******************************************************************************/

int BOARD_CODEC_Init(void)
{
    PRINTF("[CM33 Main] Configure cs42448 codec\r\n");

    if (CODEC_Init(&g_codecHandle, &g_boardCodecConfig) != kStatus_Success)
    {
        PRINTF("[CM33 Main] cs42448_Init failed!\r\n");
        return -1;
    }

    if (CODEC_SetVolume(&g_codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        return -1;
    }

    return 0;
}

void BOARD_MuteRightChannel(bool mute)
{
    /* The CODEC_SetMute() funtion sets the volume to 100 after unmuting */
    CODEC_SetVolume(&g_codecHandle, kCODEC_PlayChannelHeadphoneRight, mute ? 0 : DEMO_CODEC_VOLUME);
}

void CM33_PRINTF(const char* ptr, ...)
{
    va_list ap;
    SEMA42_Lock(APP_SEMA42, SEMA_PRINTF_NUM, SEMA_CORE_ID_CM33);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(APP_SEMA42, SEMA_PRINTF_NUM);
}

void handleShellMessage(srtm_message *msg, void *arg)
{
    /* Send message to the DSP */
    dsp_ipc_send_sync(msg);

    /* Wait for response message to be processed before returning to shell. */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

void APP_Shell_Task(void *param)
{
    PRINTF("[CM33 Main][APP_Shell_Task] start\r\n");

    /* Handle shell commands.  Return when 'exit' command entered. */
    shellCmd(handleShellMessage, param);

    PRINTF("\r\n[CM33 Main][APP_Shell_Task] audio demo end\r\n");
    while (1)
        ;
}

void APP_DSP_IPC_Task(void *param)
{
    srtm_message msg;
    app_handle_t *app = (app_handle_t *)param;

    PRINTF("[CM33 Main][APP_DSP_IPC_Task] start\r\n");

    while (1)
    {
        /* Block for IPC message from DSP */
        dsp_ipc_recv_sync(&msg);
        /* Process message */
        handleDSPMessage(app, &msg);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    int ret;

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_InputMux);
    /* Clear SEMA42 reset */
    RESET_PeripheralReset(kSEMA_RST_SHIFT_RSTn);

    /* Clear MUA reset before run DSP core */
    RESET_PeripheralReset(kMU_RST_SHIFT_RSTn);

    /* I2C */
    CLOCK_AttachClk(kFFRO_to_FLEXCOMM2);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 6);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    g_cs42448Config.i2cConfig.codecI2CSourceClock = CLOCK_GetFlexCommClkFreq(2);
    g_cs42448Config.format.mclk_HZ                = CLOCK_GetMclkClkFreq();

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);
    /* Reset the sema42 gate */
    SEMA42_ResetAllGates(APP_SEMA42);


    PRINTF("\r\n");
    PRINTF("******************************\r\n");
    PRINTF("DSP audio framework demo start\r\n");
    PRINTF("******************************\r\n");
    PRINTF("\r\n");

    ret = BOARD_CODEC_Init();
    if (ret)
    {
        PRINTF("[CM33 Main] CODEC_Init failed!\r\n");
        return -1;
    }

    /* Initialize RPMsg IPC interface between ARM and DSP cores. */
    BOARD_DSP_IPC_Init();

    /* Copy DSP image to RAM and start DSP core. */
    BOARD_DSP_Init();

    /* Wait for the DSP to lock the semaphore */
    while (kSEMA42_LockedByProc3 != SEMA42_GetGateStatus(APP_SEMA42, SEMA_STARTUP_NUM))
    {
    }

    /* Wait for the DSP to unlock the semaphore 1 */
    while (SEMA42_GetGateStatus(APP_SEMA42, SEMA_STARTUP_NUM))
    {
    }

#if DSP_IMAGE_COPY_TO_RAM
    PRINTF("[CM33 Main] DSP image copied to DSP TCM\r\n");
#endif

    /* Set IPC processing task priority = 2 */
    if (xTaskCreate(APP_DSP_IPC_Task, "DSP Msg Task", APP_TASK_STACK_SIZE, &app, tskIDLE_PRIORITY + 2,
                    &app.ipc_task_handle) != pdPASS)
    {
        PRINTF("\r\n[CM33 Main] Failed to create application task\r\n");
        while (1)
            ;
    }

    /* Set shell command task priority = 1 */
    if (xTaskCreate(APP_Shell_Task, "Shell Task", APP_TASK_STACK_SIZE, &app, tskIDLE_PRIORITY + 1,
                    &app.shell_task_handle) != pdPASS)
    {
        PRINTF("\r\n[CM33 Main] Failed to create application task\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();

    /* Shoud not reach this statement. */
    return 0;
}
