/*
* Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*
*/

#include "blfwk/Command.h"
#include "blfwk/Logging.h"
#include "bootloader_common.h"
#include "blfwk/json.h"
#include "blfwk/utils.h"
#include "blfwk/EndianUtilities.h"
#ifdef LINUX
#include <string.h>
#endif

using namespace blfwk;
using namespace utils;
using namespace std;

//#define TEST_SENDER_ABORT
//#define TEST_RECEIVER_ABORT

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Value of the terminator code in the g_statusCodes table.
const int32_t kStatusMessageTableTerminatorValue = 0x7fffffff;

//! @brief Status return code descriptions.
StatusMessageTableEntry blfwk::g_statusCodes[] = {
    // Generic statuses.
    { kStatus_Success, "Success." },
    { kStatus_Fail, "Failure." },
    { kStatus_ReadOnly, "kStatus_ReadOnly" },
    { kStatus_OutOfRange, "kStatus_OutOfRange" },
    { kStatus_InvalidArgument, "kStatus_InvalidArgument" },
    { kStatus_Timeout, "kStatus_Timeout" },
    { kStatus_NoTransferInProgress, "kStatus_NoTransferInProgress" },

    // Flash driver errors.
    { 100 /*kStatus_FlashSizeError*/, "kStatus_FlashSizeError" },
    { 101 /*kStatus_FlashAlignmentError*/, "kStatus_FlashAlignmentError" },
    { 102 /*kStatus_FlashAddressError*/, "kStatus_FlashAddressError" },
    { 103 /*kStatus_FlashAccessError*/, "kStatus_FlashAccessError" },
    { 104 /*kStatus_FlashProtectionViolation*/, "kStatus_FlashProtectionViolation" },
    { 105 /*kStatus_FlashCommandFailure*/, "kStatus_FlashCommandFailure" },
    { 106 /*kStatus_FlashUnknownProperty*/, "kStatus_FlashUnknownProperty" },
    { 108 /*kStatus_FlashRegionExecuteOnly*/, "kStatus_FlashRegionExecuteOnly" },
    { 109 /*kStatus_FlashExecuteInRamFunctionNotReady*/, "kStatus_FlashExecuteInRamFunctionNotReady" },
    { 111 /*kStatus_FLASH_CommandNotSupported*/, "kStatus_FLASH_CommandNotSupported" },
    { 132 /*kStatus_FLASH_OutOfDateCfpaPage*/, "kStatus_FLASH_OutOfDateCfpaPage" },
    { 133 /*kStatus_FLASH_BlankIfrPageData*/, "kStatus_FLASH_BlankIfrPageData" },
    { 134 /*kStatus_FLASH_EncryptedRegionsEraseNotDoneAtOnce*/, "kStatus_FLASH_EncryptedRegionsEraseNotDoneAtOnce" },
    { 135 /*kStatus_FLASH_ProgramVerificationNotAllowed*/, "kStatus_FLASH_ProgramVerificationNotAllowed" },
    { 136 /*kStatus_FLASH_HashCheckError*/, "kStatus_FLASH_HashCheckError" },
    { 137 /*kStatus_FLASH_SealedFfrRegion*/, "kStatus_FLASH_SealedFfrRegion" },
    { 138 /*kStatus_FLASH_FfrRegionWriteBroken*/, "kStatus_FLASH_FfrRegionWriteBroken" },
    { 139 /*kStatus_FLASH_NmpaUpdateNotAllowed*/, "kStatus_FLASH_NmpaUpdateNotAllowed" },
    { 140 /*kStatus_FLASH_CmpaCfgDirectEraseNotAllowed*/, "kStatus_FLASH_CmpaCfgDirectEraseNotAllowed" },
    { 141 /*kStatus_FLASH_FfrBankIsLocked*/, "kStatus_FLASH_FfrBankIsLocked" },

    // I2C driver errors.
    { 200 /*kStatus_I2C_SlaveTxUnderrun*/, "I2C Slave TX Underrun error." },
    { 201 /*kStatus_I2C_SlaveRxOverrun*/, "I2C Slave RX Overrun error." },
    { 202 /*kStatus_I2C_AribtrationLost*/, "I2C Arbitration Lost error." },

    // SPI driver errors.
    { 300 /*kStatus_SPI_SlaveTxUnderrun*/, "SPI Slave TX Underrun error." },
    { 301 /*kStatus_SPI_SlaveRxOverrun*/, "SPI Slave RX Overrun error." },

    // QSPI driver errors.
    { 400 /*kStatus_QspiFlashSizeError*/, "QSPI Flash Size error." },
    { 401 /*kStatus_QspiFlashAlignmentError*/, "QSPI Flash Alignment error." },
    { 402 /*kStatus_QspiFlashAddressError*/, "QSPI Flash Address error." },
    { 403 /*kStatus_QspiFlashCommandFailure*/, "QSPI Flash Command Failure." },
    { 404 /*kStatus_QspiFlashUnknownProperty*/, "QSPI Flash Unknown Property." },
    { 405 /*kStatus_QspiNotConfigured*/, "QSPI Not Configured." },
    { 406 /*kStatus_QspiCommandNotSupported*/, "QSPI Command Not Supported." },
    { 407 /*kStatus_QspiCommandTimeout*/, "QSPI Command Timeout" },
    { 408 /*kStatus_QspiWriteFailure*/, "QSPI Write Failure." },

    // OTFAD driver errors.
    { 500 /*kStatus_OtfadSecurityViolation*/, "OTFAD Security Violation." },
    { 501 /*kStatus_OtfadLogicallyDisabled*/, "OTFAD Logically Disabled." },
    { 502 /*kStatus_OtfadInvalidKey*/, "OTFAD Invalid Key." },
    { 503 /*Kstatus_OtfadInvalidKeyBlob*/, "OTFAD Invalid KeyBlob." },

    // SDMMC driver errors.
    { 1800 /*kStatus_SDMMC_NotSupportYet*/, "Haven't supported" },
    { 1801 /*kStatus_SDMMC_TransferFailed*/, " Send command failed" },
    { 1802 /*kStatus_SDMMC_SetCardBlockSizeFailed*/, " Set block size failed" },
    { 1803 /*kStatus_SDMMC_HostNotSupport*/, "Host doesn't support" },
    { 1804 /*kStatus_SDMMC_CardNotSupport*/, "Card doesn't support" },
    { 1805 /*kStatus_SDMMC_AllSendCidFailed*/, "Send CID failed" },
    { 1806 /*kStatus_SDMMC_SendRelativeAddressFailed*/, "Send relative address failed" },
    { 1807 /*kStatus_SDMMC_SendCsdFailed*/, "Send CSD failed" },
    { 1808 /*kStatus_SDMMC_SelectCardFailed*/, "Select card failed" },
    { 1809 /*kStatus_SDMMC_SendScrFailed*/, "Send SCR failed" },
    { 1810 /*kStatus_SDMMC_SetDataBusWidthFailed*/, "Set bus width failed" },
    { 1811 /*kStatus_SDMMC_GoIdleFailed*/, "Go idle failed" },
    { 1812 /*kStatus_SDMMC_HandShakeOperationConditionFailed*/, "Send Operation Condition failed" },
    { 1813 /*kStatus_SDMMC_SendApplicationCommandFailed*/, "Send application command failed" },
    { 1814 /*kStatus_SDMMC_SwitchFailed*/, "Switch command failed" },
    { 1815 /*kStatus_SDMMC_StopTransmissionFailed*/, "Stop transmission failed" },
    { 1816 /*kStatus_SDMMC_WaitWriteCompleteFailed*/, "Wait write complete failed" },
    { 1817 /*kStatus_SDMMC_SetBlockCountFailed*/, "Set block count failed" },
    { 1818 /*kStatus_SDMMC_SetRelativeAddressFailed*/, "Set relative address failed" },
    { 1819 /*kStatus_SDMMC_SwitchBusTimingFailed*/, "Switch high speed failed" },
    { 1820 /*kStatus_SDMMC_SendExtendedCsdFailed*/, "Send EXT_CSD failed" },
    { 1821 /*kStatus_SDMMC_ConfigureBootFailed*/, "Configure boot failed" },
    { 1822 /*kStatus_SDMMC_ConfigureExtendedCsdFailed*/, "Configure EXT_CSD failed" },
    { 1823 /*kStatus_SDMMC_EnableHighCapacityEraseFailed*/, "Enable high capacity erase failed" },
    { 1824 /*kStatus_SDMMC_SendTestPatternFailed*/, "Send test pattern failed" },
    { 1825 /*kStatus_SDMMC_ReceiveTestPatternFailed*/, "Receive test pattern failed" },
    { 1826 /*kStatus_SDMMC_SDIO_ResponseError*/, "sdio response error" },
    { 1827 /*kStatus_SDMMC_SDIO_InvalidArgument*/, "sdio invalid argument response error" },
    { 1828 /*kStatus_SDMMC_SDIO_SendOperationConditionFail*/, "sdio send operation condition fail" },
    { 1829 /*kStatus_SDMMC_InvalidVoltage*/, "invaild voltage" },
    { 1830 /*kStatus_SDMMC_SDIO_SwitchHighSpeedFail*/, "switch to high speed fail" },
    { 1831 /*kStatus_SDMMC_SDIO_ReadCISFail*/, "read CIS fail" },
    { 1832 /*kStatus_SDMMC_SDIO_InvalidCard*/, "invaild SDIO card" },
    { 1833 /*kStatus_SDMMC_TuningFail*/, "tuning fail" },
    { 1834 /*kStatus_SDMMC_SwitchVoltageFail*/, "switch voltage fail" },
    { 1835 /*kStatus_SDMMC_ReTuningRequest*/, "retuning request" },
    { 1836 /*kStatus_SDMMC_SetDriverStrengthFail*/, "set driver strength fail" },
    { 1837 /*kStatus_SDMMC_SetPowerClassFail*/, "set power class fail" },

    // Bootloader errors.
    { kStatus_UnknownCommand, "kStatus_UnknownCommand" },
    { kStatus_SecurityViolation, "Command disallowed when security is enabled." },
    { kStatus_AbortDataPhase, "kStatus_AbortDataPhase" },
    { kStatus_Ping, "kStatus_Ping" },
    { kStatus_NoResponse, "No response packet from target device." },
    { kStatus_NoResponseExpected, "No response packet from target device was expected." },

    // SB loader errors.
    { kStatusRomLdrSectionOverrun, "kStatusRomLdrSectionOverrun" },
    { kStatusRomLdrSignature, "kStatusRomLdrSignature" },
    { kStatusRomLdrSectionLength, "kStatusRomLdrSectionLength" },
    { kStatusRomLdrUnencryptedOnly, "kStatusRomLdrUnencryptedOnly" },
    { kStatusRomLdrEOFReached, "kStatusRomLdrEOFReached" },
    { kStatusRomLdrChecksum, "kStatusRomLdrChecksum" },
    { kStatusRomLdrCrc32Error, "kStatusRomLdrCrc32Error" },
    { kStatusRomLdrUnknownCommand, "kStatusRomLdrUnknownCommand" },
    { kStatusRomLdrIdNotFound, "kStatusRomLdrIdNotFound" },
    { kStatusRomLdrDataUnderrun, "kStatusRomLdrDataUnderrun" },
    { kStatusRomLdrJumpReturned, "kStatusRomLdrJumpReturned" },
    { kStatusRomLdrCallFailed, "kStatusRomLdrCallFailed" },
    { kStatusRomLdrKeyNotFound, "kStatusRomLdrKeyNotFound" },
    { kStatusRomLdrSecureOnly, "kStatusRomLdrSecureOnly" },
    { kStatusRomLdrResetReturned, "kStatusRomLdrResetReturned" },
    { kStatusRomLdrRollbackBlocked, "kStatusRomLdrRollbackBlocked" },
    { kStatusRomLdrInvalidSectionMacCount, "kStatusRomLdrInvalidSectionMacCount" },
    { kStatusRomLdrUnexpectedCommand, "kStatusRomLdrUnexpectedCommand" },

    // Memory interface errors.
    { kStatusMemoryRangeInvalid, "kStatusMemoryRangeInvalid" },
    { kStatusMemoryReadFailed, "kStatusMemoryReadFailed" },
    { kStatusMemoryWriteFailed, "kStatusMemoryWriteFailed" },
    { kStatusMemoryCumulativeWrite, "kStatusMemoryCumulativeWrite" },
    { kStatusMemoryAppOverlapWithExecuteOnlyRegion, "kStatusMemoryAppOverlapWithExecuteOnlyRegion" },
    { kStatusMemoryNotConfigured, "kStatusMemoryNotConfigured" },
    { kStatusMemoryAlignmentError, "kStatusMemoryAlignmentError" },
    { kStatusMemoryVerifyFailed, "kStatusMemoryVerifyFailed" },
    { kStatusMemoryWriteProtected, "kStatusMemoryWriteProtected" },
    { kStatusMemoryAddressError, "kStatusMemoryAddressError" },
    { kStatusMemoryBlankCheckFailed, "kStatusMemoryBlankCheckFailed" },
    { kStatusMemoryBlankPageReadDisallowed, "kStatusMemoryBlankPageReadDisallowed" },
    { kStatusMemoryProtectedPageReadDisallowed, "kStatusMemoryProtectedPageReadDisallowed" },
    { kStatusMemoryUnsupportedCommand, "kStatusMemoryUnsupportedCommand" },

    // Property store errors.
    { kStatus_UnknownProperty, "Unknown property." },
    { kStatus_ReadOnlyProperty, "Property is read-only." },
    { kStatus_InvalidPropertyValue, "Invalid property value." },

    /*!< Group number for FLASHIAP status codes */
    { kStatus_FLASHIAP_InvalidArgument, "kStatus_FLASHIAP_InvalidArgument" },
    { kStatus_FLASHIAP_UnknownProperty, "kStatus_FLASHIAP_UnknownProperty" },
    { kStatus_FLASHIAP_AlignmentError, "kStatus_FLASHIAP_AlignmentError" },
    { kStatus_FLASHIAP_AddressError, "kStatus_FLASHIAP_AddressError" },
    { kStatus_FLASHIAP_EraseKeyError, "kStatus_FLASHIAP_EraseKeyError" },
    { kStatus_FLASHIAP_MemoryNotblank, "kStatus_FLASHIAP_MemoryNotblank" },

    /*!< Group number for FLASHIAP wrapper status codes */
    { kStatus_FLASHIAP_Success, "kStatus_FLASHIAP_Success" },
    { kStatus_FLASHIAP_InvalidCommand, "kStatus_FLASHIAP_InvalidCommand" },
    { kStatus_FLASHIAP_SrcAddrError, "kStatus_FLASHIAP_SrcAddrError" },
    { kStatus_FLASHIAP_DstAddrError, "kStatus_FLASHIAP_DstAddrError" },
    { kStatus_FLASHIAP_SrcAddrNotMapped, "kStatus_FLASHIAP_SrcAddrNotMapped" },
    { kStatus_FLASHIAP_DstAddrNotMapped, "kStatus_FLASHIAP_DstAddrNotMapped" },
    { kStatus_FLASHIAP_CountError, "kStatus_FLASHIAP_CountError" },
    { kStatus_FLASHIAP_InvalidSector, "kStatus_FLASHIAP_InvalidSector" },
    { kStatus_FLASHIAP_SectorNotblank, "kStatus_FLASHIAP_SectorNotblank" },
    { kStatus_FLASHIAP_NotPrepared, "kStatus_FLASHIAP_NotPrepared" },
    { kStatus_FLASHIAP_CompareError, "kStatus_FLASHIAP_CompareError" },
    { kStatus_FLASHIAP_Busy, "kStatus_FLASHIAP_Busy" },
    { kStatus_FLASHIAP_ParamError, "kStatus_FLASHIAP_ParamError" },
    { kStatus_FLASHIAP_AddrError, "kStatus_FLASHIAP_AddrError" },
    { kStatus_FLASHIAP_AddrNotMapped, "kStatus_FLASHIAP_AddrNotMapped" },
    { kStatus_FLASHIAP_NoPower, "kStatus_FLASHIAP_NoPower" },
    { kStatus_FLASHIAP_NoClock, "kStatus_FLASHIAP_NoClock" },

    // Application crc check statuses.
    { 10400, "kStatus_AppCrcCheckPassed" },
    { 10401, "kStatus_AppCrcCheckFailed" },
    { 10402, "kStatus_AppCrcCheckInactive" },
    { 10403, "kStatus_AppCrcCheckInvalid" },
    { 10404, "kStatus_AppCrcCheckOutOfRange" },

    // Packetizer errors.
    { kStatus_NoPingResponse, "No response received for ping command." },
    { kStatus_InvalidPacketType, "Invalid packet type." },
    { kStatus_InvalidCRC, "Invalid CRC value." },
    { kStatus_NoCommandResponse, "No response received for command." },

    // Reliable Update statuses.
    { 10600, "kStatus_ReliableUpdateSuccess" },
    { 10601, "kStatus_ReliableUpdateFail" },
    { 10602, "kStatus_ReliableUpdateInacive" },
    { 10603, "kStatus_ReliableUpdateBackupApplicationInvalid" },
    { 10604, "kStatus_ReliableUpdateStillInMainApplication" },
    { 10605, "kStatus_ReliableUpdateSwapSystemNotReady" },
    { 10606, "kStatus_ReliableUpdateBackupBootloaderNotReady" },
    { 10607, "kStatus_ReliableUpdateSwapIndicatorAddressInvalid" },
    { 10608, "kStatus_ReliableUpdateSwapSystemNotAvailable" },

    // Serial NOR/EEPROM statuses.
    { 10700, "kStatus_SerialNorEepromAddressInvalid" },
    { 10701, "kStatus_SerialNorEepromTransferError" },
    { 10702, "kStatus_SerialNorEepromTypeInvalid" },
    { 10703, "kStatus_SerialNorEepromSizeInvalid" },
    { 10704, "kStatus_SerialNorEepromCommandInvalid" },

    // FlexSPI NAND statuses.
    { 20000, "kStatus_FlexSPINAND_ReadPageFail" },
    { 20001, "kStatus_FlexSPINAND_ReadCacheFail" },
    { 20002, "kStatus_FlexSPINAND_EccCheckFail" },
    { 20003, "kStatus_FlexSPINAND_PageLoadFail" },
    { 20004, "kStatus_FlexSPINAND_PageExecuteFail" },
    { 20005, "kStatus_FlexSPINAND_EraseBlockFail" },
    { 20006, "kStatus_FlexSPINAND_WaitTimeout" },
    { 20007, "SPI NAND PageSize over the max supported size" },
    { 20008, "Failed to update Flash config block to SPI NAND" },
    { 20009, "Failed to update discovered bad block table to SPI NAND" },
    { 20010, "kStatus_FlexSPINAND_WriteAlignmentError" },
    { 20011, "kStatus_FlexSPINAND_NotFound" },

    // FlexSPI NOR statuses.
    { 20100, "kStatus_FLEXSPINOR_ProgramFail" },
    { 20101, "kStatus_FLEXSPINOR_EraseSectorFail" },
    { 20102, "kStatus_FLEXSPINOR_EraseAllFail" },
    { 20103, "kStatus_FLEXSPINOR_WaitTimeout" },
    { 20104, "FlexSPI NOR PageSize over the max supported size" },
    { 20105, "kStatus_FlexSPINOR_WriteAlignmentError" },
    { 20106, "kStatus_FlexSPINOR_CommandFailure" },
    { 20107, "kStatus_FlexSPINOR_SFDP_NotFound" },
    { 20108, "kStatus_FLEXSPINOR_Unsupported_SFDP_Version" },
    { 20109, "kStatus_FLEXSPINOR_Flash_NotFound" },
    { 20110, "kStatus_FLEXSPINOR_DTRRead_DummyProbeFailed" },

    { 20200, "kStatus_OCOTP_ReadFailure" },
    { 20201, "kStatus_OCOTP_ProgramFailure" },
    { 20202, "kStatus_OCOTP_ReloadFailure" },
    { 20203, "kStatus_OCOTP_WaitTimeout" },

    // SEMC NOR statuses.
    { 21100, "kStatus_SemcNOR_DeviceTimeout" },
    { 21101, "kStatus_SemcNOR_InvalidMemoryAddress" },
    { 21102, "kStatus_SemcNOR_unmatchedCommandSet" },
    { 21103, "kStatus_SemcNOR_AddressAlignmentError" },
    { 21104, "kStatus_SemcNOR_InvalidCfiSignature" },
    { 21105, "kStatus_SemcNOR_CommandErrorNoOpToSuspend" },
    { 21106, "kStatus_SemcNOR_CommandErrorNoInfoAvailable" },
    { 21107, "kStatus_SemcNOR_BlockEraseCommandFailure" },
    { 21108, "kStatus_SemcNOR_BufferProgramCommandFailure" },
    { 21109, "kStatus_SemcNOR_ProgramVerifyFailure" },
    { 21110, "kStatus_SemcNOR_EraseVerifyFailure" },
    { 21116, "kStatus_SemcNOR_InvalidCfgTag" },

    // SEMC NAND statuses.
    { 21200, "kStatus_SemcNAND_DeviceTimeout" },
    { 21201, "kStatus_SemcNAND_InvalidMemoryAddress" },
    { 21202, "kStatus_SemcNAND_NotEqualToOnePageSize" },
    { 21203, "kStatus_SemcNAND_MoreThanOnePageSize" },
    { 21204, "kStatus_SemcNAND_EccCheckFail" },
    { 21205, "kStatus_SemcNAND_InvalidOnfiParameter" },
    { 21206, "kStatus_SemcNAND_CannotEnableDeviceEcc" },
    { 21207, "kStatus_SemcNAND_SwitchTimingModeFailure" },
    { 21208, "kStatus_SemcNAND_ProgramVerifyFailure" },
    { 21209, "kStatus_SemcNAND_EraseVerifyFailure" },
    { 21210, "kStatus_SemcNAND_InvalidReadbackBuffer" },
    { 21216, "kStatus_SemcNAND_InvalidCfgTag" },
    { 21217, "kStatus_SemcNAND_FailToUpdateFcb" },
    { 21218, "kStatus_SemcNAND_FailToUpdateDbbt" },
    { 21219, "kStatus_SemcNAND_DisallowOverwriteBcb" },
    { 21220, "kStatus_SemcNAND_OnlySupportOnfiDevice" },
    { 21221, "kStatus_SemcNAND_MoreThanMaxImageCopy" },
    { 21222, "kStatus_SemcNAND_DisorderedImageCopies" },

    // SPIFI NOR statuses.
    { 22000, "kStatus_SPIFINOR_ProgramFail" },
    { 22001, "kStatus_SPIFINOR_EraseSectorFail" },
    { 22002, "kStatus_SPIFINOR_EraseAllFail" },
    { 22003, "kStatus_SPIFINOR_WaitTimeout" },
    { 22004, "kStatus_SPIFINOR_NotSupported" },
    { 22005, "kStatus_SPIFINOR_WriteAlignmentError" },
    { 22006, "kStatus_SPIFINOR_CommandFailure" },
    { 22007, "kStatus_SPIFINOR_SFDP_NotFound" },

    // FlexSPI statuses.
    { 70000, "kStatus_FLEXSPI_SequenceExecutionTimeout" },
    { 70001, "kStatus_FLEXSPI_InvalidSequence" },
    { 70002, "kStatus_FLEXSPI_DeviceTimeout" },

    // Terminator
    { kStatusMessageTableTerminatorValue, "" }
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
Command *Command::create(const string_vector_t *argv)
{
    Command *cmd;

    if (argv->at(0) == kCommand_Reset.name)
    {
        cmd = new Reset(argv);
    }
    else if (argv->at(0) == kCommand_GetProperty.name)
    {
        cmd = new GetProperty(argv);
    }
    else if (argv->at(0) == kCommand_SetProperty.name)
    {
        cmd = new SetProperty(argv);
    }
    else if (argv->at(0) == kCommand_FlashEraseRegion.name)
    {
        cmd = new FlashEraseRegion(argv);
    }
    else if (argv->at(0) == kCommand_FlashEraseAll.name)
    {
        cmd = new FlashEraseAll(argv);
    }
    else if (argv->at(0) == kCommand_FlashEraseAllUnsecure.name)
    {
        cmd = new FlashEraseAllUnsecure(argv);
    }
    else if (argv->at(0) == kCommand_GenerateKeyBlob.name)
    {
        cmd = new GenerateKeyBlob(argv);
    }
    else if (argv->at(0) == kCommand_ReadMemory.name)
    {
        cmd = new ReadMemory(argv);
    }
    else if (argv->at(0) == kCommand_WriteMemory.name)
    {
        cmd = new WriteMemory(argv);
    }
	else if (argv->at(0) == kCommand_ProgramAESKey.name)
	{
		// Don't need customer to input address, fix it.
		(const_cast<string_vector_t *>(argv))->insert(argv->begin() + 1, "0x0");
		// Add MemoryID to command, it's always 0x200
		(const_cast<string_vector_t *>(argv))->insert(argv->begin() + 3, "0x200");
		cmd = new WriteMemory(argv);
	}
    else if (argv->at(0) == kCommand_FillMemory.name)
    {
        cmd = new FillMemory(argv);
    }
    else if (argv->at(0) == kCommand_ReceiveSbFile.name)
    {
        cmd = new ReceiveSbFile(argv);
    }
    else if (argv->at(0) == kCommand_Execute.name)
    {
        cmd = new Execute(argv);
    }
    else if (argv->at(0) == kCommand_Call.name)
    {
        cmd = new Call(argv);
    }
    else if (argv->at(0) == kCommand_FlashSecurityDisable.name)
    {
        cmd = new FlashSecurityDisable(argv);
    }
    else if (argv->at(0) == kCommand_FlashProgramOnce.name)
    {
        cmd = new FlashProgramOnce(argv);
    }
    else if (argv->at(0) == kCommand_EfuseProgramOnce.name)
    {
        // Add lenght parameter to the argv. efuse read and program are always 4bytes alignment.
        (const_cast<string_vector_t *>(argv))->insert(argv->begin() + 2, "4");
        cmd = new FlashProgramOnce(argv);
    }
    else if (argv->at(0) == kCommand_FlashReadOnce.name)
    {
        cmd = new FlashReadOnce(argv);
    }
    else if (argv->at(0) == kCommand_EfuseReadOnce.name)
    {
        // Add lenght parameter to the argv. efuse read and program are always 4bytes alignment.
        (const_cast<string_vector_t *>(argv))->insert(argv->begin() + 2, "4");
        cmd = new FlashReadOnce(argv);
    }
    else if (argv->at(0) == kCommand_FlashReadResource.name)
    {
        cmd = new FlashReadResource(argv);
    }
    else if (argv->at(0) == kCommand_ConfigureMemory.name)
    {
        cmd = new ConfigureMemory(argv);
    }
    else if (argv->at(0) == kCommand_ReliableUpdate.name)
    {
        cmd = new ReliableUpdate(argv);
    }
    else if (argv->at(0) == kCommand_KeyProvisoning.name)
    {
        cmd = new KeyProvisioning(argv);
    }
    else if (argv->at(0) == kCommand_FlashImage.name)
    {
        cmd = new FlashImage(argv);
    }
    else if (argv->at(0) == kCommand_ConfigureI2c.name)
    {
        cmd = new ConfigureI2c(argv);
    }
    else if (argv->at(0) == kCommand_ConfigureSpi.name)
    {
        cmd = new ConfigureSpi(argv);
    }
    else if (argv->at(0) == kCommand_ConfigureCan.name)
    {
        cmd = new ConfigureCan(argv);
    }
    else if (argv->at(0) == kCommand_ListMemory.name)
    {
        cmd = new ListMemory(argv);
    }
    else
    {
        return NULL;
    }

    // Validate arguments.
    if (!cmd->init())
    {
        delete cmd;
        return NULL;
    }

    return cmd;
}

// See host_command.h for documentation of this method.
std::string Command::getResponse() const
{
    Json::Value root;
    root["command"] = getName();
    root["status"] = Json::Value(Json::objectValue);
    root["status"]["value"] = static_cast<int32_t>(m_responseValues.at(0));
    root["status"]["description"] = format_string("%d (0x%X) %s", m_responseValues.at(0), m_responseValues.at(0),
                                                  getStatusMessage(m_responseValues.at(0)).c_str());
    root["response"] = Json::Value(Json::arrayValue);
    for (int i = 1; i < (int)m_responseValues.size(); ++i)
    {
        root["response"].append(Json::Value(m_responseValues.at(i)));
    }

    Json::StyledWriter writer;
    return writer.write(root);
}

std::string Command::getStatusMessage(status_t code)
{
    int i;
    for (i = 0; g_statusCodes[i].status != kStatusMessageTableTerminatorValue; ++i)
    {
        if (code == g_statusCodes[i].status)
        {
            return g_statusCodes[i].message;
        }
    }

    return format_string("Unknown error code (%d)", code);
}

// See host_command.h for documentation of this method.
void Command::logResponses() const
{
    const uint32_vector_t *respv = getResponseValues();
    size_t count = respv->size();

    Log::info("Response status = %d (0x%x) %s\n", respv->at(0), respv->at(0), getStatusMessage(respv->at(0)).c_str());

    for (int i = 1; i < (int)count; ++i)
    {
        Log::info("Response word %d = %ld (0x%lx)\n", i, respv->at(i), respv->at(i));
    }

    // Print the response details if there are any.
    if (!m_responseDetails.empty())
    {
        Log::info("%s\n", m_responseDetails.c_str());
    }

    Log::json(getResponse().c_str());
}

// See host_command.h for documentation of this method.
bool Command::processResponse(const generic_response_packet_t *packet, uint8_t commandTag)
{
    if (!packet)
    {
        Log::debug("processResponse: null packet\n");
        m_responseValues.push_back(kStatus_NoResponse);
        return false;
    }

    if (packet->commandPacket.commandTag != kCommandTag_GenericResponse)
    {
        Log::error("Error: expected kCommandTag_GenericResponse (0x%x), received 0x%x\n", kCommandTag_GenericResponse,
                   packet->commandPacket.commandTag);
        m_responseValues.push_back(kStatus_UnknownCommand);
        return false;
    }
    if (packet->commandTag != commandTag)
    {
        Log::error("Error: expected commandTag 0x%x, received 0x%x\n", commandTag, packet->commandTag);
        m_responseValues.push_back(kStatus_UnknownCommand);
        return false;
    }

    // Set the status in the response vector.
    m_responseValues.push_back(packet->status);

    if (packet->status != kStatus_Success)
    {
        return false;
    }

    if (commandTag != kCommandTag_GenerateKeyBlob)
    {
        Log::info("Successful generic response to command '%s'\n", getName().c_str());
    }
    return true;
}

//! See host_command.h for documentation on this function.
bool blfwk::DataPacket::FileDataProducer::init(string filePath, uint32_t count)
{
    m_filePointer = fopen(filePath.c_str(), "rb");
    if (!m_filePointer)
    {
        Log::error("Error: cannot open input data file '%s'\n", filePath.c_str());
        return false;
    }

    // If the fileSize wasn't specified, get the file size.
    if (count)
    {
        m_fileSize = count;
    }
    else
    {
        ::fseek(m_filePointer, 0, SEEK_END);
        m_fileSize = ftell(m_filePointer);
        ::fseek(m_filePointer, 0, SEEK_SET);
    }

    Log::info("Preparing to send %d (0x%x) bytes to the target.\n", m_fileSize, m_fileSize);
    return true;
}

//! See host_command.h for documentation on this function.
uint32_t blfwk::DataPacket::FileDataProducer::getData(uint8_t *data, uint32_t size)
{
    assert(m_filePointer);
    assert(data);
    if ((size == 0) || !hasMoreData())
    {
        return 0;
    }

    return (uint32_t)fread(data, 1, size, m_filePointer);
}

//! See host_command.h for documentation on this function.
uint32_t blfwk::DataPacket::HexDataProducer::initFromString(const string hexData)
{
    // Remove everything from string except for hex digits.
    string hexString = string_hex(hexData);

    // Clear any existing data.
    m_data.clear();

    // Load data byte array from hex string.
    // Two hex characters equals one byte.
    // Any trailing character is ignored.
    for (uint32_t i = 0; i < hexString.size(); i += 2)
    {
        string hexByte = hexString.substr(i, 2);
        long int val = strtol(hexByte.c_str(), NULL, 16);
        m_data.push_back(static_cast<uint8_t>(val));
    }

    return m_data.size();
}

//! See host_command.h for documentation on this function.
uint32_t blfwk::DataPacket::HexDataProducer::getData(uint8_t *data, uint32_t size)
{
    assert(data);

    if (!hasMoreData())
    {
        return 0;
    }

    uint32_t remaining = m_data.size() - m_byteIndex;
    size = min(size, remaining);
    memcpy(data, &m_data[m_byteIndex], size);
    m_byteIndex += size;
    return size;
}

//! See host_command.h for documentation on this function.
uint32_t blfwk::DataPacket::SegmentDataProducer::getData(uint8_t *data, uint32_t size)
{
    assert(data);

    if (!hasMoreData())
    {
        return 0;
    }

    size = m_segment->getData(m_byteIndex, size, data);
    m_byteIndex += size;
    return size;
}

//! See host_command.h for documentation on this function.
bool blfwk::DataPacket::FileDataConsumer::init(string filePath)
{
    m_filePointer = fopen(filePath.c_str(), "wb");
    if (!m_filePointer)
    {
        Log::error("Error: cannot open output data file '%s'\n", filePath.c_str());
        return false;
    }
    return true;
}

//! See host_command.h for documentation on this function.
void blfwk::DataPacket::FileDataConsumer::processData(const uint8_t *data, uint32_t size)
{
    fwrite(data, 1, size, m_filePointer);
}

//! See host_command.h for documentation on this function.
void blfwk::DataPacket::StdOutDataConsumer::processData(const uint8_t *data, uint32_t size)
{
    // Only the size of the final packet can be smaller than 32 bytes.
    // So moving the cursor to the start will not over-write the data already displayed.
    printf("\r");

    if (m_dataCacheCount)
    {
        for (int i = 0; i < m_dataCacheCount; ++i)
        {
            printf("%02x", m_dataCache[i]);
            if ((m_currentCount++ % kBytesPerLine) == 0)
            {
                printf("\n");
            }
            else
            {
                printf(" ");
            }
        }
        m_dataCacheCount = 0;
    }

    for (int i = 0; i < (int)size; ++i)
    {
        printf("%02x", data[i]);
        if ((m_currentCount++ % kBytesPerLine) == 0)
        {
            printf("\n");
            if ((size - i - 1) < kBytesPerLine)
            {
                memcpy(m_dataCache, &data[i + 1], size - i - 1);
                m_dataCacheCount = size - i - 1;
                return;
            }
        }
        else
        {
            printf(" ");
        }
    }
}

//! See host_command.h for documentation on this function.
uint8_t *blfwk::DataPacket::sendTo(Packetizer &device, uint32_t *bytesWritten, Progress *progress)
{
    *bytesWritten = 0;

    if (!m_dataProducer->hasMoreData())
    {
        device.pumpSimulator();
    }

    while (m_dataProducer->hasMoreData() && *bytesWritten < m_dataProducer->getDataSize())
    {
        uint32_t count = MIN(m_packetSize, (m_dataProducer->getDataSize() - *bytesWritten));
        count = m_dataProducer->getData(m_packet, count);
        if (count)
        {
            status_t status = device.writePacket((const uint8_t *)m_packet, count, kPacketType_Data);
            if (status != kStatus_Success)
            {
                Log::error("Data phase write aborted by status 0x%x %s\n", status,
                           Command::getStatusMessage(status).c_str());
                if ((status == kStatus_AbortDataPhase) && device.isAbortEnabled())
                {
                    Log::error("Possible JUMP or RESET command received.\n");
                }
                break;
            }

            *bytesWritten += count;

            if (progress != NULL)
            {
                // execute process callback function.
                progress->progressCallback(*bytesWritten * 100 / m_dataProducer->getDataSize());
                if (progress->abortPhase())
                {
                    device.writePacket((const uint8_t *)&m_packet, 0, kPacketType_Data);
                    break;
                }
            }
#ifdef TEST_SENDER_ABORT
            // Send zero length packet to abort data phase.
            Log::info("Testing data phase abort\n");
            device.writePacket((const uint8_t *)&m_packet, 0, kPacketType_Data);
            break;
#endif
        }
    }

    // Read final command status
    uint8_t *responsePacket;
    uint32_t responseLength;
    if (device.readPacket(&responsePacket, &responseLength, kPacketType_Command) != kStatus_Success)
    {
        return NULL;
    }
    return responsePacket;
}

//! See host_command.h for documentation on this function.
uint8_t *blfwk::DataPacket::receiveFrom(Packetizer &device, uint32_t *byteCount, Progress *progress)
{
    status_t status = kStatus_Success;
    uint32_t totalCount = *byteCount;
    // If byte count is zero, need to pump the simulator to get the final response
    if (*byteCount == 0)
    {
        device.pumpSimulator();
    }

    while (*byteCount > 0)
    {
        uint8_t *dataPacket;
        uint32_t length;

        // Pump the simulator command state machine, if it is enabled.
        device.pumpSimulator();

        status = device.readPacket(&dataPacket, &length, kPacketType_Data);

        // Bail if there was an error reading the packet.
        if (status != kStatus_Success)
        {
            Log::info("Read data packet error. Sending ACK.\n");
            m_dataConsumer->finalize();
            device.sync();
            return NULL;
        }

        // Check for sender abort of data phase.
        if (length == 0)
        {
            Log::info("Data phase aborted by sender\n");
            break;
        }

        m_dataConsumer->processData(dataPacket, length);
        *byteCount -= length;

        if (*byteCount <= 0)
        {
            m_dataConsumer->finalize();
        }

        if (progress != NULL)
        {
            progress->progressCallback((totalCount - *byteCount) * 100 / totalCount);
            if (progress->abortPhase())
            {
                device.abortPacket();
                break;
            }
        }
#ifdef TEST_RECEIVER_ABORT
        Log::info("Testing data phase abort\n");
        device.abortPacket();
        break;
#endif
    }

    // Read the final generic response packet.
    uint8_t *responsePacket;
    uint32_t responseLength;
    if (device.readPacket(&responsePacket, &responseLength, kPacketType_Command) != kStatus_Success)
    {
        return NULL;
    }
    return responsePacket;
}

//! See host_command.h for documentation on this function.
const uint8_t *blfwk::CommandPacket::sendCommandGetResponse(Packetizer &device)
{
    uint8_t *responsePacket = NULL;
    uint32_t responseLength = 0;

    status_t status = device.writePacket(getData(), getSize(), kPacketType_Command);
    if (status != kStatus_Success)
    {
        Log::error("sendCommandGetResponse.writePacket error %d.\n", status);
        return NULL;
    }
    status = device.readPacket(&responsePacket, &responseLength, kPacketType_Command);
    if (status == kStatus_Success)
    {
        return responsePacket;
    }
    else
    {
        Log::error("sendCommandGetResponse.readPacket error %d.\n", status);
        return NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Reset command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool Reset::init()
{
    if (getArgCount() != 1)
    {
        return false;
    }
    return true;
}

// See host_command.h for documentation of this method.
void Reset::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_Reset, kCommandFlag_None);
    const uint8_t *responsePacket = cmdPacket.sendCommandGetResponse(device);
    if (responsePacket)
    {
        processResponse(responsePacket);
    }
    else
    {
        Log::warning("Ignoring missing response from reset command.\n");
        this->m_responseValues.push_back(kStatus_Success);
    }
}

////////////////////////////////////////////////////////////////////////////////
// GetProperty command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool GetProperty::init()
{
    if (getArgCount() != 2 && getArgCount() != 3)
    {
        return false;
    }

    // Init property from tag or description.
    uint32_t tag = kPropertyTag_InvalidProperty;
    PropertyArray::const_iterator it;
    if (utils::stringtoui(getArg(1), tag))
    {
        for (it = kProperties.begin(); it != kProperties.end(); ++it)
        {
            if (it->value == tag)
                break;
        }
    }
    else
    {
        for (it = kProperties.begin(); it != kProperties.end(); ++it)
        {
            if (strcmp(it->description, getArg(1).c_str()) == 0)
                break;
        }
    }

    if (it == kProperties.end())
    {
        m_property = kProperty_Invalid;
    }
    else
    {
        m_property = *it;
    }

    if (getArgCount() == 3)
    {
        if (!utils::stringtoui(getArg(2), m_memoryIdorIndex))
        {
            return false;
        }
    }
    else
    {
        m_memoryIdorIndex = kMemoryInternal;
    }

    return true;
}

// See host_command.h for documentation of this method.
void GetProperty::sendTo(Packetizer &device)
{
    // See if the user just wants a list of the possible properites.
    if (m_property.value == kPropertyTag_ListProperties)
    {
        Log::info("The possible properties for the %s command are\n", getName().c_str());
        for (auto it = kProperties.begin(); it != kProperties.end(); ++it)
        {
            Log::info("    %d or '%s'\n", it->value, it->description);
        }

        m_responseValues.push_back(kStatus_NoResponseExpected);
    }
    else
    {
        // Command Phase
        blfwk::CommandPacket cmdPacket(kCommandTag_GetProperty, kCommandFlag_None, m_property.value, m_memoryIdorIndex);
        const uint8_t *responsePacket = cmdPacket.sendCommandGetResponse(device);

        const get_property_response_packet_t *packet =
            reinterpret_cast<const get_property_response_packet_t *>(responsePacket);
        processResponse(packet);
    }
}

bool GetProperty::processResponse(const get_property_response_packet_t *packet)
{
    if (!packet)
    {
        Log::debug("processResponse: null packet\n");
        m_responseValues.push_back(kStatus_NoResponse);
        return false;
    }

    // Handle generic response, which would be returned if command is not supported.
    if (packet->commandPacket.commandTag == kCommandTag_GenericResponse)
    {
        return processResponse((const uint8_t *)packet);
    }

    if (packet->commandPacket.commandTag != kCommandTag_GetPropertyResponse)
    {
        Log::error("Error: expected kCommandTag_GetPropertyResponse (0x%x), received 0x%x\n",
                   kCommandTag_GetPropertyResponse, packet->commandPacket.commandTag);
        m_responseValues.push_back(kStatus_UnknownCommand);
        return false;
    }

    // Set the status in the response vector.
    m_responseValues.push_back(packet->status);

    if (packet->status != kStatus_Success)
    {
        return false;
    }

    Log::debug("Successful response to command '%s(%s)'\n", getName().c_str(), m_property.description);

    // Currently, no properties have a data phase.
    assert(!(packet->commandPacket.flags & kCommandFlag_HasDataPhase));

    // All properties have at least one response word.
    // Attention: parameterCount = 1(response status) + response words
    for (uint8_t i = 0; i < (packet->commandPacket.parameterCount - 1); ++i)
    {
        m_responseValues.push_back(packet->propertyValue[i]);
    }

    // Format the returned property details.
    switch (m_property.value)
    {
        case kPropertyTag_BootloaderVersion:
            m_responseDetails =
                format_string("Current Version = %c%d.%d.%d", (m_responseValues.at(1) & 0xff000000) >> 24,
                              (m_responseValues.at(1) & 0x00ff0000) >> 16, (m_responseValues.at(1) & 0x0000ff00) >> 8,
                              m_responseValues.at(1) & 0x000000ff);
            break;
        case kPropertyTag_TargetVersion:
            m_responseDetails =
                format_string("Target Version = %c%d.%d.%d", (m_responseValues.at(1) & 0xff000000) >> 24,
                              (m_responseValues.at(1) & 0x00ff0000) >> 16, (m_responseValues.at(1) & 0x0000ff00) >> 8,
                              m_responseValues.at(1) & 0x000000ff);
            break;
        case kPropertyTag_AvailablePeripherals:
        {
            m_responseDetails = "Available Peripherals = ";
            uint32_t peripherals = m_responseValues.at(1);
            uint32_t position = 0;
            while (peripherals)
            {
                if (peripherals & 0x00000001)
                {
                    m_responseDetails.append(kPeripherals.at(position).description);
                    if (peripherals >> 1)
                        m_responseDetails.append(", ");
                }
                peripherals >>= 1;
                ++position;
            }
            break;
        }
        case kPropertyTag_FlashStartAddress:
            m_responseDetails = format_string("Flash Start Address = 0x%08X", m_responseValues.at(1));
            break;
        case kPropertyTag_FlashSizeInBytes:
            m_responseDetails = format_string("Flash Size = %s", utils::scale_bytes(m_responseValues.at(1)).c_str());
            break;
        case kPropertyTag_FlashSectorSize:
            m_responseDetails =
                format_string("Flash Sector Size = %s", utils::scale_bytes(m_responseValues.at(1)).c_str());
            break;
        case kPropertyTag_FlashBlockCount:
            m_responseDetails = format_string("Flash Block Count = %d", m_responseValues.at(1));
            break;
        case kPropertyTag_AvailableCommands:
        {
            m_responseDetails = "Available Commands = ";
            uint32_t commands = m_responseValues.at(1);
            uint32_t position = 0;
            while (commands)
            {
                if (commands & 0x00000001)
                {
                    m_responseDetails.append(kCommands.at(position).name);
                    if (commands >> 1)
                        m_responseDetails.append(", ");
                }
                commands >>= 1;
                ++position;
            }
            break;
        }
        case kPropertyTag_CrcCheckStatus:
            m_responseDetails =
                format_string("CRC Check Status = %s", getStatusMessage(m_responseValues.at(1)).c_str());
            break;
        case kPropertyTag_VerifyWrites:
            m_responseDetails = format_string("Verify Writes Flag = %s", m_responseValues.at(1) ? "ON" : "OFF");
            break;
        case kPropertyTag_MaxPacketSize:
            m_responseDetails =
                format_string("Max Packet Size = %s", utils::scale_bytes(m_responseValues.at(1)).c_str());
            break;
        case kPropertyTag_ReservedRegions:
        {
            uint32_t m_reservedRegionCount = packet->commandPacket.parameterCount / 2;
            uint32_t m_validRegionIndex = 0;
            m_responseDetails = "Reserved Regions = ";
            for (uint32_t i = 0; i < m_reservedRegionCount; i++)
            {
                uint32_t m_regionStart = m_responseValues.at(1 + 2 * i);
                uint32_t m_regionEnd = m_responseValues.at(2 + 2 * i);
                uint32_t m_regionLength = m_regionEnd - m_regionStart + 1;

                if (m_regionLength > 1)
                {
                    m_responseDetails.append(format_string("\n        Region%d: 0x%X-0x%X (%s) ", m_validRegionIndex,
                                                           m_regionStart, m_regionEnd,
                                                           utils::scale_bytes(m_regionLength).c_str()));
                    ++m_validRegionIndex;
                }
            }
            m_responseDetails.append("\n");

            break;
        }
        case kPropertyTag_RAMStartAddress:
            m_responseDetails = format_string("RAM Start Address = 0x%08X", m_responseValues.at(1));
            break;
        case kPropertyTag_RAMSizeInBytes:
            m_responseDetails = format_string("RAM Size = %s", utils::scale_bytes(m_responseValues.at(1)).c_str());
            break;
        case kPropertyTag_SystemDeviceId:
            m_responseDetails = format_string("System Device ID = 0x%08X", m_responseValues.at(1));
            break;
        case kPropertyTag_FlashSecurityState:
            m_responseDetails =
                format_string("Flash Security State = %s", m_responseValues.at(1) ? "SECURE" : "UNSECURE");
            break;
        case kPropertyTag_UniqueDeviceId:
            m_responseDetails = "Unique Device ID =";
            for (uint32_t i = 1; i < m_responseValues.size(); ++i)
            {
                m_responseDetails.append(format_string(" %04X %04X", (m_responseValues.at(i) & 0xffff0000) >> 16,
                                                       m_responseValues.at(i) & 0x0000ffff));
            }
            break;
        case kPropertyTag_FacSupport:
            m_responseDetails = format_string("Flash Access Controller (FAC) Support Flag = %s",
                                              m_responseValues.at(1) ? "SUPPORTED" : "UNSUPPORTED");
            break;
        case kPropertyTag_FlashAccessSegmentSize:
            m_responseDetails =
                format_string("Flash Access Segment Size = %s", utils::scale_bytes(m_responseValues.at(1)).c_str());
            break;
        case kPropertyTag_FlashAccessSegmentCount:
            m_responseDetails = format_string("Flash Access Segment Count = %d", m_responseValues.at(1));
            break;
        case kPropertyTag_FlashReadMargin:
            m_responseDetails = "Flash read margin level = ";
            if (m_responseValues.at(1) == 0)
                m_responseDetails.append("NORMAL");
            else if (m_responseValues.at(1) == 1)
                m_responseDetails.append("USER");
            else if (m_responseValues.at(1) == 2)
                m_responseDetails.append("FACTORY");
            else
                m_responseDetails.append(format_string("UNKNOWN (%d)", m_responseValues.at(1)));
            break;
        case kPropertyTag_QspiInitStatus:
            m_responseDetails =
                format_string("QSPI Init Status = %s", getStatusMessage(m_responseValues.at(1)).c_str());
            break;
        case kPropertyTag_ExternalMemoryAttributes:
        {
            uint32_t m_propertyTags = m_responseValues.at(1);
            uint32_t m_memStartAddress = m_responseValues.at(2);
            uint32_t m_memSizeInKBytes = m_responseValues.at(3);
            uint32_t m_memPageSize = m_responseValues.at(4);
            uint32_t m_memSectorSize = m_responseValues.at(5);
            uint32_t m_memBlockSize = m_responseValues.at(6);

            if (m_propertyTags > 0)
            {
                string externMemoryDescriptor = "UNKNOWN";

                for (MemoryArray::const_iterator it = kMemories.begin(); it != kMemories.end(); ++it)
                {
                    if (m_memoryIdorIndex == it->memoryId)
                    {
                        externMemoryDescriptor = it->description;
                    }
                }
                m_responseDetails = format_string("%s Attributes: ", externMemoryDescriptor.c_str());
            }

            if (m_propertyTags & (1 << (kExternalMemoryPropertyTag_StartAddress - 1)))
            {
                m_responseDetails.append(format_string("Start Address = 0x%08x  ", m_memStartAddress));
            }

            if (m_propertyTags & (1 << (kExternalMemoryPropertyTag_MemorySizeInKbytes - 1)))
            {
                uint64_t m_memorySizeInBytes = (uint64_t)m_memSizeInKBytes * 1024;
                m_responseDetails.append(
                    format_string("Total Size = %s  ", utils::scale_bytes(m_memorySizeInBytes).c_str()));
            }

            if (m_propertyTags & (1 << (kExternalMemoryPropertyTag_PageSize - 1)))
            {
                m_responseDetails.append(format_string("Page Size = %s  ", utils::scale_bytes(m_memPageSize).c_str()));
            }

            if (m_propertyTags & (1 << (kExternalMemoryPropertyTag_SectorSize - 1)))
            {
                m_responseDetails.append(
                    format_string("Sector Size = %s  ", utils::scale_bytes(m_memSectorSize).c_str()));
            }

            if (m_propertyTags & (1 << (kExternalMemoryPropertyTag_BlockSize - 1)))
            {
                m_responseDetails.append(format_string("Block Size = %s ", utils::scale_bytes(m_memBlockSize).c_str()));
            }
        }
        break;
        case kPropertyTag_ReliableUpdateStatus:
            m_responseDetails =
                format_string("Reliable Update Status = %s", getStatusMessage(m_responseValues.at(1)).c_str());
            break;
        case kPropertyTag_FlashPageSize:
            m_responseDetails =
                format_string("Flash Page Size = %s", utils::scale_bytes(m_responseValues.at(1)).c_str());
            break;
        case kPropertyTag_IrqNotifierPin:
            m_responseDetails = format_string("Irq pin value = %08x", m_responseValues.at(1));
            break;
        case kPropertyTag_FfrKeystoreUpdateOpt:
            m_responseDetails =
                format_string("FFR KeyStore Update is %s", (m_responseValues.at(1) > 0) ? "enabled" : "disabled");
            break;
        case kPropertyTag_Reserved9:
        case kPropertyTag_InvalidProperty:
        default:
            break;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// SetProperty command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool SetProperty::init()
{
    if (getArgCount() != 3)
    {
        return false;
    }
    // Save property tag number and value.
    if (!utils::stringtoui(getArg(1), m_propertyTag))
    {
        return false;
    }
    if (!utils::stringtoui(getArg(2), m_propertyValue))
    {
        return false;
    }
    return true;
}

// See host_command.h for documentation of this method.
void SetProperty::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_SetProperty, kCommandFlag_None, m_propertyTag, m_propertyValue);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// FlashEraseRegion command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool FlashEraseRegion::init()
{
    if ((getArgCount() != 3) && (getArgCount() != 4))
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_startAddress))
    {
        return false;
    }
    if (!utils::stringtoui(getArg(2), m_byteCount))
    {
        return false;
    }

    if (getArgCount() == 4)
    {
        if (!utils::stringtoui(getArg(3), m_memoryId))
        {
            return false;
        }
        // Use 0 for erasing internal 4G memory including mapped memory, such as QSPI
        if (GROUPID(m_memoryId) == kGroup_Internal)
        {
            if (m_memoryId != kMemoryInternal)
            {
                Log::warning(
                    "Note: memoryId is not required for flash-erase-region when accessing mapped external memory. "
                    "Ignore this parameter.\n");
                m_memoryId = kMemoryInternal;
            }
        }
    }
    else
    {
        m_memoryId = kMemoryInternal;
    }

    return true;
}

// See host_command.h for documentation of this method.
void FlashEraseRegion::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_FlashEraseRegion, kCommandFlag_None, m_startAddress, m_byteCount,
                                   m_memoryId);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// FlashEraseAll command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool FlashEraseAll::init()
{
    if ((getArgCount() != 1) && (getArgCount() != 2))
    {
        return false;
    }

    if (getArgCount() == 1)
    {
        m_memoryId = kMemoryInternal;
        return true;
    }
    else if (getArgCount() == 2)
    {
        if (!utils::stringtoui(getArg(1), m_memoryId))
        {
            return false;
        }
        return true;
    }

    return false;
}

// See host_command.h for documentation of this method.
void FlashEraseAll::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_FlashEraseAll, kCommandFlag_None, m_memoryId);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// FlashEraseAllUnsecure command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool FlashEraseAllUnsecure::init()
{
    if (getArgCount() != 1)
    {
        return false;
    }
    return true;
}

// See host_command.h for documentation of this method.
void FlashEraseAllUnsecure::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_FlashEraseAllUnsecure, kCommandFlag_None);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// ReadMemory command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool ReadMemory::init()
{
    if ((getArgCount() < 3) || (getArgCount() > 5))
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_startAddress))
    {
        return false; // invalid 'addr' parameter
    }
    if (!utils::stringtoui(getArg(2), m_byteCount))
    {
        return false; // invalid 'count' parameter
    }

    // File name argument is optional - will use stdout if missing.
    if (getArgCount() == 4)
    {
        // If the argument 3 is a number, then it is m_memoryId.
        // Otherwise, it is a file name.
        // File name "123" is not acceptable, while "123.bin" is OK.
        if (!utils::stringtoui(getArg(3), m_memoryId))
        {
            m_memoryId = kMemoryInternal;
            m_dataFile = getArg(3);
        }
        // Use 0 for reading internal 4G memory including mapped memory, such as QSPI
        else if (GROUPID(m_memoryId) == kGroup_Internal)
        {
            if (m_memoryId != kMemoryInternal)
            {
                Log::warning(
                    "Note: memoryId is not required for read-memory when accessing mapped external memory. Ignore this "
                    "parameter.\n");
                m_memoryId = kMemoryInternal;
            }
        }
    }

    if (getArgCount() == 5)
    {
        m_dataFile = getArg(3);
        if (!utils::stringtoui(getArg(4), m_memoryId))
        {
            return false; // invalid 'memory ID' parameter
        }
        // Use 0 for reading internal 4G memory including mapped memory, such as QSPI
        if (GROUPID(m_memoryId) == kGroup_Internal)
        {
            if (m_memoryId != kMemoryInternal)
            {
                Log::warning(
                    "Note: memoryId is not required for read-memory when accessing mapped external memory. Ignore this "
                    "parameter.\n");
                m_memoryId = kMemoryInternal;
            }
        }
    }

    return true;
}

// See host_command.h for documentation of this method.
void ReadMemory::sendTo(Packetizer &device)
{
    DataPacket::DataConsumer *dataConsumer;
    DataPacket::FileDataConsumer fileDataConsumer;
    DataPacket::StdOutDataConsumer stdoutDataConsumer;

    // Setup to write to file or stdout
    if (m_dataFile.size() > 0)
    {
        if (!fileDataConsumer.init(m_dataFile))
        {
            return;
        }
        dataConsumer = &fileDataConsumer;
    }
    else
    {
        dataConsumer = &stdoutDataConsumer;
    }

    // Send command packet.
    blfwk::CommandPacket cmdPacket(kCommandTag_ReadMemory, kCommandFlag_None, m_startAddress, m_byteCount, m_memoryId);
    const uint8_t *responsePacket = cmdPacket.sendCommandGetResponse(device);

    const read_memory_response_packet_t *packet =
        reinterpret_cast<const read_memory_response_packet_t *>(responsePacket);
    uint32_t byteCount = m_byteCount;
    if (processResponse(packet))
    {
        byteCount = packet->dataByteCount;

        // Receive data packets.
        blfwk::DataPacket dataPacket(dataConsumer);
        uint8_t *finalResponsePacket = dataPacket.receiveFrom(device, &byteCount, m_progress);
        processResponse(finalResponsePacket);
    }

    // Push the number of bytes transferred response value.
    m_responseValues.push_back(m_byteCount - byteCount);

    // Format the command transfer details.
    m_responseDetails = format_string("Read %d of %d bytes.", m_byteCount - byteCount, m_byteCount);
}

bool ReadMemory::processResponse(const read_memory_response_packet_t *packet)
{
    if (!packet)
    {
        Log::debug("processResponse: null packet\n");
        m_responseValues.push_back(kStatus_NoResponse);
        return false;
    }

    // Handle generic response, which would be returned if command is not supported.
    if (packet->commandPacket.commandTag == kCommandTag_GenericResponse)
    {
        return processResponse((const uint8_t *)packet);
    }

    if (packet->commandPacket.commandTag != kCommandTag_ReadMemoryResponse)
    {
        Log::error("Error: expected kCommandTag_ReadMemoryResponse (0x%x), received 0x%x\n",
                   kCommandTag_ReadMemoryResponse, packet->commandPacket.commandTag);
        return false;
    }
    if (packet->status != kStatus_Success)
    {
        // Set the status in the response vector.
        // If status is OK, this push will be done by final response processing
        m_responseValues.push_back(packet->status);
        return false;
    }

    Log::info("Successful response to command '%s'\n", getName().c_str());

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// WriteMemory command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool WriteMemory::init()
{
    if (getArgCount() != 3 && getArgCount() != 4)
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_startAddress))
    {
        return false;
    }
    m_fileOrData = getArg(2);

    // Try to find the separator ','.
    size_t separatorIndex = m_fileOrData.find(',', 0);
    if (separatorIndex != string::npos)
    {
        // If found, the left string is the byte count to write.
        if (!utils::stringtoui(m_fileOrData.substr(separatorIndex + 1, m_fileOrData.length() - separatorIndex - 1),
                               m_count))
        {
            return false;
        }
        m_fileOrData = m_fileOrData.substr(0, separatorIndex);
    }

    if ((m_fileOrData[0] == '{') && (m_fileOrData[1] == '{'))
    {
        DataPacket::HexDataProducer hexProducer;
        // Argument string is hex data, so use hex data producer.
        if (hexProducer.initFromString(m_fileOrData) == 0)
        {
            return false;
        }
    }

    if (getArgCount() == 4)
    {
        if (!utils::stringtoui(getArg(3), m_memoryId))
        {
            return false;
        }
        // Use 0 for writinging internal 4G memory including mapped memory, such as QSPI
        if (GROUPID(m_memoryId) == kGroup_Internal)
        {
            if (m_memoryId != kMemoryInternal)
            {
                Log::warning(
                    "Note: memoryId is not required for write-memory when accessing mapped external memory. Ignore "
                    "this parameter.\n");
                m_memoryId = kMemoryInternal;
            }
        }
    }
    else
    {
        m_memoryId = kMemoryInternal;
    }

    return true;
}

// See host_command.h for documentation of this method.
void WriteMemory::sendTo(Packetizer &device)
{
    DataPacket::HexDataProducer hexProducer(m_data);
    DataPacket::FileDataProducer fileProducer;
    DataPacket::SegmentDataProducer segmentProducer(m_segment);
    DataPacket::DataProducer *dataProducer;

    if (m_segment)
    {
        dataProducer = &segmentProducer;
    }
    else if ((m_fileOrData[0] == '{') && (m_fileOrData[1] == '{'))
    {
        // Argument string is hex data, so use hex data producer.
        if (hexProducer.initFromString(m_fileOrData) == 0)
        {
            return;
        }
        dataProducer = &hexProducer;
    }
    else if (m_data.size() > 0)
    {
        dataProducer = &hexProducer;
    }
    else
    {
        // Argument string is file name, so use file data producer.
        if (!fileProducer.init(m_fileOrData, m_count))
        {
            return;
        }
        dataProducer = &fileProducer;
    }

    // Get target bootloader data packet size.
    uint32_t packetSizeInBytes;
    GetProperty getPacketSize(kProperty_MaxPacketSize, 0 /*Not used*/);
    getPacketSize.sendTo(device);
    uint32_t fw_status = getPacketSize.getResponseValues()->at(0);
    if (fw_status != kStatus_Success)
    {
        // Failed to get data packet size.
        Log::warning("Warning: Failed to get packet size. Using default size(%d)", kMinPacketBufferSize);
        packetSizeInBytes = kMinPacketBufferSize; // No property. Use default packet size.
    }
    else
    {
        packetSizeInBytes = getPacketSize.getResponseValues()->at(1);
        if (packetSizeInBytes > device.getMaxPacketSize())
        {
            Log::error("Error: Packet size(%d) is bigger than max supported size(%d).", packetSizeInBytes,
                       kMaxHostPacketSize);
            return;
        }
    }

    // Send command packet.
    uint32_t bytesToWrite = dataProducer->getDataSize();
    uint32_t bytesWritten;
    blfwk::CommandPacket cmdPacket(kCommandTag_WriteMemory, kCommandFlag_HasDataPhase, m_startAddress, bytesToWrite,
                                   m_memoryId);
    if (!processResponse(cmdPacket.sendCommandGetResponse(device)))
    {
        m_responseDetails = format_string("Wrote 0 of %d bytes.", bytesToWrite);
        return;
    }

    // Pop the initial (successful) generic response value.
    if (m_responseValues.size())
    {
        m_responseValues.pop_back();
    }

    // Send data packets.
    blfwk::DataPacket dataPacket(dataProducer, packetSizeInBytes);

    processResponse(dataPacket.sendTo(device, &bytesWritten, m_progress));

    // Format the command transfer details.
    m_responseDetails = format_string("Wrote %d of %d bytes.", bytesWritten, bytesToWrite);
}

////////////////////////////////////////////////////////////////////////////////
// GenerateKeyBlob command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool GenerateKeyBlob::init()
{
    if (getArgCount() != 3)
    {
        return false;
    }

    m_fileDek = getArg(1);

    m_fileKeyBlob = getArg(2);

    return true;
}

// See host_command.h for documentation of this method.
void GenerateKeyBlob::sendTo(Packetizer &device)
{
    DataPacket::FileDataProducer fileProducer;
    DataPacket::DataProducer *dataProducer;

    // Argument string is file name, so use file data producer.
    if (!fileProducer.init(m_fileDek, m_count))
    {
        return;
    }
    dataProducer = &fileProducer;

    m_dataPhase = 0;
    // Send command packet.
    uint32_t bytesToWrite = dataProducer->getDataSize();
    uint32_t bytesWritten;
    blfwk::CommandPacket cmdPacket(kCommandTag_GenerateKeyBlob, kCommandFlag_HasDataPhase, 0, bytesToWrite,
                                   m_dataPhase);
    if (!processResponse(cmdPacket.sendCommandGetResponse(device)))
    {
        m_responseDetails = format_string("Wrote 0 of %d bytes.", bytesToWrite);
        return;
    }

    // Pop the initial (successful) generic response value.
    if (m_responseValues.size())
    {
        m_responseValues.pop_back();
    }

    // Send data packets.
    blfwk::DataPacket dataPacket(dataProducer);
    processResponse(dataPacket.sendTo(device, &bytesWritten, 0));

    // Format the command transfer details.
    m_responseDetails = format_string("Send %d of %d bytes.", bytesWritten, bytesToWrite);

    DataPacket::DataConsumer *dataConsumer;
    DataPacket::FileDataConsumer fileDataConsumer;
    DataPacket::StdOutDataConsumer stdoutDataConsumer;

    // Setup to write to file or stdout
    if (m_fileKeyBlob.size() > 0)
    {
        if (!fileDataConsumer.init(m_fileKeyBlob))
        {
            return;
        }
        dataConsumer = &fileDataConsumer;
    }
    else
    {
        dataConsumer = &stdoutDataConsumer;
    }

    m_count = 72;
    m_dataPhase = 1;
    // Send command packet.
    blfwk::CommandPacket cmdPacket1(kCommandTag_GenerateKeyBlob, kCommandFlag_None, 0, m_count, m_dataPhase);
    const uint8_t *responsePacket = cmdPacket1.sendCommandGetResponse(device);

    const generate_key_blob_response_packet_t *packet =
        reinterpret_cast<const generate_key_blob_response_packet_t *>(responsePacket);
    uint32_t byteCount = m_count;
    if (processResponse(packet))
    {
        byteCount = packet->dataByteCount;

        // Receive data packets.
        blfwk::DataPacket dataPacket(dataConsumer);
        uint8_t *finalResponsePacket = dataPacket.receiveFrom(device, &byteCount, m_progress);
        processResponse(finalResponsePacket);
    }

    // Push the number of bytes transferred response value.
    m_responseValues.push_back(m_count - byteCount);

    // Format the command transfer details.
    m_responseDetails = format_string("Read %d of %d bytes.", m_count - byteCount, m_count);
}

bool GenerateKeyBlob::processResponse(const generate_key_blob_response_packet_t *packet)
{
    if (!packet)
    {
        Log::debug("processResponse: null packet\n");
        m_responseValues.push_back(kStatus_NoResponse);
        return false;
    }

    // Handle generic response, which would be returned if command is not supported.
    if (packet->commandPacket.commandTag == kCommandTag_GenericResponse)
    {
        return processResponse((const uint8_t *)packet);
    }

    if (packet->commandPacket.commandTag != kCommandTag_GenerateKeyBlobResponse)
    {
        Log::error("Error: expected kCommandTag_GenerateKeyBlob (0x%x), received 0x%x\n",
                   kCommandTag_GenerateKeyBlobResponse, packet->commandPacket.commandTag);
        return false;
    }
    if (packet->status != kStatus_Success)
    {
        // Set the status in the response vector.
        // If status is OK, this push will be done by final response processing
        m_responseValues.push_back(packet->status);
        return false;
    }

    Log::info("Successful response to command '%s'\n", getName().c_str());

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// FillMemory command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool FillMemory::init()
{
    if ((getArgCount() < 4) || (getArgCount() > 5))
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_startAddress))
    {
        return false; // invalid 'addr' parameter
    }
    if (!utils::stringtoui(getArg(2), m_byteCount))
    {
        return false; // invalid 'byte_count' parameter
    }
    if (!utils::stringtoui(getArg(3), m_patternWord))
    {
        return false; // invalid 'pattern' parameter
    }

    if ((getArgCount() == 5) && (getArg(4) != "word"))
    {
        unsigned char b1 = (unsigned char)((m_patternWord >> 8) & 0xff);
        unsigned char b0 = (unsigned char)(m_patternWord & 0xff);

        if (getArg(4) == "byte")
        {
            // Replicate byte pattern in word.
            m_patternWord = b0 + (b0 << 8) + (b0 << 16) + (b0 << 24);
        }
        else if (getArg(4) == "short")
        {
            // Replicate short pattern in word.
            m_patternWord = b0 + (b1 << 8) + (b0 << 16) + (b1 << 24);
        }
        else
        {
            return false; // unknown pattern size argument
        }
    }

    return true;
}

// See host_command.h for documentation of this method.
void FillMemory::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_FillMemory, kCommandFlag_None, m_startAddress, m_byteCount,
                                   m_patternWord);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// ReceiveSbFile command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool ReceiveSbFile::init()
{
    if (getArgCount() != 2)
    {
        return false;
    }
    m_dataFile = getArg(1);
    return true;
}

// See host_command.h for documentation of this method.
void ReceiveSbFile::sendTo(Packetizer &device)
{
    DataPacket::FileDataProducer dataProducer;
    if (!dataProducer.init(m_dataFile, 0))
    {
        return;
    }

    // Get target bootloader data packet size.
    uint32_t packetSizeInBytes;
    GetProperty getPacketSize(kProperty_MaxPacketSize, 0 /*Not used*/);
    getPacketSize.sendTo(device);
    uint32_t fw_status = getPacketSize.getResponseValues()->at(0);
    if (fw_status != kStatus_Success)
    {
        // Failed to get data packet size.
        Log::warning("Warning: Failed to get packet size. Using default size(%d)", kMinPacketBufferSize);
        packetSizeInBytes = kMinPacketBufferSize; // No property. Use default packet size.
    }
    else
    {
        packetSizeInBytes = getPacketSize.getResponseValues()->at(1);
        if (packetSizeInBytes > device.getMaxPacketSize())
        {
            Log::error("Error: Packet size(%d) is bigger than max supported size(%d).", packetSizeInBytes,
                       kMaxHostPacketSize);
            return;
        }
    }

    // Send command packet.
    uint32_t bytesToWrite = dataProducer.getDataSize();
    uint32_t bytesWritten;
    blfwk::CommandPacket cmdPacket(kCommandTag_ReceiveSbFile, kCommandFlag_HasDataPhase, bytesToWrite);
    if (!processResponse(cmdPacket.sendCommandGetResponse(device)))
    {
        m_responseDetails = format_string("Wrote 0 of %d bytes.", bytesToWrite);
        return;
    }

    // Pop the initial (successful) generic response value.
    if (m_responseValues.size())
    {
        m_responseValues.pop_back();
    }

    // Send data packets.
    device.setAbortEnabled(true);
    blfwk::DataPacket dataPacket(&dataProducer, packetSizeInBytes);
    processResponse(dataPacket.sendTo(device, &bytesWritten, m_progress));
    device.setAbortEnabled(false);

    // Format the command transfer details.
    m_responseDetails = format_string("Wrote %d of %d bytes.", bytesWritten, bytesToWrite);
}

////////////////////////////////////////////////////////////////////////////////
// Execute command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool Execute::init()
{
    if (getArgCount() != 4)
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_jumpAddress))
    {
        return false;
    }

    if (!utils::stringtoui(getArg(2), m_wordArgument))
    {
        return false;
    }

    if (!utils::stringtoui(getArg(3), m_stackpointer))
    {
        return false;
    }
    return true;
}

// See host_command.h for documentation of this method.
void Execute::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_Execute, kCommandFlag_None, m_jumpAddress, m_wordArgument,
                                   m_stackpointer);
    const uint8_t *responsePacket = cmdPacket.sendCommandGetResponse(device);
    if (responsePacket)
    {
        processResponse(responsePacket);
    }
    else
    {
        Log::warning("Ignoring missing response from execute command.\n");
        this->m_responseValues.push_back(kStatus_Success);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Call command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool Call::init()
{
    if (getArgCount() != 3)
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_callAddress))
    {
        return false;
    }
    if (!utils::stringtoui(getArg(2), m_wordArgument))
    {
        return false;
    }

    return true;
}

// See host_command.h for documentation of this method.
void Call::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_Call, kCommandFlag_None, m_callAddress, m_wordArgument);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// FlashSecurityDisable command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool FlashSecurityDisable::init()
{
    if (getArgCount() != 2)
    {
        return false;
    }
    if (getArg(1).length() != 16)
    {
        return false;
    }

    // String must be hex digits with no leading 0x.
    char *endPtr;
    m_keyLow = strtoul(getArg(1).substr(0, 8).c_str(), &endPtr, 16);
    if ((endPtr == NULL) || (*endPtr != 0))
    {
        return false;
    }
    m_keyHigh = strtoul(getArg(1).substr(8, 8).c_str(), &endPtr, 16);
    if ((endPtr == NULL) || (*endPtr != 0))
    {
        return false;
    }

    return true;
}

// See host_command.h for documentation of this method.
void FlashSecurityDisable::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_FlashSecurityDisable, kCommandFlag_None, m_keyLow, m_keyHigh);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// FlashProgramOnce command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool FlashProgramOnce::init()
{
    if ((getArgCount() != 4) && (getArgCount() != 5) && (getArgCount() != 6))
    {
        return false;
    }

    if ((getArg(3).length() != 8) && (getArg(3).length() != 16))
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_index))
    {
        return false;
    }
    if (!utils::stringtoui(getArg(2), m_byteCount))
    {
        return false;
    }

    if ((m_byteCount != 4) && (m_byteCount != 8))
    {
        return false;
    }

    char *endPtr;
    m_dataHigh = strtoul(getArg(3).substr(0, 8).c_str(), &endPtr, 16);
    if ((endPtr == NULL) || (*endPtr != 0))
    {
        return false;
    }

    if (m_byteCount == 8)
    {
        m_dataLow = strtoul(getArg(3).substr(8, 8).c_str(), &endPtr, 16);
        if ((endPtr == NULL) || (*endPtr != 0))
        {
            return false;
        }
    }

    if (getArgCount() == 5)
    {
        if ((getArg(4) == "LSB") || (getArg(4) == "lsb"))
        {
            m_lsb = true;
        }
        else if ((getArg(4) == "MSB") || (getArg(4) == "msb"))
        {
            m_lsb = false;
        }
        else if (strcmp(getArg(4).c_str(), "lock") == 0)
        {
            m_index |= (1ul << 24);
        }
        else
        {
            return false;
        }
    }
    else
    {
        m_lsb = true;
    }
    return true;
}

// See host_command.h for documentation of this method.
void FlashProgramOnce::sendTo(Packetizer &device)
{
    uint32_t firstWord, secondWord;

    if (m_byteCount == 4)
    {
        if (m_lsb)
        {
            firstWord = ENDIAN_HOST_TO_LITTLE_U32(m_dataHigh);
        }
        else
        {
            firstWord = ENDIAN_HOST_TO_BIG_U32(m_dataHigh);
        }
        blfwk::CommandPacket cmdPacket(kCommandTag_FlashProgramOnce, kCommandFlag_None, m_index, m_byteCount,
                                       firstWord);
        processResponse(cmdPacket.sendCommandGetResponse(device));
    }
    else
    {
        if (m_lsb)
        {
            firstWord = ENDIAN_HOST_TO_LITTLE_U32(m_dataLow);
            secondWord = ENDIAN_HOST_TO_LITTLE_U32(m_dataHigh);
        }
        else
        {
            firstWord = ENDIAN_HOST_TO_BIG_U32(m_dataHigh);
            secondWord = ENDIAN_HOST_TO_BIG_U32(m_dataLow);
        }
        blfwk::CommandPacket cmdPacket(kCommandTag_FlashProgramOnce, kCommandFlag_None, m_index, m_byteCount, firstWord,
                                       secondWord);
        processResponse(cmdPacket.sendCommandGetResponse(device));
    }
}

////////////////////////////////////////////////////////////////////////////////
// FlashReadOnce command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool FlashReadOnce::init(void)
{
    if (getArgCount() != 3)
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_index))
    {
        return false;
    }
    if (!utils::stringtoui(getArg(2), m_byteCount))
    {
        return false;
    }

    if (m_byteCount != 4 && m_byteCount != 8)
    {
        return false;
    }

    return true;
}

// See host_command.h for documentation of this method.
void FlashReadOnce::sendTo(Packetizer &device)
{
    // Command Phase
    blfwk::CommandPacket cmdPacket(kCommandTag_FlashReadOnce, kCommandFlag_None, m_index, m_byteCount);
    const uint8_t *responsePacket = cmdPacket.sendCommandGetResponse(device);

    const flash_read_once_response_packet_t *packet =
        reinterpret_cast<const flash_read_once_response_packet_t *>(responsePacket);
    processResponse(packet);
}

// See host_command.h for documentation of this method.
bool FlashReadOnce::processResponse(const flash_read_once_response_packet_t *packet)
{
    if (!packet)
    {
        Log::debug("processResponse: null packet\n");
        m_responseValues.push_back(kStatus_NoResponse);
        return false;
    }

    // Handle generic response, which would be returned if command is not supported.
    if (packet->commandPacket.commandTag == kCommandTag_GenericResponse)
    {
        return processResponse((const uint8_t *)packet);
    }

    if (packet->commandPacket.commandTag != kCommandTag_FlashReadOnceResponse)
    {
        Log::error("Error: expected kCommandTag_FlashReadOnceResponse (0x%x), received 0x%x\n",
                   kCommandTag_FlashReadOnceResponse, packet->commandPacket.commandTag);
        m_responseValues.push_back(kStatus_UnknownCommand);
        return false;
    }

    // Set the status in the response vector.
    m_responseValues.push_back(packet->status);

    if (packet->status != kStatus_Success)
    {
        return false;
    }

    Log::debug("Successful response to command '%s'\n", getName().c_str());

    // Currently, no properties have a data phase.
    assert(!(packet->commandPacket.flags & kCommandFlag_HasDataPhase));

    // All properties have at least one response word.
    // Attention: parameterCount = 1(response status) + response words
    m_responseValues.push_back(packet->byteCount);

    // two parameters(status and byte count) should be excluded from the parameterCount
    for (uint8_t i = 0; i < (packet->commandPacket.parameterCount - 2); ++i)
    {
        m_responseValues.push_back(packet->data[i]);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// FlashReadResource command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool FlashReadResource::init()
{
    if ((getArgCount() < 4) || (getArgCount() > 5))
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_startAddress))
    {
        return false;
    }
    if (!utils::stringtoui(getArg(2), m_byteCount))
    {
        return false;
    }
    if (!utils::stringtoui(getArg(3), m_option))
    {
        return false;
    }

    // File name argument is optional - will use stdout if missing.
    if (getArgCount() == 5)
    {
        m_dataFile = getArg(4);
    }

    return true;
}

// See host_command.h for documentation of this method.
void FlashReadResource::sendTo(Packetizer &device)
{
    DataPacket::DataConsumer *dataConsumer;
    DataPacket::FileDataConsumer fileDataConsumer;
    DataPacket::StdOutDataConsumer stdoutDataConsumer;

    // Setup to write to file or stdout
    if (m_dataFile.size() > 0)
    {
        if (!fileDataConsumer.init(m_dataFile))
        {
            return;
        }
        dataConsumer = &fileDataConsumer;
    }
    else
    {
        dataConsumer = &stdoutDataConsumer;
    }

    // Send command packet.
    blfwk::CommandPacket cmdPacket(kCommandTag_FlashReadResource, kCommandFlag_None, m_startAddress, m_byteCount,
                                   m_option);
    const uint8_t *responsePacket = cmdPacket.sendCommandGetResponse(device);

    const flash_read_resource_response_packet_t *packet =
        reinterpret_cast<const flash_read_resource_response_packet_t *>(responsePacket);
    uint32_t byteCount = packet->dataByteCount;
    if (processResponse(packet))
    {
        // Receive data packets.
        blfwk::DataPacket dataPacket(dataConsumer);
        uint8_t *finalResponsePacket = dataPacket.receiveFrom(device, &byteCount, m_progress);
        processResponse(finalResponsePacket);
    }

    // Push the number of bytes transferred response value.
    m_responseValues.push_back(m_byteCount - byteCount);

    // Format the command transfer details.
    m_responseDetails = format_string("Read %d of %d bytes.", m_byteCount - byteCount, m_byteCount);
}

bool FlashReadResource::processResponse(const flash_read_resource_response_packet_t *packet)
{
    if (!packet)
    {
        Log::debug("processResponse: null packet\n");
        m_responseValues.push_back(kStatus_NoResponse);
        return false;
    }

    // Handle generic response, which would be returned if command is not supported.
    if (packet->commandPacket.commandTag == kCommandTag_GenericResponse)
    {
        return processResponse((const uint8_t *)packet);
    }

    if (packet->commandPacket.commandTag != kCommandTag_FlashReadResourceResponse)
    {
        Log::error("Error: expected kCommandTag_FlashReadResourceResponse (0x%x), received 0x%x\n",
                   kCommandTag_FlashReadResourceResponse, packet->commandPacket.commandTag);
        return false;
    }
    if (packet->status != kStatus_Success)
    {
        // Set the status in the response vector.
        // If status is OK, this push will be done by final response processing
        m_responseValues.push_back(packet->status);
        return false;
    }

    Log::info("Successful response to command '%s'\n", getName().c_str());

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Configure QuadSpi command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool ConfigureMemory::init()
{
    if (getArgCount() != 3)
    {
        return false;
    }
    if (!utils::stringtoui(getArg(1), m_memoryId))
    {
        return false;
    }
    if (!utils::stringtoui(getArg(2), m_configBlockAddress))
    {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Configure Peripheral command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
void ConfigureMemory::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_ConfigureMemory, kCommandFlag_None, m_memoryId, m_configBlockAddress);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// ReliableUpdate command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool ReliableUpdate::init()
{
    if (getArgCount() != 2)
    {
        return false;
    }
    if (!utils::stringtoui(getArg(1), m_address))
    {
        return false;
    }

    return true;
}

// See host_command.h for documentation of this method.
void ReliableUpdate::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_ReliableUpdate, kCommandFlag_None, m_address);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

// See host_command.h for documentation of this method.
bool KeyProvisioning::init()
{
    size_t separatorIndex;

    if (getArgCount() < 2) // 1 arguement at least.
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_operation))
    {
        if (strcmp(getArg(1).c_str(), "enroll") == 0)
        {
            m_operation = kKeyProvisioning_Operation_Enroll;
        }
        else if (strcmp(getArg(1).c_str(), "set_user_key") == 0)
        {
            m_operation = kKeyProvisioning_Operation_SetUserKey;
        }
        else if (strcmp(getArg(1).c_str(), "set_key") == 0)
        {
            m_operation = kKeyProvisioning_Operation_SetIntrinsicKey;
        }
        else if (strcmp(getArg(1).c_str(), "write_key_nonvolatile") == 0)
        {
            m_operation = kKeyProvisioning_Operation_WriteNonVolatile;
        }
        else if (strcmp(getArg(1).c_str(), "read_key_nonvolatile") == 0)
        {
            m_operation = kKeyProvisioning_Operation_ReadNonVolatile;
        }
        else if (strcmp(getArg(1).c_str(), "write_key_store") == 0)
        {
            m_operation = kKeyProvisioning_Operation_WriteKeyStore;
        }
        else if (strcmp(getArg(1).c_str(), "read_key_store") == 0)
        {
            m_operation = kKeyProvisioning_Operation_ReadKeyStore;
        }
        else
        {
            return false;
        }
    }

    switch (m_operation)
    {
        case kKeyProvisioning_Operation_Enroll:
            if (getArgCount() != 2)
            {
                return false;
            }
            break;
        case kKeyProvisioning_Operation_SetUserKey:
            if (getArgCount() != 4)
            {
                return false;
            }
            if (!utils::stringtoui(getArg(2), m_type))
            {
                return false;
            }

            m_fileOrData = getArg(3);

            // Try to find the separator ','.
            separatorIndex = m_fileOrData.find(',', 0);
            if (separatorIndex != string::npos)
            {
                // If found, the left string is the byte count to write.
                if (!utils::stringtoui(
                        m_fileOrData.substr(separatorIndex + 1, m_fileOrData.length() - separatorIndex - 1), m_size))
                {
                    return false;
                }
                m_fileOrData = m_fileOrData.substr(0, separatorIndex);
            }

            if ((m_fileOrData[0] == '{') && (m_fileOrData[1] == '{'))
            {
                DataPacket::HexDataProducer hexProducer;
                // Argument string is hex data, so use hex data producer.
                if (hexProducer.initFromString(m_fileOrData) == 0)
                {
                    return false;
                }
            }
            break;
        case kKeyProvisioning_Operation_SetIntrinsicKey:
            if (getArgCount() != 4)
            {
                return false;
            }
            if (!utils::stringtoui(getArg(2), m_type))
            {
                return false;
            }
            if (!utils::stringtoui(getArg(3), m_size))
            {
                return false;
            }
            break;
        case kKeyProvisioning_Operation_WriteNonVolatile:
        case kKeyProvisioning_Operation_ReadNonVolatile:
            if ((getArgCount() != 2) && (getArgCount() != 3))
            {
                return false;
            }

            if (getArgCount() == 3)
            {
                if (!utils::stringtoui(getArg(2), m_memoryId))
                {
                    return false;
                }
            }
            else
            {
                m_memoryId = 0;
            }

            break;
        case kKeyProvisioning_Operation_WriteKeyStore:
            if (getArgCount() != 3)
            {
                return false;
            }
            m_fileOrData = getArg(2);

            // Try to find the separator ','.
            separatorIndex = m_fileOrData.find(',', 0);
            if (separatorIndex != string::npos)
            {
                // If found, the left string is the byte count to write.
                if (!utils::stringtoui(
                        m_fileOrData.substr(separatorIndex + 1, m_fileOrData.length() - separatorIndex - 1), m_size))
                {
                    return false;
                }
                m_fileOrData = m_fileOrData.substr(0, separatorIndex);
            }

            if ((m_fileOrData[0] == '{') && (m_fileOrData[1] == '{'))
            {
                DataPacket::HexDataProducer hexProducer;
                // Argument string is hex data, so use hex data producer.
                if (hexProducer.initFromString(m_fileOrData) == 0)
                {
                    return false;
                }
            }
            break;
        case kKeyProvisioning_Operation_ReadKeyStore:
            if ((getArgCount() != 2) && (getArgCount() != 3))
            {
                return false;
            }
            if (getArgCount() == 3)
            {
                m_fileOrData = getArg(2);
            }
            break;
        default:
            return false;
    }

    return true;
}

// See host_command.h for documentation of this method.
void KeyProvisioning::sendTo(Packetizer &device)
{
    switch (m_operation)
    {
        case kKeyProvisioning_Operation_Enroll:
        {
            blfwk::CommandPacket cmdPacket(kCommandTag_KeyProvisoning, kCommandFlag_None, m_operation);
            processResponse(cmdPacket.sendCommandGetResponse(device));
            break;
        }
        case kKeyProvisioning_Operation_SetIntrinsicKey:
        {
            blfwk::CommandPacket cmdPacket(kCommandTag_KeyProvisoning, kCommandFlag_None, m_operation, m_type, m_size);
            processResponse(cmdPacket.sendCommandGetResponse(device));
        }
        case kKeyProvisioning_Operation_WriteNonVolatile:
        case kKeyProvisioning_Operation_ReadNonVolatile:
        {
            blfwk::CommandPacket cmdPacket(kCommandTag_KeyProvisoning, kCommandFlag_None, m_operation, m_memoryId);
            processResponse(cmdPacket.sendCommandGetResponse(device));
            break;
        }
        case kKeyProvisioning_Operation_SetUserKey:
        case kKeyProvisioning_Operation_WriteKeyStore:
            sendCmdAndData(device);
            break;
        case kKeyProvisioning_Operation_ReadKeyStore:
            sendCmdAndGetData(device);
            break;
        default:
            break;
    }
}

void KeyProvisioning::sendCmdAndGetData(Packetizer &device)
{
    DataPacket::DataConsumer *dataConsumer;
    DataPacket::FileDataConsumer fileDataConsumer;
    DataPacket::StdOutDataConsumer stdoutDataConsumer;

    // Setup to write to file or stdout
    if (m_fileOrData.size() > 0)
    {
        if (!fileDataConsumer.init(m_fileOrData))
        {
            return;
        }
        dataConsumer = &fileDataConsumer;
    }
    else
    {
        dataConsumer = &stdoutDataConsumer;
    }

    // Send command packet.
    blfwk::CommandPacket cmdPacket(kCommandTag_KeyProvisoning, kCommandFlag_None, m_operation);
    const uint8_t *responsePacket = cmdPacket.sendCommandGetResponse(device);

    const key_provisioning_response_packet_t *packet =
        reinterpret_cast<const key_provisioning_response_packet_t *>(responsePacket);
    uint32_t bytesToRead = m_size;
    if (processResponse(packet))
    {
        bytesToRead = packet->keyByteCount;
        m_size = bytesToRead;
        // Receive data packets.
        blfwk::DataPacket dataPacket(dataConsumer);
        uint8_t *finalResponsePacket = dataPacket.receiveFrom(device, &bytesToRead, m_progress);
        processResponse(finalResponsePacket);
    }

    // Push the number of bytes transferred response value.
    m_responseValues.push_back(m_size - bytesToRead);

    // Format the command transfer details.
    m_responseDetails = format_string("Read %d of %d bytes.", m_size - bytesToRead, m_size);
}

void KeyProvisioning::sendCmdAndData(Packetizer &device)
{
    DataPacket::HexDataProducer hexProducer;
    DataPacket::FileDataProducer fileProducer;
    DataPacket::DataProducer *dataProducer;

    if ((m_fileOrData[0] == '{') && (m_fileOrData[1] == '{'))
    {
        // Argument string is hex data, so use hex data producer.
        if (hexProducer.initFromString(m_fileOrData) == 0)
        {
            return;
        }
        dataProducer = &hexProducer;
    }
    else
    {
        // Argument string is file name, so use file data producer.
        if (!fileProducer.init(m_fileOrData, m_size))
        {
            return;
        }
        dataProducer = &fileProducer;
    }

    // Get target bootloader data packet size.
    uint32_t packetSizeInBytes;
    GetProperty getPacketSize(kProperty_MaxPacketSize, 0 /*Not used*/);
    getPacketSize.sendTo(device);
    uint32_t fw_status = getPacketSize.getResponseValues()->at(0);
    if (fw_status != kStatus_Success)
    {
        // Failed to get data packet size.
        Log::warning("Warning: Failed to get packet size. Using default size(%d)", kMinPacketBufferSize);
        packetSizeInBytes = kMinPacketBufferSize; // No property. Use default packet size.
    }
    else
    {
        packetSizeInBytes = getPacketSize.getResponseValues()->at(1);
        if (packetSizeInBytes > device.getMaxPacketSize())
        {
            Log::error("Error: Packet size(%d) is bigger than max supported size(%d).", packetSizeInBytes,
                       kMaxHostPacketSize);
            return;
        }
    }

    // Send command packet.
    uint32_t bytesToWrite = dataProducer->getDataSize();
    uint32_t bytesWritten;
    blfwk::CommandPacket cmdPacket(kCommandTag_KeyProvisoning, kCommandFlag_HasDataPhase, m_operation, m_type,
                                   bytesToWrite);
    const uint8_t *responsePacket = cmdPacket.sendCommandGetResponse(device);

    const key_provisioning_response_packet_t *packet =
        reinterpret_cast<const key_provisioning_response_packet_t *>(responsePacket);
    if (processResponse(packet))
    {
        if (packet->keyByteCount != bytesToWrite)
        {
            Log::error("Error: Incorrect key length %d(%#x) which should be %d(%#x), Abort the data phase.", m_size,
                       m_size, packet->keyByteCount, packet->keyByteCount);
            uint8_t dummy_data = 0;
            device.writePacket(&dummy_data, 0, kPacketType_Data);
            uint8_t *responsePacket;
            uint32_t responseLength;
            device.readPacket(&responsePacket, &responseLength, kPacketType_Command);
            return;
        }
    }
    else
    {
        m_responseDetails = format_string("Wrote 0 of %d bytes.", bytesToWrite);
        return;
    }

    // Pop the initial (successful) generic response value.
    if (m_responseValues.size())
    {
        m_responseValues.pop_back();
    }

    // Send data packets.
    blfwk::DataPacket dataPacket(dataProducer, packetSizeInBytes);

    processResponse(dataPacket.sendTo(device, &bytesWritten, m_progress));
}

bool KeyProvisioning::processResponse(const key_provisioning_response_packet_t *packet)
{
    if (!packet)
    {
        Log::debug("processResponse: null packet\n");
        m_responseValues.push_back(kStatus_NoResponse);
        return false;
    }

    // Handle generic response, which would be returned if command is not supported.
    if (packet->commandPacket.commandTag == kCommandTag_GenericResponse)
    {
        return processResponse((const uint8_t *)packet);
    }

    if (packet->commandPacket.commandTag != kCommandTag_KeyProvisonResponse)
    {
        Log::error("Error: expected kCommandTag_KeyProvisonResponse (0x%x), received 0x%x\n",
                   kCommandTag_KeyProvisonResponse, packet->commandPacket.commandTag);
        return false;
    }
    if (packet->status != kStatus_Success)
    {
        // Set the status in the response vector.
        // If status is OK, this push will be done by final response processing
        m_responseValues.push_back(packet->status);
        return false;
    }

    Log::info("Successful response to command '%s'\n", getName().c_str());

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// FlashImage command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool FlashImage::init()
{
    if (getArgCount() != 2 && getArgCount() != 3 && getArgCount() != 4)
    {
        return false;
    }

    m_fileName = getArg(1);

    if (getArgCount() == 3)
    {
        string strDoEraseOpt = getArg(2);
        if (strDoEraseOpt == "erase")
        {
            m_doEraseOpt = true;
            m_memoryId = kMemoryInternal;
        }
        else if (strDoEraseOpt == "none")
        {
            m_doEraseOpt = false;
            m_memoryId = kMemoryInternal;
        }
        else
        {
            // If not 'erase' or 'none'. Try to parse it as memoryId
            // If arg2 is a valid number, it is valid memoryId,
            // otherwise is invalid arg and return false.
            if (!utils::stringtoui(getArg(2), m_memoryId))
            {
                return false;
            }
            // Use 0 for flashing image to internal 4G memory including mapped memory, such as QSPI
            if (GROUPID(m_memoryId) == kGroup_Internal)
            {
                m_memoryId = kMemoryInternal;
            }
            // NO erase operation by default.
            m_doEraseOpt = false;
        }
    }
    else if (getArgCount() == 4)
    {
        string strDoEraseOpt = getArg(2);
        if (strDoEraseOpt == "erase")
        {
            m_doEraseOpt = true;
        }
        else if (strDoEraseOpt == "none")
        {
            m_doEraseOpt = false;
        }
        else
        {
            return false;
        }

        if (!utils::stringtoui(getArg(3), m_memoryId))
        {
            return false;
        }
        // Use 0 for flashing image to internal 4G memory including mapped memory, such as QSPI
        if (GROUPID(m_memoryId) == kGroup_Internal)
        {
            m_memoryId = kMemoryInternal;
        }
    }

    return true;
}

// See host_command.h for documentation of this method.
void FlashImage::sendTo(Packetizer &device)
{
    uint32_t fw_status;
    DataSource *dataSource;

    try
    {
        m_sourceFile = SourceFile::openFile(m_fileName);
    }
    catch (exception &e)
    {
        Log::error("Error: %s", e.what());
        return;
    }
    if (m_sourceFile->getFileType() == SourceFile::source_file_t::kBinarySourceFile)
    {
        Log::error("Error: please use write-memory command for binary file downloading.\n");
        return;
    }

    m_sourceFile->open();
    dataSource = m_sourceFile->createDataSource();

    m_progress->m_segmentCount = dataSource->getSegmentCount();

    if (m_doEraseOpt)
    {
        for (uint32_t index = 0; index < dataSource->getSegmentCount(); ++index)
        {
            DataSource::Segment *segment = dataSource->getSegmentAt(index);

            // Align the start address and length to a sector boundary
            uint32_t alignedStart = segment->getBaseAddress() & (~(MinEraseAlignment - 1));
            uint32_t alignedLength =
                ((segment->getBaseAddress() + segment->getLength() + MinEraseAlignment) & (~(MinEraseAlignment - 1))) -
                alignedStart;

            // Do erase operation to erase the necessary flash.
            FlashEraseRegion cmd(alignedStart, alignedLength, m_memoryId);
            cmd.sendTo(device);

            // Print and check the command response values.
            fw_status = cmd.getResponseValues()->at(0);
            if (fw_status != kStatus_Success)
            {
                m_responseValues.push_back(fw_status);
                delete dataSource;
                return;
            }
        }
    }

    for (uint32_t index = 0; index < dataSource->getSegmentCount(); ++index)
    {
        DataSource::Segment *segment = dataSource->getSegmentAt(index);

        // Write the file to the base address.
        Log::info("Wrote %d bytes to address %#x\n", segment->getLength(), segment->getBaseAddress());
        WriteMemory cmd(segment, m_memoryId);

        m_progress->m_segmentIndex = index + 1;
        cmd.registerProgress(m_progress);

        cmd.sendTo(device);

        // Print and check the command response values.
        fw_status = cmd.getResponseValues()->at(0);
        if (fw_status != kStatus_Success)
        {
            m_responseValues.push_back(fw_status);
            delete dataSource;
            return;
        }
    }

    m_responseValues.push_back(fw_status);
    delete dataSource;
    m_sourceFile->close();
}

////////////////////////////////////////////////////////////////////////////////
// Configure I2C command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool ConfigureI2c::init()
{
    if (getArgCount() > 1)
    {
        i2cAddress = (uint8_t)strtoul(getArg(1).c_str(), NULL, 16);

        if (i2cAddress > 0x7F)
        {
            i2cAddress &= 0x7F;
            Log::info("Only 7-bit i2c address is supported, so the effective value is 0x%x\n", i2cAddress);
        }

        if (getArgCount() > 2)
        {
            int32_t i2cSpeed = atoi(getArg(2).c_str());
            if (i2cSpeed <= 0)
                return false;

            i2cSpeedKHz = i2cSpeed;
        }
    }

    return true;
}

// See host_command.h for documentation of this method.
void ConfigureI2c::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_ConfigureI2c, kCommandFlag_None, i2cAddress, i2cSpeedKHz);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// Configure SPI command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool ConfigureSpi::init()
{
    if ((getArgCount() > 1))
    {
        int32_t spiSpeed = atoi(getArg(1).c_str());
        if (spiSpeed <= 0)
            return false;

        spiSpeedKHz = spiSpeed;

        if (getArgCount() > 2)
        {
            spiPolarity = (BusPal::spi_clock_polarity_t)atoi(getArg(2).c_str());

            if (getArgCount() > 3)
            {
                spiPhase = (BusPal::spi_clock_phase_t)atoi(getArg(3).c_str());

                if (getArgCount() > 4)
                {
                    if (!strcmp(getArg(4).c_str(), "lsb"))
                    {
                        spiDirection = BusPal::kSpiLsbFirst;
                    }
                    else if (!strcmp(getArg(4).c_str(), "msb"))
                    {
                        spiDirection = BusPal::kSpiMsbFirst;
                    }
                }
            }
        }
    }

    return true;
}

// See host_command.h for documentation of this method.
void ConfigureSpi::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_ConfigureSpi, kCommandFlag_None, spiSpeedKHz, spiPolarity, spiPhase,
                                   spiDirection);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// Configure CAN command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool ConfigureCan::init()
{
    if ((getArgCount() > 1))
    {
        canSpeed = atoi(getArg(1).c_str());
        if (canSpeed > 4)
            return false;

        if (getArgCount() > 2)
        {
            canTxid = strtoul(getArg(2).c_str(), NULL, 16) & 0x7ff;
        }

        if (getArgCount() > 3)
        {
            canRxid = strtoul(getArg(3).c_str(), NULL, 16) & 0x7ff;
        }
    }

    return true;
}

// See host_command.h for documentation of this method.
void ConfigureCan::sendTo(Packetizer &device)
{
    blfwk::CommandPacket cmdPacket(kCommandTag_ConfigureCan, kCommandFlag_None, canSpeed, canTxid, canRxid);
    processResponse(cmdPacket.sendCommandGetResponse(device));
}

////////////////////////////////////////////////////////////////////////////////
// ListMemory command
////////////////////////////////////////////////////////////////////////////////

// See host_command.h for documentation of this method.
bool ListMemory::init()
{
    if (getArgCount() != 1)
    {
        return false;
    }
    return true;
}

// See host_command.h for documentation of this method.
void ListMemory::sendTo(Packetizer &device)
{
    uint32_t fw_status;
    uint32_t firstStartAddress = 0;

    // List internal on-chip FLASH regions.
    Log::info("Internal Flash:\n");
    for (uint32_t index = 0;; index++)
    {
        // Get internal on-chip FLASH start address.
        uint32_t startAddress;
        GetProperty getFlashStartAddress(kProperty_FlashStartAddress, index);
        getFlashStartAddress.sendTo(device);
        fw_status = getFlashStartAddress.getResponseValues()->at(0);
        if (fw_status == kStatus_UnknownProperty)
        {
            // UnknownProperty means that there is no internal flash on current device.
            Log::info("  No Internal Flash available\n");
            break;
        }
        else if (fw_status != kStatus_Success)
        {
            // Failed to communicate with the device.
            m_responseValues.push_back(fw_status);
            return;
        }
        startAddress = getFlashStartAddress.getResponseValues()->at(1);

        if (index == 0)
        {
            firstStartAddress = startAddress;
        }
        else if (startAddress == firstStartAddress)
        {
            // If a flash region's start address is the same as the first region.
            // That means all flash regions are listed.
            // Then break the for-loop.
            break;
        }

        // Get internal on-chip FLASH size.
        uint32_t flashSize;
        GetProperty getFlashSize(kProperty_FlashSizeInBytes, index);
        getFlashSize.sendTo(device);
        fw_status = getFlashSize.getResponseValues()->at(0);
        if (fw_status != kStatus_Success)
        {
            // Failed to communicate with the device.
            m_responseValues.push_back(fw_status);
            return;
        }
        flashSize = getFlashSize.getResponseValues()->at(1);

        // Get internal on-chip FLASH sector size.
        uint32_t sectorSize;
        GetProperty getFlashSectorSize(kProperty_FlashSectorSize, index);
        getFlashSectorSize.sendTo(device);
        fw_status = getFlashSectorSize.getResponseValues()->at(0);
        if (fw_status != kStatus_Success)
        {
            // Failed to communicate with the device.
            m_responseValues.push_back(fw_status);
            return;
        }
        sectorSize = getFlashSectorSize.getResponseValues()->at(1);

        Log::info("  Region %d: 0x%08x - 0x%08x; Total size: %s  Sector size: %s\n", index, startAddress,
                  startAddress + flashSize - 1, utils::scale_bytes(flashSize).c_str(),
                  utils::scale_bytes(sectorSize).c_str());
    }

    // List internal on-chip RAM regions.
    Log::info("Internal RAM:\n");
    for (uint32_t index = 0;; index++)
    {
        uint32_t startAddress;
        // Get internal on-chip RAM start address.
        GetProperty getRamStartAddress(kProperty_RAMStartAddress, index);
        getRamStartAddress.sendTo(device);
        fw_status = getRamStartAddress.getResponseValues()->at(0);
        if (fw_status == kStatus_UnknownProperty)
        {
            // UnknownProperty means that there is no internal ram on current device.
            Log::info("  No Internal RAM available\n");
            break;
        }
        if (fw_status != kStatus_Success)
        {
            // Failed to communicate with the device.
            m_responseValues.push_back(fw_status);
            return;
        }
        startAddress = getRamStartAddress.getResponseValues()->at(1);

        if (index == 0)
        {
            firstStartAddress = startAddress;
        }
        else if (startAddress == firstStartAddress)
        {
            // If a RAM region's start address is the same as the first region.
            // That means all RAM regions are listed.
            // Then break the for-loop.
            break;
        }

        uint32_t ramSize;
        GetProperty getRamSize(kProperty_RAMSizeInBytes, index);
        getRamSize.sendTo(device);
        fw_status = getRamSize.getResponseValues()->at(0);
        if (fw_status != kStatus_Success)
        {
            // Failed to communicate with the device.
            m_responseValues.push_back(fw_status);
            return;
        }

        ramSize = getRamSize.getResponseValues()->at(1);

        Log::info("  Region %d: 0x%08x - 0x%08x; Total size: %s\n", index, startAddress, startAddress + ramSize - 1,
                  utils::scale_bytes(ramSize).c_str());
    }

    // List external memories.
    // Inject the get-property command to get the target version.
    GetProperty getVersion(kProperty_CurrentVersion);
    getVersion.sendTo(device);
    fw_status = getVersion.getResponseValues()->at(0);
    // Check the command status
    if (fw_status != kStatus_Success)
    {
        m_responseValues.push_back(fw_status);
        return;
    }
    uint32_t version = getVersion.getResponseValues()->at(1);
    version = version & 0x00FFFFFF; // Mask the char 'K'

    // Bootloader 2.0.0 and previous versions only support internal and QSPI0.
    if (version <= 0x20000 /*KBL2.0.0*/)
    {
        GetProperty getQspi(kProperty_ExernalMemoryAttributes, kMemory_QuadSpi.memoryId);
        getQspi.sendTo(device);
        fw_status = getQspi.getResponseValues()->at(0);
        if (fw_status == kStatus_UnknownProperty)
        {
            // UnknownProperty means that no external memories are supported by current device.
        }
        else if (fw_status == kStatus_InvalidArgument)
        {
            // QSPI0 is not supported.
        }
        else if (fw_status == 405 /*kStatus_QspiNotConfigured*/)
        {
            Log::info("%s:\n  %s\n", kMemory_QuadSpi.description, getStatusMessage(fw_status).c_str());
        }
        else if (fw_status != kStatus_Success)
        {
            // Failed to get property of the device memory.
            m_responseValues.push_back(fw_status);
            return;
        }
        else // fw_status == kStatus_Success
        {
            uint32_t propertyTags = getQspi.getResponseValues()->at(1);
            Log::info("%s:\n", kMemory_QuadSpi.description);
            if (propertyTags & (1 << (kExternalMemoryPropertyTag_StartAddress - 1)))
            {
                Log::info("  Start Address = 0x%08x", getQspi.getResponseValues()->at(2));
            }
            if (propertyTags & (1 << (kExternalMemoryPropertyTag_MemorySizeInKbytes - 1)))
            {
                Log::info("  Total Size = %s", utils::scale_bytes(getQspi.getResponseValues()->at(3) * 1024).c_str());
            }
            if (propertyTags & (1 << (kExternalMemoryPropertyTag_PageSize - 1)))
            {
                Log::info("  Page Size = %s", utils::scale_bytes(getQspi.getResponseValues()->at(4)).c_str());
            }
            if (propertyTags & (1 << (kExternalMemoryPropertyTag_SectorSize - 1)))
            {
                Log::info("  Sector Size = %s", utils::scale_bytes(getQspi.getResponseValues()->at(5)).c_str());
            }
            if (propertyTags & (1 << (kExternalMemoryPropertyTag_BlockSize - 1)))
            {
                Log::info("  Block Size = %s", utils::scale_bytes(getQspi.getResponseValues()->at(6)).c_str());
            }
            Log::info("\n");
        }
    }
    else
    {
        for (MemoryArray::const_iterator it = kMemories.begin(); it != kMemories.end(); ++it)
        {
            GetProperty getExernalAttri(kProperty_ExernalMemoryAttributes, it->memoryId);
            getExernalAttri.sendTo(device);
            fw_status = getExernalAttri.getResponseValues()->at(0);
            if (fw_status == kStatus_UnknownProperty)
            {
                // UnknownProperty means that no external memories are supported by current device.
                break;
            }
            else if (fw_status == kStatus_InvalidArgument)
            {
                // Current memory type is not supported by the device, skip to next external memory.
                continue;
            }
            // Only un-configured QSPI will return this status.
            else if (fw_status == 405 /*kStatus_QspiNotConfigured*/)
            {
                Log::info("%s:\n  %s\n", it->description, getStatusMessage(fw_status).c_str());
                continue;
            }
            // Other un-configured external memory will return this status.
            else if (fw_status == kStatusMemoryNotConfigured)
            {
                Log::info("%s:\n  %s\n", it->description, getStatusMessage(fw_status).c_str());
                continue;
            }
            else if (fw_status != kStatus_Success)
            {
                // Failed to get property of the device memory.
                m_responseValues.push_back(fw_status);
                return;
            }
            else // fw_status == kStatus_Success
            {
                uint32_t propertyTags = getExernalAttri.getResponseValues()->at(1);
                Log::info("%s:\n", it->description);
                if (propertyTags & (1 << (kExternalMemoryPropertyTag_StartAddress - 1)))
                {
                    Log::info("  Start Address = 0x%08x", getExernalAttri.getResponseValues()->at(2));
                }
                if (propertyTags & (1 << (kExternalMemoryPropertyTag_MemorySizeInKbytes - 1)))
                {
                    Log::info("  Total Size = %s",
                              utils::scale_bytes(getExernalAttri.getResponseValues()->at(3) * 1024).c_str());
                }
                if (propertyTags & (1 << (kExternalMemoryPropertyTag_PageSize - 1)))
                {
                    Log::info("  Page Size = %s",
                              utils::scale_bytes(getExernalAttri.getResponseValues()->at(4)).c_str());
                }
                if (propertyTags & (1 << (kExternalMemoryPropertyTag_SectorSize - 1)))
                {
                    Log::info("  Sector Size = %s",
                              utils::scale_bytes(getExernalAttri.getResponseValues()->at(5)).c_str());
                }
                if (propertyTags & (1 << (kExternalMemoryPropertyTag_BlockSize - 1)))
                {
                    Log::info("  Block Size = %s",
                              utils::scale_bytes(getExernalAttri.getResponseValues()->at(6)).c_str());
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
