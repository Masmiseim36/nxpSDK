/*
 * Copyright  2017-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_gpio.h"
#include "fsl_csi.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_ov7725.h"
#include "fsl_mt9m114.h"
#include "fsl_iomuxc.h"

#include "app.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/

extern void CSI_DriverIRQHandler(void);

void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();
}

/* Initialize the LCD_DISP. */
void BOARD_InitLcd(void)
{
    gpio_pin_config_t config = {
        kGPIO_DigitalOutput,
        0,
        kGPIO_NoIntmode,
    };

    /* Backlight. */
    config.outputLogic = 1;
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
}

void BOARD_InitLcdifPixClock(void)
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 60 = 9.2M.
     * Here set the LCDIF pixel clock to 9.3M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31,
        .postDivider = 8,
        .numerator   = 0,
        .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);

    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 4);

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
}

static void BOARD_PullCameraResetPin(bool pullUp)
{
    /* Reset pin is connected to DCDC_3V3. */
    return;
}

#if (DEMO_CAMERA_TYPE == DEMO_CAMERA_OV7725)
static void BOARD_PullCameraPowerDownPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(GPIO1, 4, 1);
    }
    else
    {
        GPIO_PinWrite(GPIO1, 4, 0);
    }
}

static ov7725_resource_t ov7725Resource = {
    .i2cSendFunc       = BOARD_Camera_I2C_SendSCCB,
    .i2cReceiveFunc    = BOARD_Camera_I2C_ReceiveSCCB,
    .pullResetPin      = BOARD_PullCameraResetPin,
    .pullPowerDownPin  = BOARD_PullCameraPowerDownPin,
    .inputClockFreq_Hz = 24000000,
};

camera_device_handle_t cameraDevice = {
    .resource = &ov7725Resource,
    .ops      = &ov7725_ops,
};
#else
/*
 * MT9M114 camera module has PWDN pin, but the pin is not
 * connected internally, MT9M114 does not have power down pin.
 * The reset pin is connected to high, so the module could
 * not be reseted, so at the begining, use GPIO to let camera
 * release the I2C bus.
 */
static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < 0x200; i++)
    {
        __NOP();
    }
}

#define CAMERA_I2C_SCL_GPIO GPIO1
#define CAMERA_I2C_SCL_PIN  16
#define CAMERA_I2C_SDA_GPIO GPIO1
#define CAMERA_I2C_SDA_PIN  17

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i                          = 0;
    const gpio_pin_config_t pin_config = {
        .direction     = kGPIO_DigitalOutput,
        .outputLogic   = 1,
        .interruptMode = kGPIO_NoIntmode,
    };

    CLOCK_EnableClock(kCLOCK_Iomuxc);

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16, 0U);

    GPIO_PinInit(CAMERA_I2C_SCL_GPIO, CAMERA_I2C_SCL_PIN, &pin_config);
    GPIO_PinInit(CAMERA_I2C_SDA_GPIO, CAMERA_I2C_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(CAMERA_I2C_SDA_GPIO, CAMERA_I2C_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(CAMERA_I2C_SCL_GPIO, CAMERA_I2C_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(CAMERA_I2C_SDA_GPIO, CAMERA_I2C_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(CAMERA_I2C_SCL_GPIO, CAMERA_I2C_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(CAMERA_I2C_SCL_GPIO, CAMERA_I2C_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(CAMERA_I2C_SDA_GPIO, CAMERA_I2C_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(CAMERA_I2C_SCL_GPIO, CAMERA_I2C_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(CAMERA_I2C_SDA_GPIO, CAMERA_I2C_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

static mt9m114_resource_t mt9m114Resource = {
    .i2cSendFunc       = BOARD_Camera_I2C_Send,
    .i2cReceiveFunc    = BOARD_Camera_I2C_Receive,
    .pullResetPin      = BOARD_PullCameraResetPin,
    .inputClockFreq_Hz = 24000000,
};

camera_device_handle_t cameraDevice = {
    .resource = &mt9m114Resource,
    .ops      = &mt9m114_ops,
};
#endif

void BOARD_InitCameraResource(void)
{
    BOARD_Camera_I2C_Init();

    /* CSI MCLK select 24M. */
    /*
     * CSI clock source:
     *
     * 00 derive clock from osc_clk (24M)
     * 01 derive clock from PLL2 PFD2
     * 10 derive clock from pll3_120M
     * 11 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_CsiMux, 0);
    /*
     * CSI clock divider:
     *
     * 000 divide by 1
     * 001 divide by 2
     * 010 divide by 3
     * 011 divide by 4
     * 100 divide by 5
     * 101 divide by 6
     * 110 divide by 7
     * 111 divide by 8
     */
    CLOCK_SetDiv(kCLOCK_CsiDiv, 0);

    /*
     * For RT1050, there is not dedicate clock gate for CSI MCLK, it use CSI
     * clock gate.
     */

    /* Set the pins for CSI reset and power down. */
    gpio_pin_config_t pinConfig = {
        kGPIO_DigitalOutput,
        1,
        kGPIO_NoIntmode,
    };

    GPIO_PinInit(GPIO1, 4, &pinConfig);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitDEBUG_UARTPins();
#if (DEMO_CAMERA_TYPE != DEMO_CAMERA_OV7725)
    BOARD_I2C_ReleaseBus();
#endif
    BOARD_InitCSIPins();
    BOARD_InitLCDPins();
    BOARD_InitBootClocks();
    BOARD_InitLcdifPixClock();
    BOARD_InitDebugConsole();
    BOARD_InitLcd();
    BOARD_InitCameraResource();
}

/*${function:end}*/
