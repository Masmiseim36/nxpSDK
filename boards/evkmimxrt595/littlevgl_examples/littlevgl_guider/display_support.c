/*
 * Copyright (c) 2019-2020, NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "display_support.h"
#include "fsl_gpio.h"
#if (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL)
#include "fsl_dbi_flexio_smartdma.h"
#include "fsl_dc_fb_ssd1963.h"
#include "fsl_dma.h"
#include "fsl_inputmux.h"
#elif ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL))
#include "fsl_dc_fb_lcdif.h"
#include "fsl_rm68200.h"
#include "fsl_rm68191.h"
#include "fsl_mipi_dsi.h"
#else
#include "fsl_dc_fb_dsi_cmd.h"
#include "fsl_rm67162.h"
#include "fsl_mipi_dsi.h"
#include "fsl_inputmux.h"
#include "fsl_mipi_dsi_smartdma.h"
#endif
#include "fsl_power.h"
#include "pin_mux.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if (((DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL) && (DEMO_SSD1963_USE_FLEXIO_SMARTDMA)) || \
     ((DEMO_PANEL_RM67162 == DEMO_PANEL) && (DEMO_RM67162_USE_DSI_SMARTDMA)))
static void BOARD_InitSmartDMA(void)
{
    RESET_ClearPeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX);
#if (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL)
    INPUTMUX_AttachSignal(INPUTMUX, 0, kINPUTMUX_FlexioIrqToSmartDmaInput);
#else
    INPUTMUX_AttachSignal(INPUTMUX, 0, kINPUTMUX_MipiIrqToSmartDmaInput);
#endif

    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    POWER_DisablePD(kPDRUNCFG_APD_SMARTDMA_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_SMARTDMA_SRAM);
    POWER_ApplyPD();

    RESET_ClearPeripheralReset(kSMART_DMA_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Smartdma);

    SMARTDMA_InitWithoutFirmware();
    NVIC_EnableIRQ(SDMA_IRQn);

    NVIC_SetPriority(SDMA_IRQn, 3);
}
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * PANEL_TFT_PROTO_5 SSD1963 controller
 */

/* SSD1963 XTAL_IN clock frequency, decided by the LCD board, don't change. */
#define DEMO_SSD1963_XTAL_FREQ 10000000U
/* Configures the SSD1963 output PCLK frequency, for 60Hz frame rate,
 * PCLK = (800 + 48 + 40 + 0) * (480 + 3 + 13 + 18) * 60 */
#define DEMO_SSD1963_PCLK_FREQ     30000000U
#define DEMO_SSD1963_HSW           48U
#define DEMO_SSD1963_HFP           40U
#define DEMO_SSD1963_HBP           0U
#define DEMO_SSD1963_VSW           3U
#define DEMO_SSD1963_VFP           13U
#define DEMO_SSD1963_VBP           18U
#define DEMO_SSD1963_POLARITY_FLAG 0U

#define DEMO_SSD1963_FLEXIO              FLEXIO0
#define DEMO_SSD1963_FLEXIO_CLOCK_FREQ   CLOCK_GetFlexioClkFreq()
#define DEMO_SSD1963_FLEXIO_BAUDRATE_BPS (DEMO_SSD1963_FLEXIO_CLOCK_FREQ * FLEXIO_MCULCD_DATA_BUS_WIDTH / 2)

/* Macros for LCD DMA. */
#define DEMO_SSD1963_DMA                   DMA0
#define DEMO_SSD1963_FLEXIO_TX_DMA_CHANNEL 32 /* Match the DEMO_SSD1963_FLEXIO_TX_START_SHIFTER */
#define DEMO_SSD1963_FLEXIO_RX_DMA_CHANNEL 31 /* Match the DEMO_SSD1963_FLEXIO_RX_END_SHIFTER*/
#define DEMO_SSD1963_FLEXIO_DMA_IRQn       DMA0_IRQn

/* Macros for FlexIO shifter, timer, and pins. */
#define DEMO_SSD1963_FLEXIO_WR_PIN           14
#define DEMO_SSD1963_FLEXIO_RD_PIN           15
#define DEMO_SSD1963_FLEXIO_DATA_PIN_START   0
#define DEMO_SSD1963_FLEXIO_TX_START_SHIFTER 0
#define DEMO_SSD1963_FLEXIO_RX_START_SHIFTER 0
#define DEMO_SSD1963_FLEXIO_TX_END_SHIFTER   7
#define DEMO_SSD1963_FLEXIO_RX_END_SHIFTER   7
#define DEMO_SSD1963_FLEXIO_TIMER            0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void BOARD_SetCSPin(bool set);
static void BOARD_SetRSPin(bool set);
static status_t BOARD_InitFlexioLCD(void);
static void BOARD_InitFlexioDMA(void);
static void BOARD_ResetSSD1963(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static dc_fb_ssd1963_handle_t s_dcFbSSD1963Handle;
static dbi_flexio_smartdma_xfer_handle_t g_dbiFlexioSmartdmaXferHandle;

static const dc_fb_ssd1963_config_t s_dcFbSSD1963Config = {
    .ssd1963Config = {.pclkFreq_Hz    = DEMO_SSD1963_PCLK_FREQ,
                      .pixelInterface = DC_FB_SSD1963_DEFAULT_PIXEL_FORMAT_SSD1963,
                      .panelDataWidth = kSSD1963_PanelData24Bit,
                      .polarityFlags  = DEMO_SSD1963_POLARITY_FLAG,
                      .panelWidth     = DEMO_PANEL_WIDTH,
                      .panelHeight    = DEMO_PANEL_HEIGHT,
                      .hsw            = DEMO_SSD1963_HSW,
                      .hfp            = DEMO_SSD1963_HFP,
                      .hbp            = DEMO_SSD1963_HBP,
                      .vsw            = DEMO_SSD1963_VSW,
                      .vfp            = DEMO_SSD1963_VFP,
                      .vbp            = DEMO_SSD1963_VBP},
    .xferOps       = &g_dbiFlexioSmartdmaXferOps,
    .xferOpsData   = &g_dbiFlexioSmartdmaXferHandle,
    .srcClock_Hz   = DEMO_SSD1963_XTAL_FREQ,
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsSSD1963,
    .prvData = &s_dcFbSSD1963Handle,
    .config  = &s_dcFbSSD1963Config,
};

/* The FlexIO MCU LCD device. */
static FLEXIO_MCULCD_Type flexioLcdDev = {
    .flexioBase          = DEMO_SSD1963_FLEXIO,
    .busType             = kFLEXIO_MCULCD_8080,
    .dataPinStartIndex   = DEMO_SSD1963_FLEXIO_DATA_PIN_START,
    .ENWRPinIndex        = DEMO_SSD1963_FLEXIO_WR_PIN,
    .RDPinIndex          = DEMO_SSD1963_FLEXIO_RD_PIN,
    .txShifterStartIndex = DEMO_SSD1963_FLEXIO_TX_START_SHIFTER,
    .txShifterEndIndex   = DEMO_SSD1963_FLEXIO_TX_END_SHIFTER,
    .rxShifterStartIndex = DEMO_SSD1963_FLEXIO_RX_START_SHIFTER,
    .rxShifterEndIndex   = DEMO_SSD1963_FLEXIO_RX_END_SHIFTER,
    .timerIndex          = DEMO_SSD1963_FLEXIO_TIMER,
    .setCSPin            = BOARD_SetCSPin,
    .setRSPin            = BOARD_SetRSPin,
    .setRDWRPin          = NULL /* Not used in 8080 mode. */
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_SetCSPin(bool set)
{
    GPIO_PinWrite(GPIO, BOARD_SSD1963_CS_PORT, BOARD_SSD1963_CS_PIN, (uint8_t)set);
}

static void BOARD_SetRSPin(bool set)
{
    GPIO_PinWrite(GPIO, BOARD_SSD1963_RS_PORT, BOARD_SSD1963_RS_PIN, (uint8_t)set);
}

static void BOARD_InitFlexioClock(void)
{
    CLOCK_AttachClk(kFRO_DIV2_to_FLEXIO);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1);

    RESET_ClearPeripheralReset(kFLEXIO_RST_SHIFT_RSTn);
}

static status_t BOARD_InitFlexioLCD(void)
{
    flexio_mculcd_config_t flexioMcuLcdConfig;

    /* Initialize the flexio MCU LCD. */
    /*
     * flexioMcuLcdConfig.enable = true;
     * flexioMcuLcdConfig.enableInDoze = false;
     * flexioMcuLcdConfig.enableInDebug = true;
     * flexioMcuLcdConfig.enableFastAccess = true;
     * flexioMcuLcdConfig.baudRate_Bps = 96000000U;
     */
    FLEXIO_MCULCD_GetDefaultConfig(&flexioMcuLcdConfig);
    flexioMcuLcdConfig.enableFastAccess = false;
    flexioMcuLcdConfig.baudRate_Bps     = DEMO_SSD1963_FLEXIO_BAUDRATE_BPS;

    return FLEXIO_MCULCD_Init(&flexioLcdDev, &flexioMcuLcdConfig, DEMO_SSD1963_FLEXIO_CLOCK_FREQ);
}

static void BOARD_InitFlexioDMA(void)
{
    BOARD_InitSmartDMA();
}

static void BOARD_ResetSSD1963(void)
{
    const gpio_pin_config_t resetPinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1,
    };

    /* Set SSD1963 CS, RS, and reset pin to output. */
    GPIO_PinInit(GPIO, BOARD_SSD1963_RST_PORT, BOARD_SSD1963_RST_PIN, &resetPinConfig);
    GPIO_PinInit(GPIO, BOARD_SSD1963_CS_PORT, BOARD_SSD1963_CS_PIN, &resetPinConfig);
    GPIO_PinInit(GPIO, BOARD_SSD1963_RS_PORT, BOARD_SSD1963_RS_PIN, &resetPinConfig);

    /* Reset the SSD1963 LCD controller. */
    GPIO_PinWrite(GPIO, BOARD_SSD1963_RST_PORT, BOARD_SSD1963_RST_PIN, 0);
    VIDEO_DelayMs(1); /* Delay 1ms (10ns required). */
    GPIO_PinWrite(GPIO, BOARD_SSD1963_RST_PORT, BOARD_SSD1963_RST_PIN, 1);
    VIDEO_DelayMs(5); /* Delay 5ms. */
}

status_t BOARD_PrepareDisplayController(void)
{
    status_t status;

    flexio_mculcd_smartdma_config_t flexioEzhConfig = {
#if DEMO_SSD1963_USE_RG565
        .inputPixelFormat = kFLEXIO_MCULCD_RGB565,
#else
        .inputPixelFormat = kFLEXIO_MCULCD_RGB888,
#endif
        .outputPixelFormat = kFLEXIO_MCULCD_RGB888,
    };

    BOARD_InitFlexioClock();

    status = BOARD_InitFlexioLCD();
    if (kStatus_Success != status)
    {
        return status;
    }

    BOARD_InitFlexioDMA();

    /* Create the DBI XFER handle. */
    status = DBI_FLEXIO_SMARTDMA_CreateXferHandle(&g_dbiFlexioSmartdmaXferHandle, &flexioLcdDev, &flexioEzhConfig);

    if (kStatus_Success != status)
    {
        return status;
    }

    BOARD_ResetSSD1963();

    return kStatus_Success;
}

#elif ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL))

#if BOARD_ENABLE_PSRAM_CACHE
/*
 * When PSRAM cache enabled and frame buffer placed in PSRAM, LCDIF reads the
 * cache first, then reads PSRAM if cache missed. Generally the frame buffer
 * is large, cache miss happens frequently, so the LCDIF read performance is low.
 *
 * There are three solutions:
 * 1. Disables the PSRAM cache, LCDIF reads from PSRAM directly.
 * 2. Slow down the frame rate, or use smaller pixel format, or use small resolution.
 * 3. Don't place frame buffer in PSRAM.
 *
 * In the example, solution 1 is used.
 */
#error Please read the comment about PSRAM cache and display refresh rate here
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * RK055AHD091 panel
 */

#if (DEMO_PANEL == DEMO_PANEL_RK055AHD091)
#define DEMO_LCDIF_HSW 8
#define DEMO_LCDIF_HFP 32
#define DEMO_LCDIF_HBP 32
#define DEMO_LCDIF_VSW 2
#define DEMO_LCDIF_VFP 16
#define DEMO_LCDIF_VBP 14

#elif (DEMO_PANEL_RK055IQH091 == DEMO_PANEL)

#define DEMO_LCDIF_HSW 2
#define DEMO_LCDIF_HFP 32
#define DEMO_LCDIF_HBP 30
#define DEMO_LCDIF_VSW 2
#define DEMO_LCDIF_VFP 16
#define DEMO_LCDIF_VBP 14

#endif

#define DEMO_LCDIF_POL_FLAGS \
    (kLCDIF_DataEnableActiveHigh | kLCDIF_VsyncActiveLow | kLCDIF_HsyncActiveLow | kLCDIF_DriveDataOnRisingClkEdge)

#define DEMO_LCDIF             LCDIF

/* Definitions for MIPI. */
#define DEMO_MIPI_DSI          MIPI_DSI_HOST
#define DEMO_MIPI_DSI_LANE_NUM 2

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void BOARD_PullPanelResetPin(bool pullUp);
static void BOARD_PullPanelPowerPin(bool pullUp);
static void BOARD_InitLcdifClock(void);
static void BOARD_InitMipiDsiClock(void);
static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t mipiDsiTxEscClkFreq_Hz;
static uint32_t mipiDsiDphyBitClkFreq_Hz;
static uint32_t mipiDsiDpiClkFreq_Hz;

#if (DEMO_PANEL == DEMO_PANEL_RK055AHD091)

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = BOARD_DSI_Transfer,
};

static const rm68200_resource_t rm68200Resource = {
    .dsiDevice    = &dsiDevice,
    .pullResetPin = BOARD_PullPanelResetPin,
    .pullPowerPin = BOARD_PullPanelPowerPin,
};

static display_handle_t rm68200Handle = {
    .resource = &rm68200Resource,
    .ops      = &rm68200_ops,
};

#else

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = BOARD_DSI_Transfer,
};

static const rm68191_resource_t rm68191Resource = {
    .dsiDevice    = &dsiDevice,
    .pullResetPin = BOARD_PullPanelResetPin,
    .pullPowerPin = BOARD_PullPanelPowerPin,
};

static display_handle_t rm68191Handle = {
    .resource = &rm68191Resource,
    .ops      = &rm68191_ops,
};

#endif

static dc_fb_lcdif_handle_t s_dcFbLcdifHandle;

static const dc_fb_lcdif_config_t s_dcFbLcdifConfig = {
    .lcdif         = DEMO_LCDIF,
    .width         = DEMO_PANEL_WIDTH,
    .height        = DEMO_PANEL_HEIGHT,
    .hsw           = DEMO_LCDIF_HSW,
    .hfp           = DEMO_LCDIF_HFP,
    .hbp           = DEMO_LCDIF_HBP,
    .vsw           = DEMO_LCDIF_VSW,
    .vfp           = DEMO_LCDIF_VFP,
    .vbp           = DEMO_LCDIF_VBP,
    .polarityFlags = DEMO_LCDIF_POL_FLAGS,
    .outputFormat  = kLCDIF_Output24Bit,
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsLcdif,
    .prvData = &s_dcFbLcdifHandle,
    .config  = &s_dcFbLcdifConfig,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_PullPanelResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_RST_PORT, BOARD_MIPI_RST_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_RST_PORT, BOARD_MIPI_RST_PIN, 0);
    }
}

static void BOARD_PullPanelPowerPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_POWER_PORT, BOARD_MIPI_POWER_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_POWER_PORT, BOARD_MIPI_POWER_PIN, 0);
    }
}

static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(DEMO_MIPI_DSI, xfer);
}

static void BOARD_InitLcdifClock(void)
{
    POWER_DisablePD(kPDRUNCFG_APD_DCNANO_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_DCNANO_SRAM);
    POWER_ApplyPD();

    /*
     * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
     * Here use the aux0 pll (396MHz) as clock source, pixel clock set to 36MHz,
     * then frame rate is:
     *
     * RK055IQH091: 60Hz
     * RK055AHD091: 35Hz
     */
    CLOCK_AttachClk(kAUX0_PLL_to_DCPIXEL_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivDcPixelClk, 11);

    mipiDsiDpiClkFreq_Hz = CLOCK_GetDcPixelClkFreq();

    CLOCK_EnableClock(kCLOCK_DisplayCtrl);
    RESET_ClearPeripheralReset(kDISP_CTRL_RST_SHIFT_RSTn);

    CLOCK_EnableClock(kCLOCK_AxiSwitch);
    RESET_ClearPeripheralReset(kAXI_SWITCH_RST_SHIFT_RSTn);
}

static void BOARD_InitMipiDsiClock(void)
{
    POWER_DisablePD(kPDRUNCFG_APD_MIPIDSI_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_MIPIDSI_SRAM);
    POWER_DisablePD(kPDRUNCFG_PD_MIPIDSI);
    POWER_ApplyPD();

    /* RxClkEsc max 60MHz, TxClkEsc 12 to 20MHz. */
    CLOCK_AttachClk(kFRO_DIV1_to_MIPI_DPHYESC_CLK);
    /* RxClkEsc = 192MHz / 4 = 48MHz. */
    CLOCK_SetClkDiv(kCLOCK_DivDphyEscRxClk, 4);
    /* TxClkEsc = 192MHz / 4 / 3 = 16MHz. */
    CLOCK_SetClkDiv(kCLOCK_DivDphyEscTxClk, 3);
    mipiDsiTxEscClkFreq_Hz = CLOCK_GetMipiDphyEscTxClkFreq();

    /* The DPHY bit clock must be fast enough to send out the pixels, it should be
     * larger than:
     *
     *         (Pixel clock * bit per output pixel) / number of MIPI data lane
     *
     * DPHY supports up to 895.1MHz bit clock. The MIPI panel supports up to 850MHz bit clock.
     */
    /* Use AUX1 PLL clock.
     * AUX1 PLL clock is system pll clock * 18 / pfd.
     * system pll clock is 528MHz defined in clock_config.c
     */
    CLOCK_AttachClk(kAUX1_PLL_to_MIPI_DPHY_CLK);
    CLOCK_InitSysPfd(kCLOCK_Pfd3, 18);
    CLOCK_SetClkDiv(kCLOCK_DivDphyClk, 1);
    mipiDsiDphyBitClkFreq_Hz = CLOCK_GetMipiDphyClkFreq();
}

static status_t BOARD_InitLcdPanel(void)
{
    status_t status;

    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0,
    };

    const display_config_t displayConfig = {
        .resolution   = FSL_VIDEO_RESOLUTION(DEMO_PANEL_WIDTH, DEMO_PANEL_HEIGHT),
        .hsw          = DEMO_LCDIF_HSW,
        .hfp          = DEMO_LCDIF_HFP,
        .hbp          = DEMO_LCDIF_HBP,
        .vsw          = DEMO_LCDIF_VSW,
        .vfp          = DEMO_LCDIF_VFP,
        .vbp          = DEMO_LCDIF_VBP,
        .controlFlags = 0,
        .dsiLanes     = DEMO_MIPI_DSI_LANE_NUM,
    };

    GPIO_PinInit(GPIO, BOARD_MIPI_POWER_PORT, BOARD_MIPI_POWER_PIN, &pinConfig);
    GPIO_PinInit(GPIO, BOARD_MIPI_RST_PORT, BOARD_MIPI_RST_PIN, &pinConfig);
    GPIO_PinInit(GPIO, BOARD_MIPI_BL_PORT, BOARD_MIPI_BL_PIN, &pinConfig);

#if (DEMO_PANEL == DEMO_PANEL_RK055AHD091)
    status = RM68200_Init(&rm68200Handle, &displayConfig);
#else
    status = RM68191_Init(&rm68191Handle, &displayConfig);
#endif

    if (status == kStatus_Success)
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_BL_PORT, BOARD_MIPI_BL_PIN, 1);
    }

    return status;
}

static void BOARD_SetMipiDsiConfig(void)
{
    dsi_config_t dsiConfig;
    dsi_dphy_config_t dphyConfig;

    const dsi_dpi_config_t dpiConfig = {.pixelPayloadSize = DEMO_PANEL_WIDTH,
                                        .dpiColorCoding   = kDSI_Dpi24Bit,
                                        .pixelPacket      = kDSI_PixelPacket24Bit,
                                        .videoMode        = kDSI_DpiBurst,
                                        .bllpMode         = kDSI_DpiBllpLowPower,
                                        .polarityFlags    = kDSI_DpiVsyncActiveLow | kDSI_DpiHsyncActiveLow,
                                        .hfp              = DEMO_LCDIF_HFP,
                                        .hbp              = DEMO_LCDIF_HBP,
                                        .hsw              = DEMO_LCDIF_HSW,
                                        .vfp              = DEMO_LCDIF_VFP,
                                        .vbp              = DEMO_LCDIF_VBP,
                                        .panelHeight      = DEMO_PANEL_HEIGHT,
                                        .virtualChannel   = 0};

    /*
     * dsiConfig.numLanes = 4;
     * dsiConfig.enableNonContinuousHsClk = false;
     * dsiConfig.autoInsertEoTp = true;
     * dsiConfig.numExtraEoTp = 0;
     * dsiConfig.htxTo_ByteClk = 0;
     * dsiConfig.lrxHostTo_ByteClk = 0;
     * dsiConfig.btaTo_ByteClk = 0;
     */
    DSI_GetDefaultConfig(&dsiConfig);
    dsiConfig.numLanes       = DEMO_MIPI_DSI_LANE_NUM;
    dsiConfig.autoInsertEoTp = true;

    DSI_GetDphyDefaultConfig(&dphyConfig, mipiDsiDphyBitClkFreq_Hz, mipiDsiTxEscClkFreq_Hz);

    /* Init the DSI module. */
    DSI_Init(DEMO_MIPI_DSI, &dsiConfig);

    /* Init DPHY. There is not DPHY PLL, the ref clock is not used. */
    DSI_InitDphy(DEMO_MIPI_DSI, &dphyConfig, 0);

    /* Init DPI interface. */
    DSI_SetDpiConfig(DEMO_MIPI_DSI, &dpiConfig, DEMO_MIPI_DSI_LANE_NUM, mipiDsiDpiClkFreq_Hz, mipiDsiDphyBitClkFreq_Hz);
}

status_t BOARD_InitDisplayInterface(void)
{
    /* 1. Assert MIPI DPHY reset. */
    RESET_SetPeripheralReset(kMIPI_DSI_PHY_RST_SHIFT_RSTn);

    /* 2. Setup clock. */
    BOARD_InitMipiDsiClock();

    /* 3. Configures peripheral. */
    RESET_ClearPeripheralReset(kMIPI_DSI_CTRL_RST_SHIFT_RSTn);
    BOARD_SetMipiDsiConfig();

    /* 4. Deassert reset. */
    RESET_ClearPeripheralReset(kMIPI_DSI_PHY_RST_SHIFT_RSTn);

    /* 5. Configure the panel. */
    return BOARD_InitLcdPanel();
}

void LCDIF_IRQHandler(void)
{
    DC_FB_LCDIF_IRQHandler(&g_dc);
}

status_t BOARD_PrepareDisplayController(void)
{
    status_t status;

    BOARD_InitLcdifClock();

    status = BOARD_InitDisplayInterface();

    if (kStatus_Success == status)
    {
        NVIC_SetPriority(LCDIF_IRQn, 3);
        EnableIRQ(LCDIF_IRQn);
    }

    return kStatus_Success;
}

#elif (DEMO_PANEL_RM67162 == DEMO_PANEL)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * RM67162 panel
 */

/* Definitions for MIPI. */
#define DEMO_MIPI_DSI          MIPI_DSI_HOST
#define DEMO_MIPI_DSI_LANE_NUM 1
#define DEMO_MIPI_DSI_IRQn     MIPI_IRQn

/*
 * The max TX array size:
 *
 * 1. One byte in FIFO is reserved for DSC command
 * 2. One pixel should not be split to two transfer.
 */
#define DEMO_DSI_TX_ARRAY_MAX \
    (((FSL_DSI_TX_MAX_PAYLOAD_BYTE - 1U) / DEMO_BUFFER_BYTE_PER_PIXEL) * DEMO_BUFFER_BYTE_PER_PIXEL)

typedef struct _dsi_mem_write_ctx
{
    volatile bool onGoing;
    const uint8_t *txData;
    uint32_t leftByteLen;
    uint8_t dscCmd;
} dsi_mem_write_ctx_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void BOARD_PullPanelResetPin(bool pullUp);
static void BOARD_PullPanelPowerPin(bool pullUp);
static void BOARD_InitMipiDsiClock(void);
static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer);
static status_t BOARD_DSI_MemWrite(uint8_t virtualChannel, const uint8_t *data, uint32_t length);
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if DEMO_RM67162_USE_DSI_SMARTDMA
static dsi_smartdma_handle_t s_dsiDriverHandle;
#else
static dsi_mem_write_ctx_t s_dsiMemWriteCtx;
static dsi_transfer_t s_dsiMemWriteXfer = {0};
static dsi_handle_t s_dsiDriverHandle;
static uint8_t s_dsiMemWriteTmpArray[DEMO_DSI_TX_ARRAY_MAX];
#endif

static uint32_t mipiDsiTxEscClkFreq_Hz;
static uint32_t mipiDsiDphyBitClkFreq_Hz;

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = BOARD_DSI_Transfer,
    .memWriteFunc   = BOARD_DSI_MemWrite,
};

static const rm67162_resource_t rm67162Resource = {
    .dsiDevice    = &dsiDevice,
    .pullResetPin = BOARD_PullPanelResetPin,
    .pullPowerPin = BOARD_PullPanelPowerPin,
};

static display_handle_t rm67162Handle = {
    .resource = &rm67162Resource,
    .ops      = &rm67162_ops,
};

const dc_fb_dsi_cmd_config_t s_panelConfig = {
    .commonConfig =
        {
            .resolution   = FSL_VIDEO_RESOLUTION(DEMO_PANEL_WIDTH, DEMO_PANEL_HEIGHT),
            .hsw          = 0, /* Not used. */
            .hfp          = 0, /* Not used. */
            .hbp          = 0, /* Not used. */
            .vsw          = 0, /* Not used. */
            .vfp          = 0, /* Not used. */
            .vbp          = 0, /* Not used. */
            .controlFlags = 0,
            .dsiLanes     = DEMO_MIPI_DSI_LANE_NUM,
            .pixelFormat  = DEMO_BUFFER_PIXEL_FORMAT,
        },

    .useTEPin = true,
};

static dc_fb_dsi_cmd_handle_t s_dcFbDsiCmdHandle = {
    .dsiDevice   = &dsiDevice,
    .panelHandle = &rm67162Handle,
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsDsiCmd,
    .prvData = &s_dcFbDsiCmdHandle,
    .config  = &s_panelConfig,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_PullPanelResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_RST_PORT, BOARD_MIPI_RST_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_RST_PORT, BOARD_MIPI_RST_PIN, 0);
    }
}

static void BOARD_PullPanelPowerPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_POWER_PORT, BOARD_MIPI_POWER_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_POWER_PORT, BOARD_MIPI_POWER_PIN, 0);
    }
}

static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(DEMO_MIPI_DSI, xfer);
}

#if !DEMO_RM67162_USE_DSI_SMARTDMA
static status_t BOARD_DsiMemWriteSendChunck(void)
{
    uint32_t curSendLen;
    uint32_t i;

    curSendLen =
        DEMO_DSI_TX_ARRAY_MAX > s_dsiMemWriteCtx.leftByteLen ? s_dsiMemWriteCtx.leftByteLen : DEMO_DSI_TX_ARRAY_MAX;

    s_dsiMemWriteXfer.txDataType = kDSI_TxDataDcsLongWr;
    s_dsiMemWriteXfer.dscCmd     = s_dsiMemWriteCtx.dscCmd;
    s_dsiMemWriteXfer.txData     = s_dsiMemWriteTmpArray;
    s_dsiMemWriteXfer.txDataSize = curSendLen;

    /* For each pixel, the MIPI DSI sends out low byte first, but according to
     * the MIPI DSC spec, the high byte should be send first, so swap the pixel byte
     * first.
     */
#if (DEMO_RM67162_BUFFER_FORMAT == DEMO_RM67162_BUFFER_RGB565)
    for (i = 0; i < curSendLen; i += 2)
    {
        s_dsiMemWriteTmpArray[i]     = *(s_dsiMemWriteCtx.txData + 1);
        s_dsiMemWriteTmpArray[i + 1] = *(s_dsiMemWriteCtx.txData);

        s_dsiMemWriteCtx.txData += 2;
    }
#else
    for (i = 0; i < curSendLen; i += 3)
    {
        s_dsiMemWriteTmpArray[i]     = *(s_dsiMemWriteCtx.txData + 2);
        s_dsiMemWriteTmpArray[i + 1] = *(s_dsiMemWriteCtx.txData + 1);
        s_dsiMemWriteTmpArray[i + 2] = *(s_dsiMemWriteCtx.txData);

        s_dsiMemWriteCtx.txData += 3;
    }
#endif

    s_dsiMemWriteCtx.leftByteLen -= curSendLen;
    s_dsiMemWriteCtx.dscCmd = kMIPI_DCS_WriteMemoryContinue;

    return DSI_TransferNonBlocking(DEMO_MIPI_DSI, &s_dsiDriverHandle, &s_dsiMemWriteXfer);
}

static void BOARD_DsiMemWriteCallback(MIPI_DSI_HOST_Type *base, dsi_handle_t *handle, status_t status, void *userData)
{
    if ((kStatus_Success == status) && (s_dsiMemWriteCtx.leftByteLen > 0))
    {
        status = BOARD_DsiMemWriteSendChunck();

        if (kStatus_Success == status)
        {
            return;
        }
    }

    s_dsiMemWriteCtx.onGoing = false;
    MIPI_DSI_MemoryDoneDriverCallback(status, &dsiDevice);
}
#else
static void BOARD_DsiMemWriteCallback(MIPI_DSI_HOST_Type *base,
                                      dsi_smartdma_handle_t *handle,
                                      status_t status,
                                      void *userData)
{
    MIPI_DSI_MemoryDoneDriverCallback(status, &dsiDevice);
}
#endif

static status_t BOARD_DSI_MemWrite(uint8_t virtualChannel, const uint8_t *data, uint32_t length)
{
#if DEMO_RM67162_USE_DSI_SMARTDMA
    dsi_smartdma_write_mem_transfer_t xfer = {
#if (DEMO_RM67162_BUFFER_FORMAT == DEMO_RM67162_BUFFER_RGB565)
        .inputFormat  = kDSI_SMARTDMA_InputPixelFormatRGB565,
        .outputFormat = kDSI_SMARTDMA_OutputPixelFormatRGB565,
#elif (DEMO_RM67162_BUFFER_FORMAT == DEMO_RM67162_BUFFER_RGB888)
        .inputFormat  = kDSI_SMARTDMA_InputPixelFormatRGB888,
        .outputFormat = kDSI_SMARTDMA_OutputPixelFormatRGB888,
#else
        .inputFormat  = kDSI_SMARTDMA_InputPixelFormatXRGB8888,
        .outputFormat = kDSI_SMARTDMA_OutputPixelFormatRGB888,
#endif /* DEMO_RM67162_BUFFER_FORMAT */
        .data         = data,
        .dataSize     = length,

        .virtualChannel       = virtualChannel,
        .disablePixelByteSwap = false,
    };

    return DSI_TransferWriteMemorySMARTDMA(DEMO_MIPI_DSI, &s_dsiDriverHandle, &xfer);

#else /* DEMO_RM67162_USE_DSI_SMARTDMA */

    status_t status;

    if (s_dsiMemWriteCtx.onGoing)
    {
        return kStatus_Fail;
    }

    s_dsiMemWriteXfer.virtualChannel = virtualChannel;
    s_dsiMemWriteXfer.flags          = kDSI_TransferUseHighSpeed;
    s_dsiMemWriteXfer.sendDscCmd     = true;

    s_dsiMemWriteCtx.onGoing     = true;
    s_dsiMemWriteCtx.txData      = data;
    s_dsiMemWriteCtx.leftByteLen = length;
    s_dsiMemWriteCtx.dscCmd      = kMIPI_DCS_WriteMemoryStart;

    status = BOARD_DsiMemWriteSendChunck();

    if (status != kStatus_Success)
    {
        /* Memory write does not start actually. */
        s_dsiMemWriteCtx.onGoing = false;
    }

    return status;
#endif
}

static void BOARD_InitMipiDsiClock(void)
{
    POWER_DisablePD(kPDRUNCFG_APD_MIPIDSI_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_MIPIDSI_SRAM);
    POWER_DisablePD(kPDRUNCFG_PD_MIPIDSI);
    POWER_ApplyPD();

    /* RxClkEsc max 60MHz, TxClkEsc 12 to 20MHz. */
    CLOCK_AttachClk(kFRO_DIV1_to_MIPI_DPHYESC_CLK);
    /* RxClkEsc = 192MHz / 4 = 48MHz. */
    CLOCK_SetClkDiv(kCLOCK_DivDphyEscRxClk, 4);
    /* TxClkEsc = 192MHz / 4 / 3 = 16MHz. */
    CLOCK_SetClkDiv(kCLOCK_DivDphyEscTxClk, 3);
    mipiDsiTxEscClkFreq_Hz = CLOCK_GetMipiDphyEscTxClkFreq();

    /*
     * DPHY supports up to 895.1MHz bit clock.
     * When choose the DPHY clock frequency, consider the panel frame rate and
     * resolution.
     *
     * RM67162 controller maximum total bit rate is:
     *  - 500Mbps of 2 data lanes 24-bit data format
     *  - 360Mbps of 2 data lanes 18-bit data format
     *  - 320Mbps of 2 data lanes 16-bit data format
     *
     * Here the clock is set to maximum value for better performance.
     *
     * Here use AUX1 PLL clock.
     * AUX1 PLL clock is system pll clock * 18 / pfd.
     * system pll clock is 528MHz defined in clock_config.c
     */
    CLOCK_AttachClk(kAUX1_PLL_to_MIPI_DPHY_CLK);
#if (DEMO_RM67162_BUFFER_FORMAT == DEMO_RM67162_BUFFER_RGB565)
    /* PFD value is in the range of 12~35. */
    CLOCK_InitSysPfd(kCLOCK_Pfd3, 30);
#else
    /* PFD value is in the range of 12~35. */
    CLOCK_InitSysPfd(kCLOCK_Pfd3, 19);
#endif
    CLOCK_SetClkDiv(kCLOCK_DivDphyClk, 1);
    mipiDsiDphyBitClkFreq_Hz = CLOCK_GetMipiDphyClkFreq();
}

static void BOARD_InitMipiPanelTEPin(void)
{
    const gpio_pin_config_t tePinConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0,
    };

    /*
     * TE pin configure method:
     *
     * The TE pin interrupt is like this:
     *
     *            VSYNC
     *         +--------+
     *         |        |
     *         |        |
     * --------+        +----------------
     *
     * 1. If one frame send time is shorter than one frame refresh time, then set
     *    TE pin interrupt at the start of VSYNC.
     * 2. If one frame send time is longer than one frame refresh time, and shorter
     *    than two frames refresh time, then set TE pin interrupt at the end of VSYNC.
     * 3. If one frame send time is longer than two frame refresh time, tearing effect
     *    could not be removed.
     *
     * For RM67162 @60Hz frame rate, frame refresh time is 16.7 ms. After test,
     * one frame send time is shorter than one frame refresh time. So TE interrupt is
     * set to start of VSYNC.
     */

    gpio_interrupt_config_t tePinIntConfig = {kGPIO_PinIntEnableEdge, kGPIO_PinIntEnableHighOrRise};

    GPIO_PinInit(GPIO, BOARD_MIPI_TE_PORT, BOARD_MIPI_TE_PIN, &tePinConfig);

    GPIO_SetPinInterruptConfig(GPIO, BOARD_MIPI_TE_PORT, BOARD_MIPI_TE_PIN, &tePinIntConfig);

    GPIO_PinEnableInterrupt(GPIO, BOARD_MIPI_TE_PORT, BOARD_MIPI_TE_PIN, 0);

    NVIC_SetPriority(GPIO_INTA_IRQn, 3);

    NVIC_EnableIRQ(GPIO_INTA_IRQn);
}

static status_t BOARD_InitLcdPanel(void)
{
    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0,
    };

    GPIO_PinInit(GPIO, BOARD_MIPI_POWER_PORT, BOARD_MIPI_POWER_PIN, &pinConfig);
    GPIO_PinInit(GPIO, BOARD_MIPI_RST_PORT, BOARD_MIPI_RST_PIN, &pinConfig);

    BOARD_InitMipiPanelTEPin();

    return kStatus_Success;
}

/* Smart panel TE pin IRQ handler. */
void BOARD_DisplayTEPinHandler(void)
{
    DC_FB_DSI_CMD_TE_IRQHandler(&g_dc);
}

static void BOARD_SetMipiDsiConfig(void)
{
    dsi_config_t dsiConfig;
    dsi_dphy_config_t dphyConfig;

    /*
     * dsiConfig.numLanes = 4;
     * dsiConfig.enableNonContinuousHsClk = false;
     * dsiConfig.autoInsertEoTp = true;
     * dsiConfig.numExtraEoTp = 0;
     * dsiConfig.htxTo_ByteClk = 0;
     * dsiConfig.lrxHostTo_ByteClk = 0;
     * dsiConfig.btaTo_ByteClk = 0;
     */
    DSI_GetDefaultConfig(&dsiConfig);
    dsiConfig.numLanes       = DEMO_MIPI_DSI_LANE_NUM;
    dsiConfig.autoInsertEoTp = true;

    DSI_GetDphyDefaultConfig(&dphyConfig, mipiDsiDphyBitClkFreq_Hz, mipiDsiTxEscClkFreq_Hz);

    /* Init the DSI module. */
    DSI_Init(DEMO_MIPI_DSI, &dsiConfig);

    /* Init DPHY. There is not DPHY PLL, the ref clock is not used. */
    DSI_InitDphy(DEMO_MIPI_DSI, &dphyConfig, 0);
}

status_t BOARD_InitDisplayInterface(void)
{
    /* 1. Assert MIPI DPHY reset. */
    RESET_SetPeripheralReset(kMIPI_DSI_PHY_RST_SHIFT_RSTn);

    /* 2. Setup clock. */
    BOARD_InitMipiDsiClock();

    /* 3. Configures peripheral. */
    RESET_ClearPeripheralReset(kMIPI_DSI_CTRL_RST_SHIFT_RSTn);
    BOARD_SetMipiDsiConfig();

    /* 4. Deassert reset. */
    RESET_ClearPeripheralReset(kMIPI_DSI_PHY_RST_SHIFT_RSTn);

    /* 5. Configure the panel. */
    return BOARD_InitLcdPanel();
}

status_t BOARD_PrepareDisplayController(void)
{
    status_t status;

    status = BOARD_InitDisplayInterface();

#if DEMO_RM67162_USE_DSI_SMARTDMA
    if (kStatus_Success == status)
    {
        BOARD_InitSmartDMA();
        status = DSI_TransferCreateHandleSMARTDMA(DEMO_MIPI_DSI, &s_dsiDriverHandle, BOARD_DsiMemWriteCallback, NULL);
    }

    return status;

#else

    if (kStatus_Success == status)
    {
        /*
         * Suggest setting to low priority. Because a new DSI transfer is prepared
         * in the callback BOARD_DsiMemWriteCallback, so the core spends more time
         * in ISR. Setting the low priority, then the important ISR won't be blocked.
         */
        NVIC_SetPriority(DEMO_MIPI_DSI_IRQn, 6);
    }
    else
    {
        return status;
    }

    memset(&s_dsiMemWriteCtx, 0, sizeof(dsi_mem_write_ctx_t));

    /* Create the MIPI DSI trasnfer handle for non-blocking data trasnfer. */
    return DSI_TransferCreateHandle(DEMO_MIPI_DSI, &s_dsiDriverHandle, BOARD_DsiMemWriteCallback, NULL);
#endif
}

#endif
