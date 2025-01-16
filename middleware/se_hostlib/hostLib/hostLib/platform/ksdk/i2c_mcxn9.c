/*
 *
 * Copyright 2024 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * I2C implmentation for ICs related to MCX Family
 */

#include <board.h>
#include "se_board_config.h"

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && FSL_FEATURE_SOC_LPI2C_COUNT > 0  && defined(MCXN)
#include "i2c_a7.h"
#include "fsl_clock.h"
#include "fsl_lpi2c.h"
#if defined(SDK_OS_FREE_RTOS) && SDK_OS_FREE_RTOS == 1
#include "fsl_i2c_freertos.h"
#endif
#include "fsl_port.h"
#include "sm_timer.h"
#include <stdio.h>
#include "fsl_gpio.h"
#include "sci2c_cfg.h"
#include "sm_printf.h"

#define AX_I2CM        ((LPI2C_Type *)(LPI2C2_BASE))
#define AX_I2C_CLK_SRC  CLOCK_GetLPFlexCommClkFreq(2u)
// #define AX_I2CM_IRQN   I2C2_IRQn

#if defined(SCI2C)
#define I2C_BAUDRATE (400u * 1000u) // 400K
#elif defined(T1oI2C)
//#define I2C_BAUDRATE (3400u * 1000u) // 3.4. Not used by default
#define I2C_BAUDRATE 100000U
#else
#error "Invalid combination"
#endif

//#define I2C_DEBUG
//#define DELAY_I2C_US          (T_CMDG_USec)
#define DELAY_I2C_US            (0)

#define I2C_LOG_PRINTF PRINTF

#if defined(FLOW_SILENT)
#define DEBUG_PRINT_KINETIS_I2C(Operation, status)
#elif 1 || defined(I2C_DEBUG) || 1
#define DEBUG_PRINT_KINETIS_I2C(Operation, status)                                                         \
    if(result == kStatus_Success) { /* I2C_LOG_PRINTF(Operation " OK\r\n");                   */ }         \
    else if(result == kStatus_LPI2C_Busy) I2C_LOG_PRINTF(Operation " Busy\r\n");                           \
    else if(result == kStatus_LPI2C_Idle) I2C_LOG_PRINTF(Operation " Idle\r\n");                           \
    else if(result == kStatus_LPI2C_Nak) {/* I2C_LOG_PRINTF(Operation " Nak\r\n"); */}                     \
    else if(result == kStatus_LPI2C_Timeout) I2C_LOG_PRINTF(Operation " T/O\r\n");                         \
    else if(result == kStatus_LPI2C_ArbitrationLost) I2C_LOG_PRINTF(Operation " ArbtnLost\r\n");           \
    else I2C_LOG_PRINTF (Operation " ERROR  : 0x%02lX\r\n", status);
#else
#   define DEBUG_PRINT_KINETIS_I2C(Operation, status)
#endif

/* Handle NAK from the A71CH */
static int gBackoffDelay;

void axI2CResetBackoffDelay() {
    gBackoffDelay = 0;
}

static void BackOffDelay_Wait() {
    if (gBackoffDelay < 200 ) {
        gBackoffDelay += 1;
    }
    sm_sleep(gBackoffDelay);
}

static i2c_error_t kinetisI2cStatusToAxStatus(
    status_t kinetis_i2c_status)
{
    i2c_error_t retStatus;
    switch (kinetis_i2c_status)
    {
    case kStatus_Success:
        axI2CResetBackoffDelay();
        retStatus = I2C_OK;
        break;
    case kStatus_LPI2C_Busy:
        BackOffDelay_Wait();
        retStatus = I2C_BUSY;
        break;
    case kStatus_LPI2C_Idle:
        retStatus = I2C_BUSY;
        break;
    case kStatus_LPI2C_Nak:
        BackOffDelay_Wait();
        retStatus = I2C_NACK_ON_DATA;
        break;
    case kStatus_LPI2C_ArbitrationLost:
        retStatus = I2C_ARBITRATION_LOST;
        break;
    case kStatus_LPI2C_Timeout:
        retStatus = I2C_TIME_OUT;
        break;
    // case kStatus_I2C_Addr_Nak:
    //     BackOffDelay_Wait();
    //     retStatus = I2C_NACK_ON_ADDRESS;
    //     break;
    default:
        retStatus = I2C_FAILED;
        break;
    }
    return retStatus;
}

#define RETURN_ON_BAD_kinetisI2cStatus(kinetis_i2c_status)                      \
    {                                                                           \
        i2c_error_t ax_status = kinetisI2cStatusToAxStatus(kinetis_i2c_status); \
        if ( ax_status != I2C_OK )                                              \
            return ax_status;                                                   \
    }

#if defined(SDK_OS_FREE_RTOS) && SDK_OS_FREE_RTOS == 1
i2c_rtos_handle_t gmaster_rtos_handle;
#endif

i2c_error_t axI2CInit(void **conn_ctx, const char *pDevName)
{
    lpi2c_master_config_t masterConfig;

    /*
     * Default configuration:
     * masterConfig.baudRate_Bps = 100000U;
     * masterConfig.enableHighDrive = false;
     * masterConfig.enableStopHold = false;
     * masterConfig.glitchFilterWidth = 0U;
     * masterConfig.enableMaster = true;
     */

    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = I2C_BAUDRATE;
    
#if defined(SDK_OS_FREE_RTOS) && SDK_OS_FREE_RTOS == 1
    NVIC_SetPriority(AX_I2CM_IRQN, 3);
    I2C_RTOS_Init(&gmaster_rtos_handle, AX_I2CM, &masterConfig, sourceClock);
#else
    LPI2C_MasterInit(AX_I2CM, &masterConfig, AX_I2C_CLK_SRC);
#endif
    return I2C_OK;
}

void axI2CTerm(void* conn_ctx, int mode)
{
#if defined(SDK_OS_FREE_RTOS) && SDK_OS_FREE_RTOS == 1
    I2C_RTOS_Deinit(&gmaster_rtos_handle);
#endif
}

#if defined(SDK_OS_FREE_RTOS) && SDK_OS_FREE_RTOS == 1
#    define    I2CM_TX() \
        result = I2C_RTOS_Transfer(&gmaster_rtos_handle, &masterXfer)
#else
#    define    I2CM_TX() \
        result = LPI2C_MasterTransferBlocking(AX_I2CM, &masterXfer)
#endif

unsigned int axI2CWrite(
    void* conn_ctx, unsigned char bus_unused_param, unsigned char addr, unsigned char * pTx, unsigned short txLen)
{
    status_t result;
    lpi2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer)); //clear values

    if(pTx == NULL || txLen > MAX_DATA_LEN)
    {
        return I2C_FAILED;
    }

#if defined(SCI2C_DEBUG)
    I2C_LOG_PRINTF("\r\n SCI2C Write \r\n");
#endif
    masterXfer.slaveAddress = addr >> 1; // the address of the A70CM
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pTx;
    masterXfer.dataSize = txLen;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    I2CM_TX();

    DEBUG_PRINT_KINETIS_I2C("WR",result);
    RETURN_ON_BAD_kinetisI2cStatus(result);

    return I2C_OK;
}

#if defined SCI2C

//for ARM6 taken care at file level
// #ifdef __ARMCC_VERSION
// #pragma O0
// #endif


#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
__attribute__((optimize("O0")))
#endif

unsigned int axI2CWriteRead(
    void* conn_ctx, unsigned char bus_unused_param, unsigned char addr, unsigned char * pTx, unsigned short txLen, unsigned char * pRx,
    unsigned short * pRxLen)
{
    lpi2c_master_transfer_t masterXfer;
    status_t result;
    memset(&masterXfer, 0, sizeof(masterXfer)); //clear values

    if(pTx == NULL || txLen > MAX_DATA_LEN)
    {
        return I2C_FAILED;
    }

    if(pRx == NULL || *pRxLen > MAX_DATA_LEN)
    {
        return I2C_FAILED;
    }

    *pRxLen = 0;
    memset(pRx, 0, 2);

    //Write

    masterXfer.slaveAddress = addr >> 1; // the address of the A70CM
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pTx;
    masterXfer.dataSize = txLen;
    masterXfer.flags = kI2C_TransferNoStopFlag;

    result = LPI2C_MasterTransferBlocking(AX_I2CM, &masterXfer);

    DEBUG_PRINT_KINETIS_I2C("WR",result);

#if defined(I2C_DEBUG) || 0
    I2C_LOG_PRINTF("\r\n I2C_WriteRead\r\n");
    I2C_LOG_PRINTF("\r\n WRITE[R]: 0x%04lX", result);
    //for(int i = 0; i < txLen; i++)
    //I2C_LOG_PRINTF("%.2X ", pTx[i]);
#endif //I2C_DEBUG
    RETURN_ON_BAD_kinetisI2cStatus(result);

    masterXfer.slaveAddress = addr >> 1; // the address of the A70CM
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pRx;
    masterXfer.dataSize = 0; //We don't read anything here.
    masterXfer.flags = kI2C_TransferRepeatedStartFlag;

    result = LPI2C_MasterTransferBlocking(AX_I2CM, &masterXfer);

    DEBUG_PRINT_KINETIS_I2C("RD",result);
    RETURN_ON_BAD_kinetisI2cStatus(result);

    if (result == kStatus_Success)
    {
        volatile uint8_t nRead = 0; /* volatile for optimized builds */

        /* Wait until the data register is ready for transmit. */
        while (!(AX_I2CM->S & (kI2C_TransferCompleteFlag)))
        {
        }

        /* Clear the IICIF flag. */
        AX_I2CM->S = kI2C_IntPendingFlag;

        /* Setup the I2C peripheral to receive data. */
        AX_I2CM->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);

        /* Do dummy read. */
        nRead = AX_I2CM->D;

        /* Wait until data transfer complete. */
        while (!(AX_I2CM->S & kI2C_IntPendingFlag))
        {
        }

        /* Clear the IICIF flag. */
        AX_I2CM->S = kI2C_IntPendingFlag;

        //Now we get the lenght information.
        nRead = AX_I2CM->D;
        *pRxLen = nRead + 1;
        *pRx++ = nRead;

        if (nRead == 1)
        {
            /* Issue NACK on read. */
            AX_I2CM->C1 |= I2C_C1_TXAK_MASK;
        }

        while ((nRead--))
        {
            /* Wait until data transfer complete. */
            while (!(AX_I2CM->S & (kI2C_TransferCompleteFlag)))
            {
            }

            /* Clear the IICIF flag. */
            AX_I2CM->S = kI2C_IntPendingFlag;


            /* Single byte use case. */
            if (nRead == 0)
            {
                /* Issue STOP command before reading last byte. */
                result = LPI2C_MasterStop(AX_I2CM);
            }

            if (nRead == 1)
            {
                /* Issue NACK on read. */
                AX_I2CM->C1 |= I2C_C1_TXAK_MASK;
            }


            /* Read from the data register. */
            *pRx++ = AX_I2CM->D;
        }

#if defined(I2C_DEBUG) || 1
        //pRx -= *pRxLen;
        //for(int i = 0; i < *pRxLen; i++)
        //I2C_LOG_PRINTF("%.2X ", pRx[i]);
#endif
        return I2C_OK;
    }
    return I2C_FAILED;
}
#endif /*SCI2C*/

unsigned int axI2CRead(void* conn_ctx, unsigned char bus, unsigned char addr, unsigned char * pRx, unsigned short rxLen)
{
    lpi2c_master_transfer_t masterXfer;
    status_t result;
    memset(&masterXfer, 0, sizeof(masterXfer)); //clear values

    if(pRx == NULL || rxLen > MAX_DATA_LEN)
    {
        return I2C_FAILED;
    }

#if defined(SCI2C_DEBUG)
    I2C_LOG_PRINTF("\r\n SCI2C Read \r\n");
#endif

    masterXfer.slaveAddress = addr >> 1; // the address of the A70CM
    //masterXfer.slaveAddress = addr;
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pRx;
    masterXfer.dataSize = rxLen;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    I2CM_TX();

    DEBUG_PRINT_KINETIS_I2C("RD",result);
    RETURN_ON_BAD_kinetisI2cStatus(result);

    return I2C_OK;
    
    
    
//    if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_I2C_MASTER, LPI2C_MASTER_SLAVE_ADDR_7BIT, kLPI2C_Read)) {
//        LPI2C_MasterGetFifoCounts(EXAMPLE_I2C_MASTER, NULL, &txCount);
//        while (txCount)
//        {
//            LPI2C_MasterGetFifoCounts(EXAMPLE_I2C_MASTER, NULL, &txCount);
//        }
//        /* Check communicate with slave successful or not */
//        if (LPI2C_MasterGetStatusFlags(EXAMPLE_I2C_MASTER) & kLPI2C_MasterNackDetectFlag)
//        {
//            return kStatus_LPI2C_Nak;
//        }
//
//        reVal = LPI2C_MasterReceive(EXAMPLE_I2C_MASTER, g_master_rxBuff, LPI2C_DATA_LENGTH);
//        if (reVal != kStatus_Success)
//        {
//            if (reVal == kStatus_LPI2C_Nak)
//            {
//                LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
//            }
//            return -1;
//        }
//
//        reVal = LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
//        if (reVal != kStatus_Success)
//        {
//            return -1;
//        }
//    }
    
}

#endif /* FSL_FEATURE_SOC_I2C_COUNT */
