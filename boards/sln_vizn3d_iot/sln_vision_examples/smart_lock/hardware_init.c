/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief  RT117F hardware configuration and initialization.
 */

#include "board_define.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "sln_time.h"
#include "task.h"
#include "fsl_lpi2c_freertos.h"
#include "fsl_lpspi_freertos.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_clock.h"

#include "fwk_log.h"

uint32_t LPI2C_GetInstance(LPI2C_Type *base);

static IRQn_Type const lpi2c_irqs[] = LPI2C_IRQS;
static lpi2c_rtos_handle_t master_rtos_handle[sizeof(lpi2c_irqs) / sizeof(lpi2c_irqs[0])];

/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM) / (2^POST)
 *                              = 24 * (32 + 77/100)  / 2
 *                              = 393.24MHZ
 */
static const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 0, 1, 2, 3, 4, 5 */
    .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};

#define WEAK __attribute__((weak))
WEAK void INPUT_DEV_PUSH_BUTTONS_IRQHandler(GPIO_Type *base, uint32_t intPin)
{
}
/*----------------------------------------------------------------------------------------
 * Use sub #define to identify vizn3 model selection and complete all pin configure here
 * --------------------------------------------------------------------------------------*/

void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    status_t status;
    uint32_t instance = LPI2C_GetInstance(base);
    lpi2c_master_config_t masterConfig;
    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);

#if defined(SDK_I2C_FREERTOS) && SDK_I2C_FREERTOS
    status = LPI2C_RTOS_Init(&master_rtos_handle[instance], base, &masterConfig, clkSrc_Hz);
#else
    status = LPI2C_MasterInit(base, &masterConfig, clkSrc_Hz);
#endif
    NVIC_SetPriority(lpi2c_irqs[instance], configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);

    if (status != kStatus_Success)
    {
        LOGE("LPI2C %d master: Error initializing LPI2C!", instance);
    }
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize)
{
    status_t status;
    uint32_t instance = LPI2C_GetInstance(base);
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

#if defined(SDK_I2C_FREERTOS) && SDK_I2C_FREERTOS
    status = LPI2C_RTOS_Transfer(&master_rtos_handle[instance], &xfer);
#else
    status = LPI2C_MasterTransferBlocking(base, &xfer);
#endif

    return status;
}

status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subAddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize)
{
    status_t status;
    uint32_t instance = LPI2C_GetInstance(base);
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;

#if defined(SDK_I2C_FREERTOS) && SDK_I2C_FREERTOS
    status = LPI2C_RTOS_Transfer(&master_rtos_handle[instance], &xfer);
#else
    status = LPI2C_MasterTransferBlocking(base, &xfer);
#endif

    return status;
}

#if defined(SDK_SW_SPI) && SDK_SW_SPI
#define CS_L  (BOARD_ELCDIF_DISPLAY_SPI_CS_GPIO->DR &= ~(1 << BOARD_ELCDIF_DISPLAY_SPI_CS_PIN))
#define CS_H  (BOARD_ELCDIF_DISPLAY_SPI_CS_GPIO->DR |= 1 << BOARD_ELCDIF_DISPLAY_SPI_CS_PIN)
#define SCL_L (BOARD_ELCDIF_DISPLAY_SPI_SCL_GPIO->DR &= ~(1 << BOARD_ELCDIF_DISPLAY_SPI_SCL_PIN))
#define SCL_H (BOARD_ELCDIF_DISPLAY_SPI_SCL_GPIO->DR |= 1 << BOARD_ELCDIF_DISPLAY_SPI_SCL_PIN)
#define SDA_L (BOARD_ELCDIF_DISPLAY_SPI_SDA_GPIO->DR &= ~(1 << BOARD_ELCDIF_DISPLAY_SPI_SDA_PIN))
#define SDA_H (BOARD_ELCDIF_DISPLAY_SPI_SDA_GPIO->DR |= 1 << BOARD_ELCDIF_DISPLAY_SPI_SDA_PIN)
#define SPI_SW_Delay(x)
// SDK_DelayAtLeastUs(x, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY)//vTaskDelay(x)

__attribute__((section(".ramfunc.$SRAM_ITC_cm7"))) void board_elcdif_rk024hh298_spi_send_cmd(uint8_t cmd)
{
    uint8_t n, m;

    __disable_irq();

    m = 0x80;

    CS_L;
    SPI_SW_Delay(1);
    SCL_L;
    SDA_L;
    SPI_SW_Delay(1);
    SCL_H;
    SPI_SW_Delay(2);
    for (n = 0; n < 8; n++)
    {
        if (cmd & m)
        {
            SCL_L;
            SPI_SW_Delay(1);
            SDA_H;
            SPI_SW_Delay(1);
            SCL_H;
            SPI_SW_Delay(1);
        }
        else
        {
            SCL_L;
            SPI_SW_Delay(1);
            SDA_L;
            SPI_SW_Delay(1);
            SCL_H;
            SPI_SW_Delay(1);
        }
        m = m >> 1;
        SPI_SW_Delay(1);
    }
    SCL_L;
    SDA_H;
    SPI_SW_Delay(2);
    CS_H;
    SPI_SW_Delay(10);

    __enable_irq();
}

__attribute__((section(".ramfunc.$SRAM_ITC_cm7"))) void board_elcdif_rk024hh298_spi_send_data(uint8_t data)
{
    uint8_t n, m;

    __disable_irq();

    m = 0x80;

    CS_L;
    SPI_SW_Delay(1);
    SCL_L;
    SDA_H;
    SPI_SW_Delay(1);
    SCL_H;
    SPI_SW_Delay(2);
    for (n = 0; n < 8; n++)
    {
        if (data & m)
        {
            SCL_L;
            SPI_SW_Delay(1);
            SDA_H;
            SPI_SW_Delay(1);
            SCL_H;
            SPI_SW_Delay(1);
        }
        else
        {
            SCL_L;
            SPI_SW_Delay(1);
            SDA_L;
            SPI_SW_Delay(1);
            SCL_H;
            SPI_SW_Delay(1);
        }
        m = m >> 1;
        SPI_SW_Delay(1);
    }
    SCL_L;
    SDA_H;
    SPI_SW_Delay(2);
    CS_H;
    SPI_SW_Delay(10);

    __enable_irq();
}
#endif

status_t BOARD_MIPICameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_MIPI_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_MIPICameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_MIPI_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
}

void BOARD_MIPICameraI2CInit(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_MIPI_CAMERA_I2C_CLOCK_SOURCE,
        .div      = BOARD_MIPI_CAMERA_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_MIPI_CAMERA_I2C_CLOCK_ROOT, &lpi2cClockConfig);
    BOARD_LPI2C_Init(BOARD_MIPI_CAMERA_I2C_BASE, CLOCK_GetRootClockFreq(BOARD_MIPI_CAMERA_I2C_CLOCK_ROOT));
}

void BOARD_PullCSICameraResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_CSI_CAMERA_RESET_GPIO, BOARD_CSI_CAMERA_RESET_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_CSI_CAMERA_RESET_GPIO, BOARD_CSI_CAMERA_RESET_PIN, 0U);
    }
}

void BOARD_PullCSICameraPowerDownPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_CSI_CAMERA_PWDN_GPIO, BOARD_CSI_CAMERA_PWDN_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_CSI_CAMERA_PWDN_GPIO, BOARD_CSI_CAMERA_PWDN_PIN, 0U);
    }
}

status_t BOARD_CSICameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CSI_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_CSICameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CSI_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
}

void board_csi_camera_i2c_init(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_CSI_CAMERA_I2C_CLOCK_SOURCE,
        .div      = BOARD_CSI_CAMERA_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_CSI_CAMERA_I2C_CLOCK_ROOT, &lpi2cClockConfig);
    BOARD_LPI2C_Init(BOARD_CSI_CAMERA_I2C_BASE, CLOCK_GetRootClockFreq(BOARD_CSI_CAMERA_I2C_CLOCK_ROOT));
}

void BOARD_FlexioCameraI2CInit(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_FLEXIO_CAMERA_I2C_CLOCK_SOURCE,
        .div      = BOARD_FLEXIO_CAMERA_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_FLEXIO_CAMERA_I2C_CLOCK_ROOT, &lpi2cClockConfig);

    BOARD_LPI2C_Init(BOARD_FLEXIO_CAMERA_I2C_BASE, CLOCK_GetRootClockFreq(BOARD_FLEXIO_CAMERA_I2C_CLOCK_ROOT));
}

status_t BOARD_FlexioCameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_FLEXIO_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_FlexioCameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_FLEXIO_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
}

void Board_PullFlexioCameraResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_FLEXIO_CAMERA_RESET_GPIO, BOARD_FLEXIO_CAMERA_RESET_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_FLEXIO_CAMERA_RESET_GPIO, BOARD_FLEXIO_CAMERA_RESET_PIN, 0U);
    }
}

void BOARD_PullFlexioCameraPowerDownPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_FLEXIO_CAMERA_PWDN_GPIO, BOARD_FLEXIO_CAMERA_PWDN_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_FLEXIO_CAMERA_PWDN_GPIO, BOARD_FLEXIO_CAMERA_PWDN_PIN, 0U);
    }
}

void BOARD_3DCameraI2CInit(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_3D_CAMERA_I2C_CLOCK_SOURCE,
        .div      = BOARD_3D_CAMERA_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_3D_CAMERA_I2C_CLOCK_ROOT, &lpi2cClockConfig);

    BOARD_LPI2C_Init(BOARD_3D_CAMERA_I2C_BASE, CLOCK_GetRootClockFreq(BOARD_3D_CAMERA_I2C_CLOCK_ROOT));
}

status_t BOARD_3DCameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_3D_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_3DCameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_3D_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}

void BOARD_Pull3DCameraResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_RESET_GPIO, BOARD_3D_CAMERA_RESET_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_RESET_GPIO, BOARD_3D_CAMERA_RESET_PIN, 0U);
    }
}

void BOARD_Pull3DCameraPowerEnablePin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_PWR_GPIO, BOARD_3D_CAMERA_PWR_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_PWR_GPIO, BOARD_3D_CAMERA_PWR_PIN, 0U);
    }
}

void BOARD_Pull3DCameraTestModePin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_TEST_MODE_GPIO, BOARD_3D_CAMERA_TEST_MODE_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_TEST_MODE_GPIO, BOARD_3D_CAMERA_TEST_MODE_PIN, 0U);
    }
}

void BOARD_Pull3DCameraLDPIntPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_LDP_INT_GPIO, BOARD_3D_CAMERA_LDP_INT_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_LDP_INT_GPIO, BOARD_3D_CAMERA_LDP_INT_PIN, 0U);
    }
}

void BOARD_pull_3d_camera_bridge_reset_pin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_BRIDGE_RESET_GPIO, BOARD_3D_CAMERA_BRIDGE_RESET_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_BRIDGE_RESET_GPIO, BOARD_3D_CAMERA_BRIDGE_RESET_PIN, 0U);
    }
}

void board_pull_elcdif_rk024hh298_reset_pin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_ELCDIF_DISPLAY_RESET_GPIO, BOARD_ELCDIF_DISPLAY_RESET_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_ELCDIF_DISPLAY_RESET_GPIO, BOARD_ELCDIF_DISPLAY_RESET_PIN, 0U);
    }
}

void board_elcdif_rk024hh298_backlight_control(bool on)
{
    if (on)
    {
        GPIO_PinWrite(BOARD_ELCDIF_DISPLAY_BACKLIGHT_GPIO, BOARD_ELCDIF_DISPLAY_BACKLIGHT_PIN, 0U);
    }
    else
    {
        GPIO_PinWrite(BOARD_ELCDIF_DISPLAY_BACKLIGHT_GPIO, BOARD_ELCDIF_DISPLAY_BACKLIGHT_PIN, 1U);
    }
}

void BOARD_InitRGBLedResource(void)
{
    BOARD_InitRGBLedPins();
}

void BOARD_InitElcdifRk024hh298Resource(void)
{
    /* GPIO configuration of PERI_PWREN on PMIC_STBY_REQ (pin L7) */
    gpio_pin_config_t pin_config = {
        .direction = kGPIO_DigitalOutput, .outputLogic = 0U, .interruptMode = kGPIO_NoIntmode};

    BOARD_InitElcdifRk024hh298LcdPins();

    pin_config.outputLogic = 1;

    /* reset = 1, lcd work normally. */
    GPIO_PinInit(BOARD_ELCDIF_DISPLAY_RESET_GPIO, BOARD_ELCDIF_DISPLAY_RESET_PIN, &pin_config);
    GPIO_PinInit(BOARD_ELCDIF_DISPLAY_SPI_CS_GPIO, BOARD_ELCDIF_DISPLAY_SPI_CS_PIN, &pin_config);
    GPIO_PinInit(BOARD_ELCDIF_DISPLAY_SPI_SDA_GPIO, BOARD_ELCDIF_DISPLAY_SPI_SDA_PIN, &pin_config);

    pin_config.outputLogic = 0;
    GPIO_PinInit(BOARD_ELCDIF_DISPLAY_SPI_SCL_GPIO, BOARD_ELCDIF_DISPLAY_SPI_SCL_PIN, &pin_config);

    /* backlight = 0, lcd work normally. Before splash screen ready, set 1 to power down backlight. */
    /* turn off the backlight. */
    board_elcdif_rk024hh298_backlight_control(0);
}

void BOARD_InitFlexioCameraResource(void)
{
    BOARD_InitFlexioCameraPins();

    /* Set the pins for CSI reset and power down. */
    gpio_pin_config_t pinConfig = {
        kGPIO_DigitalOutput,
        0,
        kGPIO_NoIntmode,
    };

    /* GC0308 Sensor PWDN
     * 0: normal work,
     * 1: standby     */
    GPIO_PinInit(BOARD_FLEXIO_CAMERA_PWDN_GPIO, BOARD_FLEXIO_CAMERA_PWDN_PIN, &pinConfig);

    const clock_root_config_t flexiorootclk = {
        .clockOff = false,
        .mux      = kCLOCK_FLEXIO1_ClockRoot_MuxSysPll3Div2,
        .div      = 2,
    };

    /* Configure FLEXIO1 using SYS_PLL3_DIV2_CLK */
    CLOCK_SetRootClock(kCLOCK_Root_Flexio2, &flexiorootclk);

#if 0

    /* GC0308 Sensor Reset
     * 0: chip reset
     * 1: normal work. */
    pinConfig.outputLogic = 1;
    GPIO_PinInit(BOARD_FLEXIO_CAMERA_RESET_GPIO, BOARD_FLEXIO_CAMERA_RESET_PIN, &pinConfig);

    pinConfig.outputLogic = 0;
    GPIO_PinInit(BOARD_FLEXIO_CAMERA_RESET_GPIO, BOARD_FLEXIO_CAMERA_RESET_PIN, &pinConfig);

    pinConfig.outputLogic = 1;
    GPIO_PinInit(BOARD_FLEXIO_CAMERA_RESET_GPIO, BOARD_FLEXIO_CAMERA_RESET_PIN, &pinConfig);
#endif
}

void BOARD_InitCSICameraResource(void)
{
    BOARD_InitParallelCameraPins();

    /* Set the pins for CSI reset and power down. */
    gpio_pin_config_t pinConfig = {
        kGPIO_DigitalOutput,
        0,
        kGPIO_NoIntmode,
    };

    /* GC0308 Sensor PWDN
     * 0: normal work,
     * 1: standby     */
    GPIO_PinInit(BOARD_CSI_CAMERA_PWDN_GPIO, BOARD_CSI_CAMERA_PWDN_PIN, &pinConfig);

    /* GC0308 Sensor Reset
     * 0: chip reset
     * 1: normal work. */
    pinConfig.outputLogic = 1;
    GPIO_PinInit(BOARD_CSI_CAMERA_RESET_GPIO, BOARD_CSI_CAMERA_RESET_PIN, &pinConfig);

    clock_root_config_t rootCfg = {
        .mux = kCLOCK_CSI_ClockRoot_MuxOscRc48MDiv2,
        .div = 1,
    };

    /* Configure CSI using OSC_RC_48M_DIV2 */
    CLOCK_SetRootClock(kCLOCK_Root_Csi, &rootCfg);

    board_csi_camera_i2c_init();
}

void BOARD_InitBleQn9090Resource(void)
{
    gpio_pin_config_t pin_config = {
        .direction = kGPIO_DigitalOutput, .outputLogic = 0U, .interruptMode = kGPIO_NoIntmode};

    BOARD_InitBluetoothQn9090Pins();
    GPIO_PinInit(BOARD_BLE_QN9090_QN_WAKEUP_GPIO, BOARD_BLE_QN9090_QN_WAKEUP_PIN, &pin_config);
}

void BOARD_InitWIFIAW_AM510Resource(void)
{
    BOARD_InitWiFiPins();
}

void BOARD_InitEDMA()
{
    edma_config_t edmaConfig;
    DMAMUX_Init(DMAMUX0);
    EDMA_GetDefaultConfig(&edmaConfig);
    edmaConfig.enableDebugMode = true;
    EDMA_Init(DMA0, &edmaConfig);
}

void BOARD_InitMQSResource(void)
{
    clock_root_config_t root_clock_mqs;

    CLOCK_InitAudioPll(&audioPllConfig);
    BOARD_InitMQSPins();
    root_clock_mqs.div = MQS_SAI_CLOCK_SOURCE_DIVIDER;
    root_clock_mqs.mux = MQS_SAI_CLOCK_SOURCE_SELECT;
    CLOCK_SetRootClock(kCLOCK_Root_Mqs, &root_clock_mqs);
}

void BOARD_InitFlashResources()
{
    BOARD_InitFlexSPI1Pins();
    /*Clock setting for flexspi1*/
    clock_root_config_t root_clock_flexspi1 = {.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd2, .div = 3};

    CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &root_clock_flexspi1);
}

void BOARD_InitHardware(void)
{
#if RELOCATE_VECTOR_TABLE
    BOARD_RelocateVectorTableToRam();
#endif
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitEDMA();
    BOARD_InitPushButtonPins();
    BOARD_InitLedPins();
    Time_Init(1);
}

void GPIO2_Combined_16_31_IRQHandler(void)
{
    uint32_t intPin        = 0x00;
    uint32_t pushButtonInt = 0x00;
    // Get interrupt flag for the GPIO
    intPin = GPIO_PortGetInterruptFlags(GPIO2);

    pushButtonInt = (1 << BOARD_BUTTON_SW2_PIN) | (1 << BOARD_BUTTON_SW3_PIN);

    // Check for the interrupt pin on the GPIO for USER_BUTTON (SW7)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;

        // Check if interrupt was triggered by a push button
        if (intPin & pushButtonInt)
        {
            INPUT_DEV_PUSH_BUTTONS_IRQHandler(GPIO2, intPin);
            /* clear the interrupt status */
            GPIO_PortClearInterruptFlags(GPIO2, pushButtonInt);

            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
        else
        {
            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
    }
}
void GPIO2_Combined_0_15_IRQHandler(void)
{
    uint32_t intPin        = 0x00;
    uint32_t pushButtonInt = 0x00;
    // Get interrupt flag for the GPIO
    intPin        = GPIO_PortGetInterruptFlags(GPIO2);
    pushButtonInt = (1 << BOARD_BUTTON_SW1_PIN);

    // Check for the interrupt pin on the GPIO for USER_BUTTON (SW7)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;

        // Check if interrupt was triggered by a push button
        if (intPin & pushButtonInt)
        {
            INPUT_DEV_PUSH_BUTTONS_IRQHandler(GPIO2, intPin);

            /* clear the interrupt status */
            GPIO_PortClearInterruptFlags(GPIO2, pushButtonInt);

            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
        else
        {
            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
    }
}

void GPIO13_Combined_0_31_IRQHandler(void)
{
    uint32_t intPin        = 0x00;
    uint32_t pushButtonInt = 0x00;
    // Get interrupt flag for the GPIO
    intPin        = GPIO_PortGetInterruptFlags(GPIO13);
    pushButtonInt = (1 << BOARD_USER_BUTTON_GPIO_PIN);

    // Check for the interrupt pin on the GPIO for USER_BUTTON (SW7)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;

        // Check if interrupt was triggered by a push button
        if (intPin & pushButtonInt)
        {
            INPUT_DEV_PUSH_BUTTONS_IRQHandler(GPIO13, intPin);
            /* clear the interrupt status */
            GPIO_PortClearInterruptFlags(GPIO13, pushButtonInt);

            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
        else
        {
            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
    }
}
