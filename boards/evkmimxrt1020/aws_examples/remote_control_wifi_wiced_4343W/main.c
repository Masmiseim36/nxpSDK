/*
 * Amazon FreeRTOS V1.0.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
/* SDK Included Files */
#include "board.h"
#include "fsl_debug_console.h"
#include "ksdk_mbedtls.h"

#include "pin_mux.h"

/* Amazon FreeRTOS Demo Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "aws_logging_task.h"
#include "aws_system_init.h"
#include "aws_dev_mode_key_provisioning.h"

/* Board specific accelerometer driver include */
#if defined(BOARD_ACCEL_FXOS)
#include "fsl_fxos.h"
#elif defined(BOARD_ACCEL_MMA)
#include "fsl_mma.h"
#endif

#include "aws_clientcredential.h"
#include "aws_wifi.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define INIT_SUCCESS 0
#define INIT_FAIL 1

/* LPI2C */
#define BOARD_ACCEL_I2C_BASEADDR LPI2C4
/* Select USB1 PLL (480 MHz) as LPI2C's clock source */
#define BOARD_ACCEL_I2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for LPI2C clock source */
#define BOARD_ACCEL_I2C_CLOCK_SOURCE_DIVIDER (5U)
#define BOARD_ACCEL_I2C_CLOCK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8 / (BOARD_ACCEL_I2C_CLOCK_SOURCE_DIVIDER + 1U))
#define LOGGING_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define LOGGING_TASK_STACK_SIZE (200)
#define LOGGING_QUEUE_LENGTH (16)

/* Accelerometer driver specific defines */
#if defined(BOARD_ACCEL_FXOS)
#define XYZ_DATA_CFG XYZ_DATA_CFG_REG
#define ACCEL_INIT(handle, config) FXOS_Init(handle, config)
#define ACCEL_READ_REG(handle, reg, val) FXOS_ReadReg(handle, reg, val, 1)
#define ACCELL_READ_SENSOR_DATA(handle, data) FXOS_ReadSensorData(handle, data)
#define ACCEL_GET_RESOLUTION() FXOS_GetResolutionBits()
#elif defined(BOARD_ACCEL_MMA)
#define XYZ_DATA_CFG kMMA8652_XYZ_DATA_CFG
#define ACCEL_INIT(handle, config) MMA_Init(handle, config)
#define ACCEL_READ_REG(handle, reg, val) MMA_ReadReg(handle, reg, val)
#define ACCELL_READ_SENSOR_DATA(handle, data) MMA_ReadSensorData(handle, data)
#define ACCEL_GET_RESOLUTION() MMA_GetResolutionBits()
#endif

/* Accelerometer and magnetometer */
#if defined(BOARD_ACCEL_FXOS)
fxos_handle_t accelHandle           = {0};
static const uint8_t accelAddress[] = {0x1CU, 0x1EU, 0x1DU, 0x1FU};
fxos_config_t config                = {0};
#elif defined(BOARD_ACCEL_MMA)
mma_handle_t accelHandle            = {0};
static const uint8_t accelAddress[] = {0x1CU, 0x1DU, 0x1EU, 0x1FU};
mma_config_t config                 = {0};
#endif

/* Accelerometer data scale */
uint8_t g_accelDataScale = 0;
/* Resolution of accelerometer (14 bit or 12 bit) */
uint8_t g_accelResolution = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitLEDs(void);
extern void vStartLedDemoTask(void);
extern int initNetwork(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const WIFINetworkParams_t pxNetworkParams = {
    .pcSSID           = clientcredentialWIFI_SSID,
    .ucSSIDLength     = sizeof(clientcredentialWIFI_SSID) - 1,
    .pcPassword       = clientcredentialWIFI_PASSWORD,
    .ucPasswordLength = sizeof(clientcredentialWIFI_PASSWORD) - 1,
    .xSecurity        = clientcredentialWIFI_SECURITY,
};
/* Count of LED which can be controlled */
uint8_t ledCount = 1;
/* Array of LED names */
char *ledName[] = {"LED"};
/* Array of LED colors in JSON */
char ledColors[] = "[\"green\"]";

/*******************************************************************************
 * Code
 ******************************************************************************/
int initNetwork(void)
{
    WIFIReturnCode_t result;

    configPRINTF(("Starting WiFi...\r\n"));

    result = WIFI_On();
    if (result != eWiFiSuccess)
    {
        configPRINTF(("Could not enable WiFi, reason %d.\r\n", result));
        return INIT_FAIL;
    }

    configPRINTF(("WiFi module initialized.\r\n"));

    result = WIFI_ConnectAP(&pxNetworkParams);
    if (result != eWiFiSuccess)
    {
        configPRINTF(("Could not connect to WiFi, reason %d.\r\n", result));
        return INIT_FAIL;
    }

    configPRINTF(("WiFi connected to AP %s.\r\n", pxNetworkParams.pcSSID));

    uint8_t tmp_ip[4] = {0};
    result            = WIFI_GetIP(tmp_ip);

    if (result != eWiFiSuccess)
    {
        configPRINTF(("Could not get IP address, reason %d.\r\n", result));
        return INIT_FAIL;
    }

    configPRINTF(("IP Address acquired %d.%d.%d.%d\r\n", tmp_ip[0], tmp_ip[1], tmp_ip[2], tmp_ip[3]));

    return INIT_SUCCESS;
}
void turnOnLed(uint8_t id)
{
    if (id == 0)
    {
        USER_LED_ON();
    }
}

void turnOffLed(uint8_t id)
{
    if (id == 0)
    {
        USER_LED_OFF();
    }
}

/*!
 * @brief Initialize pin for control LED
 */
void BOARD_InitLEDs()
{
    USER_LED_INIT(LOGIC_LED_OFF);
}

/* MPU configuration. */
void BOARD_ConfigMPU_app(void)
{
    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
     * SubRegionDisable, Size)
     * API in mpu_armv7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
     * disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
     * Privileged mode.
     *      Use MACROS defined in mpu_armv7.h:
     * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     * through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write
     * back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write
     * through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write
     * back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner
     * noncache
     *     1             1           0           0              Normal             shareable       outer and inner
     * noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write
     * back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write
     * back write/read acllocate
     *     2             x           0           0              Device              not shareable
     *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache
     * policy.
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
     * mpu_armv7.h.
     */

    /* Region 0 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0xC0000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 1 setting: Memory with Device type, not shareable,  non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

/* Region 2 setting */
#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Setting Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_8MB);
#else
    /* Setting Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_8MB);
#endif

    /* Region 3 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_64KB);

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32KB);

/* The define sets the cacheable memory to shareable,
 * this suggestion is referred from chapter 2.2.1 Memory regions,
 * types and attributes in Cortex-M7 Devices, Generic User Guide */
#if defined(SDRAM_IS_SHAREABLE)
    /* Region 7 setting: Memory with Normal type, shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 1, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
#else
    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
#endif

    /* Region 8 setting, set last 2MB of SDRAM can't be accessed by cache, glocal variables which are not expected to be
     * accessed by cache can be put here */
    /* Memory with Normal type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x81E00000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* RAM banks partitioning: 32KB OCRAM and 224KB DTCM */
    IOMUXC_GPR->GPR17 = 0xAAA9;
    IOMUXC_GPR->GPR14 &= ~(IOMUXC_GPR_GPR14_CM7_CFGDTCMSZ_MASK | IOMUXC_GPR_GPR14_CM7_CFGITCMSZ_MASK);
    IOMUXC_GPR->GPR14 |= 0x9 << IOMUXC_GPR_GPR14_CM7_CFGDTCMSZ_SHIFT;
    IOMUXC_GPR->GPR16 &= ~IOMUXC_GPR_GPR16_INIT_ITCM_EN_MASK;
    IOMUXC_GPR->GPR16 |= IOMUXC_GPR_GPR16_INIT_DTCM_EN_MASK;
    IOMUXC_GPR->GPR16 |= 1 << IOMUXC_GPR_GPR16_FLEXRAM_BANK_CFG_SEL_SHIFT;
}

static void BOARD_USDHCClockConfiguration(void)
{
    CLOCK_InitSysPll(&sysPllConfig_BOARD_BootClockRUN);
    /*configure system pll PFD0 fractional divider to 24, output clock is 528MHZ * 18 / 24 = 396 MHZ*/
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 24U);
    /* Configure USDHC clock source and divider */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 1U);
}

void print_string(const char *string)
{
    PRINTF(string);
}

#if defined(BOARD_ACCEL_FXOS) || defined(BOARD_ACCEL_MMA)
/*!
 * @brief Initialize accelerometer sensor
 */
status_t init_mag_accel(uint8_t *accelDataScale, uint8_t *accelResolution)
{
    uint8_t arrayAddrSize = 0;
    uint8_t sensorRange   = 0;
    uint16_t i            = 0;
    status_t result       = kStatus_Fail;

    /* Configure the I2C function */
    config.I2C_SendFunc    = BOARD_Accel_I2C_Send;
    config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;

    /* Initialize sensor devices */
    arrayAddrSize = sizeof(accelAddress) / sizeof(accelAddress[0]);
    for (i = 0; i < arrayAddrSize; i++)
    {
        config.slaveAddress = accelAddress[i];
        /* Initialize accelerometer sensor */
        result = ACCEL_INIT(&accelHandle, &config);
        if (result == kStatus_Success)
        {
            break;
        }
    }

    if (result != kStatus_Success)
    {
        PRINTF("\r\nSensor device initialize failed!\r\n");
        PRINTF("\r\nPlease check the sensor chip\r\n");
        return result;
    }

    *accelResolution = ACCEL_GET_RESOLUTION();

    /* Get sensor range */
    if (kStatus_Success != ACCEL_READ_REG(&accelHandle, XYZ_DATA_CFG, &sensorRange))
    {
        return kStatus_Fail;
    }
    if (sensorRange == 0x00)
    {
        *accelDataScale = 2U;
    }
    else if (sensorRange == 0x01)
    {
        *accelDataScale = 4U;
    }
    else if (sensorRange == 0x10)
    {
        *accelDataScale = 8U;
    }

    return kStatus_Success;
}
#endif

void vApplicationDaemonTaskStartupHook(void)
{
    /* A simple example to demonstrate key and certificate provisioning in
     * microcontroller flash using PKCS#11 interface. This should be replaced
     * by production ready key provisioning mechanism. */
    vDevModeKeyProvisioning();

    if (SYSTEM_Init() == pdPASS)
    {
        if (initNetwork() != 0)
        {
            configPRINTF(("Network init failed, stopping demo.\r\n"));
            vTaskDelete(NULL);
        }
        else
        {
            vStartLedDemoTask();
        }
    }
}

int main(void)
{
    BOARD_ConfigMPU_app();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_USDHCClockConfiguration();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();

    BOARD_InitLEDs();

    /* Clock setting for LPI2C */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, BOARD_ACCEL_I2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, BOARD_ACCEL_I2C_CLOCK_SOURCE_DIVIDER);

    SCB_DisableDCache();
    CRYPTO_InitHardware();

#if defined(BOARD_ACCEL_FXOS) || defined(BOARD_ACCEL_MMA)
    /* Initialize I2C */
    BOARD_Accel_I2C_Init();

    /* Initialize magnetometer and accelerometer */
    if (kStatus_Success != init_mag_accel(&g_accelDataScale, &g_accelResolution))
    {
        /* Failed to initialize accelerometer */
        for (;;)
            ;
    }
#endif

    xLoggingTaskInitialize(LOGGING_TASK_STACK_SIZE, LOGGING_TASK_PRIORITY, LOGGING_QUEUE_LENGTH);

    vTaskStartScheduler();
    for (;;)
        ;
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle
     * task's state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
/**
 * @brief Warn user if pvPortMalloc fails.
 *
 * Called if a call to pvPortMalloc() fails because there is insufficient
 * free memory available in the FreeRTOS heap.  pvPortMalloc() is called
 * internally by FreeRTOS API functions that create tasks, queues, software
 * timers, and semaphores.  The size of the FreeRTOS heap is set by the
 * configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h.
 *
 */
void vApplicationMallocFailedHook()
{
    configPRINTF(("ERROR: Malloc failed to allocate memory\r\n"));
}

/**
 * @brief Loop forever if stack overflow is detected.
 *
 * If configCHECK_FOR_STACK_OVERFLOW is set to 1,
 * this hook provides a location for applications to
 * define a response to a stack overflow.
 *
 * Use this hook to help identify that a stack overflow
 * has occurred.
 *
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    portDISABLE_INTERRUPTS();

    /* Loop forever */
    for (;;)
        ;
}

void *pvPortCalloc(size_t xNum, size_t xSize)
{
    void *pvReturn;

    pvReturn = pvPortMalloc(xNum * xSize);
    if (pvReturn != NULL)
    {
        memset(pvReturn, 0x00, xNum * xSize);
    }

    return pvReturn;
}
