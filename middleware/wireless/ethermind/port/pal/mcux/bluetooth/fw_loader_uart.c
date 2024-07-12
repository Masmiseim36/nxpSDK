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
/*===================== Include Files ============================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "fw_loader_uart.h"
#include "fwdnld_intf_abs.h"
#ifdef USE_LPUART_DRIVER
#include "fsl_lpuart.h"
#else
#include "fsl_adapter_uart.h"
#endif // USE_LPUART_DRIVER
#if (defined(CPU_MIMXRT1062DVMAA_cm7) || defined(CPU_MIMXRT1062DVL6A_cm7) ||  defined(CPU_MIMXRT1062DVL6B_cm7) || defined(CPU_MIMXRT1176DVMAA_cm7))
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
#else
#define PRINT(...)
#endif // DEBUG_PRINT

#ifdef PRINT_PROGRESS_D
#define PRINT_PROGRESS(...) printf(__VA_ARGS__)
#else
#define PRINT_PROGRESS(...)
#endif // PRINT_PROGRESS

#define SWAPL(x) \
    ((((x) >> 24U) & 0xffU) | (((x) >> 8) & 0xff00U) | (((x) << 8) & 0xff0000UL) | (((x) << 24) & 0xff000000UL))

#define BIT(x) (0x1U << x)
#define TRUE   1U
#define FALSE  0U

#define MAX_LENGTH             0xFFFFU // Maximum 2 byte value
#define END_SIG_TIMEOUT        2500U
#define MAX_CTS_TIMEOUT        500U  // 500ms
#define TIMEOUT_VAL_MILLISEC   4000U // Timeout for getting 0xa5 or 0xaa or 0xa7, 2 times of helper timeout
#define WAIT_AFTER_FW_DOWNLOAD 1000U
#define STRING_SIZE            6U
#define HDR_LEN                16U
#define CMD4                   0x4U
#define CMD6                   0x6U
#define CMD7                   0x7U

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

/*==================== Typedefs =================================================*/
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

/*===================== Global Vars ==============================================*/
#ifdef TEST_CODE
static uint32_t ucTestCase    = 0U;
static uint32_t ucSleepTimeMs = 0U;
static uint8_t ucTestDone     = 0U;
static uint8_t myCrcCorrByte  = 0U;
// static uint8_t myChangeCrc = 0U;
#endif

#ifdef DEBUG_PRINT
static uint8_t uiErrCnt[16] = {0};
#endif

static unsigned char crc8_table[256U]; /* 8-bit table */
static unsigned long crc_table[256U];
static uint8_t ucCalCrc[10U];
static int made_table = 0U;

static uint8_t cmd7_Req                    = FALSE;
static uint8_t EntryPoint_Req              = FALSE;
static uint32_t change_baudrata_buffer_len = 0;
static uint32_t cmd7_change_timeout_len    = 0;

static long ulTotalFileSize        = 0U; // Size of the File to be downloaded
static uint32_t ulCurrFileSize     = 0U; // Current size of the Download
static uint32_t ulLastOffsetToSend = 0xFFFFU;
static uint8_t uiErrCase           = FALSE;
static uint8_t ucRcvdHeader        = 0xFFU;
static uint8_t b16BytesData        = FALSE;
static uint16_t uiNewLen;
static uint32_t ulNewOffset;
static uint16_t uiNewError;
static uint8_t uiNewCrc;
static uint8_t uiProVer;
static uint8_t bVerChecked = FALSE;

static fw_download_setting_t fwDownloadConfig;

// CMD5 Header to change bootloader baud rate
static uint8_t cmd5_hdrData[HDR_LEN] = {0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x2c, 0x00, 0x00, 0x00, 0x77, 0xdb, 0xfd, 0xe0};
// CMD7 Header to change timeout of bootloader
static uint8_t cmd7_hdrData[HDR_LEN] = {0x07, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x5b, 0x88, 0xf8, 0xba};
// Maximum Length that could be asked by the Helper = 2 bytes
static uint8_t ucByteBuffer[MAX_LENGTH];
static uint8_t ucString[STRING_SIZE];

static const uart_baudrate_clkDiv_map_t uartBaudRate2ClockDivMap[] = {
    {UART_BR_115200, 16U, 0x0075F6FDU},
    {UART_BR_3000000, 1U, 0x00C00000U},
};
/*==================== Function Prototypes ======================================*/
static UART_HANDLE_DEFINE(s_controllerHciUartHandle);

static void fw_upload_initCrc8(void);
static void fw_upload_genCrcTable(void);
static void fw_upload_IoFuncInit(void);
static void fw_upload_reInitUartInst(uint32_t baudRate, bool isUartOpen, bool enableFlowCtrl);
static void fw_upload_deInitUartInst(bool isUartOpen);
static void fw_upload_storeBytes(uint32_t ulVal, uint8_t uiSize, uint8_t *uiStored);
static void fw_upload_sendAck(uint8_t uiAck);
static void fw_upload_getHdrStartBytes(uint8_t *ucStr);
static void fw_upload_getLast5Bytes(uint8_t *buf);
static void fw_upload_v3SendLenBytes(const uint8_t *pFileBuffer, uint16_t uiLenToSend, uint32_t ulOffset);
static void fw_upload_printUartConfig(struct fwdnldintf *intf);
static bool fw_upload_initUartConfigData(changeBaudRateConfigData_t *uartConfigData, uint32_t secondaryBr);
static uint8_t fw_upload_checkReqCrc(uint8_t *uiStr, uint8_t uiReq);
static uint8_t fw_upload_lenValid(uint16_t *uiLenToSend, uint8_t *ucArray);
static uint8_t fw_upload_wait4HdrSig(uint32_t uiMs);
static uint8_t fw_upload_wait4Req(uint32_t iSecondBaudRate);
static uint16_t fw_upload_wait4Length(uint8_t flag);
static uint16_t fw_upload_getDataLen(uint8_t *buf);
static uint16_t fw_upload_sendBuffer(uint16_t uiLenToSend, uint8_t *ucBuf, uint8_t uiHighBaudrate);
static uint16_t fw_upload_v1SendLenBytes(const uint8_t *pFileBuffer, uint16_t uiLenToSend);
static uint32_t fw_upload_getCmd(uint8_t *buf);
static unsigned long fw_upload_updateCrc(unsigned long crc_accum, char *data_blk_ptr, int data_blk_size);
static unsigned char fw_upload_crc8(unsigned char *array, unsigned char len);
/* firmware-upload main api calls */
static fw_download_uart_status_e fw_upload_changeBaudrate(uint32_t primaryBr,
                                                          uint32_t secondaryBr,
                                                          uint8_t firstWaitHdrSig);
static fw_download_uart_status_e fw_upload_ChangeTimeout(void);
static fw_download_uart_status_e fw_upload_firmwareDownload(const unsigned char *wlan_fw_bin,
                                                            unsigned int wlan_fw_bin_len);
/* firmware-download function pointers */
static fwdnld_intf_ret_t uart_fw_download_prep(struct fwdnldintf *intf, void *param);
static fwdnld_intf_ret_t uart_fw_download(struct fwdnldintf *intf,
                                          const uint8_t *buff,
                                          uint32_t transfer_len,
                                          uint32_t *transferred_len);
static int fw_upload_uartWriteWrapper(const uint8_t *buff, uint32_t transfer_len);
static int fw_upload_uartReadWrapper(uint8_t *buff, uint32_t transfer_len);

#ifdef USE_LPUART_DRIVER
/* LP UART specific function pointers */
status_t (*fw_upload_ComReadChar)(LPUART_Type *base, uint8_t *data, size_t length);
status_t (*fw_upload_ComWriteChar)(LPUART_Type *base, const uint8_t *data, size_t length);
status_t (*fw_upload_init_uart)(LPUART_Type *base, const lpuart_config_t *config, uint32_t srcClock_Hz);
size_t (*fw_upload_GetBufferSize)(LPUART_Type *base, lpuart_handle_t *handle);
void (*fw_upload_CloseUart)(LPUART_Type *base);
#else
/* HAL UART specific function pointers */
hal_uart_status_t (*fw_upload_init_uart)(hal_uart_handle_t handle, const hal_uart_config_t *config);
hal_uart_status_t (*fw_upload_ComWriteChar)(hal_uart_handle_t handle, const uint8_t *data, size_t length);
hal_uart_status_t (*fw_upload_ComReadChar)(hal_uart_handle_t handle, uint8_t *data, size_t length);
hal_uart_status_t (*fw_upload_GetBufferSize)(hal_uart_handle_t handle, uint32_t *reCount);
hal_uart_status_t (*fw_upload_CloseUart)(hal_uart_handle_t handle);
#endif // USE_LPUART_DRIVER
INT32 (*fw_upload_GetTime)(void);
void (*fw_upload_DelayInMs)(uint32_t millisec);

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
    static fwdnld_intf_t uartIntf;
    static fw_download_setting_t fw_download_setting = {
        .uartConfig =
            {
#if defined(CPU_MIMXRT1176DVMAA_cm7) && defined(WIFI_BT_USE_M2_INTERFACE)
                LPUART2,                /*FW download UART port address*/
#else
                BOARD_BT_UART_BASEADDR, /*FW download UART port address*/
#endif /*defined(CPU_MIMXRT1176DVMAA_cm7) && defined(WIFI_BT_USE_M2_INTERFACE)*/
                0,                      /*BT UART clock frequency */
                115200U,                /*initial baud-rate for boot-loader*/
                3000000U,               /*secondary baud-rate for boot-loader*/
                0U,                     /*UART parity bits*/
#if defined(CPU_MIMXRT1176DVMAA_cm7) && defined(WIFI_BT_USE_M2_INTERFACE)
                2U,                     /*UART Instance*/
#else
                BOARD_BT_UART_INSTANCE, /*UART Instance*/
#endif /*defined(CPU_MIMXRT1176DVMAA_cm7) && defined(WIFI_BT_USE_M2_INTERFACE)*/
                1U,                     /*UART stop bits*/
                true,                   /*enableTx*/
                true,                   /*enableRx*/
                false,                  /*enableRTS*/
                false,                  /*enableCTS*/
                true                    /*isSecondaryBaudRate required?*/
            },
        false, /*wait4HdrSig*/
        false  /*isFwDownloadRetry*/
    };
#if defined(CPU_MIMXRT1176DVMAA_cm7) && defined(WIFI_BT_USE_M2_INTERFACE)
    fw_download_setting.uartConfig.uartClkFreq = CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart2);
#else
    fw_download_setting.uartConfig.uartClkFreq = BOARD_BT_UART_CLK_FREQ;
#endif /*defined(CPU_MIMXRT1176DVMAA_cm7) && defined(WIFI_BT_USE_M2_INTERFACE)*/
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
static void fw_upload_genCrcTable(void)
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
static void fw_upload_IoFuncInit(void)
{
    fw_upload_DelayInMs = OSA_TimeDelay;
    fw_upload_GetTime   = EM_get_time_ms;
#ifdef USE_LPUART_DRIVER
    fw_upload_ComReadChar   = LPUART_ReadBlocking;
    fw_upload_ComWriteChar  = LPUART_WriteBlocking;
    fw_upload_init_uart     = LPUART_Init;
    fw_upload_GetBufferSize = LPUART_TransferGetRxRingBufferLength;
    fw_upload_CloseUart     = LPUART_Deinit;
#else
    fw_upload_ComReadChar   = HAL_UartReceiveBlocking;
    fw_upload_ComWriteChar  = HAL_UartSendBlocking;
    fw_upload_init_uart     = HAL_UartInit;
    fw_upload_GetBufferSize = HAL_UartGetReceiveCount;
    fw_upload_CloseUart     = HAL_UartDeinit;
#endif // USE_LPUART_DRIVER
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
static void fw_upload_reInitUartInst(uint32_t baudRate, bool isUartOpen, bool enableFlowCtrl)
{
    PRINT("fw_upload_reInitUartInst(BR %d, isUARTOpen %d, FlowCtrl %d)\n", baudRate, isUartOpen, enableFlowCtrl);

    if (baudRate != 0)
    {
#ifdef USE_LPUART_DRIVER
        lpuart_config_t config;

        if (isUartOpen)
            fw_upload_CloseUart(fwDownloadConfig.uartConfig.lpUart);

        LPUART_GetDefaultConfig(&config);
        config.baudRate_Bps = baudRate;
        config.enableTx     = fwDownloadConfig.uartConfig.enableTx;
        config.enableRx     = fwDownloadConfig.uartConfig.enableRx;
        if (enableFlowCtrl)
        {
            config.enableRxRTS = true;
            config.enableTxCTS = true;
        }
        else
        {
            config.enableRxRTS = fwDownloadConfig.uartConfig.enableRTS;
            config.enableTxCTS = fwDownloadConfig.uartConfig.enableCTS;
        }

        fw_upload_init_uart(fwDownloadConfig.uartConfig.lpUart, &config, fwDownloadConfig.uartConfig.uartClkFreq);
#else
        /* Set the HCI-UART Configuration parameters */
        if (isUartOpen)
            fw_upload_CloseUart((hal_uart_handle_t)s_controllerHciUartHandle);

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
            config.enableRxRTS = true;
            config.enableTxCTS = true;
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
        fw_upload_init_uart((hal_uart_handle_t)s_controllerHciUartHandle, &config);
#endif // USE_LPUART_DRIVER
    }
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
static void fw_upload_deInitUartInst(bool isUartOpen)
{
    if (isUartOpen)
    {
#ifdef USE_LPUART_DRIVER
        fw_upload_CloseUart(fwDownloadConfig.uartConfig.lpUart);
#else
        fw_upload_CloseUart((hal_uart_handle_t)s_controllerHciUartHandle);
#endif
    }
}

static int fw_upload_uartWriteWrapper(const uint8_t *buff, uint32_t transfer_len)
{
    int error;
#ifdef USE_LPUART_DRIVER
    error = fw_upload_ComWriteChar(fwDownloadConfig.uartConfig.lpUart, buff, transfer_len);
#else
    error = fw_upload_ComWriteChar((hal_uart_handle_t)s_controllerHciUartHandle, buff, transfer_len);
#endif // USE_LPUART_DRIVER
    return error;
}

static int fw_upload_uartReadWrapper(uint8_t *buff, uint32_t transfer_len)
{
    int error;
#ifdef USE_LPUART_DRIVER
    error = fw_upload_ComReadChar(fwDownloadConfig.uartConfig.lpUart, buff, transfer_len);
#else
    error = fw_upload_ComReadChar((hal_uart_handle_t)s_controllerHciUartHandle, buff, transfer_len);
#endif // USE_LPUART_DRIVER

    return error;
}

static int fw_upload_GetBufferSizeWrapper(uint32_t *count)
{
    int error;
#ifdef USE_LPUART_DRIVER
    error = fw_upload_GetBufferSize(fwDownloadConfig.uartConfig.lpUart, count);
#else
    error = fw_upload_GetBufferSize((hal_uart_handle_t)s_controllerHciUartHandle, count);
#endif // USE_LPUART_DRIVER
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
static void fw_upload_initCrc8(void)
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
static unsigned char fw_upload_crc8(unsigned char *array, unsigned char len)
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
 *   TRUE:   0xa5 or 0xab is received.
 *   FALSE:  0xa5 or 0xab is not received.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
static uint8_t fw_upload_wait4HdrSig(uint32_t uiMs)
{
    uint8_t ucDone     = 0; // signature not Received Yet.
    uint64_t startTime = 0;
    uint64_t currTime  = 0;
    uint8_t bResult    = TRUE;
    ucRcvdHeader       = 0xFF;
    startTime          = fw_upload_GetTime();

    while (!ucDone)
    {
        fw_upload_uartReadWrapper(&ucRcvdHeader, 1);

        if ((ucRcvdHeader == V1_HEADER_DATA_REQ) || (ucRcvdHeader == V1_START_INDICATION) ||
            (ucRcvdHeader == V3_START_INDICATION) || (ucRcvdHeader == V3_HEADER_DATA_REQ))
        {
            ucDone = 1;
#ifdef DEBUG_PRINT
            PRINT("\nReceived 0x%x ", ucRcvdHeader);
#endif
            if (!bVerChecked)
            {
                if ((ucRcvdHeader == V1_HEADER_DATA_REQ) || (ucRcvdHeader == V1_START_INDICATION))
                {
                    uiProVer = (uint8_t)VER1;
                }
                else
                {
                    uiProVer = (uint8_t)VER3;
                }
                bVerChecked = TRUE;
            }
        }
        else
        {
            if (uiMs)
            {
                currTime = fw_upload_GetTime();
                if (currTime - startTime > uiMs)
                {
                    bResult = FALSE;
#ifdef DEBUG_PRINT
                    PRINT("\ndidnt Received 0x%x , currTime %d startTime %d", ucRcvdHeader, currTime, startTime);
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
static uint16_t fw_upload_wait4Length(uint8_t flag)
{
    // Length Variables
    uint16_t uiLen      = 0x0;
    uint16_t uiLenComp  = 0x0;
    // uint8_t  uiVersion;
    //  uiLen and uiLenComp are 1's complement of each other.
    //  In such cases, the XOR of uiLen and uiLenComp will be all 1's
    //  i.e 0xffff.
    uint16_t uiXorOfLen = 0xFFFF;
    uint8_t ack;
    // do {
    // fw_upload_DelayInMs(1U);
    // fw_upload_GetBufferSizeWrapper(&read_count);
    //} while (read_count < 4);
    // Read the Lengths.
    fw_upload_uartReadWrapper((uint8_t *)&uiLen, 2);
    fw_upload_uartReadWrapper((uint8_t *)&uiLenComp, 2);

    // Check if the length is valid.
    if ((uiLen ^ uiLenComp) == uiXorOfLen) // All 1's
    {
#ifdef DEBUG_PRINT
        PRINT("\n       bootloader asks for %d bytes \n ", uiLen);
#endif
        // Successful. Send back the ack.
        if ((ucRcvdHeader == V1_HEADER_DATA_REQ) || (ucRcvdHeader == V1_START_INDICATION))
        {
            ack = V1_REQUEST_ACK;
            fw_upload_uartWriteWrapper(&ack, 1);
            if (ucRcvdHeader == V1_START_INDICATION)
            {
                /* Eliminated longjmp(resync, 1); returning restart status */
                return (uint16_t)V1_START_INDICATION;
            }
        }
    }
    else
    {
#ifdef DEBUG_PRINT
        PRINT("\n    NAK case: bootloader LEN = %x bytes \n ", uiLen);
        PRINT("\n    NAK case: bootloader LENComp = %x bytes \n ", uiLenComp);
#endif
        // Failure due to mismatch.
        ack = 0xbf;
        fw_upload_uartWriteWrapper(&ack, 1);
        // Start all over again.
        if (flag)
        {
            /* Eliminated longjmp(resync, 1); returning restart status */
            return (uint16_t)V1_START_INDICATION;
        }
        else
        {
            uiLen = 0;
        }
    }
    return uiLen;
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
static void fw_upload_storeBytes(uint32_t ulVal, uint8_t uiSize, uint8_t *uiStored)
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
static void fw_upload_sendAck(uint8_t uiAck)
{
    uint8_t uiAckCrc;
    if ((uiAck == V3_REQUEST_ACK) || (uiAck == V3_CRC_ERROR))
    {
#ifdef TEST_CODE
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
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 303 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte",
                      ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 304 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, send CRC byte",
                      ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 305 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 306 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, NOT send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 307 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 308 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 309 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte, sleep %dms",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 310 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else
            {
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
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
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 313 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte",
                      ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 314 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, send CRC byte",
                      ucTestCase, ucSleepTimeMs, ucRcvdHeader);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 315 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
                ucTestDone = 1;
            }
            else if (ucTestCase == 316 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, NOT send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 317 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
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
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 320 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms",
                      ucTestCase, ucRcvdHeader, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else
            {
                fw_upload_uartWriteWrapper(&uiAck, 1);
                fw_upload_uartWriteWrapper(&uiAckCrc, 1);
            }
        }
#else

        fw_upload_uartWriteWrapper(&uiAck, 1);

        // prepare crc for 0x7A or 0x7C
        ucCalCrc[0] = uiAck;
        uiAckCrc    = fw_upload_crc8(ucCalCrc, 1);
        fw_upload_uartWriteWrapper(&uiAckCrc, 1);

#endif
    }
    else if (uiAck == V3_TIMEOUT_ACK)
    {
        fw_upload_uartWriteWrapper(&uiAck, 1);

        // prepare crc for 0x7B
        ucCalCrc[0] = uiAck;
        fw_upload_storeBytes(ulNewOffset, sizeof(ulNewOffset), &ucCalCrc[1]);
        fw_upload_uartWriteWrapper((uint8_t *)&ulNewOffset, 4);
        uiAckCrc = fw_upload_crc8(ucCalCrc, 5);
        fw_upload_uartWriteWrapper(&uiAckCrc, 1);
    }
    else
    {
        PRINT("\nNon-empty else statement\n");
    }

#ifdef DEBUG_PRINT
    PRINT("\n ===> ACK = %x, CRC = %x \n", uiAck, uiAckCrc);
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
static uint8_t fw_upload_checkReqCrc(uint8_t *uiStr, uint8_t uiReq)
{
    uint8_t uiCalCrc;

    if (uiReq == V3_HEADER_DATA_REQ)
    {
        uiCalCrc = fw_upload_crc8(uiStr, A6REQ_PAYLOAD_LEN + REQ_HEADER_LEN);
        if (uiCalCrc != uiStr[A6REQ_PAYLOAD_LEN + REQ_HEADER_LEN])
        {
            return FALSE;
        }
    }
    else if (uiReq == V3_START_INDICATION)
    {
        uiCalCrc = fw_upload_crc8(uiStr, AbREQ_PAYLOAD_LEN + REQ_HEADER_LEN);
        if (uiCalCrc != uiStr[AbREQ_PAYLOAD_LEN + REQ_HEADER_LEN])
        {
            return FALSE;
        }
    }
    else
    {
        PRINT("\nNon-empty else statement\n");
    }

    return TRUE;
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
static uint8_t fw_upload_wait4Req(uint32_t iSecondBaudRate)
{
    uint16_t uiChipId = 0;
    uint8_t uiVersion = 0, uiReqCrc = 0;
    uint8_t uiTmp[20] = {0};
    uint8_t bCrcMatch = FALSE;
    uint8_t status    = TRUE;

    if (ucRcvdHeader == V3_HEADER_DATA_REQ)
    {
        // 0xA7 <LEN><Offset><ERR><CRC8>
        fw_upload_uartReadWrapper((uint8_t *)&uiNewLen, 2);
        fw_upload_uartReadWrapper((uint8_t *)&ulNewOffset, 4);
        fw_upload_uartReadWrapper((uint8_t *)&uiNewError, 2);
        fw_upload_uartReadWrapper((uint8_t *)&uiNewCrc, 1);
#ifdef DEBUG_PRINT
        PRINT("\n <=== REQ = 0xA7, Len = %x,Off = %x,Err = %x,CRC = %x\n ", uiNewLen, ulNewOffset, uiNewError,
              uiNewCrc);
#endif
        // check crc
        uiTmp[0] = V3_HEADER_DATA_REQ;
        fw_upload_storeBytes((uint32_t)uiNewLen, sizeof(uiNewLen), &uiTmp[1]);
        fw_upload_storeBytes(ulNewOffset, sizeof(ulNewOffset), &uiTmp[3]);
        fw_upload_storeBytes(uiNewError, sizeof(uiNewError), &uiTmp[7]);
        uiTmp[9]  = uiNewCrc;
        bCrcMatch = fw_upload_checkReqCrc(uiTmp, V3_HEADER_DATA_REQ);

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
            PRINT("\n === REQ = 0xA7, CRC Mismatched === ");
#endif
            fw_upload_sendAck(V3_CRC_ERROR);
            status = FALSE;
        }
    }
    else if (ucRcvdHeader == V3_START_INDICATION)
    {
        // 0xAB <CHIP ID> <SW loader REV 1 byte> <CRC8>
        fw_upload_uartReadWrapper((uint8_t *)&uiChipId, 2);
        fw_upload_uartReadWrapper(&uiVersion, 1);
        fw_upload_uartReadWrapper(&uiReqCrc, 1);
        PRINT("\nChipID is : %x, Version is : %x\n", uiChipId, uiVersion);

        // check crc
        uiTmp[0] = V3_START_INDICATION;
        fw_upload_storeBytes((uint32_t)uiChipId, sizeof(uiChipId), &uiTmp[1]);
        uiTmp[3]  = uiVersion;
        uiTmp[4]  = uiReqCrc;
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
                /* longjmp(resync, 1); eliminated and added return false to start again */
                return FALSE;
            }
        }
        else
        {
#ifdef DEBUG_PRINT
            PRINT("\n === REQ = 0xAB, CRC Mismatched === ");
#endif
            fw_upload_sendAck(V3_CRC_ERROR);
            status = FALSE;
            if (iSecondBaudRate == 0)
            {
                /* longjmp(resync, 1); eliminated and added return false to start again */
                return FALSE;
            }
        }
    }
    else
    {
        PRINT("\nNon-empty else statement\n");
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
static uint32_t fw_upload_getCmd(uint8_t *buf)
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
static uint16_t fw_upload_getDataLen(uint8_t *buf)
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
static uint8_t fw_upload_lenValid(uint16_t *uiLenToSend, uint8_t *ucArray)
{
    uint16_t uiLen, uiLenComp;
    uint16_t uiXorOfLen = 0xFFFF;
    uiLen               = (uint16_t)((ucArray[1] & 0xFF) | ((ucArray[2] << 8) & 0xFF00));
    uiLenComp           = (uint16_t)((ucArray[3] & 0xFF) | ((ucArray[4] << 8) & 0xFF00));
    // LEN valid if len & complement match
    if ((uiLen ^ uiLenComp) == uiXorOfLen) // All 1's
    {
        *uiLenToSend = uiLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************
 *
 * Name: fw_upload_getHdrStartBytes
 *
 * Description:
 *   This function gets 0xa5 and it's following 4 bytes length.
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
static void fw_upload_getHdrStartBytes(uint8_t *ucStr)
{
    uint8_t ucDone = FALSE, ucStringCnt = 0, i;
    while (!ucDone)
    {
        fw_upload_uartReadWrapper(&ucRcvdHeader, 1);

        if (ucRcvdHeader == V1_HEADER_DATA_REQ)
        {
            ucStr[ucStringCnt++] = ucRcvdHeader;
            ucDone               = TRUE;
#ifdef DEBUG_PRINT
            PRINT("\nReceived 0x%x\n ", ucRcvdHeader);
#endif
        }
        else
        {
            fw_upload_DelayInMs(1);
        }
    }
    // do{
    fw_upload_DelayInMs(1);
    // fw_upload_GetBufferSizeWrapper(&RcvCount);
    //}while(RcvCount < 4);
    for (i = 0; i < 4; i++)
    {
        fw_upload_uartReadWrapper(&ucRcvdHeader, 1);
        ucStr[ucStringCnt++] = ucRcvdHeader;
    }
}
/******************************************************************************
 *
 * Name: fw_upload_getLast5Bytes
 *
 * Description:
 *   This function gets last valid request.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   *buf: buffer that stores header and following data.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *

 *****************************************************************************/
static void fw_upload_getLast5Bytes(uint8_t *buf)
{
    uint8_t a5cnt, i;
    uint8_t ucTemp[STRING_SIZE];
    uint16_t uiTempLen = 0;
    uint16_t len       = 0;
    uint32_t fifosize  = 0;
    uint8_t alla5times = FALSE;

    // initialise
    memset(ucString, 0x00, STRING_SIZE);

    // fw_upload_GetBufferSizeWrapper(&fifosize);

    fw_upload_getHdrStartBytes(ucString);

    if (fw_upload_lenValid(&uiTempLen, ucString) == TRUE)
    {
        // Valid length recieved
#ifdef DEBUG_PRINT
        PRINT("\n Valid length = %d \n", uiTempLen);
#endif
    }

    len = fw_upload_getDataLen(buf);
    if ((fifosize < 6) && ((uiTempLen == HDR_LEN) || (uiTempLen == len)))
    {
#ifdef DEBUG_PRINT
        PRINT("=========>success case\n");
#endif
        uiErrCase = FALSE;
    }
    else // start to get last valid 5 bytes
    {
#ifdef DEBUG_PRINT
        PRINT("=========>fail case\n");
#endif
        while (fw_upload_lenValid(&uiTempLen, ucString) == FALSE)
        {
            fw_upload_getHdrStartBytes(ucString);
            fifosize -= 5;
        }
#ifdef DEBUG_PRINT
        PRINT("Error cases 1, 2, 3, 4, 5...\n");
#endif
        if (fifosize > 5)
        {
            fifosize -= 5;
            do
            {
                do
                {
                    a5cnt = 0;
                    do
                    {
                        fw_upload_getHdrStartBytes(ucTemp);
                        fifosize -= 5;
                    } while ((fw_upload_lenValid(&uiTempLen, ucTemp) == TRUE) && (!alla5times) && (fifosize > 5));
                    // if 5bytes are all 0xa5, continue to clear 0xa5
                    for (i = 0; i < 5; i++)
                    {
                        if (ucTemp[i] == V1_HEADER_DATA_REQ)
                        {
                            a5cnt++;
                        }
                    }
                    alla5times = TRUE;
                } while (a5cnt == 5);
#ifdef DEBUG_PRINT
                PRINT("a5 count in last 5 bytes: %d\n", a5cnt);
#endif
                if (fw_upload_lenValid(&uiTempLen, ucTemp) == FALSE)
                {
                    for (i = 0; i < (5 - a5cnt); i++)
                    {
                        fw_upload_uartReadWrapper(&ucTemp[i + a5cnt], 1);
                    }
                    if (a5cnt > 0)
                    {
                        memcpy(ucString, &ucTemp[a5cnt - 1], (5 - a5cnt) * sizeof(uint8_t));
                    }
                }
                else
                {
                    memcpy(ucString, ucTemp, 5 * sizeof(uint8_t));
                }
            } while (fw_upload_lenValid(&uiTempLen, ucTemp) == FALSE);
        }
        uiErrCase = TRUE;
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
static uint16_t fw_upload_sendBuffer(uint16_t uiLenToSend, uint8_t *ucBuf, uint8_t uiHighBaudrate)
{
    uint16_t uiBytesToSend = HDR_LEN, uiFirstChunkSent = 0;
    uint16_t uiDataLen = 0;
    uint8_t ucSentDone = 0;
    uint8_t uiValidLen = FALSE;

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
                if ((uiFirstChunkSent == 0) || ((uiFirstChunkSent == 1) && (uiErrCase == TRUE)))
                {
                    // Write first 16 bytes of buffer
#ifdef DEBUG_PRINT
                    PRINT("\n====>  Sending first chunk...\n");
                    PRINT("\n====>  Sending %d bytes...\n", uiBytesToSend);
#endif
                    fw_upload_uartWriteWrapper(ucBuf, uiBytesToSend);
                    if (cmd7_Req == TRUE || EntryPoint_Req == TRUE)
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
                            b16BytesData = TRUE;
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
                    fw_upload_uartWriteWrapper(&ucBuf[HDR_LEN], uiBytesToSend);
                    uiFirstChunkSent = 1;
                    // We should expect 16, then next block will start
                    uiBytesToSend = HDR_LEN;
                    b16BytesData  = FALSE;
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
                    fw_upload_uartWriteWrapper(ucBuf, (uiLenToSend - 1));
                    uiBytesToSend    = uiDataLen;
                    uiFirstChunkSent = 0;
                }
                else if (uiLenToSend == (uiDataLen + 1))
                {
                    // Send second chunk again
#ifdef DEBUG_PRINT
                    PRINT("\n2. Resending second chunk...\n");
#endif
                    fw_upload_uartWriteWrapper(&ucBuf[HDR_LEN], (uiLenToSend - 1));
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
                fw_upload_uartWriteWrapper(ucBuf, uiLenToSend);
                uiBytesToSend    = uiDataLen;
                uiFirstChunkSent = 0;
            }
            else if (uiLenToSend == uiDataLen)
            {
                PRINT("Restart sending 2nd chunk...");
                fw_upload_uartWriteWrapper(&ucBuf[HDR_LEN], uiLenToSend);
                uiBytesToSend    = HDR_LEN;
                uiFirstChunkSent = 1;
            }
            else
            {
                PRINT("\nNon-empty else statement\n");
            }
        }
        // Get last 5 bytes now
        fw_upload_getLast5Bytes(ucBuf);
        // Get next length
        uiValidLen = FALSE;
        do
        {
            if (fw_upload_lenValid(&uiLenToSend, ucString) == TRUE)
            {
                // Valid length received
                uiValidLen = TRUE;
#ifdef DEBUG_PRINT
                PRINT("\n Valid length = %d \n", uiLenToSend);
#endif
                // ACK the bootloader
                tmp = V1_REQUEST_ACK;
                fw_upload_uartWriteWrapper(&tmp, 1);
#ifdef DEBUG_PRINT
                PRINT("\n  BOOT_HEADER_ACK 0x5a sent \n");
#endif
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
static uint16_t fw_upload_v1SendLenBytes(const uint8_t *pFileBuffer, uint16_t uiLenToSend)
{
    uint16_t ucDataLen, uiLen;
    uint32_t ulCmd;
#ifdef DEBUG_PRINT
    uint16_t i;
#endif
    memset(ucByteBuffer, 0, sizeof(ucByteBuffer));

    cmd7_Req       = FALSE;
    EntryPoint_Req = FALSE;

    if (ulCurrFileSize + uiLenToSend > ulTotalFileSize)
        uiLenToSend = (uint16_t)(ulTotalFileSize - ulCurrFileSize);

    memcpy(&ucByteBuffer[uiLenToSend] - uiLenToSend, pFileBuffer + ulCurrFileSize, uiLenToSend);
    ulCurrFileSize += uiLenToSend;
    ulCmd = fw_upload_getCmd(ucByteBuffer);
    if (ulCmd == CMD7)
    {
        cmd7_Req  = TRUE;
        ucDataLen = 0;
    }
    else
    {
        ucDataLen = fw_upload_getDataLen(ucByteBuffer);
        memcpy(&ucByteBuffer[uiLenToSend], pFileBuffer + ulCurrFileSize, ucDataLen);
        ulCurrFileSize += ucDataLen;
        if ((ulCurrFileSize < ulTotalFileSize) && (ulCmd == CMD6 || ulCmd == CMD4))
        {
            EntryPoint_Req = TRUE;
        }
    }

#ifdef DEBUG_PRINT
    PRINT("The buffer is to be sent: %d", uiLenToSend + ucDataLen);
    for (i = 0; i < (uiLenToSend + ucDataLen); i++)
    {
        if (i % 16 == 0)
        {
            PRINT("\n");
        }
        PRINT(" %02x ", ucByteBuffer[i]);
    }
#endif
    // start to send Temp buffer
    uiLen = fw_upload_sendBuffer(uiLenToSend, ucByteBuffer, FALSE);
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
static void fw_upload_v3SendLenBytes(const uint8_t *pFileBuffer, uint16_t uiLenToSend, uint32_t ulOffset)
{
    // Retransmittion of previous block
    if (ulOffset == ulLastOffsetToSend)
    {
#ifdef DEBUG_PRINT
        PRINT("\nResend offset %d...\n", ulOffset);
#endif
        fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
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
                fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
                ucTestDone = 1;
            }
            else if (ucTestCase == 322 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send only 8 bytes of 16-byte HEADER, then sleep for %dms", ucTestCase,
                      ucSleepTimeMs);
                fw_upload_uartWriteWrapper(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 323 && !ucTestDone)
            {
                PRINT(
                    "\n         TC-%d:  Send 8 bytes of 16-byte HEADER, sleep for %dms, then send remaining 8 bytes "
                    "HEADER",
                    ucTestCase, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&ucByteBuffer[8], 8);
                ucTestDone = 1;
            }
            else if (ucTestCase == 324 && !ucTestDone)
            {
                PRINT(
                    "\n         TC-%d:  Send 8 bytes of 16-byte HEADER, sleep for %dms, then send full 16 bytes HEADER",
                    ucTestCase, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
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
                fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
                ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
                ucTestDone                    = 1;
            }
            else if (ucTestCase == 327 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send 16-byte HEADER with last byte changed to 7C, then sleep for %dms",
                      ucTestCase, ucSleepTimeMs);
                myCrcCorrByte                 = ucByteBuffer[uiLenToSend - 1];
                ucByteBuffer[uiLenToSend - 1] = 0x7c;
                fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
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
                fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
            }
        }
        else
        {
            if (ucTestCase == 301 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Sleeping for %dms before sending %d bytes DATA", ucTestCase, ucSleepTimeMs,
                      uiLenToSend);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
                ucTestDone = 1;
            }
            else if (ucTestCase == 302 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send only first 8 bytes of %d bytes of DATA, then sleep for %dms", ucTestCase,
                      uiLenToSend, ucSleepTimeMs);
                fw_upload_uartWriteWrapper(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else if (ucTestCase == 303 && !ucTestDone)
            {
                PRINT(
                    "\n         TC-%d:  Send first 8 bytes of %d bytes DATA, sleep for %dms, then send remaining %d "
                    "DATA",
                    ucTestCase, uiLenToSend, ucSleepTimeMs, uiLenToSend - 8);
                fw_upload_uartWriteWrapper(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(&ucByteBuffer[8], uiLenToSend - 8);
                ucTestDone = 1;
            }
            else if (ucTestCase == 304 && !ucTestDone)
            {
                PRINT(
                    "\n         TC-%d:  Send first 8 bytes of %d bytes DATA, sleep for %dms, then send full %d bytes "
                    "DATA",
                    ucTestCase, uiLenToSend, ucSleepTimeMs, uiLenToSend);
                fw_upload_uartWriteWrapper(ucByteBuffer, 8);
                fw_upload_DelayInMs(ucSleepTimeMs);
                fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
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
                fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
                ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
                ucTestDone                    = 1;
            }
            else if (ucTestCase == 307 && !ucTestDone)
            {
                PRINT("\n         TC-%d:  Send %d bytes DATA with last byte changed to 7C, then sleep for %dms",
                      ucTestCase, uiLenToSend, ucSleepTimeMs);
                myCrcCorrByte                 = ucByteBuffer[uiLenToSend - 1];
                ucByteBuffer[uiLenToSend - 1] = 0x7c;
                fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
                ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
                fw_upload_DelayInMs(ucSleepTimeMs);
                ucTestDone = 1;
            }
            else
            {
                fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);
            }
        }

#else

        fw_upload_uartWriteWrapper(ucByteBuffer, uiLenToSend);

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
static bool fw_upload_initUartConfigData(changeBaudRateConfigData_t *uartConfigData, uint32_t secondaryBr)
{
    bool isSuccess = false;

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
                isSuccess                     = true;
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
 *   TRUE:            Change baud rate successfully
 *   FALSE:           Change baud rate unsuccessfully
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
static fw_download_uart_status_e fw_upload_changeBaudrate(uint32_t primaryBr,
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
    uint8_t flag                 = FALSE;
    uint8_t bRetVal              = FALSE;
    uint8_t ucLoadPayload        = 0;
    uint8_t uiReUsedInitBaudrate = FALSE;

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
        PRINT("uiProVer:%d\n", uiProVer);

        // Wait to Receive 0xa5, 0xaa, 0xab, 0xa7
        // If the second baudrate is used, wait for 2s to check 0xa5
        flag = fw_upload_wait4HdrSig(TIMEOUT_VAL_MILLISEC);

        PRINT("\nflag %d firstWaitHdrSig %d\n", flag, firstWaitHdrSig);

        if (firstWaitHdrSig && flag)
        {
            if (ucLoadPayload)
            {
                if (uiProVer == VER3)
                {
                    change_baudrata_buffer_len = (uint32_t)HDR_LEN + (uint32_t)uiNewLen;
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
                PRINT(
                    "\n 0xa5 or 0xa7 is not received by using the second baudrate,change baud rate to the first "
                    "baudrate\n");
                fw_upload_reInitUartInst(primaryBr, true, false);
                ucLoadPayload        = 0;
                uiReUsedInitBaudrate = TRUE;
                continue;
            }
        }

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
                fw_upload_sendBuffer(uiLenToSend, ucBuffer, TRUE);
                fw_upload_reInitUartInst(secondaryBr, true, true);
                ucLoadPayload = 1;
            }
            else
            {
                // Download CMD5 header and Payload packet
                fw_upload_uartWriteWrapper(uartConfig, configDataLen);
                fw_upload_reInitUartInst(secondaryBr, true, true);
                ucLoadPayload = 1;
            }
        }
        else if (uiProVer == VER3)
        {
            flag = fw_upload_wait4Req(secondaryBr);
            if (!firstWaitHdrSig || flag)
            {
                if (uiNewLen != 0 && ucRcvdHeader == V3_HEADER_DATA_REQ)
                {
                    if (uiNewError == 0)
                    {
                        fw_upload_sendAck(V3_REQUEST_ACK);
                        firstWaitHdrSig = TRUE;

                        if (uiNewLen == HDR_LEN)
                        {
                            PRINT("sending command5...\n");
                            PRINT("command5, len %d, data: ", uiNewLen);
                            for (int i = 0; i < uiNewLen; i++)
                            {
                                PRINT("%02x ", cmd5_hdrData[i]);
                            }
                            PRINT("\n");
                            fw_upload_uartWriteWrapper(cmd5_hdrData, uiNewLen);
                            ulLastOffsetToSend = ulNewOffset;
                        }
                        else
                        {
                            PRINT("uartconfig, len %d, data: ", uiNewLen);
                            for (int i = 0; i < uiNewLen; i++)
                            {
                                PRINT("%02x ", uartConfig[i]);
                            }
                            PRINT("\n");

                            fw_upload_uartWriteWrapper(uartConfig, uiNewLen);
                            // fw_upload_DelayInMs (500U);
                            // Reopen Uart by using the second baudrate after downloading the payload.
                            // config.enableRxRTS =  fwDownloadConfig.uartConfig.enableRTS;
                            // config.enableTxCTS =  fwDownloadConfig.uartConfig.enableCTS;
                            fw_upload_reInitUartInst(secondaryBr, true, true);
                            ucLoadPayload = 1;
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
            PRINT("\nNon-empty terminating else statement\n");
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
static fw_download_uart_status_e fw_upload_ChangeTimeout(void)
{
    fw_download_uart_status_e status = FW_DOWNLOAD_UART_CHANGE_TIMEOUT_FAIL;
    uint8_t bFirst                   = TRUE;
    uint8_t bRetVal                  = FALSE;
    uint8_t reTryNumber              = 0;
    fw_upload_genCrcTable();

    while (!bRetVal)
    {
        PRINT("fw_upload_ChangeTimeout..\n");
        if (fw_upload_wait4HdrSig(TIMEOUT_VAL_MILLISEC))
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
                                PRINT("timeOutCmd, len %d, data: ", uiNewLen);
                                for (int i = 0; i < uiNewLen; i++)
                                {
                                    PRINT("%02x ", cmd7_hdrData[i]);
                                }
                                PRINT("\n");
                                fw_upload_uartWriteWrapper(cmd7_hdrData, uiNewLen);
                                ulLastOffsetToSend = ulNewOffset;
                                bFirst             = FALSE;
                            }
                            else
                            {
                                bRetVal = TRUE;
                                cmd7_change_timeout_len      = HDR_LEN;
                                fwDownloadConfig.wait4HdrSig = false;
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
                                bRetVal = TRUE;
                            }
                        }
                    }
                }
            }
            if (uiProVer == VER1)
            {
                status = FW_DOWNLOAD_UART_SUCCESS;
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
static fw_download_uart_status_e fw_upload_firmwareDownload(const unsigned char *wlan_fw_bin,
                                                            unsigned int wlan_fw_bin_len)
{
    fw_download_uart_status_e status = FW_DOWNLOAD_UART_FAILURE;
    uint16_t uiLenToSend             = 0;
    uint8_t flag                     = FALSE;

    if (wlan_fw_bin != NULL)
    {
        fwDownloadConfig.wait4HdrSig = true;
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
            // jme check the flag first
            flag = fw_upload_wait4HdrSig(TIMEOUT_VAL_MILLISEC);

            if (!fwDownloadConfig.uartConfig.isSecondaryBaudRateReq && !flag)
            {
                PRINT("\n0xa5,0xaa,0xab or 0xa7 is not received in %d ms\n", TIMEOUT_VAL_MILLISEC);
                return FW_DOWNLOAD_UART_HDR_SIG_TIMEOUT;
            }

            fwDownloadConfig.uartConfig.isSecondaryBaudRateReq = false;

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
                if (ucRcvdHeader == V3_START_INDICATION)
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
                            PRINT("\n === Succ: REQ = 0xA7, Errcode = 0 ");
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
static fwdnld_intf_ret_t uart_fw_download_prep(struct fwdnldintf *intf, void *param)
{
    fwdnld_intf_ret_t ulResult = FWDNLD_INTF_EPARAM;
    (void)param;

    if ((intf != NULL) && (intf->intf_type == FWDNLD_INTF_UART))
    {
        fw_download_setting_t *fwDownloadSettingObj = intf->intf_s.intf_specific;

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
            fw_upload_reInitUartInst(fwDownloadConfig.uartConfig.primaryBaudRate, false, false);
            ulResult = FWDNLD_INTF_SUCCESS;
        }
        else
        {
            PRINT("invalid object!\n");
            fw_upload_printUartConfig(intf);
        }
    }

    return ulResult;
}

void wakeUpControllerFromBootSleep(void)
{
#if (defined(CPU_MIMXRT1062DVMAA_cm7) || defined(CPU_MIMXRT1062DVL6A_cm7) ||  defined(CPU_MIMXRT1062DVL6B_cm7) || defined(CPU_MIMXRT1176DVMAA_cm7))

      gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 1U, kGPIO_NoIntmode};

#if (defined(CPU_MIMXRT1062DVMAA_cm7) || defined(CPU_MIMXRT1062DVL6A_cm7) ||  defined(CPU_MIMXRT1062DVL6B_cm7)) // For RT1060
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO          GPIO1
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IOMUX    IOMUXC_GPIO_AD_B1_05_GPIO1_IO21
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IO       21
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_REVERT_IOMUX  IOMUXC_GPIO_AD_B1_05_LPUART3_RTS_B
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_IOMUX_CONFIG  0x10B0U
#elif defined(CPU_MIMXRT1176DVMAA_cm7) // For RT1170
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO          GPIO11
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IOMUX    IOMUXC_GPIO_DISP_B2_13_GPIO11_IO14
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_GPIO_IO       14
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_REVERT_IOMUX  IOMUXC_GPIO_DISP_B2_13_LPUART2_RTS_B
    #define WAKEUP_FROM_BOOTSLEEP_TRIGGER_IOMUX_CONFIG  0x02U
#endif /* (defined(CPU_MIMXRT1062DVMAA_cm7) || defined(CPU_MIMXRT1062DVL6A_cm7) ||  defined(CPU_MIMXRT1062DVL6B_cm7)) */

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

static fwdnld_intf_ret_t uart_fw_download(struct fwdnldintf *intf,
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
        PRINT("fw-download %s(ret:%d)!\n", (status != FW_DOWNLOAD_UART_SUCCESS) ? "failed" : "success", status);
        fw_upload_DelayInMs(WAIT_AFTER_FW_DOWNLOAD);
        fw_upload_deInitUartInst(true);
    }

    return status;
}

static void fw_upload_printUartConfig(struct fwdnldintf *intf)
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
