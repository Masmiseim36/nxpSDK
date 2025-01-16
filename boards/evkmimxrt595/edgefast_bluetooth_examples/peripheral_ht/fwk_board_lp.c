/* -------------------------------------------------------------------------- */
/*                           Copyright 2023 NXP                               */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "board.h"
#include "pin_mux.h"
#include "fwk_board_lp.h"
#include "fwk_platform_lowpower.h"
#include "fsl_pm_core.h"
#include "fwk_debug.h"
#include "fsl_usart.h"
#include "fsl_debug_console.h"
#ifdef CONFIG_BT_SETTINGS
#include "fsl_adapter_flash.h"
#include "fsl_flexspi.h"
#endif

/* -------------------------------------------------------------------------- */
/*                            Private definitions                             */
/* -------------------------------------------------------------------------- */

#define NOR_CMD_LUT_SEQ_IDX_SLEEP_OPI 11
#define NOR_CMD_LUT_SEQ_IDX_WKUP_OPI  12

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Callback registered to SDK Power Manager to get notified of entry/exit of low power modes
 *
 * \param[in] eventType event specifying if we entered or exited from low power mode
 * \param[in] powerState low power mode used during low power period
 * \param[in] data Optional data passed when the callback got registered (not used currently)
 * \return status_t
 */
static status_t BOARD_LowpowerCallback(pm_event_type_t eventType, uint8_t powerState, void *data);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static uint32_t            s_DebugUart[3];
static uint32_t            s_ISPPin[3];
static pm_notify_element_t boardLpNotifyGroup = {
    .notifyCallback = BOARD_LowpowerCallback,
    .data           = NULL,
};

#if 0
#if defined(APP_MEM_POWER_OPT) && (APP_MEM_POWER_OPT > 0)
static bool s_PSRAMDeepPowerDown = false;
#endif /* APP_MEM_POWER_OPT */
#endif

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void BOARD_LowPowerInit(void)
{
    status_t status;

    status = PM_RegisterNotify(kPM_NotifyGroup2, &boardLpNotifyGroup);
    assert(status == kStatus_Success);
    (void)status;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static status_t BOARD_PSRAMOperate(
    FLEXSPI_Type *base, uint8_t regAddr, uint32_t *mrVal, flexspi_command_type_t type, uint8_t index, uint8_t size)
{
    flexspi_transfer_t flashXfer;
    status_t           status;

    flashXfer.deviceAddress = regAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = type;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = index;
    flashXfer.data          = mrVal;
    flashXfer.dataSize      = size;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

static status_t BOARD_PSRAMEnterDeepPowerDownMode(void)
{
    status_t status;
    uint32_t mr1Val[1];
    uint32_t mr0mr1[1];
    uint32_t mr6Val[1];
    mr6Val[0] = 0xC0;
    /* Read data */
    status = BOARD_PSRAMOperate(BOARD_FLEXSPI_PSRAM, 0x0, mr0mr1, kFLEXSPI_Read, 2, 2);
    if (status != kStatus_Success)
    {
        return status;
    }
    mr1Val[0] = (mr0mr1[0] & 0xFF00U) >> 8;
    if (mr1Val[0] & 0x80)
    {
        /* Write data */
        status = BOARD_PSRAMOperate(BOARD_FLEXSPI_PSRAM, 0x6, mr6Val, kFLEXSPI_Write, 3, 1);
        SDK_DelayAtLeastUs(200U, SystemCoreClock);
    }

    if (status != kStatus_Success)
    {
        return status;
    }
    return status;
}

static void BOARD_FlashOperate(FLEXSPI_Type *base, uint32_t baseAddr, uint8_t index)
{
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = index;
    FLEXSPI_TransferBlocking(base, &flashXfer);
}

static void BOARD_FlashEnterDPMode(FLEXSPI_Type *base, uint32_t baseAddr)
{
    /* Please notice that after calling BOARD_FlashEnterDPMode(),
     * all functions and data which will be excuted should be placed in RAM untill BOARD_FlashExitDPMode() is called. */
    BOARD_FlashOperate(FLEXSPI0, 0, NOR_CMD_LUT_SEQ_IDX_SLEEP_OPI);
    /* SDK_DelayAtLeastUs(10U, SystemCoreClock); */
    /* Please aviod using SDK_DelayAtLeastUs() here. Some compilers may optimize SDK_DelayAtLeastUs(),
     * causing excute certain lib functions which are not yet replaced into RAM.
     * Or repalce all these lib(.o files) into RAM. */
    /* for loop of 50000 is about 1ms (@200 MHz CPU) */
    for (uint32_t i = 0; i < 500; i++)
    {
        __NOP();
    }
}

static void BOARD_FlashExitDPMode(FLEXSPI_Type *base, uint32_t baseAddr)
{
    BOARD_FlashOperate(FLEXSPI0, 0, NOR_CMD_LUT_SEQ_IDX_WKUP_OPI);
    /* SDK_DelayAtLeastUs(30U, SystemCoreClock); */
    for (uint32_t i = 0; i < 1500; i++)
    {
        __NOP();
    }
}

static void BOARD_SetLowPowerMEM(void)
{
    /* Power domain: MEM_1V8 */
    /* PSRAM is not stable when enter sleep mode. */
#if 0
#if defined(APP_MEM_POWER_OPT) && (APP_MEM_POWER_OPT > 0)
    if (!s_PSRAMDeepPowerDown)
    {
        BOARD_PSRAMEnterDeepPowerDownMode();
        s_PSRAMDeepPowerDown = true;
    }
#endif /* APP_MEM_POWER_OPT */
#endif

#if defined(CONFIG_BT_SETTINGS) && (CONFIG_BT_SETTINGS > 0)
    BOARD_FlashEnterDPMode(FLEXSPI0, 0);
#endif /* CONFIG_BT_SETTINGS */
}

static void BOARD_RestoreLowPowerMEM(void)
{
#if defined(CONFIG_BT_SETTINGS) && (CONFIG_BT_SETTINGS > 0)
    BOARD_FlashExitDPMode(FLEXSPI0, 0);
#endif /* CONFIG_BT_SETTINGS */
}

static void BOARD_SetLowPowerISPPins(void)
{
    /* Power domain: VDDIO_0*/
    s_ISPPin[0]        = IOPCTL->PIO[1][15];
    s_ISPPin[1]        = IOPCTL->PIO[3][28];
    s_ISPPin[2]        = IOPCTL->PIO[3][29];
    IOPCTL->PIO[1][15] = 0;
    IOPCTL->PIO[3][28] = 0;
    IOPCTL->PIO[3][29] = 0;
}

static void BOARD_RestoreLowPowerISPPins(void)
{
    IOPCTL->PIO[1][15] = s_ISPPin[0];
    IOPCTL->PIO[3][28] = s_ISPPin[1];
    IOPCTL->PIO[3][29] = s_ISPPin[2];
}

static void BOARD_SetLowPowerDebugUartPins(void)
{
    /* Power domain: VDDIO_3 & VDDIO_1. */
    /* Power will not be decreased untill debugging device is unplugged at hardware level. */
    uint8_t num = 0;

    /* Wait for debug console output finished. */
    while (((uint32_t)kUSART_TxIdleFlag & USART_GetStatusFlags(USART12)) == 0U)
    {
    }

    for (; num < 2; num++)
    {
        s_DebugUart[num]         = IOPCTL->PIO[4][num + 30];
        IOPCTL->PIO[4][num + 30] = 0;
    }

    s_DebugUart[2]     = IOPCTL->PIO[2][24];
    IOPCTL->PIO[2][24] = 0;
}

static void BOARD_RestoreLowPowerDebugUartPins(void)
{
    uint8_t num = 0;
    for (; num < 2; num++)
    {
        IOPCTL->PIO[4][num + 30] = s_DebugUart[num];
    }

    IOPCTL->PIO[2][24] = s_DebugUart[2];
}

static void BOARD_EnterDeepSleepCb(void)
{
    BOARD_SetLowPowerDebugUartPins();
    BOARD_SetLowPowerISPPins();
    BOARD_SetLowPowerMEM();
}

static void BOARD_ExitDeepSleepCb(void)
{
    BOARD_RestoreLowPowerMEM();
    BOARD_RestoreLowPowerDebugUartPins();
    BOARD_RestoreLowPowerISPPins();
}

/* -------------------------------------------------------------------------- */
/*                              Low Power callbacks                           */
/* -------------------------------------------------------------------------- */

static status_t BOARD_LowpowerCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    status_t ret = kStatus_Success;

    if (powerState >= PLATFORM_DEEP_SLEEP_STATE)
    {
        if (eventType == kPM_EventEnteringSleep)
        {
            BOARD_EnterDeepSleepCb();
        }
        else
        {
            BOARD_ExitDeepSleepCb();
        }
    }
    else
    {
        /* Nothing to do when entering WFI or Sleep low power state
         * NVIC fully functional to trigger upcoming interrupts */
    }

    return ret;
}
