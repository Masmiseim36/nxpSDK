/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_semc.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//!@brief SEMC Misc Info
enum
{
    kSemcIpCommandMagicKey = 0xA55AU,
};

//!@brief SEMC AXI queue instance
enum
{
    kSemcAxiCommandQueueInstance_A = 0x0,
    kSemcAxiCommandQueueInstance_B = 0x1,
};

//!@brief SEMC AXI queue weight setting
typedef struct _semc_axi_queue_weigth_config
{
    // There are Queue A and Queue B
    // Queue 0/A can be used for All Device
    // Queue 1/B can be used for SDRAM device only)
    uint8_t qos;
    uint8_t aging;
    uint8_t pageHit;
    uint8_t slaveHit_noswitch;
    uint8_t slaveHit_switch;
    uint8_t bankRotation;
    uint8_t reserved0[2];
} semc_axi_queue_weigth_config_t;

////////////////////////////////////////////////////////////////////////////////
// Local prototypes
////////////////////////////////////////////////////////////////////////////////
//!@brief Reset internal logical of SEMC module
static void semc_sw_reset(SEMC_Type *base);
//!@brief Configure macthed data size of SEMC IP command
static status_t semc_set_macthed_ipg_command_data_size(uint8_t dataPortSize);
//!@brief Wait the execution of SEMC IP command to be done
static status_t semc_ipg_command_wait_for_done(void);
//!@brief Configure AXI queue of SEMC
static void semc_axi_queue_config(uint32_t instance, semc_axi_queue_weigth_config_t *queueConfig);

//!@brief Convert memory size to ms parameter for BRx configuration
static status_t semc_convert_memory_size_to_ms(uint32_t memorySize, uint8_t *ms);
//!@brief Convert burst length to bl parameter for NORCR0/NANDCR0 configuration
static status_t semc_convert_burst_length_to_bl(uint32_t burstLength, uint8_t *bl);

//!@brief Configure NOR flash control registers
static status_t semc_config_nor_flash_control_registers(semc_mem_config_t *config);
//!@brief Configure NAND flash control registers
static status_t semc_config_nand_flash_control_registers(semc_mem_config_t *config);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

const semc_axi_queue_weigth_config_t s_semcAxiQueueWeightConfig = {
    .qos = 0x5U,
    .aging = 0x8U,
    .slaveHit_noswitch = 0x40U,
    .slaveHit_switch = 0x40U
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

//!@brief Reset internal logical of SEMC module
static void semc_sw_reset(SEMC_Type *base)
{
    base->MCR |= SEMC_MCR_SWRST_MASK;
    while (base->MCR & SEMC_MCR_SWRST_MASK)
    {
    }
    __DSB();
    __ISB();
}

//!@brief Wait the SEMC module to be in IDLE state
status_t semc_wait_for_idle(void)
{
    SEMC_Type *base = SEMC;

    // IDLE bit indicates whether SEMC is in IDLE state.
    while (!(base->STS0 & SEMC_STS0_IDLE_MASK))
    {
        // Check whether AXI command execution is timeout
        if (base->INTR & SEMC_INTR_AXICMDERR_MASK)
        {
            base->INTR |= SEMC_INTR_AXICMDERR_MASK;
            return kStatus_SEMC_AxiCommandExecutionError;
        }
    }

    return kStatus_Success;
}

//!@brief Configure data size of SEMC IP command
status_t semc_ipg_command_set_data_size(uint8_t sizeInBytes)
{
    SEMC_Type *base = SEMC;

    if ((sizeInBytes > kSemcMiscProperty_MaxIpCommandDataSizeInBytes) ||
       (sizeInBytes < kSemcMiscProperty_MinIpCommandDataSizeInBytes))
    {
        return kStatus_SEMC_InvalidIpcmdDataSize;
    }
    // Set data size
    /* Note: It is better to set data size as the device data port width when transfering
     *    device command data. but for device memory data transfer, it can be set freely.
     * Note: If the data size is greater than data port width, for example, datsz = 4, data port = 16bit,
     *    then the 4-byte data transfer will be split into two 2-byte transfer, the slave address
     *    will be switched automatically according to connected device type*/
    base->IPCR1 = SEMC_IPCR1_DATSZ(sizeInBytes);

    // Set byte mask (optional)
    base->IPCR2 &= (uint32_t)(~(SEMC_IPCR2_BM3_MASK | SEMC_IPCR2_BM2_MASK | SEMC_IPCR2_BM1_MASK));
    if (sizeInBytes < 4)
    {
        base->IPCR2 |= SEMC_IPCR2_BM3_MASK;
    }
    if (sizeInBytes < 3)
    {
        base->IPCR2 |= SEMC_IPCR2_BM2_MASK;
    }
    if (sizeInBytes < 2)
    {
        base->IPCR2 |= SEMC_IPCR2_BM1_MASK;
    }

    return kStatus_Success;
}

//!@brief Get data size of SEMC IP command
uint8_t semc_ipg_command_get_data_size(void)
{
    SEMC_Type *base = SEMC;

    uint8_t dataSize = (uint8_t)((base->IPCR1 & SEMC_IPCR1_DATSZ_MASK) >> SEMC_IPCR1_DATSZ_SHIFT);

    if ((dataSize == 0) || (dataSize > kSemcMiscProperty_MaxIpCommandDataSizeInBytes))
    {
        dataSize = kSemcMiscProperty_MaxIpCommandDataSizeInBytes;
    }

    return dataSize;
}

//!@brief Configure macthed data size of SEMC IP command
static status_t semc_set_macthed_ipg_command_data_size(uint8_t dataPortSize)
{
    status_t status = kStatus_Success;

    // Re-cover port size
    if (dataPortSize == 8)
    {
        status = semc_ipg_command_set_data_size(1);
    }
    else if (dataPortSize == 16)
    {
        status = semc_ipg_command_set_data_size(2);
    }
    else
    {
        status = kStatus_SEMC_InvalidDataPortWidth;
    }

    return status;
}

//!@brief Wait the execution of SEMC IP command to be done
static status_t semc_ipg_command_wait_for_done(void)
{
    SEMC_Type *base = SEMC;

    // Poll status bit till command is done
    // Note: no matter whether there is error or not during command
    //  execution, the INTR[IPCMDDONE] will always be set
    while(!(base->INTR & SEMC_INTR_IPCMDDONE_MASK));

    // Clear status bit
    base->INTR |= SEMC_INTR_IPCMDDONE_MASK;

    // Check error status
    // Note: Only when command is done, then we can check error
    if (base->INTR & SEMC_INTR_IPCMDERR_MASK)
    {
        base->INTR |= SEMC_INTR_IPCMDERR_MASK;
        return kStatus_SEMC_IpCommandExecutionError;
    }

    return kStatus_Success;
}

//!@brief Write device data via SEMC IP commmand
status_t semc_ipg_command_device_write(uint32_t slaveAddress, uint16_t commandCode, uint32_t writeData, uint8_t lengthInBytes)
{
    status_t status;
    SEMC_Type *base = SEMC;

    // Configure data size if necessary
    if (semc_ipg_command_get_data_size() != lengthInBytes)
    {
        status = semc_ipg_command_set_data_size(lengthInBytes);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    // Clear status bit
    base->INTR |= SEMC_INTR_IPCMDDONE_MASK;
    // Set tx data
    base->IPTXDAT = writeData;
    // Set slave address
    base->IPCR0 = slaveAddress;
    // Set command code
    base->IPCMD = ((uint32_t)kSemcIpCommandMagicKey << 16) + commandCode;

    return semc_ipg_command_wait_for_done();
}

//!@brief Read device data via SEMC IP commmand
status_t semc_ipg_command_device_read(uint32_t slaveAddress, uint16_t commandCode, uint8_t *readoutData, uint8_t lengthInBytes)
{
    status_t status;
    SEMC_Type *base = SEMC;
    uint32_t rxdat;

    // Configure data size if necessary
    if (semc_ipg_command_get_data_size() != lengthInBytes)
    {
        status = semc_ipg_command_set_data_size(lengthInBytes);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    // Clear status bit
    base->INTR |= SEMC_INTR_IPCMDDONE_MASK;
    // Set slave address
    base->IPCR0 = slaveAddress;
    // Set command code
    base->IPCMD = ((uint32_t)kSemcIpCommandMagicKey << 16) + commandCode;
    // Command execution
    status = semc_ipg_command_wait_for_done();
    if (status != kStatus_Success)
    {
        return status;
    }
    // Get rx data
    rxdat = base->IPRXDAT;
    for (uint32_t i = 0; i < lengthInBytes; i++)
    {
        *readoutData = (uint8_t)(rxdat & 0xFFu);
        rxdat >>= 8u;
        readoutData++;
    }

    return kStatus_Success;
}

//!@brief Read memory data via SEMC IPG commmand
status_t semc_ipg_memory_read(semc_mem_config_t *config, uint8_t *readoutData, uint32_t lengthInBytes)
{
    status_t status = kStatus_Success;

    if (config->deviceMemType == kSemcDeviceMemType_NAND)
    {
        // Only commandMode is necessary for Read only Command
        uint16_t commandCode = semc_ipg_command_get_nand_code(0, 0, SEMC_IPCMD_NAND_CM_IPG_READ_ONLY); // Read Only

        while (lengthInBytes >= kSemcMiscProperty_MaxIpCommandDataSizeInBytes)
        {
            status = semc_ipg_command_device_read(config->nandMemConfig.ipgMemBaseAddress, commandCode, readoutData, kSemcMiscProperty_MaxIpCommandDataSizeInBytes);
            if (status != kStatus_Success)
            {
                break;
            }
            lengthInBytes -= kSemcMiscProperty_MaxIpCommandDataSizeInBytes;
            readoutData += kSemcMiscProperty_MaxIpCommandDataSizeInBytes;
        }
        if (lengthInBytes && (status == kStatus_Success))
        {
            status = semc_ipg_command_device_read(config->nandMemConfig.ipgMemBaseAddress, commandCode, readoutData, (uint8_t)lengthInBytes);
        }

        // Re-cover port size
        semc_set_macthed_ipg_command_data_size(config->nandMemConfig.ioPortWidth);
    }

    return status;
}

//!@brief Write memory data via SEMC IPG commmand
status_t semc_ipg_memory_write(semc_mem_config_t *config, uint8_t *writeData, uint32_t lengthInBytes)
{
    status_t status = kStatus_Success;

    if (config->deviceMemType == kSemcDeviceMemType_NAND)
    {
        // Only commandMode is necessary for Write only Command
        uint16_t commandCode = semc_ipg_command_get_nand_code(0, 0, SEMC_IPCMD_NAND_CM_IPG_WRITE_ONLY); // Write Only
        uint32_t writeDataWord;

        while (lengthInBytes >= kSemcMiscProperty_MaxIpCommandDataSizeInBytes)
        {
            writeDataWord = 0;
            for (uint32_t i = 0; i < kSemcMiscProperty_MaxIpCommandDataSizeInBytes; i++)
            {
                writeDataWord |= (uint32_t)(*(uint8_t *)(writeData + i)) << (8 * i);
            }

            status = semc_ipg_command_device_write(config->nandMemConfig.ipgMemBaseAddress, commandCode, writeDataWord, kSemcMiscProperty_MaxIpCommandDataSizeInBytes);
            if (status != kStatus_Success)
            {
                break;
            }
            lengthInBytes -= kSemcMiscProperty_MaxIpCommandDataSizeInBytes;
            writeData += kSemcMiscProperty_MaxIpCommandDataSizeInBytes;
        }
        if (lengthInBytes && (status == kStatus_Success))
        {
            writeDataWord = 0;
            for (uint32_t i = 0; i < lengthInBytes; i++)
            {
                writeDataWord |= (uint32_t)(*(uint8_t *)(writeData + i)) << (8 * i);
            }
            status = semc_ipg_command_device_write(config->nandMemConfig.ipgMemBaseAddress, commandCode, writeDataWord, (uint8_t)lengthInBytes);
        }

        // Re-cover port size
        semc_set_macthed_ipg_command_data_size(config->nandMemConfig.ioPortWidth);
    }

    return status;
}

//!@brief Read memory data via SEMC AXI commmand
status_t semc_axi_memory_read(uint32_t axiAddress, uint8_t *readoutData, uint32_t lengthInBytes)
{
    status_t status = kStatus_Success;

    memcpy(readoutData, (uint8_t *)axiAddress, lengthInBytes);
    status = semc_wait_for_idle();

    return status;
}

//!@brief Write memory data via SEMC AXI commmand
status_t semc_axi_memory_write(uint32_t axiAddress, uint8_t *writeData, uint32_t lengthInBytes)
{
    status_t status = kStatus_Success;

    memcpy((uint8_t *)axiAddress, writeData, lengthInBytes);
    status = semc_wait_for_idle();

    return status;
}

//!@brief Wait the NAND device to be ready
bool semc_is_nand_ready(void)
{
    bool isReady;
    // NARDY bit indicates NAND device Ready/WAIT# pin level.
    isReady = (SEMC->STS0 & SEMC_STS0_NARDY_MASK);

    return isReady;
}

//!@brief Get NAND command code for SEMC IP commmand
uint16_t semc_ipg_command_get_nand_code(uint8_t userCommandCode, uint8_t addressMode, uint8_t commandMode)
{
    uint16_t commandCode = 0;

    // Bit 15-8 (Command Code)
    commandCode = (uint16_t)userCommandCode << 8;
    // Bit 7-4 (Address mode)
    commandCode |= (uint8_t)(addressMode << 4);
    // Bit 3-0 (Command mode)
    commandCode |= commandMode & 0x0Fu;

    return commandCode;
}

//!@brief Convert NAND salve address for SEMC IP commmand
uint32_t semc_ipg_command_convert_nand_address(semc_mem_config_t *config, uint32_t slaveAddress)
{
    uint32_t convertedAddress;

    slaveAddress += config->nandMemConfig.ipgMemBaseAddress;

    // Note NA3.0: From SEMC IP spec, the x8/x16 conversion is done in IP level:
    //    x8:           SA[COL-1:0]=CAx, SA[31:COL]=RAx,   col=COL
    //    x16: SA[0]=0, SA[COL:1]=CAx,   SA[31:COL+1]=RAx, col=COL + 1
    //  slaveAddress is always byte address here, so we don't need to do any 
    //  conversion for x8/x16 address
    if (config->nandMemConfig.columnAddressOption == kSemcNandColumnAddressOption_SpareAreaAccess)
    {
        // Note NA3.1: When we try to access NAND spare area via IPG command (change column read)
        //   or AXI command, we have to increase col by 1 bit when configuing COL bit 
        //   of SEMC NAND controller according to SEMC IP design (see Note NA4.0). so we 
        //   need to convert NAND address here.
        uint32_t columnAddressWidth = config->nandMemConfig.columnAddressWidth;

        // convertedAddress[31:col+1] = slaveAddress[30:col].
        // convertedAddress[col] = 1'b0.
        convertedAddress = (slaveAddress >> columnAddressWidth) << (columnAddressWidth + 1);
        // convertedAddress[col-1:0] = slaveAddress[col-1:0].
        convertedAddress |= slaveAddress & (~(0xFFFFFFFFu << columnAddressWidth));
    }
    else
    {
        // convertedAddress[31:0] = slaveAddress[31:0].
        convertedAddress = slaveAddress;
    }

    return convertedAddress;
}

//!@brief Access NAND via SEMC IP commmand
status_t semc_ipg_command_nand_access(uint32_t slaveAddress, uint16_t commandCode, uint32_t writeData, uint32_t *readoutData)
{
    status_t status;
    SEMC_Type *base = SEMC;
    bool isReadCmd, isWriteCmd;
    uint8_t cmdMode;

    // Clear status bit
    base->INTR |= SEMC_INTR_IPCMDDONE_MASK;
    // Check command mode
    cmdMode = (commandCode & kSemcMiscProperty_NAND_CommandModeBitMask) >> kSemcMiscProperty_NAND_CommandModeBitShift;
    isReadCmd = (cmdMode == SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR_READ) ||
                (cmdMode == SEMC_IPCMD_NAND_CM_IPG_READ_ONLY) ||
                (cmdMode == SEMC_IPCMD_NAND_CM_AXI_CMD_ADDR_CMD_READ) ||
                (cmdMode == SEMC_IPCMD_NAND_CM_AXI_CMD_ADDR_WRITE) ||
                (cmdMode == SEMC_IPCMD_NAND_CM_IPG_CMD_READ);

    isWriteCmd = (cmdMode == SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR_WRITE) ||
                 (cmdMode == SEMC_IPCMD_NAND_CM_IPG_WRITE_ONLY) ||
                 (cmdMode == SEMC_IPCMD_NAND_CM_AXI_CMD_ADDR_CMD_READ) ||
                 (cmdMode == SEMC_IPCMD_NAND_CM_AXI_CMD_ADDR_WRITE) ||
                 (cmdMode == SEMC_IPCMD_NAND_CM_IPG_CMD_WRITE);

    // Set slave address
    base->IPCR0 = slaveAddress;
    if (isWriteCmd)
    {
        // Set tx data
        base->IPTXDAT = writeData;
    }
    // Set command code
    base->IPCMD = ((uint32_t)kSemcIpCommandMagicKey << 16) + commandCode;
    // Command execution
    status = semc_ipg_command_wait_for_done();
    if (status != kStatus_Success)
    {
        return status;
    }
    if (isReadCmd)
    {
        // Get rx data
        *readoutData = base->IPRXDAT;
    }

    return kStatus_Success;
}

//!@brief Convert NOR salve address for SEMC IP commmand
uint32_t semc_ipg_command_convert_nor_address(semc_mem_config_t *config, uint32_t slaveAddress)
{
    uint32_t convertedAddress = 0;

    if (config->norMemConfig.dataPortWidth == 8)
    {
        convertedAddress = slaveAddress;
    }
    else if (config->norMemConfig.dataPortWidth == 16)
    {
        convertedAddress = slaveAddress << 1;
    }

    convertedAddress += config->norMemConfig.comMemBaseAddress;

    return convertedAddress;
}

//!@brief Configure AXI queue of SEMC
static void semc_axi_queue_config(uint32_t instance, semc_axi_queue_weigth_config_t *queueConfig)
{
    SEMC_Type *base = SEMC;

    // Set BMCR0 register: Queue 0/A can be used for All Device
    if (instance == 0)
    {
        base->BMCR0 = SEMC_BMCR0_WQOS(queueConfig->qos) |
                      SEMC_BMCR0_WAGE(queueConfig->aging) |
                      SEMC_BMCR0_WSH(queueConfig->slaveHit_noswitch) |
                      SEMC_BMCR0_WRWS(queueConfig->slaveHit_switch);
    }
    // Set BMCR1 register: Queue 1/B can be used for SDRAM device only)
    else if (instance == 1)
    {
        base->BMCR1 = SEMC_BMCR1_WQOS(queueConfig->qos) |
                      SEMC_BMCR1_WAGE(queueConfig->aging) |
                      SEMC_BMCR1_WPH(queueConfig->pageHit) |
                      SEMC_BMCR1_WRWS(queueConfig->slaveHit_switch) |
                      SEMC_BMCR1_WBR(queueConfig->bankRotation);
    }
    // Note: BMCR2/3 registers are used for debugging only, User shouldn't be able to see it.
}

//!@brief Convert memory size to ms parameter for BRx configuration
static status_t semc_convert_memory_size_to_ms(uint32_t memorySize, uint8_t *ms)
{
    /* Note: Max supported memory size is 2GB. */
    /* Note: When access address is not in valid range, there will be a bus error. */
    if (memorySize <= SMEC_MAX_DEVICE_MEMORY_SIZE_IN_BYTES)
    {
        memorySize >>= 10u;
        *ms = 0;
        uint32_t size = memorySize / 8;
        while (size)
        {
            size >>= 1;
            (*ms)++;
        }
        return kStatus_Success;
    }
    else
    {
        return kStatus_SEMC_InvalidFlashMemorySize;
    }
}

//!@brief Convert burst length to bl parameter for NORCR0/NANDCR0 configuration
static status_t semc_convert_burst_length_to_bl(uint32_t burstLength, uint8_t *bl)
{
    if ((burstLength >= kSemcMiscProperty_NORNAND_MinBurstLengthInBytes) &&
        (burstLength <= kSemcMiscProperty_NORNAND_MaxBurstLengthInBytes))
    {
        *bl = 0;
        uint32_t length = burstLength / 2;
        while (length)
        {
            length >>= 1;
            (*bl)++;
        }
        return kStatus_Success;
    }
    else
    {
        return kStatus_SEMC_InvalidBurstLength;
    }
}

//!@brief Configure NOR flash control registers
static status_t semc_config_nor_flash_control_registers(semc_mem_config_t *config)
{
    status_t status = kStatus_Success;
    uint8_t ms, bl;
    SEMC_Type *base = SEMC;

    /////////////////////////////////////////////
    // 1. Set BR5 register: base address, memory size, valid
    // Set NOR flash memory base address and size (AXI/IP BUS command)
    // (default 0x9000_0000), memory size(default 128MB)
    base->BR[5] &= ((uint32_t) ~(SEMC_BR_BA_MASK |
                               SEMC_BR_MS_MASK |
                               SEMC_BR_VLD_MASK));
    /* Note: Available SEMC memory region is defined in chip system memory map. */
    base->BR[5] |= SEMC_BR_BA((config->norMemConfig.comMemBaseAddress & SEMC_BR_BA_MASK) >> SEMC_BR_BA_SHIFT);
    status = semc_convert_memory_size_to_ms(config->norMemConfig.comMemSizeInByte, &ms);
    if (status == kStatus_Success)
    {
        base->BR[5] |= SEMC_BR_MS(ms);
    }
    else
    {
        return status;
    }
    base->BR[5] |= SEMC_BR_VLD_MASK;

    /////////////////////////////////////////////
    // 2. Set IOCR register: (Address, CE# port pinmux)
    // Set NOR flash address port pinmux (SW pinmux)
    base->IOCR &= ((uint32_t) ~(SEMC_IOCR_MUX_A8_MASK |
                                SEMC_IOCR_MUX_CSX0_MASK |
                                SEMC_IOCR_MUX_CSX1_MASK |
                                SEMC_IOCR_MUX_CSX2_MASK |
                                SEMC_IOCR_MUX_CSX3_MASK |
                                SEMC_IOCR_MUX_RDY_MASK));
    if (config->norMemConfig.addressPortWidth > kSemcMiscProperty_NOR_BaseFlashAddressPortWidth)
    {
        if (config->norMemConfig.addressPortWidth >= kSemcMiscProperty_NOR_BaseFlashAddressPortWidth + 1)
        {
            // NOR Address bit 24 (A24)
            if (config->norMemConfig.cePortOutputSelection == kSemcCeOutputSelection_MUX_CSX0)
            {
                return kStatus_SEMC_InvalidSwPinmuxSelection;
            }
        }
        if (config->norMemConfig.addressPortWidth >= kSemcMiscProperty_NOR_BaseFlashAddressPortWidth + 2)
        {
            // NOR Address bit 25 (A25)
            if (config->norMemConfig.cePortOutputSelection == kSemcCeOutputSelection_MUX_CSX1)
            {
                return kStatus_SEMC_InvalidSwPinmuxSelection;
            }
        }
        if (config->norMemConfig.addressPortWidth >= kSemcMiscProperty_NOR_BaseFlashAddressPortWidth + 3)
        {
            // NOR Address bit 26 (A26)
            if (config->norMemConfig.cePortOutputSelection == kSemcCeOutputSelection_MUX_CSX2)
            {
                return kStatus_SEMC_InvalidSwPinmuxSelection;
            }
        }
        if (config->norMemConfig.addressPortWidth >= kSemcMiscProperty_NOR_BaseFlashAddressPortWidth + 4)
        {
            // NOR Address bit 27 (A27)
            // Note: IOCR[MUX_RDY] can also choose SEMC_RDY as A27 Pad, but we don't support it.
            if (config->norMemConfig.cePortOutputSelection == kSemcCeOutputSelection_MUX_CSX3)
            {
                return kStatus_SEMC_InvalidSwPinmuxSelection;
            }
        }
        if (config->norMemConfig.addressPortWidth > kSemcMiscProperty_NOR_MaxFlashAddressPortWidth)
        {
            return kStatus_SEMC_InvalidAddressPortWidth;
        }
    }
    // Set NOR flash CE# port pinmux (SW pinmux)
    switch(config->norMemConfig.cePortOutputSelection)
    {
        case kSemcCeOutputSelection_MUX_A8:
            base->IOCR |= SEMC_IOCR_MUX_A8(2);
            break;
        case kSemcCeOutputSelection_MUX_CSX0:
            base->IOCR |= SEMC_IOCR_MUX_CSX0(5);
            break;
        case kSemcCeOutputSelection_MUX_CSX1:
            base->IOCR |= SEMC_IOCR_MUX_CSX1(5);
            break;
        case kSemcCeOutputSelection_MUX_CSX2:
            base->IOCR |= SEMC_IOCR_MUX_CSX2(5);
            break;
        case kSemcCeOutputSelection_MUX_CSX3:
            base->IOCR |= SEMC_IOCR_MUX_CSX3(5);
            break;
        case kSemcCeOutputSelection_MUX_RDY:
            base->IOCR |= SEMC_IOCR_MUX_RDY(4);
            break;
        default:
            status = kStatus_SEMC_InvalidSwPinmuxSelection;
            break;
    }
    if (status != kStatus_Success)
    {
        return status;
    }

    /////////////////////////////////////////////
    // 3. Set MCR register: WAIT/RDY# polarity
    base->MCR &= (uint32_t)~SEMC_MCR_WPOL0_MASK;
    base->MCR |= SEMC_MCR_WPOL0(config->norMemConfig.rdyPortPolarity);

    /////////////////////////////////////////////
    // 4. Set NORCR0 register: Data port size, Burst Length, Address mode, ADV# polarity
    // Set Address mode, ADV# polarity
    base->NORCR0 &= ((uint32_t) ~(SEMC_NORCR0_AM_MASK |
                                  SEMC_NORCR0_ADVP_MASK |
                                  SEMC_NORCR0_BL_MASK |
                                  SEMC_NORCR0_PS_MASK |
                                  SEMC_NORCR0_COL_MASK));
    base->NORCR0 |= SEMC_NORCR0_AM(config->norMemConfig.addressMode) |
                    SEMC_NORCR0_ADVP(config->norMemConfig.advPortPolarity);
    // Set NOR flash burst data length
    status = semc_convert_burst_length_to_bl(config->norMemConfig.burstLengthInBytes, &bl);
    if (status == kStatus_Success)
    {
        base->NORCR0 |= SEMC_NORCR0_BL(bl);
    }
    else
    {
        return status;
    }
    // Set NOR flash data port width
    if (config->norMemConfig.dataPortWidth == 8U)
    {
        base->NORCR0 |= SEMC_NORCR0_PS(0U);
    }
    else if (config->norMemConfig.dataPortWidth == 16U)
    {
        base->NORCR0 |= SEMC_NORCR0_PS(1U);
    }
    else
    {
        return kStatus_SEMC_InvalidDataPortWidth;
    }
    // Set NOR flash column address bit width
    if ((config->norMemConfig.columnAddressWidth <= kSemcMiscProperty_NOR_MaxColumnAddressBitWidth) &&
        (config->norMemConfig.columnAddressWidth >= kSemcMiscProperty_NOR_MinColumnAddressBitWidth))
    {
        uint8_t col = kSemcMiscProperty_NOR_MaxColumnAddressBitWidth;
        col -= config->norMemConfig.columnAddressWidth;
        base->NORCR0 |= SEMC_NORCR0_COL(col);

    }
    else
    {
        return kStatus_SEMC_InvalidColumnAddressBitWidth;
    }

    /////////////////////////////////////////////
    // 5. Set NORCR1/2/3 register: AC Characteristics
    base->NORCR1 = SEMC_NORCR1_REH(config->norMemConfig.asyncOeHighTime) |
                   SEMC_NORCR1_REL(config->norMemConfig.asyncOeLowTime) |
                   SEMC_NORCR1_WEH(config->norMemConfig.asyncWeHighTime) |
                   SEMC_NORCR1_WEL(config->norMemConfig.asyncWeLowTime) |
                   SEMC_NORCR1_AH(config->norMemConfig.addressHoldTime) |
                   SEMC_NORCR1_AS(config->norMemConfig.addressSetupTime) |
                   SEMC_NORCR1_CEH(config->norMemConfig.ceMinHoldTime) |
                   SEMC_NORCR1_CES(config->norMemConfig.ceSetupTime);

    base->NORCR2 = SEMC_NORCR2_CEITV(config->norMemConfig.ceMinIntervalTime) |
                   SEMC_NORCR2_RD(config->norMemConfig.syncReadCycleTime) |
                   SEMC_NORCR2_LC(config->norMemConfig.syncLatencyCount) |
                   SEMC_NORCR2_AWDH(config->norMemConfig.asyncAddressToDataHoldTime) |
                   SEMC_NORCR2_TA(config->norMemConfig.asyncTurnaroundTime) |
                   SEMC_NORCR2_WDH(config->norMemConfig.syncDataHoldTime) |
                   SEMC_NORCR2_WDS(config->norMemConfig.syncDataSetupTime);

    return status;
}

//!@brief Configure NAND flash control registers
static status_t semc_config_nand_flash_control_registers(semc_mem_config_t *config)
{
    status_t status = kStatus_Success;
    uint8_t ms, bl, columnAddressWidth;
    SEMC_Type *base = SEMC;

    /////////////////////////////////////////////
    // 1. Set BR4/8 register: base address, memory size, valid
    // Set NAND flash memory base address and size (AXI command)
    // (default 0x9E00_0000), memory size(default 32MB)
    base->BR[4] &= ((uint32_t) ~(SEMC_BR_BA_MASK |
                               SEMC_BR_MS_MASK |
                               SEMC_BR_VLD_MASK));
    /* Note: Available SEMC memory region is defined in chip system memory map. */
    base->BR[4] |= SEMC_BR_BA((config->nandMemConfig.axiMemBaseAddress & SEMC_BR_BA_MASK) >> SEMC_BR_BA_SHIFT);
    status = semc_convert_memory_size_to_ms(config->nandMemConfig.axiMemSizeInByte, &ms);
    if (status == kStatus_Success)
    {
        base->BR[4] |= SEMC_BR_MS(ms);
    }
    else
    {
        return status;
    }
    base->BR[4] |= SEMC_BR_VLD_MASK;

    // Set NAND flash memory base address and size (IP BUS command)
    // (default 0x0000_0000), memory size(default 2GB)
    base->BR[8] &= ((uint32_t) ~(SEMC_BR_BA_MASK |
                               SEMC_BR_MS_MASK |
                               SEMC_BR_VLD_MASK));
    base->BR[8] |= SEMC_BR_BA((config->nandMemConfig.ipgMemBaseAddress & SEMC_BR_BA_MASK) >> SEMC_BR_BA_SHIFT);
    status = semc_convert_memory_size_to_ms(config->nandMemConfig.ipgMemSizeInByte, &ms);
    if (status == kStatus_Success)
    {
        base->BR[8] |= SEMC_BR_MS(ms);
    }
    else
    {
        return status;
    }
    base->BR[8] |= SEMC_BR_VLD_MASK;

    /////////////////////////////////////////////
    // 2. Set IOCR register: (CE# port pinmux)
    // Set NAND flash CE# port pinmux (SW pinmux)
    base->IOCR &= ((uint32_t) ~(SEMC_IOCR_MUX_A8_MASK |
                                SEMC_IOCR_MUX_CSX0_MASK |
                                SEMC_IOCR_MUX_CSX1_MASK |
                                SEMC_IOCR_MUX_CSX2_MASK |
                                SEMC_IOCR_MUX_CSX3_MASK |
                                SEMC_IOCR_MUX_RDY_MASK));
    switch(config->nandMemConfig.cePortOutputSelection)
    {
        case kSemcCeOutputSelection_MUX_A8:
            base->IOCR |= SEMC_IOCR_MUX_A8(1);
            break;
        case kSemcCeOutputSelection_MUX_CSX0:
            base->IOCR |= SEMC_IOCR_MUX_CSX0(4);
            break;
        case kSemcCeOutputSelection_MUX_CSX1:
            base->IOCR |= SEMC_IOCR_MUX_CSX1(4);
            break;
        case kSemcCeOutputSelection_MUX_CSX2:
            base->IOCR |= SEMC_IOCR_MUX_CSX2(4);
            break;
        case kSemcCeOutputSelection_MUX_CSX3:
            base->IOCR |= SEMC_IOCR_MUX_CSX3(4);
            break;
        default:
            status = kStatus_SEMC_InvalidSwPinmuxSelection;
            break;
    }
    if (status != kStatus_Success)
    {
        return status;
    }
    
    // Set NAND RDY port pinmux
    base->IOCR |= SEMC_IOCR_MUX_RDY(0);

    /////////////////////////////////////////////
    // 3. Set MCR register: WAIT/RDY# polarity
    base->MCR &= (uint32_t)~SEMC_MCR_WPOL1_MASK;
    base->MCR |= SEMC_MCR_WPOL1(config->nandMemConfig.rdyPortPolarity);

    /////////////////////////////////////////////
    // 4. Set NANDCR0 register: Data port size, Burst Length
    base->NANDCR0 &= ((uint32_t) ~(SEMC_NANDCR0_EDO_MASK |
                                   SEMC_NANDCR0_BL_MASK |
                                   SEMC_NANDCR0_PS_MASK |
                                   SEMC_NANDCR0_COL_MASK));
    base->NANDCR0 |= SEMC_NANDCR0_EDO(config->nandMemConfig.edoMode);
    // Set NAND flash burst data length
    status = semc_convert_burst_length_to_bl(config->nandMemConfig.burstLengthInBytes, &bl);
    if (status == kStatus_Success)
    {
        base->NANDCR0 |= SEMC_NANDCR0_BL(bl);
    }
    else
    {
        return status;
    }
    // Set NAND flash data port width
    if (config->nandMemConfig.ioPortWidth == 8U)
    {
        base->NANDCR0 |= SEMC_NANDCR0_PS(0U);
    }
    else if (config->nandMemConfig.ioPortWidth == 16U)
    {
        base->NANDCR0 |= SEMC_NANDCR0_PS(1U);
    }
    else
    {
        return kStatus_SEMC_InvalidDataPortWidth;
    }
    // Set NAND flash column address bit width
    // Note NA4.0: Actual one page area of NAND device usually contains main area and spare area,
    //  main area stores page data, spare area stores bad block info and ECC data.
    //  So we should set COL to be (actual column address bits + 1) which means accessible page size
    //  is twice as much as actual page size, then we can access spare area by AXI command or 
    //  IPG command (change column read).
    if (config->nandMemConfig.columnAddressOption == kSemcNandColumnAddressOption_SpareAreaAccess)
    {
        columnAddressWidth = (config->nandMemConfig.columnAddressWidth + 1);
    }
    else
    {
        columnAddressWidth = config->nandMemConfig.columnAddressWidth;
    }
    if ((columnAddressWidth <= kSemcMiscProperty_NAND_MaxColumnAddressBitWidth) &&
        (columnAddressWidth >= kSemcMiscProperty_NAND_MinColumnAddressBitWidth))
    {
        uint8_t col = kSemcMiscProperty_NAND_MaxColumnAddressBitWidth;
        col -= columnAddressWidth;
        base->NANDCR0 |= SEMC_NANDCR0_COL(col);
    }
    else
    {
        return kStatus_SEMC_InvalidColumnAddressBitWidth;
    }

    /////////////////////////////////////////////
    // 5. Set NANDCR1/2 register: AC Characteristics
    base->NANDCR1 = SEMC_NANDCR1_CEITV(config->nandMemConfig.ceMinIntervalTime) |
                    SEMC_NANDCR1_TA(config->nandMemConfig.asyncTurnaroundTime) |
                    SEMC_NANDCR1_REH(config->nandMemConfig.reHighTime) |
                    SEMC_NANDCR1_REL(config->nandMemConfig.reLowTime) |
                    SEMC_NANDCR1_WEH(config->nandMemConfig.weHighTime) |
                    SEMC_NANDCR1_WEL(config->nandMemConfig.weLowTime) |
                    SEMC_NANDCR1_CEH(config->nandMemConfig.ceMinHoldTime) |
                    SEMC_NANDCR1_CES(config->nandMemConfig.ceSetupTime);

    base->NANDCR2 = SEMC_NANDCR2_TWB(config->nandMemConfig.weHighToBusyTime) |
                    SEMC_NANDCR2_TRR(config->nandMemConfig.readyToReLowTime) |
                    SEMC_NANDCR2_TADL(config->nandMemConfig.aleToDataStartTime) |
                    SEMC_NANDCR2_TRHW(config->nandMemConfig.reHighToWeLowTime) |
                    SEMC_NANDCR2_TWHR(config->nandMemConfig.weHighToReLowTime);
    // 6. Set NANDCR3 register: Addressing option
    base->NANDCR3 = SEMC_NANDCR3_NDOPT3((config->nandMemConfig.arrayAddressOption >> 2) & 0x1u) |
                    SEMC_NANDCR3_NDOPT2((config->nandMemConfig.arrayAddressOption >> 1) & 0x1u) |
                    SEMC_NANDCR3_NDOPT1(config->nandMemConfig.arrayAddressOption & 0x1u);

    return status;
}

#if BL_FEATURE_SEMC_NAND_MODULE || BL_FEATURE_SEMC_NOR_MODULE
//!@brief Initialize SEMC module
status_t semc_init(semc_mem_config_t *config)
{
    status_t status = kStatus_Success;
    uint32_t setValue;
    uint8_t portWidth;
    semc_clk_freq_t clkFreq;
    SEMC_Type *base = SEMC;

    // Check config block tag
    if (config->tag != kSemcConfigBlockTag)
    {
        return kStatus_InvalidArgument;
    }

    semc_clock_gate_disable();

    // Configure SEMC HW pin mux
    semc_iomux_config(config);

    // Configure SEMC module AXI clock
    clkFreq = (semc_clk_freq_t)config->asyncClkFreq;
    if (clkFreq <= SEMC_MAX_CLK_FREQ)
    {
        semc_axi_clock_config(clkFreq);
    }
    else
    {
        return kStatus_SEMC_InvalidIpClockConfiguration;
    }

    semc_clock_gate_enable();

    // Enable and reset SEMC module
    base->MCR &= ~SEMC_MCR_MDIS_MASK;
    semc_sw_reset(base);

    // Disable SEMC module during configuring control registers.
    base->MCR |= SEMC_MCR_MDIS_MASK;

    // Configure device control register
    if (config->deviceMemType == kSemcDeviceMemType_NOR)
    {
        portWidth = config->norMemConfig.dataPortWidth;
        status = semc_config_nor_flash_control_registers(config);
    }
    else if (config->deviceMemType == kSemcDeviceMemType_NAND)
    {
        portWidth = config->nandMemConfig.ioPortWidth;
        status = semc_config_nand_flash_control_registers(config);
    }
    else
    {
        return kStatus_SEMC_InvalidDeviceType;
    }
    if (status != kStatus_Success)
    {
        return status;
    }

    // Configure bus access control registers
    semc_axi_queue_config(kSemcAxiCommandQueueInstance_A, (semc_axi_queue_weigth_config_t *)&s_semcAxiQueueWeightConfig);
    // Set port size
    status = semc_set_macthed_ipg_command_data_size(portWidth);
    if (status != kStatus_Success)
    {
        return status;
    }

    setValue = base->MCR & ((uint32_t) ~(SEMC_MCR_BTO_MASK | SEMC_MCR_CTO_MASK | SEMC_MCR_DQSMD_MASK));
    // Set bus and commmand execution timeout cycles
    setValue |= SEMC_MCR_CTO(config->commandExecutionTimeoutCycles) | SEMC_MCR_BTO(config->busTimeoutCycles);
    // Set read strobe mode
    setValue |= SEMC_MCR_DQSMD(config->readStrobeMode);
    base->MCR = setValue;

    // Enable SEMC module
    base->MCR &= ~SEMC_MCR_MDIS_MASK;

    return status;
}
#endif //BL_FEATURE_SEMC_NAND_MODULE || BL_FEATURE_SEMC_NOR_MODULE

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
