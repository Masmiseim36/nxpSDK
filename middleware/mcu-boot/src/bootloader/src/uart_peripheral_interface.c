/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "autobaud.h"
#include "bl_context.h"
#include "bl_irq_common.h"
#include "bootloader_common.h"
#include "fsl_assert.h"
#include "fsl_device_registers.h"
#include "fsl_uart.h"
#include "serial_packet.h"

#if (BL_CONFIG_UART)

//! @addtogroup uart_peripheral
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define UART_TYPE_REGS_BASE(x) ((UART_Type *)REGS_UART_BASE(x))

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static bool uart_poll_for_activity(const peripheral_descriptor_t *self);
static status_t uart_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function);
static void uart_full_shutdown(const peripheral_descriptor_t *self);

static status_t uart_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount);
static void uart_irq_handler(uint32_t instance);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

const peripheral_control_interface_t g_uartControlInterface = {
    .pollForActivity = uart_poll_for_activity, .init = uart_full_init, .shutdown = uart_full_shutdown, .pump = 0
};

const peripheral_byte_inteface_t g_uartByteInterface = { .init = NULL, .write = uart_write };

static serial_byte_receive_func_t s_uart_byte_receive_callback;
static bool g_uartInitStatus[FSL_FEATURE_SOC_UART_COUNT] = { false }; // not initialized.
static const uint32_t g_uartBaseAddr[] = UART_BASE_ADDRS;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
bool uart_poll_for_activity(const peripheral_descriptor_t *self)
{
    uint32_t baud;
    uint32_t instance;
    instance = self->instance;

    status_t autoBaudCompleted = autobaud_get_rate(instance, &baud);

    if (autoBaudCompleted == kStatus_Success)
    {
        uart_config_t userConfig;
        uint32_t baseAddr = g_uartBaseAddr[instance];
        UART_GetDefaultConfig(&userConfig);

        userConfig.baudRate_Bps = baud;
        userConfig.enableTx = true;
        userConfig.enableRx = true;

        if (UART_Init((UART_Type *)baseAddr, &userConfig, get_uart_clock(instance)) == kStatus_Success)
        {
            // Configure selected pin as uart peripheral interface
            self->pinmuxConfig(instance, kPinmuxType_Peripheral);
            // update pheripheral interface init status
            g_uartInitStatus[instance] = true;

            UART_SetSystemIRQ(instance, kPeripheralEnableIRQ);
            UART_EnableInterrupts((UART_Type *)baseAddr, kUART_RxDataRegFullInterruptEnable);

            // This was the byte pattern identified in autobaud detection, inform the command layer
            s_uart_byte_receive_callback(kFramingPacketStartByte);
            s_uart_byte_receive_callback(kFramingPacketType_Ping);
            return true;
        }
        else
        {
            autobaud_init(instance);
        }
    }

    return false;
}

status_t uart_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function)
{
    s_uart_byte_receive_callback = function;

    // Since we are using autobaud once the detection is completed
    // it will call the UART initialization and remux the pins when it completes
    self->pinmuxConfig(self->instance, kPinmuxType_PollForActivity);

    // Init autobaud detector.
    autobaud_init(self->instance);

    return kStatus_Success;
}

void uart_full_shutdown(const peripheral_descriptor_t *self)
{
    uint32_t instance = self->instance;
    if (g_uartInitStatus[instance])
    {
        uint32_t baseAddr = g_uartBaseAddr[instance];
        UART_SetSystemIRQ(instance, kPeripheralDisableIRQ);
        UART_Deinit((UART_Type *)baseAddr);
    }

//! Note: if not deinit autobaud(IRQ method), user app may encounters hardfault
//! if it doesn't provide related pin interrupt service routine.
#if BL_FEATURE_UART_AUTOBAUD_IRQ
    // De-init autobaud detector.
    autobaud_deinit(instance);
#endif

#if BL_FEATURE_6PINS_PERIPHERAL
    // When the active peripheral is not UART, we should only restore
    //   those pin which we used to poll for activity.
    if (g_bootloaderContext.activePeripheral == NULL)
    {
        self->pinmuxConfig(instance, kPinmuxType_RestoreForActivity);
    }
    // When the active peripheral is UART, we should restore all
    //  the UART peripheral pin.
    else
#endif
    {
        // Restore selected pin to default state to reduce IDD.
        self->pinmuxConfig(instance, kPinmuxType_Default);
    }
}

status_t uart_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount)
{
    uint32_t baseAddr = g_uartBaseAddr[self->instance];
    UART_WriteBlocking((UART_Type *)baseAddr, buffer, byteCount);
    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_irq_handler
 * Description   : uart bootloader interrupt handler
 *
 *END**************************************************************************/
static void uart_irq_handler(uint32_t instance)
{
    UART_Type *base = (UART_Type *)g_uartBaseAddr[instance];
    if (base->S1 & UART_S1_RDRF_MASK)
    {
        s_uart_byte_receive_callback(base->D);
    }
}

#if defined(FSL_FEATURE_UART_HAS_SHARED_IRQ0_IRQ1_IRQ2_IRQ3) && FSL_FEATURE_UART_HAS_SHARED_IRQ0_IRQ1_IRQ2_IRQ3
void UART0_UART1_UART2_UART3_IRQHandler(void)
{
    for (uint32_t i = 0; i < FSL_FEATURE_SOC_UART_COUNT; i++)
    {
        if (g_uartInitStatus[i])
        {
            uart_irq_handler(i);
        }
    }
}
#else
/********************************************************************/
/*
 * UART0 IRQ Handler
 *
 */
void UART0_IRQHandler(void)
{
    if (UART0->S1 & UART_S1_RDRF_MASK)
    {
        s_uart_byte_receive_callback(UART0->D);
    }
}

/********************************************************************/
/*
 * UART1 IRQ Handler
 *
 */
void UART1_IRQHandler(void)
{
    if (UART1->S1 & UART_S1_RDRF_MASK)
    {
        s_uart_byte_receive_callback(UART1->D);
    }
}

#endif // defined FSL_FEATURE_UART_HAS_SHARED_IRQ0_IRQ1_IRQ2_IRQ3
//! @}

#endif // (BL_CONFIG_UART)

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
