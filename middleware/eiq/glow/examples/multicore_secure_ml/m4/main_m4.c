/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "board_init.h"
#include "erpc_client_setup.h"
#include "secure_ml.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mcmgr.h"
#include "assert.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define APP_TASK_STACK_SIZE       (256U)

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

TaskHandle_t app_task_handle = NULL;

// ---------------------------- Application -----------------------------

#define LENET_MNIST_INPUT_SIZE    	(28 * 28 * sizeof(float))
#define LENET_MNIST_OUTPUT_CLASS  	(10)

static const uint8_t nineImageData[LENET_MNIST_INPUT_SIZE] __attribute__((aligned(64))) =
{
#include "input_image.inc"
};

static float classification[LENET_MNIST_OUTPUT_CLASS] __attribute__((aligned(64)));

static void tryToAccess(int32_t addr)
{
    (void) *(volatile int32_t *) addr;
}

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}

static void app_task(void *param)
{
    static uint8_t imageData[LENET_MNIST_INPUT_SIZE];

    erpc_transport_t transport = erpc_transport_mu_init(MUB);

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory = erpc_mbf_dynamic_init();

    /* eRPC client side initialization */
    erpc_client_t client = erpc_client_init(transport, message_buffer_factory);


    memcpy(imageData, nineImageData, sizeof(nineImageData));

    for (int iteration = 0;; iteration++)
    {
    	if (iteration > 1000)
    	{
    		/* Tests to see whether M7 owned memories are properly protected by RDC: */
            if (0) tryToAccess(0x20240000);  /* OCRAM1 */
            if (0) tryToAccess(0x202c0000);  /* OCRAM2 */
            if (0) tryToAccess(0x20340000);  /* OCRAM1 ECC */
            if (0) tryToAccess(0x20350000);  /* OCRAM2 ECC */
            if (0) tryToAccess(0x20380000);  /* M7 OCRAM */
            if (0) tryToAccess(0x20360000);  /* M7 OCRAM ECC */
            if (0) tryToAccess(0x30000000);  /* Flash */
    	}

    	/* Rotate input image by 180 degrees (9 becomes 6 or 6 becomes 9) */
    	for (int i1 = 0; i1 < 28; i1++)
    	    for (int j1 = 0; j1 < i1; j1++)
    	    {
    	        int n = sizeof(float);
    	        int i2 = 27 - i1;
    	        int j2 = 27 - j1;

    	        for (int k = 0; k < n; k++)
    	        {
    	        	uint8_t tmp1 = imageData[(i1 * 28 + j1) * n + k];
    	        	uint8_t tmp2 = imageData[(i2 * 28 + j2) * n + k];
    	        	imageData[(i2 * 28 + j2) * n + k] = tmp1;
    	        	imageData[(i1 * 28 + j1) * n + k] = tmp2;
    	        }
    	    }

    	/* Flush dcache for input to memory */
    	DCACHE_CleanByRange((int32_t) imageData, sizeof(imageData));

    	/* Call inference om M7. Input and outout are passed by reference. */
        int32_t result = inference((int32_t) imageData, (int32_t) classification);

        /* Exit loop on failure */
        if (result != 0)
        	break;

        /* Invalidate dcache for output */
        DCACHE_InvalidateByRange((int32_t) classification, sizeof(classification));

        /* Compute top-1 classification */
        int class = 0;
        for (int i = 1; i < LENET_MNIST_OUTPUT_CLASS; i++)
        {
        	if (classification[i] > classification[class])
        		class = i;
        }

        if (iteration & 1)
        	assert(class == 9);  /* odd iterations should recognize '9'. */
        else
        	assert(class == 6);  /* even iterations should recognize '6'. */

    	PRINTF("eiq multicore secure ml %d (%d%%) (%s)\r\n", class, (int) (100 * classification[class]), __TIME__);
    }

    for (;;)
        ;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_Init();

    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();

    if (xTaskCreate(app_task, "APP_TASK", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1U, &app_task_handle) != pdPASS)
    {
        /* Failed to create application task */
        return -1;
    }

    vTaskStartScheduler();

    return 0;
}
