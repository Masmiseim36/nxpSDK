/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"

#include "pin_mux.h"
#include "fsl_irqsteer.h"
#include "clock_config.h"
#include "fsl_lpi2c.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_CAN ADMA__CAN0
/*
 * When CLK_SRC=1, the protocol engine works at fixed frequency of 160M.
 * If other frequency wanted, please use CLK_SRC=0 and set the working frequency for SC_R_CAN_0.
 */
#define EXAMPLE_CAN_CLK_FREQ (SC_160MHZ)
#define RX_MESSAGE_BUFFER_NUM (9)
#define TX_MESSAGE_BUFFER_NUM (8)
/*
 * ADMA_I2C1 is used to control PCA9646, PCA9557 to handle the RST pin for PCA6416
 * M4_I2C is used to control PCA6416 to handle the RST and STB pin for CAN XCVR TJA1043
 */
#define EXAMPLE_IOEXP_LPI2C_BAUDRATE (400000) /*in i2c example it is 100000*/
#define EXAMPLE_IOEXP_LPI2C_MASTER_CLOCK_FREQUENCY SC_133MHZ
#define EXAMPLE_IOEXP_LPI2C_MASTER ADMA__LPI2C1

#define EXAMPLE_IOEXP_BASE_LPI2C_BAUDRATE (400000) /*in i2c example it is 100000*/
#define EXAMPLE_IOEXP_BASE_LPI2C_MASTER_CLOCK_FREQUENCY SC_133MHZ
#define EXAMPLE_IOEXP_BASE_LPI2C_MASTER CM4__LPI2C

/*PCA6416 I2C Register Map*/
#define PCA6416_REG_INPUT_PORT_0 (0x0)
#define PCA6416_REG_INPUT_PORT_1 (0x1)
#define PCA6416_REG_OUTPUT_PORT_0 (0x2)
#define PCA6416_REG_OUTPUT_PORT_1 (0x3)
#define PCA6416_REG_POLARITY_INVERSION_PORT_0 (0x4)
#define PCA6416_REG_POLARITY_INVERSION_PORT_1 (0x5)
#define PCA6416_REG_CONFIGURATION_PORT_0 (0x6)
#define PCA6416_REG_CONFIGURATION_PORT_1 (0x7)

/*PCA9557 I2C Register Map*/
#define PCA9557_REG_INTPUT_PORT (0x00)
#define PCA9557_REG_OUTPUT_PORT (0x01)
#define PCA9557_REG_POLARITY_INVERSION (0x02)
#define PCA9557_REG_CONFIGURATION (0x03)

/*Board I2C Addresses*/
#define EXAMPLE_I2C_EXPANSION_CAN_ADDR (0x20)
#define EXAMPLE_I2C_EXPANSION_A_ADDR (0x1A)
#define EXAMPLE_I2C_SWITCH_ADDR (0x71)

#define DLC (8)

/* To get most precise baud rate under some circumstances, users need to set
   quantum which is composed of PSEG1/PSEG2/PROPSEG. Because CAN clock prescaler
   = source clock/(baud rate * quantum), for e.g. 84M clock and 1M baud rate, the
   quantum should be .e.g 14=(6+3+1)+4, so prescaler is 6. By default, quantum
   is set to 10=(3+2+1)+4, because for most platforms e.g. 120M source clock/(1M
   baud rate * 10) is an integer. Remember users must ensure the calculated
   prescaler an integer thus to get precise baud rate. */
#define SET_CAN_QUANTUM 1
#define PSEG1 6
#define PSEG2 4
#define PROPSEG 6
#define FPSEG1 6
#define FPSEG2 4
#define FPROPSEG 7
#if (defined(FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829) && FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829)
/* To consider the First valid MB must be used as Reserved TX MB for ERR005829
   If RX FIFO enable(RFEN bit in MCE set as 1) and RFFN in CTRL2 is set default zero, the first valid TX MB Number is 8
   If RX FIFO enable(RFEN bit in MCE set as 1) and RFFN in CTRL2 is set by other value(0x1~0xF), User should consider
   detail first valid MB number
   If RX FIFO disable(RFEN bit in MCE set as 0) , the first valid MB number is zero */
#ifdef RX_MESSAGE_BUFFER_NUM
#undef RX_MESSAGE_BUFFER_NUM
#define RX_MESSAGE_BUFFER_NUM (10)
#endif
#ifdef TX_MESSAGE_BUFFER_NUM
#undef TX_MESSAGE_BUFFER_NUM
#define TX_MESSAGE_BUFFER_NUM (9)
#endif
#endif
#ifndef DEMO_FORCE_CAN_SRC_OSC
#define DEMO_FORCE_CAN_SRC_OSC 0
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexcan_handle_t flexcanHandle;
volatile bool txComplete = false;
volatile bool rxComplete = false;
volatile bool wakenUp    = false;
flexcan_mb_transfer_t txXfer, rxXfer;
#if (defined(USE_CANFD) && USE_CANFD)
flexcan_fd_frame_t frame;
#else
flexcan_frame_t frame;
#endif
uint32_t txIdentifier;
uint32_t rxIdentifier;

/*******************************************************************************
 * Code
 ******************************************************************************/
static bool PCA6416_WriteReg(
    LPI2C_Type *base, const uint8_t dev_addr, uint8_t reg_offset, uint8_t *txBuff, uint32_t txSize)
{
    status_t reVal = kStatus_Fail;

    if (kStatus_Success == LPI2C_MasterStart(base, dev_addr, kLPI2C_Write))
    {
        while (LPI2C_MasterGetStatusFlags(base) & kLPI2C_MasterNackDetectFlag)
        {
        }

        reVal = LPI2C_MasterSend(base, &reg_offset, 1);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterSend(base, txBuff, txSize);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterStop(base);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
    }
    return kStatus_Success;
}

static bool PCA9557_WriteReg(
    LPI2C_Type *base, const uint8_t dev_addr, uint8_t reg_offset, uint8_t *txBuff, uint32_t txSize)
{
    status_t reVal = kStatus_Fail;

    if (kStatus_Success == LPI2C_MasterStart(base, dev_addr, kLPI2C_Write))
    {
        while (LPI2C_MasterGetStatusFlags(base) & kLPI2C_MasterNackDetectFlag)
        {
        }

        reVal = LPI2C_MasterSend(base, &reg_offset, 1);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterSend(base, txBuff, txSize);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterStop(base);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
    }
    return kStatus_Success;
}

static bool PCA9646_WriteReg(LPI2C_Type *base, const uint8_t dev_addr, uint8_t *txBuff, uint32_t txSize)
{
    status_t reVal = kStatus_Fail;

    if (kStatus_Success == LPI2C_MasterStart(base, dev_addr, kLPI2C_Write))
    {
        while (LPI2C_MasterGetStatusFlags(base) & kLPI2C_MasterNackDetectFlag)
        {
        }

        reVal = LPI2C_MasterSend(base, txBuff, txSize);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterStop(base);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
    }
    return kStatus_Success;
}

sc_err_t BOARD_ConfigureExpansionIO()
{
    sc_err_t err = SC_ERR_NONE;
    lpi2c_master_config_t masterConfig;
    uint8_t txBuffer[4] = {0};
    uint32_t i;

    /*
     * 1. Channel 3 is opened on PCA9646
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = EXAMPLE_IOEXP_LPI2C_BAUDRATE;
    LPI2C_MasterInit(EXAMPLE_IOEXP_LPI2C_MASTER, &masterConfig, EXAMPLE_IOEXP_LPI2C_MASTER_CLOCK_FREQUENCY);

    txBuffer[0] = 0x8;
    PCA9646_WriteReg(EXAMPLE_IOEXP_LPI2C_MASTER, EXAMPLE_I2C_SWITCH_ADDR, txBuffer, 1);
    /*
     * 2. PCA9557 is configured to reset PCA6416
     */
    txBuffer[0] = 0;
    PCA9557_WriteReg(EXAMPLE_IOEXP_LPI2C_MASTER, EXAMPLE_I2C_EXPANSION_A_ADDR, PCA9557_REG_CONFIGURATION, txBuffer, 1);
    txBuffer[0] = 0x0;
    PCA9557_WriteReg(EXAMPLE_IOEXP_LPI2C_MASTER, EXAMPLE_I2C_EXPANSION_A_ADDR, PCA9557_REG_OUTPUT_PORT, txBuffer, 1);
    i = 0;
    while (i < 15000000)
    {
        __ASM("nop");
        i++;
    }
    txBuffer[0] = 0x8;
    PCA9557_WriteReg(EXAMPLE_IOEXP_LPI2C_MASTER, EXAMPLE_I2C_EXPANSION_A_ADDR, PCA9557_REG_OUTPUT_PORT, txBuffer, 1);

    /*
     * 3. PCA6416 is configured to reset TJA1043
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = EXAMPLE_IOEXP_BASE_LPI2C_BAUDRATE;
    LPI2C_MasterInit(EXAMPLE_IOEXP_BASE_LPI2C_MASTER, &masterConfig, EXAMPLE_IOEXP_BASE_LPI2C_MASTER_CLOCK_FREQUENCY);
    txBuffer[0] = 0;
    PCA6416_WriteReg(EXAMPLE_IOEXP_BASE_LPI2C_MASTER, EXAMPLE_I2C_EXPANSION_CAN_ADDR, PCA6416_REG_CONFIGURATION_PORT_0,
                     txBuffer, 1);
    txBuffer[0] = 0;
    PCA6416_WriteReg(EXAMPLE_IOEXP_BASE_LPI2C_MASTER, EXAMPLE_I2C_EXPANSION_CAN_ADDR, PCA6416_REG_OUTPUT_PORT_0,
                     txBuffer, 1);
    i = 0;
    while (i < 15000000)
    {
        __ASM("nop");
        i++;
    }
    txBuffer[0] = 0x28;
    PCA6416_WriteReg(EXAMPLE_IOEXP_BASE_LPI2C_MASTER, EXAMPLE_I2C_EXPANSION_CAN_ADDR, PCA6416_REG_OUTPUT_PORT_0,
                     txBuffer, 1);

    LPI2C_MasterDeinit(EXAMPLE_IOEXP_LPI2C_MASTER);
    LPI2C_MasterDeinit(EXAMPLE_IOEXP_BASE_LPI2C_MASTER);

    return err;
}

/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    switch (status)
    {
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                rxComplete = true;
            }
            break;

        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                txComplete = true;
            }
            break;

        case kStatus_FLEXCAN_WakeUp:
            wakenUp = true;
            break;

        default:
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;
    uint8_t node_type;

    /* Initialize board hardware. */
    sc_ipc_t ipc;
    sc_pm_clock_rate_t src_rate = SC_133MHZ;

    ipc = BOARD_InitRpc();
    BOARD_InitPinsPre();
    BOARD_InitPins(ipc);
    BOARD_PowerOnBaseBoard();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();

    /* Power on Peripherals. */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_CAN_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on FLEXCAN\r\n");
    }

    /* open the lpi2c power and clock */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_M4_0_I2C, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on SC_R_M4_0_I2C\r\n");
    }

    if (sc_pm_clock_enable(ipc, SC_R_M4_0_I2C, SC_PM_CLK_PER, true, false) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable SC_R_M4_0_I2C clock \r\n");
    }

    if (sc_pm_set_clock_rate(ipc, SC_R_M4_0_I2C, SC_PM_CLK_PER, &src_rate) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set SC_R_M4_0_I2C clock rate\r\n");
    }

    if (sc_pm_set_resource_power_mode(ipc, SC_R_I2C_1, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on SC_R_I2C_1\r\n");
    }

    src_rate = SC_133MHZ;

    if (sc_pm_clock_enable(ipc, SC_R_I2C_1, SC_PM_CLK_PER, true, false) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable SC_R_I2C_1 clock \r\n");
    }

    if (sc_pm_set_clock_rate(ipc, SC_R_I2C_1, SC_PM_CLK_PER, &src_rate) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set SC_R_I2C_1 clock rate\r\n");
    }

    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTR\r\n");
    }

    BOARD_ConfigureExpansionIO();

    IRQSTEER_Init(IRQSTEER);
    NVIC_EnableIRQ(IRQSTEER_4_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, ADMA_FLEXCAN0_INT_IRQn);

    PRINTF("********* FLEXCAN Interrupt EXAMPLE *********\r\n");
    PRINTF("    Message format: Standard (11 bit id)\r\n");
    PRINTF("    Message buffer %d used for Rx.\r\n", RX_MESSAGE_BUFFER_NUM);
    PRINTF("    Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);
    PRINTF("    Interrupt Mode: Enabled\r\n");
    PRINTF("    Operation Mode: TX and RX --> Normal\r\n");
    PRINTF("*********************************************\r\n\r\n");

    do
    {
        PRINTF("Please select local node as A or B:\r\n");
        PRINTF("Note: Node B should start first.\r\n");
        PRINTF("Node:");
        node_type = GETCHAR();
        PRINTF("%c", node_type);
        PRINTF("\r\n");
    } while ((node_type != 'A') && (node_type != 'B') && (node_type != 'a') && (node_type != 'b'));

    /* Select mailbox ID. */
    if ((node_type == 'A') || (node_type == 'a'))
    {
        txIdentifier = 0x321;
        rxIdentifier = 0x123;
    }
    else
    {
        txIdentifier = 0x123;
        rxIdentifier = 0x321;
    }

    /* Get FlexCAN module default Configuration. */
    /*
     * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
     * flexcanConfig.baudRate = 1000000U;
     * flexcanConfig.baudRateFD = 2000000U;
     * flexcanConfig.maxMbNum = 16;
     * flexcanConfig.enableLoopBack = false;
     * flexcanConfig.enableSelfWakeup = false;
     * flexcanConfig.enableIndividMask = false;
     * flexcanConfig.enableDoze = false;
     * flexcanConfig.timingConfig = timingConfig;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
/* Init FlexCAN module. */
#if (!defined(DEMO_FORCE_CAN_SRC_OSC)) || !DEMO_FORCE_CAN_SRC_OSC
#if (!defined(FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE)) || !FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE
    flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
#else
#if defined(CAN_CTRL1_CLKSRC_MASK)
    if (!FSL_FEATURE_FLEXCAN_INSTANCE_SUPPORT_ENGINE_CLK_SEL_REMOVEn(EXAMPLE_CAN))
    {
        flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
    }
#endif
#endif /* FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE */
#endif /* DEMO_FORCE_CAN_SRC_OSC */
/* If special quantum setting is needed, set the timing parameters. */
#if (defined(SET_CAN_QUANTUM) && SET_CAN_QUANTUM)
    flexcanConfig.timingConfig.phaseSeg1 = PSEG1;
    flexcanConfig.timingConfig.phaseSeg2 = PSEG2;
    flexcanConfig.timingConfig.propSeg   = PROPSEG;
#if (defined(FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE) && FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE)
    flexcanConfig.timingConfig.fphaseSeg1 = FPSEG1;
    flexcanConfig.timingConfig.fphaseSeg2 = FPSEG2;
    flexcanConfig.timingConfig.fpropSeg   = FPROPSEG;
#endif
#endif

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
#if (defined(USE_CANFD) && USE_CANFD)
    if (FLEXCAN_FDCalculateImprovedTimingValues(flexcanConfig.baudRate, flexcanConfig.baudRateFD, EXAMPLE_CAN_CLK_FREQ,
                                                &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        PRINTF("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#else
    if (FLEXCAN_CalculateImprovedTimingValues(flexcanConfig.baudRate, EXAMPLE_CAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        PRINTF("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#endif
#endif

#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_FDInit(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ, BYTES_IN_MB, true);
#else
    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);
#endif

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* Set Rx Masking mechanism. */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_MB_STD_MASK(rxIdentifier, 0, 0));

    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type   = kFLEXCAN_FrameTypeData;
    mbConfig.id     = FLEXCAN_ID_STD(rxIdentifier);
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_SetFDRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);
#else
    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);
#endif

/* Setup Tx Message Buffer. */
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_SetFDTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
#else
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
#endif

    if ((node_type == 'A') || (node_type == 'a'))
    {
        PRINTF("Press any key to trigger one-shot transmission\r\n\r\n");
        frame.dataByte0 = 0;
    }
    else
    {
        PRINTF("Start to Wait data from Node A\r\n\r\n");
    }

    while (1)
    {
        if ((node_type == 'A') || (node_type == 'a'))
        {
            GETCHAR();

            frame.id     = FLEXCAN_ID_STD(txIdentifier);
            frame.format = kFLEXCAN_FrameFormatStandard;
            frame.type   = kFLEXCAN_FrameTypeData;
            frame.length = DLC;
#if (defined(USE_CANFD) && USE_CANFD)
            frame.brs = 1;
#endif
            txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
            txXfer.framefd = &frame;
            FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#else
            txXfer.frame = &frame;
            FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#endif

            while (!txComplete)
            {
            };
            txComplete = false;

            /* Start receive data through Rx Message Buffer. */
            rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
            rxXfer.framefd = &frame;
            FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#else
            rxXfer.frame = &frame;
            FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#endif

            /* Wait until Rx MB full. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            PRINTF("Rx MB ID: 0x%3x, Rx MB data: 0x%x, Time stamp: %d\r\n", frame.id >> CAN_ID_STD_SHIFT,
                   frame.dataByte0, frame.timestamp);
            PRINTF("Press any key to trigger the next transmission!\r\n\r\n");
            frame.dataByte0++;
            frame.dataByte1 = 0x55;
        }
        else
        {
            /* Before this , should first make node B enter STOP mode after FlexCAN
             * initialized with enableSelfWakeup=true and Rx MB configured, then A
             * sends frame N which wakes up node B. A will continue to send frame N
             * since no acknowledgement, then B received the second frame N(In the
             * application it seems that B received the frame that woke it up which
             * is not expected as stated in the reference manual, but actually the
             * output in the terminal B received is the same second frame N). */
            if (wakenUp)
            {
                PRINTF("B has been waken up!\r\n\r\n");
            }

            /* Start receive data through Rx Message Buffer. */
            rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
            rxXfer.framefd = &frame;
            FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#else
            rxXfer.frame = &frame;
            FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#endif

            /* Wait until Rx receive full. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            PRINTF("Rx MB ID: 0x%3x, Rx MB data: 0x%x, Time stamp: %d\r\n", frame.id >> CAN_ID_STD_SHIFT,
                   frame.dataByte0, frame.timestamp);

            frame.id     = FLEXCAN_ID_STD(txIdentifier);
            txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
            frame.brs      = 1;
            txXfer.framefd = &frame;
            FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#else
            txXfer.frame = &frame;
            FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#endif

            while (!txComplete)
            {
            };
            txComplete = false;
            PRINTF("Wait Node A to trigger the next transmission!\r\n\r\n");
        }
    }
}
