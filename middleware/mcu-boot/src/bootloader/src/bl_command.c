/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bootloader.h"
#include "memory/memory.h"
#include "sbloader/sbloader.h"
#include "property/property.h"
#include "utilities/fsl_assert.h"
#include "utilities/fsl_rtos_abstraction.h"
#include <string.h>
#include <stdint.h>
#include "bl_command.h"

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#if !(defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES)
#include "fsl_flash.h"
#else
#include "flashiap_wrapper/fsl_flashiap_wrapper.h"
#endif
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
#include "fsl_device_registers.h"
#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
#include "qspi.h"
#endif // #if BL_FEATURE_QSPI_MODULE
#if defined(BL_FEATURE_OTFAD_MODULE) && BL_FEATURE_OTFAD_MODULE
#include "fsl_otfad_driver.h"
#endif // #if BL_FEATURE_OTFAD_MODULE
#if defined(BL_FEATURE_OCOTP_MODULE) && BL_FEATURE_OCOTP_MODULE
#include "ocotp/fsl_ocotp.h"
#endif // #if BL_FEATURE_OCOTP_MODULE
#if defined(BL_DEVICE_IS_LPC_SERIES) && defined(OTP_API)
#include "otp/otp.h"
#endif
#if defined(BL_FEATURE_RELIABLE_UPDATE) && BL_FEATURE_RELIABLE_UPDATE
#include "bootloader/bl_reliable_update.h"
#endif

#if defined(BL_FEATURE_GEN_KEYBLOB) && BL_FEATURE_GEN_KEYBLOB
#include "bootloader/bl_keyblob.h"
#endif

#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
#include "authentication/key_store.h"
#include "authentication/key_store_hal.h"
/* the RAM address of key store comes from hal (key_store_hal_lpc54s018.c) */
extern skboot_key_store_t *const s_keyStore;
#endif

//! @addtogroup command
//! @{

//! @name State machine
//@{
static status_t handle_command(uint8_t *packet, uint32_t packetLength);
static status_t handle_data(bool *hasMoreData);
//@}

//! @name Command handlers
//@{
static void handle_reset(uint8_t *packet, uint32_t packetLength);
static void handle_flash_erase_all(uint8_t *packet, uint32_t packetLength);
#if defined(BL_FEATURE_ERASEALL_UNSECURE) && BL_FEATURE_ERASEALL_UNSECURE
static void handle_flash_erase_all_unsecure(uint8_t *packet, uint32_t packetLength);
#endif
static void handle_flash_erase_region(uint8_t *packet, uint32_t packetLength);
static void handle_receive_sb_file(uint8_t *packet, uint32_t packetLength);
static void handle_read_memory(uint8_t *packet, uint32_t packetLength);
static void handle_fill_memory(uint8_t *packet, uint32_t packetLength);
static void handle_set_property(uint8_t *packet, uint32_t packetLength);
static void handle_get_property(uint8_t *packet, uint32_t packetLength);
static void handle_write_memory(uint8_t *packet, uint32_t packetLength);
static void handle_execute(uint8_t *packet, uint32_t packetLength);
static void handle_call(uint8_t *packet, uint32_t packetLength);
static void handle_flash_security_disable(uint8_t *packet, uint32_t packetLength);
static void handle_flash_program_once(uint8_t *packet, uint32_t length);
static void handle_flash_read_once(uint8_t *packet, uint32_t length);
static void handle_flash_read_resource(uint8_t *packet, uint32_t length);
#if (defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE) || \
    (defined(BL_FEATURE_FLEXSPI_NOR_MODULE) && BL_FEATURE_FLEXSPI_NOR_MODULE) || \
    (defined(BL_FEATURE_SEMC_NOR_MODULE) && BL_FEATURE_SEMC_NOR_MODULE) || \
    (defined(BL_FEATURE_EXPAND_MEMORY) && BL_FEATURE_EXPAND_MEMORY) || \
    (defined(BL_FEATURE_SPI_NOR_EEPROM_MODULE) && BL_FEATURE_SPI_NOR_EEPROM_MODULE) || \
    (defined(BL_FEATURE_SPIFI_NOR_MODULE) && BL_FEATURE_SPIFI_NOR_MODULE)
static void handle_configure_memory(uint8_t *packet, uint32_t packetLength);
#endif
static void handle_reliable_update(uint8_t *packet, uint32_t packetLength);
#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
static void handle_key_provisioning(uint8_t *packet, uint32_t packetLength);
#endif
#if defined(BL_FEATURE_GEN_KEYBLOB) && BL_FEATURE_GEN_KEYBLOB
static void handle_generate_key_blob(uint8_t *packet, uint32_t packetLength);
static status_t handle_key_blob_data(bool *hasMoreData);
#endif
//@}

//! @name Command responses
//@{
static void send_read_memory_response(uint32_t commandStatus, uint32_t length);
#if defined(BL_FEATURE_GEN_KEYBLOB) && BL_FEATURE_GEN_KEYBLOB
static void send_generate_key_blob_response(uint32_t commandStatus, uint32_t length);
#endif
static void send_generic_response(uint32_t commandStatus, uint32_t commandTag);
static void send_get_property_response(uint32_t commandStatus, uint32_t *value, uint32_t numValues);
static void send_flash_read_once_response(uint32_t commandStatus, uint32_t *value, uint32_t byteCount);
static void send_flash_read_resource_response(uint32_t commandStatus, uint32_t length);
#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
static void send_key_provisioning_response(uint32_t commandStatus, uint32_t length);
#endif
//@}

//! @name Data phase
//@{
static void reset_data_phase(void);
static void finalize_data_phase(status_t status);
static status_t handle_data_producer(bool *hasMoreData);
static status_t handle_data_consumer(bool *hasMoreData);
#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
static status_t handle_data_bidirection(bool *hasMoreData);
#endif
//@}

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//#define TEST_SENDER_ABORT
//#define TEST_RECEIVER_ABORT

enum _secure_commands
{
    //! @brief Bitmask of commands allowed when flash security is enabled.
    //!
    //! This bitmask uses the same format as the AvailableCommands property. This is,
    //! the bit number for a given command is the command's tag value minus one.
    kCommandsAllowedWhenSecure = (HAS_CMD(kCommandTag_FlashSecurityDisable) | HAS_CMD(kCommandTag_GetProperty) |
                                  HAS_CMD(kCommandTag_Reset) | HAS_CMD(kCommandTag_SetProperty) |
                                  HAS_CMD(kCommandTag_FlashEraseAllUnsecure) | HAS_CMD(kCommandTag_ReceiveSbFile)
#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
                                  | HAS_CMD(kCommandTag_KeyProvisioning) 
#endif // BL_FEATURE_KEY_PROVISIONING
                                  )
};

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Command handler table.
const command_handler_entry_t g_commandHandlerTable[] = {
// cmd handler              // data handler or NULL
#if !(defined(BL_FEATURE_MIN_PROFILE) && BL_FEATURE_MIN_PROFILE)
    { handle_flash_erase_all, NULL },              // kCommandTag_FlashEraseAll = 0x01
    { handle_flash_erase_region, NULL },           // kCommandTag_FlashEraseRegion = 0x02
    { handle_read_memory, handle_data_producer },  // kCommandTag_ReadMemory = 0x03
    { handle_write_memory, handle_data_consumer }, // kCommandTag_WriteMemory = 0x04
    { handle_fill_memory, NULL },                  // kCommandTag_FillMemory = 0x05
#if defined(BL_FEATURE_FLASH_SECURITY) && BL_FEATURE_FLASH_SECURITY
    { handle_flash_security_disable, NULL }, // kCommandTag_FlashSecurityDisable = 0x06
#else
    { 0u },
#endif                                                // BL_FEATURE_FLASH_SECURITY
    { handle_get_property, NULL },                    // kCommandTag_GetProperty = 0x07
    { handle_receive_sb_file, handle_data_consumer }, // kCommandTag_ReceiveSbFile = 0x08
    { handle_execute, NULL },                         // kCommandTag_Execute = 0x09
    { handle_call, NULL },                            // kCommandTag_Call = 0x0a
    { handle_reset, NULL },                           // kCommandTag_Reset = 0x0b
    { handle_set_property, NULL },                    // kCommandTag_SetProperty = 0x0c
#if defined(BL_FEATURE_ERASEALL_UNSECURE) && BL_FEATURE_ERASEALL_UNSECURE
    { handle_flash_erase_all_unsecure, NULL }, // kCommandTag_FlashEraseAllUnsecure = 0x0d
#else  // BL_FEATURE_ERASEALL_UNSECURE
    { 0u }, // kCommandTag_FlashEraseAllUnsecure = 0x0d
#endif // BL_FEATURE_ERASEALL_UNSECURE
#if (((!(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)) || \
    (defined(BL_FEATURE_OCOTP_MODULE) && BL_FEATURE_OCOTP_MODULE)) && \
    (!(defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES)))|| \
    ((defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES) && defined(OTP_API))
    { handle_flash_program_once, NULL }, // kCommandTag_ProgramOnce = 0x0e
    { handle_flash_read_once, NULL },    // kCommandTag_ReadOnce = 0x0f
#if !(defined(BL_FEATURE_HAS_NO_READ_SOURCE) && BL_FEATURE_HAS_NO_READ_SOURCE)
    { handle_flash_read_resource, handle_data_producer }, // kCommandTag_ReadResource = 0x10
#else
    { 0u },
#endif // !BL_FEATURE_HAS_NO_READ_SOURCE
#else
    { 0u },
    { 0u },
    { 0u },
#endif // #if (((!BL_FEATURE_HAS_NO_INTERNAL_FLASH) || BL_FEATURE_OCOTP_MODULE) && (!BL_DEVICE_IS_LPC_SERIES))|| ((BL_DEVICE_IS_LPC_SERIES) && defined(OTP_API))
#if (defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE) || \
    (defined(BL_FEATURE_FLEXSPI_NOR_MODULE) && BL_FEATURE_FLEXSPI_NOR_MODULE) || \
    (defined(BL_FEATURE_SEMC_NOR_MODULE) && BL_FEATURE_SEMC_NOR_MODULE) || \
    (defined(BL_FEATURE_EXPAND_MEMORY) && BL_FEATURE_EXPAND_MEMORY) || \
    (defined(BL_FEATURE_SPI_NOR_EEPROM_MODULE) && BL_FEATURE_SPI_NOR_EEPROM_MODULE) || \
    (defined(BL_FEATURE_SPIFI_NOR_MODULE) && BL_FEATURE_SPIFI_NOR_MODULE)
    { handle_configure_memory, NULL }, // kCommandTag_ConfigureMemory = 0x11
#else
    { 0u }, // kCommandTag_ConfigureMemory = 0x11
#endif // BL_FEATURE_QSPI_MODULE || BL_FEATURE_FLEXSPI_NOR_MODULE || BL_FEATURE_SEMC_NOR_MODULE || BL_FEATURE_EXPAND_MEMORY || BL_FEATURE_SPI_NOR_EEPROM_MODULE || BL_FEATURE_SPIFI_NOR_MODULE
#if defined(BL_FEATURE_RELIABLE_UPDATE) && BL_FEATURE_RELIABLE_UPDATE
    { handle_reliable_update, NULL }, // kCommandTag_ReliableUpdate = 0x12
#else
    { 0u }, // kCommandTag_ReliableUpdate = 0x12
#endif // BL_FEATURE_RELIABLE_UPDATE
#if defined(BL_FEATURE_GEN_KEYBLOB) && BL_FEATURE_GEN_KEYBLOB
    { handle_generate_key_blob, handle_key_blob_data }, // kCommandTag_GenerateKeyBlob = 0x13
#endif
    { 0u },
    { 0u },
#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
    { handle_key_provisioning, handle_data_bidirection }, // kCommandTag_KeyProvisioning = 0x15
#endif // BL_FEATURE_KEY_PROVISIONING
#else // BL_FEATURE_MIN_PROFILE
    { handle_flash_erase_all, NULL },    // kCommandTag_FlashEraseAll = 0x01
    { handle_flash_erase_region, NULL }, // kCommandTag_FlashEraseRegion = 0x02
#if defined(BL_FEATURE_READ_MEMORY) && BL_FEATURE_READ_MEMORY
    { handle_read_memory, handle_data_producer }, // kCommandTag_ReadMemory = 0x03
#else // BL_FEATURE_READ_MEMORY
    { 0u }, // kCommandTag_ReadMemory = 0x03
#endif
    { handle_write_memory, handle_data_consumer }, // kCommandTag_WriteMemory = 0x04
#if defined(BL_FEATURE_FILL_MEMORY)BL_FEATURE_FILL_MEMORY && BL_FEATURE_FILL_MEMORY
    { handle_fill_memory, NULL }, // kCommandTag_FillMemory = 0x05
#else
    { 0u },
#endif // BL_FEATURE_FILL_MEMORY
#if defined(BL_FEATURE_FLASH_SECURITY) && BL_FEATURE_FLASH_SECURITY
    { handle_flash_security_disable, NULL }, // kCommandTag_FlashSecurityDisable = 0x06
#else
    { 0u },
#endif // BL_FEATURE_FLASH_SECURITY
    { handle_get_property, NULL },             // kCommandTag_GetProperty = 0x07
    { 0u},                                     // kCommandTag_ReceiveSbFile = 0x08
    { handle_execute, NULL },                  // kCommandTag_Execute = 0x09
    { 0u },                                     // kCommandTag_Call = 0x0a
    { handle_reset, NULL },                    // kCommandTag_Reset = 0x0b
    { handle_set_property, NULL },             // kCommandTag_SetProperty = 0x0c
#if defined(BL_FEATURE_ERASEALL_UNSECURE) && BL_FEATURE_ERASEALL_UNSECURE
    { handle_flash_erase_all_unsecure, NULL }, // kCommandTag_FlashEraseAllUnsecure = 0x0d
#else  // BL_FEATURE_ERASEALL_UNSECURE
    { 0u }, // kCommandTag_FlashEraseAllUnsecure = 0x0d
#endif // BL_FEATURE_ERASEALL_UNSECURE
    { 0u },                                     // kCommandTag_ProgramOnce = 0x0e
    { 0u },                                     // kCommandTag_ReadOnce = 0x0f
    { 0u },                                     // kCommandTag_ReadResource = 0x10
    { 0u },                                     // kCommandTag_ConfigureQuadSpi = 0x11
    { 0u },                                     // kCommandTag_ReliableUpdate = 0x12
#if defined(BL_FEATURE_GEN_KEYBLOB) && BL_FEATURE_GEN_KEYBLOB
    { handle_generate_key_blob, handle_key_blob_data }, // kCommandTag_GenerateKeyBlob = 0x13
#endif
    { 0u },
    { 0u },
#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
    { handle_key_provisioning, handle_data_bidirection }, // kCommandTag_KeyProvisioning = 0x15
#endif // BL_FEATURE_KEY_PROVISIONING
#endif // BL_FEATURE_MIN_PROFILE
};

//! @brief Command processor state data.
command_processor_data_t g_commandData = {0};

// See bl_command.h for documentation on this interface.
command_interface_t g_commandInterface = { bootloader_command_init, bootloader_command_pump,
                                           (command_handler_entry_t *)&g_commandHandlerTable, &g_commandData };

#if defined(BL_FEATURE_EXPAND_PACKET_SIZE) && BL_FEATURE_EXPAND_PACKET_SIZE
static uint8_t s_dataProducerPacket[kMaxBootloaderPacketSize];
#endif // BL_FEATURE_EXPAND_PACKET_SIZE

#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
uint8_t s_userKeyBuffer[kPUF_KeySizeMax];
#endif
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See bl_command.h for documentation on this function.
status_t bootloader_command_init(void)
{
    command_processor_data_t *data = g_bootloaderContext.commandInterface->stateData;

    data->state = (int32_t)kCommandState_CommandPhase;
    return (int32_t)kStatus_Success;
}

// See bl_command.h for documentation on this function.
status_t bootloader_command_pump(void)
{
    status_t status = (int32_t)kStatus_Success;
    bool hasMoreData = false;

    if (g_bootloaderContext.activePeripheral->packetInterface != (void *)0u)
    {
        switch (g_bootloaderContext.commandInterface->stateData->state)
        {
            default:
                // same as kCommandState_CommandPhase
            case (int32_t)kCommandState_CommandPhase:
                status = g_bootloaderContext.activePeripheral->packetInterface->readPacket(
                    g_bootloaderContext.activePeripheral, &g_bootloaderContext.commandInterface->stateData->packet,
                    &g_bootloaderContext.commandInterface->stateData->packetLength, kPacketType_Command);
                if ((status != (int32_t)kStatus_Success) && (status != (int32_t)kStatus_AbortDataPhase) && (status != (int32_t)kStatus_Ping))
                {
                    debug_printf("Error: readPacket returned status 0x%x\r\n", status);
                    break;
                }
                if (g_bootloaderContext.commandInterface->stateData->packetLength == 0u)
                {
                    // No command packet is available. Return success.
                    break;
                }
                status = handle_command(g_bootloaderContext.commandInterface->stateData->packet,
                                        g_bootloaderContext.commandInterface->stateData->packetLength);
                if (status != (int32_t)kStatus_Success)
                {
                    debug_printf("Error: handle_command returned status 0x%x\r\n", status);
                    break;
                }
                g_bootloaderContext.commandInterface->stateData->state = (int32_t)kCommandState_DataPhase;
                break;

            case (int32_t)kCommandState_DataPhase:
                status = handle_data(&hasMoreData);
                if (status != (int32_t)kStatus_Success)
                {
                    debug_printf("Error: handle_data returned status 0x%x\r\n", status);
                    g_bootloaderContext.commandInterface->stateData->state = (int32_t)kCommandState_CommandPhase;
                    break;
                }
                g_bootloaderContext.commandInterface->stateData->state =
                    hasMoreData ? (int32_t)kCommandState_DataPhase : (int32_t)kCommandState_CommandPhase;
                break;
        }
    }

    return status;
}

//! @brief Find command handler entry.
//!
//! @retval NULL if no entry found.
static const command_handler_entry_t *find_entry(uint8_t tag)
{
    const command_handler_entry_t *entry;

    if (tag < (uint8_t)kFirstCommandTag || tag > (uint8_t)kLastCommandTag)
    {
        entry = (void *)0u; // invalid command
    }
    else
    {
        entry = &g_bootloaderContext.commandInterface->handlerTable[(tag - (uint8_t)kFirstCommandTag)];
    }

    return entry;
}

//! @brief Handle a command transaction.
static status_t handle_command(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        command_packet_t *commandPacket;
    } buffer;

    buffer.bytes = packet;
    uint8_t commandTag = buffer.commandPacket->commandTag;
    status_t status = (int32_t)kStatus_Success;
    bool commandSuccess = false;

    // Look up the handler entry and save it for the data phaase.
    g_bootloaderContext.commandInterface->stateData->handlerEntry = find_entry(commandTag);

    if ((g_bootloaderContext.commandInterface->stateData->handlerEntry != (void *)0u) &&
        (g_bootloaderContext.commandInterface->stateData->handlerEntry->handleCommand != (void *)0u))
    {
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#if !(defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES)
        // Get flash security state.
        // Note: Both Main and Secondary flash share the same security state
        //  So it doesn't matter what index of allFlashState[] we use for this FLASH API.
        ftfx_security_state_t securityState;
        status = g_bootloaderContext.flashDriverInterface->flash_get_security_state(
            g_bootloaderContext.allFlashState, &securityState);
        if (status == (int32_t)kStatus_Success)
        {
            // If flash security is enabled, make sure the command is one that is allowed. If
            // it's not, then we return an error response.
            if ((securityState != kFTFx_SecurityStateNotSecure) &&
                !IS_CMD_AVAILABLE((uint8_t)kCommandsAllowedWhenSecure, commandTag))
            {
                // Security is enabled and the command is not one of the few that can be
                // run, so return a security violation error.
                debug_printf("Error: command 0x%x not available due to flash security\r\n", buffer.commandPacket->commandTag);
                status = (int32_t)kStatus_SecurityViolation;
            }
            else
            {
#endif // !BL_DEVICE_IS_LPC_SERIES
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
                // Process the command normally.
                g_bootloaderContext.commandInterface->stateData->handlerEntry->handleCommand(packet, packetLength);
                status = (int32_t)kStatus_Success;
                commandSuccess = true;
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#if !(defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES)
            }
        }
#endif // !BL_DEVICE_IS_LPC_SERIES
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
    }
    else
    {
        // We don't recognize this command, so return an error response.
        debug_printf("unknown command 0x%x\r\n", buffer.commandPacket->commandTag);
        status = (int32_t)kStatus_UnknownCommand;
    }

    if (!commandSuccess)
    {
        // Should only get to this point if an error occurred before running the command handler.
        send_generic_response((uint32_t)status, (uint32_t)commandTag);
    }

    return status;
}

//! @brief Handle a data transaction.
static status_t handle_data(bool *hasMoreData)
{
    status_t status = (int32_t)kStatus_Success;
    if (g_bootloaderContext.commandInterface->stateData->handlerEntry != (void *)0u)
    {
        // Run data phase if present, otherwise just return success.
        *hasMoreData = false;
        status = (g_bootloaderContext.commandInterface->stateData->handlerEntry->handleData != (void *)0u) ?
                   g_bootloaderContext.commandInterface->stateData->handlerEntry->handleData(hasMoreData) :
                   (int32_t)kStatus_Success;
    }

    debug_printf("Error: no handler entry for data phase\r\n");
    return status;
}

////////////////////////////////////////////////////////////////////////////////
// Command Handlers
////////////////////////////////////////////////////////////////////////////////

//! @brief Reset command handler.
static void handle_reset(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        command_packet_t *commandPacket;
    } buffer;

    buffer.bytes = packet;
    send_generic_response((uint32_t)kStatus_Success, (uint32_t)buffer.commandPacket->commandTag);

    // Wait for the ack from the host to the generic response
    (void)g_bootloaderContext.activePeripheral->packetInterface->finalize(g_bootloaderContext.activePeripheral);

    // Prepare for shutdown.
    shutdown_cleanup(kShutdownType_Reset);
#if defined(BL_FEATURE_6PINS_PERIPHERAL) && BL_FEATURE_6PINS_PERIPHERAL
    shutdown_cleanup(kShutdownType_Cleanup);
#endif // BL_FEATURE_6PINS_PERIPHERAL

    NVIC_SystemReset();
    // Does not get here.
    assert(0);
}

#if defined(BL_FEATURE_RELIABLE_UPDATE) && BL_FEATURE_RELIABLE_UPDATE
//! @brief Reliable Update command handler.
static void handle_reliable_update(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        command_packet_t *commandPacket;
    } buffer;

    union
    {
        uint8_t *bytes;
        reliable_update_packet_t *command;
    } buffer1;

    buffer.bytes = packet;
    buffer1.bytes = packet;
    status_t status = (int32_t)kStatus_Success;

    uint32_t address = buffer1.command->address;

    // Call reliable update implementation..
    bootloader_reliable_update_as_requested(kReliableUpdateOption_Swap, address);

    status = (int32_t)g_bootloaderContext.propertyInterface->store->reliableUpdateStatus;
    send_generic_response((uint32_t)status, (uint32_t)buffer.commandPacket->commandTag);

    // A system reset is needed, For HW implementation swap command will only take
    //  effect after reset; For SW implementation, reset makes sure that behavior is consistent
    if (status == (int32_t)kStatus_ReliableUpdateSuccess)
    {
        // Wait for the ack from the host to the generic response
        (void)g_bootloaderContext.activePeripheral->packetInterface->finalize(g_bootloaderContext.activePeripheral);

        // Prepare for shutdown.
        shutdown_cleanup(kShutdownType_Reset);

        NVIC_SystemReset();
        // Does not get here.
        assert(0u);
    }
}
#endif // BL_FEATURE_RELIABLE_UPDATE

//! @brief Reset data phase variables.
static void reset_data_phase(void)
{
    (void)memset(&g_bootloaderContext.commandInterface->stateData->dataPhase, 0,
           sizeof(g_bootloaderContext.commandInterface->stateData->dataPhase));
}

//! @brief Flash Erase All command handler.
static void handle_flash_erase_all(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        flash_erase_all_packet_t *commandPacket;
    } buffer;

    buffer.bytes = packet;
    status_t status = (int32_t)kStatus_Success;

// Call flash erase all implementation.
// For target without QSPI module, ignore the memory identifier
#if (!(defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE)) && \
    (!(defined(BL_FEATURE_FAC_ERASE) && BL_FEATURE_FAC_ERASE)) && \
    (!(defined(BL_FEATURE_EXPAND_MEMORY) && BL_FEATURE_EXPAND_MEMORY)) && \
    (!(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH))
    status = flash_mem_erase_all();
#if defined(BL_FEATURE_SUPPORT_DFLASH ) && BL_FEATURE_SUPPORT_DFLASH
    if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
    {
        status += flexNVM_mem_erase_all();   
    }
#endif // BL_FEATURE_SUPPORT_DFLASH    
#else
    switch (buffer.commandPacket->memoryId)
    {
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#if defined(BL_FEATURE_FAC_ERASE) && BL_FEATURE_FAC_ERASE
        case kMemoryInternal:
            status = flash_mem_erase_all(kFlashEraseAllOption_Blocks);
#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH
            if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
            {
                status += flexNVM_mem_erase_all();  
            }
#endif // BL_FEATURE_SUPPORT_DFLASH             
            break;
        case kMemoryFlashExecuteOnly:
            status = flash_mem_erase_all(kFlashEraseAllOption_ExecuteOnlySegments);
            break;
#else
        case kMemoryInternal:
            status = flash_mem_erase_all();
#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH
            if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
            {
                status += flexNVM_mem_erase_all();   
            }
#endif // BL_FEATURE_SUPPORT_DFLASH             
            break;
#endif // BL_FEATURE_FAC_ERASE
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE) && BL_FEATURE_FLEXSPI_NOR_MODULE
        case kMemoryFlexSpiNor:
            status = flexspi_nor_mem_erase_all();
            break;
#endif // #if BL_FEATURE_FLEXSPI_NOR_MODULE
#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
        case kMemoryQuadSpi0:
            status = qspi_mem_erase_all();
            break;
#endif
#if defined(BL_FEATURE_SPINAND_MODULE) && BL_FEATURE_SPINAND_MODULE
        case kMemorySpiNand:
            status = spinand_mem_erase_all();
            break;
#endif
#if defined(BL_FEATURE_SPIFI_NOR_MODULE) && BL_FEATURE_SPIFI_NOR_MODULE
        case kMemorySpifiNor:
            status = spifi_nor_mem_erase_all();
            break;
#endif // #if BL_FEATURE_SPIFI_NOR_MODULE
#if defined(BL_FEATURE_SEMC_NOR_MODULE) && BL_FEATURE_SEMC_NOR_MODULE
        case kMemorySemcNor:
            status = semc_nor_mem_erase_all();
            break;
#endif
#if defined(BL_FEATURE_SEMC_NAND_MODULE) && BL_FEATURE_SEMC_NAND_MODULE
        case kMemorySemcNand:
            status = semc_nand_mem_erase_all();
            break;
#endif
#if defined(BL_FEATURE_SPI_NOR_EEPROM_MODULE) && BL_FEATURE_SPI_NOR_EEPROM_MODULE
        case kMemorySpiNorEeprom:
            status = spi_nor_eeprom_mem_erase_all();
            break;
#endif
#if defined(BL_FEATURE_MMC_MODULE) && BL_FEATURE_MMC_MODULE
        case kMemoryMMCCard:
            status = mmc_mem_erase_all();
            break;
#endif
#if defined(BL_FEATURE_SD_MODULE) && BL_FEATURE_SD_MODULE
        case kMemorySDCard:
            status = sd_mem_erase_all();
            break;
#endif
        default:
            status = kStatus_InvalidArgument;
            break;
    }
#endif // #if ((!BL_FEATURE_QSPI_MODULE) && (!BL_FEATURE_FAC_ERASE) && (!BL_FEATURE_EXPAND_MEMORY) &&
// (!BL_FEATURE_HAS_NO_INTERNAL_FLASH))

    send_generic_response((uint32_t)status, (uint32_t)buffer.commandPacket->commandPacket.commandTag);
}

#if defined(BL_FEATURE_ERASEALL_UNSECURE) && BL_FEATURE_ERASEALL_UNSECURE
//! @brief Flash Erase All Unsecure command handler.
static void handle_flash_erase_all_unsecure(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        command_packet_t *commandPacket;
    } buffer;

    buffer.bytes = packet;
    status_t status = (int32_t)kStatus_Success;

// Call flash erase all unsecure implementation.
#if defined(BL_FEATURE_ERASEALL_UNSECURE) && BL_FEATURE_ERASEALL_UNSECURE
    status = flash_mem_erase_all_unsecure();
#if defined(BL_FEATURE_SUPPORT_DFLASH ) && BL_FEATURE_SUPPORT_DFLASH
    if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
    {
        status += flexNVM_mem_erase_all_unsecure();   
    }
#endif // BL_FEATURE_SUPPORT_DFLASH     
#endif //BL_FEATURE_ERASEALL_UNSECURE

    send_generic_response((uint32_t)status, (uint32_t)buffer.commandPacket->commandTag);
}
#endif

//! @brief Flash Erase Region command handler.
static void handle_flash_erase_region(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        flash_erase_region_packet_t *command;
    } buffer;

    buffer.bytes = packet;
    status_t status = (int32_t)kStatus_Success;

// Call flash erase region implementation.
    status = g_bootloaderContext.memoryInterface->erase(buffer.command->startAddress, buffer.command->byteCount, buffer.command->memoryId);

    send_generic_response((uint32_t)status, (uint32_t)buffer.command->commandPacket.commandTag);
}

//! @brief Receive SB File command handler.
static void handle_receive_sb_file(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        receive_sb_file_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    // Start the data phase.
    reset_data_phase();
    g_bootloaderContext.commandInterface->stateData->dataPhase.count = buffer.command->byteCount;
    g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag = (uint8_t)kCommandTag_ReceiveSbFile;
    send_generic_response((uint32_t)kStatus_Success, (uint32_t)buffer.command->commandPacket.commandTag);

    // Initialize the SB file loader state machine
    (void)sbloader_init();
}

//! @brief Get Property command handler.
static void handle_get_property(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        get_property_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    uint32_t *value = (void *)0u;
    uint32_t valueSize = 0u;
    status_t status = g_bootloaderContext.propertyInterface->get(buffer.command->propertyTag, buffer.command->memoryId,
                                                                 (const void **)&value, &valueSize);

    // Make sure the property's size is no more than the size of the max number of return parameters.
    assert(valueSize <= (kMaxPropertyReturnValues * sizeof(uint32_t)));

    // Currently there are no property responses that contain a data phase.
    g_bootloaderContext.commandInterface->stateData->dataPhase.count = 0u;
    send_get_property_response((uint32_t)status, value, (valueSize / sizeof(uint32_t)));
}

//! @brief Set Property command handler.
static void handle_set_property(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        set_property_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    status_t status = g_bootloaderContext.propertyInterface->set_uint32(buffer.command->propertyTag, buffer.command->propertyValue);

    send_generic_response((uint32_t)status, (uint32_t)buffer.command->commandPacket.commandTag);
}

#if (defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE) || \
    (defined(BL_FEATURE_FLEXSPI_NOR_MODULE) && BL_FEATURE_FLEXSPI_NOR_MODULE) || \
    (defined(BL_FEATURE_SEMC_NOR_MODULE) && BL_FEATURE_SEMC_NOR_MODULE) || \
    (defined(BL_FEATURE_EXPAND_MEMORY) && BL_FEATURE_EXPAND_MEMORY) || \
    (defined(BL_FEATURE_SPI_NOR_EEPROM_MODULE) && BL_FEATURE_SPI_NOR_EEPROM_MODULE) || \
    (defined(BL_FEATURE_SPIFI_NOR_MODULE) && BL_FEATURE_SPIFI_NOR_MODULE)
//! @brief Configure memory command handler.
static void handle_configure_memory(uint8_t *packet, uint32_t packetLength)
{
    configure_memory_packet_t *command = (configure_memory_packet_t *)packet;
    status_t status = kStatus_InvalidArgument;

#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
    if (command->flashMemId == kMemoryQuadSpi0)
    {
        status = configure_qspi(command->configBlockAddress);
    }
#endif // BL_FEATURE_QSPI_MODULE

#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE) && BL_FEATURE_FLEXSPI_NOR_MODULE
        if (command->flashMemId == kMemoryFlexSpiNor)
    {
        status = flexspi_nor_mem_config((uint32_t *)command->configBlockAddress);
    }
#endif // #if BL_FEATURE_FLEXSPI_NOR_MODULE

#if defined(BL_FEATURE_SPIFI_NOR_MODULE) && BL_FEATURE_SPIFI_NOR_MODULE
    if (command->flashMemId == kMemorySpifiNor)
    {
        status = spifi_nor_mem_config((uint32_t *)command->configBlockAddress);
    }
#endif // #if BL_FEATURE_SPIFI_NOR_MODULE

#if defined(BL_FEATURE_SEMC_NOR_MODULE) && BL_FEATURE_SEMC_NOR_MODULE
        if (command->flashMemId == kMemorySemcNor)
    {
        status = semc_nor_mem_config((uint32_t *)command->configBlockAddress);
    }
    else
#endif // #if BL_FEATURE_SEMC_NOR_MODULE

#if defined(BL_FEATURE_EXPAND_MEMORY) && BL_FEATURE_EXPAND_MEMORY
        if (GROUPID(command->flashMemId) == kGroup_External)
    {
        uint32_t index;
        status = find_external_map_index(command->flashMemId, &index);
        if (status == (int32_t)kStatus_Success)
        {
            external_memory_map_entry_t *map =
                (external_memory_map_entry_t *)&g_bootloaderContext.externalMemoryMap[index];
            status = map->memoryInterface->config((uint32_t *)command->configBlockAddress);
        }
    }
#endif // #if BL_FEATURE_EXPAND_MEMORY
    send_generic_response((uint32_t)status, (uint32_t)command->commandPacket.commandTag);
}
#endif // BL_FEATURE_QSPI_MODULE || BL_FEATURE_FLEXSPI_NOR_MODULE || BL_FEATURE_EXPAND_MEMORY || BL_FEATURE_SPI_NOR_EEPROM_MODULE || BL_FEATURE_SPIFI_NOR_MODULE

#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
status_t configure_qspi(const uint32_t address)
{
    status_t status = (int32_t)kStatus_Success;

    uint32_t startAddr = address;
    uint32_t endAddr = startAddr + sizeof(qspi_config_t) - 1u;

    // Validate parameters.
    if ((!is_valid_application_location(startAddr)) || (!is_valid_application_location(endAddr)))
    {
        status = kStatus_InvalidArgument;
    }

    // Call configure quadspi implementation.
    if (status == (int32_t)kStatus_Success)
    {
        status = quadspi_init((void *)startAddr);
        if (status == (int32_t)kStatus_Success)
        {
            // Re-init memory interface to intialize qspi memory interface
            g_bootloaderContext.memoryInterface->init();
        }
        g_bootloaderContext.propertyInterface->store->qspiInitStatus = status;
    }

    return status;
}
#endif // BL_FEATURE_QSPI_MODULE

//! @brief Write Memory command handler.
static void handle_write_memory(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        write_memory_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    // Start the data phase.
    reset_data_phase();
    g_bootloaderContext.commandInterface->stateData->dataPhase.memoryId = buffer.command->memoryId;
    g_bootloaderContext.commandInterface->stateData->dataPhase.count = buffer.command->byteCount;
    g_bootloaderContext.commandInterface->stateData->dataPhase.address = buffer.command->startAddress;
    g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag = (uint8_t)kCommandTag_WriteMemory;
    send_generic_response((uint32_t)kStatus_Success, (uint32_t)buffer.command->commandPacket.commandTag);
}

#if defined(BL_FEATURE_GEN_KEYBLOB) && BL_FEATURE_GEN_KEYBLOB

#define GEN_KEYBLOB_KEY_SIZE (16) // 128 bit
#define GEN_KEYBLOB_KEY_SIZE_WORD (GEN_KEYBLOB_KEY_SIZE / 4)
#define GEN_KEYBLOB_SIZE (72) // key blob size
static uint32_t key_data[GEN_KEYBLOB_KEY_SIZE_WORD];
static uint8_t key_blob[GEN_KEYBLOB_SIZE];

//! @brief Generate Key Blob command handler.
static void handle_generate_key_blob(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        generate_key_blob_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    // Start the data phase.
    reset_data_phase();
    g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag = kCommandTag_GenerateKeyBlob;
    g_bootloaderContext.commandInterface->stateData->dataPhase.option = buffer.command->keyDataPhase;

    if (buffer.command->keyDataPhase == 0u)
    {
        // receiving DEK key 16 bytes (128bits key)
        g_bootloaderContext.commandInterface->stateData->dataPhase.count = GEN_KEYBLOB_KEY_SIZE;
        g_bootloaderContext.commandInterface->stateData->dataPhase.address = (uint32_t)&key_data[0];

        send_generic_response((uint32_t)kStatus_Success, (uint32_t)buffer.command->commandPacket.commandTag);
    }
    else
    {
        // generate key blob 72 bytes
        generate_key_blob((uint32_t *)&key_data[0], key_blob);

        g_bootloaderContext.commandInterface->stateData->dataPhase.count = GEN_KEYBLOB_SIZE;
        g_bootloaderContext.commandInterface->stateData->dataPhase.address = (uint32_t)&key_blob[0];

        send_generate_key_blob_response(kStatus_Success, 72u);
    }
}
#endif

//! @brief Read Memory command handler.
static void handle_read_memory(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        read_memory_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    // Start the data phase.
    reset_data_phase();
    g_bootloaderContext.commandInterface->stateData->dataPhase.memoryId = buffer.command->memoryId;
    g_bootloaderContext.commandInterface->stateData->dataPhase.count = buffer.command->byteCount;
    g_bootloaderContext.commandInterface->stateData->dataPhase.address = buffer.command->startAddress;
    g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag = (uint8_t)kCommandTag_ReadMemory;
    send_read_memory_response((uint32_t)kStatus_Success, buffer.command->byteCount);
}

//! @brief Complete the data phase, optionally send a response.
static void finalize_data_phase(status_t status)
{
    g_bootloaderContext.commandInterface->stateData->dataPhase.address = 0u;
    g_bootloaderContext.commandInterface->stateData->dataPhase.count = 0u;

    // Force to write cached data to target memory
    if (g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag == (uint8_t)kCommandTag_WriteMemory)
    {
        assert(g_bootloaderContext.memoryInterface->flush);
        status_t flushStatus = g_bootloaderContext.memoryInterface->flush();

        // Update status only if the last operation result is successfull in order to reflect
        // real result of the write operation.
        if (status == (int32_t)kStatus_Success)
        {
            status = flushStatus;
        }
    }
#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
    if ((status == kStatus_Success) &&
        (g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag == kCommandTag_KeyProvisioning))
    {
        if (g_bootloaderContext.commandInterface->stateData->dataPhase.argument0 ==
            kKeyProvisioning_Operation_SetUserKey)
        {
            skboot_key_metadata_t keyMetaData = {.keyType = kPUF_KeyType_Invalid };
            keyMetaData.keyType =
                (skboot_key_type_t)g_bootloaderContext.commandInterface->stateData->dataPhase.argument1;
            status = skboot_key_secretbox(
                s_userKeyBuffer, g_bootloaderContext.commandInterface->stateData->dataPhase.argument2, &keyMetaData);
            switch (status)
            {
                case kStatus_SKBOOT_Success:
                    status = kStatus_Success;
                    break;
                case kStatus_SKBOOT_Fail:
                    status = kStatus_Fail;
                    break;
                case kStatus_SKBOOT_InvalidArgument:
                    status = kStatus_InvalidArgument;
                    break;
                default:
                    // no change
                    break;
            }
        }
    }
#endif // #if BL_FEATURE_KEY_PROVISIONING
#if defined(BL_FEATURE_EXPAND_MEMORY) && BL_FEATURE_EXPAND_MEMORY
    // Reset the state machine of memory interface.
    assert(g_bootloaderContext.memoryInterface->finalize);
    status_t finalizeStatus = g_bootloaderContext.memoryInterface->finalize();
    if (status == (int32_t)kStatus_Success)
    {
        status = finalizeStatus;
    }
#endif // BL_FEATURE_EXPAND_MEMORY

    // Send final response packet.
    send_generic_response((uint32_t)status, (uint32_t)g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag);

#if !(defined(BL_FEATURE_MIN_PROFILE) && BL_FEATURE_MIN_PROFILE)
    if ((status == (int32_t)kStatus_AbortDataPhase) &&
        g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag == (uint8_t)kCommandTag_ReceiveSbFile)
    {
        // Aborting due to sb loader jump or reset command.
        // If jump or reset successful, this will not return.
        // In the current architecture there is no way to handle an error return from sbloader_finalize()
        // because we already sent the "abort" status above to indicate that a jump command was encountered.
        (void)sbloader_finalize();
    }
#endif // !BL_FEATURE_MIN_PROFILE
}

#if defined(BL_FEATURE_GEN_KEYBLOB) && BL_FEATURE_GEN_KEYBLOB
//! @brief Handle generate key blob data phase
static status_t handle_key_blob_data(bool *hasMoreData)
{
    if (g_bootloaderContext.commandInterface->stateData->dataPhase.option == 0u)
    {
        return handle_data_consumer(hasMoreData);
    }
    else
    {
        return handle_data_producer(hasMoreData);
    }
}
#endif

#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
//! @brief Handle data phase, direction is defined in dataPhase.option.
static status_t handle_data_bidirection(bool *hasMoreData)
{
    status_t retStatus = (int32_t)kStatus_Success;

    if (g_bootloaderContext.commandInterface->stateData->dataPhase.option == (uint8_t)kCmd_DataPhase_Option_Consumer)
    {
        retStatus = handle_data_consumer(hasMoreData);
    }
    else if (g_bootloaderContext.commandInterface->stateData->dataPhase.option == (uint8_t)kCmd_DataPhase_Option_Producer)
    {
        retStatus = handle_data_producer(hasMoreData);
    }
    else // Skip the data phase.
    {
        *hasMoreData = false;
    }
    return retStatus;
}
#endif

//! @brief Handle data phase with data consumer (read from host).
static status_t handle_data_consumer(bool *hasMoreData)
{
    status_t retStatus = (int32_t)kStatus_Success;

    if (g_bootloaderContext.commandInterface->stateData->dataPhase.count == 0u)
    {
        // No data phase.
        *hasMoreData = false;
        finalize_data_phase((int32_t)kStatus_Success);
    }
    else
    {
        *hasMoreData = true;
        uint32_t memoryId = g_bootloaderContext.commandInterface->stateData->dataPhase.memoryId;
        uint32_t remaining = g_bootloaderContext.commandInterface->stateData->dataPhase.count;
        uint32_t dataAddress = g_bootloaderContext.commandInterface->stateData->dataPhase.address;
        uint8_t *packet;
        uint32_t packetLength = 0u;
        status_t status;

        // Read the data packet.
        status = g_bootloaderContext.activePeripheral->packetInterface->readPacket(
            g_bootloaderContext.activePeripheral, &packet, &packetLength, kPacketType_Data);
        if (status != (int32_t)kStatus_Success)
        {
            // Abort data phase due to error.
            debug_printf("consumer abort data phase due to status 0x%x\r\n", status);
            g_bootloaderContext.activePeripheral->packetInterface->abortDataPhase(g_bootloaderContext.activePeripheral);
            finalize_data_phase(status);
            *hasMoreData = false;
        }
        else
        {
            if (packetLength == 0u)
            {
                // Sender requested data phase abort.
                debug_printf("Data phase aborted by sender\r\n");
                finalize_data_phase((int32_t)kStatus_AbortDataPhase);
                *hasMoreData = false;
            }
            else
            {
                //
                // Write the data to the destination address.
                //

                packetLength = MIN(packetLength, remaining);

#if !(defined(BL_FEATURE_MIN_PROFILE) && BL_FEATURE_MIN_PROFILE)
                if (g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag == (uint8_t)kCommandTag_ReceiveSbFile)
                {
                    // Consumer is sb loader state machine
                    g_bootloaderContext.commandInterface->stateData->dataPhase.data = packet;
                    g_bootloaderContext.commandInterface->stateData->dataPhase.dataBytesAvailable = packetLength;

                    status = sbloader_pump(packet, packetLength);

                    // kStatusRomLdrDataUnderrun means need more data
                    // kStatusRomLdrSectionOverrun means we reached the end of the sb file processing
                    // either of these are OK
                    if ((status == (int32_t)kStatusRomLdrDataUnderrun) || (status == (int32_t)kStatusRomLdrSectionOverrun))
                    {
                        status = (int32_t)kStatus_Success;
                    }
                }
                else if (g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag == (uint8_t)kCommandTag_GenerateKeyBlob)
                {
                    (void)memcpy((uint8_t *)dataAddress, packet, packetLength);
                    dataAddress += packetLength;
                }
                else
#endif // !BL_FEATURE_MIN_PROFILE
                {
#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
                    if (g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag == (uint8_t)kCommandTag_KeyProvisioning)
                    {
                        memcpy((uint8_t *)dataAddress, packet, packetLength);
                        dataAddress += packetLength;
                    }
                    else
#endif // BL_FEATURE_KEY_PROVISIONING
                    {
                        // Consumer is memory interface.
                        status = g_bootloaderContext.memoryInterface->write(dataAddress, packetLength, packet, memoryId);
                        dataAddress += packetLength;
                    }
#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH
                     const memory_map_entry_t *mapEntry = (void *)0u;  
                     (void)find_map_entry((dataAddress - packetLength), packetLength, &mapEntry);

#if defined(BL_FEATURE_SUPPORT_PFLASH_DFLASH_CROSS_WRITE) && BL_FEATURE_SUPPORT_PFLASH_DFLASH_CROSS_WRITE
                    if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
                    {      
                        if ((mapEntry != (void *)0u) && 
                            (mapEntry->memoryProperty == ((uint32_t)kMemoryIsExecutable | (uint32_t)kMemoryType_FLASH)) &&
                            (dataAddress > mapEntry->endAddress))
                        { 
                            assert(g_bootloaderContext.memoryInterface->flush);
                            (void)g_bootloaderContext.memoryInterface->flush(); 
                            dataAddress = FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS;
                        }
                    }
#else
                    if (dataAddress > mapEntry->endAddress)
                    {
                         finalize_data_phase(kStatus_OutOfRange);   
                    }
#endif  // BL_FEATURE_SUPPORT_PFLASH_DFLASH_CROSS_WRITE         
#endif // BL_FEATURE_SUPPORT_DFLASH   
                } 
                remaining -= packetLength;

#ifdef TEST_RECEIVER_ABORT
                status = kStatus_Fail;
#endif

                if (remaining == 0u)
                {
                    finalize_data_phase(status);
                    *hasMoreData = false;
                }
                else if (status != (int32_t)kStatus_Success)
                {
                    // Abort data phase due to error.
                    debug_printf("Data phase error 0x%x, aborting\r\n", status);
                    g_bootloaderContext.activePeripheral->packetInterface->abortDataPhase(g_bootloaderContext.activePeripheral);
                    finalize_data_phase(status);
                    *hasMoreData = false;
                }
                else
                {
                    g_bootloaderContext.commandInterface->stateData->dataPhase.count = remaining;
                    g_bootloaderContext.commandInterface->stateData->dataPhase.address = dataAddress;
                }
            }
        }
    }
    return retStatus;
}

//! @brief Handle data phase with data producer (send to host).
static status_t handle_data_producer(bool *hasMoreData)
{
    status_t retStatus = (int32_t)kStatus_Success;
    if (g_bootloaderContext.commandInterface->stateData->dataPhase.count == 0u)
    {
        // No data phase.
        *hasMoreData = false;
        finalize_data_phase((int32_t)kStatus_Success);
    }
    else
    {
        *hasMoreData = true;
        uint32_t memoryId = g_bootloaderContext.commandInterface->stateData->dataPhase.memoryId;
        uint32_t remaining = g_bootloaderContext.commandInterface->stateData->dataPhase.count;
        uint32_t dataAddress = g_bootloaderContext.commandInterface->stateData->dataPhase.address;
        uint8_t *data = g_bootloaderContext.commandInterface->stateData->dataPhase.data;
        uint8_t commandTag = g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag;
        status_t status = (int32_t)kStatus_Success;

        // Initialize the data packet to send.
        uint32_t packetSize;
#if defined(BL_FEATURE_EXPAND_PACKET_SIZE) && BL_FEATURE_EXPAND_PACKET_SIZE
        uint8_t *packet = s_dataProducerPacket;
        uint32_t packetBufferSize =
            g_bootloaderContext.activePeripheral->packetInterface->getMaxPacketSize(g_bootloaderContext.activePeripheral);
        packetSize = MIN(packetBufferSize, remaining);
#else
        uint8_t packet[(uint32_t)kMinPacketBufferSize];
        packetSize = MIN((uint32_t)kMinPacketBufferSize, remaining);
#endif // BL_FEATURE_EXPAND_PACKET_SIZE

        // Copy the data into the data packet.
        if (data != (void *)0u)
        {
            // Copy data using compiler-generated memcpy.
            (void)memcpy(packet, data, packetSize);
            data += packetSize;
            status = (int32_t)kStatus_Success;
        }
        else
        {
            if (commandTag == (uint8_t)kCommandTag_ReadMemory)
            {
                // Copy data using memory interface.
                status = g_bootloaderContext.memoryInterface->read(dataAddress, packetSize, packet, memoryId);
            }
#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
            else if (g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag == kCommandTag_KeyProvisioning)
            {
                (void)memcpy(packet, (uint8_t *)dataAddress, packetSize);
            }
#endif
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#if !(defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES)
            else if (commandTag == (uint8_t)kCommandTag_FlashReadResource)
            {
      // Read data from special-purpose flash memory
                ftfx_read_resource_opt_t option =
                    (ftfx_read_resource_opt_t)g_bootloaderContext.commandInterface->stateData->dataPhase.option;
                lock_acquire();
                // Note: Both Main and Secondary flash share the same IFR Memory
                //  So it doesn't matter what index of allFlashState[] we use for this FLASH API.
                status = g_bootloaderContext.flashDriverInterface->flash_read_resource(
                    g_bootloaderContext.allFlashState, dataAddress, (uint8_t *)packet, packetSize,
                    option);
                lock_release();
            }
#endif // !BL_DEVICE_IS_LPC_SERIES
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
            else if (commandTag == (uint8_t)kCommandTag_GenerateKeyBlob)
            {
                (void)memcpy(packet, (uint8_t *)dataAddress, packetSize);
            }
            else
            {
            }
            dataAddress += packetSize;
        }

        if (status != (int32_t)kStatus_Success)
        {
      //        debug_printf("Error: %s returned status 0x%x, abort data phase\r\n",
      //                     (commandTag == (uint8_t)kCommandTag_ReadMemory) ? "read memory" : "flash read resource", status);
            // Send zero length packet to tell host we are aborting data phase
            (void)g_bootloaderContext.activePeripheral->packetInterface->writePacket(
                g_bootloaderContext.activePeripheral, (const uint8_t *)packet, 0u, kPacketType_Data);
            finalize_data_phase(status);
            *hasMoreData = false;
        }
        else
        {
            remaining -= packetSize;

#ifdef TEST_SENDER_ABORT
      // Disble IAR "statement is unreachable" error
#pragma diag_suppress = Pe111
        // Send zero length packet to abort data phase.
        g_bootloaderContext.activePeripheral->packetInterface->writePacket(g_bootloaderContext.activePeripheral,
                                                                           (const uint8_t *)packet, 0u, kPacketType_Data);
        finalize_data_phase(kStatus_AbortDataPhase);
        *hasMoreData = false;
        return kStatus_Success;
#endif // TEST_SENDER_ABORT;

            status = g_bootloaderContext.activePeripheral->packetInterface->writePacket(
                g_bootloaderContext.activePeripheral, (const uint8_t *)packet, packetSize, kPacketType_Data);

            if (remaining == 0u)
            {
                finalize_data_phase(status);
                *hasMoreData = false;
            }
            else if (status != (int32_t)kStatus_Success)
            {
                debug_printf("writePacket aborted due to status 0x%x\r\n", status);
                finalize_data_phase(status);
                *hasMoreData = false;
            }
            else
            {
                g_bootloaderContext.commandInterface->stateData->dataPhase.count = remaining;
                g_bootloaderContext.commandInterface->stateData->dataPhase.address = dataAddress;
            }
        }
    }
    return retStatus;
}

//! @brief Fill Memory command handler.
static void handle_fill_memory(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        fill_memory_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    status_t status =
        g_bootloaderContext.memoryInterface->fill(buffer.command->startAddress, buffer.command->byteCount, buffer.command->patternWord);

    send_generic_response((uint32_t)status, (uint32_t)buffer.command->commandPacket.commandTag);
}

//! @brief Execute command handler.
static void handle_execute(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        execute_call_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    static uint32_t s_addr = 0u;
    uint32_t call_address = buffer.command->callAddress;
    uint32_t argument_word = buffer.command->argumentWord;
    s_addr = buffer.command->stackpointer;
    status_t responseStatus = (int32_t)kStatus_Success;

    // Validate stack pointer address. It must either be 0 or within the RAM range.
    if (!((s_addr == 0u) || is_valid_stackpointer_location(s_addr)))
    {
        // Invalid stack pointer value, respond with kStatus_InvalidArgument.
        responseStatus = (int32_t)kStatus_InvalidArgument;
    }

    // Validate call address.
    if (!is_valid_application_location(call_address))
    {
        // Invalid address, respond with kStatus_InvalidArgument.
        responseStatus = (int32_t)kStatus_InvalidArgument;
    }

#if defined(BL_FEATURE_OTFAD_MODULE) && BL_FEATURE_OTFAD_MODULE
    if (is_qspi_present())
    {
        quadspi_cache_clear();
        status_t status = otfad_init_as_needed();
        if (status != (int32_t)kStatus_Success)
        {
            responseStatus = (int32_t)kStatus_OtfadInvalidKeyBlob;
        }
        update_qspi_otfad_init_status(status);
    }
#endif

    // Send response immediately since call may not return
    send_generic_response((uint32_t)responseStatus, (uint32_t)buffer.command->commandPacket.commandTag);

    if (responseStatus == (int32_t)kStatus_Success)
    {
        static call_function_t s_callFunction = (void *)0u;
        union
        {
            uint32_t    address;
            call_function_t ptr;
        } func_ptr;

        func_ptr.address = call_address;
        s_callFunction = func_ptr.ptr;

        // Prepare for shutdown.
        shutdown_cleanup(kShutdownType_Shutdown);

        // Static variables are needed since we are changing the stack pointer out from under the compiler
        // we need to ensure the values we are using are not stored on the previous stack
        static uint32_t s_argument = 0u;
        s_argument = argument_word;

        if (s_addr != 0u)
        {
            // Set main stack pointer and process stack pointer
            __set_MSP(s_addr);
            __set_PSP(s_addr);
        }

        (void)s_callFunction(s_argument);
        // Dummy fcuntion call, should never go to this fcuntion call
        shutdown_cleanup(kShutdownType_Shutdown);
    }
}

//! @brief Call command handler.
static void handle_call(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        execute_call_packet_t *command;
    } buffer;

    buffer.bytes = packet;
    status_t responseStatus = (int32_t)kStatus_Success;

    // Validate call address.
    if (!is_valid_application_location(buffer.command->callAddress))
    {
        // Invalid address, respond with kStatus_InvalidArgument.
        responseStatus = (int32_t)kStatus_InvalidArgument;
    }
    else
    {
        union
        {
            uint32_t    address;
            call_function_t ptr;
        } func_ptr;

        func_ptr.address = buffer.command->callAddress;
        call_function_t callFunction = func_ptr.ptr;
        shutdown_cleanup(kShutdownType_Cleanup);
        responseStatus = callFunction(buffer.command->argumentWord);
    }

    send_generic_response((uint32_t)responseStatus, (uint32_t)buffer.command->commandPacket.commandTag);
}

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
//! @brief Flash Security Disable command handler.
static void handle_flash_security_disable(uint8_t *packet, uint32_t packetLength)
{
    union
    {
        uint8_t *bytes;
        flash_security_disable_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    status_t status = (int32_t)kStatus_Success;
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#if !(defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES)
    // Flash interface wants little endian, so just send two uint32s.
    // Note: Both Main and Secondary flash share the same security control
    //  So it doesn't matter what index of allFlashState[] we use for this FLASH API.
    status = g_bootloaderContext.flashDriverInterface->flash_security_bypass(
        g_bootloaderContext.allFlashState, (uint8_t *)&buffer.command->keyLow);
#endif // !BL_DEVICE_IS_LPC_SERIES
#endif

    send_generic_response((uint32_t)status, (uint32_t)buffer.command->commandPacket.commandTag);
}
#endif // !BL_FEATURE_HAS_NO_INTERNAL_FLASH

//! @brief  Flash Program Once command handler
static void handle_flash_program_once(uint8_t *packet, uint32_t length)
{
    union
    {
        uint8_t *bytes;
        flash_program_once_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    status_t status = (int32_t)kStatus_Success;

    lock_acquire();
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#if !(defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES)
    // Note: Both Main and Secondary flash share the same IFR Memory
    //  So it doesn't matter what index of allFlashState[] we use for this FLASH API.
    status = g_bootloaderContext.flashDriverInterface->flash_program_once(
        &g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main], buffer.command->index, (uint8_t *)&buffer.command->data[0], buffer.command->byteCount);
#endif // !BL_DEVICE_IS_LPC_SERIES
#elif defined(BL_FEATURE_OCOTP_MODULE) && BL_FEATURE_OCOTP_MODULE
    status = ocotp_program_once(OCOTP, buffer.command->index, &buffer.command->data[0], buffer.command->byteCount);
#elif defined(BL_DEVICE_IS_LPC_SERIES) && defined(OTP_API)
    status = ocotp_program_once(buffer.command->index, &buffer.command->data[0], buffer.command->byteCount);
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
    lock_release();

    send_generic_response((uint32_t)status, (uint32_t)buffer.command->commandPacket.commandTag);
}

//! @brief  Flash Read Once command handler
static void handle_flash_read_once(uint8_t *packet, uint32_t length)
{
    union
    {
        uint8_t *bytes;
        flash_read_once_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    uint32_t readOnceItemData[2] = { 0u };

    status_t status = (int32_t)kStatus_Success;

    lock_acquire();
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#if !(defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES)
    // Note: Both Main and Secondary flash share the same IFR Memory
    //  So it doesn't matter what index of allFlashState[] we use for this FLASH API.
    status = g_bootloaderContext.flashDriverInterface->flash_read_once(
        &g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main], buffer.command->index, (uint8_t *)&readOnceItemData[0], buffer.command->byteCount);
#endif // !BL_DEVICE_IS_LPC_SERIES
#elif defined(BL_FEATURE_OCOTP_MODULE) && BL_FEATURE_OCOTP_MODULE
    status = ocotp_read_once(OCOTP, buffer.command->index, &readOnceItemData[0], buffer.command->byteCount);
#elif defined(BL_DEVICE_IS_LPC_SERIES) && defined(OTP_API)
    status = ocotp_read_once(buffer.command->index, &readOnceItemData[0], buffer.command->byteCount);
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
    lock_release();

    send_flash_read_once_response((uint32_t)status, readOnceItemData, buffer.command->byteCount);
}

//! @brief  Flash Read Resource command handler
static void handle_flash_read_resource(uint8_t *packet, uint32_t length)
{
    union
    {
        uint8_t *bytes;
        flash_read_resource_packet_t *command;
    } buffer;

    buffer.bytes = packet;

    // Start the data phase.
    reset_data_phase();
    g_bootloaderContext.commandInterface->stateData->dataPhase.count = buffer.command->byteCount;
    g_bootloaderContext.commandInterface->stateData->dataPhase.address = buffer.command->startAddress;
    g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag = (uint8_t)kCommandTag_FlashReadResource;
    g_bootloaderContext.commandInterface->stateData->dataPhase.option = (uint8_t)buffer.command->option;
    send_flash_read_resource_response((uint32_t)kStatus_Success, buffer.command->byteCount);
}

//! @brief Send a generic response packet.
static void send_generic_response(uint32_t commandStatus, uint32_t commandTag)
{
    generic_response_packet_t responsePacket;
    responsePacket.commandPacket.commandTag = (uint8_t)kCommandTag_GenericResponse;
    responsePacket.commandPacket.flags = 0u;
    responsePacket.commandPacket.reserved = 0u;
    responsePacket.commandPacket.parameterCount = 2u;
    responsePacket.status = commandStatus;
    responsePacket.commandTag = commandTag;

    status_t status = g_bootloaderContext.activePeripheral->packetInterface->writePacket(
        g_bootloaderContext.activePeripheral, (const uint8_t *)&responsePacket, sizeof(responsePacket),
        kPacketType_Command);
    if (status != (int32_t)kStatus_Success)
    {
        debug_printf("Error: writePacket returned status 0x%x\r\n", status);
    }
}

//! @brief Send a get property response packet.
static void send_get_property_response(uint32_t commandStatus, uint32_t *value, uint32_t numValues)
{
    get_property_response_packet_t responsePacket;
    responsePacket.commandPacket.commandTag = (uint8_t)kCommandTag_GetPropertyResponse;
    responsePacket.commandPacket.flags = 0u;
    responsePacket.commandPacket.reserved = 0u;
    responsePacket.commandPacket.parameterCount = (uint8_t)(1u + numValues); // status + value words
    responsePacket.status = commandStatus;

    for (uint32_t i = 0u; i < numValues; ++i)
    {
        responsePacket.propertyValue[i] = value[i];
    }

    uint32_t packetSize = (uint32_t)(sizeof(responsePacket.commandPacket) + (responsePacket.commandPacket.parameterCount * (uint32_t)sizeof(uint32_t)));

    status_t status = g_bootloaderContext.activePeripheral->packetInterface->writePacket(
        g_bootloaderContext.activePeripheral, (const uint8_t *)&responsePacket, packetSize, kPacketType_Command);
    if (status != (int32_t)kStatus_Success)
    {
        debug_printf("Error: writePacket returned status 0x%x\r\n", status);
    }
}

#if defined(BL_FEATURE_GEN_KEYBLOB) && BL_FEATURE_GEN_KEYBLOB
//! @brief Send a read memory response packet.
static void send_generate_key_blob_response(uint32_t commandStatus, uint32_t length)
{
    generate_key_blob_response_packet_t responsePacket;
    responsePacket.commandPacket.commandTag = (uint8_t)kCommandTag_GenerateKeyBlobResponse;
    responsePacket.commandPacket.flags = (uint8_t)kCommandFlag_HasDataPhase;
    responsePacket.commandPacket.reserved = 0u;
    responsePacket.commandPacket.parameterCount = 2u;
    responsePacket.status = commandStatus;
    responsePacket.dataByteCount = length;

    status_t status = g_bootloaderContext.activePeripheral->packetInterface->writePacket(
        g_bootloaderContext.activePeripheral, (const uint8_t *)&responsePacket, sizeof(responsePacket),
        kPacketType_Command);
    if (status != (int32_t)kStatus_Success)
    {
        debug_printf("Error: writePacket returned status 0x%x\r\n", status);
    }
}
#endif

//! @brief Send a read memory response packet.
static void send_read_memory_response(uint32_t commandStatus, uint32_t length)
{
    read_memory_response_packet_t responsePacket;
    responsePacket.commandPacket.commandTag = (uint8_t)kCommandTag_ReadMemoryResponse;
    responsePacket.commandPacket.flags = (uint8_t)kCommandFlag_HasDataPhase;
    responsePacket.commandPacket.reserved = 0u;
    responsePacket.commandPacket.parameterCount = 2u;
    responsePacket.status = commandStatus;
    responsePacket.dataByteCount = length;

    status_t status = g_bootloaderContext.activePeripheral->packetInterface->writePacket(
        g_bootloaderContext.activePeripheral, (const uint8_t *)&responsePacket, sizeof(responsePacket),
        kPacketType_Command);
    if (status != (int32_t)kStatus_Success)
    {
        debug_printf("Error: writePacket returned status 0x%x\r\n", status);
    }
}

//! @brief Send a flash read once resposne packet.
static void send_flash_read_once_response(uint32_t commandStatus, uint32_t *value, uint32_t byteCount)
{
    flash_read_once_response_packet_t responsePacket;
    responsePacket.commandPacket.commandTag = (uint8_t)kCommandTag_FlashReadOnceResponse;
    responsePacket.commandPacket.flags = 0u;
    responsePacket.commandPacket.reserved = 0u;
    responsePacket.commandPacket.parameterCount = 2u; // always includes two parameters: status and byte count
    responsePacket.status = commandStatus;

    if (commandStatus == (uint32_t)kStatus_Success)
    {
        responsePacket.commandPacket.parameterCount += (uint8_t)(byteCount / sizeof(uint32_t)); // add parameter: data
        responsePacket.byteCount = byteCount;
        (void)memcpy(responsePacket.data, value, byteCount);
    }
    else
    {
        responsePacket.byteCount = 0u;
    }

    uint32_t packetSize =
        (uint32_t)sizeof(responsePacket.commandPacket) + ((uint32_t)responsePacket.commandPacket.parameterCount * (uint32_t)sizeof(uint32_t));

    status_t status = g_bootloaderContext.activePeripheral->packetInterface->writePacket(
        g_bootloaderContext.activePeripheral, (const uint8_t *)&responsePacket, packetSize, kPacketType_Command);
    if (status != (int32_t)kStatus_Success)
    {
        debug_printf("Error: writePacket returned status 0x%x\r\n", status);
    }
}

//! @brief Send a flash read resource memory response packet.
static void send_flash_read_resource_response(uint32_t commandStatus, uint32_t length)
{
    flash_read_resource_response_packet_t responsePacket;
    responsePacket.commandPacket.commandTag = (uint8_t)kCommandTag_FlashReadResourceResponse;
    responsePacket.commandPacket.flags = (uint8_t)kCommandFlag_HasDataPhase;
    responsePacket.commandPacket.reserved = 0u;
    responsePacket.commandPacket.parameterCount = 2u;
    responsePacket.status = commandStatus;
    responsePacket.dataByteCount = length;

    status_t status = g_bootloaderContext.activePeripheral->packetInterface->writePacket(
        g_bootloaderContext.activePeripheral, (const uint8_t *)&responsePacket, sizeof(responsePacket),
        kPacketType_Command);
    if (status != (int32_t)kStatus_Success)
    {
        debug_printf("Error: writePacket returned status 0x%x\r\n", status);
    }
}

#if defined(BL_FEATURE_KEY_PROVISIONING) && BL_FEATURE_KEY_PROVISIONING
//! @brief Key Provisioning command handler.
static void handle_key_provisioning(uint8_t *packet, uint32_t packetLength)
{
    status_t status = kStatus_Success;

    key_provisioning_packet_t *command = (key_provisioning_packet_t *)packet;
    uint32_t operation = command->operation;
    bool isDataPhaseRequired = false;
    skboot_key_metadata_t keyMetaData = {.keyType = kPUF_KeyType_Invalid };
    uint32_t memoryId = kMemoryInternal;

    g_bootloaderContext.commandInterface->stateData->dataPhase.option = (uint8_t)kCmd_DataPhase_Option_Skip;
    switch (operation)
    {
        case kKeyProvisioning_Operation_Enroll:
            status = skboot_key_secretbox_enroll();
            break;
        case kKeyProvisioning_Operation_SetUserKey:
            isDataPhaseRequired = true;
            if (command->size > kPUF_KeySizeMax)
            {
                status = (int32_t)kStatus_InvalidArgument;
                break;
            }
            // Start the data phase.
            reset_data_phase();
            g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag = kCommandTag_KeyProvisioning;
            g_bootloaderContext.commandInterface->stateData->dataPhase.count = command->size;
            g_bootloaderContext.commandInterface->stateData->dataPhase.address = (uint32_t)&s_userKeyBuffer;
            g_bootloaderContext.commandInterface->stateData->dataPhase.option = (uint8_t)kCmd_DataPhase_Option_Consumer;
            g_bootloaderContext.commandInterface->stateData->dataPhase.argument0 = command->operation;
            g_bootloaderContext.commandInterface->stateData->dataPhase.argument1 = command->type;
            g_bootloaderContext.commandInterface->stateData->dataPhase.argument2 = command->size;
            break;
        case kKeyProvisioning_Operation_SetIntrinsicKey:
            keyMetaData.keyType = (skboot_key_type_t)command->type;
            status = skboot_key_secretbox(NULL, command->size, &keyMetaData);
            break;
        case kKeyProvisioning_Operation_WriteNonVolatile:
            memoryId = command->type;
            status = skboot_hal_key_secretbox_write_to_nonvolatile(memoryId);
            break;
        case kKeyProvisioning_Operation_ReadNonVolatile:
            // uint32_t memoryId = command->type;
            // status = skboot_hal_key_secretbox_read_to_nonvolatile();
            break;
        case kKeyProvisioning_Operation_WriteKeyStore:
            isDataPhaseRequired = true;
            // Start the data phase.
            reset_data_phase();
            g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag = kCommandTag_KeyProvisioning;
            g_bootloaderContext.commandInterface->stateData->dataPhase.count = sizeof(*s_keyStore);
            g_bootloaderContext.commandInterface->stateData->dataPhase.address = (uint32_t)s_keyStore;
            g_bootloaderContext.commandInterface->stateData->dataPhase.option = (uint8_t)kCmd_DataPhase_Option_Consumer;
            break;
        case kKeyProvisioning_Operation_ReadKeyStore:
            isDataPhaseRequired = true;
            // Start the data phase.
            reset_data_phase();
            g_bootloaderContext.commandInterface->stateData->dataPhase.commandTag = kCommandTag_KeyProvisioning;
            g_bootloaderContext.commandInterface->stateData->dataPhase.count = sizeof(*s_keyStore);
            g_bootloaderContext.commandInterface->stateData->dataPhase.address = (uint32_t)s_keyStore;
            g_bootloaderContext.commandInterface->stateData->dataPhase.option = (uint8_t)kCmd_DataPhase_Option_Producer;
            break;
        default:
            status = (int32_t)kStatus_InvalidArgument;
            break;
    }

    switch (status)
    {
        case kStatus_SKBOOT_Success:
            status = (int32_t)kStatus_Success;
            break;
        case kStatus_SKBOOT_Fail:
            status = (int32_t)kStatus_Fail;
            break;
        case kStatus_SKBOOT_InvalidArgument:
            status = (int32_t)kStatus_InvalidArgument;
            break;
        default:
            // no change
            break;
    }

    if (isDataPhaseRequired)
    {
        send_key_provisioning_response((uint32_t)status, g_bootloaderContext.commandInterface->stateData->dataPhase.count);
    }
    else
    {
        send_generic_response((uint32_t)status, (uint32_t)command->commandPacket.commandTag);
    }
}

//! @brief Send a key provisioning response packet.
static void send_key_provisioning_response(uint32_t commandStatus, uint32_t length)
{
    read_memory_response_packet_t responsePacket;
    responsePacket.commandPacket.commandTag = (uint8_t)kCommandTag_KeyProvisioningResponse;
    responsePacket.commandPacket.flags = (uint8_t)kCommandFlag_HasDataPhase;
    responsePacket.commandPacket.reserved = 0u;
    responsePacket.commandPacket.parameterCount = 2u;
    responsePacket.status = commandStatus;
    responsePacket.dataByteCount = length;

    status_t status = g_bootloaderContext.activePeripheral->packetInterface->writePacket(
        g_bootloaderContext.activePeripheral, (const uint8_t *)&responsePacket, sizeof(responsePacket),
        kPacketType_Command);
    if (status != (int32_t)kStatus_Success)
    {
        debug_printf("Error: writePacket returned status 0x%x\r\n", status);
    }
}
#endif // BL_FEATURE_KEY_PROVISIONING

//! @}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
