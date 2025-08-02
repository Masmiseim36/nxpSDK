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

#include "mb.h"
#include "mbport.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_IMX_RT1180 (1)
#define SLAVE_ADDRESS   0x01
#define SLAVE_PORT      0x01

#define COIL_START_ADDR                 0           /*coil register start address*/
#define COIL_NUMS                       10          /*coil register numbers*/
 
#define DISCRETE_INPUT_START_ADDR       10000	    /*discrete register start address*/
#define DISCRETE_INPUT_NUMS             10          /*discrete register numbers*/
 
#define INPUT_REGISTER_START_ADDR       30000		/*input register start address*/
#define INPUT_REGISTER_NUMS             10		    /*input register numbers*/
 
#define HOLDING_REGISTER_START_ADDR     40000		/*holding register start address*/
#define HOLDING_REGISTER_NUMS           10		    /*holding register numbers*/

#define LED_START_ADDR                  0		/*LED start address*/
#define LED_NUMS                        2		    /*LED numbers*/

/* ----------------------- Static variables ------------------------------- */
static uint16_t     inputRegStart = INPUT_REGISTER_START_ADDR;

static uint16_t     inputRegArrays[INPUT_REGISTER_NUMS];

static uint16_t     holdingRegStart = HOLDING_REGISTER_START_ADDR;

static uint16_t     holdingRegArrays[HOLDING_REGISTER_NUMS];

static uint16_t     initArrays[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

static uint16_t     led1_status;
static uint16_t     led2_status;

/* ------------------------ Implementation -------------------------------- */
/**
 * Modbus slave input register callback function.
 * @param pucRegBuffer input register buffer	
 * @param usAddress input register address		
 * @param usNRegs input register number			
 * @return result
 */
eMBErrorCode
eMBRegInputCB( uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
 
    /* it already plus one in modbus function method. */
    usAddress--;
 
    if( ( usAddress >= INPUT_REGISTER_START_ADDR )
        && ( usAddress + usNRegs <= INPUT_REGISTER_START_ADDR + INPUT_REGISTER_NUMS ) )
    {
        iRegIndex = ( int )( usAddress - inputRegStart );
        while( usNRegs > 0 )
        {
            inputRegArrays[iRegIndex] = iRegIndex;
            *pucRegBuffer++ = ( unsigned char )( inputRegArrays[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( inputRegArrays[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
 
/**
 * Modbus slave LED status callback function.	
 * @param pucRegBuffer LED status buffer	
 * @param usAddress LED address	
 * @param usNRegs LED number		
 * @param eMode read or write					
 * @return result
 */
eMBErrorCode
eMBLedStatusCB( uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    /* it already plus one in modbus function method. */
    usAddress--;

    if( usAddress +  usNRegs <= LED_NUMS )
    {
        if ( eMode == MB_REG_READ)
        {
            while( usNRegs > 0 )
            {
                if (usAddress == 0)
                {
                    *pucRegBuffer++ = ( uint8_t ) ( led1_status> 8 );
                    *pucRegBuffer++ = ( uint8_t ) ( led1_status & 0xFF );
                }
                else
                {
                    *pucRegBuffer++ = ( uint8_t ) ( led2_status> 8 );
                    *pucRegBuffer++ = ( uint8_t ) ( led2_status & 0xFF );
                }
                usAddress++;
                usNRegs--;
            }  
        } 
        else 
        {
            while( usNRegs > 0 )
            {
                if (usAddress == 0)
                {
                    led1_status =  *pucRegBuffer++ << 8;
                    led1_status |= *pucRegBuffer++;
                    RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN1, led1_status);
                    PRINTF("LED1 status: %d\r\n", led1_status);
                } 
                else
                {
                    led2_status =  *pucRegBuffer++ << 8;
                    led2_status |= *pucRegBuffer++;
                    RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN2, led2_status);
                    PRINTF("LED2 status: %d\r\n", led2_status);
                }
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
        PRINTF("Invalid LED address! \r\n");
    }
    
    return eStatus;
}

/**
 * Modbus slave holding register callback function.	
 * @param pucRegBuffer holding register buffer	
 * @param usAddress holding register address	
 * @param usNRegs holding register number		
 * @param eMode read or write					
 * @return result
 */
eMBErrorCode
eMBRegHoldingCB( uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
 
    /* it already plus one in modbus function method. */
    usAddress--;
    if( ( usAddress >= HOLDING_REGISTER_START_ADDR ) &&
        ( usAddress + usNRegs <= HOLDING_REGISTER_START_ADDR + HOLDING_REGISTER_NUMS ) )
    {
        iRegIndex = ( int )( usAddress - holdingRegStart );
        if ( eMode == MB_REG_READ)
        {
            while( usNRegs > 0 )
            {
                holdingRegArrays[iRegIndex] = initArrays[iRegIndex];
                *pucRegBuffer++ = ( uint8_t ) ( holdingRegArrays[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( uint8_t ) ( holdingRegArrays[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
        } 
        else 
        {
            while( usNRegs > 0 )
            {
                holdingRegArrays[iRegIndex] = *pucRegBuffer++ << 8;
                holdingRegArrays[iRegIndex] |= *pucRegBuffer++;
                initArrays[iRegIndex] = holdingRegArrays[iRegIndex];
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
 
eMBErrorCode
eMBRegCoilsCB( uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNCoils, eMBRegisterMode eMode )
{
    return MB_ENOREG;
}
 
eMBErrorCode
eMBRegDiscreteCB( uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNDiscrete )
{
    return MB_ENOREG;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();
    PRINTF("Modbus RTU Server Example Start!\r\n");
    
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t led1_config = {
        kRGPIO_DigitalOutput,
        0,
    };
    rgpio_pin_config_t led2_config = {
        kRGPIO_DigitalOutput,
        0,
    };

    /* Init output LED GPIO. */
    RGPIO_PinInit(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN1, &led1_config);
    RGPIO_PinInit(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN2, &led2_config);

    RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN1, 1);
    RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN2, 1);

    /* Select either ASCII or RTU Mode. */
    eMBInit( MB_RTU, SLAVE_ADDRESS, SLAVE_PORT, BOARD_DEBUG_UART_BAUDRATE, MB_PAR_NONE );

    /* Enable the Modbus Protocol Stack. */
    eMBEnable(  );

    for( ;; )
    {
        ( void )eMBPoll(  );
    }
}
