/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "svc/misc/misc_api.h"
#include "fsl_irqsteer.h"
#include "fsl_video_common.h"
#include "fsl_display.h"
#include "isi_config.h"
#include "isi_example.h"
#include "fsl_irqsteer.h"
#include "fsl_dpu_irqsteer.h"
#include "fsl_debug_console.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_isi_camera_adapter.h"
#include "fsl_camera_device.h"

#if (!defined(CAMERA_DEVICE)) || (CAMERA_DEVICE == CAMERA_DEVICE_OV5640)
#include "fsl_ov5640.h"
#elif (CAMERA_DEVICE == CAMERA_DEVICE_MAX9286)
#include "fsl_max9286.h"
#endif
#include "fsl_mipi_csi2rx.h"
#if DPU_EXAMPLE_DI == DPU_DI_MIPI
#if !APP_DISPLAY_EXTERNAL_CONVERTOR
#include "fsl_mipi_dsi.h"
#include "fsl_rm67191.h"
#else
#include "fsl_adv7535.h"
#endif
#elif DPU_EXAMPLE_DI == DPU_DI_LVDS
#include "fsl_ldb.h"
#include "fsl_it6263.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * Camera reset pin and power down pin:
 *
 * MIPI CSI 0:
 * Reset Pin: MIPI_CSI0_GPIO0_00
 * Power Down Pin: MIPI_CSI0_GPIO0_01
 *
 * MIPI CSI 1:
 * Reset Pin: MIPI_CSI1_GPIO0_00
 * Power Down Pin: MIPI_CSI1_GPIO0_01
 */
#if (APP_MIPI_CSI2_BASE == MIPI_CSI_0__MIPI_CSI2RX_BASE)
#define BOARD_Camera_I2C_Init BOARD_Camera0_I2C_Init
#define BOARD_Camera_I2C_Deinit BOARD_Camera0_I2C_Deinit
#define BOARD_Camera_I2C_SendSCCB BOARD_Camera0_I2C_SendSCCB
#define BOARD_Camera_I2C_ReceiveSCCB BOARD_Camera0_I2C_ReceiveSCCB
#define MIPI_CSI_RSRC SC_R_CSI_0
#define CAMERA_GPIO MIPI_CSI_0__GPIO
#define CAMERA_RST_PIN_IDX 0
#define CAMERA_PD_PIN_IDX 1
#else
#define BOARD_Camera_I2C_Init BOARD_Camera1_I2C_Init
#define BOARD_Camera_I2C_Deinit BOARD_Camera1_I2C_Deinit
#define BOARD_Camera_I2C_SendSCCB BOARD_Camera1_I2C_SendSCCB
#define BOARD_Camera_I2C_ReceiveSCCB BOARD_Camera1_I2C_ReceiveSCCB
#define MIPI_CSI_RSRC SC_R_CSI_1
#define CAMERA_GPIO MIPI_CSI_1__GPIO
#define CAMERA_RST_PIN_IDX 0
#define CAMERA_PD_PIN_IDX 1
#endif

#if (DPU_EXAMPLE_DI == DPU_DI_LVDS) && (!APP_DISPLAY_EXTERNAL_CONVERTOR)
#error LDB only support external convertor
#endif

#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)

#if (APP_MIPI_DSI_BASE == MIPI_DSI_HOST0_BASE)
#define BOARD_Display_I2C_Init BOARD_Display0_I2C_Init
#define BOARD_Display_I2C_Send BOARD_Display0_I2C_Send
#define BOARD_Display_I2C_Receive BOARD_Display0_I2C_Receive
#define MIPI_DSI_RSRC SC_R_MIPI_0
#else /* (APP_MIPI_DSI_BASE == MIPI_DSI_HOST0_BASE) */
#define BOARD_Display_I2C_Init BOARD_Display1_I2C_Init
#define BOARD_Display_I2C_Send BOARD_Display1_I2C_Send
#define BOARD_Display_I2C_Receive BOARD_Display1_I2C_Receive
#define MIPI_DSI_RSRC SC_R_MIPI_1
#endif /* (APP_MIPI_DSI_BASE == MIPI_DSI_HOST0_BASE) */

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)

#if (APP_LDB_BASE == DI_LVDS_0__LDB_BASE)
#if (APP_LDB_CH == 0)
#define BOARD_Display_I2C_Init BOARD_Display2_I2C_Init
#define BOARD_Display_I2C_Send BOARD_Display2_I2C_Send
#define BOARD_Display_I2C_Receive BOARD_Display2_I2C_Receive
#else
#define BOARD_Display_I2C_Init BOARD_Display3_I2C_Init
#define BOARD_Display_I2C_Send BOARD_Display3_I2C_Send
#define BOARD_Display_I2C_Receive BOARD_Display3_I2C_Receive
#endif
#define LDB_RSRC SC_R_LVDS_0
#else /* APP_LDB_BASE == DI_LVDS_0__LDB_BASE) */
#if (APP_LDB_CH == 0)
#define BOARD_Display_I2C_Init BOARD_Display4_I2C_Init
#define BOARD_Display_I2C_Send BOARD_Display4_I2C_Send
#define BOARD_Display_I2C_Receive BOARD_Display4_I2C_Receive
#else
#define BOARD_Display_I2C_Init BOARD_Display5_I2C_Init
#define BOARD_Display_I2C_Send BOARD_Display5_I2C_Send
#define BOARD_Display_I2C_Receive BOARD_Display5_I2C_Receive
#endif
#define LDB_RSRC SC_R_LVDS_1
#endif /* (APP_LDB_BASE == DI_LVDS_0__LDB_BASE) */

#endif /*DPU_EXAMPLE_DI */

#if (APP_DPU_BASE == DC_0__IRIS_MVPL_BASE)

#define DC_RSRC SC_R_DC_0

#if (0 == APP_DPU_DISPLAY_INDEX)
#define DC_PLL_RSRC SC_R_DC_0_PLL_0
#else /* (0 == APP_DPU_DISPLAY_INDEX) */
#define DC_PLL_RSRC SC_R_DC_0_PLL_1
#endif /* (0 == APP_DPU_DISPLAY_INDEX) */

#else /* (APP_DPU_BASE == DC_0__IRIS_MVPL_BASE) */

#define DC_RSRC SC_R_DC_1

#if (0 == APP_DPU_DISPLAY_INDEX)
#define DC_PLL_RSRC SC_R_DC_1_PLL_0
#else /* (0 == APP_DPU_DISPLAY_INDEX) */
#define DC_PLL_RSRC SC_R_DC_1_PLL_1
#endif /* (0 == APP_DPU_DISPLAY_INDEX) */

#endif /* (APP_DPU_BASE == DC_0__IRIS_MVPL_BASE) */

#if (0 == APP_DPU_DISPLAY_INDEX)
#define DC_DISPLAY_CLOCK SC_PM_CLK_MISC0
#else /* (0 == APP_DPU_DISPLAY_INDEX) */
#define DC_DISPLAY_CLOCK SC_PM_CLK_MISC1
#endif /* (0 == APP_DPU_DISPLAY_INDEX) */

#if (APP_ISI_BASE == IMAGING__ISI0_BASE)
#define ISI_RSRC SC_R_ISI_CH0
#define APP_ISI_IRQn IMAGING_PDMA_STREAM0_INT_IRQn
#define APP_ISI_IRQHandler IMAGING_PDMA_STREAM0_INT_IRQHandler
#elif (APP_ISI_BASE == IMAGING__ISI1_BASE)
#define ISI_RSRC SC_R_ISI_CH1
#define APP_ISI_IRQn IMAGING_PDMA_STREAM1_INT_IRQn
#define APP_ISI_IRQHandler IMAGING_PDMA_STREAM1_INT_IRQHandler
#elif (APP_ISI_BASE == IMAGING__ISI2_BASE)
#define ISI_RSRC SC_R_ISI_CH2
#define APP_ISI_IRQn IMAGING_PDMA_STREAM2_INT_IRQn
#define APP_ISI_IRQHandler IMAGING_PDMA_STREAM2_INT_IRQHandler
#elif (APP_ISI_BASE == IMAGING__ISI3_BASE)
#define ISI_RSRC SC_R_ISI_CH3
#define APP_ISI_IRQn IMAGING_PDMA_STREAM3_INT_IRQn
#define APP_ISI_IRQHandler IMAGING_PDMA_STREAM3_INT_IRQHandler
#elif (APP_ISI_BASE == IMAGING__ISI4_BASE)
#define ISI_RSRC SC_R_ISI_CH4
#define APP_ISI_IRQn IMAGING_PDMA_STREAM4_INT_IRQn
#define APP_ISI_IRQHandler IMAGING_PDMA_STREAM4_INT_IRQHandler
#elif (APP_ISI_BASE == IMAGING__ISI5_BASE)
#define ISI_RSRC SC_R_ISI_CH5
#define APP_ISI_IRQn IMAGING_PDMA_STREAM5_INT_IRQn
#define APP_ISI_IRQHandler IMAGING_PDMA_STREAM5_INT_IRQHandler
#elif (APP_ISI_BASE == IMAGING__ISI6_BASE)
#define ISI_RSRC SC_R_ISI_CH6
#define APP_ISI_IRQn IMAGING_PDMA_STREAM6_INT_IRQn
#define APP_ISI_IRQHandler IMAGING_PDMA_STREAM6_INT_IRQHandler
#else
#define ISI_RSRC SC_R_ISI_CH7
#define APP_ISI_IRQn IMAGING_PDMA_STREAM7_INT_IRQn
#define APP_ISI_IRQHandler IMAGING_PDMA_STREAM7_INT_IRQHandler
#endif
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)

#if !APP_DISPLAY_EXTERNAL_CONVERTOR
#define RM67191_RESET_GPIO LSIO__GPIO1
#define RM67191_RESET_PIN 7U
#endif

/* Pixel Link address */
typedef struct _dpu_mipi_pl_addr
{
    IRIS_MVPL_Type *dpu;
    uint8_t dpuDisplayIndex;
    MIPI_DSI_HOST_Type *dsi;
    uint8_t plAddr;
} dpu_mipi_pl_addr_t;

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)

#define IT6263_RESET_GPIO LSIO__GPIO1
#define IT6263_RESET_PIN 6U

/* Pixel Link address */
typedef struct _dpu_ldb_pl_addr
{
    IRIS_MVPL_Type *dpu;
    uint8_t dpuDisplayIndex;
    LDB_Type *ldb;
    uint8_t plAddr;
} dpu_ldb_pl_addr_t;

#endif /* DPU_EXAMPLE_DI */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void APP_DisplayIRQHandler(void);
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)

#if !APP_DISPLAY_EXTERNAL_CONVERTOR
static void RM67191_PullResetPin(bool pullUp);
status_t RM67191_DSI_Transfer(dsi_transfer_t *xfer);
#endif

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
static void IT6263_PullResetPin(bool pullUp);
#endif /* DPU_EXAMPLE_DI */

/*******************************************************************************
 * Variables
 ******************************************************************************/
isi_private_data_t isiPrivateData;

isi_resource_t isiResource = {
    .isiBase = APP_ISI,
/*
 * ISI port definition:
 *
 * 0 - DC 0
 * 1 - DC 1
 * 2 - MIPI CSI2 0
 * 3 - MIPI CSI2 1
 * 4 - HDMI
 * 5 - memory
 */
#if (APP_MIPI_CSI2_BASE == MIPI_CSI_0__MIPI_CSI2RX_BASE)
    .isiInputPort = 2,
#else
    .isiInputPort = 3,
#endif
};

camera_receiver_handle_t cameraReceiver = {
    .resource    = &isiResource,
    .ops         = &isi_ops,
    .privateData = &isiPrivateData,
};

static void APP_PullCameraResetPin(bool pullUp);
#if (!defined(CAMERA_DEVICE)) || (CAMERA_DEVICE == CAMERA_DEVICE_OV5640)
static void APP_PullCameraPowerDownPin(bool pullUp);
#endif

#if (!defined(CAMERA_DEVICE)) || (CAMERA_DEVICE == CAMERA_DEVICE_OV5640)
ov5640_resource_t ov5640Resource = {
    .i2cSendFunc      = BOARD_Camera_I2C_SendSCCB,
    .i2cReceiveFunc   = BOARD_Camera_I2C_ReceiveSCCB,
    .pullResetPin     = APP_PullCameraResetPin,
    .pullPowerDownPin = APP_PullCameraPowerDownPin,
};

camera_device_handle_t cameraDevice = {
    .resource = &ov5640Resource,
    .ops      = &ov5640_ops,
};
#elif (CAMERA_DEVICE == CAMERA_DEVICE_MAX9286)
max9286_resource_t max9286Resource = {
    .i2cSendFunc      = BOARD_Camera_I2C_SendSCCB,
    .i2cReceiveFunc   = BOARD_Camera_I2C_ReceiveSCCB,
    .pullPowerDownPin = APP_PullCameraResetPin,
};

camera_device_handle_t cameraDevice = {
    .resource = &max9286Resource,
    .ops      = &max9286_ops,
};
#endif

#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)

#if !APP_DISPLAY_EXTERNAL_CONVERTOR
static const rm67191_resource_t rm67191Resource = {
    .dsiDevice =
        {
            .virtualChannel = 0,
            .xferFunc       = RM67191_DSI_Transfer,
        },
    .pullResetPin = RM67191_PullResetPin,
};

static display_handle_t rm67191Handle = {
    .resource = &rm67191Resource,
    .ops      = &rm67191_ops,
};
#endif

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)

static it6263_resource_t it6263Resource = {
    .i2cAddr        = 0x98,
    .pullResetPin   = IT6263_PullResetPin,
    .i2cSendFunc    = BOARD_Display_I2C_Send,
    .i2cReceiveFunc = BOARD_Display_I2C_Receive,
};

static display_handle_t it6263Handle = {
    .resource = &it6263Resource,
    .ops      = &it6263_ops,
};
#endif /* DPU_EXAMPLE_DI  */

static sc_rm_pt_t isi_pt, cm4_pt;
static sc_rm_mr_t isi_mr;
/*******************************************************************************
 * Codes
 ******************************************************************************/
/*
 * The DPU output interrupts are:
 *
 * INT_OUT [0] - common
 * INT_OUT [1] - display 0 A
 * INT_OUT [2] - display 0 B (a duplicate to be sent to Coretex-M core)
 * INT_OUT [3] - display 1 A
 * INT_OUT [4] - display 1 B
 * INT_OUT [5] - Reserved
 * INT_OUT [6] - Reserved
 * INT_OUT [7] - BLIT
 *
 * Here use display 0 B and display 1 B.
 */
#if (APP_DPU_BASE == DC_0__IRIS_MVPL_BASE)

#if (0 == APP_DPU_DISPLAY_INDEX)
void DISPLAY0_INT_OUT2_IRQHandler(void)
{
    APP_DisplayIRQHandler();
}
#else
void DISPLAY0_INT_OUT4_IRQHandler(void)
{
    APP_DisplayIRQHandler();
}
#endif

#else

#if (0 == APP_DPU_DISPLAY_INDEX)
void DISPLAY1_INT_OUT2_IRQHandler(void)
{
    APP_DisplayIRQHandler();
}
#else
void DISPLAY1_INT_OUT4_IRQHandler(void)
{
    APP_DisplayIRQHandler();
}
#endif

#endif

void APP_ISI_IRQHandler(void)
{
    ISI_ADAPTER_IRQHandler(&cameraReceiver);
}

void SOC_InitDpuInterrupt(void)
{
#if (APP_DPU_BASE == DC_0__IRIS_MVPL_BASE)

#if (0 == APP_DPU_DISPLAY_INDEX)
    IRQSTEER_EnableInterrupt(IRQSTEER, DISPLAY0_INT_OUT2_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU0_IRQSTEER, ExtDst0ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU0_IRQSTEER, ExtDst4ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU0_IRQSTEER, Display0ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU0_IRQSTEER, Sig0ShadowLoad_DPU_IRQn);
#else
    IRQSTEER_EnableInterrupt(IRQSTEER, DISPLAY0_INT_OUT4_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU0_IRQSTEER, ExtDst1ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU0_IRQSTEER, ExtDst5ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU0_IRQSTEER, Display1ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU0_IRQSTEER, Sig1ShadowLoad_DPU_IRQn);
#endif

#else

#if (0 == APP_DPU_DISPLAY_INDEX)
    IRQSTEER_EnableInterrupt(IRQSTEER, DISPLAY1_INT_OUT2_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU1_IRQSTEER, ExtDst0ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU1_IRQSTEER, ExtDst4ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU1_IRQSTEER, Display0ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU1_IRQSTEER, Sig0ShadowLoad_DPU_IRQn);
#else
    IRQSTEER_EnableInterrupt(IRQSTEER, DISPLAY1_INT_OUT4_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU1_IRQSTEER, ExtDst1ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU1_IRQSTEER, ExtDst5ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU1_IRQSTEER, Display1ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_EnableInterrupt(DPU1_IRQSTEER, Sig1ShadowLoad_DPU_IRQn);
#endif

#endif
}

static void SOC_DeinitDpuInterrupt(void)
{
#if (APP_DPU_BASE == DC_0__IRIS_MVPL_BASE)

#if (0 == APP_DPU_DISPLAY_INDEX)
    IRQSTEER_DisableInterrupt(IRQSTEER, DISPLAY0_INT_OUT2_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU0_IRQSTEER, ExtDst0ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU0_IRQSTEER, ExtDst4ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU0_IRQSTEER, Display0ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU0_IRQSTEER, Sig0ShadowLoad_DPU_IRQn);
#else
    IRQSTEER_DisableInterrupt(IRQSTEER, DISPLAY0_INT_OUT4_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU0_IRQSTEER, ExtDst1ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU0_IRQSTEER, ExtDst5ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU0_IRQSTEER, Display1ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU0_IRQSTEER, Sig1ShadowLoad_DPU_IRQn);
#endif

#else

#if (0 == APP_DPU_DISPLAY_INDEX)
    IRQSTEER_DisableInterrupt(IRQSTEER, DISPLAY1_INT_OUT2_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU1_IRQSTEER, ExtDst0ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU1_IRQSTEER, ExtDst4ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU1_IRQSTEER, Display0ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU1_IRQSTEER, Sig0ShadowLoad_DPU_IRQn);
#else
    IRQSTEER_DisableInterrupt(IRQSTEER, DISPLAY1_INT_OUT4_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU1_IRQSTEER, ExtDst1ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU1_IRQSTEER, ExtDst5ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU1_IRQSTEER, Display1ShadowLoad_DPU_IRQn);
    DPU_IRQSTEER_DisableInterrupt(DPU1_IRQSTEER, Sig1ShadowLoad_DPU_IRQn);
#endif

#endif
}

#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
status_t SOC_SetDpuMipiDsiPixelLink(sc_ipc_t ipc, IRIS_MVPL_Type *dpu, uint8_t displayIndex, MIPI_DSI_HOST_Type *dsi)
{
    /*
     * Pixel link setting.
     *
     * DPU 0 display0 is connected to MIPI DSI 0 using address 0.
     * DPU 1 display0 is connected to MIPI DSI 1 using address 0.
     *
     * SC_R_DC_0    SC_C_PXL_LINK_MST1_ADDR   Master 1 pixel link address
     * SC_R_DC_0    SC_C_PXL_LINK_MST1_ENB    Master 1 pixel link enable
     * SC_R_DC_0    SC_C_PXL_LINK_MST1_VLD    Master 1 pixel link valid
     * SC_R_DC_0    SC_C_PXL_LINK_MST2_ADDR   Master 2 pixel link address
     * SC_R_DC_0    SC_C_PXL_LINK_MST2_ENB    Master 2 pixel link enable
     * SC_R_DC_0    SC_C_PXL_LINK_MST2_VLD    Master 2 pixel link valid
     * SC_R_DC_0    SC_C_SYNC_CTRL0           PL sync ctrl 0
     * SC_R_DC_0    SC_C_SYNC_CTRL1           PL sync ctrl 1
     */
    uint8_t pixelLinkAddr = 0;
    uint32_t i;
    sc_err_t err = SC_ERR_NONE;

    const dpu_mipi_pl_addr_t dpuMipiPlAddrs[] = {
        {
            .dpu             = DC_0__IRIS_MVPL,
            .dpuDisplayIndex = 0,
            .dsi             = MIPI_DSI_HOST0,
            .plAddr          = 0,
        },
        {
            .dpu             = DC_1__IRIS_MVPL,
            .dpuDisplayIndex = 0,
            .dsi             = MIPI_DSI_HOST1,
            .plAddr          = 0,
        },
    };

    const sc_ctrl_t pixelLinkCtrl[][4] = {
        {
            SC_C_PXL_LINK_MST1_ADDR,
            SC_C_PXL_LINK_MST1_ENB,
            SC_C_PXL_LINK_MST1_VLD,
            SC_C_SYNC_CTRL0,
        },
        {
            SC_C_PXL_LINK_MST2_ADDR,
            SC_C_PXL_LINK_MST2_ENB,
            SC_C_PXL_LINK_MST2_VLD,
            SC_C_SYNC_CTRL1,
        },
    };

    /* Get the pixel link address. */
    for (i = 0; i < ARRAY_SIZE(dpuMipiPlAddrs); i++)
    {
        if ((dpu == dpuMipiPlAddrs[i].dpu) && (displayIndex == dpuMipiPlAddrs[i].dpuDisplayIndex) &&
            (dsi == dpuMipiPlAddrs[i].dsi))
        {
            pixelLinkAddr = dpuMipiPlAddrs[i].plAddr;
            break;
        }
    }

    if (ARRAY_SIZE(dpuMipiPlAddrs) <= i)
    {
        PRINTF("ERROR: This DPU to MIPI DSI path is not supported.\r\n");
        return kStatus_Fail;
    }

    /* Set address. */
    err = sc_misc_set_control(ipc, DC_RSRC, pixelLinkCtrl[displayIndex][0], pixelLinkAddr);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Pull down sync control. */
    err = sc_misc_set_control(ipc, DC_RSRC, pixelLinkCtrl[displayIndex][3], 0);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Enable pixel link. */
    err = sc_misc_set_control(ipc, DC_RSRC, pixelLinkCtrl[displayIndex][1], 1);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Delay at least 3 pixel clock. */
    for (volatile uint32_t i = 0; i < 0x100000; i++)
    {
    }

    /* Valid pixel link. */
    err = sc_misc_set_control(ipc, DC_RSRC, pixelLinkCtrl[displayIndex][2], 1);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Pull up sync control. */
    err = sc_misc_set_control(ipc, DC_RSRC, pixelLinkCtrl[displayIndex][3], 1);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    return kStatus_Success;
}
#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
status_t SOC_SetDpuLdbPixelLink(sc_ipc_t ipc, IRIS_MVPL_Type *dpu, uint8_t displayIndex, LDB_Type *ldb)
{
    /*
     * Pixel link setting.
     *
     * DPU 0 display1 is connected to LDB 0 using address 0.
     * DPU 1 display1 is connected to LDB 1 using address 0.
     *
     * SC_R_DC_0    SC_C_PXL_LINK_MST1_ADDR   Master 1 pixel link address
     * SC_R_DC_0    SC_C_PXL_LINK_MST1_ENB    Master 1 pixel link enable
     * SC_R_DC_0    SC_C_PXL_LINK_MST1_VLD    Master 1 pixel link valid
     * SC_R_DC_0    SC_C_PXL_LINK_MST2_ADDR   Master 2 pixel link address
     * SC_R_DC_0    SC_C_PXL_LINK_MST2_ENB    Master 2 pixel link enable
     * SC_R_DC_0    SC_C_PXL_LINK_MST2_VLD    Master 2 pixel link valid
     * SC_R_DC_0    SC_C_SYNC_CTRL0           PL sync ctrl 0
     * SC_R_DC_0    SC_C_SYNC_CTRL1           PL sync ctrl 1
     */
    uint8_t pixelLinkAddr = 0;
    uint32_t i;
    sc_err_t err = SC_ERR_NONE;

    const dpu_ldb_pl_addr_t dpuLdbPlAddrs[] = {
        {
            .dpu             = DC_0__IRIS_MVPL,
            .dpuDisplayIndex = 1,
            .ldb             = DI_LVDS_0__LDB,
            .plAddr          = 0,
        },
        {
            .dpu             = DC_1__IRIS_MVPL,
            .dpuDisplayIndex = 1,
            .ldb             = DI_LVDS_1__LDB,
            .plAddr          = 0,
        },
    };

    const sc_ctrl_t pixelLinkCtrl[][4] = {
        {
            SC_C_PXL_LINK_MST1_ADDR,
            SC_C_PXL_LINK_MST1_ENB,
            SC_C_PXL_LINK_MST1_VLD,
            SC_C_SYNC_CTRL0,
        },
        {
            SC_C_PXL_LINK_MST2_ADDR,
            SC_C_PXL_LINK_MST2_ENB,
            SC_C_PXL_LINK_MST2_VLD,
            SC_C_SYNC_CTRL1,
        },
    };

    /* Get the pixel link address. */
    for (i = 0; i < ARRAY_SIZE(dpuLdbPlAddrs); i++)
    {
        if ((dpu == dpuLdbPlAddrs[i].dpu) && (displayIndex == dpuLdbPlAddrs[i].dpuDisplayIndex) &&
            (ldb == dpuLdbPlAddrs[i].ldb))
        {
            pixelLinkAddr = dpuLdbPlAddrs[i].plAddr;
            break;
        }
    }

    if (ARRAY_SIZE(dpuLdbPlAddrs) <= i)
    {
        PRINTF("ERROR: This DPU to LDB path is not supported.\r\n");
        return kStatus_Fail;
    }

    /* Set address. */
    err = sc_misc_set_control(ipc, DC_RSRC, pixelLinkCtrl[displayIndex][0], pixelLinkAddr);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Pull down sync control. */
    err = sc_misc_set_control(ipc, DC_RSRC, pixelLinkCtrl[displayIndex][3], 0);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Enable pixel link. */
    err = sc_misc_set_control(ipc, DC_RSRC, pixelLinkCtrl[displayIndex][1], 1);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Delay at least 3 pixel clock. */
    for (volatile uint32_t i = 0; i < 0x100000; i++)
    {
    }

    /* Valid pixel link. */
    err = sc_misc_set_control(ipc, DC_RSRC, pixelLinkCtrl[displayIndex][2], 1);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Pull up sync control. */
    err = sc_misc_set_control(ipc, DC_RSRC, pixelLinkCtrl[displayIndex][3], 1);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    return kStatus_Success;
}

#endif /* DPU_EXAMPLE_DI */

void APP_InitPixelLink(void)
{
    sc_ipc_t ipc;

    ipc = SystemGetScfwIpcHandle();

#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
    SOC_SetDpuMipiDsiPixelLink(ipc, APP_DPU, APP_DPU_DISPLAY_INDEX, APP_MIPI_DSI);
#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
    SOC_SetDpuLdbPixelLink(ipc, APP_DPU, APP_DPU_DISPLAY_INDEX, APP_LDB);
#endif
}

void BOARD_PrepareDisplay(void)
{
    uint32_t dpuPllClkFreq_Hz;
    uint32_t dpuDisplayClkFreq_Hz;

    sc_ipc_t ipc;
    sc_err_t err = SC_ERR_NONE;

    ipc = SystemGetScfwIpcHandle();

    /*
     * DPU clock definition
     *
     * Resource            Clock              Description
     * SC_R_DC_0           SC_PM_CLK_MISC0    Display 0 clock
     * SC_R_DC_0           SC_PM_CLK_MISC1    Display 1 clock
     * SC_R_DC_0_PLL_0     SC_PM_CLK_PLL      User programmable PLL clock
     * SC_R_DC_0_PLL_1     SC_PM_CLK_PLL      User programmable PLL clock
     * SC_R_DC_0_VIDEO0    SC_PM_CLK_MISC     Bypass 0 clock
     * SC_R_DC_0_VIDEO1    SC_PM_CLK_MISC     Bypass 1 clock
     */
    err = sc_pm_set_resource_power_mode(ipc, DC_RSRC, SC_PM_PW_MODE_ON);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_set_resource_power_mode(ipc, DC_PLL_RSRC, SC_PM_PW_MODE_ON);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* DPU PLL0. */
    dpuPllClkFreq_Hz = APP_PIXEL_CLOCK_HZ * 8;
    err              = sc_pm_set_clock_rate(ipc, DC_PLL_RSRC, SC_PM_CLK_PLL, &dpuPllClkFreq_Hz);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, DC_PLL_RSRC, SC_PM_CLK_PLL, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* DPU display clock set to the same as pixel clock. */
    dpuDisplayClkFreq_Hz = APP_PIXEL_CLOCK_HZ;
    err                  = sc_pm_set_clock_rate(ipc, DC_RSRC, DC_DISPLAY_CLOCK, &dpuDisplayClkFreq_Hz);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, DC_RSRC, DC_DISPLAY_CLOCK, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)

    /* Power up MIPI DSI subsystem. */
    err = sc_pm_set_resource_power_mode(ipc, MIPI_DSI_RSRC, SC_PM_PW_MODE_ON);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /*
     * MIPI clock setting.
     * MIPI DSI clock definition:
     * SC_PM_CLK_BYPASS    Bypass clock
     * SC_PM_CLK_MST_BUS   DSI tx escape clock
     * SC_PM_CLK_PER       DPI (pixel) clock
     * SC_PM_CLK_SLV_BUS   DSI rx escape clock
     * SC_R_MIPI           SC_PM_CLK_PHY DPHY PLL ref
     */
    /* The DPI clock sources from the bypass clock, for simple, set them to the same rate. */
    mipiDsiDpiClkFreq_Hz = dpuDisplayClkFreq_Hz;
    err                  = sc_pm_set_clock_rate(ipc, MIPI_DSI_RSRC, SC_PM_CLK_BYPASS, &mipiDsiDpiClkFreq_Hz);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }
    err = sc_pm_clock_enable(ipc, MIPI_DSI_RSRC, SC_PM_CLK_BYPASS, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    mipiDsiDpiClkFreq_Hz = dpuDisplayClkFreq_Hz;
    err                  = sc_pm_set_clock_rate(ipc, MIPI_DSI_RSRC, SC_PM_CLK_PER, &mipiDsiDpiClkFreq_Hz);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }
    /* DPI clock (pixel clock), max value is 300MHz. */
    err = sc_pm_clock_enable(ipc, MIPI_DSI_RSRC, SC_PM_CLK_PER, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* tx_esc clock, max value is 20MHz. */
    mipiDsiTxEscClkFreq_Hz = 20000000;
    err                    = sc_pm_clock_enable(ipc, MIPI_DSI_RSRC, SC_PM_CLK_MST_BUS, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* rx_esc resample clock, max value is 60MHz. */
    mipiDsiRxEscClkFreq_Hz = 60000000;
    err                    = sc_pm_clock_enable(ipc, MIPI_DSI_RSRC, SC_PM_CLK_SLV_BUS, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* DPHY reference clock is enabled by default, 27MHz. */
    mipiDsiDphyRefClkFreq_Hz = 27000000U;
    err = sc_pm_clock_enable(ipc, MIPI_DSI_RSRC, SC_PM_CLK_PHY, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /*
     * DPHY bit clock, max value 1.5GHz.
     *
     * If external convertor such as ADV7535 is used, the MIPI DSI works in
     * non-burst-with-sync-pulse mode, in this mode the DPHY clock should be
     * matched with the pixel clock. When ppb is 24, the DPHY bit clock
     * frequency is (pixel clock * 24 / data lane used).
     *
     * If external convertor is not used, the MIPI DSI works in burst mode,
     * in this mode, the DPHY clock should be quick enough to send out pixels
     * in time.
     */

#if APP_DISPLAY_EXTERNAL_CONVERTOR
    mipiDsiDphyBitClkFreq_Hz = mipiDsiDpiClkFreq_Hz * (24 / APP_MIPI_DSI_LANE_NUM);
#else
    mipiDsiDphyBitClkFreq_Hz = 891000000;
#endif

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)

    /* Power up LVDS subsystem. */
    err = sc_pm_set_resource_power_mode(ipc, LDB_RSRC, SC_PM_PW_MODE_ON);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /*
     * LVDS clock setting.
     * SC_PM_CLK_BYPASS    Bypass
     * SC_PM_CLK_PER       Pixel
     * SC_PM_CLK_PHY       Phy
     */
    displayPixelClkFreq_Hz = dpuDisplayClkFreq_Hz;

    err = sc_pm_set_clock_rate(ipc, LDB_RSRC, SC_PM_CLK_BYPASS, &displayPixelClkFreq_Hz);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, LDB_RSRC, SC_PM_CLK_BYPASS, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_set_clock_rate(ipc, LDB_RSRC, SC_PM_CLK_PER, &displayPixelClkFreq_Hz);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, LDB_RSRC, SC_PM_CLK_PER, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_set_clock_rate(ipc, LDB_RSRC, SC_PM_CLK_PHY, &displayPixelClkFreq_Hz);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, LDB_RSRC, SC_PM_CLK_PHY, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

#endif

    SOC_InitDpuInterrupt();
}

void BOARD_UnprepareDisplay(void)
{
    sc_ipc_t ipc;
    sc_err_t err = SC_ERR_NONE;

    SOC_DeinitDpuInterrupt();

    ipc = SystemGetScfwIpcHandle();

#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
    err = sc_pm_clock_enable(ipc, MIPI_DSI_RSRC, SC_PM_CLK_BYPASS, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, MIPI_DSI_RSRC, SC_PM_CLK_PER, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, MIPI_DSI_RSRC, SC_PM_CLK_MST_BUS, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, MIPI_DSI_RSRC, SC_PM_CLK_SLV_BUS, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, MIPI_DSI_RSRC, SC_PM_CLK_PHY, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Power off MIPI DSI subsystem. */
    err = sc_pm_set_resource_power_mode(ipc, MIPI_DSI_RSRC, SC_PM_PW_MODE_OFF);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)

    err = sc_pm_clock_enable(ipc, LDB_RSRC, SC_PM_CLK_BYPASS, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, LDB_RSRC, SC_PM_CLK_PER, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, LDB_RSRC, SC_PM_CLK_PHY, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Power up LVDS subsystem. */
    err = sc_pm_set_resource_power_mode(ipc, LDB_RSRC, SC_PM_PW_MODE_OFF);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }
#endif

    /* DPU PLL0. */
    err = sc_pm_clock_enable(ipc, DC_PLL_RSRC, SC_PM_CLK_PLL, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, DC_RSRC, DC_DISPLAY_CLOCK, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_set_resource_power_mode(ipc, DC_RSRC, SC_PM_PW_MODE_OFF);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_set_resource_power_mode(ipc, DC_PLL_RSRC, SC_PM_PW_MODE_OFF);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }
}

#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)

#if !APP_DISPLAY_EXTERNAL_CONVERTOR
static void RM67191_PullResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PortSet(RM67191_RESET_GPIO, (1U << RM67191_RESET_PIN));
    }
    else
    {
        GPIO_PortClear(RM67191_RESET_GPIO, (1U << RM67191_RESET_PIN));
    }
}

status_t RM67191_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(APP_MIPI_DSI, xfer);
}
#endif

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
static void IT6263_PullResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PortSet(IT6263_RESET_GPIO, (1U << IT6263_RESET_PIN));
    }
    else
    {
        GPIO_PortClear(IT6263_RESET_GPIO, (1U << IT6263_RESET_PIN));
    }
}
#endif

void BOARD_InitLcdPanel(void)
{
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)

#if !APP_DISPLAY_EXTERNAL_CONVERTOR
    sc_ipc_t ipc;

    const display_config_t displayConfig = {
        .resolution   = FSL_VIDEO_RESOLUTION(APP_FRAME_WIDTH, APP_FRAME_HEIGHT),
        .hsw          = APP_HSW,
        .hfp          = APP_HFP,
        .hbp          = APP_HBP,
        .vsw          = APP_VSW,
        .vfp          = APP_VFP,
        .vbp          = APP_VBP,
        .controlFlags = 0,
        .dsiLanes     = APP_MIPI_DSI_LANE_NUM,
    };

    ipc = SystemGetScfwIpcHandle();

    /* Init the resource for RM67191. */
    gpio_pin_config_t config = {
        .direction   = kGPIO_DigitalOutput,
        .outputLogic = 1,
    };

    if (sc_pm_set_resource_power_mode(ipc, SC_R_GPIO_1, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on GPIO\r\n");
    }

    GPIO_PinInit(RM67191_RESET_GPIO, RM67191_RESET_PIN, &config);

    if (kStatus_Success != RM67191_Init(&rm67191Handle, &displayConfig))
    {
        PRINTF("Error: Failed to init the LCD panel\r\n");
    }

    return;
#else
    static adv7535_resource_t adv7535Resource;

    static display_handle_t handle = {
        .resource = &adv7535Resource,
        .ops      = &adv7535_ops,
    };

    const display_config_t displayConfig = {
        .resolution   = FSL_VIDEO_RESOLUTION(APP_FRAME_WIDTH, APP_FRAME_HEIGHT),
        .hsw          = APP_HSW,
        .hfp          = APP_HFP,
        .hbp          = APP_HBP,
        .vsw          = APP_VSW,
        .vfp          = APP_VFP,
        .vbp          = APP_VBP,
        .controlFlags = 0,
        .dsiLanes     = APP_MIPI_DSI_LANE_NUM,
    };

    /* Init the resource. */
    BOARD_Display_I2C_Init();
    adv7535Resource.i2cAddr        = 0x3D;
    adv7535Resource.i2cSendFunc    = BOARD_Display_I2C_Send;
    adv7535Resource.i2cReceiveFunc = BOARD_Display_I2C_Receive;

    ADV7535_Init(&handle, &displayConfig);

    return;
#endif

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
    sc_ipc_t ipc;

    display_config_t displayConfig = {
        .resolution    = FSL_VIDEO_RESOLUTION(APP_FRAME_WIDTH, APP_FRAME_HEIGHT),
        .hsw           = APP_HSW,
        .hfp           = APP_HFP,
        .hbp           = APP_HBP,
        .vsw           = APP_VSW,
        .vfp           = APP_VFP,
        .vbp           = APP_VBP,
        .controlFlags  = 0,
        .pixelClock_Hz = displayPixelClkFreq_Hz,
    };

    ipc = SystemGetScfwIpcHandle();

    /* Init the resource for IT6263. */
    BOARD_Display_I2C_Init();

    gpio_pin_config_t config = {
        .direction   = kGPIO_DigitalOutput,
        .outputLogic = 1,
    };

    if (sc_pm_set_resource_power_mode(ipc, SC_R_GPIO_1, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on GPIO\r\n");
    }

    GPIO_PinInit(IT6263_RESET_GPIO, IT6263_RESET_PIN, &config);

    if (kStatus_Success != IT6263_Init(&it6263Handle, &displayConfig))
    {
        PRINTF("Error: Failed to init the LCD panel\r\n");
    }

    IT6263_Start(&it6263Handle);

    return;
#endif /* DPU_EXAMPLE_DI */
}

void BOARD_DeinitLcdPanel(void)
{
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)

#if !APP_DISPLAY_EXTERNAL_CONVERTOR
    if (kStatus_Success != RM67191_Deinit(&rm67191Handle))
    {
        PRINTF("Error: Failed to deinit the LCD panel\r\n");
    }

    return;
#else
    static adv7535_resource_t adv7535Resource;

    static display_handle_t handle = {
        .resource = &adv7535Resource,
        .ops      = &adv7535_ops,
    };

    adv7535Resource.i2cAddr        = 0x3D;
    adv7535Resource.i2cSendFunc    = BOARD_Display_I2C_Send;
    adv7535Resource.i2cReceiveFunc = BOARD_Display_I2C_Receive;

    ADV7535_Deinit(&handle);
    /* Deinit the resource. */
    BOARD_Display_I2C_Deinit();
    return;
#endif

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)

    IT6263_Stop(&it6263Handle);

    /* Don't call IT6263_Deinit() which will reset both LVDS instance and lead to integration issue with xen. */

    /* Deinit the resource for IT6263. */
    BOARD_Display_I2C_Deinit();
    return;
#endif /* DPU_EXAMPLE_DI */
}
/*
 * Prepare for the camera:
 *
 * Initialize the MIPI CSI2, the input frames are sent to ISI.
 * Initialize the I2C to communicate with camera.
 * Enable the ISI interrupt.
 */
void BOARD_PrepareCamera(void)
{
    sc_ipc_t ipc;
    sc_err_t err = SC_ERR_NONE;

    ipc = SystemGetScfwIpcHandle();

    err = sc_pm_set_resource_power_mode(ipc, ISI_RSRC, SC_PM_PW_MODE_ON);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /*
     * MIPI CSI2 subsystem clock.
     *
     * Resource              Clock              Description
     * SC_R_CSI_0          SC_PM_CLK_MISC       MIPI escape clock
     * SC_R_CSI_0          SC_PM_CLK_PER        MIPI core clock
     * SC_R_CSI_0_I2C_0    SC_PM_CLK_PER        I2C baud
     * SC_R_CSI_0_PWM_0    SC_PM_CLK_PER        PWM high
     */
    err = sc_pm_set_resource_power_mode(ipc, MIPI_CSI_RSRC, SC_PM_PW_MODE_ON);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /* Initialize the camera I2C. */
    BOARD_Camera_I2C_Init();

#if (!defined(CAMERA_DEVICE)) || (CAMERA_DEVICE == CAMERA_DEVICE_OV5640)
    /* GPIO pin for camera. */
    gpio_pin_config_t gpioConfig = {
        kGPIO_DigitalOutput,
        1,
    };

    GPIO_PinInit(CAMERA_GPIO, CAMERA_RST_PIN_IDX, &gpioConfig);
    GPIO_PinInit(CAMERA_GPIO, CAMERA_PD_PIN_IDX, &gpioConfig);
#elif (CAMERA_DEVICE == CAMERA_DEVICE_MAX9286)
    /* GPIO pin for camera. */
    gpio_pin_config_t gpioConfig = {
        kGPIO_DigitalOutput,
        0,
    };

    GPIO_PinInit(CAMERA_GPIO, CAMERA_RST_PIN_IDX, &gpioConfig);
#endif

    /* Enable ISI interrupt. */
    IRQSTEER_EnableInterrupt(IRQSTEER, APP_ISI_IRQn);
}

void BOARD_UnprepareCamera(void)
{
    sc_ipc_t ipc;
    sc_err_t err = SC_ERR_NONE;

    ipc = SystemGetScfwIpcHandle();

    /*
     * MIPI CSI2 subsystem clock.
     *
     * Resource              Clock              Description
     * SC_R_CSI_0          SC_PM_CLK_MISC       MIPI escape clock
     * SC_R_CSI_0          SC_PM_CLK_PER        MIPI core clock
     * SC_R_CSI_0_I2C_0    SC_PM_CLK_PER        I2C baud
     * SC_R_CSI_0_PWM_0    SC_PM_CLK_PER        PWM high
     */
    BOARD_Camera_I2C_Deinit();

    err = sc_pm_set_resource_power_mode(ipc, ISI_RSRC, SC_PM_PW_MODE_OFF);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_set_resource_power_mode(ipc, MIPI_CSI_RSRC, SC_PM_PW_MODE_OFF);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    IRQSTEER_DisableInterrupt(IRQSTEER, APP_ISI_IRQn);
}

void BOARD_InitCameraInterface(void)
{
    sc_ipc_t ipc;
    sc_err_t err = SC_ERR_NONE;
    uint32_t mipiCsiEscClkFreq_Hz;
    uint32_t mipiCsiCoreClkFreq_Hz;
    csi2rx_config_t csi2rxConfig = {0};

    ipc = SystemGetScfwIpcHandle();

    /*
     * MIPI CSI2 subsystem clock.
     *
     * Resource              Clock              Description
     * SC_R_CSI_0          SC_PM_CLK_MISC       MIPI escape clock
     * SC_R_CSI_0          SC_PM_CLK_PER        MIPI core clock
     * SC_R_CSI_0_I2C_0    SC_PM_CLK_PER        I2C baud
     * SC_R_CSI_0_PWM_0    SC_PM_CLK_PER        PWM high
     */
    mipiCsiEscClkFreq_Hz = 72000000;
    err                  = sc_pm_set_clock_rate(ipc, MIPI_CSI_RSRC, SC_PM_CLK_MISC, &mipiCsiEscClkFreq_Hz);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, MIPI_CSI_RSRC, SC_PM_CLK_MISC, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    mipiCsiCoreClkFreq_Hz = 360000000;
    err                   = sc_pm_set_clock_rate(ipc, MIPI_CSI_RSRC, SC_PM_CLK_PER, &mipiCsiCoreClkFreq_Hz);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, MIPI_CSI_RSRC, SC_PM_CLK_PER, true, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /*
     * Initialize the MIPI CSI2
     *
     * T_HS_SETTLE setting for camera:
     *
     *    Resolution  |  frame rate  |  T_HS_SETTLE
     *  =============================================
     *     720P       |     30       |     0x10
     *  ---------------------------------------------
     *     720P       |     15       |     0x15
     *  ---------------------------------------------
     *      VGA       |     30       |     0x1D
     *  ---------------------------------------------
     *      VGA       |     15       |     0x22
     *  ---------------------------------------------
     *     QVGA       |     30       |     0x1D
     *  ---------------------------------------------
     *     QVGA       |     15       |     0x22
     *  ---------------------------------------------
     */
    static uint32_t csi2rxHsSettle[][3] = {
        {
            kVIDEO_Resolution1080P,
            30,
            0x0A,
        },
        {
            kVIDEO_Resolution1080P,
            15,
            0x0F,
        },
        {
            kVIDEO_Resolution720P,
            30,
            0x10,
        },
        {
            kVIDEO_Resolution720P,
            15,
            0x15,
        },
        {
            kVIDEO_ResolutionVGA,
            30,
            0x1D,
        },
        {
            kVIDEO_ResolutionVGA,
            15,
            0x22,
        },
        {
            kVIDEO_ResolutionQVGA,
            30,
            0x1D,
        },
        {
            kVIDEO_ResolutionQVGA,
            15,
            0x22,
        },
    };

    csi2rxConfig.laneNum          = APP_MIPI_CSI_LANES;
    csi2rxConfig.tHsSettle_EscClk = 0x10;

    for (uint8_t i = 0; i < ARRAY_SIZE(csi2rxHsSettle); i++)
    {
        if ((FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT) == csi2rxHsSettle[i][0]) &&
            (csi2rxHsSettle[i][1] == APP_CAMERA_FRAME_RATE))
        {
            csi2rxConfig.tHsSettle_EscClk = csi2rxHsSettle[i][2];
            break;
        }
    }

    CSI2RX_Init(APP_MIPI_CSI2, &csi2rxConfig);

    /* Deassert the reset. */
    err = sc_misc_set_control(ipc, MIPI_CSI_RSRC, SC_C_MIPI_RESET, 1);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }
}

void BOARD_DeinitCameraInterface(void)
{
    sc_ipc_t ipc;
    sc_err_t err = SC_ERR_NONE;

    ipc = SystemGetScfwIpcHandle();

    CSI2RX_Deinit(APP_MIPI_CSI2);

    /* Assert the reset. */
    err = sc_misc_set_control(ipc, MIPI_CSI_RSRC, SC_C_MIPI_RESET, 0);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    /*
     * MIPI CSI2 subsystem clock.
     *
     * Resource              Clock              Description
     * SC_R_CSI_0          SC_PM_CLK_MISC       MIPI escape clock
     * SC_R_CSI_0          SC_PM_CLK_PER        MIPI core clock
     * SC_R_CSI_0_I2C_0    SC_PM_CLK_PER        I2C baud
     * SC_R_CSI_0_PWM_0    SC_PM_CLK_PER        PWM high
     */
    err = sc_pm_clock_enable(ipc, MIPI_CSI_RSRC, SC_PM_CLK_MISC, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }

    err = sc_pm_clock_enable(ipc, MIPI_CSI_RSRC, SC_PM_CLK_PER, false, false);
    if (SC_ERR_NONE != err)
    {
        assert(false);
    }
}

static void APP_PullCameraResetPin(bool pullUp)
{
    GPIO_WritePinOutput(CAMERA_GPIO, CAMERA_RST_PIN_IDX, pullUp ? 1 : 0);
}

#if (!defined(CAMERA_DEVICE)) || (CAMERA_DEVICE == CAMERA_DEVICE_OV5640)
static void APP_PullCameraPowerDownPin(bool pullUp)
{
    GPIO_WritePinOutput(CAMERA_GPIO, CAMERA_PD_PIN_IDX, pullUp ? 1 : 0);
}
#endif

#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
void SOC_ResetMipiDsi(MIPI_DSI_HOST_Type *base, mipi_dsi_part_t part, uint32_t reset)
{
    sc_ipc_t ipc;
    sc_err_t err = SC_ERR_NONE;

    sc_ctrl_t ctrls[] = {SC_C_DPI_RESET, SC_C_MIPI_RESET, SC_C_PHY_RESET};

    ipc = SystemGetScfwIpcHandle();

    err = sc_misc_set_control(ipc, MIPI_DSI_RSRC, ctrls[part], reset);

    if (SC_ERR_NONE != err)
    {
        assert(false);
    }
}
#endif /* DPU_EXAMPLE_DI */

/*
 * The ISI has an overflow issue, ISI uses VSYNC to switch output buffer,
 * but if the input frame VSYNC is missed because of noise or camera and ISI
 * not synchronized, the ISI will not switch output buffer, it continue saving
 * the input data in the previous buffer and overflow happens.
 * To protect the other data from overwritten by ISI, XRDC is used to limit
 * ISI write permission to the frame buffer region.
 */
void SOC_SetIsiPermission(uint64_t startAddr, uint64_t endAddr)
{
    sc_ipc_t ipc;
    sc_err_t err = SC_ERR_NONE;

    ipc = SystemGetScfwIpcHandle();

    err = sc_rm_partition_alloc(ipc, &isi_pt, false, true, false, false, false);

    if (SC_ERR_NONE != err)
    {
        PRINTF("Could not alloc partition for ISI\r\n");
        return;
    }

    err = sc_rm_assign_resource(ipc, isi_pt, ISI_RSRC);

    if (SC_ERR_NONE != err)
    {
        PRINTF("Could not assign ISI to new allocate partition\r\n");
        return;
    }

    err = sc_rm_memreg_alloc(ipc, &isi_mr, startAddr, endAddr);
    if (SC_ERR_NONE != err)
    {
        PRINTF("Could not alloc memory region for frame buffer memory\r\n");
        return;
    }

    err = sc_rm_set_memreg_permissions(ipc, isi_mr, isi_pt, SC_RM_PERM_FULL);
    if (SC_ERR_NONE != err)
    {
        PRINTF("Could not set the frame buffer memory permission for ISI\r\n");
        return;
    }

    /* Set ISI accessible by current master. */
    err = sc_rm_get_partition(ipc, &cm4_pt);
    if (SC_ERR_NONE != err)
    {
        PRINTF("Could not get current master partition\r\n");
        return;
    }

    err = sc_rm_set_peripheral_permissions(ipc, ISI_RSRC, cm4_pt, SC_RM_PERM_FULL);
    if (SC_ERR_NONE != err)
    {
        PRINTF("Could not set ISI accessible\r\n");
        return;
    }

    return;
}

void SOC_UnsetIsiPermission(void)
{
    sc_ipc_t ipc;
    sc_err_t err = SC_ERR_NONE;

    ipc = SystemGetScfwIpcHandle();

    err = sc_rm_partition_free(ipc, isi_pt);

    if (SC_ERR_NONE != err)
    {
        PRINTF("Could not free partition for ISI\r\n");
        return;
    }

    err = sc_rm_memreg_free(ipc, isi_mr);
    if (SC_ERR_NONE != err)
    {
        PRINTF("Could not free memory region for frame buffer memory\r\n");
        return;
    }

    return;
}
