/** @file fw_loader_uart.c
 *
 *  @brief  This file provides firmware download related API for BT only fw download
 *
 *  Copyright 2021-2023 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */
#ifdef CONFIG_BT_IND_DNLD
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#define FW_LOADER_NONBLOCKING
#endif

#if defined(MIMXRT1176_cm7_SERIES) || defined(MIMXRT1175_cm7_SERIES) || defined(MIMXRT1173_cm7_SERIES) || \
defined(MIMXRT1172_SERIES) || defined(MIMXRT1171_SERIES)
#define MIMXRT1170_CPU_SERIES
#endif

#if defined(MIMXRT1062_SERIES) || defined(MIMXRT1061_SERIES)
#define MIMXRT1060_CPU_SERIES
#endif

/*===================== Include Files ============================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#if defined(FW_LOADER_NONBLOCKING)
#include "fsl_os_abstraction.h"
#endif
#include "fw_loader_uart.h"
#include "fwdnld_intf_abs.h"
#if defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
#include "fsl_lpuart.h"
#else
#include "fsl_adapter_uart.h"
#endif // USE_LPUART_DRIVER
#if (defined(MIMXRT1060_CPU_SERIES) || defined(MIMXRT1170_CPU_SERIES))
#include "fsl_iomuxc.h"
#endif /* RT1060 and RT1170 */
#include "eBT_os.h"
#include <board.h>
/*===================== Macros ===================================================*/

// #define DEBUG_PRINT	1U
#define PRINT_PROGRESS_D
// #define TEST_CODE

#ifdef DEBUG_PRINT
#define PRINT(...) printf(__VA_ARGS__)
#define PRINT_ERR(...) printf(__VA_ARGS__)
#else
#define PRINT(...)
#define PRINT_ERR(...) printf(__VA_ARGS__)
#endif // DEBUG_PRINT

#ifdef PRINT_PROGRESS_D
#define PRINT_PROGRESS(...) printf(__VA_ARGS__)
#else
#define PRINT_PROGRESS(...)
#endif // PRINT_PROGRESS

#define SWAPL(x) \
    ((((x) >> 24U) & 0xffU) | (((x) >> 8) & 0xff00U) | (((x) << 8) & 0xff0000UL) | (((x) << 24) & 0xff000000UL))

#define MAX_LENGTH             0xFFFFU // Maximum 2 byte value
#define END_SIG_TIMEOUT        2500U
#define MAX_CTS_TIMEOUT        500U  // 500ms
#define TIMEOUT_VAL_MILLISEC   4000U // Timeout for getting 0xa5 or 0xaa or 0xa7, 2 times of helper timeout
#define WAIT_AFTER_FW_DOWNLOAD 500U
#define STRING_SIZE            6U
#define HDR_LEN                16U
#define CMD4                   0x4U
#define CMD6                   0x6U
#define CMD7                   0x7U
#define FW_LOADER_TIMEOUT      500

#define V1_HEADER_DATA_REQ  0xa5U
#define V1_REQUEST_ACK      0x5aU
#define V1_START_INDICATION 0xaaU

#define V3_START_INDICATION 0xabU
#define V3_HEADER_DATA_REQ  0xa7U
#define V3_REQUEST_ACK      0x7aU
#define V3_TIMEOUT_ACK      0x7bU
#define V3_CRC_ERROR        0x7cU

#define REQ_HEADER_LEN    1U
#define A6REQ_PAYLOAD_LEN 8U
#define AbREQ_PAYLOAD_LEN 3U
#define CRC_LEN           4U

#define CRC_ERR_BIT          1 << 0U
#define NAK_REC_BIT          1 << 1U
#define TIMEOUT_REC_ACK_BIT  1 << 2U
#define TIMEOUT_REC_HEAD_BIT 1 << 3U
#define TIMEOUT_REC_DATA_BIT 1 << 4U
#define INVALID_CMD_REC_BIT  1 << 5U
#define WIFI_MIC_FAIL_BIT    1 << 6U
#define BT_MIC_FAIL_BIT      1 << 7U

#define DI         0x07U
#define POLYNOMIAL 0x04c11db7UL

#define CLKDIVAddr     0x7f00008fU
#define UARTDIVAddr    0x7f000090U
#define UARTMCRAddr    0x7f000091U
#define UARTREINITAddr 0x7f000092U
#define UARTICRAddr    0x7f000093U
#define UARTFCRAddr    0x7f000094U

#define MCR  0x00000022U
#define INIT 0x00000001U
#define ICR  0x000000c7U
#define FCR  0x000000c7U

#define UART_BR_115200  115200U
#define UART_BR_3000000 3000000U

#define FW_LOADER_PKT_SIZE 		10
#define FW_LOADER_BOOT_HEADER	1
#define FW_LOADER_PAYLOAD 		2
#define FW_LOADER_FAILURE       1
#define FW_LOADER_SUCCESS       0
#define FW_LOADER_RETRY_CNT     3
#define FW_LOADER_IGNORE_UNUSED_PARAM(v) (void)(v)
/*==================== Typedefs =================================================*/

#if defined(__GNUC__)
#define FW_LOADER_PACKED_STRUCT struct __attribute__((__packed__))
#define FW_LOADER_PACKED_UNION  union __attribute__((__packed__))
#elif defined(__IAR_SYSTEMS_ICC__)
#define FW_LOADER_PACKED_STRUCT __packed struct
#define FW_LOADER_PACKED_UNION  __packed union
#elif defined(__CC_ARM)
#define FW_LOADER_PACKED_STRUCT struct __attribute__((packed))
#define FW_LOADER_PACKED_UNION  union __attribute__((packed))
#else
#warning No definition for FW_LOADER_PACKED_STRUCT and FW_LOADER_PACKED_UNION!
#endif

/* Definition for True/False */
#ifndef BT_FALSE
#define BT_FALSE                            (UINT8)0U
#endif /* BT_FALSE */

#ifndef BT_TRUE
#define BT_TRUE                             (UINT8)1U
#endif /* BT_TRUE */

typedef FW_LOADER_PACKED_STRUCT {
  uint8_t uiChipId;
  uint8_t uiRev;
  uint8_t uiChipIdComp;
  uint8_t  uiRevComp;
} V1_START_IND;        /* 4 bytes */
typedef FW_LOADER_PACKED_STRUCT {
    uint16_t uiLen;
    uint16_t uiLenComp;
} V1_DATA_REQ;         /* 4 bytes */
typedef FW_LOADER_PACKED_STRUCT {
  uint16_t uiChipId;
  uint8_t  uiLoaderVer;
  uint8_t  uiCrc;
} V3_START_IND;        /* 4 bytes */
typedef FW_LOADER_PACKED_STRUCT {
  uint16_t uiLen;
  uint32_t ulOffset;
  uint16_t uiError;
  uint8_t  uiCrc;
} V3_DATA_REQ;         /* 9 bytes */
typedef FW_LOADER_PACKED_STRUCT {
	uint8_t uiCrc;
} V3_RESP_ACK_NAK;     /* 1 bytes */
typedef FW_LOADER_PACKED_STRUCT {
  uint32_t ulOffset;
  uint8_t uiCrc;
} V3_RESP_TIMEOUT;     /* 5 bytes */

typedef struct _FW_PARSE
{
    /* Packet Pointer */
    UCHAR * packet;

    /* Packet Expected Length */
    UINT16 packet_expected_len;

    /* Packet state */
    UCHAR packet_state;

} FW_PARSE;

typedef FW_LOADER_PACKED_STRUCT {
	uint8_t header;
  union {
    /* Request: 4~9 bytes */
    V1_START_IND    startIndV1;
    V1_DATA_REQ     dataReqV1;
    V3_START_IND    startIndV3;
    V3_DATA_REQ     dataReqV3;
    /* Response: 1~5 bytes */
    V3_RESP_ACK_NAK ackNakV3;
    V3_RESP_TIMEOUT timeoutV3;
    /* Raw */
    uint8_t        raw[FW_LOADER_PKT_SIZE];
  };
} CTRL_FRAME;

CTRL_FRAME rx_frame,uart_frame,tx_frame;

typedef enum
{
    VER1,
    VER2,
    VER3,
} bootLoaderSuppVersion_e;

typedef struct
{
    uint32_t uartBaudRate;
    uint32_t uartDivisio;
    uint32_t uartClkDivisor;
} uart_baudrate_clkDiv_map_t;

typedef struct changeBaudRateConfigData
{
    uint32_t clkDivAddr;
    uint32_t clkDivVal;
    uint32_t uartClkDivAddr;
    uint32_t uartClkDivVal;
    uint32_t mcrAddr;
    uint32_t mcrVal;
    uint32_t reInitAddr;
    uint32_t reInitVal;
    uint32_t icrAddr;
    uint32_t icrVal;
    uint32_t fcrAddr;
    uint32_t fcrVal;
} changeBaudRateConfigData_t;

/* UART Read Task Synchronization */
/* TODO: Check if we need to use any other common define */
#if defined(FW_LOADER_NONBLOCKING)
#define FW_LOADER_MAX_TX_CACHE 2048
#define FW_LOADER_MAX_RX_CACHE 	(16)
#define FW_LOADER_DEFINE_SEMAPHORE_TYPE(type, semaphore) type osa_semaphore_handle_t semaphore;

FW_LOADER_DEFINE_SEMAPHORE_TYPE(DECL_STATIC, fw_loader_frame_rx_cb)
FW_LOADER_DEFINE_SEMAPHORE_TYPE(DECL_STATIC, fw_loader_new_frame_ready)
FW_LOADER_DEFINE_SEMAPHORE_TYPE(DECL_STATIC, fw_loader_frame_tx_cb)

DECL_STATIC UCHAR  fw_loader_rx_state;
DECL_STATIC UINT16 fw_loader_rx_bytes;
DECL_STATIC UCHAR fw_loader_state;
DECL_STATIC BT_thread_type fw_read_task;
DECL_STATIC UCHAR fw_loader_assert;

AT_NONCACHEABLE_SECTION_ALIGN_INIT(DECL_STATIC uint8_t  fw_loader_rx_data_buff[FW_LOADER_MAX_RX_CACHE], 4);
AT_NONCACHEABLE_SECTION_ALIGN_INIT(DECL_STATIC UART_DMA_HANDLE_DEFINE(fw_loader_DmaHandle), 4);
AT_NONCACHEABLE_SECTION_ALIGN_INIT(DECL_STATIC UART_HANDLE_DEFINE(s_controllerHciUartHandle), 4);
AT_NONCACHEABLE_SECTION_ALIGN_INIT(DECL_STATIC uint8_t fw_loader_tx_data_buff[FW_LOADER_MAX_TX_CACHE], 4);

#else
UART_HANDLE_DEFINE(s_controllerHciUartHandle);
#endif /* FW_LOADER_NONBLOCKING */
/*===================== Global Vars ==============================================*/
#ifdef TEST_CODE
DECL_STATIC uint32_t ucTestCase    = 0U;
DECL_STATIC uint32_t ucSleepTimeMs = 0U;
DECL_STATIC uint8_t ucTestDone     = 0U;
DECL_STATIC uint8_t myCrcCorrByte  = 0U;
#endif

#ifdef DEBUG_PRINT
DECL_STATIC uint8_t uiErrCnt[16] = {0};
#endif

DECL_STATIC unsigned char crc8_table[256U]; /* 8-bit table */
DECL_STATIC unsigned long crc_table[256U];
DECL_STATIC int made_table = 0U;

DECL_STATIC uint8_t cmd7_Req                    = BT_FALSE;
DECL_STATIC uint8_t EntryPoint_Req              = BT_FALSE;
DECL_STATIC uint32_t change_baudrata_buffer_len = 0;
DECL_STATIC uint32_t cmd7_change_timeout_len    = 0;

DECL_STATIC long ulTotalFileSize        = 0U; // Size of the File to be downloaded
DECL_STATIC uint32_t ulCurrFileSize     = 0U; // Current size of the Download
DECL_STATIC uint32_t ulLastOffsetToSend = 0xFFFFU;
DECL_STATIC uint8_t uiErrCase           = BT_FALSE;
DECL_STATIC uint8_t b16BytesData        = BT_FALSE;
DECL_STATIC uint16_t uiNewLen;
DECL_STATIC uint32_t ulNewOffset;
DECL_STATIC uint16_t uiNewError;
DECL_STATIC uint8_t uiProVer;
DECL_STATIC uint8_t bVerChecked = BT_FALSE;
DECL_STATIC FW_PARSE fw;
DECL_STATIC fw_download_setting_t fwDownloadConfig;

// CMD5 Header to change bootloader baud rate
DECL_STATIC uint8_t cmd5_hdrData[HDR_LEN] = {0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x2c, 0x00, 0x00, 0x00, 0x77, 0xdb, 0xfd, 0xe0};
// CMD7 Header to change timeout of bootloader
DECL_STATIC uint8_t cmd7_hdrData[HDR_LEN] = {0x07, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x5b, 0x88, 0xf8, 0xba};
// Maximum Length that could be asked by the Helper = 2 bytes
DECL_STATIC uint8_t ucByteBuffer[MAX_LENGTH];

DECL_STATIC const uart_baudrate_clkDiv_map_t uartBaudRate2ClockDivMap[] = {
    {UART_BR_115200, 16U, 0x0075F6FDU},
    {UART_BR_3000000, 1U, 0x00C00000U},
};
/*==================== Function Prototypes ======================================*/

DECL_STATIC void fw_upload_initCrc8(void);
DECL_STATIC void fw_upload_genCrcTable(void);
DECL_STATIC void fw_upload_IoFuncInit(void);
DECL_STATIC bool fw_upload_reInitUartInst(uint32_t baudRate, bool isUartOpen, bool enableFlowCtrl);
DECL_STATIC void fw_upload_deInitUartInst(bool isUartOpen);
DECL_STATIC void fw_upload_storeBytes(uint32_t ulVal, uint8_t uiSize, uint8_t *uiStored);
DECL_STATIC void fw_upload_sendAck(uint8_t uiAck);
DECL_STATIC void fw_upload_v3SendLenBytes(const uint8_t *pFileBuffer, uint16_t uiLenToSend, uint32_t ulOffset);
DECL_STATIC void fw_upload_printUartConfig(struct fwdnldintf *intf);
DECL_STATIC bool fw_upload_initUartConfigData(changeBaudRateConfigData_t *uartConfigData, uint32_t secondaryBr);
DECL_STATIC uint8_t fw_upload_checkReqCrc(uint8_t *uiStr, uint8_t uiReq);
DECL_STATIC uint8_t fw_upload_lenValid(uint16_t *uiLenToSend);
DECL_STATIC uint8_t fw_upload_wait4HdrFrame(uint32_t uiMs);
DECL_STATIC uint8_t fw_upload_wait4Req(uint32_t iSecondBaudRate);
DECL_STATIC uint16_t fw_upload_wait4Length(uint8_t flag);
DECL_STATIC uint16_t fw_upload_getDataLen(uint8_t *buf);
DECL_STATIC uint16_t fw_upload_sendBuffer(uint16_t uiLenToSend, uint8_t *ucBuf, uint8_t uiHighBaudrate);
DECL_STATIC uint16_t fw_upload_v1SendLenBytes(const uint8_t *pFileBuffer, uint16_t uiLenToSend);
DECL_STATIC uint32_t fw_upload_getCmd(uint8_t *buf);
DECL_STATIC unsigned long fw_upload_updateCrc(unsigned long crc_accum, char *data_blk_ptr, int data_blk_size);
DECL_STATIC unsigned char fw_upload_crc8(unsigned char *array, unsigned char len);
/* firmware-upload main api calls */
DECL_STATIC fw_download_uart_status_e fw_upload_changeBaudrate(uint32_t primaryBr,
                                                          uint32_t secondaryBr,
                                                          uint8_t firstWaitHdrSig);
DECL_STATIC fw_download_uart_status_e fw_upload_ChangeTimeout(void);
DECL_STATIC fw_download_uart_status_e fw_upload_firmwareDownload(const unsigned char *wlan_fw_bin,
                                                            unsigned int wlan_fw_bin_len);
/* firmware-download function pointers */
DECL_STATIC fwdnld_intf_ret_t uart_fw_download_prep(struct fwdnldintf *intf, void *param);
DECL_STATIC fwdnld_intf_ret_t uart_fw_download(struct fwdnldintf *intf,
                                          const uint8_t *buff,
                                          uint32_t transfer_len,
                                          uint32_t *transferred_len);

DECL_STATIC int fw_upload_uartWriteFrame(uint8_t *buff, uint32_t buff_size);
DECL_STATIC int fw_upload_uartReadFrame();

#if defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
/* LP UART specific function pointers */
status_t (*fw_upload_ComReadChar)(LPUART_Type *base, uint8_t *data, size_t length);
status_t (*fw_upload_ComWriteChar)(LPUART_Type *base, const uint8_t *data, size_t length);
status_t (*fw_upload_init_uart)(LPUART_Type *base, const lpuart_config_t *config, uint32_t srcClock_Hz);
size_t (*fw_upload_GetBufferSize)(LPUART_Type *base, lpuart_handle_t *handle);
void (*fw_upload_CloseUart)(LPUART_Type *base);
#elif defined(FW_LOADER_NONBLOCKING)
hal_uart_status_t (*fw_upload_init_uart)(hal_uart_handle_t handle, const hal_uart_config_t *config);
hal_uart_dma_status_t (*fw_upload_ComWriteChar)(hal_uart_handle_t handle,  uint8_t *data, size_t length);
hal_uart_dma_status_t (*fw_upload_ComReadChar)(hal_uart_handle_t handle, uint8_t *data, size_t length,bool receiveAll);
hal_uart_status_t (*fw_upload_GetBufferSize)(hal_uart_handle_t handle, uint32_t *reCount);
hal_uart_status_t (*fw_upload_CloseUart)(hal_uart_handle_t handle);
#else
/* HAL UART specific function pointers */
hal_uart_status_t (*fw_upload_init_uart)(hal_uart_handle_t handle, const hal_uart_config_t *config);
hal_uart_status_t (*fw_upload_ComWriteChar)(hal_uart_handle_t handle, const uint8_t *data, size_t length);
hal_uart_status_t (*fw_upload_ComReadChar)(hal_uart_handle_t handle, uint8_t *data, size_t length);
hal_uart_status_t (*fw_upload_GetBufferSize)(hal_uart_handle_t handle, uint32_t *reCount);
hal_uart_status_t (*fw_upload_CloseUart)(hal_uart_handle_t handle);
#endif // defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
INT32 (*fw_upload_GetTime)(void);
void (*fw_upload_DelayInMs)(uint32_t millisec);

DECL_STATIC uint16_t fw_loader_parse_packet (FW_PARSE * fw_pkt)
{
    uint16_t retval;
	if (fw_pkt == NULL)
	{
		retval = FW_LOADER_FAILURE;
		return retval;
	}
	else
	{
		retval = FW_LOADER_SUCCESS;
	}

    /* Next action depends upon which state we are in now */
    switch (fw_pkt->packet_state)
    {
    case FW_LOADER_BOOT_HEADER:

        switch (fw_pkt->packet[0U])
        {
        case V1_HEADER_DATA_REQ:
            fw_pkt->packet_state = FW_LOADER_PAYLOAD;
            fw_pkt->packet_expected_len = 4U;
            break;

        case V3_START_INDICATION:
            fw_pkt->packet_state = FW_LOADER_PAYLOAD;
            fw_pkt->packet_expected_len = 4U;
            break;

        case V3_HEADER_DATA_REQ:
            fw_pkt->packet_state = FW_LOADER_PAYLOAD;
            fw_pkt->packet_expected_len = 9U;
            break;

        default:
        	PRINT_ERR(
        	        "[FW_LOADER]: Invalid Header 0x%02X\n", fw_pkt->packet[0U]);
            /* Reset Transport Packet State Machine */
            fw_pkt->packet_state = FW_LOADER_BOOT_HEADER;
            fw_pkt->packet_expected_len = 1U;
            retval = FW_LOADER_FAILURE;
            break;
        }
        break;

    case FW_LOADER_PAYLOAD:
        fw_pkt->packet_state = FW_LOADER_BOOT_HEADER;
        fw_pkt->packet_expected_len = 1U;
        break;

    default:
    	PRINT_ERR(
        "[FW_LOADER]: Invalid State 0x%02X \n",	fw_pkt->packet_state);
        fw_pkt->packet_state = FW_LOADER_BOOT_HEADER;
        fw_pkt->packet_expected_len = 1U;
        retval = FW_LOADER_FAILURE;
        break;
    }
    return retval;
}

#if defined(FW_LOADER_NONBLOCKING)

DECL_STATIC void fw_loader_uartdma_transmit_cb(hal_uart_dma_handle_t handle,
		hal_dma_callback_msg_t *msg, void *callbackParam)
{
	INT32 ret;
	uint16_t retval;
#ifdef DEBUG_PRINT
	PRINT("[FW-LOADER]In DMA cb \n");
#endif
	if (NULL != handle)
	{
		if ((((hal_uart_dma_status_t) kStatus_HAL_UartDmaRxIdle == msg->status)
				|| (kStatus_HAL_UartDmaIdleline == msg->status))
				&& (msg->dataSize != 0))
		{
			fw_loader_rx_bytes += msg->dataSize;
			retval = fw_loader_parse_packet(&fw);
			if (FW_LOADER_SUCCESS != retval)
			{
				fw_loader_assert = 1U;
			}
			if ((1U == fw.packet_expected_len)
					&& (fw.packet_state == FW_LOADER_BOOT_HEADER))
			{
				fw_loader_rx_state = BT_TRUE;
#ifdef DEBUG_PRINT
				PRINT("[FW-LOADER] Posting Frame \n");
#endif
				ret = OSA_SemaphorePost(&fw_loader_frame_rx_cb);
				if (0U != ret)
				{
					PRINT_ERR("[FW-LOADER] SIGNAL failed (%d)\n", ret);
				}
			}
			if (fw_loader_rx_state == BT_FALSE)
			{
				if (fw_loader_state == BT_TRUE)
				{
#ifdef DEBUG_PRINT
					PRINT("Reading payload %d for %02x, fw_loader_rx_bytes = %d\n",fw.packet_expected_len,fw.packet[0],fw_loader_rx_bytes );
#endif
					ret = fw_upload_ComReadChar(
							(hal_uart_handle_t) s_controllerHciUartHandle,
							&fw.packet[fw_loader_rx_bytes],
							fw.packet_expected_len,
							BT_TRUE);
				}

			}
		}
		else if ((hal_uart_dma_status_t) kStatus_HAL_UartDmaTxIdle
				== msg->status)
		{
#ifdef DEBUG_PRINT
			PRINT("[FW-LOADER] Ack tx frame write\n");
#endif
			ret = OSA_SemaphorePost(&fw_loader_frame_tx_cb);
			if (0U != ret)
			{
				PRINT_ERR("[FW-LOADER] SIGNAL failed (%d)\n", ret);
			}
		}
		else
		{
			assert(0);
		}
	}
}

DECL_STATIC bool fw_loader_uart_dma_init(uint32_t baudRate, bool enableFlowCtrl)
{
	hal_uart_config_t config;
	hal_uart_status_t ret;
	hal_uart_dma_status_t status;
	hal_uart_dma_config_t dmaConfig;

	config.srcClock_Hz = fwDownloadConfig.uartConfig.uartClkFreq;
	config.baudRate_Bps = baudRate;
	config.parityMode = kHAL_UartParityDisabled;
	config.stopBitCount = kHAL_UartOneStopBit;
	config.enableRx = 1U;
	config.enableTx = 1U;
	config.instance = fwDownloadConfig.uartConfig.uartInstance;
	if (enableFlowCtrl)
	{
		config.enableRxRTS = 1;
		config.enableTxCTS = 1;
	}
	else
	{
		config.enableRxRTS = 0;
		config.enableTxCTS = 0;
	}

#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
	config.txFifoWatermark = 0U;
	config.rxFifoWatermark = 0U;
#endif

	ret = fw_upload_init_uart((hal_uart_handle_t) s_controllerHciUartHandle, &config);
	if (ret != kStatus_HAL_UartSuccess)
	{
		PRINT_ERR(
				"[FW-LOADER] FAILED to Open UART Device ret : %d\n",
				ret);
		return BT_FALSE;
	}

	dmaConfig.uart_instance = fwDownloadConfig.uartConfig.uartInstance;
	dmaConfig.dma_instance = fwDownloadConfig.uartConfig.dma_instance;
	dmaConfig.rx_channel = fwDownloadConfig.uartConfig.rx_channel;
	dmaConfig.tx_channel = fwDownloadConfig.uartConfig.tx_channel;

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
	dma_mux_configure_t dma_mux;
	dma_mux.dma_dmamux_configure.dma_mux_instance =
			fwDownloadConfig.uartConfig.dma_mux_instance;
	dma_mux.dma_dmamux_configure.rx_request =
			fwDownloadConfig.uartConfig.rx_request;
	dma_mux.dma_dmamux_configure.tx_request =
			fwDownloadConfig.uartConfig.tx_request;
	dmaConfig.dma_mux_configure = &dma_mux;
#endif

#if (defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && (FSL_FEATURE_EDMA_HAS_CHANNEL_MUX > 0U))
    dma_channel_mux_configure_t dmaChannelMux;
    dmaChannelMux.dma_dmamux_configure.dma_rx_channel_mux = fwDownloadConfig.uartConfig.rx_request;
    dmaChannelMux.dma_dmamux_configure.dma_tx_channel_mux = fwDownloadConfig.uartConfig.tx_request;
    dmaConfig.dma_channel_mux_configure = &dmaChannelMux;
#endif /* (defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && (FSL_FEATURE_EDMA_HAS_CHANNEL_MUX > 0U)) */

	status = HAL_UartDMAInit((hal_uart_handle_t) s_controllerHciUartHandle,
			(hal_uart_dma_handle_t*) fw_loader_DmaHandle, &dmaConfig);
	if (status != kStatus_HAL_UartDmaSuccess)
	{
		PRINT_ERR(
				"[FW-LOADER] FAILED to HAL_UartDMAInit : %d\n",
				status);
		ret = fw_upload_CloseUart((hal_uart_handle_t) s_controllerHciUartHandle);
		if (ret != kStatus_HAL_UartSuccess)
		{
			PRINT_ERR(
					"[FW_LOADER] UART De-Init Failed for UART Instance: %d\n",
					status);
			return BT_FALSE;
		}
	}
	status = HAL_UartDMATransferInstallCallback(
			(hal_uart_handle_t) s_controllerHciUartHandle,
			fw_loader_uartdma_transmit_cb,
			NULL);
	if (status != kStatus_HAL_UartDmaSuccess)
	{
		PRINT_ERR(
				"[FW-LOADER] FAILED to install Callback status: %d\n",
				status);
		status = HAL_UartDMADeinit(
				(hal_uart_handle_t) s_controllerHciUartHandle);
		if (status != kStatus_HAL_UartDmaSuccess)
		{
			PRINT_ERR("[FW_LOADER] UART DMA De-Init Failed: %d\n", status);
			return BT_FALSE;
		}
		else
		{
			ret = fw_upload_CloseUart((hal_uart_handle_t) s_controllerHciUartHandle);
			if (ret != kStatus_HAL_UartSuccess)
			{
				PRINT_ERR("[FW_LOADER] UART De-Init Failed status: %d\n", status);
				return BT_FALSE;
			}
		}
	}
	/* Initialize */
	fw.packet_state = FW_LOADER_BOOT_HEADER;
	fw.packet_expected_len = 1;
	fw_loader_rx_bytes = 0U;
	fw.packet = &fw_loader_rx_data_buff[0U];
	fw_loader_state = BT_TRUE;
	OSA_SemaphorePost(&fw_loader_frame_tx_cb);
#ifdef DEBUG_PRINT
	PRINT("Reading first data after init\n");
#endif
	status = fw_upload_ComReadChar(
			(hal_uart_handle_t) s_controllerHciUartHandle, fw.packet,
			fw.packet_expected_len, BT_TRUE);

	if (status != kStatus_HAL_UartDmaSuccess)
	{
		PRINT_ERR(
				"[FW-LOADER] Read setup failed: %d\n",
				status);
	}
	PRINT("[FW-LOADER] uart bt init complete\n");
	return BT_TRUE;
}

DECL_STATIC bool fw_loader_uart_dma_deinit(void)
{
	bool ret = BT_TRUE;
    OSA_SemaphoreWait (&fw_loader_frame_tx_cb, FW_LOADER_TIMEOUT);
    fw_loader_state = 0x0U;

    if (HAL_UartDMADeinit((hal_uart_handle_t)s_controllerHciUartHandle) != kStatus_HAL_UartDmaSuccess)
    {
    	PRINT_ERR(
        "[FW_LOADER] UART DMA De-Init Failed\n");
    	ret = BT_FALSE;
    }
    else
    {
        if (fw_upload_CloseUart((hal_uart_handle_t)s_controllerHciUartHandle) != kStatus_HAL_UartSuccess)
        {
            PRINT_ERR("[FW_LOADER] UART De-Init Failed\n");
            ret = BT_FALSE;
        }
    }
    return ret;
}

DECL_STATIC BT_THREAD_RETURN_TYPE fw_loader_read_packet_task(BT_THREAD_ARGS args)
{
	PRINT("Read Task Started\n");
    FW_LOADER_IGNORE_UNUSED_PARAM(args);
	while(BT_TRUE)
	{
		(void) OSA_SemaphoreWait(&fw_loader_frame_rx_cb, osaWaitForever_c);
		if (BT_TRUE == fw_loader_state)
		{
			if (fw_loader_assert == 1U)
			{
				PRINT_ERR(
						"Invalid Packet:*%d*: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n",
						fw_loader_rx_bytes, fw_loader_rx_data_buff[0U],
						fw_loader_rx_data_buff[1U], fw_loader_rx_data_buff[2U],
						fw_loader_rx_data_buff[3U], fw_loader_rx_data_buff[4U],
						fw_loader_rx_data_buff[5U], fw_loader_rx_data_buff[6U],
						fw_loader_rx_data_buff[7U], fw_loader_rx_data_buff[8U],
						fw_loader_rx_data_buff[9U]);
				memset(fw_loader_rx_data_buff,0,sizeof(fw_loader_rx_data_buff));
				fw_loader_assert = 0U;
			}
			else
			{
				if (fw_loader_rx_state == BT_TRUE)
				{
					uart_frame.header = fw_loader_rx_data_buff[0];
					for(int i = 0 ; i < fw_loader_rx_bytes; i++)
					{
						uart_frame.raw[i] = fw_loader_rx_data_buff[1+i];
					}
					OSA_SemaphorePost(&fw_loader_new_frame_ready);
				}
			}
			fw_loader_rx_bytes = 0U;
			fw_loader_rx_state = BT_FALSE;
#ifdef DEBUG_PRINT
			PRINT("Reading header %d\n",fw.packet_expected_len);
#endif
			fw_upload_ComReadChar((hal_uart_handle_t) s_controllerHciUartHandle,
					fw.packet, fw.packet_expected_len, BT_TRUE);
		}
	}
}

/** FW_LOADER Initialization */
DECL_STATIC bool fw_loader_init_read (void)
{
	bool ret = BT_FALSE;
    BT_thread_attr_type fw_loader_uart_task_attr;

    OSA_SemaphoreCreateBinary(&fw_loader_frame_rx_cb);
    OSA_SemaphoreCreateBinary(&fw_loader_frame_tx_cb);
    OSA_SemaphoreCreateBinary(&fw_loader_new_frame_ready);

    /* Initialize the Write Task Attributes */
    fw_loader_uart_task_attr.thread_name       = (DECL_CONST CHAR  *)"Fw Load Task";
    fw_loader_uart_task_attr.thread_stack_size = BT_TASK_STACK_DEPTH;
    /* Setting the Priority 1 Higher than the Default EtherMind Tasks */
    fw_loader_uart_task_attr.thread_priority   = (configMAX_PRIORITIES - 3);

    /* Create a thread to receive data From Serial PORT and BUFFER it */
    if (0U != BT_thread_create(&fw_read_task, &fw_loader_uart_task_attr, fw_loader_read_packet_task, NULL))
    {
        PRINT_ERR("[FW-LOADER] read thread create fail\n");
    }
    else
    {
    	ret = BT_TRUE;
    }

	BT_mem_set(fw_loader_rx_data_buff, 0x0, sizeof(fw_loader_rx_data_buff));
	fw_loader_rx_state = BT_FALSE;
	fw_loader_rx_bytes = 0U;
	fw_loader_state = BT_FALSE;

    return  ret;
}
#endif
/*==================== external function =========================================*/
/******************************************************************************
 *
 * Name: uart_init_interface
 *
 * Description:
 *   Initialize firmware download instance for UART intf
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   settings : pointer of fwdnld_intf_t type object
 *
 * Return Value:
 *   settings : pointer of fwdnld_intf_t type object
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
void *uart_init_interface(void)
{
    DECL_STATIC fwdnld_intf_t uartIntf;
    DECL_STATIC fw_download_setting_t fw_download_setting = {
        .uartConfig =
            {
#if defined(MIMXRT1170_CPU_SERIES) && defined(WIFI_BT_USE_M2_INTERFACE)
                LPUART2,                /*FW download UART port address*/
#else
                BOARD_BT_UART_BASEADDR, /*FW download UART port address*/
#endif /*defined(MIMXRT1170_CPU_SERIES)  && defined(WIFI_BT_USE_M2_INTERFACE)*/
                0,                      /*BT UART clock frequency */
                115200U,                /*initial baud-rate for boot-loader*/
                3000000U,               /*secondary baud-rate for boot-loader*/
                0U,                     /*UART parity bits*/
#if defined(MIMXRT1170_CPU_SERIES) && defined(WIFI_BT_USE_M2_INTERFACE)
                2U,                     /*UART Instance*/
#else
                BOARD_BT_UART_INSTANCE, /*UART Instance*/
#endif /*defined(MIMXRT1170_CPU_SERIES)  && defined(WIFI_BT_USE_M2_INTERFACE)*/
                1U,                     /*UART stop bits*/
                BT_TRUE,                /*enableTx*/
                BT_TRUE,                /*enableRx*/
                BT_FALSE,               /*enableRTS*/
                BT_FALSE,               /*enableCTS*/
                BT_TRUE,                /*isSecondaryBaudRate required?*/
#if defined(FW_LOADER_NONBLOCKING)
#if defined(MIMXRT1170_CPU_SERIES) && defined(WIFI_BT_USE_M2_INTERFACE)
				0,						/*dma_instance*/
				4,						/*rx_channel*/
				5,						/*tx_channel*/
				0,						/*dma_mux_instance*/
				kDmaRequestMuxLPUART2Rx,/*rx_request*/
				kDmaRequestMuxLPUART2Tx /*tx_request*/
#elif defined(MCXN947_cm33_core0_SERIES)
				0,						/*dma_instance*/
				1,						/*rx_channel*/
				0,						/*tx_channel*/
				0,						/*dma_mux_instance*/
				kDma0RequestMuxLpFlexcomm2Rx,/*rx_request*/
				kDma0RequestMuxLpFlexcomm2Tx /*tx_request*/
#else
				0,						/*dma_instance*/
				4,						/*rx_channel*/
				5,						/*tx_channel*/
				0,						/*dma_mux_instance*/
				kDmaRequestMuxLPUART3Rx,/*rx_request*/
				kDmaRequestMuxLPUART3Tx /*tx_request*/
#endif
#endif
            },
        BT_FALSE, /*wait4HdrSig*/
        BT_FALSE  /*isFwDownloadRetry*/
    };
#if defined(MIMXRT1170_CPU_SERIES) && defined(WIFI_BT_USE_M2_INTERFACE)
    fw_download_setting.uartConfig.uartClkFreq = CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart2);
#else
    fw_download_setting.uartConfig.uartClkFreq = BOARD_BT_UART_CLK_FREQ;
#endif /*defined(MIMXRT1170_CPU_SERIES) && defined(WIFI_BT_USE_M2_INTERFACE)*/
    uartIntf.intf_s.intf_specific              = &fw_download_setting;
    uartIntf.intf_s.fwdnld_intf_send           = uart_fw_download;
    uartIntf.intf_s.fwdnld_intf_prepare        = uart_fw_download_prep;
    uartIntf.intf_s.fwdnld_intf_check_ready    = NULL;
    uartIntf.intf_s.outbuf                     = NULL;
    uartIntf.intf_type                         = FWDNLD_INTF_UART;
    return (void *)&uartIntf;
}

/*==================== internal function =========================================*/
/******************************************************************************

 *
 * Name: gen_crc_table
 *
 * Description:
 *   Generate crc table
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC void fw_upload_genCrcTable(void)
{
    int i, j;
    unsigned long crc_accum;

    for (i = 0; i < 256; i++)
    {
        crc_accum = ((unsigned long)i << 24);
        for (j = 0; j < 8; j++)
        {
            if (crc_accum & 0x80000000L)
            {
                crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
            }
            else
            {
                crc_accum = (crc_accum << 1);
            }
        }
        crc_table[i] = crc_accum;
    }

    return;
}

/******************************************************************************

 *
 * Name: update_crc
 *
 * Description:
 *   update the CRC on the data block one byte at a time
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   ata_blk_ptr:   the buffer pointer for updating crc.
 *   data_blk_size: the size of buffer
 *
 * Return Value:
 *   CRC value.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
unsigned long fw_upload_updateCrc(unsigned long crc_accum, char *data_blk_ptr, int data_blk_size)
{
    unsigned int i, j;

    for (j = 0; j < data_blk_size; j++)
    {
        i         = ((unsigned int)(crc_accum >> 24) ^ *data_blk_ptr++) & 0xff;
        crc_accum = (crc_accum << 8) ^ crc_table[i];
    }
    return crc_accum;
}

/******************************************************************************
 *
 * Name: fw_upload_IoFuncInit
 *
 * Description:
 *   This function initializes the IO function pointers.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC void fw_upload_IoFuncInit(void)
{
    fw_upload_DelayInMs = OSA_TimeDelay;
    fw_upload_GetTime   = EM_get_time_ms;
#if defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
    fw_upload_ComReadChar   = LPUART_ReadBlocking;
    fw_upload_ComWriteChar  = LPUART_WriteBlocking;
    fw_upload_init_uart     = LPUART_Init;
    fw_upload_GetBufferSize = LPUART_TransferGetRxRingBufferLength;
    fw_upload_CloseUart     = LPUART_Deinit;
#elif defined(FW_LOADER_NONBLOCKING)
    fw_upload_ComReadChar   = HAL_UartDMATransferReceive;
    fw_upload_ComWriteChar  = HAL_UartDMATransferSend;
    fw_upload_init_uart     = HAL_UartInit;
    fw_upload_GetBufferSize = NULL;
    fw_upload_CloseUart     = HAL_UartDeinit;
#else
    fw_upload_ComReadChar   = HAL_UartReceiveBlocking;
    fw_upload_ComWriteChar  = HAL_UartSendBlocking;
    fw_upload_init_uart     = HAL_UartInit;
    fw_upload_GetBufferSize = NULL;
    fw_upload_CloseUart     = HAL_UartDeinit;
#endif // defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
}

/******************************************************************************
 *
 * Name: fw_upload_reInitUartInst
 *
 * Description:
 *   This function reinitializes uart instance with given baud-rate.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   baudRate : baudrate to be configured
 *   isUartOpen : flag is to verify if UART already initialized or not
 *   enableFlowCtrl : enable flow control request
 *
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC bool fw_upload_reInitUartInst(uint32_t baudRate, bool isUartOpen, bool enableFlowCtrl)
{
	bool ret = BT_FALSE;
    PRINT("fw_upload_reInitUartInst(BR %d, isUARTOpen %d, FlowCtrl %d)\n", baudRate, isUartOpen, enableFlowCtrl);

    if (baudRate != 0)
    {
#if defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
        lpuart_config_t config;

        if (isUartOpen)
            fw_upload_CloseUart(fwDownloadConfig.uartConfig.lpUart);

        LPUART_GetDefaultConfig(&config);
        config.baudRate_Bps = baudRate;
        config.enableTx     = fwDownloadConfig.uartConfig.enableTx;
        config.enableRx     = fwDownloadConfig.uartConfig.enableRx;
        if (enableFlowCtrl)
        {
            config.enableRxRTS = BT_TRUE;
            config.enableTxCTS = BT_TRUE;
        }
        else
        {
            config.enableRxRTS = fwDownloadConfig.uartConfig.enableRTS;
            config.enableTxCTS = fwDownloadConfig.uartConfig.enableCTS;
        }

        if(fw_upload_init_uart(fwDownloadConfig.uartConfig.lpUart, &config, fwDownloadConfig.uartConfig.uartClkFreq) == kStatus_HAL_UartSuccess)
        {
        	ret = BT_TRUE;
        }
#elif defined(FW_LOADER_NONBLOCKING)
		if (isUartOpen == BT_FALSE)
		{
			if (fw_loader_init_read())
			{
				if(fw_loader_uart_dma_init(baudRate, enableFlowCtrl) != BT_TRUE)
				{
					PRINT_ERR("Failure in UART DMA Init\n");
				}
				else
				{
					PRINT("UART and DMA Init Success\n");
					ret = BT_TRUE;
				}
			}
		}
        else
        {
        	if(fw_loader_uart_dma_deinit() == BT_TRUE)
        	{
        		if(fw_loader_uart_dma_init(baudRate, enableFlowCtrl) == BT_TRUE)
        		{
        			ret = BT_TRUE;
        		}
        		else
        		{
        			PRINT_ERR("UART and DMA Init fail\n");
        		}
        	}
        	else
        	{
        		PRINT_ERR("UART and DMA deinit failure\n");
        	}

        }

#else
        /* Set the FW_LOADER Configuration parameters */
        if (isUartOpen)
        {
        	fw_upload_CloseUart((hal_uart_handle_t)s_controllerHciUartHandle);
        }


        hal_uart_config_t config;
        memset(&config, 0U, sizeof(config));
        config.srcClock_Hz  = fwDownloadConfig.uartConfig.uartClkFreq;
        config.baudRate_Bps = baudRate;
        config.parityMode   = kHAL_UartParityDisabled;
        config.stopBitCount = kHAL_UartOneStopBit;
        config.enableRx     = fwDownloadConfig.uartConfig.enableRx;
        config.enableTx     = fwDownloadConfig.uartConfig.enableTx;
        config.instance     = fwDownloadConfig.uartConfig.uartInstance;
        if (enableFlowCtrl)
        {
            config.enableRxRTS = BT_TRUE;
            config.enableTxCTS = BT_TRUE;
        }
        else
        {
            config.enableRxRTS = fwDownloadConfig.uartConfig.enableRTS;
            config.enableTxCTS = fwDownloadConfig.uartConfig.enableCTS;
        }
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
        config.txFifoWatermark = 0U;
        config.rxFifoWatermark = 0U;
#endif
        if(fw_upload_init_uart((hal_uart_handle_t)s_controllerHciUartHandle, &config) == kStatus_HAL_UartSuccess)
        {
        	ret = BT_TRUE;
        }
#endif // defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
    }
    PRINT("fw_upload_reInitUartInst status %d\n", ret);
    return ret;
}

/******************************************************************************
 *
 * Name: fw_upload_deInitUartInst
 *
 * Description:
 *   This function deinitialize uart instance.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   isUartOpen : flag to check if UART initialized or not.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC void fw_upload_deInitUartInst(bool isUartOpen)
{
    if (isUartOpen)
    {
#if defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
        fw_upload_CloseUart(fwDownloadConfig.uartConfig.lpUart);
#elif defined(FW_LOADER_NONBLOCKING)
        fw_loader_uart_dma_deinit();
        BT_thread_delete(fw_read_task);
        OSA_SemaphoreDestroy(&fw_loader_frame_rx_cb);
        OSA_SemaphoreDestroy(&fw_loader_new_frame_ready);
		OSA_SemaphoreDestroy(&fw_loader_frame_tx_cb);
#else
        fw_upload_CloseUart((hal_uart_handle_t)s_controllerHciUartHandle);
#endif // defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
    }
}

DECL_STATIC int fw_upload_uartWriteFrame(uint8_t *buff, uint32_t buff_size)
{
	int ret_code = FW_LOADER_FAILURE;
#if defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
    ret_code = fw_upload_ComWriteChar(fwDownloadConfig.uartConfig.lpUart, buff, buff_size);
#elif defined(FW_LOADER_NONBLOCKING)
	uint32_t transfered_len, curr_transfer_len, retry_cnt;
	transfered_len = 0;
	retry_cnt = FW_LOADER_RETRY_CNT;
	while ((transfered_len != buff_size) && (retry_cnt > 0))
	{
		(void) OSA_SemaphoreWait(&fw_loader_frame_tx_cb, FW_LOADER_TIMEOUT);
		curr_transfer_len =
				((buff_size - transfered_len) > FW_LOADER_MAX_TX_CACHE) ?
						FW_LOADER_MAX_TX_CACHE : (buff_size - transfered_len);
		for(int i =0; i < curr_transfer_len; i++)
		{
			fw_loader_tx_data_buff[i] = buff[transfered_len + i];
		}
        ret_code = fw_upload_ComWriteChar((hal_uart_handle_t) s_controllerHciUartHandle,
				fw_loader_tx_data_buff, curr_transfer_len);
		if(ret_code == kStatus_HAL_UartDmaSuccess)
		{
			transfered_len += curr_transfer_len;
			retry_cnt = 3;
		}
		else
		{
			PRINT("Write Failure retrying: retry_cnt = %d \n",retry_cnt);
			retry_cnt -- ;
		}
	}
	if(retry_cnt == 0)
	{
		PRINT_ERR("Failure in writting the data\n");
	}
#else
     ret_code = fw_upload_ComWriteChar(s_controllerHciUartHandle, buff, buff_size);
#endif // defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
	return ret_code;
}

DECL_STATIC int fw_upload_uartReadFrame()
{
	int error;
#if defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
    fw.packet_state = FW_LOADER_BOOT_HEADER;
    	fw.packet_expected_len =1;
        fw.packet = (UCHAR *)&rx_frame;
    	do{
    		error = fw_upload_ComReadChar(fwDownloadConfig.uartConfig.lpUart,fw.packet,fw.packet_expected_len);
    	}while(fw_loader_parse_packet(&fw) && (error == kStatus_Success));

    	if(error == kStatus_Success)
    	{
            fw.packet = (UCHAR *)&rx_frame.raw;
            error = fw_upload_ComReadChar(fwDownloadConfig.uartConfig.lpUart,fw.packet,fw.packet_expected_len);
    	}
#elif defined(FW_LOADER_NONBLOCKING)
#ifdef DEBUG_PRINT
    PRINT("Waiting for frame \n");
#endif
	if (OSA_SemaphoreWait(&fw_loader_new_frame_ready, TIMEOUT_VAL_MILLISEC)
			== 0)
	{
		memcpy(&rx_frame, &uart_frame, sizeof(uart_frame));
		error = 0;
	}
	else
	{
		PRINT_ERR("Failure to read new frame \n");
		error =  1;
	}

#else
	fw.packet_state = FW_LOADER_BOOT_HEADER;
	fw.packet_expected_len =1;
	fw.packet = (UCHAR *)&rx_frame;
	do
    {
	    error = fw_upload_ComReadChar(s_controllerHciUartHandle,fw.packet,fw.packet_expected_len);
	}while(fw_loader_parse_packet(&fw) && (error == kStatus_Success));
	if(error == kStatus_Success)
	{
		fw.packet = (UCHAR *)&rx_frame.raw;
		error = fw_upload_ComReadChar(s_controllerHciUartHandle,fw.packet,fw.packet_expected_len);
	}
#endif //defined(USE_LPUART_DRIVER) && !defined(FW_LOADER_NONBLOCKING)
	return error;
}

/******************************************************************************
 *
 * Name: fw_upload_initCrc8
 *
 * Description:
 *   This function init crc.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC void fw_upload_initCrc8(void)
{
    int i, j;
    int crc;
    if (!made_table)
    {
        for (i = 0; i < 256; i++)
        {
            crc = i;
            for (j = 0; j < 8; j++)
                crc = (crc << 1) ^ ((crc & 0x80) ? DI : 0);
            crc8_table[i] = (unsigned char)((unsigned char)crc & (unsigned char)0xFF);
        }
        made_table = 1;
    }
}

/******************************************************************************
 *
 * Name: fw_upload_crc8
 *
 * Description:
 *   This function calculate crc.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   array: array to be calculated.
 *   len :  len of array.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC unsigned char fw_upload_crc8(unsigned char *array, unsigned char len)
{
    unsigned char CRC = 0xff;
    for (; len > 0; len--)
    {
        CRC = crc8_table[CRC ^ *array];
        array++;
    }
    return CRC;
}

/******************************************************************************
 *
 * Name: fw_upload_wait4HdrSig(uint32_t uiMs)
 *
 * Description:
 *   This function basically waits for reception
 *   of character 0xa5 on UART Rx.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   uiMs:   the expired time.
 *
 * Return Value:
 *   BT_TRUE:   0xa5 or 0xab is received.
 *   BT_FALSE:  0xa5 or 0xab is not received.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC uint8_t fw_upload_wait4HdrFrame(uint32_t uiMs)
{
    uint8_t ucDone     = 0; // signature not Received Yet.
    uint64_t startTime = 0;
    uint64_t currTime  = 0;
    uint8_t bResult    = BT_TRUE;
    startTime          = fw_upload_GetTime();

    while (!ucDone)
    {

        int retcode = fw_upload_uartReadFrame();
        if ((retcode == 0) && ((rx_frame.header == V1_HEADER_DATA_REQ) || (rx_frame.header == V1_START_INDICATION) ||
            (rx_frame.header == V3_START_INDICATION) || (rx_frame.header == V3_HEADER_DATA_REQ)))
        {
            ucDone = 1;
#ifdef DEBUG_PRINT
            PRINT("\nReceived 0x%x ", rx_frame.header);
#endif
            if (!bVerChecked)
            {
                if ((rx_frame.header == V1_HEADER_DATA_REQ) || (rx_frame.header == V1_START_INDICATION))
                {
                    uiProVer = (uint8_t)VER1;
                }
                else
                {
                    uiProVer = (uint8_t)VER3;
                }
                bVerChecked = BT_TRUE;
            }
        }
        else
        {
            if (uiMs)
            {
                currTime = fw_upload_GetTime();
                if (currTime - startTime > uiMs)
                {
                    bResult = BT_FALSE;
#ifdef DEBUG_PRINT
                    PRINT("\ndidnt Received 0x%x , currTime %d startTime %d", rx_frame.header , currTime, startTime);
#endif
                    break;
                }
            }
            fw_upload_DelayInMs(1);
        }
    }
    return bResult;
}

/******************************************************************************
 *
 * Name: fw_upload_wait4Length
 *
 * Description:
 *   This function waits to receive the 4 Byte length.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pFile: The handler of file
 *
 * Return Value:
 *   2 Byte Length to send back to the Helper.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC uint16_t fw_upload_wait4Length(uint8_t flag)
{
    // Length Variables
    uint16_t uiXorOfLen = 0xFFFF;
    uint8_t ack;
    // Check if the length is valid.
    if ((rx_frame.dataReqV1.uiLen ^ rx_frame.dataReqV1.uiLenComp) == uiXorOfLen) // All 1's
    {
#ifdef DEBUG_PRINT
        PRINT("\n       bootloader asks for %d bytes \n ", rx_frame.dataReqV1.uiLen);
#endif
        // Successful. Send back the ack.
        if ((rx_frame.header == V1_HEADER_DATA_REQ) || (rx_frame.header == V1_START_INDICATION))
        {
            ack = V1_REQUEST_ACK;
            fw_upload_uartWriteFrame(&ack, 1);
            if (rx_frame.header == V1_START_INDICATION)
            {
                /* Eliminated longjmp(resync, 1); returning restart status */
                return (uint16_t)V1_START_INDICATION;
            }
        }
    }
    else
    {
#ifdef DEBUG_PRINT
        PRINT("\n    NAK case: bootloader LEN = %x bytes \n ", rx_frame.dataReqV1.uiLen);
        PRINT("\n    NAK case: bootloader LENComp = %x bytes \n ", rx_frame.dataReqV1.uiLenComp);
#endif
        // Failure due to mismatch.
        ack = 0xbf;
        fw_upload_uartWriteFrame(&ack, 1);
        // Start all over again.
        if (flag)
        {
            /* Eliminated longjmp(resync, 1); returning restart status */
            return (uint16_t)V1_START_INDICATION;
        }
        else
        {
        	rx_frame.dataReqV1.uiLen = 0;
        }
    }
    return rx_frame.dataReqV1.uiLen;
}

/******************************************************************************
 *
 * Name: fw_upload_storeBytes
 *
 * Description:
 *   This function stores mul-bytes variable to array.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   ulVal: variable to be stored.
 *   uiSize: size of bytes of this variable.
 *   uiStored: array to store variable.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC void fw_upload_storeBytes(uint32_t ulVal, uint8_t uiSize, uint8_t *uiStored)
{
    uint8_t i;
    for (i = 0; i < uiSize; i++)
    {
        uiStored[i] = (uint8_t)(ulVal >> (i * (uint8_t)8)) & (uint8_t)0xFF;
    }
}

/******************************************************************************
 *
 * Name: fw_upload_sendAck
 *
 * Description:
 *   This function sends ack to per req.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   uiAck: the ack type.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC void fw_upload_sendAck(uint8_t uiAck)
{
    if ((uiAck == V3_REQUEST_ACK) || (uiAck == V3_CRC_ERROR))
    {
#ifdef TEST_CODE
        DECL_STATIC uint8_t ucCalCrc[10U];
    	uint8_t uiAckCrc;
        if (ucRcvdHeader == V3_START_INDICATION)
        {
            // prepare crc for 0x7A or 0x7C
            ucCalCrc[0] = uiAck;
            uiAckCrc    = fw_upload_crc8(ucCalCrc, 1);

            if (ucTestCase == 301 && !ucTestDone)
            {
                PRINT(
                    "\n         TC-%d:  Sleep %dms, NOT send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC "
                    "byte",
                    ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 302 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep %dms, NOT send V3_REQUEST_ACK for Header Signature %02X, send CRC byte",
                      ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 303 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte",
                      ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 304 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, send CRC byte",
                      ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 305 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 306 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, NOT send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 307 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 308 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 309 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte, sleep %dms",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 310 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else
            {
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
            }
        }
        else if (ucRcvdHeader == V3_HEADER_DATA_REQ)
        {
            // prepare crc for 0x7A or 0x7C
            ucCalCrc[0] = uiAck;
            uiAckCrc    = fw_upload_crc8(ucCalCrc, 1);

            if (ucTestCase == 311 && !ucTestDone)
            {
                PRINT(
                    "\n         TC-%d:  Sleep %dms, NOT send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC "
                    "byte",
                    ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 312 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep %dms, NOT send V3_REQUEST_ACK for Header Signature %02X, send CRC byte",
                      ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 313 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte",
                      ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 314 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, send CRC byte",
                      ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 315 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 316 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, NOT send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 317 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 318 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_ComWriteChar((hal_uart_handle_t)s_controllerHciUartHandle, &uiAckCrc, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 319 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte, sleep %dms",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 320 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else
            {
                fw_upload_uartWriteFrame(&uiAck, 1);
                fw_upload_uartWriteFrame(&uiAckCrc, 1);
            }
        }
#else
        // prepare crc for 0x7A or 0x7C
        tx_frame.header = uiAck;
        tx_frame.ackNakV3.uiCrc    = fw_upload_crc8(&tx_frame.header, 1);
#ifdef DEBUG_PRINT
        PRINT("Writing ACK %02x \n",uiAck);
#endif
        fw_upload_uartWriteFrame((uint8_t *)&tx_frame, sizeof(tx_frame.header) + sizeof(tx_frame.ackNakV3.uiCrc));

#endif
    }
    else if (uiAck == V3_TIMEOUT_ACK)
    {
    	// prepare crc for 0x7B
    	tx_frame.header = uiAck;
    	tx_frame.timeoutV3.ulOffset=ulNewOffset;
    	tx_frame.timeoutV3.uiCrc = fw_upload_crc8((uint8_t *)&tx_frame, sizeof(tx_frame.header) + sizeof(tx_frame.timeoutV3) - 1);
#ifdef DEBUG_PRINT
    	PRINT("Writing ACK %02x \n",uiAck);
#endif
        fw_upload_uartWriteFrame((uint8_t *)&tx_frame, sizeof(tx_frame.header) + sizeof(tx_frame.timeoutV3));
    }
    else
    {
        PRINT_ERR("\nNon-empty else statement\n");
    }
#ifdef DEBUG_PRINT
    PRINT("\n ===> ACK = %x, CRC = %x \n", tx_frame.ackNakV3.uiCrc, tx_frame.ackNakV3.uiCrc);
#endif
}
/******************************************************************************
 *
 * Name: fw_upload_checkReqCrc
 *
 * Description:
 *   This function check the request crc.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   uiStr: array to put req header + payload.
 *   uiReq: the request type.
 *
 * Return Value:
 *   result of crc check.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC uint8_t fw_upload_checkReqCrc(uint8_t *uiStr, uint8_t uiReq)
{
    uint8_t uiCalCrc;

    if (uiReq == V3_HEADER_DATA_REQ)
    {
        uiCalCrc = fw_upload_crc8(uiStr, A6REQ_PAYLOAD_LEN + REQ_HEADER_LEN);
        if (uiCalCrc != uiStr[A6REQ_PAYLOAD_LEN + REQ_HEADER_LEN])
        {
            return BT_FALSE;
        }
    }
    else if (uiReq == V3_START_INDICATION)
    {
        uiCalCrc = fw_upload_crc8(uiStr, AbREQ_PAYLOAD_LEN + REQ_HEADER_LEN);
        if (uiCalCrc != uiStr[AbREQ_PAYLOAD_LEN + REQ_HEADER_LEN])
        {
            return BT_FALSE;
        }
    }
    else
    {
    	PRINT_ERR("\nNon-empty else statement\n");
    }

    return BT_TRUE;
}

/******************************************************************************
 *
 * Name: fw_upload_wait4Req
 *
 * Description:
 *   This function waits for req from bootcode or helper.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC uint8_t fw_upload_wait4Req(uint32_t iSecondBaudRate)
{
    uint8_t uiTmp[20] = {0};
    uint8_t bCrcMatch = BT_FALSE;
    uint8_t status    = BT_TRUE;

    if (rx_frame.header == V3_HEADER_DATA_REQ)
    {
#ifdef DEBUG_PRINT
        PRINT("\n <=== REQ = 0xA7, Len = %x,Off = %x,Err = %x,CRC = %x\n ", rx_frame.dataReqV3.uiLen, rx_frame.dataReqV3.ulOffset, rx_frame.dataReqV3.uiError,
        		rx_frame.dataReqV3.uiCrc);
#endif
        // todo use the main array
        uiTmp[0] = V3_HEADER_DATA_REQ;
        fw_upload_storeBytes((uint32_t)rx_frame.dataReqV3.uiLen, sizeof(rx_frame.dataReqV3.uiLen), &uiTmp[1]);
        fw_upload_storeBytes(rx_frame.dataReqV3.ulOffset, sizeof(rx_frame.dataReqV3.ulOffset), &uiTmp[3]);
        fw_upload_storeBytes(rx_frame.dataReqV3.uiError, sizeof(rx_frame.dataReqV3.uiError), &uiTmp[7]);
        uiTmp[9]  = rx_frame.dataReqV3.uiCrc;
        bCrcMatch = fw_upload_checkReqCrc(uiTmp, V3_HEADER_DATA_REQ);
        uiNewLen = rx_frame.dataReqV3.uiLen;
        ulNewOffset = rx_frame.dataReqV3.ulOffset;
        uiNewError = rx_frame.dataReqV3.uiError;
#ifdef TEST_CODE

        if (ucTestCase == 331 && !ucTestDone)
        {
            PRINT("\n         TC-%d:  Simulate Device CRC error on Header Signature 0x%X", ucTestCase, ucRcvdHeader);
            bCrcMatch  = 0;
            ucTestDone = 1;
        }

#endif

        if (!bCrcMatch)
        {
#ifdef DEBUG_PRINT
        	PRINT_ERR("\n === REQ = 0xA7, CRC Mismatched === ");
#endif
            fw_upload_sendAck(V3_CRC_ERROR);
            status = BT_FALSE;
        }
    }
    else if (rx_frame.header == V3_START_INDICATION)
    {
        PRINT("\nChipID is : %x, Version is : %x CRC = %x\n", rx_frame.startIndV3.uiChipId, rx_frame.startIndV3.uiLoaderVer,rx_frame.startIndV3.uiCrc);
        // check crc
        uiTmp[0] = V3_START_INDICATION;
        fw_upload_storeBytes((uint32_t)rx_frame.startIndV3.uiChipId, sizeof(rx_frame.startIndV3.uiChipId), &uiTmp[1]);
        uiTmp[3]  = rx_frame.startIndV3.uiLoaderVer;
        uiTmp[4]  = rx_frame.startIndV3.uiCrc;
        bCrcMatch = fw_upload_checkReqCrc(uiTmp, V3_START_INDICATION);

#ifdef TEST_CODE

        if (ucTestCase == 330 && !ucTestDone)
        {
            PRINT("\n         TC-%d:  Simulate Device CRC error on Header Signature 0x%X", ucTestCase, ucRcvdHeader);
            bCrcMatch  = 0;
            ucTestDone = 1;
        }

#endif

        if (bCrcMatch)
        {
#ifdef DEBUG_PRINT
            PRINT("\n === REQ = 0xAB, CRC Matched === ");
#endif
            fw_upload_sendAck(V3_REQUEST_ACK);
            if (iSecondBaudRate == 0)
            {
                return BT_FALSE;
            }
        }
        else
        {
#ifdef DEBUG_PRINT
        	PRINT_ERR("\n === REQ = 0xAB, CRC Mismatched === ");
#endif
            fw_upload_sendAck(V3_CRC_ERROR);
            status = BT_FALSE;
            if (iSecondBaudRate == 0)
            {
                return BT_FALSE;
            }
        }
    }
    else
    {
        PRINT_ERR("\nNon-empty else statement\n");
    }
    return status;
}

/******************************************************************************
 *
 * Name: fw_upload_getCmd
 *
 * Description:
 *   This function gets CMD value in the header.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   *buf: buffer that stores header and following data.
 *
 * Return Value:
 *   CMD value part in the buffer.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC uint32_t fw_upload_getCmd(uint8_t *buf)
{
    return (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24));
}

/******************************************************************************
 *
 * Name: fw_upload_getDataLen
 *
 * Description:
 *   This function gets buf data length.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   *buf: buffer that stores header and following data.
 *
 * Return Value:
 *   length of data part in the buffer.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC uint16_t fw_upload_getDataLen(uint8_t *buf)
{
    return (buf[8] | (buf[9] << 8));
}

/******************************************************************************
 *
 * Name: fw_upload_lenValid
 *
 * Description:
 *   This function validates the length from 5 bytes request.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   ucArray: store the 5 bytes request.
 *
 * Return Value:
 *   uiLenToSend: if the length is valid, get value from ucArray.
 *
 * Notes:
 *   None.
 *

 *****************************************************************************/
DECL_STATIC uint8_t fw_upload_lenValid(uint16_t *uiLenToSend)
{
    uint16_t uiXorOfLen = 0xFFFF;
    // LEN valid if len & complement match
    if ((rx_frame.dataReqV1.uiLen ^ rx_frame.dataReqV1.uiLenComp) == uiXorOfLen) // All 1's
    {
        *uiLenToSend = rx_frame.dataReqV1.uiLen;
        return BT_TRUE;
    }
    else
    {
        return BT_FALSE;
    }
}


/******************************************************************************
 *
 * Name: fw_upload_sendBuffer
 *
 * Description:
 *   This function sends buffer with header and following data.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *      uiLenToSend: len of header request.
 *            ucBuf: the buf to be sent.
 *   uiHighBaudrate: send the buffer for high baud rate change.
 * Return Value:
 *   Returns the len of next header request.
 *
 * Notes:
 *   None.
 *

 *****************************************************************************/
DECL_STATIC uint16_t fw_upload_sendBuffer(uint16_t uiLenToSend, uint8_t *ucBuf, uint8_t uiHighBaudrate)
{
    uint16_t uiBytesToSend = HDR_LEN, uiFirstChunkSent = 0;
    uint16_t uiDataLen = 0;
    uint8_t ucSentDone = 0;
    uint8_t uiValidLen = BT_FALSE;
    uint8_t tmp;

    // Get data len
    uiDataLen = fw_upload_getDataLen(ucBuf);
    // Send buffer
    while (!ucSentDone)
    {
        if (uiBytesToSend == uiLenToSend)
        {
            // All good
            if ((uiBytesToSend == HDR_LEN) && (!b16BytesData))
            {
                if ((uiFirstChunkSent == 0) || ((uiFirstChunkSent == 1) && (uiErrCase == BT_TRUE)))
                {
                    // Write first 16 bytes of buffer
#ifdef DEBUG_PRINT
                    PRINT("\n====>  Sending first chunk...\n");
                    PRINT("\n====>  Sending %d bytes...\n", uiBytesToSend);
#endif
                    fw_upload_uartWriteFrame(ucBuf, uiBytesToSend);
                    if (cmd7_Req == BT_TRUE || EntryPoint_Req == BT_TRUE)
                    {
                        uiBytesToSend    = HDR_LEN;
                        uiFirstChunkSent = 1;
                    }
                    else
                    {
                        uiBytesToSend    = uiDataLen;
                        uiFirstChunkSent = 0;
                        if (uiBytesToSend == HDR_LEN)
                        {
                            b16BytesData = BT_TRUE;
                        }
                    }
                }
                else
                {
                    // Done with buffer
                    ucSentDone = 1;
                    break;
                }
            }
            else
            {
                // Write remaining bytes
#ifdef DEBUG_PRINT
                PRINT("\n====>  Sending %d bytes...\n", uiBytesToSend);
#endif
                if (uiBytesToSend != 0)
                {
                    fw_upload_uartWriteFrame(&ucBuf[HDR_LEN], uiBytesToSend);
                    uiFirstChunkSent = 1;
                    // We should expect 16, then next block will start
                    uiBytesToSend = HDR_LEN;
                    b16BytesData  = BT_FALSE;
                    if (uiHighBaudrate)
                    {
                        return 0;
                    }
                }
                else // end of bin download
                {
#ifdef DEBUG_PRINT
                    PRINT("\n ========== Download Complete =========\n\n");
#endif
                    return 0;
                }
            }
        }
        else
        {
            // Something not good
            if ((uiLenToSend & 0x01) == 0x01)
            {
                // some kind of error
                if (uiLenToSend == (HDR_LEN + 1))
                {
                    // Send first chunk again
#ifdef DEBUG_PRINT
                	PRINT("\n1. Resending first chunk...\n");
#endif
                    fw_upload_uartWriteFrame(ucBuf, (uiLenToSend - 1));
                    uiBytesToSend    = uiDataLen;
                    uiFirstChunkSent = 0;
                }
                else if (uiLenToSend == (uiDataLen + 1))
                {
                    // Send second chunk again
#ifdef DEBUG_PRINT
                    PRINT("\n2. Resending second chunk...\n");
#endif
                    fw_upload_uartWriteFrame(&ucBuf[HDR_LEN], (uiLenToSend - 1));
                    uiBytesToSend    = HDR_LEN;
                    uiFirstChunkSent = 1;
                }
                else
                {
                    PRINT("\nNon-empty terminating else statement\n");
                }
            }
            else if (uiLenToSend == HDR_LEN)
            {
                // Out of sync. Restart sending buffer
#ifdef DEBUG_PRINT
                PRINT("\n3.  Restart sending the buffer...\n");
#endif
                fw_upload_uartWriteFrame(ucBuf, uiLenToSend);
                uiBytesToSend    = uiDataLen;
                uiFirstChunkSent = 0;
            }
            else if (uiLenToSend == uiDataLen)
            {
                PRINT("Restart sending 2nd chunk...");
                fw_upload_uartWriteFrame(&ucBuf[HDR_LEN], uiLenToSend);
                uiBytesToSend    = HDR_LEN;
                uiFirstChunkSent = 1;
            }
            else
            {
                PRINT("\nNon-empty else statement\n");
            }
        }
        // Get last 5 bytes now
        fw_upload_uartReadFrame();
        // Get next length
        uiValidLen = BT_FALSE;
        do
        {
            if (fw_upload_lenValid(&uiLenToSend) == BT_TRUE)
            {
                // Valid length received
                uiValidLen = BT_TRUE;
#ifdef DEBUG_PRINT
                PRINT("\n Valid length = %d \n", uiLenToSend);
#endif
                // ACK the bootloader
                tmp = V1_REQUEST_ACK;
                fw_upload_uartWriteFrame(&tmp, 1);
#ifdef DEBUG_PRINT
                PRINT("\n  BOOT_HEADER_ACK 0x5a sent \n");
#endif
            }
            else
            {
            	fw_upload_uartReadFrame();
            }
        } while (!uiValidLen);
    }
#ifdef DEBUG_PRINT
    PRINT("\n ========== Buffer is successfully sent =========\n\n");
#endif
    return uiLenToSend;
}

/******************************************************************************
 *
 * Name: fw_upload_v1SendLenBytes
 *
 * Description:
 *   This function sends Len bytes(header+data) to the boot code.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pFileBuffer: bin file buffer being sent.
 *   uiLenTosend: the length will be sent.
 *
 * Return Value:
 *   the 'len' of next header request.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC uint16_t fw_upload_v1SendLenBytes(const uint8_t *pFileBuffer, uint16_t uiLenToSend)
{
    uint16_t ucDataLen, uiLen;
    uint32_t ulCmd;
#ifdef DEBUG_PRINT
    uint16_t i;
#endif
    memset(ucByteBuffer, 0, sizeof(ucByteBuffer));
    cmd7_Req       = BT_FALSE;
    EntryPoint_Req = BT_FALSE;

    if (ulCurrFileSize + uiLenToSend > ulTotalFileSize)
        uiLenToSend = (uint16_t)(ulTotalFileSize - ulCurrFileSize);

    memcpy(&ucByteBuffer[uiLenToSend] - uiLenToSend, pFileBuffer + ulCurrFileSize, uiLenToSend);
    ulCurrFileSize += uiLenToSend;
    ulCmd = fw_upload_getCmd(ucByteBuffer);
    if (ulCmd == CMD7)
    {
        cmd7_Req  = BT_TRUE;
        ucDataLen = 0;
    }
    else
    {
        ucDataLen = fw_upload_getDataLen(ucByteBuffer);
        memcpy(&ucByteBuffer[uiLenToSend], pFileBuffer + ulCurrFileSize, ucDataLen);
        ulCurrFileSize += ucDataLen;
        if ((ulCurrFileSize < ulTotalFileSize) && (ulCmd == CMD6 || ulCmd == CMD4))
        {
            EntryPoint_Req = BT_TRUE;
        }
    }

//#ifdef DEBUG_PRINT
//    PRINT("The buffer is to be sent: %d", uiLenToSend + ucDataLen);
//    for (i = 0; i < (uiLenToSend + ucDataLen); i++)
//    {
//        if (i % 16 == 0)
//        {
//            PRINT("\n");
//        }
//        PRINT(" %02x ", ucByteBuffer[i]);
//    }
//#endif
    // start to send Temp buffer
    uiLen = fw_upload_sendBuffer(uiLenToSend, ucByteBuffer, BT_FALSE);
    PRINT("File downloaded: %8u:%8ld\r", ulCurrFileSize, ulTotalFileSize);

    return uiLen;
}

/******************************************************************************
 *
 * Name: fw_upload_v3SendLenBytes
 *
 * Description:
 *   This function sends Len bytes to the Helper.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pFileBuffer: bin file buffer being sent.
 *   uiLenTosend: the length will be sent.
 *   ulOffset: the offset of current sending.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC void fw_upload_v3SendLenBytes(const uint8_t *pFileBuffer, uint16_t uiLenToSend, uint32_t ulOffset)
{
    // Retransmittion of previous block
    if (ulOffset == ulLastOffsetToSend)
    {
#ifdef DEBUG_PRINT
        PRINT("\nResend offset %d...\n", ulOffset);
#endif
        fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
    }
    else
    {
        // The length requested by the Helper is equal to the Block
        // sizes used while creating the FW.bin. The usual
        // block sizes are 128, 256, 512.
        // uiLenToSend % 16 == 0. This means the previous packet
        // was error free (CRC ok) or this is the first packet received.
        //  We can clear the ucByteBuffer and populate fresh data.
        memset(ucByteBuffer, 0, MAX_LENGTH * sizeof(uint8_t));
        memcpy(ucByteBuffer, pFileBuffer + ulOffset - change_baudrata_buffer_len - cmd7_change_timeout_len,
               uiLenToSend);
        ulCurrFileSize = ulOffset - change_baudrata_buffer_len - cmd7_change_timeout_len + uiLenToSend;
#ifdef TEST_CODE

        if (uiLenToSend == HDR_LEN)
        {
            if (ucTestCase == 321 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleeping for %dms before sending %d bytes HEADER", ucTestCase, ucSleepTimeMs,
                      uiLenToSend);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
                ucTestDone = 1;
            }
            else if (ucTestCase == 322 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send only 8 bytes of 16-byte HEADER, then sleep for %dms", ucTestCase,
                      ucSleepTimeMs);
                fw_upload_uartWriteFrame(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 323 && !ucTestDone)
            {
                PRINT(
                    "\n         TC-%d:  Send 8 bytes of 16-byte HEADER, sleep for %dms, then send remaining 8 bytes "
                    "HEADER",
                    ucTestCase, ucSleepTimeMs);
                fw_upload_uartWriteFrame(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&ucByteBuffer[8], 8);
                ucTestDone = 1;
            }
            else if (ucTestCase == 324 && !ucTestDone)
            {
                PRINT(
                    "\n         TC-%d:  Send 8 bytes of 16-byte HEADER, sleep for %dms, then send full 16 bytes HEADER",
                    ucTestCase, ucSleepTimeMs);
                fw_upload_uartWriteFrame(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
                ucTestDone = 1;
            }
            else if (ucTestCase == 325 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep for %dms, and NOT sending 16-bytes HEADER, but send DATA", ucTestCase,
                      ucSleepTimeMs);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 326 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send 16-byte HEADER with last byte changed to 7C", ucTestCase);
                myCrcCorrByte                 = ucByteBuffer[uiLenToSend - 1];
                ucByteBuffer[uiLenToSend - 1] = 0x7c;
                fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
                ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
                ucTestDone                    = 1;
            }
            else if (ucTestCase == 327 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send 16-byte HEADER with last byte changed to 7C, then sleep for %dms",
                      ucTestCase, ucSleepTimeMs);
                myCrcCorrByte                 = ucByteBuffer[uiLenToSend - 1];
                ucByteBuffer[uiLenToSend - 1] = 0x7c;
                fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
                ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 328 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep for %dms, and NOT sending 16-bytes HEADER, and NOT sending DATA",
                      ucTestCase, ucSleepTimeMs);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else
            {
                fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
            }
        }
        else
        {
            if (ucTestCase == 301 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleeping for %dms before sending %d bytes DATA", ucTestCase, ucSleepTimeMs,
                      uiLenToSend);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
                ucTestDone = 1;
            }
            else if (ucTestCase == 302 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send only first 8 bytes of %d bytes of DATA, then sleep for %dms", ucTestCase,
                      uiLenToSend, ucSleepTimeMs);
                fw_upload_uartWriteFrame(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 303 && !ucTestDone)
            {
                PRINT(
                    "\n         TC-%d:  Send first 8 bytes of %d bytes DATA, sleep for %dms, then send remaining %d "
                    "DATA",
                    ucTestCase, uiLenToSend, ucSleepTimeMs, uiLenToSend - 8);
                fw_upload_uartWriteFrame(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(&ucByteBuffer[8], uiLenToSend - 8);
                ucTestDone = 1;
            }
            else if (ucTestCase == 304 && !ucTestDone)
            {
                PRINT(
                    "\n         TC-%d:  Send first 8 bytes of %d bytes DATA, sleep for %dms, then send full %d bytes "
                    "DATA",
                    ucTestCase, uiLenToSend, ucSleepTimeMs, uiLenToSend);
                fw_upload_uartWriteFrame(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
                ucTestDone = 1;
            }
            else if (ucTestCase == 305 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep for %dms, and NOT sending %d bytes DATA", ucTestCase, ucSleepTimeMs,
                      uiLenToSend);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 306 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send %d bytes DATA with last byte changed to 7C", ucTestCase, uiLenToSend);
                myCrcCorrByte                 = ucByteBuffer[uiLenToSend - 1];
                ucByteBuffer[uiLenToSend - 1] = 0x7c;
                fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
                ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
                ucTestDone                    = 1;
            }
            else if (ucTestCase == 307 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send %d bytes DATA with last byte changed to 7C, then sleep for %dms",
                      ucTestCase, uiLenToSend, ucSleepTimeMs);
                myCrcCorrByte                 = ucByteBuffer[uiLenToSend - 1];
                ucByteBuffer[uiLenToSend - 1] = 0x7c;
                fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
                ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else
            {
                fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);
            }
        }

#else

        fw_upload_uartWriteFrame(ucByteBuffer, uiLenToSend);

#endif
        ulLastOffsetToSend = ulOffset;
    }
}

/******************************************************************************
 *
 * Name: fw_upload_initUartConfigData
 *
 * Description:
 *   fw_upload_initUartConfigData
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC bool fw_upload_initUartConfigData(changeBaudRateConfigData_t *uartConfigData, uint32_t secondaryBr)
{
    bool isSuccess = BT_FALSE;

    if ((uartConfigData != NULL) && (secondaryBr != 0))
    {
        memset(uartConfigData, 0, sizeof(changeBaudRateConfigData_t));
        uartConfigData->clkDivAddr     = CLKDIVAddr;
        uartConfigData->uartClkDivAddr = UARTDIVAddr;
        uartConfigData->fcrAddr        = UARTFCRAddr;
        uartConfigData->fcrVal         = FCR;
        uartConfigData->icrAddr        = UARTICRAddr;
        uartConfigData->icrVal         = ICR;
        uartConfigData->mcrAddr        = UARTMCRAddr;
        uartConfigData->mcrVal         = MCR;
        uartConfigData->reInitAddr     = UARTREINITAddr;
        uartConfigData->reInitVal      = INIT;

        for (uint8_t j = 0; j < sizeof(uartBaudRate2ClockDivMap) / sizeof(uart_baudrate_clkDiv_map_t); j++)
        {
            if (secondaryBr == uartBaudRate2ClockDivMap[j].uartBaudRate)
            {
                uartConfigData->uartClkDivVal = uartBaudRate2ClockDivMap[j].uartDivisio;
                uartConfigData->clkDivVal     = uartBaudRate2ClockDivMap[j].uartClkDivisor;
                isSuccess                     = BT_TRUE;
                break;
            }
        }
    }
    return isSuccess;
}

/******************************************************************************
 *
 * Name: fw_upload_changeBaudrate
 *
 * Description:
 *   This function changes the baud rate of bootrom.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pPortName:        Serial port value.
 *   iFirstBaudRate:   The default baud rate of boot rom.
 *   iSecondBaudRate:  The chaned baud rate.
 *
 * Return Value:
 *   BT_TRUE:            Change baud rate successfully
 *   BT_FALSE:           Change baud rate unsuccessfully
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC fw_download_uart_status_e fw_upload_changeBaudrate(uint32_t primaryBr,
                                                          uint32_t secondaryBr,
                                                          uint8_t firstWaitHdrSig)
{
    changeBaudRateConfigData_t uartConfigData;
    fw_download_uart_status_e status = FW_DOWNLOAD_UART_SECOND_BR_NOT_SUPP;
    uint8_t uartConfig[60];
    uint8_t ucBuffer[80];
    uint32_t headLen             = 0;
    uint32_t uiCrc               = 0;
    uint16_t uiLenToSend         = 0;
    uint8_t flag                 = BT_FALSE;
    uint8_t bRetVal              = BT_FALSE;
    uint8_t ucLoadPayload        = 0;
    uint8_t uiReUsedInitBaudrate = BT_FALSE;

    if (!fw_upload_initUartConfigData(&uartConfigData, secondaryBr))
    {
        return status;
    }

    memset(uartConfig, 0, 60);
    memset(ucBuffer, 0, 80);
    memcpy(uartConfig, &uartConfigData, sizeof(uartConfigData));
    headLen = sizeof(uartConfigData) + CRC_LEN;

    // Generate CRC value for CMD5 payload
    fw_upload_genCrcTable();
    memcpy(cmd5_hdrData + 8, &headLen, 4);
    uiCrc = SWAPL((uint32_t)fw_upload_updateCrc((unsigned long)0, (char *)cmd5_hdrData, (int)12));
    memcpy(cmd5_hdrData + 12, &uiCrc, CRC_LEN);
    uiCrc = SWAPL((uint32_t)fw_upload_updateCrc((unsigned long)0, (char *)uartConfig, (int)sizeof(uartConfigData)));
    memcpy(uartConfig + sizeof(uartConfigData), &uiCrc, CRC_LEN);
    uint16_t configDataLen = sizeof(uartConfigData) + CRC_LEN;

    while (!bRetVal)
    {
    	// Wait to Receive 0xa5, 0xaa, 0xab, 0xa7
    		// If the second baudrate is used, wait for 2s to check 0xa
    	PRINT("\nflag %d firstWaitHdrSig %d\n", flag, firstWaitHdrSig);

    	if(firstWaitHdrSig == 1)
        flag = fw_upload_wait4HdrFrame(TIMEOUT_VAL_MILLISEC);

		if (firstWaitHdrSig && flag)
		{
			if (ucLoadPayload)
			{
				fwDownloadConfig.wait4HdrSig = BT_FALSE;
				if (uiProVer == VER3)
				{
					change_baudrata_buffer_len = (uint32_t) HDR_LEN
							+ (uint32_t) uiNewLen;
				}
				status = FW_DOWNLOAD_UART_SUCCESS;
				break;
			}
		}
		else
		{
			if (uiReUsedInitBaudrate)
			{
				status = FW_DOWNLOAD_UART_HDR_SIG_TIMEOUT;
				break;
			}

			if (ucLoadPayload)
			{
				// If 0xa5 or 0xa7 is not received by using the second baudrate, change baud rate to the first baudrate.

				PRINT_ERR(
						"\n 0xa5 or 0xa7 is not received by using the second baudrate,change baud rate to the first "
								"baudrate\n");
				if(fw_upload_reInitUartInst(primaryBr, BT_TRUE, BT_FALSE) == BT_TRUE)
				{
					ucLoadPayload = 0;
					uiReUsedInitBaudrate = BT_TRUE;
				}
				else
				{
					return status;
				}

				continue;
			}
		}
        PRINT("uiProVer:%d\n", uiProVer);
        firstWaitHdrSig = BT_TRUE;
        if (uiProVer == VER1)
        {
            uiLenToSend = fw_upload_wait4Length(0);
            if (uiLenToSend == V1_START_INDICATION)
            {
                return FW_DOWNLOAD_UART_CHANGE_BR_FAIL;
            }
            if (uiLenToSend == 0)
            {
                continue;
            }
            else if (uiLenToSend == HDR_LEN)
            {
                // Download CMD5 header and Payload packet.
                memcpy(ucBuffer, cmd5_hdrData, HDR_LEN);
                memcpy(ucBuffer + HDR_LEN, uartConfig, configDataLen);
                fw_upload_sendBuffer(uiLenToSend, ucBuffer, BT_TRUE);
                if(fw_upload_reInitUartInst(secondaryBr, BT_TRUE, BT_TRUE) == BT_TRUE)
                {
                	ucLoadPayload = 1;
                }
                else
                {
                	return status;
                }
            }
            else
            {
                // Download CMD5 header and Payload packet
                fw_upload_uartWriteFrame(uartConfig, configDataLen);
                if( fw_upload_reInitUartInst(secondaryBr, BT_TRUE, BT_TRUE) == BT_TRUE)
                {
                	ucLoadPayload = 1;
                }
                else
                {
                	return status;
                }
            }
        }
        else if (uiProVer == VER3)
        {
            flag = fw_upload_wait4Req(secondaryBr);
            if (flag)
            {
                if (uiNewLen != 0 && rx_frame.header == V3_HEADER_DATA_REQ)
                {
                    if (uiNewError == 0)
                    {
                        fw_upload_sendAck(V3_REQUEST_ACK);
                        if (uiNewLen == HDR_LEN)
                        {
#ifdef DEBUG_PRINT
                            PRINT("sending command5...\n");
                            PRINT("command5, len %d, data: ", uiNewLen);
                            for (int i = 0; i < uiNewLen; i++)
                            {
                                PRINT("%02x ", cmd5_hdrData[i]);
                            }
                            PRINT("\n");
#endif
                            fw_upload_uartWriteFrame(cmd5_hdrData, uiNewLen);
                            ulLastOffsetToSend = ulNewOffset;
                        }
                        else
                        {
#ifdef DEBUG_PRINT
                        	PRINT("uartconfig, len %d, data: ", uiNewLen);
                            for (int i = 0; i < uiNewLen; i++)
                            {
                                PRINT("%02x ", uartConfig[i]);
                            }
                            PRINT("\n");
#endif
                            fw_upload_uartWriteFrame(uartConfig, uiNewLen);
                            if(fw_upload_reInitUartInst(secondaryBr, BT_TRUE, BT_TRUE) == BT_TRUE)
                            {
                            	ucLoadPayload = 1;
                            }
                            else
                            {
                            	return status;
                            }

                        }
                    }
                    else // NAK,TIMEOUT,INVALID COMMAND...
                    {
                        fw_upload_sendAck(V3_TIMEOUT_ACK);
                    }
                }
            }
        }
        else
        {
        	PRINT_ERR("\nNon-empty terminating else statement\n");
        }
    }
    return status;
}

/******************************************************************************
 *
 * Name: fw_upload_ChangeTimeout
 *
 * Description:
 *   This function changes timeout value of boot loader
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pPortName:       Com port number.

 * Return Value:
 *   the status  of changing timeout value
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC fw_download_uart_status_e fw_upload_ChangeTimeout(void)
{
    fw_download_uart_status_e status = FW_DOWNLOAD_UART_CHANGE_TIMEOUT_FAIL;
    uint8_t bFirst                   = BT_TRUE;
    uint8_t bRetVal                  = BT_FALSE;
    uint8_t reTryNumber              = 0;
    fw_upload_genCrcTable();

    while (!bRetVal)
    {
        PRINT("fw_upload_ChangeTimeout..\n");
        if (fw_upload_wait4HdrFrame(TIMEOUT_VAL_MILLISEC))
        {
            if (uiProVer == VER3)
            {
                if (fw_upload_wait4Req(1))
                {
                    if (uiNewLen != 0)
                    {
                        if (uiNewError == 0)
                        {
#ifdef DEBUG_PRINT
                            PRINT("\n === Succ: REQ = 0xA7, Errcode = 0 ");
#endif
                            if (bFirst || ulLastOffsetToSend == ulNewOffset)
                            {
                                fw_upload_sendAck(V3_REQUEST_ACK);
#ifdef DEBUG_PRINT
                                PRINT("timeOutCmd, len %d, data: ", uiNewLen);
                                for (int i = 0; i < uiNewLen; i++)
                                {
                                    PRINT("%02x ", cmd7_hdrData[i]);
                                }
                                PRINT("\n");
#endif
                                fw_upload_uartWriteFrame(cmd7_hdrData, uiNewLen);
                                ulLastOffsetToSend = ulNewOffset;
                                bFirst             = BT_FALSE;
                            }
                            else
                            {
                            	PRINT("Change Timeout Complete\n");
                                bRetVal = BT_TRUE;
                                cmd7_change_timeout_len      = HDR_LEN;
                                fwDownloadConfig.wait4HdrSig = BT_FALSE;
                                status  = FW_DOWNLOAD_UART_SUCCESS;
                            }
                        }
                        else
                        {
                            if (reTryNumber < 6)
                            {
                                fw_upload_sendAck(V3_TIMEOUT_ACK);
                                reTryNumber++;
                            }
                            else
                            {
                                bRetVal = BT_TRUE;
                            }
                        }
                    }
                }
            }
            if (uiProVer == VER1)
            {
                status = FW_DOWNLOAD_UART_SUCCESS;
                fwDownloadConfig.wait4HdrSig = BT_FALSE;
                PRINT("Change Timeout Complete V1\n");
                break;
            }
        }
        else
        {
            PRINT("Timeout for waiting header signature in fw_Change_Timeout function\n");
            return status;
        }
    }
    return status;
}

/******************************************************************************
 *
 * Name: fw_upload_firmwareDownload
 *
 * Description:
 *   This function performs the task of FW load over UART.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pPortName:       Com port number.
 *   uartBaudRate:       the initial baud rate.
 *   pFileName:       the file name for downloading.
 *   iSecondBaudRate: the second baud rate.
 *
 * Return Value:
 *   the error code of downloading
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC fw_download_uart_status_e fw_upload_firmwareDownload(const unsigned char *wlan_fw_bin,
                                                            unsigned int wlan_fw_bin_len)
{
    fw_download_uart_status_e status = FW_DOWNLOAD_UART_FAILURE;
    uint16_t uiLenToSend             = 0;
    uint8_t flag                     = BT_FALSE;

    if (wlan_fw_bin != NULL)
    {
    	PRINT("Start download \n");
        fwDownloadConfig.wait4HdrSig = BT_TRUE;
        status                       = fw_upload_ChangeTimeout();
        PRINT("\nfw_upload_ChangeTimeout() ret %d hdr %d\n", status, fwDownloadConfig.wait4HdrSig);
        if (status != FW_DOWNLOAD_UART_SUCCESS)
        {
            return status;
        }

        if (fwDownloadConfig.uartConfig.isSecondaryBaudRateReq != 0)
        {
            PRINT("change baud-rate req to %d\n", fwDownloadConfig.uartConfig.secondaryBaudRate);
            status =
                fw_upload_changeBaudrate(fwDownloadConfig.uartConfig.primaryBaudRate,
                                         fwDownloadConfig.uartConfig.secondaryBaudRate, fwDownloadConfig.wait4HdrSig);
            PRINT("fw_upload_changeBaudrate() ret %d\n", status);
        }

        ulTotalFileSize = (long)wlan_fw_bin_len;
        ulCurrFileSize  = 0;
        PRINT_PROGRESS("download starts(%d)\n", ulTotalFileSize);
        status = FW_DOWNLOAD_UART_INPROGRESS;

        while (status != FW_DOWNLOAD_UART_SUCCESS)
        {
            // Wait to Receive 0xa5, 0xaa, 0xab, 0xa7
        	if(fwDownloadConfig.wait4HdrSig == BT_TRUE)
            flag = fw_upload_wait4HdrFrame(TIMEOUT_VAL_MILLISEC);

            if (!fwDownloadConfig.uartConfig.isSecondaryBaudRateReq && !flag)
            {
                PRINT_ERR("\n0xa5,0xaa,0xab or 0xa7 is not received in %d ms\n", TIMEOUT_VAL_MILLISEC);
                return FW_DOWNLOAD_UART_HDR_SIG_TIMEOUT;
            }

            fwDownloadConfig.uartConfig.isSecondaryBaudRateReq = BT_FALSE;
            fwDownloadConfig.wait4HdrSig = BT_TRUE;
            if (uiProVer == VER1)
            {
                // Read the 'Length' bytes requested by Helper
                uiLenToSend = fw_upload_wait4Length(1);
                if (uiLenToSend == V1_START_INDICATION)
                {
                    //  restart again
                    continue;
                }
                do
                {
                    PRINT("[v1]fw_upload_v1SendLenBytes(%d)\n", uiLenToSend);
                    uiLenToSend = fw_upload_v1SendLenBytes(wlan_fw_bin, uiLenToSend);
                    PRINT_PROGRESS(".");
                } while (uiLenToSend != 0);
                // If the Length requested is 0, download is complete.
                if (uiLenToSend == 0)
                {
                    status = FW_DOWNLOAD_UART_SUCCESS;
                    PRINT_PROGRESS("\ndownload success!\n");
                    break;
                }
            }
            else if (uiProVer == VER3)
            {
                // check if restart required
                if (rx_frame.header == V3_START_INDICATION)
                {
                    fw_upload_wait4Req(0);
                    continue;
                }
                else if (fw_upload_wait4Req(0))
                {
                    if (uiNewLen != 0)
                    {
                        if (uiNewError == 0)
                        {
                            PRINT("\n === Succ: REQ = 0xA7, Errcode = 0 \n");
                            fw_upload_sendAck(V3_REQUEST_ACK);
                            fw_upload_v3SendLenBytes(wlan_fw_bin, uiNewLen, ulNewOffset);
                            PRINT("\n sent %d bytes..\n", uiNewLen);
                        }
                        else // NAK,TIMEOUT,INVALID COMMAND...
                        {
#ifdef DEBUG_PRINT
                            uint8_t i;
                            PRINT("\n === Fail: REQ = 0xA7, Errcode != 0 ");
                            for (i = 0; i < 7; i++)
                            {
                                uiErrCnt[i] += (uiNewError >> i) & 0x1;
                            }
#endif
                            fw_upload_sendAck(V3_TIMEOUT_ACK);
                            if (uiNewError & BT_MIC_FAIL_BIT)
                            {
                                change_baudrata_buffer_len = 0;
                                ulCurrFileSize             = 0;
                                ulLastOffsetToSend         = 0xFFFF;
                            }
                        }
                    }
                    else
                    {
                        /* check if download complete */
                        if (uiNewError == 0)
                        {
                            fw_upload_sendAck(V3_REQUEST_ACK);
                            status = FW_DOWNLOAD_UART_SUCCESS;
                            PRINT_PROGRESS("\ndownload success!\n");
                            break;
                        }
                        else if (uiNewError & BT_MIC_FAIL_BIT)
                        {
#ifdef DEBUG_PRINT
                            uiErrCnt[7] += 1;
#endif
                            fw_upload_sendAck(V3_REQUEST_ACK);
                            change_baudrata_buffer_len = 0;
                            ulCurrFileSize             = 0;
                            ulLastOffsetToSend         = 0xFFFF;
                        }
                        else if (uiNewError & TIMEOUT_REC_ACK_BIT)
                        {
                            // Send ACK when Timeout & Len=0 .
                            fw_upload_sendAck(V3_TIMEOUT_ACK);
                        }
                        else
                        {
                            PRINT("\nNon-empty terminating else statement\n");
                        }
                    }
                }
#ifdef TEST_CODE
                PRINT("\n");
#endif
                PRINT("file download: %8u:%8ld\r", ulCurrFileSize, ulTotalFileSize);
                PRINT_PROGRESS(".");
            }
            else
            {
                status = FW_DOWNLOAD_UART_BL_VER_NOT_SUPP;
            }
        }
    }
    else
    {
        status = FW_DOWNLOAD_UART_INVALID_OBJ;
    }
    return status;
}

/******************************************************************************
 *
 * Name: main
 *
 * Description:
 *   Main Entry point of the application.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
DECL_STATIC fwdnld_intf_ret_t uart_fw_download_prep(struct fwdnldintf *intf, void *param)
{
    fwdnld_intf_ret_t ulResult = FWDNLD_INTF_EPARAM;
    (void)param;

    if ((intf != NULL) && (intf->intf_type == FWDNLD_INTF_UART))
    {
        fw_download_setting_t *fwDownloadSettingObj = intf->intf_s.intf_specific;
        fw_upload_printUartConfig(intf);
        if ((fwDownloadSettingObj != NULL) && (fwDownloadSettingObj->uartConfig.lpUart != NULL))
        {
            fw_download_setting_t *fwDownloadSettingObj = intf->intf_s.intf_specific;

            /* Initialize LP-UART function pointers for UART fw download*/
            fw_upload_IoFuncInit();
            /* Initialize crc8 table for crc validation during UART fw download*/
            fw_upload_initCrc8();

            /* Copy firmware download interface specific instance to global object*/
            memset(&fwDownloadConfig, 0, sizeof(fw_download_setting_t));
            memcpy(&fwDownloadConfig, fwDownloadSettingObj, sizeof(fw_download_setting_t));

            /* Initialize host UART with primary baud-rate */
            PRINT("initialize host UART with primary br %d\n", fwDownloadConfig.uartConfig.primaryBaudRate);
			if (fw_upload_reInitUartInst(
					fwDownloadConfig.uartConfig.primaryBaudRate, BT_FALSE,
					BT_FALSE) == BT_TRUE)
			{
				ulResult = FWDNLD_INTF_SUCCESS;
			}
			else
			{
				ulResult = FWDNLD_INTF_FAIL;
			}
        }
        else
        {
            PRINT("invalid object!\n");
        }
    }

    return ulResult;
}

void wakeUpControllerFromBootSleep(void)
{
#if (defined(MIMXRT1060_CPU_SERIES) ||  defined(MIMXRT1170_CPU_SERIES))

      gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 1U, kGPIO_NoIntmode};

#if (defined(MIMXRT1060_CPU_SERIES)) // For RT1060
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO          GPIO1
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IOMUX    IOMUXC_GPIO_AD_B1_05_GPIO1_IO21
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IO       21
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_REVERT_IOMUX  IOMUXC_GPIO_AD_B1_05_LPUART3_RTS_B
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_IOMUX_CONFIG  0x10B0U
#elif (defined(MIMXRT1170_CPU_SERIES)) // For RT1170
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO          GPIO11
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IOMUX    IOMUXC_GPIO_DISP_B2_13_GPIO11_IO14
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IO       14
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_REVERT_IOMUX  IOMUXC_GPIO_DISP_B2_13_LPUART2_RTS_B
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_IOMUX_CONFIG  0x02U
#endif /* (defined(MIMXRT1060_CPU_SERIES)) */

      IOMUXC_SetPinMux(WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IOMUX,0U);
      IOMUXC_SetPinConfig (WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IOMUX, WAKEUP_FROM_BOOTSLEEP_TRIGGER_IOMUX_CONFIG);

      GPIO_PinInit(WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO, WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IO, &gpio_config);
      GPIO_WritePinOutput(WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO, WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IO, 0);

      fw_upload_DelayInMs(10);

      /*reconfiguring pin-mux for UART-RTS*/
      IOMUXC_SetPinMux(WAKEUP_FROM_BOOTSLEEP_TRIGGER_REVERT_IOMUX,0U);
      IOMUXC_SetPinConfig (WAKEUP_FROM_BOOTSLEEP_TRIGGER_REVERT_IOMUX, WAKEUP_FROM_BOOTSLEEP_TRIGGER_IOMUX_CONFIG);
#endif /* RT1060 and RT1170 */
}

DECL_STATIC fwdnld_intf_ret_t uart_fw_download(struct fwdnldintf *intf,
                                          const uint8_t *buff,
                                          uint32_t transfer_len,
                                          uint32_t *transferred_len)
{
    fwdnld_intf_ret_t status = FWDNLD_INTF_FAIL;

    if ((intf != NULL) && (buff != NULL))
    {
        wakeUpControllerFromBootSleep();
        PRINT("fw-download start!\n");
        status = (fwdnld_intf_ret_t)fw_upload_firmwareDownload(buff, transfer_len);
        PRINT("fw-download %s(ret:%d)!!!\n", (status != FW_DOWNLOAD_UART_SUCCESS) ? "failed" : "success", status);
        fw_upload_DelayInMs(WAIT_AFTER_FW_DOWNLOAD);
        fw_upload_deInitUartInst(BT_TRUE);
    }

    return status;
}

DECL_STATIC void fw_upload_printUartConfig(struct fwdnldintf *intf)
{
    PRINT("==========================================================\n");
    PRINT("fw_upload_printUartConfig\n");
    PRINT("==========================================================\n");
    if (intf != NULL)
    {
        PRINT("intf_type %d\n", intf->intf_type);
        PRINT("fwdnld_intf_send %p\n", intf->intf_s.fwdnld_intf_send);
        PRINT("intf_specific %p\n", intf->intf_s.intf_specific);
        if (intf->intf_s.intf_specific != NULL)
        {
            fw_download_setting_t *config = intf->intf_s.intf_specific;
            PRINT("primary br %d\n", config->uartConfig.primaryBaudRate);
            PRINT("secondary br %d\n", config->uartConfig.secondaryBaudRate);
            PRINT("IssecondaryBrReq %d\n", config->uartConfig.isSecondaryBaudRateReq);
            PRINT("enableRx %d\n", config->uartConfig.enableRx);
            PRINT("enableTx %d\n", config->uartConfig.enableTx);
            PRINT("enableRTS %d\n", config->uartConfig.enableRTS);
            PRINT("enableCTS %d\n", config->uartConfig.enableCTS);
            PRINT("parity %d\n", config->uartConfig.parity);
            PRINT("stopbits %d\n", config->uartConfig.stopBits);
            PRINT("wait4HdrSig %d\n", config->wait4HdrSig);
            PRINT("isFwDownloadRetry %d\n", config->isFwDownloadRetry);
#ifdef FW_LOADER_NONBLOCKING
            PRINT("dma_instance %d\n", config->uartConfig.dma_instance);
            PRINT("rx_channel %d\n", config->uartConfig.rx_channel);
            PRINT("tx_channel %d\n", config->uartConfig.tx_channel);
            PRINT("dma_mux_instance %d\n", config->uartConfig.dma_mux_instance);
            PRINT("rx_request %d\n", config->uartConfig.rx_request);
            PRINT("tx_request %d\n", config->uartConfig.tx_request);
#endif
            (void) config;
        }
    }
    else
    {
        PRINT("intf is NULL\n");
    }
    PRINT("==========================================================\n");
}
#endif /*CONFIG_BT_IND_DNLD*/
