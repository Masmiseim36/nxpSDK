/*
 * Copyright 2024 NXP
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
#include "fsl_gpio.h"
#include "camera_config.h"
#include "fsl_ov7670.h"
#include "fsl_inputmux.h"
#include "fsl_ezhv.h"
#include "ezhv_para.h"
#include "ezhv_support.h"
#include "fsl_flexio_camera.h"
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define VSYNC_IRQ_HANDLER GPIO20_IRQHandler
#define SCCB_CLOCK_FREQ    CLOCK_GetLPFlexCommClkFreq(8U)
#define SCCB_BASE          ((LPI2C_Type *)LPI2C8_BASE)
#define DEMO_FLEXIO_CLOCK_FREQ CLOCK_GetFlexioClkFreq()
#define FLEXIO_MAX_FREQ (DEMO_FLEXIO_CLOCK_FREQ / 2U)
#define FLEXIO_MIN_FREQ (DEMO_FLEXIO_CLOCK_FREQ / 512U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void CAMERA_I2CInit(void);
static status_t CAMERA_I2CSend(uint8_t deviceAddress,
                         uint32_t subAddress,
                         uint8_t subAddressSize,
                         const uint8_t *txBuff,
                         uint8_t txBuffSize);
static status_t CAMERA_I2CReceive(uint8_t deviceAddress,
                            uint32_t subAddress,
                            uint8_t subAddressSize,
                            uint8_t *rxBuff,
                            uint8_t rxBuffSize);
static void CAMERA_XclkInit(uint32_t freq_Hz);
static void CAMERA_FlexioInit(void);
static void CAMERA_InterruptsInit(void);
static void CAMERA_ResetPinInit(void);
static void CAMERA_Init(void);
/*******************************************************************************
 * Variables
 *******************************************************************************/
/* variable of transfer handle. */
static lpi2c_master_handle_t s_handle;
/* flag of I2C transfer completion. */
static volatile bool g_MasterCompletionFlag = false;
/* flag of I2C transfer with no ack. */
static volatile bool g_MasterNackFlag       = false;
/* camera buffers */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_buf[CAMERA_HEIGHT*CAMERA_STRIDE], 128);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_buf1[CAMERA_HEIGHT*CAMERA_STRIDE], 128);

volatile uint32_t g_newVideoFrame = 0;

volatile CameraDvpTransfer g_dvpTransfer = {
  .driverIdx = 0,
  .userIdx = 1,
  .queue[0] = {
    .len = CAMERA_HEIGHT * CAMERA_STRIDE,
    .pBuf = NULL,
  },
  .queue[1] = {
    .len = CAMERA_HEIGHT* CAMERA_STRIDE,
    .pBuf = NULL,
  },
};

volatile CameraBuffer_t *g_stCamBuf = NULL;

static ov7670_resource_t ov7670Resource = {
    .i2cSendFunc    = CAMERA_I2CSend,
    .i2cReceiveFunc = CAMERA_I2CReceive,
    .xclock         = kOV7670_InputClock12MHZ,
};

camera_device_handle_t cameraDevice = {
    .resource = &ov7670Resource,
    .ops      = &ov7670_ops,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
void camera_task(void *pvParam)
{

    /* boot EZHV */
    CAMERA_Init();

    BOARD_EZHV_Init();

    EZHV_Para_t *pstPara = (EZHV_Para_t *)EZHV_GetParaAddr();
    pstPara->apiIdx = kEZHV_API_flexioDvp;
    g_stCamBuf = (CameraBuffer_t*)(&pstPara->paraAddr);
    g_stCamBuf->pBuf = g_dvpTransfer.queue[g_dvpTransfer.driverIdx].pBuf;
    g_stCamBuf->len = g_dvpTransfer.queue[g_dvpTransfer.driverIdx].len;

    EZHV_WakeUpEzhv(kEZHV_ARM2EZHV_MSI);
    while(1)
    {
        vTaskDelay(portMAX_DELAY);
    }
}

/*!
 * @brief Main function
 */
static void CAMERA_Init(void)
{
    const camera_config_t cameraConfig = {
        .pixelFormat = kVIDEO_PixelFormatRGB565,
	    .bytesPerPixel = CAMERA_BPP,
	    .resolution = FSL_VIDEO_RESOLUTION(CAMERA_WIDTH,CAMERA_HEIGHT),
	    .interface = kCAMERA_InterfaceGatedClock,
	    .controlFlags = CAMERA_CTRL_FLAG,
	    .framePerSec = CAMERA_FRAME_RATE
    };

    g_dvpTransfer.queue[0].pBuf = (void *)s_buf;
    g_dvpTransfer.queue[1].pBuf = (void *)s_buf1;

    /* make sure flexio root clock freq >= 4 * XCLK freq */
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_FLEXIO);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 5U);
    RESET_PeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);

    CAMERA_ResetPinInit();
    /* Init the I2C to configure ov7670 */
    CAMERA_I2CInit();
    /* Init Flexio to receive data from ov7670 */
    CAMERA_FlexioInit();
    /* Init Interrupts */
    CAMERA_InterruptsInit();
    /* Configure the camera sensor OV7670 */
    status_t ov7670_status = kStatus_Success;
    do
    {
       ov7670_status = CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);
    } while(ov7670_status != kStatus_Success);

    return;
}

/*!
 * brief callback function for lpi2c.
 *
 */
static void lpi2c_camera_callback(LPI2C_Type *base,
                     lpi2c_master_handle_t *handle,
                                   status_t status,
                                    void *userData)
{
    if (status == kStatus_LPI2C_Nak)
    {
        g_MasterNackFlag = true;
    }
    else
    {
        g_MasterCompletionFlag = true;
        /* Display failure information when status is not success. */
        if (status != kStatus_Success)
        {
            //PRINTF("Error occured during transfer!");
        }
    }
}

/*!
 * brief Initialize Camera SCCB interface.
 *
 */
static void CAMERA_I2CInit(void)
{
    /* i2c master init */
    lpi2c_master_config_t masterConfig;

    CLOCK_AttachClk(kOSC_CLK_to_FCCLK0);
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM8);

    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_MasterTransferCreateHandle(SCCB_BASE, &s_handle,
                                lpi2c_camera_callback, NULL);

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
    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(SCCB_BASE, &masterConfig, SCCB_CLOCK_FREQ);
}

/*!
 * brief Camera SCCB Send.
 *
 */
static status_t CAMERA_I2CSend(uint8_t deviceAddress,
                         uint32_t subAddress,
                         uint8_t subAddressSize,
                         const uint8_t *txBuff,
                         uint8_t txBuffSize)
{
    status_t reVal = kStatus_Success;
    lpi2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kLPI2C_Write;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data           = (void *)txBuff;
    masterXfer.dataSize       = txBuffSize;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferNonBlocking(SCCB_BASE, &s_handle, &masterXfer);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
	
    /*  wait for transfer completed. */
    while ((!g_MasterNackFlag) && (!g_MasterCompletionFlag))
    {
    }

    g_MasterNackFlag = false;

    if (g_MasterCompletionFlag == true)
    {
        g_MasterCompletionFlag = false;
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }

}

/*!
 * brief Camera SCCB Receive.
 *
 */
static status_t CAMERA_I2CReceive(uint8_t deviceAddress,
                            uint32_t subAddress,
                            uint8_t subAddressSize,
                            uint8_t *rxBuff,
                            uint8_t rxBuffSize)
{
    status_t reVal = kStatus_Success;
    lpi2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));
    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = NULL;
    masterXfer.dataSize       = 0;
    masterXfer.direction      = kLPI2C_Write;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferNonBlocking(SCCB_BASE, &s_handle, &masterXfer);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /*  wait for transfer completed. */
    while ((!g_MasterNackFlag) && (!g_MasterCompletionFlag))
    {
    }

    g_MasterNackFlag = false;
    if (g_MasterCompletionFlag == true)
    {
        g_MasterCompletionFlag = false;
    }
    else
    {
        return kStatus_Fail;
    }

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.subaddress     = 0x0;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = rxBuff;
    masterXfer.dataSize       = rxBuffSize;
    masterXfer.direction      = kLPI2C_Read;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferNonBlocking(SCCB_BASE, &s_handle, &masterXfer);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /*  wait for transfer completed. */
    while ((!g_MasterNackFlag) && (!g_MasterCompletionFlag))
    {
    }

    g_MasterNackFlag = false;
    if (g_MasterCompletionFlag == true)
    {
        g_MasterCompletionFlag = false;
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

static void CAMERA_ResetPinInit(void)
{
    gpio_pin_config_t outConfig = {kGPIO_DigitalOutput, 1};

    GPIO_PinInit(DEMO_RESET_PORT, DEMO_RESET_PIN, &outConfig);
}

/*!
 * brief Initialize flexio for cameraIf.
 *
 */
static void CAMERA_FlexioInit(void)
{
    /* Initialize */
    FLEXIO_CAMERA_Type base = {
		.flexioBase = FLEXIO,
		.timerIdx = DEMO_FLEXIO_PCLK_TIMER,
		.datPinStartIdx = DEMO_FLEXIO_DATA0_IDX,
		.hrefPinIdx =DEMO_FELXIO_HREF_IDX,
		.pclkPinIdx = DEMO_FELXIO_PCLK_IDX,
		.shifterStartIdx = DEMO_FLEXIO_SHIFTER0_IDX,
		.shifterCount = DEMO_FLEXIO_SHIFTER_NUM,
    };

    flexio_camera_config_t config;
    FLEXIO_CAMERA_GetDefaultConfig(&config);
    config.enablecamera = true;
    config.enableInDebug = true;
    FLEXIO_CAMERA_Init(&base, &config);
    CAMERA_XclkInit(DEMO_FLEXIO_XCLK_FREQ_HZ);

    /* flexio's shifter0 interrupt is used by the EZH-V. */
    FLEXIO_CAMERA_EnableInterrupt(&base);
}

/*!
 * brief Configure the XCLK frequency.
 *
 */
static void CAMERA_XclkInit(uint32_t freq_Hz)
{
    assert((freq_Hz < FLEXIO_MAX_FREQ) && (freq_Hz > FLEXIO_MIN_FREQ));

    uint32_t lowerValue = 0;
    uint32_t upperValue = 0;
    uint32_t sum        = 0;
    flexio_timer_config_t timerConfig;

    /* Configure the timer DEMO_FLEXIO_TIMER_CH for generating PWM */
    timerConfig.triggerSelect   = 0U; /* any value, ok */
    timerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    timerConfig.pinConfig       = kFLEXIO_PinConfigOutput;
    timerConfig.pinPolarity     = kFLEXIO_PinActiveHigh;
    timerConfig.pinSelect       = DEMO_FLEXIO_XCLK_IDX; /* Set pwm output */
    timerConfig.timerMode       = kFLEXIO_TimerModeDual8BitPWM;
    timerConfig.timerOutput     = kFLEXIO_TimerOutputOneNotAffectedByReset;
    timerConfig.timerDecrement  = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerDisable    = kFLEXIO_TimerDisableNever;
    timerConfig.timerEnable     = kFLEXIO_TimerEnabledAlways;
    timerConfig.timerReset      = kFLEXIO_TimerResetNever;
    timerConfig.timerStart      = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerStop       = kFLEXIO_TimerStopBitDisabled;

    /* Calculate timer lower and upper values of TIMCMP */
    /* Calculate the nearest integer value for sum, 
       using formula round(x) = (2 * floor(x) + 1) / 2 */
    /* sum = DEMO_FLEXIO_CLOCK_FREQ / freq_H */
    sum = (DEMO_FLEXIO_CLOCK_FREQ * 2 / freq_Hz + 1) / 2;

    /* Calculate the nearest integer value for lowerValue,
       the high period of the pwm output */
    lowerValue = (sum >> 1) - 1;
    /* Calculate upper value, the low period of the pwm output */
    upperValue = (sum >> 1) - 1;

    timerConfig.timerCompare = ((upperValue << 8U) | (lowerValue));

    FLEXIO_SetTimerConfig(FLEXIO, DEMO_FLEXIO_XCLK_TIMER, &timerConfig);
}

static void EZHV_Callback(void *userData)
{
    g_newVideoFrame = 1;
    g_dvpTransfer.driverIdx = (g_dvpTransfer.driverIdx+1) % QUEUE_SIZE;
    g_stCamBuf->len = g_dvpTransfer.queue[g_dvpTransfer.driverIdx].len;
    g_stCamBuf->pBuf = g_dvpTransfer.queue[g_dvpTransfer.driverIdx].pBuf;
}

static void CAMERA_InterruptsInit(void)
{
    /* XSYNC gpio interrupt setting for EZH-V */
    GPIO_SetPinInterruptConfig(DEMO_XSYNC_PORT, DEMO_XSYNC_PIN, kGPIO_InterruptRisingEdge);
    GPIO_EnableInterruptControlNonSecure(GPIO2, 0x3);
    GPIO_EnablePinControlNonSecure(GPIO2, 1 << DEMO_XSYNC_PIN);
    GPIO_EnablePinControlNonPrivilege(GPIO2, 1 << DEMO_XSYNC_PIN);
    GPIO_EnableInterruptControlNonPrivilege(GPIO2, 0x3);

    /* flexio irq to ezhv's trigger */
    INPUTMUX_Init(INPUTMUX0);
    RESET_ClearPeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);    
    INPUTMUX_AttachSignal(INPUTMUX0, 0, kINPUTMUX_Gpio2IrqToEzhv);
    INPUTMUX_AttachSignal(INPUTMUX0, 1, kINPUTMUX_FlexioIrqToEzhv);
    INPUTMUX_Deinit(INPUTMUX0);

    /* eanble ezhv interrupt to ARM */
    EZHV_EnableEzhv2ArmIntChan(kEZHV_EzhvToArmIntChan0); 
    EnableIRQ(EZHV_IRQn);
    NVIC_SetPriority(EZHV_IRQn, 2);
    EZHV_SetCallback(EZHV_Callback, 0, NULL);
}
