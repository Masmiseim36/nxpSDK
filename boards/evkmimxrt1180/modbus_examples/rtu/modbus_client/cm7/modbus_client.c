/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lpuart.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "port.h"

#include "mb_m.h"
#include "mbport.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_IMX_RT1180 (1)

#define COIL_START_ADDR                 0           /*coil register start address*/
#define COIL_NUMS                       10          /*coil register numbers*/
 
#define DISCRETE_INPUT_START_ADDR       10000	    /*discrete register start address*/
#define DISCRETE_INPUT_NUMS             10          /*discrete register numbers*/
 
#define INPUT_REGISTER_START_ADDR       30000		/*input register start address*/
#define INPUT_REGISTER_NUMS             10		    /*input register numbers*/
 
#define HOLDING_REGISTER_START_ADDR     40000		/*holding register start address*/
#define HOLDING_REGISTER_NUMS           10		    /*holding register numbers*/

#define M_DISCRETE_INPUT_START        0
#define M_DISCRETE_INPUT_NDISCRETES   16
#define M_COIL_START                  0
#define M_COIL_NCOILS                 64
#define M_REG_INPUT_START             0
#define M_REG_INPUT_NREGS             100
#define M_REG_HOLDING_START           0
#define M_REG_HOLDING_NREGS           100
/* master mode: holding register's all address */
#define          M_HD_RESERVE                     0
/* master mode: input register's all address */
#define          M_IN_RESERVE                     0
/* master mode: coil's all address */
#define          M_CO_RESERVE                     0
/* master mode: discrete's all address */
#define          M_DI_RESERVE                     0

/* ------------------------ Implementation -------------------------------- */
eMBErrorCode eMBMasterRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    return eStatus;
}

eMBErrorCode eMBMasterRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
    USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    return eStatus;
}

eMBErrorCode eMBMasterRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress,
    USHORT usNCoils, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    return eStatus;
}

eMBErrorCode eMBMasterRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    return eStatus;
}

eMBErrorCode
eMBMasterLedStatusCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    return eStatus;
}

void BOARD_TPM_SECOND_HANDLER(void)
{
    /* Clear interrupt flag.*/
    TPM_ClearStatusFlags(BOARD_TPM_SECOND, kTPM_TimeOverflowFlag);
    eMBMasterPoll( );
    SDK_ISR_EXIT_BARRIER;
}

void TPM2_Init(void)
{
    tpm_config_t tpmInfo;
    TPM_GetDefaultConfig(&tpmInfo);

    /* TPM clock divide by TPM_PRESCALER */
    tpmInfo.prescale = TPM_PRESCALER;

    /* Initialize TPM module */
    TPM_Init(BOARD_TPM_SECOND, &tpmInfo);

    /* Set timer period */
    TPM_SetTimerPeriod(BOARD_TPM_SECOND, USEC_TO_COUNT(DEMO_TIMER_PERIOD_US, TPM_SOURCE_CLOCK / (1U << tpmInfo.prescale)));

    TPM_EnableInterrupts(BOARD_TPM_SECOND, kTPM_TimeOverflowInterruptEnable);

    EnableIRQ(BOARD_TPM_SECOND_IRQ_NUM);

    TPM_StartTimer(BOARD_TPM_SECOND, kTPM_SystemClock);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();
    PRINTF("Modbus RTU Client Example Start!\r\n");
    TPM2_Init();

    /* Select either ASCII or RTU Mode. */
    eMBMasterInit(MB_RTU, 0, BOARD_DEBUG_UART_BAUDRATE, MB_PAR_NONE);
    /* Enable the Modbus Protocol Stack. */
    eMBMasterEnable();

    for ( ;; )
    {
        eMBMasterReqWriteLedStatus(0x01, 0x0, 0, 100);
        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        eMBMasterReqWriteLedStatus(0x01, 0x1, 0, 100);
        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        eMBMasterReqWriteLedStatus(0x01, 0x0, 1, 100);
        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        eMBMasterReqWriteLedStatus(0x01, 0x1, 1, 100);
        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }
}
