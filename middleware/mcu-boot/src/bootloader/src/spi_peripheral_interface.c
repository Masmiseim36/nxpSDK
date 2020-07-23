/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_context.h"
#include "bl_irq_common.h"
#include "bootloader_common.h"
#include "command_packet.h"
#include "fsl_assert.h"
#include "fsl_device_registers.h"
#include "fsl_spi.h"
#include "serial_packet.h"

#if BL_CONFIG_SPI
//! @addtogroup spi_peripheral
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief Synchronization state between SPI ISR and read/write functions.
typedef struct _spi_transfer_info
{
    const uint8_t *writeData;                                //!< The applications data to write
    volatile uint32_t bytesToTransfer;                       //!< The total number of bytes to be transmitted
    void (*data_source)(uint8_t *source_byte);               // !< Callback used to get byte to transmit.
    void (*data_sink)(uint8_t sink_byte, uint16_t instance); // !< Callback used to put received byte.
} spi_transfer_info_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

//! @brief SPI slave poll for activity function
static bool spi_poll_for_activity(const peripheral_descriptor_t *self);
//! @brief SPI slave init function
static status_t spi_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function);
//! @brief SPI slave shutdown function
static void spi_full_shutdown(const peripheral_descriptor_t *self);
//! @brief SPI slave sending data function
static void spi_data_source(uint8_t *source_byte);
//! @brief SPI slave receiving first byte data function
static void spi_initial_data_sink(uint8_t sink_byte, uint16_t instance);
//! @brief SPI slave receiving data function
static void spi_data_sink(uint8_t sink_byte, uint16_t instance);
//! @brief SPI slave writing data function
static status_t spi_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief SPI slave control interface information
 */
const peripheral_control_interface_t g_spiControlInterface = {
    .pollForActivity = spi_poll_for_activity, .init = spi_full_init, .shutdown = spi_full_shutdown, .pump = 0
};

/*!
 * @brief SPI slave byte interface information
 */
const peripheral_byte_inteface_t g_spiByteInterface = { .init = NULL, .write = spi_write };

//! @brief Global state for the SPI slave peripheral interface.
static spi_transfer_info_t s_spiInfo = {
    .writeData = 0, .bytesToTransfer = 0, .data_source = spi_data_source, .data_sink = spi_initial_data_sink
};

//! @brief Flag for detecting device activity
static bool s_spiActivity[FSL_FEATURE_SOC_SPI_COUNT] = { false };
static bool s_spiIntialized[FSL_FEATURE_SOC_SPI_COUNT] = { false };

/*!
 * @brief SPI slave receiving data call back function
 */
static serial_byte_receive_func_t s_spi_app_data_sink_callback;

const static uint32_t g_spiBaseAddr[] = SPI_BASE_ADDRS;

static const IRQn_Type spi_irq_ids[FSL_FEATURE_SOC_SPI_COUNT] = SPI_IRQS;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void SPI_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    if (set == kPeripheralEnableIRQ)
    {
        NVIC_EnableIRQ(spi_irq_ids[instance]);
    }
    else
    {
        NVIC_DisableIRQ(spi_irq_ids[instance]);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : spi_poll_for_activity
 * Description   : Polling for SPI slave activities
 *
 *END**************************************************************************/
bool spi_poll_for_activity(const peripheral_descriptor_t *self)
{
    return s_spiActivity[self->instance];
}

/*FUNCTION**********************************************************************
 *
 * Function Name : spi_data_source
 * Description   : SPI slave sending data function
 *
 *END**************************************************************************/
void spi_data_source(uint8_t *source_byte)
{
    assert(source_byte);

    if (s_spiInfo.bytesToTransfer)
    {
        *source_byte = *s_spiInfo.writeData++;
        s_spiInfo.bytesToTransfer--;
    }
    else
    {
        *source_byte = 0;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : spi_initial_data_sink
 * Description   : SPI slave receiving first byte data function
 *
 *END**************************************************************************/
void spi_initial_data_sink(uint8_t sink_byte, uint16_t instance)
{
    if (sink_byte == kFramingPacketStartByte)
    {
        s_spiActivity[instance] = true;
        s_spiInfo.data_sink = spi_data_sink;
        s_spi_app_data_sink_callback(sink_byte);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : spi_data_sink
 * Description   : SPI slave receiving data function
 *
 *END**************************************************************************/
void spi_data_sink(uint8_t sink_byte, uint16_t instance)
{
    s_spi_app_data_sink_callback(sink_byte);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : spi_full_init
 * Description   : SPI slave full init function
 *
 *END**************************************************************************/
status_t spi_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function)
{
    s_spi_app_data_sink_callback = function;

    spi_slave_config_t config;
    uint32_t baseAddr = g_spiBaseAddr[self->instance];

    SPI_SlaveGetDefaultConfig(&config);

    config.enableSlave = true;
    config.direction = kSPI_MsbFirst;
    config.polarity = kSPI_ClockPolarityActiveLow;
    config.phase = kSPI_ClockPhaseSecondEdge;

    s_spiInfo.data_source = spi_data_source;
    s_spiInfo.data_sink = spi_initial_data_sink;

    // Configure selected pin as spi peripheral interface
    self->pinmuxConfig(self->instance, kPinmuxType_Peripheral);

    SPI_SlaveInit((SPI_Type *)baseAddr, &config);
#if defined(FSL_FEATURE_SPI_HAS_FIFO) && FSL_FEATURE_SPI_HAS_FIFO
    SPI_EnableFIFO((SPI_Type *)baseAddr, false);
#endif

    s_spiIntialized[self->instance] = true;

    SPI_EnableInterrupts((SPI_Type *)baseAddr, kSPI_RxFullAndModfInterruptEnable | kSPI_TxEmptyInterruptEnable);
    SPI_SetSystemIRQ(self->instance, kPeripheralEnableIRQ);

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : spi_full_shutdown
 * Description   : SPI slave full shutdown function
 *
 *END**************************************************************************/
void spi_full_shutdown(const peripheral_descriptor_t *self)
{
    if (s_spiIntialized[self->instance])
    {
        uint32_t baseAddr = g_spiBaseAddr[self->instance];
        SPI_SetSystemIRQ(self->instance, kPeripheralDisableIRQ);
        SPI_Deinit((SPI_Type *)baseAddr);

        // Restore selected pin to default state to reduce IDD.
        self->pinmuxConfig(self->instance, kPinmuxType_Default);

        s_spiIntialized[self->instance] = false;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : spi_write
 * Description   : SPI slave writing data function
 *
 *END**************************************************************************/
status_t spi_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount)
{
    s_spiInfo.writeData = buffer;
    s_spiInfo.bytesToTransfer = byteCount;

    while (s_spiInfo.bytesToTransfer)
        ;

    return kStatus_Success;
}

void spi_slave_irq_handler(uint32_t instance)
{
    uint32_t baseAddr = g_spiBaseAddr[instance];

    if (((SPI_Type *)baseAddr)->S & SPI_S_SPRF_MASK)
    {
        // SPI receive interrupt
        uint8_t rd = ((SPI_Type *)baseAddr)->DL;
        s_spiInfo.data_sink(rd, instance);
    }

    if (((SPI_Type *)baseAddr)->S & SPI_S_SPTEF_MASK)
    {
        // SPI transimit interrupt
        uint8_t source_byte = 0;
        s_spiInfo.data_source(&source_byte);

        // Write the data to data register
        ((SPI_Type *)baseAddr)->DL = source_byte;
    }
}

#if defined(FSL_FEATURE_SPI_HAS_SHARED_IRQ0_IRQ1_IRQ2) && FSL_FEATURE_SPI_HAS_SHARED_IRQ0_IRQ1_IRQ2
void SPI0_SPI1_SPI2_IRQHandler(void)
{
    for (uint32_t i = 0; i < FSL_FEATURE_SOC_SPI_COUNT; i++)
    {
        if (s_spiIntialized[i])
        {
            spi_slave_irq_handler(i);
        }
    }
}
#else
#if defined(SPI0)
void SPI0_IRQHandler(void)
{
    spi_slave_irq_handler(0);
}
#endif

#if defined(SPI1)
void SPI1_IRQHandler(void)
{
    spi_slave_irq_handler(1);
}
#endif

#if defined(SPI2)
void SPI2_IRQHandler(void)
{
    spi_slave_irq_handler(2);
}
#endif

#endif // defined FSL_FEATURE_SPI_HAS_SHARED_IRQ0_IRQ1_IRQ2

//! @}

#endif // BL_CONFIG_SPI
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
