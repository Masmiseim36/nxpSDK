/*
 * Copyright 2019, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */
#include "board.h"
#include "pin_mux.h"
#include "wwd.h"
#include "wiced_rtos.h"
#include "fsl_lpuart_freertos.h"
#include "fsl_lpuart.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "platform_bluetooth.h"
#include "wwd_logging.h"
#include "platform_imxrt.h"
#if defined(FSL_RTOS_FREE_RTOS)
#include "FreeRTOS.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BT_HCI_UART_DRV_RX_RING_BUF_SIZE 256

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint8_t background_buffer[BT_HCI_UART_DRV_RX_RING_BUF_SIZE];

static lpuart_rtos_handle_t handle;
static lpuart_handle_t      t_handle;

static platform_nvm_t nvmConfig;

/* To change default baudrate, change only the macro defined in 
 * platform_bluetooth.h file
 */ 
lpuart_rtos_config_t lpuart_config = 
{
    .baudrate = WICED_BT_UART_DEFAULT_BAUD_RATE,
    .parity = kLPUART_ParityDisabled,
    .stopbits = kLPUART_OneStopBit,
    .buffer = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

/*******************************************************************************
 * Code
 ******************************************************************************/

void ble_pwr_on(void)
{

    host_rtos_delay_milliseconds( (uint32_t) 500 );

    GPIO_PinWrite(BOARD_INITPINS_BT_REG_ON_GPIO, BOARD_INITPINS_BT_REG_ON_PIN, 1U);

    host_rtos_delay_milliseconds( (uint32_t) 500 );
}

void ble_pwr_off(void)
{
    host_rtos_delay_milliseconds( (uint32_t) 500 );

    GPIO_PinWrite(BOARD_INITPINS_BT_REG_ON_GPIO, BOARD_INITPINS_BT_REG_ON_PIN, 0U);

    host_rtos_delay_milliseconds( (uint32_t) 500 );
}

int imxrt_bt_uart_init(void)
{
    NVIC_SetPriority(BOARD_BT_UART_IRQ, 7U);

    lpuart_config.srcclk = BOARD_BT_UART_CLK_FREQ;
    lpuart_config.base   = BOARD_BT_UART_BASEADDR;
#if defined(FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT) && FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT
    lpuart_config.enableRxRTS = true;
    lpuart_config.enableTxCTS = true;
    lpuart_config.txCtsSource = kLPUART_CtsSourcePin;
    lpuart_config.txCtsConfig = kLPUART_CtsSampleAtStart;
#endif

    if (LPUART_RTOS_Init(&handle, &t_handle, &lpuart_config))
    {
       WWD_LOG(("LPUART_RTOS_Init failed\n"));
       return 1;
    }
    ble_pwr_on();
    return 0;
}

int imxrt_bt_uart_deinit(void)
{
    int result;

    result = LPUART_RTOS_Deinit(&handle);
    ble_pwr_off();
    return result;
}

int imxrt_bt_uart_set_baudrate(uint32_t baudrate)
{
    WWD_LOG(("HCI change baudrate to %d\n", baudrate));
    return LPUART_SetBaudRate(BOARD_BT_UART_BASEADDR, baudrate, BOARD_BT_UART_CLK_FREQ);
}

int imxrt_bt_uart_write(uint8_t *buf, int len)
{
    int ii;

    if( len == 4 && buf[1]==3 && buf[2]==0xc  && buf[3]==0)
        WWD_LOG(("HCI RESET\n"));

    WWD_LOG(("HCI write: "));
    for (ii=0; ii<len; ii++)
        WWD_LOG(("%02x ",buf[ii]));
    WWD_LOG(("\n"));

    return LPUART_RTOS_Send(&handle, buf, len);
}

int imxrt_bt_uart_read(uint8_t *buf, int len, int *read_len)
{
    int ret;
    int ii, rlen;
 
    ret = LPUART_RTOS_Receive(&handle, buf, len, (void *) read_len);

    if(*read_len)
    {
        rlen = *read_len;
        WWD_LOG(("HCI  read: "));
        for(ii=0; ii<rlen; ii++)
            WWD_LOG(("%02x ",buf[ii]));
        WWD_LOG(("\n"));
    }

    return ret;
}

void *bte_port_malloc( uint32_t size )
{
#if defined(FSL_RTOS_FREE_RTOS)
    return pvPortMalloc(size);
#else
    return malloc(size);
#endif
}

void bte_port_free( void *mem)
{
#if defined(FSL_RTOS_FREE_RTOS)
    vPortFree(mem);
#else
    free(mem);
#endif
}

void platform_set_nvm(platform_nvm_t *nvm)
{
    if (NULL != nvm)
    {
        nvmConfig.platform_nvm_save = nvm->platform_nvm_save;
        nvmConfig.platform_nvm_read = nvm->platform_nvm_read;
        nvmConfig.platform_nvm_erase = nvm->platform_nvm_erase;
    }
}

platform_nvm_t *platform_get_nvm(void)
{
    return &nvmConfig;
}
