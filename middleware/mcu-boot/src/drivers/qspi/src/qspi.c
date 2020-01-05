/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <string.h>
#include "fsl_device_registers.h"
#include "qspi.h"
#include "memory/memory.h"
#include "bootloader_common.h"
#include "property/property.h"
#include "bootloader/bl_context.h"
#include "microseconds.h"
#include "utilities/fsl_rtos_abstraction.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

typedef enum _qspi_command_seqid
{
    kQspiCmd_SeqId_Read = 0UL,            //!< QSPI SEQ ID indicating Read operation
    kQspiCmd_SeqId_WriteEnable = 1UL,     //!< QSPI SEQ ID indicating Write Enable operation
    kQspiCmd_SeqId_EraseAll = 2UL,        //!< QSPI SEQ ID indicating Erase All operation
    kQspiCmd_SeqId_ReadStatus = 3UL,      //!< QSPI SEQ ID indicating Read Status operation
    kQspiCmd_SeqId_PageProgram = 4UL,     //!< QSPI SEQ ID indicating Page Program operation
    kQspiCmd_SeqId_PreErase = 6UL,        //!< QSPI SEQ ID indicating Pre Erase operation.
    kQspiCmd_SeqId_SectorErase = 7UL,     //!< QSPI SEQ ID indicating Sector Erase operation
    kQspiCmd_SeqId_Dummy = 8UL,           //!< QSPI SEQ ID indicating a dummy operation
    kQspiCmd_SeqId_PreWriteEnable = 9UL,  //!< QSPI SEQ ID indicating Pre WrteEnable operation.
    kQspiCmd_SeqId_PrePageProgram = 10UL, //!< QSPI SEQ ID indicating Pre Program operation.
    kQspiCmd_SeqId_PreReadStatus = 11UL,  //!< QSPI SEQ ID indicating Pre ReadStatus operation.
} qspi_command_seqid_t;

#define AHB_BFCR_DUMMY_MASTER_ID 0xe

typedef enum _ips_command_clk_config_option
{
    kIpsCommandClock_Config = 1,
    kIpsCommandClock_Restore = 2,
} ips_command_clock_config_option_t;

enum
{
    kInvalidArgument = 0xFFFFFFFFUL,
    kFlashReadFlagShift = 16,
    kQuadSPIModuleBusyMaxTimeoutMs = 1000,
};

typedef status_t (*qspi_callback_t)(void);
typedef status_t (*qspi_erase_all_callback_t)(uint32_t address);

static qspi_config_t qspiflash_config_param; //!< Parameters read from given address
static bool s_qspiConfigured = false; //!< The state which indicates whether QuadSPI block is successfully configured.
static uint32_t s_totalFlashSize;     //!< Total size of all flash devices connected on QuadSPI

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
//! @brief Get QuadSPI config block from given config_base
static status_t get_quadspi_config_data(qspi_config_t *config_base);
//! @brief Clear sequence pinter
static void quadspi_clear_sequence_pointer(void);
//! @brief Do write enable
static status_t quadspi_write_enable(void);
//! @brief Wait until QuadSPI module is idle
static status_t quadspi_cmd_wait_for_finish(void);
//! @brief clear QSPI AHB buffer
static void quadspi_reset_ahb_buffer(void);
//! @brief Unlock Lookup table
static void quadspi_unlockLookupTable(void);
//! @brief Lock Lookup table
static void quadspi_lockLookupTable(void);
//! @brief Wait until the external Serial NOR flash is idle
status_t quadspi_wait_while_flash_busy(void);
//! @brief Enable QSPI clock gate
static void quadspi_clock_gate_enable(void);
//! @brief Disable QSPI clock gate
static void quadspi_clock_gate_disable(void);
//! @brief Default initialization of QuadSPI serial clock
static void quadspi_default_serial_clock_configure(void);
//! @brief Configure QSPI controller to expected mode
static status_t configure_quad_spi_controller(qspi_config_t *config);
//! @brief Configure Lookup table
static void configure_lut(void);
//! @brief Configure AHB buffer
static void config_ahb_buffer(void);
//! @brief Perform config commands according to parameters in QuadSPI config block
static status_t quadspi_perform_config_cmds(qspi_config_t *config);
//! @brief Configure all external flash chips according to parameters in
//!        QuadSPI config block
static status_t quadspi_cmd_config_all_banks(qspi_config_t *config);
//! @brief Write configuration register of external Serial NOR flash according to
//!        parameters in QuadSPI config block
static void quadspi_cmd_write_register(qspi_config_t *config);
//! @brief Configure work mode of external Serial NOR flash
static void quadspi_cmd_config_workmode(qspi_config_t *config);
//! @brief Configure work mode for all connected external Serial NOR flash devices
static void quadspi_cmd_config_workmode_all_banks(qspi_config_t *config);
//! @brief Configure QuadSPI module including pinmux and controller.
static status_t configure_quadspi(qspi_config_t *config);
//! @brief Erase one sector under serial mode (non-parallel mode)
static status_t quadspi_erase_sector_under_serial_mode(uint32_t address);
//! @brief Check if the parameters for parallel mode is valid
static bool is_quadspi_parallel_mode_parameter_valid(void);
//! @brief Wait until external Serial NOR flash devices are idle
static status_t quadspi_wait_while_flash_busy_parallel_mode(uint32_t address);
//! @brief issue write enable command under parallel mode
static status_t quadspi_write_enable_parallel_mode(uint32_t address);
//! @brief Common function used for parallel mode
static status_t quadspi_common_parallel_mode(uint32_t address, qspi_callback_t callback);
//! @brief Send dummy command to external Serial NOR flash if needed.
static void quadspi_send_dummy_command(void);
//! @brief Configure QuadSPI serial command for IPS command.
static void quadspi_configure_ips_command_divider(ips_command_clock_config_option_t option);
#if BL_FEATURE_QSPI_HYPERFLASH
//! @brief Return the base address based on current Address
static uint32_t quadspi_get_base_address(uint32_t currentAddress);
#endif // #if BL_FEATURE_QSPI_HYPERFLASH
//! @brief Pre-configuration before configure QuadSPI controller
static void quadspi_pre_configure(qspi_config_t *config);
//! @brief Clear TXFIFO and RXFIFO
//! This APIs is needed only for the sequence which contains read or write instruction.
static void quadspi_clear_fifo(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
void quadspi_unlockLookupTable(void)
{
    QuadSPI0->LUTKEY = 0x5AF05AF0;
    QuadSPI0->LCKCR = 0x02;
}

void quadspi_lockLookupTable(void)
{
    QuadSPI0->LUTKEY = 0x5AF05AF0;
    QuadSPI0->LCKCR = 0x01;
}

void quadspi_clear_fifo(void)
{
    QuadSPI0->MCR |= (QuadSPI_MCR_CLR_TXF_MASK | QuadSPI_MCR_CLR_RXF_MASK);
    // Wait until this operation completes.
    while (QuadSPI0->MCR & (QuadSPI_MCR_CLR_TXF_MASK | QuadSPI_MCR_CLR_RXF_MASK))
    {
    }
}

#if BL_FEATURE_QSPI_HYPERFLASH
uint32_t quadspi_get_base_address(uint32_t currentAddress)
{
    // Only check if currentAddress with SFxxAD, remove other check because invalid input
    // has been rejected by memory interface.
    if (currentAddress < QuadSPI0->SFA1AD)
    {
        return FSL_FEATURE_QSPI_AMBA_BASE;
    }
    else if (currentAddress < QuadSPI0->SFA2AD)
    {
        return (QuadSPI0->SFA1AD);
    }
    else if (currentAddress < QuadSPI0->SFB2AD)
    {
        return (QuadSPI0->SFA2AD);
    }
    else if (currentAddress < QuadSPI0->SFB2AD)
    {
        return (QuadSPI0->SFB1AD);
    }

    return kInvalidArgument; // Return Invalid Argument.
}
#endif // #if BL_FEATURE_QSPI_HYPERFLASH

status_t quadspi_write_enable()
{
    // Send dummy command, make sure device exists continuous read mode
    quadspi_send_dummy_command();

    quadspi_clear_sequence_pointer();

#if BL_FEATURE_QSPI_HYPERFLASH
    // For some types of flash, two phases are required to perform WriteEnable(Unlock) command.
    uint32_t previousAddress = QuadSPI0->SFAR;
    if (qspiflash_config_param.need_multi_phases)
    {
        if (qspiflash_config_param.pre_unlock_cmd_address_offset != kInvalidArgument)
        {
            // Perform first phase of WriteEnable Operation
            uint32_t address =
                quadspi_get_base_address(previousAddress) + qspiflash_config_param.pre_unlock_cmd_address_offset;
            QuadSPI0->SFAR = address;
        }

        // Send Pre-WriteEnable command
        QuadSPI0->IPCR = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_PreWriteEnable);
        quadspi_cmd_wait_for_finish();

        if (qspiflash_config_param.unlock_cmd_address_offset != kInvalidArgument)
        {
            // Perform first phase of WriteEnable Operation
            uint32_t address =
                quadspi_get_base_address(previousAddress) + qspiflash_config_param.unlock_cmd_address_offset;
            QuadSPI0->SFAR = address;
        }
    }
#endif // BL_FEATURE_QSPI_HYPERFLASH
    // Send Write Enable command
    QuadSPI0->IPCR = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_WriteEnable);
    quadspi_cmd_wait_for_finish();

#if BL_FEATURE_QSPI_HYPERFLASH
    if (qspiflash_config_param.need_multi_phases)
    {
        // Restore address
        QuadSPI0->SFAR = previousAddress;
    }
#endif // BL_FEATURE_QSPI_HYPERFLASH

    return kStatus_Success;
}

status_t quadspi_cmd_wait_for_finish(void)
{
    uint64_t ticksPerMillisecond = microseconds_convert_to_ticks(1000);
    uint64_t startTicks = microseconds_get_ticks();
    uint64_t timeoutTicks = startTicks + (uint64_t)kQuadSPIModuleBusyMaxTimeoutMs * ticksPerMillisecond;

    do
    {
        uint64_t currentTicks = microseconds_get_ticks();
        if (currentTicks > timeoutTicks)
        {
            return kStatusQspiModuleBusy;
        }
    } while (QuadSPI0->SR & QuadSPI_SR_BUSY_MASK);

    return kStatus_Success;
}

void quadspi_configure_ips_command_divider(ips_command_clock_config_option_t option)
{
    uint32_t secondDivider = qspiflash_config_param.ips_command_second_divider;
    if (secondDivider < 2)
    {
        return;
    }

    // Disable QuadSPI module before changing QuadSPI Serial Clock.
    QuadSPI0->MCR |= QuadSPI_MCR_MDIS_MASK;
    if (option == kIpsCommandClock_Config)
    {
        uint32_t secondDivider = qspiflash_config_param.ips_command_second_divider;

        // secondDivider = 0, means it is unnecessary to configure second Divider
        if (secondDivider)
        {
            uint32_t previousDivider = ((QuadSPI0->MCR & QuadSPI_MCR_SCLKCFG_MASK) >> QuadSPI_MCR_SCLKCFG_SHIFT) + 1;

            uint32_t currentDivider = secondDivider * previousDivider;

            if (currentDivider > 16)
            {
                currentDivider = 16;
            }

            uint32_t tmp = QuadSPI0->MCR;
            tmp &= (uint32_t)~QuadSPI_MCR_SCLKCFG_MASK;
            tmp |= QuadSPI_MCR_SCLKCFG(currentDivider - 1);
            QuadSPI0->MCR = tmp;
        }
    }
    else if (option == kIpsCommandClock_Restore)
    {
        quadspi_serial_clock_configure((qspi_serial_clock_freq_t)qspiflash_config_param.sclk_freq);
    }

    // Enable QuadSPI module.
    QuadSPI0->MCR &= (uint32_t)~QuadSPI_MCR_MDIS_MASK;
}

void quadspi_clear_sequence_pointer(void)
{
    // Reset sequence points.
    QuadSPI0->SPTRCLR |= (QuadSPI_SPTRCLR_IPPTRC_MASK | QuadSPI_SPTRCLR_BFPTRC_MASK);
    // Wait until this operation completes.
    while (QuadSPI0->SPTRCLR & (QuadSPI_SPTRCLR_IPPTRC_MASK | QuadSPI_SPTRCLR_BFPTRC_MASK))
    {
    }
}

void quadspi_send_dummy_command(void)
{
    quadspi_clear_sequence_pointer();
    QuadSPI0->IPCR = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_Dummy);
    quadspi_cmd_wait_for_finish();
}

void quadspi_reset_ahb_buffer(void)
{
    /*
    * Follow K8x/KL8x QSPI BG:
    * The MCR[MDIS] should therefore be set to 0 when the software reset bits are asserted.
    * Also, before they can be de-asserted again (by setting MCR[SWRSTHD] to 0), it is recommended
    * to set the MCR[MDIS] bit to 1. Once the software resets have been de-asserted, the normal
    * operation can be started by setting the MCR[MDIS] bit to 0.
    */
    QuadSPI0->MCR &= (uint32_t)~QuadSPI_MCR_MDIS_MASK;
    QuadSPI0->MCR |= QuadSPI_MCR_SWRSTHD_MASK | QuadSPI_MCR_SWRSTSD_MASK;
    // Wait several ticks until the AHB domain and serial flash domain are reset
    microseconds_delay(10);
    // Disable module during the reset procedure
    QuadSPI0->MCR |= QuadSPI_MCR_MDIS_MASK;
    // Clear the reset bits.
    QuadSPI0->MCR &= (uint32_t) ~(QuadSPI_MCR_SWRSTHD_MASK | QuadSPI_MCR_SWRSTSD_MASK);
    // Re-enable QSPI module
    QuadSPI0->MCR &= (uint32_t)~QuadSPI_MCR_MDIS_MASK;
}

// See qspi.h for detailed information for this function
status_t quadspi_get_property(uint32_t whichProperty, uint32_t *value)
{
    assert(value);

    switch (whichProperty)
    {
        case kQspiFlashProperty_InitStatus:
            *value = is_quadspi_configured() ? kStatus_Success : kStatus_QspiNotConfigured;
            break;

        case kQspiFlaghProperty_StartAddress:
            *value = FSL_FEATURE_QSPI_AMBA_BASE;
            break;

        case kQspiFlashProperty_TotalFlashSizeInKBytes:
        {
            *value = s_totalFlashSize / 1024;
        }
        break;

        case kQspiFlashProperty_PageSize:
            *value = qspiflash_config_param.pagesize;
            break;

        case kQspiFlashProperty_SectorSize:
            *value = qspiflash_config_param.sectorsize;
            break;

        case kQspiFlashProperty_TotalFlashSize:
        {
            *value = s_totalFlashSize;
        }
        break;

        default: // catch inputs that are not recognized
            return kStatus_InvalidArgument;
    }

    return kStatus_Success;
}

status_t quadspi_wait_while_flash_busy(void)
{
    bool doTimeoutCheck = qspiflash_config_param.timeout_milliseconds ? true : false;

    uint64_t startTicks = 0;
    volatile uint64_t timeoutTicks;
    uint64_t ticksPerMillisecond = microseconds_convert_to_ticks(1000);

    if (doTimeoutCheck)
    {
        startTicks = microseconds_get_ticks();
        timeoutTicks = (uint64_t)qspiflash_config_param.timeout_milliseconds * ticksPerMillisecond;
    }

    bool isBusy = false;
    uint32_t busyMask = 1 << (qspiflash_config_param.busy_bit_offset & 0xFFFF);
    volatile uint32_t status_value = 0;
    do
    {
        if (doTimeoutCheck)
        {
            uint64_t currentTicks = microseconds_get_ticks();
            if (currentTicks - startTicks > timeoutTicks)
            {
                return kStatus_QspiCommandTimeout;
            }
        }

        quadspi_clear_fifo();
        quadspi_clear_sequence_pointer();

#if BL_FEATURE_QSPI_HYPERFLASH
        // For HyperFLASH, the ReadStatus command requires two stages,
        // this is the 1st stage
        uint32_t previousAddress = QuadSPI0->SFAR;
        if (qspiflash_config_param.need_multi_phases)
        {
            if (qspiflash_config_param.pre_read_status_cmd_address_offset != kInvalidArgument)
            {
                uint32_t address = quadspi_get_base_address(previousAddress) +
                                   qspiflash_config_param.pre_read_status_cmd_address_offset;
                QuadSPI0->SFAR = address;
            }
            QuadSPI0->IPCR = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_PreReadStatus);
            status_t status = quadspi_cmd_wait_for_finish();
            if (status != kStatus_Success)
            {
                return status;
            }

            // Restore Address
            QuadSPI0->SFAR = previousAddress;
        }
#endif // BL_FEATURE_QSPI_HYPERFLASH

        QuadSPI0->IPCR = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_ReadStatus);
        status_t status = quadspi_cmd_wait_for_finish();
        if (status != kStatus_Success)
        {
            return status;
        }

        // Wait until data is ready in RX buffer
        startTicks = microseconds_get_ticks();
        timeoutTicks = (uint64_t)ticksPerMillisecond * kQuadSPIModuleBusyMaxTimeoutMs;
        do
        {
            uint64_t currentTicks = microseconds_get_ticks();
            if (currentTicks - startTicks > timeoutTicks)
            {
                return kStatus_QspiCommandTimeout;
            }
        } while (!(QuadSPI0->RBSR & QuadSPI_RBSR_RDBFL_MASK));

        status_value = QuadSPI0->RBDR[0];

        QuadSPI0->FR |= QuadSPI_FR_RBDF_MASK; /* read complete */

        // Determine if the busy_offset represents the offset of ready flag
        if (qspiflash_config_param.busy_bit_offset & (1 << kFlashReadFlagShift))
        {
            isBusy = (~status_value) & busyMask;
        }
        else
        {
            isBusy = status_value & busyMask;
        }

    } while (isBusy);

    return kStatus_Success;
}

status_t quadspi_common_parallel_mode(uint32_t address, qspi_callback_t callback)
{
    uint32_t offset = address - FSL_FEATURE_QSPI_AMBA_BASE;

    uint32_t baseAddr[2];
    status_t status;

    if (offset < (qspiflash_config_param.sflash_A1_size * 2))
    {
        baseAddr[0] = FSL_FEATURE_QSPI_AMBA_BASE;
        baseAddr[1] = QuadSPI0->SFA2AD;
    }
    else
    {
        baseAddr[0] = QuadSPI0->SFA1AD;
        baseAddr[1] = QuadSPI0->SFB1AD;
    }

    uint32_t maxItemCnt = sizeof(baseAddr) / sizeof(uint32_t);
    for (uint32_t i = 0; i < maxItemCnt; i++)
    {
        QuadSPI0->SFAR = baseAddr[i];
        if (callback)
        {
            status = callback();
            if (status != kStatus_Success)
            {
                return status;
            }
        }
    }

    return kStatus_Success;
}

// Wait until all external Serial NOR flash devices are idle after having issued parallel programming command
status_t quadspi_wait_while_flash_busy_parallel_mode(uint32_t address)
{
    return quadspi_common_parallel_mode(address, quadspi_wait_while_flash_busy);
}

status_t quadspi_write_enable_parallel_mode(uint32_t address)
{
    return quadspi_common_parallel_mode(address, quadspi_write_enable);
}

// See qspi.h for documentation on this function
#if defined(__CC_ARM)
#pragma push
#pragma Otime
#elif defined(__ICCARM__)
#pragma optimize = speed
#elif defined(__GNUC__)
__attribute__((optimize("Ofast")))
#endif
status_t quadspi_page_program(uint32_t dst_addr, const uint32_t *src, uint32_t size)
{
    uint32_t pageSize = qspiflash_config_param.pagesize;
    uint32_t ipcrValue = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_PageProgram) | pageSize;
    uint32_t endAddress = dst_addr + size;
    uint32_t currentAddress = dst_addr;
    status_t status = kStatus_Success;

    // Enable parallel mode bit if needed
    if (qspiflash_config_param.parallel_mode_enable)
    {
        ipcrValue |= QuadSPI_IPCR_PAR_EN_MASK;
    }

    while (currentAddress < endAddress)
    {
        // Reduce serial clock frequency if needed.
        quadspi_configure_ips_command_divider(kIpsCommandClock_Config);

        QuadSPI0->SFAR = currentAddress;

        // Write Enable
        if (!qspiflash_config_param.parallel_mode_enable)
        {
            quadspi_write_enable();
        }
        else
        {
            quadspi_write_enable_parallel_mode(currentAddress);
            // Restore address
            QuadSPI0->SFAR = currentAddress;
        }

#if BL_FEATURE_QSPI_HYPERFLASH
        // For HyperFLASH, Two phases are required for Page program.
        if (qspiflash_config_param.need_multi_phases)
        {
            quadspi_clear_sequence_pointer();

            // First stage
            if (qspiflash_config_param.pre_program_cmd_address_offset != 0xFFFFFFFF)
            {
                uint32_t address =
                    quadspi_get_base_address(currentAddress) + qspiflash_config_param.pre_program_cmd_address_offset;
                QuadSPI0->SFAR = address;
            }
            QuadSPI0->IPCR = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_PrePageProgram);
            quadspi_cmd_wait_for_finish();

            // Second stage.
            QuadSPI0->SFAR = currentAddress;
        }
#endif // BL_FEATURE_QSPI_HYPERFLASH

        quadspi_clear_sequence_pointer();
        quadspi_clear_fifo();
        QuadSPI0->FR |= QuadSPI_FR_TBFF_MASK;

        // Ensure that the program operation cannots be interrupted.
        uint32_t regPrimask = 0U;
        regPrimask = __get_PRIMASK();
        __disable_irq();

        // fill tx fifo (QSPI_TXFIFO_DEPTH * 4 bytes)
        for (uint32_t i = FSL_FEATURE_QSPI_TXFIFO_DEPTH; i > 0; i--)
        {
            QuadSPI0->TBDR = *src++;
        }

        // Configure the Watermark to 4 longwords
        QuadSPI0->TBCT = QuadSPI_TBCT_WMRK(4 - 1);

        // Calculate left bytes to be programmed.
        int32_t leftLongWords = (pageSize - FSL_FEATURE_QSPI_TXFIFO_DEPTH * sizeof(uint32_t)) / sizeof(uint32_t);

        QuadSPI0->IPCR = ipcrValue; // page program

        // Note: leftLongWords is always 16 byte aligned according to the page size feature for Serial NOR FLASH.
        while (leftLongWords > 0)
        {
            if (QuadSPI0->SR & QuadSPI_SR_TXWA_MASK)
            {
                QuadSPI0->TBDR = *src++;
                QuadSPI0->TBDR = *src++;
                QuadSPI0->TBDR = *src++;
                QuadSPI0->TBDR = *src++;

                leftLongWords -= 4;
            }
            else if (QuadSPI0->FR & QuadSPI_FR_TBUF_MASK)
            {
                status = kStatus_QspiWriteFailure;
                break;
            }
        }

        // Release lock after the write operation completes.
        __set_PRIMASK(regPrimask);

        // Wait until QSPI module completes the command transfer.
        quadspi_cmd_wait_for_finish();

        // Restore serial clock frequency if needed.
        quadspi_configure_ips_command_divider(kIpsCommandClock_Restore);

        // There is also one case that the TX buffer is not full but TX buffer is underflow,
        // QSPI driver needs to handle this case.
        if (QuadSPI0->FR & QuadSPI_FR_TBUF_MASK)
        {
            status = kStatus_QspiWriteFailure;
            break;
        }

        // Wait until all external spi flash devices are idle
        if (!qspiflash_config_param.parallel_mode_enable)
        {
            status = quadspi_wait_while_flash_busy();
        }
        else
        {
            status = quadspi_wait_while_flash_busy_parallel_mode(currentAddress);
        }

        if (status != kStatus_Success)
        {
            break;
        }

        currentAddress += pageSize;
    }

    quadspi_reset_ahb_buffer();

    return status;
}
#if (defined(__CC_ARM))
#pragma pop
#endif

static status_t quadspi_erase_all_common(qspi_erase_all_callback_t callback)
{
    status_t status = kStatus_Success;
    uint32_t serialFlashBase = FSL_FEATURE_QSPI_AMBA_BASE;

    for (uint32_t flashIndex = 0; flashIndex < 4; flashIndex++)
    {
        uint32_t flashSize = 0;
        switch (flashIndex)
        {
            case 0:
                flashSize = qspiflash_config_param.sflash_A1_size;
                break;
            case 1:
                flashSize = qspiflash_config_param.sflash_A2_size;
                break;
            case 2:
                flashSize = qspiflash_config_param.sflash_B1_size;
                break;
            case 3:
                flashSize = qspiflash_config_param.sflash_B2_size;
                break;
        }
        if (flashSize)
        {
            QuadSPI0->SFAR = serialFlashBase;

            status = callback(serialFlashBase);
            if (status != kStatus_Success)
            {
                break;
            }
        }

        serialFlashBase += flashSize;
    }

    return status;
}

static status_t quadspi_send_erase_all_command(uint32_t address)
{
    quadspi_configure_ips_command_divider(kIpsCommandClock_Config);

    // write enable
    quadspi_write_enable();

#if BL_FEATURE_QSPI_HYPERFLASH
    uint32_t previousAddress = address;
    // For HyperFLASH, 4 stages are required for ChipErase.
    // Phase 1 is Pre-Erase
    // Combination phase 2 and phase 3 as the 2nd WriteEnable
    if (qspiflash_config_param.need_multi_phases)
    {
        if (qspiflash_config_param.pre_erase_cmd_address_offset != kInvalidArgument)
        {
            uint32_t writeAddress =
                quadspi_get_base_address(previousAddress) + qspiflash_config_param.pre_erase_cmd_address_offset;
            QuadSPI0->SFAR = writeAddress;
        }

        quadspi_clear_sequence_pointer();
        QuadSPI0->IPCR = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_PreErase);
        quadspi_cmd_wait_for_finish();
        QuadSPI0->SFAR = previousAddress;

        if (qspiflash_config_param.is_spansion_hyperflash)
        {
            // Combination of Phase 2 and Phase3 : 2nd WriteEnable
            quadspi_write_enable();
        }

        // Phase 4. Write Special Address for Chip Erase.
        if (qspiflash_config_param.erase_all_cmd_address_offset != kInvalidArgument)
        {
            uint32_t writeAddress =
                quadspi_get_base_address(previousAddress) + qspiflash_config_param.erase_all_cmd_address_offset;
            QuadSPI0->SFAR = writeAddress;
        }
    }
#endif // BL_FEATURE_QSPI_HYPERFLASH

    // send erase command
    quadspi_clear_sequence_pointer();
    QuadSPI0->IPCR = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_EraseAll);
    quadspi_cmd_wait_for_finish();

    quadspi_configure_ips_command_divider(kIpsCommandClock_Restore);

    return kStatus_Success;
}

static status_t quadspi_wait_erase_all_complete(uint32_t address)
{
    return quadspi_wait_while_flash_busy();
}

// See qspi.h for documentation on this function
status_t quadspi_erase_all(void)
{
    quadspi_erase_all_common(quadspi_send_erase_all_command);
    status_t status = quadspi_erase_all_common(quadspi_wait_erase_all_complete);

    quadspi_reset_ahb_buffer();

    return status;
}

// Erase one sector under serial mode (non-parallel mode)
status_t quadspi_erase_sector_under_serial_mode(uint32_t address)
{
    quadspi_configure_ips_command_divider(kIpsCommandClock_Config);

    QuadSPI0->SFAR = address;

    // Write enable
    quadspi_write_enable();

#if BL_FEATURE_QSPI_HYPERFLASH
    uint32_t previousAddress = address;
    // For  HyperFLASh, 4 stages are required for SectorErase.
    // Phase 1 is PreErase
    // Combination phase 2 and phase 3 as the 2nd WriteEnable
    if (qspiflash_config_param.need_multi_phases)
    {
        if (qspiflash_config_param.pre_erase_cmd_address_offset != kInvalidArgument)
        {
            uint32_t writeAddress =
                quadspi_get_base_address(previousAddress) + qspiflash_config_param.pre_erase_cmd_address_offset;
            QuadSPI0->SFAR = writeAddress;
        }
        quadspi_clear_sequence_pointer();
        QuadSPI0->IPCR = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_PreErase);
        quadspi_cmd_wait_for_finish();
        QuadSPI0->SFAR = previousAddress;

        if (qspiflash_config_param.is_spansion_hyperflash)
        {
            // Combination of Phase 2 and Phase3 : 2nd WriteEnable
            quadspi_write_enable();
        }
    }
#endif // BL_FEATURE_QSPI_HYPERFLASH

    // Send erase sector command
    quadspi_clear_sequence_pointer();
    QuadSPI0->IPCR = QuadSPI_IPCR_SEQID(kQspiCmd_SeqId_SectorErase);
    quadspi_cmd_wait_for_finish();
    quadspi_configure_ips_command_divider(kIpsCommandClock_Restore);

    status_t status = quadspi_wait_while_flash_busy();

    quadspi_reset_ahb_buffer();

    return status;
}

// Check if the parameters for parallel mode is valid
bool is_quadspi_parallel_mode_parameter_valid(void)
{
    if ((qspiflash_config_param.sflash_A1_size != qspiflash_config_param.sflash_B1_size) ||
        (qspiflash_config_param.sflash_A2_size != qspiflash_config_param.sflash_B2_size))
    {
        return false;
    }

    return true;
}

// See qspi.h for documentation on this function
status_t quadspi_erase_sector(uint32_t address)
{
    if (!qspiflash_config_param.parallel_mode_enable)
    {
        return quadspi_erase_sector_under_serial_mode(address);
    }

    // Get the offset of erase location.
    uint32_t offset = address - FSL_FEATURE_QSPI_AMBA_BASE;

    // If the offset is less than 2 * flash A1 size
    if (offset < (qspiflash_config_param.sflash_A1_size * 2))
    {
        // Erase actual sector of Flash A1 under non-parallel mode
        status_t status = quadspi_erase_sector_under_serial_mode(FSL_FEATURE_QSPI_AMBA_BASE + offset / 2);
        if (status != kStatus_Success)
        {
            return status;
        }

        // Erase actual sector of Flash B1 under non-parallel mode
        status = quadspi_erase_sector_under_serial_mode(QuadSPI0->SFA2AD + offset / 2);
        return status;
    }
    else
    {
        // Erase actual sector of Flash A2 under non-parallel mode
        status_t status = quadspi_erase_sector_under_serial_mode(FSL_FEATURE_QSPI_AMBA_BASE + offset / 2);
        if (status != kStatus_Success)
        {
            return status;
        }

        // Erase actual sector of Flash B2 (calculate actual offset on Flash B2)
        status = quadspi_erase_sector_under_serial_mode(QuadSPI0->SFB1AD + offset / 2 -
                                                        qspiflash_config_param.sflash_A1_size);
        return status;
    }
}

void config_ahb_buffer(void)
{
    /* Configure BUFFER 3 for All Master Access, Configure AHB data transfer size to 64bytes */
    QuadSPI0->BUF3CR = QuadSPI_BUF3CR_ALLMST_MASK | QuadSPI_BUF3CR_ADATSZ(kQspiAHBDataTransferSize_64Bytes);

    /* If CPU masterID is zero then qspi data is fetched from AHB buffer 0.
    Where as we expect the data to be on BUF 3. Hence we program the Master ID
    for Buffer control register 0,1,2 with a dummy masterIDs
    */
    QuadSPI0->BUF0CR = AHB_BFCR_DUMMY_MASTER_ID;
    QuadSPI0->BUF1CR = AHB_BFCR_DUMMY_MASTER_ID;
    QuadSPI0->BUF2CR = AHB_BFCR_DUMMY_MASTER_ID;
}

void quadspi_clock_gate_enable(void)
{
#if defined(SIM_SCGC2_QSPI_MASK)
    SIM->SCGC2 |= SIM_SCGC2_QSPI_MASK;
#elif defined(SIM_SCGC5_QSPI0_MASK)
    SIM->SCGC5 |= SIM_SCGC5_QSPI0_MASK;
#else
#error "Unknown QSPI clock gate"
#endif
}

void quadspi_clock_gate_disable()
{
#if defined(SIM_SCGC2_QSPI_MASK)
    SIM->SCGC2 &= (uint32_t)~SIM_SCGC2_QSPI_MASK;
#elif defined(SIM_SCGC5_QSPI0_MASK)
    SIM->SCGC5 &= (uint32_t)~SIM_SCGC5_QSPI0_MASK;
#else
#error "Unknown QSPI clock gate"
#endif
}

void quadspi_default_serial_clock_configure(void)
{
    /* choose the FLL as the clock source for QuadSPI */
    QuadSPI0->SOCCR = QuadSPI_SOCCR_QSPISRC(1);

    /* Set default serial clock divider to 2 */
    uint32_t tmp = QuadSPI0->MCR;
    tmp &= (uint32_t)~QuadSPI_MCR_SCLKCFG_MASK;
    tmp |= QuadSPI_MCR_SCLKCFG(0x01);
    QuadSPI0->MCR = tmp;
}

// Get QuadSPI configuration data, only validate if the tag is valid.
status_t get_quadspi_config_data(qspi_config_t *config_base)
{
    if (config_base == NULL)
    {
        // Set GPIO for basic Read
        quadspi_iomux_configuration(NULL);
    }

    // Do minimum configuration to run basic read command 0x03
    /* Enable QSPI clock gate */
    quadspi_clock_gate_enable();

    /* Disable QuadSPI module */
    QuadSPI0->MCR |= QuadSPI_MCR_MDIS_MASK;

    // Configure default serial clock including source and divider
    quadspi_default_serial_clock_configure();

    /* initialization for AHB Buffer Access */
    config_ahb_buffer();

    // Set Endianess to 64bit LE
    QuadSPI0->MCR |= QuadSPI_MCR_END_CFG_MASK;

    /* Enable QuadSPI module */
    QuadSPI0->MCR &= (uint32_t)~QuadSPI_MCR_MDIS_MASK;

    // If config_base is equal to NULL, which means config block should be loaded from external flssh address 0
    if (config_base == NULL)
    {
        config_base = (qspi_config_t *)FSL_FEATURE_QSPI_AMBA_BASE;

        // Assuming external Serial NOR flash is still in continuous read mode while target is reset,
        // This dummy read is used to force Serial NOR flash to exit continuous read mode
        volatile uint32_t dummy = *(uint32_t *)FSL_FEATURE_QSPI_AMBA_BASE;
        (void)dummy;
        quadspi_reset_ahb_buffer();

        // Use 3-byte read first
        // It means QSPI config block is invalid
        if (config_base->tag != kQspiConfigTag)
        {
            // try to use 4-byte read if 3-byte read fails
            const uint32_t k_32bit_dedicated_read_lut[2] = { 0x08200413, 0x1c04 };
            quadspi_unlockLookupTable();
            QuadSPI0->LUT[0] = k_32bit_dedicated_read_lut[0];
            QuadSPI0->LUT[1] = k_32bit_dedicated_read_lut[1];
            quadspi_lockLookupTable();
        }

        quadspi_reset_ahb_buffer();

        if (config_base->tag != kQspiConfigTag)
        {
            return kStatus_InvalidArgument;
        }
    }

    // Copy qspi config block from given address to internal configure block maintained by driver
    memcpy((uint32_t *)&qspiflash_config_param, config_base, sizeof(qspiflash_config_param));

    return kStatus_Success;
}

//! @brief Configure QSPI controller to expected mode
status_t configure_quad_spi_controller(qspi_config_t *config)
{
    // Re-configure BUF3CR Register
    QuadSPI0->BUF3CR = QuadSPI_BUF3CR_ALLMST_MASK | QuadSPI_BUF3CR_ADATSZ(kQspiAHBDataTransferSize_512Bytes);

    // Disable this feature by default, enable them as needed later.
    QuadSPI0->MCR &= (uint32_t) ~(QuadSPI_MCR_DDR_EN_MASK | QuadSPI_MCR_DQS_LAT_EN_MASK | QuadSPI_MCR_DQS_EN_MASK);

    /* Enable DDR mode*/
    if (config->ddr_mode_enable)
    {
        QuadSPI0->MCR |= QuadSPI_MCR_DDR_EN_MASK;
    }

    // FLL as clock source
    uint32_t soccr = QuadSPI_SOCCR_QSPISRC(1);

    // Enable DQS
    if (config->dqs_enable)
    {
        QuadSPI0->MCR |= QuadSPI_MCR_DQS_EN_MASK;

        // Determine if DQS_LAT_EN should be asserted.
        if (config->dqs_latency_enable)
        {
            QuadSPI0->MCR |= QuadSPI_MCR_DQS_LAT_EN_MASK;
        }

        // Enable DQSINVESEL
        if (qspiflash_config_param.dqs_inverse_sel)
        {
            soccr |= QuadSPI_SOCCR_DQSINVSEL_MASK;
        }

        // Select DQS phase
        soccr |= QuadSPI_SOCCR_DQSPHASEL(config->dqs_phase_sel);

        // FA DQS delay chain selection
        soccr |= QuadSPI_SOCCR_DLYTAPSELA(config->dqs_fa_delay_chain_sel);

        // FB DQS delay chain selection
        soccr |= QuadSPI_SOCCR_DLYTAPSELB(config->dqs_fb_delay_chain_sel);

        /* Set DQS LoopBack to enable dummy pad if enabled from Configuration */
        if (config->dqs_loopback)
        {
            if (!config->dqs_loopback_internal)
            {
                soccr |= QuadSPI_SOCCR_DQSPADLPEN_MASK;
            }
            else
            {
                soccr |= QuadSPI_SOCCR_DQSLPEN_MASK;
            }
        }
    }

    // Enable octal mode
    if (qspiflash_config_param.sflash_type == kQspiFlashPad_Octal)
    {
        soccr |= QuadSPI_SOCCR_OCTEN_MASK;
    }

    // Enable Differential flash clock pin
    if (qspiflash_config_param.differential_clock_pin_enable)
    {
        soccr |= QuadSPI_SOCCR_DIFFCKEN_MASK;
    }

    // Enable Flash CK2 clock pin
    if (qspiflash_config_param.flash_CK2_clock_pin_enable)
    {
        soccr |= QuadSPI_SOCCR_CK2EN_MASK;
    }

    QuadSPI0->SOCCR = soccr;

    /* Program Sampling Parameters */
    // Note: QuadSPI_SMPR[HSDLY] and QuadSPI_SMPR[HFPHS] are ignored.
    QuadSPI0->SMPR = QuadSPI_SMPR_FSDLY(qspiflash_config_param.fsdly) |
                     QuadSPI_SMPR_FSPHS(qspiflash_config_param.fsphs) |
                     QuadSPI_SMPR_DDRSMP(qspiflash_config_param.ddrsmp);

    // Enable parallel flash access, only enable this bit for AHB command
    QuadSPI0->BFGENCR = QuadSPI_BFGENCR_PAR_EN(qspiflash_config_param.parallel_mode_enable) |
                        QuadSPI_BFGENCR_SEQID(kQspiCmd_SeqId_Read);

    // Enable Word Addressable bit
    // Set Column Address Space, don't check validity of this parameter, users should guarantee
    // that this parameter is valid.
    QuadSPI0->SFACR = QuadSPI_SFACR_CAS(qspiflash_config_param.column_address_space) |
                      QuadSPI_SFACR_WA(qspiflash_config_param.word_addressable);

    /* Setup SFLASH hold and setup time, Data hold time */
    QuadSPI0->FLSHCR = QuadSPI_FLSHCR_TCSH(qspiflash_config_param.cs_hold_time) |
                       QuadSPI_FLSHCR_TCSS(qspiflash_config_param.cs_setup_time) |
                       QuadSPI_FLSHCR_TDH(qspiflash_config_param.data_hold_time);

    /* Configure and Enable SCLK*/
    quadspi_serial_clock_configure((qspi_serial_clock_freq_t)qspiflash_config_param.sclk_freq);

    return kStatus_Success;
}

void configure_lut(void)
{
    uint32_t temp;
    uint32_t cnt;
    uint32_t look_up_table_seq = 0;

    quadspi_unlockLookupTable();

    // Copy LUT from parameter table to QSPI_LUT table
    for (cnt = 0; cnt < FSL_FEATURE_QSPI_LUT_DEPTH; cnt++)
    {
        temp = qspiflash_config_param.look_up_table[cnt];
        QuadSPI0->LUT[look_up_table_seq++] = temp;
    }

    quadspi_lockLookupTable();
}

status_t quadspi_perform_config_cmds(qspi_config_t *config)
{
    status_t status = kStatus_Success;

    if (config->config_cmds[0])
    {
        for (uint32_t i = 0; i < QSPI_PRE_CMD_CNT; i++)
        {
            if (config->config_cmds[i])
            {
                // Do write enable sequence
                quadspi_write_enable();

                quadspi_clear_sequence_pointer();
                quadspi_clear_fifo();

                // Command to tx transferred to device
                QuadSPI0->TBDR = config->config_cmds_args[i];
                QuadSPI0->TBDR = 0;
                QuadSPI0->TBDR = 0;
                QuadSPI0->TBDR = 0;

                // Do write register sequence
                QuadSPI0->IPCR = config->config_cmds[i];

                // Wait for IP command to finish
                quadspi_cmd_wait_for_finish();

                // Check status register for write in progress
                status = quadspi_wait_while_flash_busy();
                if (status != kStatus_Success)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    return status;
}

status_t quadspi_cmd_config_all_banks(qspi_config_t *config)
{
    status_t status = kStatus_Success;
    uint32_t baseAddress = FSL_FEATURE_QSPI_AMBA_BASE;

    uint32_t *flashDevicesSize = &config->sflash_A1_size;

    for (uint32_t index = 0; index < 4; index++)
    {
        uint32_t currentFlashSize = *flashDevicesSize++;
        if (currentFlashSize)
        {
            QuadSPI0->SFAR = baseAddress;
            baseAddress += currentFlashSize;
            status = quadspi_perform_config_cmds(config);
            if (status != kStatus_Success)
            {
                return status;
            }
        }
    }

    return kStatus_Success;
}

void quadspi_cmd_write_register(qspi_config_t *config)
{
    quadspi_clear_sequence_pointer();
    quadspi_clear_fifo();

    // Command to tx transferred to device
    QuadSPI0->TBDR = config->device_cmd;
    QuadSPI0->TBDR = 0;
    QuadSPI0->TBDR = 0;
    QuadSPI0->TBDR = 0;

    // Do write register sequence
    QuadSPI0->IPCR = config->write_cmd_ipcr;

    // Wait for IP command to finish
    quadspi_cmd_wait_for_finish();
}

void quadspi_cmd_config_workmode(qspi_config_t *config)
{
    // Do write enable sequence
    quadspi_write_enable();

    // Enable Quad operations
    quadspi_cmd_write_register(config);

    // Check status register for Write In Progress
    quadspi_wait_while_flash_busy();
}

void quadspi_cmd_config_workmode_all_banks(qspi_config_t *config)
{
    uint32_t baseAddress = FSL_FEATURE_QSPI_AMBA_BASE;

    uint32_t *flashDevicesSize = &config->sflash_A1_size;

    for (uint32_t index = 0; index < 4; index++)
    {
        uint32_t currentFlashSize = *flashDevicesSize++;
        if (currentFlashSize)
        {
            QuadSPI0->SFAR = baseAddress;
            baseAddress += currentFlashSize;
            quadspi_cmd_config_workmode(config);
        }
    }
}

void quadspi_pre_configure(qspi_config_t *config)
{
    // Configure Serial Flash Top Address
    uint32_t topAddress = FSL_FEATURE_QSPI_AMBA_BASE + qspiflash_config_param.sflash_A1_size;
    QuadSPI0->SFA1AD = topAddress;
    topAddress += qspiflash_config_param.sflash_A2_size;
    QuadSPI0->SFA2AD = topAddress;
    topAddress += qspiflash_config_param.sflash_B1_size;
    QuadSPI0->SFB1AD = topAddress;
    topAddress += qspiflash_config_param.sflash_B2_size;
    QuadSPI0->SFB2AD = topAddress;

    s_totalFlashSize = topAddress - FSL_FEATURE_QSPI_AMBA_BASE;

    // Assuming external Serial NOR flash is still in continuous read mode while target is reset,
    // This dummy read is used to force Serial NOR flash to exit continuous read mode
    uint32_t address = FSL_FEATURE_QSPI_AMBA_BASE;
    volatile uint32_t dummy = 0;

    uint32_t *flashDevicesSize = &config->sflash_A1_size;
    for (uint32_t i = 0; i < 4; i++)
    {
        uint32_t currentFlashSize = *flashDevicesSize++;
        if (currentFlashSize)
        {
            dummy = *(uint32_t *)address;
            address += currentFlashSize;
        }
    }
    (void)dummy; // This code is used to suppress warnings of compiler.

    quadspi_reset_ahb_buffer();
}

status_t configure_quadspi(qspi_config_t *config)
{
    status_t status = kStatus_Success;

    // Wait until qspi bus is idle.
    quadspi_cmd_wait_for_finish();

    // Enable QSPI clock gate
    quadspi_clock_gate_enable();

    /* QUADSPI IOMUX config */
    status = quadspi_iomux_configuration(config);
    if (status != kStatus_Success)
    {
        // Disable QSPI clock gate in order to save energy.
        quadspi_clock_gate_disable();
        return status;
    }

    /* Force the Serial Flash devices to enter normal mode before updating LUT  */
    quadspi_pre_configure(config);

    // Do parameter check here.
    if ((!s_totalFlashSize) ||
        (qspiflash_config_param.parallel_mode_enable && (!is_quadspi_parallel_mode_parameter_valid())))
    {
        return kStatus_InvalidArgument;
    }

    /*Program the look up table with flash sequence command*/
    configure_lut();

    /* Disable QuadSPI module logic before changing registers */
    QuadSPI0->MCR |= QuadSPI_MCR_MDIS_MASK;

    // Configure QSPI controller
    status = configure_quad_spi_controller(config);
    if (status != kStatus_Success)
    {
        // Disable QSPI clock gate in order to save energy.
        quadspi_clock_gate_disable();
        return status;
    }

    // Enable QSPI module
    QuadSPI0->MCR &= (uint32_t)~QuadSPI_MCR_MDIS_MASK;

    // Always get IP read data from RX Buffer within QuadSPI module
    QuadSPI0->RBCT |= QuadSPI_RBCT_RXBRD_MASK;

    // Send config commands to device to configure device by write device registers
    if (config->config_cmd_en)
    {
        status_t status = quadspi_cmd_config_all_banks(config);
        if (status != kStatus_Success)
        {
            // Disable QSPI clock gate in order to save energy.
            quadspi_clock_gate_disable();
            return status;
        }
    }

    // Send Device Mode enable command to device
    if (config->device_mode_config_en)
    {
        quadspi_cmd_config_workmode_all_banks(config);
    }

    return kStatus_Success;
}

// See qspi.h for detailed document on this function
status_t quadspi_init(qspi_config_t *config_base)
{
    // Ensure the start address of qspi config block is 4-byte aligned.
    if (config_base && ((uint32_t)config_base & 0x03))
    {
        return kStatus_InvalidArgument;
    }

    // First, Clear the qspi configuration state.
    s_qspiConfigured = false;

    /* Get the configuration parameter  */
    status_t status = get_quadspi_config_data(config_base);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = configure_quadspi(&qspiflash_config_param);

    if (status == kStatus_Success)
    {
        s_qspiConfigured = true;
    }

    return status;
}

// See qspi.h for documentation on this function.
bool is_quadspi_configured(void)
{
    return s_qspiConfigured;
}

// See qspi.h for documentation on this function.
status_t quadspi_cache_clear(void)
{
    if (is_quadspi_configured())
    {
        quadspi_reset_ahb_buffer();
    }

    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
