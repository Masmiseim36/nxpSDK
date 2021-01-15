/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "fsl_debug_console.h"
#include "task.h"
#include "queue.h"
#include "dsp_ipc.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "counter.h"
#include "lenet.h"
#include "input100.h"
#include "output.h"

#include "dsp_support.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NN_APP_TASK_STACK_SIZE    (512)
#define CB_WORKER_TASK_STACK_SIZE (512)

#ifndef BENCH_ITERS
#define BENCH_ITERS 100
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void app_nameservice_isr_cb(unsigned int new_ept, const char *new_ept_name, unsigned long flags, void *user_data)
{
    unsigned long *data = (unsigned long *)user_data;

    *data = new_ept;
}

/*********************************** LENET ***********************************/

static TaskHandle_t nn_app_task_handle = NULL;
void nn_app_task(void *param)
{
    PRINTF("NN started\r\n");

    volatile unsigned long tic, toc;
    int i;
    unsigned long total_cycles = 0;
    float total_ms;
    const int lenet_input_size = 28 * 28 * 4;
    unsigned char output_placeholder;
    unsigned int good = 0;
    unsigned int bad  = 0;

    ml_lenet_t lenet;
    lenet_init(&lenet);

    for (i = 0; i < BENCH_ITERS; i++)
    {
        lenet_feed_input(&lenet, (void *)(input100 + lenet_input_size * i));

        KIN1_ResetCycleCounter();
        tic = get_ccount();
        lenet_process(&lenet, &output_placeholder);
        toc = get_ccount();
        total_cycles += toc - tic;
        if (output_placeholder == output[i])
        {
            good++;
        }
        else
        {
            bad++;
        }
    }

    total_cycles /= BENCH_ITERS;
    total_ms = COUNT_TO_USEC(total_cycles, SystemCoreClock) / 1000.0;

    PRINTF("GOOD:\t%d\r\n", good);
    PRINTF("BAD:\t%d\r\n", bad);
    PRINTF("ACC:\t%f\r\n", ((float)(good)) / (good + bad));
    PRINTF("Avg Inference cycles: %u time: %.3f ms\r\n", total_cycles, total_ms);
    PRINTF("Throughput: %.1f fps\r\n", 1000 / total_ms);

    lenet_deinit(&lenet);

    while (1)
        ;
}

static TaskHandle_t cb_worker_task_handle = NULL;

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Clear MUA reset before run DSP core */
    RESET_PeripheralReset(kMU_RST_SHIFT_RSTn);

    PRINTF("\r\nStarted LeNet demo and benchmark (%d iterations)\r\n", BENCH_ITERS);

    /* Start dsp firmware */
    BOARD_DSP_IPC_Init();
    BOARD_DSP_IPCAsync_Init(15);
    BOARD_DSP_Init();

#if DSP_IMAGE_COPY_TO_RAM
    PRINTF("DSP Image copied to SRAM\r\n");
#else
    PRINTF("DSP Image not copied. The image must be copied using the debugger\r\n");
#endif // DSP_IMAGE_COPY_TO_RAM

    if (xTaskCreate(nn_app_task, "NN_APP_TASK", NN_APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2,
                    &nn_app_task_handle) != pdPASS)
    {
        PRINTF("\r\nFailed to create nn application task\r\n");
        while (1)
            ;
    }

    if (xTaskCreate(dsp_ipc_queue_worker_task, "CB_WORKER_TASK", CB_WORKER_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1,
                    &cb_worker_task_handle) != pdPASS)
    {
        PRINTF("\r\nFailed to create callback worker task\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();

    PRINTF("Failed to start FreeRTOS on core0.\r\n");
    while (1)
        ;
}
