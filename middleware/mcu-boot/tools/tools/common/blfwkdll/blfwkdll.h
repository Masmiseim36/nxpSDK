/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifdef BLFWKDLL_EXPORTS
#define BLFWKDLL_API __declspec(dllexport)
#else
#define BLFWKDLL_API __declspec(dllimport)
#endif

#pragma once
#include <string>
#include <vector>

//! @brief Tag value used to validate the bootloader configuration data.
#define kBootloaderConfigAreaTag (('k' << 0) | ('c' << 8) | ('f' << 16) | ('g' << 24))
//! @brief Peripheral type bit mask definitions.
#define kPeripheralMaskUart (1 << 0)
#define kPeripheralMaskI2c (1 << 1)
#define kPeripheralMaskSpi (1 << 2)
#define kPeripheralMaskCan (1 << 3)
#define kPeripheralMaskUsb (1 << 4)
#define kPeripheralMask \
    (kPeripheralMaskUart | kPeripheralMaskI2c | kPeripheralMaskSpi | kPeripheralMaskCan | kPeripheralMaskUsb)
//! @brief Bit positions for boot flags in configuration data
#define kBootFlagDirectBoot (1 << 0)
//! @brief Bit positions for clock flags in configuration data.
#define kClockFlagHighSpeed (1 << 0)

// Copy from src/bootloader/src/flexcan_peripheral_interface.c
// config1 8 bit
#define FLEXCAN_SPEED_MASK 0x0F       // bit[3:0]
#define FLEXCAN_SPEED_SPEC_MASK 0x08  // bit[3:3]
#define FLEXCAN_SPEED_INDEX_MASK 0x07 // bit[2:0]
#define FLEXCAN_CLKSEL_MASK 0x80      // bit[7:7]

#define FLEXCAN_PROPSEG_MASK 0x70 // bit[6:4]
#define FLEXCAN_PROPSEG_SHIFT 4
// config2 16 bit
#define FLEXCAN_PRESCALER_MASK 0xFF00 // bit[15:8]
#define FLEXCAN_PRESCALER_SHIFT 8
#define FLEXCAN_PSEG1_MASK 0x00E0 // bit[7:5]
#define FLEXCAN_PSEG1_SHIFT 5
#define FLEXCAN_PSEG2_MASK 0x001C // bit[4:2]
#define FLEXCAN_PSEG2_SHIFT 2
#define FLEXCAN_RJW_MASK 0x0003 // bit[1:0]
#define FLEXCAN_RJW_SHIFT 0

#define FLEXCAN_SPEED_125K 0x00 // bit[2:0]
#define FLEXCAN_SPEED_250K 0x01 // bit[2:0]
#define FLEXCAN_SPEED_500K 0x02 // bit[2:0]
#define FLEXCAN_SPEED_1M 0x04   // bit[2:0]

namespace blfwkdll
{
// Copy from src/property/property.c
//! @brief Format of bootloader configuration data on Flash.
struct BLFWKDLL_API BootloaderConfigureAreaData
{
    uint32_t tag; //!< [00:03] Magic number to verify bootloader configuration is valid. Must be set to 'kcfg'.
    uint32_t crcStartAddress; //!< [04:07] Start address for application image CRC check. If the bits are all set then
    //! Bootloader by default will not perform any CRC check.
    uint32_t crcByteCount; //!< [08:0b] Byte count for application image CRC check. If the bits are all set then
    //! bootloader by default will not prform any CRC check.
    uint32_t crcExpectedValue; //!< [0c:0f] Expected CRC value for application CRC check. If the bits are all set then
    //! Bootloader by default will not perform any CRC check.
    uint8_t enabledPeripherals; //!< [10:10] Bitfield of peripherals to enable.
                                //! bit 0 - LPUART, bit 1 - I2C, bit 2 - SPI, bit 3 - CAN, bit 4 - USB
                                //! Bootloader will enable the peripheral if corresponding bit is set to 1.
    uint8_t i2cSlaveAddress;    //!< [11:11] If not 0xFF, used as the 7-bit I2C slave address. If 0xFF, defaults to 0x10
    //! for I2C slave address.
    uint16_t peripheralDetectionTimeoutMs; //!< [12:13] Timeout in milliseconds for active peripheral detection. If
    //! 0xFFFF, defaults to 5 seconds.
    uint16_t usbVid; //!< [14:15] Sets the USB Vendor ID reported by the device during enumeration. If 0xFFFF, it
    //! defaults to 0x15A2.
    uint16_t usbPid;            //!< [16:17] Sets the USB Product ID reported by the device during enumeration.
    uint32_t usbStringsPointer; //!< [18:1b] Sets the USB Strings reported by the device during enumeration.
    uint8_t clockFlags; //!< [1c:1c] The flags in the clockFlags configuration field are enabled if the corresponding
    //! bit is cleared (0).
    //! bit 0 - HighSpeed Enable high speed mode (i.e., 48 MHz).
    uint8_t
        clockDivider; //!< [1d:1d] Inverted value of the divider to use for core and bus clocks when in high speed mode.
    uint8_t bootFlags; //!< [1e:1e] If bit 0 is cleared, then Bootloader will jump to either Quad SPI Flash or
    //! internal flash image depending on FOPT BOOTSRC_SEL bits.
    //! If the bit is set, then Bootloader will prepare for host communication over serial peripherals.
    uint8_t pad0;                       //!< [1f:1f] Reserved, set to 0xFF
    uint32_t mmcauConfigPointer;        //!< [20:23] A pointer to the MMCAU configuration structure in memory.
    uint32_t keyBlobPointer;            //!< [24:27]Holds a pointer value to the key blob array used to configure OTFAD
    uint8_t pad1;                       //!< [28:28] reserved
    uint8_t canConfig1;                 //!< [29:29] ClkSel[1], PropSeg[3], SpeedIndex[4]
    uint16_t canConfig2;                //!< [2a:2b] Pdiv[8], Pseg1[3], Pseg2[3],  rjw[2]
    uint16_t canTxId;                   //!< [2c:2d] txId
    uint16_t canRxId;                   //!< [2e:2f] rxId
    uint32_t qspi_config_block_pointer; //!< [30:33] A pointer to the QSPI config block in internal flash array.
    uint32_t reserved[3];               //!<[34:3f] reserved
};

/*!
* \brief Update class contains the functionality necessary to update the
* firmware on a device running Bootloader.
*
* The purpose of this class is to provide a common interface for
* updating any device running the Bootloader from several different file formats.
*/
class BLFWKDLL_API Updater
{
public:
    //! @brief Bootloader Transports.
    enum HOST_PERIPHERAL_TYPE_TAG
    {
        kHostPeripheralType_None,
        kHostPeripheralType_UART,
        kHostPeripheralType_BUSPAL_UART,
        kHostPeripheralType_USB_HID,
        kHostPeripheralType_SIM
    };

    //! @brief BusPal Transports.
    enum BUS_PAL_TYPE
    {
        kBusPalFunction_None,
        kBusPalFunction_SPI,
        kBusPalFunction_I2C,
        kBusPalFunction_CAN,
        kBusPalFunction_GPIO_CONFIG,
        kBusPalFunction_GPIO_SET,
        kBusPalFunction_FPGA_CLOCK_SET
    };

    //! @brief SPI clock polarity configuration.
    enum SPI_CLOCK_POLARITY
    {
        kSpiClockPolarity_ActiveHigh = 0, //!< Active-high SPI clock (idles low).
        kSpiClockPolarity_ActiveLow = 1   //!< Active-low SPI clock (idles high).
    };

    //! @brief SPI clock phase configuration.
    enum SPI_CLOCK_PHASE
    {
        kSpiClockPhase_FirstEdge =
            0, //!< First edge on SPSCK occurs at the middle of the first cycle of a data transfer.
        kSpiClockPhase_SecondEdge =
            1 //!< First edge on SPSCK occurs at the start of the first cycle of a data transfer.
    };

    //! @brief SPI data shifter direction options.
    enum SPI_SHIFT_DIRECTION
    {
        kSpiMsbFirst = 0, //!< Data transfers start with most significant bit.
        kSpiLsbFirst = 1  //!< Data transfers start with least significant bit.
    };

    //! @brief BusPal configuration data.
    struct BusPalConfig
    {
        BUS_PAL_TYPE function;
        uint32_t spiSpeedKHz;
        SPI_CLOCK_POLARITY spiPolarity;
        SPI_CLOCK_PHASE spiPhase;
        SPI_SHIFT_DIRECTION spiDirection;
        uint8_t i2cAddress;
        uint32_t i2cSpeedKHz;
        uint32_t canSpeed;
        uint32_t canTxid;
        uint32_t canRxid;
        uint8_t gpioPort;
        uint8_t gpioPinNum;
        uint8_t gpioLevel;
        uint8_t gpioMux;
        uint32_t fpgaClockMhz;
    };

    //! @brief Peripheral configuration data.
    struct PeripheralConfig
    {
        HOST_PERIPHERAL_TYPE_TAG peripheralType;
        std::string comPortName;
        long comPortSpeed;
        uint32_t packetTimeoutMs;
        unsigned short usbHidVid;
        unsigned short usbHidPid;
        std::string usbHidSerialNumber;
        bool ping;
        BusPalConfig busPalConfig;
        std::string usbPath;
    };

    //! @brief Bootloader command mask.
    enum COMMAND_MASK
    {
        kCommandMask_FlashEraseAll = 0x00000001,
        kCommandMask_ReadMemory = 0x00000004,
        kCommandMask_WriteMemory = 0x00000008,
        kCommandMask_FillMemory = 0x00000010,
        kCommandMask_FlashSecurityDisable = 0x00000020,
        kCommandMask_GetProperty = 0x00000040,
        kCommandMask_ReceiveSBFile = 0x00000080,
        kCommandMask_Execute = 0x00000100,
        kCommandMask_Call = 0x00000200,
        kCommandMask_Reset = 0x00000400,
        kCommandMask_SetProperty = 0x00000800,
        kCommandMask_FlashEraseAllUnsecure = 0x00001000,
        kCommandMask_FlashProgramOnce = 0x00002000,
        kCommandMask_FlashReadOnce = 0x00004000,
        kCommandMask_FlashReadResource = 0x00008000,
        kCommandMask_ConfigureQuadSpi = 0x00010000,
    };

    //! @brief Bootloader property.
    enum PROPERTY_TAG
    {
        kPropertyTag_ListProperties = 0x00,
        kPropertyTag_BootloaderVersion = 0x01,
        kPropertyTag_AvailablePeripherals = 0x02,
        kPropertyTag_FlashStartAddress = 0x03,
        kPropertyTag_FlashSizeInBytes = 0x04,
        kPropertyTag_FlashSectorSize = 0x05,
        kPropertyTag_FlashBlockCount = 0x06,
        kPropertyTag_AvailableCommands = 0x07,
        kPropertyTag_CrcCheckStatus = 0x08,
        kPropertyTag_Reserved9 = 0x09,
        kPropertyTag_VerifyWrites = 0x0a,
        kPropertyTag_MaxPacketSize = 0x0b,
        kPropertyTag_ReservedRegions = 0x0c,
        kPropertyTag_Reserved13 = 0x0d,
        kPropertyTag_RAMStartAddress = 0x0e,
        kPropertyTag_RAMSizeInBytes = 0x0f,
        kPropertyTag_SystemDeviceId = 0x10,
        kPropertyTag_FlashSecurityState = 0x11,
        kPropertyTag_UniqueDeviceId = 0x12,
        kPropertyTag_FacSupport = 0x13,
        kPropertyTag_FlashAccessSegmentSize = 0x14,
        kPropertyTag_FlashAccessSegmentCount = 0x15,
        kPropertyTag_FlashReadMargin = 0x16,
        kPropertyTag_QspiInitStatus = 0x17,
        kPropertyTag_TargetVersion = 0x18,
        kPropertyTag_ExternalMemoryAttributes = 0x19,
        kPropertyTag_InvalidProperty = 0xFF,
    };
    typedef std::pair<uint32_t, std::string> MemoryId;
    typedef std::vector<MemoryId> MemoryList;

private:
    // Due to the reason that MFC project must include this headfile to use blfwkdll,
    // while cannot include blfwk/Updater.h.
    // So use a void pointer here
    void *updater; //!< A pointer to blfwk::Updater.

public:
    //! \brief Default constructor.
    Updater(PeripheralConfig *config);

    //! \brief Destructor.
    ~Updater();

    //! \brief register callback function
    //!
    //! \param progress     Progress callback function
    //!
    //! \param abort        The variable to abort the data phase.
    void registerCallback(void (*progress)(int, int, int), bool *abort);

    //! \brief  Program flash on the device.
    //!
    //! \exception  Raised if the file could not be opened successfully.
    //!
    //! \param  filename       The file to program into the device.
    //! \param  base_address   The address on the device where the file wiill be written.
    //!                        (for hex,srec and sb file, the param can be any value.)
    uint32_t flashFirmware(std::string *filename, uint32_t baseAddress, uint32_t memoryId);

    //! \brief Erase all flash blocks and release MCU security
    //!
    //! \exception  Raised if the FlashEraseAllUnsecure command does not return success.
    //!             Raised if the FlashEraseAllUnsecure command is not supported.
    void eraseAllUnsecure();

    //! \brief Execute the FlashEraseRegion bootloader command.
    //!
    //! \exception  Thrown if an error occurred while sending the
    //!             FlashEraseRegion(start, length) bootloader command.
    //!
    //! \param  start   The beginning address of the memory region to be erased.
    //! \param  length  The length in bytes of the memory region to be erased.
    void eraseRegion(uint32_t start, uint32_t length, uint32_t memoryId);

    //! \brief Execute the FlashEraseAll bootloader command.
    //!
    //! \exception  Thrown if an error occurred while sending the
    //!             FlashEraseAll bootloader command.
    void eraseAll(uint32_t memoryId);

    //! \brief Release security using BackdoorKey
    //!
    //! \exception  Raised if the FlashSecurityDisable command does not return success.
    //!             Raised if the FlashSecurityDisable command is not supported.
    //!             Raised if the parameter is illegal
    //! \param backdoor_key     The 16 hex digitals used to release the security
    void unlockWithKey(const std::string &backdoorKey);

    //! \brief Checks if Bootloader device supports a given command.
    //!
    //! \exception  Thrown if an error occurred while sending the
    //!             GetProperty(kProperty_AvailableCommands) bootloader command.
    //!
    //! \param  command The command to check.
    //!
    //! \return true if command is supported, false if not.
    bool isCommandSupported(const std::string &command);

    //! \brief get Device's property by using get-property command.
    //!
    //! \exception  Thrown if an error occurred while sending the
    //!             GetProperty(property) bootloader command.
    //!
    //! \param  property tag
    //!
    //! \param  pointer of the response values.
    void getProperty(PROPERTY_TAG tag, uint32_t *responseWords);

    //! \brief get total internal flash size of current device.
    //!
    //! \exception Raised if the operation is failed.
    //!
    //! \return the size in bytes. Return 0 means no internal Flash available.
    uint32_t getInternalFlashSize(void);

    //! \brief get total internal RAM size of current device.
    //!
    //! \exception Raised if the operation is failed.
    //!
    //! \return the size in bytes. Return 0 means no internal RAM available.
    uint32_t getInternalRAMSize(void);

    //! \brief Execute the FlashProgramOnce bootloader command.
    //!
    //! \exception  Thrown if an error occurred while sending the
    //!             FlashEraseAll bootloader command.
    //!
    //! \param  index   The index of a specific program once field.
    //!
    //! \param  byteCount  The length in bytes of a specific program once field.
    //!
    //! \param  data    The 8/16 hex digitals to write.
    void programOnce(uint32_t index, uint32_t byteCount, const std::string &data, bool isLsb);

    //! \brief Execute the reset command.
    //!
    //! \exception  Raised if the reset command is failed.
    void reset();

    //! @brief Get the serial protocol version.
    //!
    //! \exception  Raised if the operation is failed.
    uint32_t getSerialProtocolVersion();

    //! \brief Send a ping if applicable.
    //!
    //! \exception  Raised if ping response is not received successfully.
    void ping(int32_t retries, uint32_t delay, uint32_t comSpeed);

    //! \brief return all memory IDs supported by Bootloader.
    //!
    //! \param  list  return the list.
    static void getSupportedMemories(MemoryList &list);

    //! \brief Check if a given memory ID is also a memory space.
    //!
    //! \param  memoryId  The given memory ID to check.
    //!
    //! \return true/false
    static bool IsMemorySpaceId(uint32_t memoryId);
};

/*!
* \brief Crc class contains the functionality about crc ccalculation.
*/
class BLFWKDLL_API Crc
{
public:
    //! \brief Default constructor.
    Crc(){};

    //! \brief Destructor.
    virtual ~Crc(){};

    //! \brief calculate crc32 for a given memory region
    //!
    //! \param start    The start address of a memory region that contains the data
    //! \
    //! \param length   Length in bytes to calculate
    //! \
    //! \return calculated result
    static uint32_t calculate_application_crc32(const uint8_t *start, uint32_t length);
};

} // namespace blfwkdll
