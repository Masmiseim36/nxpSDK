/*
 * Copyright 2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/*
 * I2C implmentation for ICs related to LPC54018 & Family of MCUs
 */

#include <board.h>

#if defined(FSL_FEATURE_SOC_FLEXCOMM_COUNT) && FSL_FEATURE_SOC_FLEXCOMM_COUNT > 0

#include "i2c_a7.h"
#include "fsl_common.h"

#include "fsl_clock.h"
#include "fsl_i2c.h"
#include "sm_printf.h"

#if defined(SDK_OS_FREE_RTOS) && SDK_OS_FREE_RTOS == 1
#include "fsl_i2c_freertos.h"
#endif

#include "sm_timer.h"
#include <stdio.h>
#include "fsl_gpio.h"
#include "sci2c_cfg.h"

//#define DELAY_I2C_US          (T_CMDG_USec)
#define DELAY_I2C_US (0)

// #define I2C_DEBUG
#define I2C_LOG_PRINTF printf

#define AX_I2CM I2C4
#define AX_I2C_CLK (12000000)
#define AX_I2CM_IRQN FLEXCOMM4_IRQn
#define I2C_BAUDRATE (400000) /* 400K */

// #if defined(SDK_OS_FREE_RTOS) && SDK_OS_FREE_RTOS == 1
// i2c_rtos_handle_t gmaster_rtos_handle;
// #endif
#if defined(I2C_DEBUG) || 1
#define DEBUG_PRINT_KINETIS_I2C(Operation, status)                                                \
    if (result == kStatus_Success) { /* I2C_LOG_PRINTF(Operation " OK\r\n");                   */ \
    }                                                                                             \
    else if (result == kStatus_I2C_Addr_Nak) { /* I2C_LOG_PRINTF(Operation " A-Nak\r\n");      */ \
    }                                                                                             \
    else if (result == kStatus_I2C_Busy)                                                          \
        I2C_LOG_PRINTF(Operation " Busy\r\n");                                                    \
    else if (result == kStatus_I2C_Idle)                                                          \
        I2C_LOG_PRINTF(Operation " Idle\r\n");                                                    \
    else if (result == kStatus_I2C_Nak)                                                           \
        I2C_LOG_PRINTF(Operation " Nak\r\n");                                                     \
    else if (result == kStatus_I2C_Timeout)                                                       \
        I2C_LOG_PRINTF(Operation " T/O\r\n");                                                     \
    else if (result == kStatus_I2C_ArbitrationLost)                                               \
        I2C_LOG_PRINTF(Operation " ArbtnLost\r\n");                                               \
    else                                                                                          \
        I2C_LOG_PRINTF(Operation " ERROR  : 0x%02lX\r\n", status);
#else
#define DEBUG_PRINT_KINETIS_I2C(Operation, status)
#endif
static int gBackoffDelay;

void axI2CResetBackoffDelay()
{
    gBackoffDelay = 0;
}

static void BackOffDelay_Wait()
{
    if (gBackoffDelay < 200)
        gBackoffDelay += 1;
    sm_sleep(gBackoffDelay);
}

static i2c_error_t kinetisI2cStatusToAxStatus(status_t kinetis_i2c_status)
{
    i2c_error_t retStatus;
    switch (kinetis_i2c_status) {
    case kStatus_Success:
        axI2CResetBackoffDelay();
        retStatus = I2C_OK;
        break;
    case kStatus_I2C_Busy:
        retStatus = I2C_BUSY;
        break;
    case kStatus_I2C_Idle:
        retStatus = I2C_BUSY;
        break;
    case kStatus_I2C_Nak:
        retStatus = I2C_NACK_ON_DATA;
        break;
    case kStatus_I2C_ArbitrationLost:
        retStatus = I2C_ARBITRATION_LOST;
        break;
    case kStatus_I2C_Timeout:
        retStatus = I2C_TIME_OUT;
        break;
    case kStatus_I2C_Addr_Nak:
        BackOffDelay_Wait();
        retStatus = I2C_NACK_ON_ADDRESS;
        break;
    default:
        retStatus = I2C_FAILED;
        break;
    }
    return retStatus;
}

#define RETURN_ON_BAD_I2cStatus(kinetis_i2c_status)                             \
    {                                                                           \
        i2c_error_t ax_status = kinetisI2cStatusToAxStatus(kinetis_i2c_status); \
        if (ax_status != I2C_OK)                                                \
            return ax_status;                                                   \
    }

#if defined(SDK_OS_FREE_RTOS) && SDK_OS_FREE_RTOS == 1
i2c_rtos_handle_t gmaster_rtos_handle;
#endif
i2c_error_t axI2CInit(void)
{
    i2c_master_config_t masterConfig;

    /* Get default configuration for master. */
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = I2C_BAUDRATE;
    /* Init I2C master. */
#if defined(SDK_OS_FREE_RTOS) && SDK_OS_FREE_RTOS == 1
    NVIC_SetPriority(AX_I2CM_IRQN, 3);
    EnableIRQ(AX_I2CM_IRQN);
    I2C_RTOS_Init(&gmaster_rtos_handle, AX_I2CM, &masterConfig, AX_I2C_CLK);
#else
    I2C_MasterInit(AX_I2CM, &masterConfig, AX_I2C_CLK);
#endif
    return I2C_OK;
}

void axI2CTerm(int mode)
{
}

#if defined(SDK_OS_FREE_RTOS) && SDK_OS_FREE_RTOS == 1
#define I2CM_TX() result = I2C_RTOS_Transfer(&gmaster_rtos_handle, &masterXfer)
#else
#define I2CM_TX() result = I2C_MasterTransferBlocking(AX_I2CM, &masterXfer)
#endif

unsigned int axI2CWrite(unsigned char bus_unused_param, unsigned char addr, unsigned char *pTx, unsigned short txLen)
{
    status_t result;
    i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer)); //clear values

#if defined(SCI2C_DEBUG)
    I2C_LOG_PRINTF("\r\n I2C Write \r\n");
#endif
    masterXfer.slaveAddress = addr >> 1; // the address of the A70CM
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pTx;
    masterXfer.dataSize = txLen;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    /* Send master blocking data to slave */
    result = I2C_MasterTransferBlocking(AX_I2CM, &masterXfer);
    DEBUG_PRINT_KINETIS_I2C("WR", result);
    RETURN_ON_BAD_I2cStatus(result);
    return I2C_OK;
}

#if defined SCI2C
//for ARM6 taken care at file level
// #ifdef __ARMCC_VERSION
// #pragma O0
// #endif

#ifdef __ICCARM__
#pragma optimize = none
#endif

#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
__attribute__((optimize("O0")))
#endif

unsigned int
axI2CWriteRead(unsigned char bus_unused_param,
    unsigned char addr,
    unsigned char *pTx,
    unsigned short txLen,
    unsigned char *pRx,
    unsigned short *pRxLen)
{
    status_t result;
    i2c_master_transfer_t masterXfer;
    uint32_t master_state;
    uint32_t status = 0;
    memset(&masterXfer, 0, sizeof(masterXfer)); //clear values
    *pRxLen = 0;
    memset(pRx, 0, 2);

#if defined(SCI2C_DEBUG)
    I2C_LOG_PRINTF("\r\n I2C Write \r\n");
#endif
    masterXfer.slaveAddress = addr >> 1; // the address of the A70CM
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pTx;
    masterXfer.dataSize = txLen;
    masterXfer.flags = kI2C_TransferNoStopFlag;

    /* Send master blocking data to slave */
    result = I2C_MasterTransferBlocking(AX_I2CM, &masterXfer);
#if defined(I2C_DEBUG) || 0
    I2C_LOG_PRINTF("\r\n I2C_WriteRead\r\n");
    I2C_LOG_PRINTF("\r\n WRITE[R]: 0x%04lX", result);
    //for(int i = 0; i < txLen; i++)
    //I2C_LOG_PRINTF("%.2X ", pTx[i]);
#endif //I2C_DEBUG
    RETURN_ON_BAD_I2cStatus(result)

        // Reading from A71CH

        masterXfer.slaveAddress = addr >> 1; // the address of the A70CM
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pRx;
    masterXfer.dataSize = 0;
    masterXfer.flags = kI2C_TransferRepeatedStartFlag;

    /* Send master blocking data to slave */
    result = I2C_MasterTransferBlocking(AX_I2CM, &masterXfer);
//    DEBUG_PRINT_KINETIS_I2C("RD1", result);
    RETURN_ON_BAD_I2cStatus(result);

    volatile uint8_t nRead = 0;
    if (result == kStatus_Success) {
        do {
            status = AX_I2CM->STAT;
        } while ((status & I2C_STAT_MSTPENDING_MASK) == 0);
        AX_I2CM->STAT = (I2C_STAT_MSTARBLOSS_MASK | I2C_STAT_MSTSTSTPERR_MASK);

        nRead = AX_I2CM->MSTDAT;

        AX_I2CM->MSTCTL = I2C_MSTCTL_MSTCONTINUE_MASK;
        *pRxLen = nRead + 1;
        *pRx++ = nRead;

        while (nRead--) {
            do {
                status = I2C_GetStatusFlags(AX_I2CM);
            } while ((status & I2C_STAT_MSTPENDING_MASK) == 0);

            AX_I2CM->STAT = (I2C_STAT_MSTARBLOSS_MASK | I2C_STAT_MSTSTSTPERR_MASK);

            if (status & (I2C_STAT_MSTARBLOSS_MASK | I2C_STAT_MSTSTSTPERR_MASK)) {
                break;
            }

            master_state = (status & I2C_STAT_MSTSTATE_MASK) >> I2C_STAT_MSTSTATE_SHIFT;
            switch (master_state) {
            case I2C_STAT_MSTCODE_RXREADY:
                /* ready to send next byte */
                if (nRead == 0) {
                    AX_I2CM->MSTCTL = I2C_MSTCTL_MSTSTOP_MASK;
                    do {
                        status = I2C_GetStatusFlags(AX_I2CM);
                    } while ((status & I2C_STAT_MSTPENDING_MASK) == 0);
                    AX_I2CM->STAT = (I2C_STAT_MSTARBLOSS_MASK | I2C_STAT_MSTSTSTPERR_MASK);
                }
                if (nRead) {
                    AX_I2CM->MSTCTL = I2C_MSTCTL_MSTCONTINUE_MASK;
                }
                *(pRx++) = AX_I2CM->MSTDAT;
                break;
            case I2C_STAT_MSTCODE_NACKADR:
                result = kStatus_I2C_Addr_Nak;
                break;
            case I2C_STAT_MSTCODE_NACKDAT:
                /* slave nacked the last byte */
                result = kStatus_I2C_Nak;
                break;

            default:
                /* unexpected state */
                result = kStatus_I2C_UnexpectedState;
                break;
            }

            if (result != kStatus_Success) {
                RETURN_ON_BAD_I2cStatus(result);
            }
        }

        if (status & I2C_STAT_MSTARBLOSS_MASK) {
            result = kStatus_I2C_ArbitrationLost;
            RETURN_ON_BAD_I2cStatus(result);
        }

        if (status & I2C_STAT_MSTSTSTPERR_MASK) {
            result = kStatus_I2C_StartStopError;
            RETURN_ON_BAD_I2cStatus(result);
        }
    }
    return I2C_OK;
}
#endif /*SCI2C*/

unsigned int axI2CRead(unsigned char bus, unsigned char addr, unsigned char *pRx, unsigned short rxLen)
{
    i2c_master_transfer_t masterXfer;
    status_t result;
    memset(&masterXfer, 0, sizeof(masterXfer)); //clear values

#if defined(SCI2C_DEBUG)
    I2C_LOG_PRINTF("\r\n I2C Read \r\n");
#endif

    masterXfer.slaveAddress = addr >> 1; // the address of the A70CM
    //masterXfer.slaveAddress = addr;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pRx;
    masterXfer.dataSize = rxLen;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    I2CM_TX();

//    DEBUG_PRINT_KINETIS_I2C("RD2", result);
    RETURN_ON_BAD_I2cStatus(result);

    return I2C_OK;
}

#endif
