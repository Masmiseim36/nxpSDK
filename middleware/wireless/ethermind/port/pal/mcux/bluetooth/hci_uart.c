
/**
 *  \file hci_uart.c
 *
 *  This File contains the routines to Transmit and Receive Data from
 *  the UART Transport Layer (Windows Version).
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "hci_transport.h"

/* IMX RT1060 SDK Includes */
#include "fsl_common.h"
#include "fsl_debug_console.h"

/* #include "fsl_lpuart.h" */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "fsl_adapter_uart.h"

#include "controller_hci_uart.h"

#ifdef BT_UART

#define HCI_UART_TRC(...)
#define HCI_UART_INF(...)
#define HCI_UART_ERR(...) PRINTF(__VA_ARGS__)

/* #define HAVE_HCI_TX_RX_BYTE_COUNT */

/* ----------------------------------------- Function Declarations */
/* This call should be made before the Stack in Initialized */
void hci_uart_set_serial_settings (CHAR *device, UINT32 baud);

DECL_STATIC BT_THREAD_RETURN_TYPE hci_uart_read_task (BT_THREAD_ARGS args);

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */
typedef struct _hci_uart_meta_data_
{
    uint8_t * data;     /* The buffer of data to be Txed/Rxed */
    size_t    dataSize; /* The byte count to be Txed/Rxed. */
} hci_uart_meta_data;

/* ----------------------------------------- Static Global Variables */

/* UART Read Task State */
DECL_STATIC UCHAR hci_uart_state;

#ifdef HCI_UART_COLLECT_AND_WR_COMPLETE_PKT
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
static uint8_t odd;
/* Buffer for writing HCI Commands & ACL Data packets, make sure HCI_UART_WR_BUF_SIZE is multiple of 4 */
AT_NONCACHEABLE_SECTION_ALIGN(DECL_STATIC UCHAR hci_uart_wr_buf [2][HCI_UART_WR_BUF_SIZE], 4);
#else
/* Buffer for writing HCI Commands & ACL Data packets */
AT_NONCACHEABLE_SECTION_ALIGN(DECL_STATIC UCHAR hci_uart_wr_buf [HCI_UART_WR_BUF_SIZE], 4);
#endif
#endif /* HCI_UART_COLLECT_AND_WR_COMPLETE_PKT */

/* UART Read Task Synchronization */
BT_DEFINE_MUTEX_TYPE(DECL_STATIC, hci_uart_mutex)
BT_DEFINE_COND_TYPE(DECL_STATIC, hci_uart_cond)

#if HCI_UART_TX_NONBLOCKING || ((defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U)))
BT_DEFINE_MUTEX_TYPE(DECL_STATIC, hci_uart_tx_mutex)
BT_DEFINE_COND_TYPE(DECL_STATIC, hci_uart_tx_cond)
#endif

#ifdef HAVE_HCI_TX_RX_BYTE_COUNT
/* Number of bytes Transmitted */
DECL_STATIC UINT32 hci_uart_tx_byte_count;

/* Number of bytes Received */
DECL_STATIC UINT32 hci_uart_rx_byte_count;
#endif /* HAVE_HCI_TX_RX_BYTE_COUNT */

#ifdef HAVE_HCI_TX_RX_BYTE_COUNT
/* Macro to increament Tx and Rx byte count */
#define HCI_UART_INC_TX_BYTES(nbytes) hci_uart_tx_byte_count += (nbytes)
#define HCI_UART_INC_RX_BYTES(nbytes) hci_uart_rx_byte_count += (nbytes)
#else
#define HCI_UART_INC_TX_BYTES(nbytes)
#define HCI_UART_INC_RX_BYTES(nbytes)
#endif /* HAVE_HCI_TX_RX_BYTE_COUNT */

/** HCI Transport DS to identify the next length of UART data expected */
DECL_STATIC HT_PARSE ht;

/* TODO: Check if we need to use any other common define */
#define HCI_RX_QUEUE_SIZE (1024U)

#ifdef BT_SNOOP_WRITE_TRUNCATE
#define BT_SNOOP_PKTLEN_LIMIT 256U
#endif /* BT_SNOOP_WRITE_TRUNCATE */

DECL_STATIC UART_HANDLE_DEFINE(hci_uart_handle);
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
UART_DMA_HANDLE_DEFINE(hci_uart_DmaHandle);
#endif
/* HCI UART RX Meta Data */
hci_uart_meta_data hci_uart_rx;

AT_NONCACHEABLE_SECTION_ALIGN(static UCHAR  hci_uart_rx_data_buff[HCI_RX_QUEUE_SIZE], 4);
UCHAR  hci_uart_rx_state;
UINT16 hci_uart_rx_bytes;
UCHAR  hci_uart_instance;

static UCHAR assert;

/* ----------------------------------------- Functions */
void hci_uart_set_serial_settings(CHAR *device, UINT32 baud)
{
}

/** HCI-UART Initialization */
void hci_uart_init (void)
{
    BT_thread_type tid;
    BT_thread_attr_type hci_uart_task_attr;

    BT_MUTEX_INIT_VOID (hci_uart_mutex, TRANSPORT);
    BT_COND_INIT_VOID(hci_uart_cond, TRANSPORT);
#if HCI_UART_TX_NONBLOCKING || ((defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U)))
    BT_MUTEX_INIT_VOID (hci_uart_tx_mutex, TRANSPORT);
    BT_COND_INIT_VOID(hci_uart_tx_cond, TRANSPORT);
#endif

    /* Initialize the Write Task Attributes */
    hci_uart_task_attr.thread_name       = (DECL_CONST CHAR  *)"EtherMind UART Task";
    hci_uart_task_attr.thread_stack_size = BT_TASK_STACK_DEPTH;
    /* Setting the Priority 1 Higher than the Default EtherMind Tasks */
    hci_uart_task_attr.thread_priority   = (BT_TASK_PRIORITY);

    /* Create a thread to receive data From Serial PORT and BUFFER it */
    if (0U != BT_thread_create(&tid, &hci_uart_task_attr, hci_uart_read_task, NULL))
    {
        HCI_UART_ERR(
        "[HCI-UART] Could NOT Create UART Read Thread\n");

        return;
    }

    BT_mem_set(hci_uart_rx_data_buff, 0x0, sizeof(hci_uart_rx_data_buff));
    hci_uart_rx.data     = NULL;
    hci_uart_rx.dataSize = 0U;
    hci_uart_rx_state    = BT_FALSE;
    hci_uart_rx_bytes    = 0U;

    /* Initialize UART State */
    hci_uart_state = 0x0U;

    HCI_UART_TRC(
    "[HCI-UART] UART Power On Initialization Complete\n");

    return;
}
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
static void hci_uartdma_transmit_cb
          (hal_uart_dma_handle_t handle,
           hal_dma_callback_msg_t *msg,
           void *callbackParam)
     {

#ifndef EM_ENABLE_PAL_OS
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t ret;
#else /* EM_ENABLE_PAL_OS */
    INT32      ret;
#endif /* EM_ENABLE_PAL_OS */
    API_RESULT retval;

    /* Data Received? */
    if (NULL != handle)
    {
        if (((hal_uart_dma_status_t)kStatus_HAL_UartDmaRxIdle == msg->status) ||
        (kStatus_HAL_UartDmaIdleline == msg->status))
        {
            /* Increment the rx Bytes marker */
            hci_uart_rx_bytes += hci_uart_rx.dataSize;

            /* Parse the HCI RX buffer packet */
            retval = ht_parse_packet (&ht);

            if (API_SUCCESS != retval)
            {
                /* BT_assert(0); */
                assert = 1U;
            }

            if (1U == ht.packet_expected_len)
            {
                /**
                 * Set the RX Receive State to be "BT_TRUE".
                 * The HCI UART Read Task should now pick the
                 * relevant data for further processing and
                 * invoke subsequent UART Reads(Non-blocking) for
                 * the required Bytes of Data.
                 */
                hci_uart_rx_state = BT_TRUE;

                /* Signal the Read Task Thread */
                /**
                 * NOTE:
                 * This Signal is made from an ISR context.
                 * TODO:
                 * Need to have a ISR Safe FreeRTOS API mapping here!
                 */
#ifndef EM_ENABLE_PAL_OS
                ret = xSemaphoreGiveFromISR
                      (
                          hci_uart_cond,
                          &xHigherPriorityTaskWoken
                      );

                if (pdPASS != ret)
                {
                    HCI_UART_ERR(
                    "[HCI-UART] SIGNAL from ISR failed with RET%d\r\n", ret);
                }
#else /* EM_ENABLE_PAL_OS */
                ret = BT_thread_cond_signal (&hci_uart_cond);

                if (0U != ret)
                {
                    HCI_UART_ERR(
                    "[HCI-UART] SIGNAL from ISR failed with RET%d\r\n", ret);
                }
#endif /* EM_ENABLE_PAL_OS */
            }

            if (BT_FALSE == hci_uart_rx_state)
            {
                /* Point the place holder for next data reception */
                hci_uart_rx.data     = hci_uart_rx_data_buff + hci_uart_rx_bytes;
                hci_uart_rx.dataSize = ht.packet_expected_len;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
               ret = HAL_UartDMATransferReceive
                (
                    (hal_uart_handle_t)hci_uart_handle,
                    hci_uart_rx.data,
                    hci_uart_rx.dataSize, true
                );
#else
                /* Invoke Receive Non-Blocking over UART again for expected length */
                (BT_IGNORE_RETURN_VALUE) HAL_UartReceiveNonBlocking
                (
                    (hal_uart_handle_t)hci_uart_handle,
                    hci_uart_rx.data,
                    hci_uart_rx.dataSize
                );
#endif
            }
        }
        else  if ((hal_uart_dma_status_t)kStatus_HAL_UartDmaTxIdle == msg->status)
        {
#if HCI_UART_TX_NONBLOCKING  || ((defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U)))
            ret = BT_thread_cond_signal (&hci_uart_tx_cond);

            if (0U != ret)
            {
                HCI_UART_ERR(
                "[HCI-UART] SIGNAL from ISR failed with RET%d\r\n", ret);
            }
#else

#endif /* HCI_UART_TX_NONBLOCKING */
        }
        else
        {
            assert(0);
        }
#ifndef EM_ENABLE_PAL_OS
        if (0 != xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(0U);
        }
#endif
    }
}
#endif
#ifdef BT_HAVE_SHUTDOWN
/** HCI-UART Shutdown */
void hci_uart_shutdown (void)
{
    /* Signal UART Read Task */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hci_uart_mutex);

    /* Check for Shutdown state */
    if (0x00U == hci_uart_state)
    {
        hci_uart_state = 0xFFU;

        /* Required to Signal the task */
        (BT_IGNORE_RETURN_VALUE) BT_thread_cond_signal (&hci_uart_cond);
    }

    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hci_uart_mutex);
}
#endif /* BT_HAVE_SHUTDOWN */

void hci_uart_transmit_cb
     (
         hal_uart_handle_t handle,
         hal_uart_status_t status,
         void              *userData
     )
{
#ifndef EM_ENABLE_PAL_OS
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t ret;
#else /* EM_ENABLE_PAL_OS */
    INT32      ret;
#endif /* EM_ENABLE_PAL_OS */
    API_RESULT retval;

    /* Data Received? */
    if (NULL != handle)
    {
        if ((hal_uart_status_t)kStatus_HAL_UartRxIdle == status)
        {
            /* Increment the rx Bytes marker */
            hci_uart_rx_bytes += hci_uart_rx.dataSize;

            /* Parse the HCI RX buffer packet */
            retval = ht_parse_packet (&ht);

            if (API_SUCCESS != retval)
            {
                /* BT_assert(0); */
                assert = 1U;
            }

            if (1U == ht.packet_expected_len)
            {
                /**
                 * Set the RX Receive State to be "BT_TRUE".
                 * The HCI UART Read Task should now pick the
                 * relevant data for further processing and
                 * invoke subsequent UART Reads(Non-blocking) for
                 * the required Bytes of Data.
                 */
                hci_uart_rx_state = BT_TRUE;

                /* Signal the Read Task Thread */
                /**
                 * NOTE:
                 * This Signal is made from an ISR context.
                 * TODO:
                 * Need to have a ISR Safe FreeRTOS API mapping here!
                 */
#ifndef EM_ENABLE_PAL_OS
                ret = xSemaphoreGiveFromISR
                      (
                          hci_uart_cond,
                          &xHigherPriorityTaskWoken
                      );

                if (pdPASS != ret)
                {
                    HCI_UART_ERR(
                    "[HCI-UART] SIGNAL from ISR failed with RET%d\r\n", ret);
                }
#else /* EM_ENABLE_PAL_OS */
                ret = BT_thread_cond_signal (&hci_uart_cond);

                if (0U != ret)
                {
                    HCI_UART_ERR(
                    "[HCI-UART] SIGNAL from ISR failed with RET%d\r\n", ret);
                }
#endif /* EM_ENABLE_PAL_OS */
            }

            if (BT_FALSE == hci_uart_rx_state)
            {
                /* Point the place holder for next data reception */
                hci_uart_rx.data     = &hci_uart_rx_data_buff[hci_uart_rx_bytes];
                hci_uart_rx.dataSize = ht.packet_expected_len;

                /* Invoke Receive Non-Blocking over UART again for expected length */
                (BT_IGNORE_RETURN_VALUE) HAL_UartReceiveNonBlocking
                (
                    (hal_uart_handle_t)hci_uart_handle,
                    hci_uart_rx.data,
                    hci_uart_rx.dataSize
                );
            }
        }
        else if ((hal_uart_status_t)kStatus_HAL_UartTxIdle == status)
        {
#if HCI_UART_TX_NONBLOCKING
            ret = BT_thread_cond_signal (&hci_uart_tx_cond);

            if (0U != ret)
            {
                HCI_UART_ERR(
                "[HCI-UART] SIGNAL from ISR failed with RET%d\r\n", ret);
            }
#endif /* HCI_UART_TX_NONBLOCKING */
        }
        else
        {
        }
#ifndef EM_ENABLE_PAL_OS
        if (0 != xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(0U);
        }
#endif
    }
}

/** HCI-UART Bluetooth-ON Initialization */
void hci_uart_bt_init(void)
{
    hal_uart_config_t config;
    hal_uart_status_t ret;
#if HCI_UART_TX_NONBLOCKING || ((defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U)))
    INT32 retVal;
#endif
    int error;
    controller_hci_uart_config_t getConfig;

    error = controller_hci_uart_get_configuration(&getConfig);
    assert(0 == error);
    if (0 != error)
    {
        /* TODO: To Be Removed! */
        while (true)
        {
        }
    }

    /* Set the HCI-UART Configuration parameters */
    config.srcClock_Hz  = getConfig.clockSrc;
    config.baudRate_Bps = getConfig.runningBaudrate;
    config.parityMode   = kHAL_UartParityDisabled;
    config.stopBitCount = kHAL_UartOneStopBit;
    config.enableRx     = 1U;
    config.enableTx     = 1U;
    config.instance     = getConfig.instance;
    config.enableRxRTS  = getConfig.enableRxRTS;
    config.enableTxCTS  = getConfig.enableTxCTS;
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
    config.txFifoWatermark = 0U;
    config.rxFifoWatermark = 0U;
#endif
    hci_uart_instance = getConfig.instance;

    (void)hci_uart_instance;

    HCI_UART_TRC(
    "[HCI-UART] Configuring Host Controller on Device(UART Inst) : %d\n",
    hci_uart_instance);

    /* Initialize UART with Adapter */
    ret = HAL_UartInit
          (
              (hal_uart_handle_t)hci_uart_handle,
              &config
          );
    /* Check if Assert or Log and return? */
    if (ret != kStatus_HAL_UartSuccess)
    {
        HCI_UART_ERR("Failed to initialize internal Bluetooth HCI UART!\r\n");

        HCI_UART_ERR(
        "[HCI-UART] FAILED to Open UART Device(UART Inst) : %d\n",
        hci_uart_instance);

        /* TODO: To Be Removed! */
        while (true)
        {
        }
    }
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    hal_uart_dma_config_t dmaConfig;
    dmaConfig.uart_instance = hci_uart_instance;
    dmaConfig.dma_instance  = getConfig.dma_instance;
    dmaConfig.rx_channel    = getConfig.rx_channel;
    dmaConfig.tx_channel    = getConfig.tx_channel;
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    dma_mux_configure_t  dma_mux;
    dma_mux.dma_dmamux_configure.dma_mux_instance = getConfig.dma_mux_instance;
    dma_mux.dma_dmamux_configure.rx_request = getConfig.rx_request ;
    dma_mux.dma_dmamux_configure.tx_request = getConfig.tx_request ;
    dmaConfig.dma_mux_configure = &dma_mux;
#endif
    
#if (defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && (FSL_FEATURE_EDMA_HAS_CHANNEL_MUX > 0U))
    dma_channel_mux_configure_t dmaChannelMux;
    dmaChannelMux.dma_dmamux_configure.dma_rx_channel_mux = getConfig.rx_request;
    dmaChannelMux.dma_dmamux_configure.dma_tx_channel_mux = getConfig.tx_request;
    dmaConfig.dma_channel_mux_configure = &dmaChannelMux;
#endif /* (defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && (FSL_FEATURE_EDMA_HAS_CHANNEL_MUX > 0U)) */

    // Init uart dma
    HAL_UartDMAInit( (hal_uart_handle_t)hci_uart_handle, (hal_uart_dma_handle_t *)hci_uart_DmaHandle, &dmaConfig);
#endif

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    ret = (hal_uart_status_t)HAL_UartDMATransferInstallCallback( (hal_uart_handle_t)hci_uart_handle,
                                                     hci_uartdma_transmit_cb,
                                                     NULL);
#else
    /* Install the UART TX-RX Callback */
    ret = HAL_UartInstallCallback
          (
              (hal_uart_handle_t)hci_uart_handle,
              hci_uart_transmit_cb,
              NULL
          );
#endif
    /* Check if Assert or Log and return? */
    if (ret != kStatus_HAL_UartSuccess)
    {
        HCI_UART_ERR("Failed to Register UART Rx-Tx Callback!\r\n");

        HCI_UART_ERR(
        "[HCI-UART] FAILED to register RX-TX Callback for UART Inst: %d\n",
        hci_uart_instance);

        /* TODO: To Be Removed! */
        while (true)
        {
        }
    }

    /* Initialize */
    /* Initialize the HCI-Transport Parser Structure */
    ht_parse_packet_init (&ht);
    /* Initialize the RX Byte Count for the current read session */
    hci_uart_rx_bytes    = 0U;
    /* Point the HCI-Transport Parser Databuffer to the Global RX buffer */
    ht.packet            = &hci_uart_rx_data_buff[0U];

    /* Initialize the UART RX DS to be used in the Platform API */
    hci_uart_rx.data     = &hci_uart_rx_data_buff[hci_uart_rx_bytes];
    hci_uart_rx.dataSize = ht.packet_expected_len;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
        ret = (hal_uart_status_t)HAL_UartDMATransferReceive
          (
              (hal_uart_handle_t)hci_uart_handle,
              hci_uart_rx.data,
              hci_uart_rx.dataSize, true
          );
#else
    ret = HAL_UartReceiveNonBlocking
          (
              (hal_uart_handle_t)hci_uart_handle,
              hci_uart_rx.data,
              hci_uart_rx.dataSize
          );
#endif
    if (ret != kStatus_HAL_UartSuccess)
    {
        HCI_UART_ERR(
        "[HCI-UART] First UART Receive Non-Blocking Failed for Inst: %d\n",
        hci_uart_instance);

        /* TODO: To Be Removed! */
        while (true)
        {
        }
    }

#if HCI_UART_TX_NONBLOCKING || ((defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U)))
    do
    {
        /* Clear the UART Send signal */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hci_uart_tx_mutex);
        retVal = BT_thread_cond_wait_timeout (&hci_uart_tx_cond, &hci_uart_tx_mutex, 0U);
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hci_uart_tx_mutex);
    } while (0U == retVal);
#endif

    /* Signal UART Read Task */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hci_uart_mutex);
    hci_uart_state = 0x1U;
    (BT_IGNORE_RETURN_VALUE) BT_thread_cond_signal (&hci_uart_cond);
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hci_uart_mutex);

#if HCI_UART_TX_NONBLOCKING || ((defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U)))
    /* Signal UART Send */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hci_uart_tx_mutex);
    (BT_IGNORE_RETURN_VALUE) BT_thread_cond_signal (&hci_uart_tx_cond);
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hci_uart_tx_mutex);
#endif

    assert = 0U;

    HCI_UART_TRC(
    "[HCI-UART] UART Bluetooth-ON Initialization Complete\n");

    return;
}

/** HCI-UART Bluetooth-OFF Shutdown */
void hci_uart_bt_shutdown (void)
{
    hal_uart_status_t ret;

#if HCI_UART_TX_NONBLOCKING || ((defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U)))
    /* Lock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hci_uart_tx_mutex);
#endif
    /* Signal UART Read Task */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hci_uart_mutex);

    hci_uart_state = 0x0U;

    ret = HAL_UartAbortReceive((hal_uart_handle_t)hci_uart_handle);
    if (ret != kStatus_HAL_UartSuccess)
    {
        HCI_UART_ERR(
        "[HCI-UART] UART Abort Receive Failed for UART Instance: %d\n",
        hci_uart_instance);

        /* TODO: To Be Removed! */
        while (true)
        {
        }
    }

    ret = HAL_UartAbortSend((hal_uart_handle_t)hci_uart_handle);
    if (ret != kStatus_HAL_UartSuccess)
    {
        HCI_UART_ERR(
        "[HCI-UART] UART Abort Send Failed for UART Instance: %d\n",
        hci_uart_instance);

        /* TODO: To Be Removed! */
        while (true)
        {
        }
    }

    ret = HAL_UartDeinit((hal_uart_handle_t)hci_uart_handle);
    if (ret != kStatus_HAL_UartSuccess)
    {
        HCI_UART_ERR(
        "[HCI-UART] UART De-Init Failed for UART Instance: %d\n",
        hci_uart_instance);

        /* TODO: To Be Removed! */
        while (true)
        {
        }
    }

    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hci_uart_mutex);
#if HCI_UART_TX_NONBLOCKING || ((defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U)))
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hci_uart_tx_mutex);
#endif
}

/** HCI-UART Read Task */
DECL_STATIC BT_THREAD_RETURN_TYPE hci_uart_read_task (BT_THREAD_ARGS args)
{
#ifdef HT_ENQUEUE_WITH_RETURN
    UINT16 nqd, tnqd;
    API_RESULT retval;
#endif /* HT_ENQUEUE_WITH_RETURN */

    BT_LOOP_FOREVER()
    {
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hci_uart_mutex);

        HCI_UART_TRC(
        "[HCI-UART] UART Read Task: Waiting to Service ..\n");

        (BT_IGNORE_RETURN_VALUE) BT_thread_cond_wait (&hci_uart_cond, &hci_uart_mutex);

        HCI_UART_TRC(
        "[HCI-UART] UART Read Task: Ready to Service ..\n");

        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hci_uart_mutex);

        if (0x1U == hci_uart_state)
        {
            /* Check if the RX State of UART is set to BT_TRUE */
            if (BT_TRUE == hci_uart_rx_state)
            {
                if (assert == 1U)
                {
                    printf("*%d*: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n", hci_uart_rx_bytes,
                            hci_uart_rx_data_buff[0U], hci_uart_rx_data_buff[1U],
                            hci_uart_rx_data_buff[2U], hci_uart_rx_data_buff[3U],
                            hci_uart_rx_data_buff[4U], hci_uart_rx_data_buff[5U],
                            hci_uart_rx_data_buff[6U], hci_uart_rx_data_buff[7U],
                            hci_uart_rx_data_buff[8U], hci_uart_rx_data_buff[9U]);

                    assert = 0U;
                }
                else
                {
                    /*printf("*%d*: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n", hci_uart_rx_bytes,
                            hci_uart_rx_data_buff[0], hci_uart_rx_data_buff[1],
                            hci_uart_rx_data_buff[2], hci_uart_rx_data_buff[3],
                            hci_uart_rx_data_buff[4], hci_uart_rx_data_buff[5],
                            hci_uart_rx_data_buff[6], hci_uart_rx_data_buff[7],
                            hci_uart_rx_data_buff[8], hci_uart_rx_data_buff[9]);*/

#ifdef HT_ENQUEUE_WITH_RETURN
                    nqd = 0U;
                    tnqd = 0U;
                    BT_LOOP_FOREVER()
                    {
                        tnqd += nqd;
                        retval = hci_transport_enqueue ((&hci_uart_rx_data_buff[tnqd]), ((UINT16)hci_uart_rx_bytes - tnqd), &nqd);

                        /* Not checking retval */
                        if (nqd == (hci_uart_rx_bytes - tnqd))
                        {
                            break;
                        }

                        BT_usleep(1U);
                    }

                    BT_IGNORE_UNUSED_PARAM(retval);

#else /* HT_ENQUEUE_WITH_RETURN */
                    hci_transport_enqueue(hci_uart_rx_data_buff, (UINT16)hci_uart_rx_bytes);

#endif /* HT_ENQUEUE_WITH_RETURN */
                }

                /* Reset the Rx Bytes marker */
                hci_uart_rx_bytes = 0U;
                /* BT_mem_set(hci_uart_rx_data_buff, 0x77, sizeof(hci_uart_rx_data_buff)); */

                /**
                 * Point the place holder for next data reception
                 * The global RX buffer hci_uart_rx_data_buff should be offset
                 * from its starting position, with the current bytes received
                 * and kept in that buffer.
                 */
                hci_uart_rx.data     = &hci_uart_rx_data_buff[hci_uart_rx_bytes];

                /**
                 * At this point the Expected len should be 1.
                 */
                hci_uart_rx.dataSize = ht.packet_expected_len;

                /* Reset  the HCI UART Receive State */
                hci_uart_rx_state = BT_FALSE;
                (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hci_uart_mutex);
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
                if (0x1U == hci_uart_state)
                {
                    /* Invoke Receive Non-Blocking over UART again for expected length */
                    (BT_IGNORE_RETURN_VALUE) HAL_UartDMATransferReceive
                      (
                          (hal_uart_handle_t)hci_uart_handle,
                          hci_uart_rx.data,
                          hci_uart_rx.dataSize, true
                      );

                }

#else
                if (0x1U == hci_uart_state)
                {
                    /* Invoke Receive Non-Blocking over UART again for expected length */
                    (BT_IGNORE_RETURN_VALUE) HAL_UartReceiveNonBlocking
                    (
                        (hal_uart_handle_t)hci_uart_handle,
                        hci_uart_rx.data,
                        hci_uart_rx.dataSize
                    );
                }
#endif
                (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hci_uart_mutex);

                /**
                 * TODO: Check what needs to be done if rx_bytes is returned by this
                 * function here itself with some partial data?
                 */
            }
        }
    }

#if 0
    /* fix build warning: statement is unreachable. */
    return BT_THREAD_RETURN_VAL_SUCCESS;
#endif /* 0 */
}

#ifdef HCI_UART_COLLECT_AND_WR_COMPLETE_PKT
/** HCI-UART Send Data */
API_RESULT hci_uart_send_data
           (UCHAR type, UCHAR * buf, UINT16 length, UCHAR flag)
{
    static UINT32 total_len = 0U;
    static UINT32 cur_len = 0U;
    static UCHAR acl_data_pkt = BT_FALSE;

    if (0x1U != hci_uart_state)
    {
        HCI_UART_ERR(
        "[HCI-UART] Incorrect UART State(%d)\n",
        hci_uart_state);

        return API_FAILURE;
    }

    if (0U != flag)
    {
        if (HCI_ACL_DATA_PACKET == type)
        {
            acl_data_pkt = BT_TRUE;
            total_len = (((UINT32)(buf[3U]) << 8U) | (UINT32)(buf[2U])) + 5U;
        }
        else
        {
            total_len = (UINT32)length + 1U;
        }

        if (HCI_UART_WR_BUF_SIZE < total_len)
        {
            HCI_UART_ERR(
            "[HCI-UART] HCI Packet Size %d exceeds Configuration %d bytes\n",
            total_len, HCI_UART_WR_BUF_SIZE);

            return API_FAILURE;
        }
    }

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    if (0U != acl_data_pkt)
    {
        if (0U == cur_len)
        {
            hci_uart_wr_buf[odd][0U] = type;
            cur_len = 1U;
        }

        BT_mem_copy (&hci_uart_wr_buf[odd][cur_len], buf, length);
        cur_len += length;

        if (cur_len != total_len)
        {
            return API_SUCCESS;
        }
    }
    else
    {
        hci_uart_wr_buf[odd][0U] = type;
        BT_mem_copy (&hci_uart_wr_buf[odd][1U], buf, length);
        total_len = (UINT32)length + 1U;
    }

    /* Write HCI Packet */
    hci_uart_write_data (hci_uart_wr_buf[odd], total_len);

    /* Transmitted packet logging in btsnoop format */
    BT_snoop_write_packet(hci_uart_wr_buf[odd][0U], 0U, (&hci_uart_wr_buf[odd][1U]), (total_len - 1U));
#else
    if (0U != acl_data_pkt)
    {
        if (0U == cur_len)
        {
            hci_uart_wr_buf[0U] = type;
            cur_len = 1U;
        }

        BT_mem_copy (&hci_uart_wr_buf[cur_len], buf, length);
        cur_len += length;

        if (cur_len != total_len)
        {
            return API_SUCCESS;
        }
    }
    else
    {
        hci_uart_wr_buf[0U] = type;
        BT_mem_copy (&hci_uart_wr_buf[1U], buf, length);
        total_len = (UINT32)length + 1U;
    }

    /* Write HCI Packet */
    hci_uart_write_data (hci_uart_wr_buf, total_len);

#ifdef BT_SNOOP_WRITE_TRUNCATE
    if (BT_SNOOP_PKTLEN_LIMIT > total_len)
    {
#endif /* BT_SNOOP_WRITE_TRUNCATE */
    /* Transmitted packet logging in btsnoop format */
    BT_snoop_write_packet(hci_uart_wr_buf[0U], 0U, (&hci_uart_wr_buf[1U]), (total_len - 1U));
#ifdef BT_SNOOP_WRITE_TRUNCATE
    }
#endif /* BT_SNOOP_WRITE_TRUNCATE */
#endif

    /* Re-initialize */
    cur_len = 0U;
    total_len = 0U;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    odd ^= 1;
#endif
    if (BT_FALSE != acl_data_pkt)
    {
        acl_data_pkt = BT_FALSE;
    }

    return API_SUCCESS;
}
#else
/** HCI-UART Send Data */
API_RESULT hci_uart_send_data
           (UCHAR type, UCHAR * buf, UINT16 length, UCHAR flag)
{
    if (0x1U != hci_uart_state)
    {
        HCI_UART_ERR(
        "[HCI-UART] Incorrect UART State(%d)\n",
        hci_uart_state);

        return API_FAILURE;
    }

    /* Write Packet Identifier */
    if (0x1U == flag)
    {
        hci_uart_write_data (&type, 1U);
    }

    /* Write HCI Packet */
    hci_uart_write_data (buf, length);

    return API_SUCCESS;
}
#endif /* HCI_UART_COLLECT_AND_WR_COMPLETE_PKT */


/** HCI-UART Write Data */
void hci_uart_write_data (UCHAR * buf, UINT16 length)
{
    HCI_UART_TRC(
    "[HCI-UART] Attempting to write HCI Packet of Length %d\n",
    length);

    /*printf("*%d*: %02x %02x %02x %02x\r\n", length, buf[0], buf[1], buf[2], buf[3]);*/

#if HCI_UART_TX_NONBLOCKING
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hci_uart_tx_mutex);
    HCI_UART_TRC(
    "[HCI-UART] HCI write: Waiting to Service ..\n");
    (BT_IGNORE_RETURN_VALUE) BT_thread_cond_wait (&hci_uart_tx_cond, &hci_uart_tx_mutex);
    HCI_UART_TRC(
    "[HCI-UART] HCI write: Ready to Service ..\n");
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    (BT_IGNORE_RETURN_VALUE) HAL_UartDMATransferSend
    (
        (hal_uart_handle_t)hci_uart_handle,
        buf,
        length
    );
#else
        /* Write HCI Packet */
    (BT_IGNORE_RETURN_VALUE) HAL_UartSendNonBlocking
    (
        (hal_uart_handle_t)hci_uart_handle,
        buf,
        length
    );
#endif
    HCI_UART_INC_TX_BYTES(length);

    BT_thread_mutex_unlock (&hci_uart_tx_mutex);
#else
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hci_uart_tx_mutex);
    HCI_UART_TRC(
    "[HCI-UART] HCI write: Waiting to Service ..\n");
    (BT_IGNORE_RETURN_VALUE) BT_thread_cond_wait (&hci_uart_tx_cond, &hci_uart_tx_mutex);
    HCI_UART_TRC(
    "[HCI-UART] HCI write: Ready to Service ..\n");
    /* Write HCI Packet */
    (BT_IGNORE_RETURN_VALUE) HAL_UartDMATransferSend
    (
        (hal_uart_handle_t)hci_uart_handle,
        buf,
        length
    );
    BT_thread_mutex_unlock (&hci_uart_tx_mutex);
#else
        /* Write HCI Packet */
    (BT_IGNORE_RETURN_VALUE) HAL_UartSendBlocking
    (
        (hal_uart_handle_t)hci_uart_handle,
        buf,
        length
    );
#endif

    HCI_UART_INC_TX_BYTES(length);
#endif /* HCI_UART_TX_NONBLOCKING */

#ifdef UART_TEST_MODE
    BT_debug_dump_bytes (buf, length);
#endif /* UART_TEST_MODE */
}

API_RESULT hci_transport_write_data (UCHAR type, UCHAR * buf, UINT16 length, UCHAR flag)
{
       return hci_uart_send_data(type, buf, length, flag);
}

#endif /* BT_UART */


