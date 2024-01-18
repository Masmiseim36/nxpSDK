/*! *********************************************************************************
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file
 *
 * This source file contains the code that enables Over The Wire (OTW) communication
 * between a host device and a K32W0.
 * A UART communication bewteen the two devices is mandatory and the support
 * of a DMA on this UART is also mandatory.
 *
 * Configurations:
 * The below configuration flags can be overriden depending on board configurations
 * OTW_ISP_UART_INSTANCE
 * OTW_RESET_PIN_PORT
 * OTW_RESET_PIN_NUM
 * OTW_DIO5_PIN_PORT
 * OTW_DIO5_PIN_NUM
 *
 * Initialization sequence summary:
 *
 * 1. Switch the k32w0 to ISP mode
 * 2. Init the UART link to the default 115200 baudrate, send an increase baudrate
 *    command to 1.1 MBit
 * 3. Send the unlock procedure to the k32w0
 * 4. Get the memory flash info
 * 5. Send the open memory request
 *
 * Firmware download/read/write requests:
 * - Each request requires to have the initiliazation step correclty run.
 * - Each command sends to the k32w0 is blocking the calling task until a response is
 *   received or after given timeout.
 * - A DMA is used for TX and RX buffers to improve communication performances
 *
 * k32w0 Firmware download sequence summary:
 * Note: the k32w0 firmware to download must contain a 256 bytes signature at the end
 * of the binary.
 * - check if the k32w0 firmware to download is required by comparing the expected
 *    signature value with the current one read on the k32w0.
 *          - if the signature is the same => nothing to do the update is not required
 *          - if the signature is not the same => erase the whole k320w flash
 *            and write block by block the given firmware to the remote k32w0
 *
 ********************************************************************************** */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */
#include <stdbool.h>

#include "fwk_otw.h"
#include "fsl_adapter_gpio.h"
#include "fsl_adapter_crc.h"
#include "fsl_adapter_uart.h"
#include "fsl_os_abstraction.h"
#include "board.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

#if !defined(HAL_UART_DMA_ENABLE) || !HAL_UART_DMA_ENABLE
#error "HAL_UART_DMA_ENABLE is mandatory and must be enabled"
#endif

#ifndef OTW_ISP_UART_INSTANCE
#define OTW_ISP_UART_INSTANCE 3
#endif

#ifndef OTW_RESET_PIN_PORT
#define OTW_RESET_PIN_PORT 6
#endif

#ifndef OTW_RESET_PIN_NUM
#define OTW_RESET_PIN_NUM 2
#endif

#ifndef OTW_DIO5_PIN_PORT
#define OTW_DIO5_PIN_PORT 6
#endif

#ifndef OTW_DIO5_PIN_NUM
#define OTW_DIO5_PIN_NUM 26
#endif

#define LPUART_TX_DMA_CHANNEL 0U
#define LPUART_RX_DMA_CHANNEL 1U

#ifndef OTW_CONFIG_DEFAULT_RESET_DELAY_MS
// Default delay, waiting for the transceiver to reboot, in miliseconds.
#define OTW_CONFIG_DEFAULT_RESET_DELAY_MS 100
#endif

#define ISP_UART_CLOCK_RATE BOARD_BT_UART_CLK_FREQ

#define ISP_DEFAULT_UART_BAUD_RATE 115200

#define ISP_INCREASE_UART_BAUD_RATE 1100000

#define OTW_K32W0_IMG_HEADER_CRC_OFFSET 40

#define OTW_ISP_TX_MSG_FLAGS_HEADER_SIZE 4
#define OTW_ISP_RX_MSG_FLAGS_HEADER_SIZE 5
#define OTW_ISP_MSG_CRC_SIZE             (sizeof(uint32_t))

#define OTW_MESSAGE_CONTENT_MAX_LEN          640
#define OTW_MESSAGE_BUFFER_TX_HEADER_MAX_LEN 128
#define OTW_MESSAGE_BUFFER_TX_MAX_LEN                                                       \
    OTW_ISP_TX_MSG_FLAGS_HEADER_SIZE + OTW_ISP_MSG_CRC_SIZE + OTW_MESSAGE_CONTENT_MAX_LEN + \
        OTW_MESSAGE_BUFFER_TX_HEADER_MAX_LEN
#define OTW_MESSAGE_BUFFER_RX_MAX_LEN \
    OTW_ISP_RX_MSG_FLAGS_HEADER_SIZE + OTW_ISP_MSG_CRC_SIZE + OTW_MESSAGE_CONTENT_MAX_LEN

#define SIGNATURE_LEN 256

#ifndef OTW_LOG_ENABLED
#define OTW_LOG_ENABLED 0
#endif

#if OTW_LOG_ENABLED
#ifndef OTW_BUFFER_DUMP
#define OTW_BUFFER_DUMP 1
#endif
#include "fsl_debug_console.h"
#define OTW_LOG(...)               \
    PRINTF("[%s] ", __FUNCTION__); \
    PRINTF(__VA_ARGS__);           \
    PRINTF("\n\r");
#if OTW_BUFFER_DUMP
#define OTW_DUMP_BUFFER(buffer, bufferLen, ...)                                        \
    PRINTF("[%s] ", __FUNCTION__);                                                     \
    PRINTF(__VA_ARGS__);                                                               \
    PRINTF(" len = %d = [", bufferLen);                                                \
    for (uint16_t otwDumpIterator = 0; otwDumpIterator < bufferLen; otwDumpIterator++) \
    {                                                                                  \
        PRINTF("%d ", buffer[otwDumpIterator]);                                        \
    }                                                                                  \
    PRINTF("]\n\r");
#else
#define OTW_DUMP_BUFFER(buffer, bufferLen, ...)
#endif
#else
#define OTW_LOG(...)
#define OTW_DUMP_BUFFER(...)
#endif

/* -------------------------------------------------------------------------- */
/*                                Private types                               */
/* -------------------------------------------------------------------------- */

typedef enum
{
    E_BL_MSG_TYPE_NONE           = 0x0,
    E_BL_MSG_TYPE_RESET_REQUEST  = 0x14,
    E_BL_MSG_TYPE_RESET_RESPONSE = 0x15,

    E_BL_MSG_TYPE_RAM_RUN_REQUEST  = 0x21,
    E_BL_MSG_TYPE_RAM_RUN_RESPONSE = 0x22,

    E_BL_MSG_TYPE_SET_BAUD_REQUEST  = 0x27,
    E_BL_MSG_TYPE_SET_BAUD_RESPONSE = 0x28,

    E_BL_MSG_TYPE_GET_CHIPID_REQUEST  = 0x32,
    E_BL_MSG_TYPE_GET_CHIPID_RESPONSE = 0x33,

    TYPE_MEM_OPEN_REQUEST = 0x40,
    TYPE_MEM_OPEN_RESPONSE,
    TYPE_MEM_ERASE_REQUEST,
    TYPE_MEM_ERASE_RESPONSE,
    TYPE_MEM_BLANK_CHECK_REQUEST,
    TYPE_MEM_BLANK_CHECK_RESPONSE,
    TYPE_MEM_READ_REQUEST,
    TYPE_MEM_READ_RESPONSE,
    TYPE_MEM_WRITE_REQUEST,
    TYPE_MEM_WRITE_RESPONSE,
    TYPE_MEM_CLOSE_REQUEST,
    TYPE_MEM_CLOSE_RESPONSE,
    TYPE_MEM_GET_INFO_REQUEST,
    TYPE_MEM_GET_INFO_RESPONSE,

    TYPE_UNLOCK_ISP_REQUEST,
    TYPE_UNLOCK_ISP_RESPONSE,

    TYPE_START_AUTHENTICATION_REQUEST,
    TYPE_START_AUTHENTICATION_RESPONSE,

    TYPE_USE_CERTIFICATE_REQUEST,
    TYPE_USE_CERTIFICATE_RESPONSE,

    TYPE_SET_ENCRYPTION_REQUEST,
    TYPE_SET_ENCRYPTION_RESPONSE,

} teBL_MessageType;

typedef enum
{
    E_BL_RESPONSE_OK                    = 0x00,
    E_BL_RESPONSE_MEMORY_INVALID_MODE   = 0xef,
    E_BL_RESPONSE_NOT_SUPPORTED         = 0xff,
    E_BL_RESPONSE_WRITE_FAIL            = 0xfe,
    E_BL_RESPONSE_INVALID_RESPONSE      = 0xfd,
    E_BL_RESPONSE_CRC_ERROR             = 0xfc,
    E_BL_RESPONSE_ASSERT_FAIL           = 0xfb,
    E_BL_RESPONSE_USER_INTERRUPT        = 0xfa,
    E_BL_RESPONSE_READ_FAIL             = 0xf9,
    E_BL_RESPONSE_TST_ERROR             = 0xf8,
    E_BL_RESPONSE_AUTH_ERROR            = 0xf7,
    E_BL_RESPONSE_NO_RESPONSE           = 0xf6,
    E_BL_RESPONSE_MEMORY_INVALID        = 0xf5,
    E_BL_RESPONSE_MEMORY_NOT_SUPPORTED  = 0xf4,
    E_BL_RESPONSE_MEMORY_ACCESS_INVALID = 0xf3,
    E_BL_RESPONSE_MEMORY_OUT_OF_RANGE   = 0xf2,
    E_BL_RESPONSE_MEMORY_TOO_LONG       = 0xf1,
    E_BL_RESPONSE_BAD_STATE             = 0xf0,
} teBL_Response;

typedef enum
{
    E_BL_FLASH_ID,
    E_BL_PSECT_ID,
    E_BL_pFlash_ID,
    E_BL_Config_ID,
    E_BL_EFUSE_ID,
    E_BL_ROM_ID,
    E_BL_RAM0_ID,
    E_BL_RAM1_ID,
} teBL_MemoryID;

typedef enum
{
    E_READER_STATE_MSG_HEADER_FLAG,
    E_READER_STATE_MSG_CONTENT,
} eOtwIspMsgReaderState;

typedef struct
{
    uint32_t u32BaseAddress;
    uint32_t u32Size;
    uint32_t u32BlockSize;
    uint8_t  u8Index;
    uint8_t  u8ManufacturerID; /**< Flash manufacturer ID */
    uint8_t  u8DeviceID;       /**< Flash device ID */
    uint8_t  u8ChipSelect;     /**< Which SPI chip select for this flash */
    uint8_t  u8Access;
} sOtwK32w0MemFlashInfo;

typedef enum
{
    E_EVENT_FLAG_RX_DATA = 1 << 0,
} eOtwEventFlag;

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

/* default unlock key is 0x11223344556677881122334455667788 */
static const uint8_t otwUnlockKey[] = {17, 34, 51, 68, 85, 102, 119, 136, 17, 34, 51, 68, 85, 102, 119, 136};

/* TX and RX would be accessed by a DMA so data need to be in a noncacheable section */
AT_NONCACHEABLE_SECTION(static uint8_t otwTxMsgBuffer[OTW_MESSAGE_BUFFER_TX_MAX_LEN]);
AT_NONCACHEABLE_SECTION(static uint8_t otwRxMsgBuffer[OTW_MESSAGE_BUFFER_RX_MAX_LEN]);
static uint16_t otwRxMsgBufferReadIndex = 0;
static OSA_EVENT_HANDLE_DEFINE(otwRxEventHandle);

static bool otwIsInitialized = false;
static bool otwIsBusy        = false;

static sOtwK32w0MemFlashInfo otwK32w0MemFlashInfo;

static GPIO_HANDLE_DEFINE(GpioResetHandle);
static GPIO_HANDLE_DEFINE(GpioDIO5Handle);

static UART_HANDLE_DEFINE(g_uartHandle);
static hal_uart_config_t uartConfig = {
    .baudRate_Bps    = ISP_DEFAULT_UART_BAUD_RATE,
    .parityMode      = kHAL_UartParityDisabled,
    .stopBitCount    = kHAL_UartOneStopBit,
    .enableRx        = 1,
    .enableTx        = 1,
    .enableRxRTS     = 0,
    .enableTxCTS     = 0,
    .instance        = OTW_ISP_UART_INSTANCE,
    .txFifoWatermark = 0,
    .rxFifoWatermark = 0,
};

static UART_DMA_HANDLE_DEFINE(g_uartDmaHandle);
#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && (FSL_FEATURE_SOC_DMAMUX_COUNT > 0U))
static const dma_mux_configure_t dma_mux = {
    .dma_dmamux_configure =
        {
            .dma_mux_instance = 0,
            .rx_request       = kDmaRequestMuxLPUART3Rx,
            .tx_request       = kDmaRequestMuxLPUART3Tx,
        },
};
#endif
#if (defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && (FSL_FEATURE_EDMA_HAS_CHANNEL_MUX > 0U))
static const dma_mux_configure_t dma_channel_mux = {
    .dma_dmamux_configure =
        {
            .dma_mux_instance   = 0,
            .dma_tx_channel_mux = kDmaRequestMuxLPUART3Rx,
            .dma_rx_channel_mux = kDmaRequestMuxLPUART3Tx,
        },
};
#endif
static const hal_uart_dma_config_t dmaConfig = {
    .uart_instance = OTW_ISP_UART_INSTANCE,
    .dma_instance  = 0,
    .rx_channel    = LPUART_RX_DMA_CHANNEL,
    .tx_channel    = LPUART_TX_DMA_CHANNEL,
#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && (FSL_FEATURE_SOC_DMAMUX_COUNT > 0U))
    .dma_mux_configure = (void *)&dma_mux,
#else
    .dma_mux_configure         = NULL,
#endif
#if (defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && (FSL_FEATURE_EDMA_HAS_CHANNEL_MUX > 0U))
    .dma_channel_mux_configure = (void *)&dma_channel_mux,
#else
    .dma_channel_mux_configure = NULL,
#endif
};

static const hal_gpio_pin_config_t resetPinConfig = {
    .direction = kHAL_GpioDirectionOut,
    .level     = 0U,
    .port      = OTW_RESET_PIN_PORT,
    .pin       = OTW_RESET_PIN_NUM,
};

static const hal_gpio_pin_config_t DIO5PinConfig = {
    .direction = kHAL_GpioDirectionOut,
    .level     = 0U,
    .port      = OTW_DIO5_PIN_PORT,
    .pin       = OTW_DIO5_PIN_NUM,
};

static const hal_crc_config_t crcConfig = {
    .crcRefIn           = KHAL_CrcRefInput,
    .crcRefOut          = KHAL_CrcRefOutput,
    .crcByteOrder       = KHAL_CrcLSByteFirst,
    .crcSeed            = 0xFFFFFFFF,
    .crcPoly            = KHAL_CrcPolynomial_CRC_32,
    .crcXorOut          = 0xFFFFFFFF,
    .complementChecksum = false,
    .crcSize            = 4,
    .crcStartByte       = 0,
};

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static eOtwStatus Otw_InitUartLink(uint32_t baudRate);
static eOtwStatus Otw_DeInitUartLink(void);
static eOtwStatus Otw_SetUartBaudRate(uint32_t uartBaudRate);
static void       Otw_DmaCallBack(hal_uart_dma_handle_t handle, hal_dma_callback_msg_t *msg, void *callbackParam);
static eOtwStatus Otw_WriteMessageAndGetResponse(teBL_MessageType eReqType,
                                                 teBL_MessageType eExpectedResType,
                                                 uint8_t          headerLength,
                                                 uint8_t *        pu8Header,
                                                 uint16_t         u16DataLength,
                                                 const uint8_t *  pu8Data,
                                                 uint8_t *        responseStatus,
                                                 uint8_t *        responseType,
                                                 uint8_t *        responseBuffer,
                                                 uint16_t *       responseBufferSize);
static eOtwStatus Otw_TryReadRxIspMsg(uint8_t * responseStatus,
                                      uint8_t * responseType,
                                      uint8_t * responseBuffer,
                                      uint16_t *responseBufferSize);
static eOtwStatus Otw_ParseIspResponse(uint8_t * responseStatus,
                                       uint8_t * responseType,
                                       uint16_t  ispMsgLen,
                                       uint8_t * responseBuffer,
                                       uint16_t *responseBufferSize);
static eOtwStatus Otw_SwitchToIspMode(void);
static eOtwStatus Otw_SwapAndCopy(uint8_t *desBuffer, uint8_t *content, uint8_t contentSize);
static eOtwStatus Otw_WriteChunkToFlash(uint32_t writeDstAddr, const uint8_t *bufferToWrite, uint32_t bufferToWriteLen);
static eOtwStatus Otw_ReadChunkFromFlash(uint32_t readAddr, uint8_t *readBuffer, uint32_t readBufferLen);
static eOtwStatus Otw_RunUnlockProcedure(void);
static eOtwStatus Otw_InitMemoryFlashInfo(void);
static eOtwStatus Otw_OpenMemoryRequest(void);
static eOtwStatus Otw_CloseMemoryRequest(void);

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

eOtwStatus Otw_FirmwareDownload(const uint8_t *firmwareData, uint32_t firmwareDataLen, bool verifyAfterWrite)
{
    eOtwStatus otwStatus = E_OTW_OK;

    do
    {
        otwStatus = Otw_Init();
        if (otwStatus != E_OTW_OK)
            break;
        otwStatus = Otw_IsUpdateRequired(firmwareData, firmwareDataLen);
        if (otwStatus == E_OTW_FIRMWARE_UPDATE_REQUIRED)
        {
            otwStatus = Otw_EraseAllFlashProcedure();
            if (otwStatus != E_OTW_OK)
                break;
            OTW_LOG("Erase OK");
            otwStatus = Otw_WriteToFlash(firmwareData, firmwareDataLen, 0);
            if (otwStatus != E_OTW_OK)
                break;
            OTW_LOG("firmware len = %d - write OK", firmwareDataLen);
            if (verifyAfterWrite)
            {
                /* Read the memory and compare with what we have downloaded */
                otwStatus = Otw_ReadFromFlashAndCompare(firmwareData, firmwareDataLen, 0);
                if (otwStatus != E_OTW_OK)
                    break;
                OTW_LOG("firmware len = %d - verify OK", firmwareDataLen);
            }
        }
    } while (false);

    /* Always Deinit the module */
    Otw_DeInit();

    return otwStatus;
}

eOtwStatus Otw_Init(void)
{
    eOtwStatus otwStatus = E_OTW_OK;

    do
    {
        if (otwIsInitialized)
            break;
        /* Init the reset output gpio */
        HAL_GpioInit((hal_gpio_handle_t)GpioResetHandle, (hal_gpio_pin_config_t *)&resetPinConfig);
        HAL_GpioInit((hal_gpio_handle_t)GpioDIO5Handle, (hal_gpio_pin_config_t *)&DIO5PinConfig);
        otwStatus = Otw_SwitchToIspMode();
        if (otwStatus != E_OTW_OK)
            break;
        otwStatus = Otw_InitUartLink(ISP_DEFAULT_UART_BAUD_RATE);
        if (otwStatus != E_OTW_OK)
            break;
        if (OSA_EventCreate((osa_event_handle_t)otwRxEventHandle, true) != KOSA_StatusSuccess)
            break;
        otwStatus = Otw_RunUnlockProcedure();
        if (otwStatus != E_OTW_OK)
            break;
        otwStatus = Otw_InitMemoryFlashInfo();
        if (otwStatus != E_OTW_OK)
            break;
        otwStatus = Otw_OpenMemoryRequest();
        if (otwStatus != E_OTW_OK)
            break;

        otwIsInitialized = true;

    } while (false);

    return otwStatus;
}

eOtwStatus Otw_DeInit(void)
{
    hal_gpio_status_t gpioStatus;
    eOtwStatus        otwStatus = E_OTW_OK;

    do
    {
        if (!otwIsInitialized)
            break;
        otwStatus = Otw_CloseMemoryRequest();
        if (otwStatus != E_OTW_OK)
            break;
        otwStatus = Otw_DeInitUartLink();
        if (otwStatus != E_OTW_OK)
            break;
        if (OSA_EventDestroy((osa_event_handle_t)otwRxEventHandle) != KOSA_StatusSuccess)
        {
            otwStatus = E_OTW_UART_FAILURE;
            break;
        }
        memset(&otwK32w0MemFlashInfo, 0x0, sizeof(otwK32w0MemFlashInfo));
        gpioStatus = HAL_GpioDeinit(GpioResetHandle);
        if (gpioStatus != kStatus_HAL_GpioSuccess)
        {
            otwStatus = E_OTW_GPIO_FAILURE;
            break;
        }
        gpioStatus = HAL_GpioDeinit(GpioDIO5Handle);
        if (gpioStatus != kStatus_HAL_GpioSuccess)
        {
            otwStatus = E_OTW_GPIO_FAILURE;
            break;
        }
        otwIsInitialized = false;
    } while (false);

    return otwStatus;
}

eOtwStatus Otw_Reset(void)
{
    eOtwStatus        otwStatus = E_OTW_OK;
    hal_gpio_status_t gpioStatus;
    do
    {
        if (!otwIsInitialized)
        {
            /* In case we were not in ISP mode, init the reset pin */
            gpioStatus = HAL_GpioInit((hal_gpio_handle_t)GpioResetHandle, (hal_gpio_pin_config_t *)&resetPinConfig);
            if (gpioStatus != kStatus_HAL_GpioSuccess)
            {
                otwStatus = E_OTW_GPIO_FAILURE;
                break;
            }
        }
        // Set Reset pin to low level.
        gpioStatus = HAL_GpioSetOutput((hal_gpio_handle_t)GpioResetHandle, 0);
        if (gpioStatus != kStatus_HAL_GpioSuccess)
        {
            otwStatus = E_OTW_GPIO_FAILURE;
            break;
        }
        OSA_TimeDelay(10);
        // Set Reset pin to high level.
        gpioStatus = HAL_GpioSetOutput((hal_gpio_handle_t)GpioResetHandle, 1);
        if (gpioStatus != kStatus_HAL_GpioSuccess)
        {
            otwStatus = E_OTW_GPIO_FAILURE;
            break;
        }
        OSA_TimeDelay(OTW_CONFIG_DEFAULT_RESET_DELAY_MS);

    } while (false);

    return otwStatus;
}

eOtwStatus Otw_EraseAllFlashProcedure(void)
{
    eOtwStatus status         = E_OTW_OK;
    uint8_t    responseStatus = 0;
    uint8_t    responseType   = 0;
    uint16_t   rxResponseSize = 0;
    uint8_t    au8CmdBuffer[10];

    do
    {
        if (!otwIsInitialized)
        {
            status = E_OTW_MODULE_NOT_INITIALIZED;
            break;
        }
        /* Fill the cmd buffer */
        au8CmdBuffer[0] = (uint8_t)E_BL_FLASH_ID;
        au8CmdBuffer[1] = 0; /* Mode */
        /* Send an erase all flash request */
        memcpy((uint8_t *)&au8CmdBuffer[2], (uint8_t *)&otwK32w0MemFlashInfo.u32BaseAddress, sizeof(uint32_t));
        memcpy((uint8_t *)&au8CmdBuffer[6], (uint8_t *)&otwK32w0MemFlashInfo.u32Size, sizeof(uint32_t));
        status = Otw_WriteMessageAndGetResponse(TYPE_MEM_ERASE_REQUEST, TYPE_MEM_ERASE_RESPONSE, sizeof(au8CmdBuffer),
                                                au8CmdBuffer, 0, NULL, &responseStatus, (uint8_t *)&responseType, NULL,
                                                &rxResponseSize);
        if (status != E_OTW_OK)
            break;
        /* Do a blank check */
        status = Otw_WriteMessageAndGetResponse(TYPE_MEM_BLANK_CHECK_REQUEST, TYPE_MEM_BLANK_CHECK_RESPONSE,
                                                sizeof(au8CmdBuffer), au8CmdBuffer, 0, NULL, &responseStatus,
                                                (uint8_t *)&responseType, NULL, &rxResponseSize);
        if (status != E_OTW_OK)
            break;

    } while (false);

    return status;
}

eOtwStatus Otw_WriteToFlash(const uint8_t *pData, uint32_t dataLen, uint32_t offset)
{
    eOtwStatus status           = E_OTW_OK;
    uint32_t   blockSizeToWrite = 0;
    uint32_t   i                = 0;
    do
    {
        if (!otwIsInitialized)
        {
            status = E_OTW_MODULE_NOT_INITIALIZED;
            break;
        }
        blockSizeToWrite = otwK32w0MemFlashInfo.u32BlockSize;
        if ((offset + dataLen) > otwK32w0MemFlashInfo.u32Size)
        {
            status = E_OTW_FILE_SIZE;
            break;
        }
        while (i < dataLen && status == E_OTW_OK)
        {
            if ((i + blockSizeToWrite) > dataLen)
                blockSizeToWrite = dataLen % blockSizeToWrite;
            status =
                Otw_WriteChunkToFlash(otwK32w0MemFlashInfo.u32BaseAddress + offset + i, pData + i, blockSizeToWrite);
            i += blockSizeToWrite;
        }

    } while (false);

    return status;
}

eOtwStatus Otw_ReadFromFlashAndCompare(const uint8_t *pData, uint32_t dataLen, uint32_t offset)
{
    eOtwStatus status          = E_OTW_OK;
    uint32_t   i               = 0;
    uint8_t *  readBufferChunk = NULL;
    uint32_t   blockSizeToRead = 0;

    do
    {
        if (!otwIsInitialized)
        {
            status = E_OTW_MODULE_NOT_INITIALIZED;
            break;
        }
        blockSizeToRead = otwK32w0MemFlashInfo.u32BlockSize;
        if ((offset + dataLen) > otwK32w0MemFlashInfo.u32Size)
        {
            status = E_OTW_FILE_SIZE;
            break;
        }
        readBufferChunk = OSA_MemoryAllocate(blockSizeToRead);
        while (i < dataLen && status == E_OTW_OK)
        {
            if ((i + blockSizeToRead) > dataLen)
                blockSizeToRead = dataLen - i;
            status = Otw_ReadChunkFromFlash(otwK32w0MemFlashInfo.u32BaseAddress + offset + i, readBufferChunk,
                                            blockSizeToRead);
            if (memcmp(readBufferChunk, pData + i, blockSizeToRead) != 0)
            {
                OTW_LOG("Read addr = 0x%x", otwK32w0MemFlashInfo.u32BaseAddress + offset + i);
                OTW_DUMP_BUFFER(readBufferChunk, blockSizeToRead, "Buffer read");
                OTW_DUMP_BUFFER((pData + i), blockSizeToRead, "Expected value");
                status = E_OTW_READ_COMPARE_FAILURE;
            }
            i += blockSizeToRead;
        }

    } while (false);

    if (readBufferChunk != NULL)
        OSA_MemoryFree(readBufferChunk);

    return status;
}

eOtwStatus Otw_IsUpdateRequired(const uint8_t *firmwareData, uint32_t firmwareDataLen)
{
    eOtwStatus status          = E_OTW_FAILURE;
    uint32_t   signatureOffset = (firmwareDataLen - SIGNATURE_LEN);
    uint32_t   sizeToRead      = SIGNATURE_LEN;

    do
    {
        if (!otwIsInitialized)
        {
            status = E_OTW_MODULE_NOT_INITIALIZED;
            break;
        }
        if (firmwareDataLen < SIGNATURE_LEN)
            break;
        /* align the signatureOffset to a block */
        sizeToRead += (signatureOffset % otwK32w0MemFlashInfo.u32BlockSize);
        signatureOffset = signatureOffset - (signatureOffset % otwK32w0MemFlashInfo.u32BlockSize);
        status          = Otw_ReadFromFlashAndCompare(firmwareData + signatureOffset, sizeToRead, signatureOffset);
        if (status == E_OTW_READ_COMPARE_FAILURE)
        {
            status = E_OTW_FIRMWARE_UPDATE_REQUIRED;
        }
        else
        {
            status = E_OTW_FIRMWARE_UPDATE_NOT_REQUIRED;
        }

    } while (false);

    return status;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static eOtwStatus Otw_InitUartLink(uint32_t baudRate)
{
    eOtwStatus otwStatus = E_OTW_UART_FAILURE;

    do
    {
        /* Retrieve the UART clock rate at runtime as it can depend on clock config */
        uartConfig.srcClock_Hz  = ISP_UART_CLOCK_RATE;
        uartConfig.baudRate_Bps = baudRate;
        if (HAL_UartInit((hal_uart_handle_t)g_uartHandle, &uartConfig) != kStatus_HAL_UartSuccess)
            break;
        if (HAL_UartDMAInit((hal_uart_handle_t)g_uartHandle, (hal_uart_dma_handle_t)g_uartDmaHandle,
                            (hal_uart_dma_config_t *)&dmaConfig) != kStatus_HAL_UartDmaSuccess)
            break;
        if (HAL_UartDMATransferInstallCallback((hal_uart_handle_t)g_uartHandle, Otw_DmaCallBack, NULL) !=
            kStatus_HAL_UartDmaSuccess)
            break;
        otwStatus = E_OTW_OK;
    } while (false);

    return otwStatus;
}

static eOtwStatus Otw_DeInitUartLink(void)
{
    eOtwStatus otwStatus = E_OTW_UART_FAILURE;
    OSA_InterruptDisable();
    do
    {
        if (HAL_UartDMAAbortReceive((hal_uart_handle_t)g_uartHandle) != kStatus_HAL_UartDmaSuccess)
            break;
        if (HAL_UartDMAAbortSend((hal_uart_handle_t)g_uartHandle) != kStatus_HAL_UartDmaSuccess)
            break;
        if (HAL_UartDeinit((hal_uart_handle_t)g_uartHandle) != kStatus_HAL_UartSuccess)
            break;
        if (HAL_UartDMADeinit((hal_uart_handle_t)g_uartHandle) != kStatus_HAL_UartDmaSuccess)
            break;
        otwStatus = E_OTW_OK;
    } while (false);

    OSA_InterruptEnable();
    return otwStatus;
}

static eOtwStatus Otw_SetUartBaudRate(uint32_t uartBaudRate)
{
    eOtwStatus status         = E_OTW_OK;
    uint8_t    responseStatus = 0;
    uint8_t    responseType   = 0;
    uint16_t   rxResponseSize = 0;
    uint8_t    au8CmdBuffer[5];

    do
    {
        /* Fill the cmd buffer */
        au8CmdBuffer[0] = 0;
        memcpy((uint8_t *)&au8CmdBuffer[1], (uint8_t *)&uartBaudRate, sizeof(uint32_t));
        /* Send an read from flash request */
        status = Otw_WriteMessageAndGetResponse(E_BL_MSG_TYPE_SET_BAUD_REQUEST, E_BL_MSG_TYPE_SET_BAUD_RESPONSE,
                                                sizeof(au8CmdBuffer), au8CmdBuffer, 0, NULL, &responseStatus,
                                                (uint8_t *)&responseType, NULL, &rxResponseSize);
        if (status != E_OTW_OK)
            break;

    } while (false);

    return status;
}

static eOtwStatus Otw_WriteMessageAndGetResponse(teBL_MessageType eType,
                                                 teBL_MessageType eExpectedResType,
                                                 uint8_t          headerLength,
                                                 uint8_t *        pu8Header,
                                                 uint16_t         u16DataLength,
                                                 const uint8_t *  pu8Data,
                                                 uint8_t *        responseStatus,
                                                 uint8_t *        responseType,
                                                 uint8_t *        responseBuffer,
                                                 uint16_t *       responseBufferSize)
{
    eOtwStatus status    = E_OTW_OK;
    uint32_t   i         = 0;
    uint16_t   msgLength = OTW_ISP_TX_MSG_FLAGS_HEADER_SIZE + OTW_ISP_MSG_CRC_SIZE + u16DataLength + headerLength;
    uint32_t   crc       = 0;

    do
    {
        if (otwIsBusy)
        {
            status = E_OTW_BUSY;
            break;
        }
        if (msgLength > sizeof(otwTxMsgBuffer))
        {
            status = E_OTW_FAILURE;
            break;
        }
        otwIsBusy = true;
        /* Always abort any pending DMA transfert */
        HAL_UartDMAAbortReceive((hal_uart_handle_t)g_uartHandle);
        HAL_UartDMAAbortSend((hal_uart_handle_t)g_uartHandle);
        /* Clear any pending RX event */
        OSA_EventClear((osa_event_handle_t)otwRxEventHandle, E_EVENT_FLAG_RX_DATA);
        /* Always re-initialize RX read index */
        otwRxMsgBufferReadIndex = 0;
        /* build the msg */
        otwTxMsgBuffer[i++] = 0;
        Otw_SwapAndCopy(&otwTxMsgBuffer[i], (uint8_t *)&msgLength, sizeof(msgLength));
        i += sizeof(msgLength);
        otwTxMsgBuffer[i++] = (uint8_t)eType;
        memcpy(&otwTxMsgBuffer[i], pu8Header, headerLength);
        i += headerLength;
        memcpy(&otwTxMsgBuffer[i], pu8Data, u16DataLength);
        i += u16DataLength;
        /* Calculate CRC of the message */
        crc = HAL_CrcCompute((hal_crc_config_t *)&crcConfig, otwTxMsgBuffer, msgLength - OTW_ISP_MSG_CRC_SIZE);
        Otw_SwapAndCopy(&otwTxMsgBuffer[i], (uint8_t *)&crc, sizeof(crc));
        i += sizeof(crc);
        /* Prepare the dma buffer to receive RX data */
        if (HAL_UartDMATransferReceive((hal_uart_handle_t)g_uartHandle, otwRxMsgBuffer, sizeof(otwRxMsgBuffer),
                                       false) != kStatus_HAL_UartDmaSuccess)
        {
            otwIsBusy = false;
            status    = E_OTW_UART_FAILURE;
            break;
        }
        if (HAL_UartDMATransferSend((hal_uart_handle_t)g_uartHandle, otwTxMsgBuffer, i) != kStatus_HAL_UartDmaSuccess)
        {
            otwIsBusy = false;
            status    = E_OTW_UART_FAILURE;
            break;
        }
        /* A response is now expected, try to read Rx data and to extract response status */
        status    = Otw_TryReadRxIspMsg(responseStatus, responseType, responseBuffer, responseBufferSize);
        otwIsBusy = false;
        if (status != E_OTW_OK || *responseType != eExpectedResType || *responseStatus != E_BL_RESPONSE_OK)
        {
            OTW_DUMP_BUFFER(otwTxMsgBuffer, i, "TX REQ");
            OTW_LOG("status = %d respType = %d respStatus = %d respSize = %d", status, *responseType, *responseStatus,
                    *responseBufferSize);
            status = E_OTW_FAILURE;
        }
    } while (false);

    return status;
}

static eOtwStatus Otw_TryReadRxIspMsg(uint8_t * responseStatus,
                                      uint8_t * responseType,
                                      uint8_t * responseBuffer,
                                      uint16_t *responseBufferSize)
{
    eOtwStatus            status       = E_OTW_OK;
    eOtwIspMsgReaderState currentState = E_READER_STATE_MSG_HEADER_FLAG;
    uint16_t              ispMsgLen    = 0;
    osa_event_flags_t     rxEventFlags = 0;
    osa_status_t          osaStatus;

    do
    {
        osaStatus = OSA_EventWait((osa_event_handle_t)otwRxEventHandle, E_EVENT_FLAG_RX_DATA, 0, 2000, &rxEventFlags);
        /* Block here until RX data is received */
        if (osaStatus != KOSA_StatusSuccess)
        {
            OTW_LOG("osaStatus = %d, otwRxMsgBufferReadIndex = %d", osaStatus, otwRxMsgBufferReadIndex);
            status = E_OTW_UART_FAILURE;
            break;
        }
        if (currentState == E_READER_STATE_MSG_HEADER_FLAG)
        {
            if (otwRxMsgBufferReadIndex >= OTW_ISP_RX_MSG_FLAGS_HEADER_SIZE)
            {
                Otw_SwapAndCopy((uint8_t *)&ispMsgLen, &otwRxMsgBuffer[1], sizeof(ispMsgLen));
                if (ispMsgLen > sizeof(otwRxMsgBuffer) || ispMsgLen < OTW_ISP_RX_MSG_FLAGS_HEADER_SIZE)
                {
                    /* In case the msg is too big break here */
                    status = E_OTW_FAILURE;
                    break;
                }
                currentState = E_READER_STATE_MSG_CONTENT;
            }
        }
        if (currentState == E_READER_STATE_MSG_CONTENT)
        {
            if (otwRxMsgBufferReadIndex >= ispMsgLen)
            {
                /* If the full msg has been read try to parse it */
                status =
                    Otw_ParseIspResponse(responseStatus, responseType, ispMsgLen, responseBuffer, responseBufferSize);
                break;
            }
        }
    } while (true);

    return status;
}

static eOtwStatus Otw_ParseIspResponse(uint8_t * responseStatus,
                                       uint8_t * responseType,
                                       uint16_t  ispMsgLen,
                                       uint8_t * responseBuffer,
                                       uint16_t *responseBufferSize)
{
    eOtwStatus status = E_OTW_OK;
    uint32_t   crcCalculated =
        HAL_CrcCompute((hal_crc_config_t *)&crcConfig, otwRxMsgBuffer, ispMsgLen - OTW_ISP_MSG_CRC_SIZE);
    uint32_t crcFound;
    uint16_t msgContentLen = (ispMsgLen - OTW_ISP_MSG_CRC_SIZE - 5);

    do
    {
        Otw_SwapAndCopy((uint8_t *)&crcFound, &otwRxMsgBuffer[ispMsgLen - OTW_ISP_MSG_CRC_SIZE], sizeof(crcFound));
        if (crcCalculated != crcFound)
        {
            status = E_OTW_CRC_FAILURE;
            break;
        }
        *responseType   = otwRxMsgBuffer[3];
        *responseStatus = otwRxMsgBuffer[4];
        /* Extract the content */
        if (responseBuffer != NULL && *responseBufferSize > 0)
        {
            if (*responseBufferSize > msgContentLen)
                *responseBufferSize = msgContentLen;
            memcpy(responseBuffer, &otwRxMsgBuffer[5], *responseBufferSize);
        }
        if (otwRxMsgBuffer[3] == 0)
        {
            OTW_DUMP_BUFFER(otwRxMsgBuffer, otwRxMsgBufferReadIndex, "RX RES: msgLen = %d", ispMsgLen);
        }

    } while (false);

    return status;
}

static void Otw_DmaCallBack(hal_uart_dma_handle_t handle, hal_dma_callback_msg_t *msg, void *callbackParam)
{
    if (msg->status == kStatus_HAL_UartDmaIdleline)
    {
        otwRxMsgBufferReadIndex += msg->dataSize;
        /* Post an event to indicate that some RX data have been received */
        OSA_EventSet((osa_event_handle_t)otwRxEventHandle, E_EVENT_FLAG_RX_DATA);
    }
}

static eOtwStatus Otw_SwitchToIspMode(void)
{
    eOtwStatus        status = E_OTW_OK;
    hal_gpio_status_t gpioStatus;

    do
    {
        // Set Reset pin to low level.
        gpioStatus = HAL_GpioSetOutput((hal_gpio_handle_t)GpioResetHandle, 0);
        if (gpioStatus != kStatus_HAL_GpioSuccess)
        {
            status = E_OTW_GPIO_FAILURE;
            break;
        }
        // Set DIO5 pin to low level.
        gpioStatus = HAL_GpioSetOutput((hal_gpio_handle_t)GpioDIO5Handle, 0);
        if (gpioStatus != kStatus_HAL_GpioSuccess)
        {
            status = E_OTW_GPIO_FAILURE;
            break;
        }
        OSA_TimeDelay(10);
        // Set Reset pin to high level.
        gpioStatus = HAL_GpioSetOutput((hal_gpio_handle_t)GpioResetHandle, 1);
        if (gpioStatus != kStatus_HAL_GpioSuccess)
        {
            status = E_OTW_GPIO_FAILURE;
            break;
        }
        OSA_TimeDelay(10);
        // Set DIO5 pin to high level.
        gpioStatus = HAL_GpioSetOutput((hal_gpio_handle_t)GpioDIO5Handle, 1);
        if (gpioStatus != kStatus_HAL_GpioSuccess)
        {
            status = E_OTW_GPIO_FAILURE;
            break;
        }
        OSA_TimeDelay(OTW_CONFIG_DEFAULT_RESET_DELAY_MS);
    } while (false);

    return status;
}

static eOtwStatus Otw_SwapAndCopy(uint8_t *desBuffer, uint8_t *content, uint8_t contentSize)
{
    eOtwStatus status = E_OTW_FAILURE;

    if (contentSize == sizeof(uint16_t))
    {
        uint16_t lenToCopy16b = (content[1] | (content[0] << 8));
        memcpy(desBuffer, &lenToCopy16b, sizeof(lenToCopy16b));
        status = E_OTW_OK;
    }
    else if (contentSize == sizeof(uint32_t))
    {
        uint32_t lenToCopy32b = (content[3] | (content[2] << 8) | (content[1] << 16) | (content[0] << 24));
        memcpy(desBuffer, &lenToCopy32b, sizeof(lenToCopy32b));
        status = E_OTW_OK;
    }
    return status;
}

static eOtwStatus Otw_WriteChunkToFlash(uint32_t writeDstAddr, const uint8_t *bufferToWrite, uint32_t bufferToWriteLen)
{
    eOtwStatus status         = E_OTW_OK;
    uint8_t    responseStatus = 0;
    uint8_t    responseType   = 0;
    uint16_t   rxResponseSize = 0;
    uint8_t    au8CmdBuffer[10];

    // OTW_LOG("writeAddr = 0x%x size = %d", writeDstAddr, bufferToWriteLen);

    do
    {
        /* Allows to write only block by block */
        if (bufferToWriteLen > otwK32w0MemFlashInfo.u32BlockSize)
        {
            status = E_OTW_ARG_FAILURE;
            break;
        }
        /* Fill the cmd buffer */
        au8CmdBuffer[0] = (uint8_t)E_BL_FLASH_ID;
        au8CmdBuffer[1] = 0; /* Mode */
        memcpy((uint8_t *)&au8CmdBuffer[2], (uint8_t *)&writeDstAddr, sizeof(uint32_t));
        memcpy((uint8_t *)&au8CmdBuffer[6], (uint8_t *)&bufferToWriteLen, sizeof(uint32_t));
        /* Send an write to flash request */
        status = Otw_WriteMessageAndGetResponse(TYPE_MEM_WRITE_REQUEST, TYPE_MEM_WRITE_RESPONSE, sizeof(au8CmdBuffer),
                                                au8CmdBuffer, bufferToWriteLen, bufferToWrite, &responseStatus,
                                                (uint8_t *)&responseType, NULL, &rxResponseSize);
        if (status != E_OTW_OK)
            break;

    } while (false);

    // OTW_LOG("status = %d", status);

    return status;
}

static eOtwStatus Otw_ReadChunkFromFlash(uint32_t readAddr, uint8_t *readBuffer, uint32_t readBufferLen)
{
    eOtwStatus status         = E_OTW_OK;
    uint8_t    responseStatus = 0;
    uint8_t    responseType   = 0;
    uint16_t   rxResponseSize = readBufferLen;
    uint8_t    au8CmdBuffer[10];

    // OTW_LOG("readAddr = 0x%x size = %d", readAddr, readBufferLen);

    do
    {
        /* Allows to write only block by block */
        if (readBufferLen > otwK32w0MemFlashInfo.u32BlockSize)
        {
            status = E_OTW_ARG_FAILURE;
            break;
        }
        /* Fill the cmd buffer */
        au8CmdBuffer[0] = (uint8_t)E_BL_FLASH_ID;
        au8CmdBuffer[1] = 0; /* Mode */
        memcpy((uint8_t *)&au8CmdBuffer[2], (uint8_t *)&readAddr, sizeof(uint32_t));
        memcpy((uint8_t *)&au8CmdBuffer[6], (uint8_t *)&readBufferLen, sizeof(uint32_t));
        /* Send an read from flash request */
        status = Otw_WriteMessageAndGetResponse(TYPE_MEM_READ_REQUEST, TYPE_MEM_READ_RESPONSE, sizeof(au8CmdBuffer),
                                                au8CmdBuffer, 0, NULL, &responseStatus, (uint8_t *)&responseType,
                                                readBuffer, &rxResponseSize);
        if (status != E_OTW_OK)
            break;

    } while (false);

    // OTW_LOG("status = %d", status);

    return status;
}

static eOtwStatus Otw_OpenMemoryRequest(void)
{
    eOtwStatus status         = E_OTW_OK;
    uint8_t    responseStatus = 0;
    uint8_t    responseType   = 0;
    uint16_t   rxResponseSize = 0;
    uint8_t    au8CmdBuffer[2];

    do
    {
        /* Fill the cmd buffer */
        au8CmdBuffer[0] = (uint8_t)E_BL_FLASH_ID;
        au8CmdBuffer[1] = 0x0f; /* Mode */
        /* Send an open memory request command */
        status = Otw_WriteMessageAndGetResponse(TYPE_MEM_OPEN_REQUEST, TYPE_MEM_OPEN_RESPONSE, 2, au8CmdBuffer, 0, NULL,
                                                &responseStatus, (uint8_t *)&responseType, NULL, &rxResponseSize);
        if (status != E_OTW_OK)
            break;
    } while (false);

    return status;
}

static eOtwStatus Otw_CloseMemoryRequest(void)
{
    eOtwStatus status         = E_OTW_OK;
    uint8_t    responseStatus = 0;
    uint8_t    responseType   = 0;
    uint16_t   rxResponseSize = 0;
    uint8_t    au8CmdBuffer[1];

    do
    {
        /* Fill the cmd buffer */
        au8CmdBuffer[0] = (uint8_t)E_BL_FLASH_ID;
        /* Send a mem close */
        Otw_WriteMessageAndGetResponse(TYPE_MEM_CLOSE_REQUEST, TYPE_MEM_CLOSE_RESPONSE, 1, au8CmdBuffer, 0, NULL,
                                       &responseStatus, (uint8_t *)&responseType, NULL, &rxResponseSize);
        if (status != E_OTW_OK)
            break;
    } while (false);

    return status;
}

static eOtwStatus Otw_RunUnlockProcedure(void)
{
    eOtwStatus status         = E_OTW_OK;
    uint32_t   chipId         = 0;
    uint8_t    unlockMode     = 0;
    uint8_t    responseStatus = 0;
    uint8_t    responseType   = 0;
    uint16_t   responseSize   = 0;

    do
    {
        /* Send unlock with mode 0 */
        status = Otw_WriteMessageAndGetResponse(TYPE_UNLOCK_ISP_REQUEST, TYPE_UNLOCK_ISP_RESPONSE, sizeof(unlockMode),
                                                &unlockMode, 0, NULL, &responseStatus, (uint8_t *)&responseType, NULL,
                                                &responseSize);
        if (status != E_OTW_OK)
            break;
        /* Get the chipId */
        responseSize = sizeof(chipId);
        status =
            Otw_WriteMessageAndGetResponse(E_BL_MSG_TYPE_GET_CHIPID_REQUEST, E_BL_MSG_TYPE_GET_CHIPID_RESPONSE, 0, NULL,
                                           0, NULL, &responseStatus, &responseType, (uint8_t *)&chipId, &responseSize);
        if (status != E_OTW_OK)
            break;
        /* Send unlock with mode 1 */
        unlockMode   = 1;
        responseSize = 0;
        status = Otw_WriteMessageAndGetResponse(TYPE_UNLOCK_ISP_REQUEST, TYPE_UNLOCK_ISP_RESPONSE, sizeof(unlockMode),
                                                &unlockMode, sizeof(otwUnlockKey), otwUnlockKey, &responseStatus,
                                                &responseType, NULL, &responseSize);
        if (status != E_OTW_OK)
            break;
#if ISP_DEFAULT_UART_BAUD_RATE != ISP_INCREASE_UART_BAUD_RATE
        status = Otw_SetUartBaudRate(ISP_INCREASE_UART_BAUD_RATE);
        if (status != E_OTW_OK)
            break;
        status = Otw_DeInitUartLink();
        if (status != E_OTW_OK)
            break;
        status = Otw_InitUartLink(ISP_INCREASE_UART_BAUD_RATE);
        if (status != E_OTW_OK)
            break;
        /* Put a delay here to let some time to switch to the new UART baud rate */
        OSA_TimeDelay(5);
#endif
    } while (false);

    return status;
}

static eOtwStatus Otw_InitMemoryFlashInfo(void)
{
    eOtwStatus status           = E_OTW_OK;
    uint8_t    responseStatus   = 0;
    uint8_t    responseType     = 0;
    uint8_t    memoryFlashId    = (uint8_t)E_BL_FLASH_ID;
    uint8_t *  rxResponseBuffer = NULL;
    uint16_t   rxResponseSize   = OTW_MESSAGE_CONTENT_MAX_LEN;

    do
    {
        rxResponseBuffer = OSA_MemoryAllocate(OTW_MESSAGE_CONTENT_MAX_LEN);
        status           = Otw_WriteMessageAndGetResponse(TYPE_MEM_GET_INFO_REQUEST, TYPE_MEM_GET_INFO_RESPONSE,
                                                sizeof(memoryFlashId), &memoryFlashId, 0, NULL, &responseStatus,
                                                (uint8_t *)&responseType, rxResponseBuffer, &rxResponseSize);

        if (status != E_OTW_OK)
            break;
        /* Fill the flash info structure */
        otwK32w0MemFlashInfo.u8Index = rxResponseBuffer[0];
        memcpy((uint8_t *)&otwK32w0MemFlashInfo.u32BaseAddress, &rxResponseBuffer[1], sizeof(uint32_t));
        memcpy((uint8_t *)&otwK32w0MemFlashInfo.u32Size, &rxResponseBuffer[5], sizeof(uint32_t));
        memcpy((uint8_t *)&otwK32w0MemFlashInfo.u32BlockSize, &rxResponseBuffer[9], sizeof(uint32_t));
        otwK32w0MemFlashInfo.u8Access = rxResponseBuffer[14];
        OTW_LOG("u8Index = %d, baseAddr = 0x%x, size = %d, blockSize = %d access = 0x%x", otwK32w0MemFlashInfo.u8Index,
                otwK32w0MemFlashInfo.u32BaseAddress, otwK32w0MemFlashInfo.u32Size, otwK32w0MemFlashInfo.u32BlockSize,
                otwK32w0MemFlashInfo.u8Access);
    } while (false);

    if (rxResponseBuffer != NULL)
        OSA_MemoryFree(rxResponseBuffer);

    return status;
}