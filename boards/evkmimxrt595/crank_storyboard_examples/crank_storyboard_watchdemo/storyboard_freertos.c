/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "vglite_support.h"
/*-----------------------------------------------------------*/
#include "vg_lite.h"

#include "fsl_gpio.h"
#include "display_support.h"
#include "touch.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void sbengine_main_task(void *argument);
void sbengine_input_task(void *argument);

#if (CUSTOM_VGLITE_MEMORY_CONFIG != 1)
#error "Application must be compiled with CUSTOM_VGLITE_MEMORY_CONFIG=1"
#else
#define VGLITE_COMMAND_BUFFER_SZ (128 * 1024)
/* On RT595S */
#if defined(MIMXRT595S_cm33_SERIES)
#define VGLITE_HEAP_SZ 3955776 /* 3.8 MB */
/* On RT1170 */
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
#define VGLITE_HEAP_SZ 8912896 /* 8.5 MB */
#else
#error "Unsupported CPU !"
#endif

/* Allocate the heap and set the command buffer(s) size */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t vglite_heap[VGLITE_HEAP_SZ], 64);

void *vglite_heap_base        = &vglite_heap;
uint32_t vglite_heap_size     = VGLITE_HEAP_SZ;
uint32_t vglite_cmd_buff_size = VGLITE_COMMAND_BUFFER_SZ;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (DEMO_PANEL_RM67162 == DEMO_PANEL)
void GPIO_INTA_IRQHandler(void)
{
    uint32_t intStat;

    intStat = GPIO_PortGetInterruptStatus(GPIO, BOARD_MIPI_TE_PORT, kGPIO_InterruptA);

    if (intStat & (1UL << BOARD_MIPI_TE_PIN))
    {
        GPIO_PortClearInterruptFlags(GPIO, BOARD_MIPI_TE_PORT, kGPIO_InterruptA, (1UL << BOARD_MIPI_TE_PIN));
        BOARD_DisplayTEPinHandler();
    }

    intStat = GPIO_PortGetInterruptStatus(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, kGPIO_InterruptA);

    if (intStat & (1UL << BOARD_MIPI_PANEL_TOUCH_INT_PIN))
    {
        GPIO_PortClearInterruptFlags(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, kGPIO_InterruptA,
                                     (1UL << BOARD_MIPI_PANEL_TOUCH_INT_PIN));
        BOARD_TouchIntHandler();
    }
}
#endif


int main(void)
{
    status_t status;

    BOARD_InitUARTPins();
    BOARD_InitPsRamPins();

#if (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL)
    BOARD_InitFlexIOPanelPins();

    GPIO_PortInit(GPIO, BOARD_SSD1963_RST_PORT);
    GPIO_PortInit(GPIO, BOARD_SSD1963_CS_PORT);
    GPIO_PortInit(GPIO, BOARD_SSD1963_RS_PORT);
#else
    BOARD_InitMipiPanelPins();

    GPIO_PortInit(GPIO, BOARD_MIPI_POWER_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_BL_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_RST_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PORT);

#if (DEMO_PANEL_RM67162 == DEMO_PANEL)
    GPIO_PortInit(GPIO, BOARD_MIPI_TE_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT);
#endif

#endif

    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    status = BOARD_InitPsRam();
    if (status != kStatus_Success)
    {
        assert(false);
    }

#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || \
     (DEMO_PANEL_RM67162 == DEMO_PANEL))
    /* Use 48 MHz clock for the FLEXCOMM4 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM4);

    BOARD_MIPIPanelTouch_I2C_Init();
#else
    /* Use 48 MHz clock for the FLEXCOMM4 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM4);
#endif


    PRINTF("STARTING STORYBOARD\n");
    if (xTaskCreate(sbengine_main_task, "sbengine", 16384, NULL, configMAX_PRIORITIES / 2, NULL) != pdPASS)
    {
    	PRINTF("Task creation failed!.\r\n");
    	while (1);
    }

    if (xTaskCreate(sbengine_input_task, "sbengine-input", 1024, NULL, configMAX_PRIORITIES / 2, NULL) != pdPASS)
    {
    	PRINTF("Task creation failed!.\r\n");
    	while (1);
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
