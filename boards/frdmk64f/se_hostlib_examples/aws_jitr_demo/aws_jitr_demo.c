/* Copyright 2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 * */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "ksdk_mbedtls.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "tst_utils_kinetis.h"
#include "nxLog_App.h"

#include "ax_api.h"
#include "aws_iot_config.h"
#ifdef IMX_RT
#include "pin_mux.h"
#include "fsl_trng.h"
#include "fsl_dcp.h"
#include "sm_printf.h"
#include "fsl_iomuxc.h"


#endif

#ifdef CPU_LPC54018JET180
#   include "fsl_power.h"
#endif
/*******************************************************************
* MACROS
*******************************************************************/
#ifdef CPU_MIMXRT1052DVL6B
#define TRNG0 TRNG
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define AWS_TASK_PRIORITY (tskIDLE_PRIORITY)
#define AWS_TASK_STACK_SIZE 9000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void awsPubSub_task(void *);


/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined (IMX_RT)
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {true, false, 1};
    CLOCK_InitEnetPll(&config);
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}
#endif
/*!
 * @brief Main function
 */
int main(void)
{
#if defined(IMX_RT)
    dcp_config_t dcpConfig;
    trng_config_t trngConfig;

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    delay();
    GPIO_WritePinOutput(GPIO1, 9, 1);

    SCB_DisableDCache();
    CRYPTO_InitHardware();


    /* Initialize DCP */
    DCP_GetDefaultConfig(&dcpConfig);
    DCP_Init(DCP, &dcpConfig);

    /* Initialize TRNG */
    TRNG_GetDefaultConfig(&trngConfig);
    /* Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.
    * It is optional.*/
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;

    /* Initialize TRNG */
    TRNG_Init(TRNG0, &trngConfig);
#elif defined(CPU_LPC54018JET180)

   MPU_Type *base = MPU;

   CLOCK_EnableClock(kCLOCK_InputMux);
   /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
   CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

   /* Enable Clock for RIT */
   CLOCK_EnableClock(kCLOCK_Gpio3);

   /* attach 12 MHz clock to FLEXCOMM2 (I2C master) */
   CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

   /* reset FLEXCOMM for I2C */
   RESET_PeripheralReset(kFC2_RST_SHIFT_RSTn);

#if defined(FSL_FEATURE_SOC_SHA_COUNT) && (FSL_FEATURE_SOC_SHA_COUNT > 0)
    CLOCK_EnableClock(kCLOCK_Sha0);
    RESET_PeripheralReset(kSHA_RST_SHIFT_RSTn);
#endif

   BOARD_InitBootPins();
   BOARD_BootClockFROHF96M();
   BOARD_InitDebugConsole();
   /* Disable MPU. */
   base->CTRL &= ~(0x1U);
#else
    SYSMPU_Type *base = SYSMPU;
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    base->CESR &= ~SYSMPU_CESR_VLD_MASK;
#endif

    /*Initialize the Tri-color LED */
    LED_GREEN_INIT(1);
    LED_RED_INIT(1);
    LED_BLUE_INIT(1);

    CRYPTO_InitHardware();

    if (xTaskCreate(&awsPubSub_task, "awsPubSub", AWS_TASK_STACK_SIZE, NULL, AWS_TASK_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    for (;;)
        ;
}
