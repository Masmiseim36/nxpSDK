/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "app.h"
#include "board.h"
#include "board_init.h"
#include "erpc_server_setup.h"
#include "secure_ml_server.h"
#include "secure_ml.h"
#include "fsl_rdc.h"
#include "fsl_cache.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "mcmgr.h"
#include "lenet_mnist.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS (void *)0x20200000

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
extern unsigned char core1_image_start[];
#define CORE1_IMAGE_START core1_image_start
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern int core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif

#define APP_TASK_STACK_SIZE       (256)

/*******************************************************************************
 * Variables
 ******************************************************************************/

static TaskHandle_t app_task_handle = NULL;

GLOW_MEM_ALIGN(LENET_MNIST_MEM_ALIGN)
static const uint8_t constantWeight[LENET_MNIST_CONSTANT_MEM_SIZE] =
{
#include "lenet_mnist.weights.txt"
};

GLOW_MEM_ALIGN(LENET_MNIST_MEM_ALIGN)
static uint8_t mutableWeight[LENET_MNIST_MUTABLE_MEM_SIZE];

GLOW_MEM_ALIGN(LENET_MNIST_MEM_ALIGN)
static uint8_t activations[LENET_MNIST_ACTIVATIONS_MEM_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/
float expf(float f){
    return exp(f);
}

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)&core1_image_start;
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif

#define M7_DID 				0
#define M4_DID 				1

#define BASE_OCRAM1   		((uint8_t *) 0x20240000)
#define BASE_OCRAM2   		((uint8_t *) 0x202c0000)
#define BASE_OCRAM1_ECC   	((uint8_t *) 0x20340000)
#define BASE_OCRAM2_ECC   	((uint8_t *) 0x20350000)
#define BASE_OCRAM_M7 		((uint8_t *) 0x20380000)
#define BASE_OCRAM_M7_ECC 	((uint8_t *) 0x20360000)
#define BASE_SEMC     		((uint8_t *) 0x80000000)
#define BASE_FLASH    		((uint8_t *) 0x30000000)
#define BASE_M4_TCM_M7		((uint8_t *) 0x20200000)		// Address in M7 memory map
#define	BASE_M4_TCM_M4		((uint8_t *) 0x1ffe0000)		// Address in M4 memory map

#define TOP_OCRAM1    		(BASE_OCRAM1       + 512 * 1024) 				// 512KB OCRAM
#define TOP_OCRAM2    		(BASE_OCRAM2       + 512 * 1024) 				// 512KB OCRAM
#define TOP_OCRAM1_ECC    	(BASE_OCRAM1_ECC   +  64 * 1024) 				// 64KB OCRAM ECC
#define TOP_OCRAM2_ECC    	(BASE_OCRAM2_ECC   +  64 * 1024) 				// 64KB OCRAM ECC
#define TOP_OCRAM_M7  		(BASE_OCRAM_M7     + 512 * 1024) 				// 0..512KB OCRAM
#define TOP_OCRAM_M7_ECC  	(BASE_OCRAM_M7_ECC + 128 * 1024) 				// 0..128KB OCRAM ECC
#define TOP_SEMC      		(BASE_SEMC         + 512 * 1024 * 1024 / 8) 	// 512Mbit SDRAM
#define TOP_FLASH     		(BASE_FLASH        + 128 * 1024 * 1024 / 8 / 2) // 128Mbit Flash (bottom half).
#define	TOP_M4_TCM_M7		(BASE_M4_TCM_M7    + 256 * 1024) 				// 2 x 128KB TCM
#define TOP_M4_TCM_M4		(BASE_M4_TCM_M4    + 256 * 1024) 				// 2 x 128KB TCM

static void configureRDCMemAccess(rdc_mem_t mem, void *base, void *end, bool M4, bool M7)
{
	rdc_mem_access_config_t memConfig;
	int policy = 0;

	if (M4)
		policy |= RDC_ACCESS_POLICY(M4_DID, kRDC_ReadWrite);

	if (M7)
		policy |= RDC_ACCESS_POLICY(M7_DID, kRDC_ReadWrite);

	assert((RDC->MR[mem].MRC & RDC_MRC_LCK_MASK) == 0);

	memConfig.mem         = mem;
	memConfig.baseAddress = (uintptr_t) base;
	memConfig.endAddress  = (uintptr_t) end;
	memConfig.policy      = policy;
	memConfig.lock        = true;

	RDC_SetMemAccessConfig(RDC, &memConfig);

	assert((RDC->MR[mem].MRC & RDC_MRC_LCK_MASK) != 0);
	assert((RDC->MR[mem].MRC & RDC_MRC_ENA_MASK) != 0);
	assert(RDC->MR[mem].MRSA == (uintptr_t) base);
	assert(RDC->MR[mem].MREA == (uintptr_t) end);
}

static void configureRDCPeriphAccess(rdc_periph_t periph, bool M4, bool M7)
{
	rdc_periph_access_config_t periphConfig;
	int policy = 0;

	if (M4)
		policy |= RDC_ACCESS_POLICY(M4_DID, kRDC_ReadWrite);

	if (M7)
		policy |= RDC_ACCESS_POLICY(M7_DID, kRDC_ReadWrite);

	periphConfig.periph      = periph;
	periphConfig.policy      = policy;
	periphConfig.lock        = true;
	periphConfig.enableSema  = false;

	RDC_SetPeriphAccessConfig(RDC, &periphConfig);
}

static void configureRDC(void)
{
	rdc_domain_assignment_t masterConfig;
	rdc_mem_t mem;

	assert(mutableWeight >= BASE_OCRAM1);
	assert(mutableWeight + sizeof(mutableWeight) < TOP_OCRAM2);
	assert(activations >= BASE_OCRAM1);
	assert(activations + sizeof(activations) < TOP_OCRAM2);
	assert(constantWeight >= BASE_FLASH);
	assert(constantWeight + sizeof(constantWeight) < TOP_FLASH);

	// Configure access to FlexSPI1 (Flash): M7 only
	mem = kRDC_Mem_MRC1_0;
	configureRDCMemAccess(mem++, 0,          BASE_FLASH, true,  true);
	configureRDCMemAccess(mem++, BASE_FLASH, TOP_FLASH,  false, true);
	configureRDCMemAccess(mem++, TOP_FLASH,  0,          true,  true);
	while (mem <= kRDC_Mem_MRC1_7)
  	    configureRDCMemAccess(mem++, 0, 0, false, false);

	// Configure access to OCRAM1: M7 only
	mem = kRDC_Mem_MRC3_0;
	configureRDCMemAccess(mem++, BASE_OCRAM1,     TOP_OCRAM1,     false, true);
	configureRDCMemAccess(mem++, BASE_OCRAM1_ECC, TOP_OCRAM1_ECC, false, true);
	while (mem <= kRDC_Mem_MRC3_7)
	    configureRDCMemAccess(mem++, 0, 0, false, false);

	// Configure access to OCRAM2: M7 only
	mem = kRDC_Mem_MRC4_0;
	configureRDCMemAccess(mem++, BASE_OCRAM2,     TOP_OCRAM2,     false, true);
	configureRDCMemAccess(mem++, BASE_OCRAM2_ECC, TOP_OCRAM2_ECC, false, true);
	while (mem <= kRDC_Mem_MRC4_7)
	    configureRDCMemAccess(mem++, 0, 0, false, false);

	// Configure access to M7 OCRAM: M7 only
	mem = kRDC_Mem_MRC5_0;
	configureRDCMemAccess(mem++, BASE_OCRAM_M7,     TOP_OCRAM_M7,     false, true);
	configureRDCMemAccess(mem++, BASE_OCRAM_M7_ECC, TOP_OCRAM_M7_ECC, false, true);
	while (mem <= kRDC_Mem_MRC5_3)
	    configureRDCMemAccess(mem++, 0, 0, false, false);

	// All other masters are in the M4 domain:
    for (rdc_master_t master = kRDC_Master_ENET_1G_TX; master <= kRDC_Master_CSI; master++)
    {
        masterConfig.domainId = M4_DID;
        masterConfig.lock     = true;

    	RDC_SetMasterDomainAssignment(RDC, master, &masterConfig);
    }

    // Disable access to ECC controller of OCRAMs (to prevent fault injections)
    configureRDCPeriphAccess(kRDC_Periph_MECC1, false, false);
    configureRDCPeriphAccess(kRDC_Periph_MECC2, false, false);

    // Disable access to FlexRAM control and XRDC2 (might be exploitable)
    // I do not understand the naming of the constants for FlexRAM and XRDC2.
    configureRDCPeriphAccess(kRDC_Periph_CM7_IMXRT, false, false);
    configureRDCPeriphAccess(kRDC_Periph_SEMA1,     false, false);

    // Disable access to FlexSPI and SEMC peripherals. They have a master
    // interface that can not be controlled via RDC. See mail with
    // Melissa Hunter.
    configureRDCPeriphAccess(kRDC_Periph_FLEXSPI1,  false, true);
    configureRDCPeriphAccess(kRDC_Periph_FLEXSPI2,  false, true);
    configureRDCPeriphAccess(kRDC_Periph_SEMC,      false, true);
}

uint8_t *checkAndConvertM4toM7(uint8_t *addr)
{
	if (addr >= BASE_M4_TCM_M4 && addr < TOP_M4_TCM_M4)
       return addr - BASE_M4_TCM_M4 + BASE_M4_TCM_M7;

	if (addr >= BASE_SEMC && addr < TOP_SEMC)
	   return addr;

	return NULL;	// invalid address
}

/*!
 * @brief LeNet MNIST implemented by Glow.
 *
 * @param ifm Address of input feature map
 * @param ofm Address of output feature map
 * @result Non-zero if an error occurred
 */
int32_t inference(int32_t ifm, int32_t ofm)
{
	float *input      = (float *) GLOW_GET_ADDR(mutableWeight, LENET_MNIST_data);
	float *output     = (float *) GLOW_GET_ADDR(mutableWeight, LENET_MNIST_softmax);
	size_t inputSize  = sizeof(float[28][28]);
	size_t outputSize = sizeof(float[10]);

	ifm = (int32_t) checkAndConvertM4toM7((uint8_t *) ifm);
	ofm = (int32_t) checkAndConvertM4toM7((uint8_t *) ofm);

	// Return error if invalid addresses are given
	if (ifm == 0 || ofm == 0)
		return -1;

	// Invalidate ifm in data cache
	DCACHE_InvalidateByRange(ifm, inputSize);

	// Copy ifm to mutableWeight
	memcpy(input, (uint8_t *) ifm, inputSize);

	// Perform the inference
	lenet_mnist((uint8_t *) constantWeight, mutableWeight, activations);

	// Copy ofm from mutableWeight
	memcpy((uint8_t *) ofm, output, outputSize);

	// Flush ofm in dcache to memory
	DCACHE_CleanByRange(ofm, outputSize);

	return 0;
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
#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* Calculate size of the image */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    (void)PRINTF("Copy CORE1 image to address: 0x%x, size: %d\r\n", (void *)(char *)CORE1_BOOT_ADDRESS,
                 core1_image_size);

    /* Copy application from FLASH to RAM */
    (void)memcpy((void *)(char *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);
#endif

    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();

    /* Configure RDC to protect the ML on the M7 from malware on M4 */
    configureRDC();

    /* Boot source for Core 1 */
    (void)MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, 0U, kMCMGR_Start_Asynchronous);

    erpc_transport_t transport = erpc_transport_mu_init(MUA);

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory = erpc_mbf_dynamic_init();

    /* eRPC server side initialization */
    erpc_server_t server = erpc_server_init(transport, message_buffer_factory);

    /* adding the service to the server */
    erpc_service_t service = create_SecureMLService_service();
    erpc_add_service_to_server(server, service);

    erpc_status_t status = erpc_server_run(server);

    /* handle error status */
    if (status != (erpc_status_t)kErpcStatus_Success)
    {

        /* removing the service from the server */
        erpc_remove_service_from_server(server, service);
        destroy_SecureMLService_service();

        /* stop erpc server */
        erpc_server_stop(server);

        /* print error description */
        erpc_server_deinit(server);
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
    BOARD_InitSEMC();

    if (xTaskCreate(app_task, "APP_TASK", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1U, &app_task_handle) != pdPASS)
    {
        (void)PRINTF("\r\nFailed to create application task\r\n");
        return -1;
    }

    vTaskStartScheduler();

    return 0;
}
