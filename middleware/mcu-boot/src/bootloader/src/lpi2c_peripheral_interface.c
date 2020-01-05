/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include "bootloader/bl_context.h"
#include "bootloader_common.h"
#include "bootloader/bl_peripheral_interface.h"
#include "packet/command_packet.h"
#include "fsl_lpi2c.h"
#include "fsl_device_registers.h"
#include "packet/serial_packet.h"

//! @addtogroup lpi2c_peripheral
//! @{

#if BL_CONFIG_LPI2C

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define I2C_EMPTY_CHAR (0x00) //!< Empty character.

// Allow the default to be overridden via the bootloader_config.h file.
#if !defined(BL_DEFAULT_I2C_SLAVE_ADDRESS)
//! @brief Default LPI2C slave address in 7-bit format.
#define BL_DEFAULT_I2C_SLAVE_ADDRESS (0x10)
#endif // BL_DEFAULT_I2C_SLAVE_ADDRESS

//! @brief Synchronization state between LPI2C ISR and read/write functions.
typedef struct _lpi2c_transfer_info
{
    const uint8_t *writeData;                                //!< The applications data to write
    volatile uint32_t bytesToTransfer;                       //!< The total number of bytes to be transmitted
    void (*data_source)(uint8_t *source_byte);               // !< Callback used to get byte to transmit.
    void (*data_sink)(uint8_t sink_byte, uint32_t instance); // !< Callback used to put received byte.
} lpi2c_transfer_info_t;

////////////////////////////////////////////////////////////////////////////////
// Private Prototypes
////////////////////////////////////////////////////////////////////////////////

static bool lpi2c_poll_for_activity(const peripheral_descriptor_t *self);
static status_t lpi2c_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function);
static void lpi2c_full_shutdown(const peripheral_descriptor_t *self);
static status_t lpi2c_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount);

static void lpi2c_set_glitch_filter_width(uint32_t instance, uint32_t busClock_Hz, uint32_t glitchWidth_ns);

static void lpi2c_data_source(uint8_t *source_byte);
static void lpi2c_initial_data_sink(uint8_t sink_byte, uint32_t instance);
static void lpi2c_data_sink(uint8_t sink_byte, uint32_t instance);

static void lpi2c_SlaveIRQHandler(uint32_t instance);

extern void LPI2C_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

enum
{
    //! Width of glitches to filter in nanoseconds.
    kLPI2CGlitchFilterWidth_ns = 50,
    // Max cycles supported to set glitch filter
    kLPI2CGlitchFilterMaxCycles = 15,
    //! Part of SDA data valid delay time.
    kLPI2CDataValidDelayCycles = 1,
    //! Part of SCL clock hold time.
    kLPI2CClockHoldCycles = 2,
    //! Timeout before shut down module
    kLPI2CShutdownTimeout_ms = 100
};

const peripheral_control_interface_t g_lpi2cControlInterface = {
    .pollForActivity = lpi2c_poll_for_activity, .init = lpi2c_full_init, .shutdown = lpi2c_full_shutdown, .pump = 0
};

const peripheral_byte_inteface_t g_lpi2cByteInterface = {.init = NULL, .write = lpi2c_write };

//! @brief Global state for the LPI2C peripheral interface.
static lpi2c_transfer_info_t s_lpi2cInfo = {
    .writeData = 0, .bytesToTransfer = 0, .data_source = lpi2c_data_source, .data_sink = lpi2c_initial_data_sink
};

static bool s_lpi2cActivity[FSL_FEATURE_SOC_LPI2C_COUNT] = { false };
static bool s_lpi2cIntialized[FSL_FEATURE_SOC_LPI2C_COUNT] = { false };

const static uint32_t g_lpi2cBaseAddr[] = LPI2C_BASE_ADDRS;

static serial_byte_receive_func_t s_lpi2c_app_data_sink_callback;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

#if !defined(BL_FEATURE_I2C_OPTIMIZE) || USE_ONLY_I2C(0)
//! @brief Implementation of LPI2C0 handler named in startup code.
//!
//! Passes instance to generic LPI2C IRQ handler.
void LPI2C0_IRQHandler(void)
{
    lpi2c_SlaveIRQHandler(0);
}
#endif // !defined(BL_FEATURE_I2C_OPTIMIZE) || USE_ONLY_I2C(0)

#if (!defined(BL_FEATURE_I2C_OPTIMIZE) || USE_ONLY_I2C(1)) && (FSL_FEATURE_SOC_LPI2C_COUNT > 1)
//! @brief Implementation of LPI2C1 handler named in startup code.
//!
//! Passes instance to generic LPI2C IRQ handler.
void LPI2C1_IRQHandler(void)
{
    lpi2c_SlaveIRQHandler(1);
}
#endif //  (FSL_FEATURE_SOC_LPI2C_COUNT > 1)

#if (FSL_FEATURE_SOC_LPI2C_COUNT > 2)
//! @brief Implementation of LPI2C2 handler named in startup code.
//!
//! Passes instance to generic LPI2C IRQ handler.
void LPI2C2_IRQHandler(void)
{
    lpi2c_SlaveIRQHandler(2);
}
#endif //  (FSL_FEATURE_SOC_LPI2C_COUNT > 2)

//! @brief LPI2C Slave Generic IRQ handler.
//!
//! Implements the flow chart at the end of the LPI2C chapter in the Kinetis
//! KL25 Sub-Family Reference Manual. It uses callbacks to get/put data
//! from/to the application as well as alert the application of an error condition.
//!
//! @param instance Instance number of the LPI2C module.
static void lpi2c_SlaveIRQHandler(uint32_t instance)
{
    assert(instance < FSL_FEATURE_SOC_LPI2C_COUNT);

    LPI2C_Type *baseAddr = (LPI2C_Type *)g_lpi2cBaseAddr[instance];

    uint32_t status = LPI2C_SlaveGetStatusFlags(baseAddr);

    if (status & kLPI2C_SlaveAddressValidFlag)
    {
        LPI2C_SlaveGetReceivedAddress(baseAddr);
    }
    if (status & kLPI2C_SlaveRxReadyFlag)
    {
        // Get byte from slave receive register.
        uint8_t sink_byte;
        size_t *actualRxSize = 0;
        LPI2C_SlaveReceive(baseAddr, &sink_byte, 1, actualRxSize);
        s_lpi2cInfo.data_sink(sink_byte, instance);
    }
    if (status & kLPI2C_SlaveTxReadyFlag)
    {
        uint8_t source_byte = I2C_EMPTY_CHAR;
        size_t *actualTxSize = 0;

        s_lpi2cInfo.data_source(&source_byte);

        // Store char to slave transmit register.
        LPI2C_SlaveSend(baseAddr, &source_byte, 1, actualTxSize);
    }
    if (status & kLPI2C_SlaveStopDetectFlag)
    {
        LPI2C_SlaveClearStatusFlags(baseAddr, kLPI2C_SlaveStopDetectFlag);
    }
}

// See lpi2c_slave.h for documentation of this function.
static void lpi2c_set_glitch_filter_width(uint32_t instance, uint32_t busClock_Hz, uint32_t glitchWidth_ns)
{
    uint32_t baseAddr = g_lpi2cBaseAddr[instance];
    uint32_t busCycle_ns = 1000000 / (busClock_Hz / 1000);

    // Search for the cycle count just below the desired glitch width.
    uint32_t cycles = 0;
    while (((cycles + 1) * busCycle_ns) < glitchWidth_ns)
    {
        ++cycles;
    }

    // If we end up with zero cycles, then set the filter to a single cycle unless the
    // bus clock is greater than 10x the desired glitch width.
    if ((cycles == 0) && (busCycle_ns <= (glitchWidth_ns * 10)))
    {
        cycles = 1;
    }
    // If the cycles is greater the max cycles supported to set glitch filter,
    // then cycles should be equal to max cycles
    else if (cycles > kLPI2CGlitchFilterMaxCycles)
    {
        cycles = kLPI2CGlitchFilterMaxCycles;
    }

    ((LPI2C_Type *)baseAddr)->SCFGR2 |= (LPI2C_SCFGR2_FILTSDA(cycles) | LPI2C_SCFGR2_FILTSCL(cycles));
}

bool lpi2c_poll_for_activity(const peripheral_descriptor_t *self)
{
    return s_lpi2cActivity[self->instance];
}

void lpi2c_data_source(uint8_t *source_byte)
{
    assert(source_byte);

    if (s_lpi2cInfo.bytesToTransfer)
    {
        *source_byte = *s_lpi2cInfo.writeData++;
        s_lpi2cInfo.bytesToTransfer--;
    }
    else
    {
        *source_byte = 0;
    }
}

void lpi2c_initial_data_sink(uint8_t sink_byte, uint32_t instance)
{
    if (sink_byte == kFramingPacketStartByte)
    {
        s_lpi2cActivity[instance] = true;
        s_lpi2cInfo.data_sink = lpi2c_data_sink;
        s_lpi2c_app_data_sink_callback(sink_byte);
    }
}

void lpi2c_data_sink(uint8_t sink_byte, uint32_t instance)
{
    //    s_lpi2cActivity[instance] = true;

    s_lpi2c_app_data_sink_callback(sink_byte);
}

status_t lpi2c_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function)
{
    s_lpi2c_app_data_sink_callback = function;

    uint32_t baseAddr = g_lpi2cBaseAddr[self->instance];

    // Read the address from the configuration field. If it is not set, i.e. 0xff,
    // use the default address.
    uint8_t slaveAddress = g_bootloaderContext.propertyInterface->store->configurationData.i2cSlaveAddress;
    if (slaveAddress == 0xff)
    {
        slaveAddress = BL_DEFAULT_I2C_SLAVE_ADDRESS;
    }

    // Init the driver.
    lpi2c_slave_config_t config;

    LPI2C_SlaveGetDefaultConfig(&config);
    config.address0 = slaveAddress;

    // Configure selected pin as lpi2c peripheral interface
#if defined(BL_FEATURE_6PINS_PERIPHERAL) && BL_FEATURE_6PINS_PERIPHERAL
    self->pinmuxConfig(self->instance, kPinmuxType_PollForActivity);
#else
    self->pinmuxConfig(self->instance, kPinmuxType_Peripheral);
#endif // BL_FEATURE_6PINS_PERIPHERAL

    // Ungate the LPI2C clock.
    LPI2C_SlaveInit((LPI2C_Type *)baseAddr, &config, get_bus_clock());
    LPI2C_SlaveEnable((LPI2C_Type *)baseAddr, true);
    LPI2C_SlaveEnableInterrupts((LPI2C_Type *)baseAddr, LPI2C_SIER_AVIE_MASK | LPI2C_SIER_TDIE_MASK |
                                                            LPI2C_SIER_RDIE_MASK | LPI2C_SIER_SDIE_MASK);

    // Transmit Data Flag will assert whenever the transmit data register is empty.
    ((LPI2C_Type *)baseAddr)->SCFGR1 |= LPI2C_SCFGR1_TXCFG_MASK;

    // Set Slave glitch filter for both SDA and SCL input
    lpi2c_set_glitch_filter_width(self->instance, get_bus_clock(), kLPI2CGlitchFilterWidth_ns);

    // Set the SDA data valid delay time for the LPI2C slave equal to FILTSCL+DATAVD+3 cycles
    // Note: SDA data valid delay function is active only when feeding a non-zero value here
    ((LPI2C_Type *)baseAddr)->SCFGR2 |=
        (LPI2C_SCFGR2_DATAVD(kLPI2CDataValidDelayCycles) | LPI2C_SCFGR2_CLKHOLD(kLPI2CClockHoldCycles));

    // Enable LPI2C interrupt
    LPI2C_SetSystemIRQ(self->instance, kPeripheralEnableIRQ);

    s_lpi2cInfo.data_sink = lpi2c_initial_data_sink;

    s_lpi2cIntialized[self->instance] = true;

    return kStatus_Success;
}

void lpi2c_full_shutdown(const peripheral_descriptor_t *self)
{
    if (s_lpi2cIntialized[self->instance])
    {
        uint32_t baseAddr = g_lpi2cBaseAddr[self->instance];

        // Ungate the LPI2C clock.
        LPI2C_SlaveDeinit((LPI2C_Type *)baseAddr);
#if BL_FEATURE_6PINS_PERIPHERAL
        // When the active peripheral is not I2C, we should only restore
        //   those pin which we used to poll for activity.
        if (g_bootloaderContext.activePeripheral == NULL)
        {
            self->pinmuxConfig(self->instance, kPinmuxType_RestoreForActivity);
        }
        // When the active peripheral is I2C, we should restore all
        //  the I2C peripheral pin.
        else
#endif
        {
            // Restore selected pin to default state to reduce IDD.
            self->pinmuxConfig(self->instance, kPinmuxType_Default);
        }

        s_lpi2cIntialized[self->instance] = false;
    }
}

status_t lpi2c_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount)
{
    s_lpi2cInfo.writeData = buffer;
    s_lpi2cInfo.bytesToTransfer = byteCount;

    while (s_lpi2cInfo.bytesToTransfer)
        ;

    return kStatus_Success;
}

//! @}

#endif // BL_CONFIG_LPI2C

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
