/*
 * Copyright 2019-2021, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "fsl_lpuart_freertos.h"
#include "serial_mwm_port.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SERIAL_MWM_PORT_NVIC_PRIO 5

/* Ring buffer size */
#define RING_BUFFER_SIZE 2048

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_background_buffer[RING_BUFFER_SIZE];

static lpuart_rtos_handle_t s_handle;
static struct _lpuart_handle s_t_handle;

static lpuart_rtos_config_t s_lpuart_config = {
    .baudrate    = 115200,
    .parity      = kLPUART_ParityDisabled,
    .stopbits    = kLPUART_OneStopBit,
    .buffer      = s_background_buffer,
    .buffer_size = sizeof(s_background_buffer),
};

/*******************************************************************************
 * Code
 ******************************************************************************/
int mwm_rx(uint8_t *read_buf, uint32_t len)
{
    if ((read_buf == NULL) || (len == 0u))
    {
        return -1;
    }

    int error;
    size_t n = 0;
    error    = LPUART_RTOS_Receive(&s_handle, (uint8_t *)read_buf, len, &n);
    if (error == kStatus_Success)
    {
        return 0;
    }

    return -1;
}

int mwm_tx(uint8_t *write_buf, uint32_t len)
{
    if ((write_buf == NULL) || (len == 0u))
    {
        return -1;
    }

    int error;
    error = LPUART_RTOS_Send(&s_handle, (uint8_t *)write_buf, len);
    if (error == kStatus_Success)
    {
        return 0;
    }

    return -1;
}

int mwm_port_init(void)
{
#if defined(BOARD_SERIAL_MWM_PORT) && defined(BOARD_SERIAL_MWM_PORT_CLK_FREQ) && defined(BOARD_SERIAL_MWM_PORT_IRQn)
    s_lpuart_config.srcclk = BOARD_SERIAL_MWM_PORT_CLK_FREQ;
    s_lpuart_config.base   = BOARD_SERIAL_MWM_PORT;

    NVIC_SetPriority(BOARD_SERIAL_MWM_PORT_IRQn, SERIAL_MWM_PORT_NVIC_PRIO);
#else
/*
 * Example of LPUART settings:
 * #define BOARD_SERIAL_MWM_PORT_CLK_FREQ     BOARD_DebugConsoleSrcFreq()
 * #define BOARD_SERIAL_MWM_PORT              LPUART2
 * #define BOARD_SERIAL_MWM_PORT_IRQn         LPUART2_IRQn
 */
#warning "Define BOARD_SERIAL_MWM_PORT, BOARD_SERIAL_MWM_PORT_CLK_FREQ, BOARD_SERIAL_MWM_PORT_IRQn in board.h"
#endif

    if (0 > LPUART_RTOS_Init(&s_handle, &s_t_handle, &s_lpuart_config))
    {
        return -1;
    }

    return 0;
}

int mwm_port_deinit(void)
{
    if (0 > LPUART_RTOS_Deinit(&s_handle))
    {
        return -1;
    }

    return 0;
}
