/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_common.h"
#include "controller_ind_reset.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_os_abstraction.h"
#include <porting.h>
#if (defined(CONFIG_BT_IND_RESET) && (CONFIG_BT_IND_RESET > 0U))
#include "BT_hci_api.h"
#include "controller_hci_uart.h"
#include "fw_loader_uart.h"
#include "controller.h"
#include "fsl_adapter_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PRINTF_E(...)                     PRINTF(__VA_ARGS__)

#ifdef ENABLE_IND_RESET_DBG
#define PRINTF_D(...)                     PRINTF(__VA_ARGS__)
#else
#define PRINTF_D(...)
#endif /*ENABLE_IND_RESET_DBG*/

#define IND_RESET_INBAND_OPCODE                 0xFCFCU
#define ir_state_update(status)                  \
{                                                \
    ir_state = status;                           \
    PRINTF_D("IR status update: %s\n", #status); \
}
#define INDEPENDENT_RESET_VENDOR_CMD_ENABLE_IR  0x0DU
#define INDEPENDENT_RESET_VENDOR_CMD_TRIGGER_IR 0xFCU
#define UART_RX_QUEUE_SIZE                      (1024U)

typedef enum
{
    IND_RESET_BT_FW_DNLD_COMPELTED,
    IND_RESET_CONFIG_OOB_GPIO,
    IND_RESET_CONFIG_IR,
    IND_RESET_CONFIG_SEND_CMD,
    IND_RESET_CONFIG_CMPLT,
    IND_RESET_UNSUPPORTED_MODE,
    IND_RESET_BT_FW_DNLD_START,
    IND_RESET_OOB_TRIGGER_IR,
    IND_RESET_CONFIG_RESP_STATUS,
    IND_RESET_INBAND_TRIGGER_START,
    IND_RESET_INBAND_SEND_TRIGGER,
    IND_RESET_INBAND_CHECK_TRIGGER_RSP,
    IND_RESET_INBAND_TRIGGER_WAIT_RSP,
    IND_RESET_TRIGGER_COMPLETE,
    IND_RESET_NOT_CONFIGURED,
    IND_RESET_BT_OFF,
    IND_RESET_CONFIG_RESP_STATUS_ERR,
    IND_RESET_CONFIG_GET_RSP_ERR,
    IND_RESET_UART_DEINIT_FAILURE,
    IND_RESET_TRIGGER_FAIL,
    IND_RESET_UART_SEND_FAILURE,
    IND_RESET_UART_INIT_FAILURE
} ir_state_t;
typedef struct _uart_meta_data_
{
    uint8_t *data;   /* The buffer of data to be Txed/Rxed */
    size_t dataSize; /* The byte count to be Txed/Rxed. */
} uartMetaData_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
#if (defined(CONFIG_BT_IND_RESET) && (CONFIG_BT_IND_RESET > 0U))
static void configure_ir_reset_ind_gpio(void);
static void controller_ir_trigger_oob_ir(void);
static uint8_t controller_ir_wait_for_ir_config_status(void);
static bool controller_ir_trigger_inband_ir(void);
static void controller_ir_uart_tr_rx_cb(hal_uart_handle_t handle, hal_uart_status_t status, void *userData);
static hal_uart_status_t controller_ir_init_uart(hal_uart_handle_t handle, uint32_t u32BaudRate, bool flowControl);
static hal_uart_status_t controller_ir_deinit_uart(hal_uart_handle_t handle);
static BT_THREAD_RETURN_TYPE controller_ir_redownload_firmware(BT_THREAD_ARGS args);
static void controller_ir_create_uart_read_task(BT_THREAD_START_ROUTINE routine);
static UART_HANDLE_DEFINE(s_uartHandle);
extern uint16_t ethermind_hci_event_callback
           (uint8_t  event_type,
            uint8_t *event_data,
            uint8_t  event_datalen);
#endif /* ENABLE_BT_IND_RESET */

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if (defined(CONFIG_BT_IND_RESET) && (CONFIG_BT_IND_RESET > 0U))
uint8_t g_ir_hci_cmd_status = 0xFFU;
AT_NONCACHEABLE_SECTION_ALIGN(static UCHAR uart_rx_data_buff[UART_RX_QUEUE_SIZE], 4);
static uartMetaData_t uart_rx;
static UCHAR uart_rx_state;
static UINT16 uart_rx_bytes;
static uint16_t uart_read_expected_len;
static BT_thread_type btUartRdTask_tid = NULL;
static ind_reset_mode_t ind_reset_mode               = IND_RESET_DISABLE;
static ir_state_t ir_state;
OSA_SEMAPHORE_HANDLE_DEFINE(xStartIRThread);
OSA_SEMAPHORE_HANDLE_DEFINE(xUartTxSent);
OSA_SEMAPHORE_HANDLE_DEFINE(xUartRxRcvd);
OSA_SEMAPHORE_HANDLE_DEFINE(xWaitForBTDownload);
#if (defined(CONFIG_BT_IND_RESET_OOB) && (CONFIG_BT_IND_RESET_OOB > 0U))
static GPIO_HANDLE_DEFINE(ir_gpio_handle);
static hal_gpio_pin_config_t ir_gpio_config = 
{
    .direction = kHAL_GpioDirectionOut,
    .level     = 1,
    .port      = BOARD_BT_IND_RESET_OOB_PORT,
    .pin       = BOARD_BT_IND_RESET_OOB_PIN,
};
#endif/*ENABLE_BT_IND_RESET_OOB*/
#endif /*ENABLE_BT_IND_RESET*/

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint8_t controller_ir_indconfig_cmd(ind_reset_mode_t ind_reset)
{
    uint8_t             err;
    struct net_buf *buf;
    UCHAR ir_payload[2];

    buf = bt_hci_cmd_create(BT_OP(BT_OGF_VS, INDEPENDENT_RESET_VENDOR_CMD_ENABLE_IR), 2U);
    if (NULL == buf)
    {
        PRINTF("No buffer space available\r\n");
        return -1;
    }

    ir_payload[0] = ind_reset;
    ir_payload[1] = 0xFFU; /*default GPIO will be used by controller for IR*/
    
    net_buf_add_mem(buf, &ir_payload[0], 2U);
    err = bt_hci_cmd_send_sync(BT_OP(BT_OGF_VS, INDEPENDENT_RESET_VENDOR_CMD_ENABLE_IR), buf, NULL);
    if (0 != err)
    {
        PRINTF("Send hci command 0x%x failed (err %d)\n", INDEPENDENT_RESET_VENDOR_CMD_ENABLE_IR, err);
        return err;
    }
    else
    {
        OSA_TimeDelay(100);
        err = 0;
    }
    return err;
}

static void controler_config_ind_reset(ind_reset_mode_t ind_reset)
{
    ir_state_update(IND_RESET_CONFIG_IR);

    if ((ind_reset == IND_RESET_DISABLE) || (ind_reset == IND_RESET_OOB) || (ind_reset == IND_RESET_INBAND))
    {
        ir_state_update(IND_RESET_CONFIG_SEND_CMD);
        uint8_t status = controller_ir_indconfig_cmd(ind_reset);
        if (status != 0x00)
        {
            ir_state_update(IND_RESET_CONFIG_GET_RSP_ERR);
            PRINTF_E("Error: IR configure hci response ir_state = %d for ind_reset_mode:%d\n", ir_state, ind_reset);
        }
        else
        {
            ind_reset_mode = ind_reset;
            PRINTF("IR configured successfully for mode %d, ir_state = %d\n", ind_reset_mode, ir_state);
            ir_state_update(IND_RESET_CONFIG_CMPLT);
        }
    }
    else
    {
        PRINTF_E("Error: Unsupported IR mode entered, %d, ir_state = %d\n", ind_reset_mode, ir_state);
        ir_state_update(IND_RESET_UNSUPPORTED_MODE);
    }
    return;
}

static BT_THREAD_RETURN_TYPE controller_ir_redownload_firmware(BT_THREAD_ARGS args)
{
    while (1)
    {
        OSA_SemaphoreWait(xStartIRThread, osaWaitForever_c);
        ir_state_update(IND_RESET_BT_FW_DNLD_START);
        controller_init();
        ir_state_update(IND_RESET_BT_FW_DNLD_COMPELTED);
        OSA_SemaphorePost(xWaitForBTDownload);
    }
}

static void controller_ir_trigger_oob_ir(void)
{
#if (defined(CONFIG_BT_IND_RESET_OOB) && (CONFIG_BT_IND_RESET_OOB > 0U))
    /*since, controller's isr registered with falling edge trigger,
     * there is no need to maintain pulse interval more than 10ms*/
    PRINTF("Sending Out of Band IR Trigger\n");
    
    HAL_GpioInit(ir_gpio_handle, &ir_gpio_config);
    ir_state_update(IND_RESET_OOB_TRIGGER_IR);
    HAL_GpioSetOutput(ir_gpio_handle,  0);
    OSA_TimeDelay(10);
    HAL_GpioSetOutput(ir_gpio_handle,  1);
#endif/*ENABLE_BT_IND_RESET_OOB*/
}

static bool controller_ir_trigger_inband_ir(void)
{
    hal_uart_status_t error;
    bool ret            = false;
    uint8_t sendingBuffer[5];
    uint8_t inbandStatusIndex = 6U;
    uint16_t resp_opcode;
    PRINTF("Sending Inband IR Trigger\n");
    ir_state_update(IND_RESET_INBAND_TRIGGER_START);
    memset(sendingBuffer, 0, sizeof(sendingBuffer));

    if ((error = controller_ir_init_uart((hal_uart_handle_t)s_uartHandle, 3000000U, true)) != kStatus_HAL_UartSuccess)
    {
        PRINTF_E("fw_loader_init_uart Failed for UART Instance: %X\n", error);
        ir_state_update(IND_RESET_UART_INIT_FAILURE);
        return ret;
    }
    ir_state_update(IND_RESET_INBAND_SEND_TRIGGER);
    /* sending ir-inband trigger command!!*/
    sendingBuffer[0] = 0x01U;
    uint16_t *opcode = (uint16_t *)&sendingBuffer[1];
    *opcode          = (uint16_t)BT_OP(BT_OGF_VS, INDEPENDENT_RESET_VENDOR_CMD_TRIGGER_IR);
    sendingBuffer[3] = 0; /* no data, so 0 parameter length*/

    PRINTF_D("IR INBAND CMD:%02X %02X %02X %02X\n", sendingBuffer[0], sendingBuffer[1], sendingBuffer[2],
             sendingBuffer[3]);
    error = HAL_UartSendNonBlocking((hal_uart_handle_t)s_uartHandle, &sendingBuffer[0], 4U);
    if (error != kStatus_HAL_UartSuccess)
    {
        PRINTF_E("HAL_UartSendNonBlocking Failed for UART Instance: %X\n", error);
        ir_state_update(IND_RESET_UART_SEND_FAILURE);
        return ret;
    }
    OSA_SemaphoreWait(xUartTxSent, 10);
    ir_state_update(IND_RESET_INBAND_CHECK_TRIGGER_RSP);
    uart_read_expected_len = 7U;
    uart_rx.dataSize       = uart_read_expected_len;
    error = HAL_UartReceiveNonBlocking((hal_uart_handle_t)s_uartHandle, uart_rx.data, uart_rx.dataSize);
    if (error != kStatus_HAL_UartSuccess)
    {
        PRINTF_E("HAL_UartReceiveNonBlocking Failed for UART Instance: %X\n", error);
        ir_state_update(IND_RESET_CONFIG_RESP_STATUS_ERR);
        return ret;
    }
    ir_state_update(IND_RESET_INBAND_TRIGGER_WAIT_RSP);
    OSA_SemaphoreWait(xUartRxRcvd, 10);
    resp_opcode = (uart_rx.data[4] << 8) | uart_rx.data[5];
    PRINTF_D("IR INBAND RSP (0x%04X):%02X %02X %02X %02X %02X %02X %02X\n", resp_opcode, uart_rx.data[0],
             uart_rx.data[1], uart_rx.data[2], uart_rx.data[3], uart_rx.data[4], uart_rx.data[5], uart_rx.data[6]);

    if ((resp_opcode == IND_RESET_INBAND_OPCODE) && (uart_rx.data[inbandStatusIndex] == 0x00))
    {
        ir_state_update(IND_RESET_TRIGGER_COMPLETE);
        ret = true;
    }
    else
    {
        ir_state_update(IND_RESET_TRIGGER_FAIL);
    }

    if ((error = controller_ir_deinit_uart((hal_uart_handle_t)s_uartHandle)) != kStatus_HAL_UartSuccess)
    {
        PRINTF_E("HAL_UartDeInit Failed for UART Instance: %X\n", error);
        ir_state_update(IND_RESET_UART_DEINIT_FAILURE);
    }
    return ret;
}

void controler_ind_reset(ind_reset_mode_t ind_reset)
{
    bool ir_triggered = false;
    API_RESULT retval;
 #if (defined(CONFIG_BT_IND_RESET_OOB) && (CONFIG_BT_IND_RESET_OOB > 0U))
   if ((ind_reset != IND_RESET_INBAND) && (ind_reset != IND_RESET_OOB))  
#else
    if (ind_reset != IND_RESET_INBAND)
#endif
    {
        ir_state_update(IND_RESET_NOT_CONFIGURED);
        PRINTF("IND RESET NOT CONFIGURED\n");
        return;
    }  
    controler_config_ind_reset(ind_reset);

    ir_state_update(IND_RESET_BT_OFF);
    PRINTF("EtherMind: Bluetooth OFF ...\n");
    (void)BT_bluetooth_off();
    OSA_TimeDelay(10);

    if (ind_reset_mode == IND_RESET_INBAND)
    {
        ir_triggered = controller_ir_trigger_inband_ir();
    }
    else if (ind_reset_mode == IND_RESET_OOB)
    {
        controller_ir_trigger_oob_ir();
        ir_triggered = true;
    }
    ind_reset_mode = IND_RESET_DISABLE;
    if (ir_triggered == true)
    {
        static bool oneTimeInit = true;
        if (oneTimeInit)
        {
            OSA_SemaphoreCreate(xWaitForBTDownload, 0);
            OSA_SemaphoreCreate(xStartIRThread, 0);
            controller_ir_create_uart_read_task(controller_ir_redownload_firmware);
            oneTimeInit = false;
        }
        OSA_SemaphorePost(xStartIRThread);
        OSA_SemaphoreWait(xWaitForBTDownload, (1000U * 20U));
    }
    PRINTF("IR exit with state = %d\n", ir_state);

    retval = BT_bluetooth_on
         (
             ethermind_hci_event_callback,
             NULL,
             (CHAR *)CONFIG_BT_DEVICE_NAME
         );
    if (API_SUCCESS != retval)
    {
        PRINTF("BT_bluetooth_on failure = %d\n", retval);
    }
    
    return;
}

static void controller_ir_create_uart_read_task(BT_THREAD_START_ROUTINE routine)
{
    BT_thread_attr_type btUartRd_attr;

    /* Initialize the Write Task Attributes */
    btUartRd_attr.thread_name       = (DECL_CONST CHAR *)"BTURTRD";
    btUartRd_attr.thread_stack_size = BT_TASK_STACK_DEPTH;
    btUartRd_attr.thread_priority   = BT_TASK_PRIORITY;

    if (BT_thread_create(&btUartRdTask_tid, &btUartRd_attr, routine, NULL) < 0)
    {
        PRINTF_E("failed to Create btUartRead_task_create\n");
    }
}

static hal_uart_status_t controller_ir_init_uart(hal_uart_handle_t handle, uint32_t u32BaudRate, bool flowControl)
{
    hal_uart_config_t config;
    controller_hci_uart_config_t hciUartConfig;
    hal_uart_status_t status = kStatus_HAL_UartSuccess;

    memset(&hciUartConfig, 0, sizeof(hciUartConfig));
    memset(&config, 0, sizeof(config));
    controller_hci_uart_get_configuration(&hciUartConfig);

    /* Set the HCI-UART Configuration parameters */
    config.srcClock_Hz  = hciUartConfig.clockSrc;
    config.parityMode   = kHAL_UartParityDisabled;
    config.stopBitCount = kHAL_UartOneStopBit;
    config.enableRx     = 1;
    config.enableTx     = 1;
    config.instance     = hciUartConfig.instance;
    config.baudRate_Bps = u32BaudRate;
    config.enableRxRTS  = 0;
    config.enableTxCTS  = 0;
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
    config.txFifoWatermark = 0U;
    config.rxFifoWatermark = 0U;
#endif

    if (flowControl)
    {
        config.enableRxRTS = hciUartConfig.enableRxRTS;
        config.enableTxCTS = hciUartConfig.enableTxCTS;
    }

    status = HAL_UartInit((hal_uart_handle_t)handle, &config);
    if (status != kStatus_HAL_UartSuccess)
    {
        PRINTF_E("Failed to initialize BT UART, %x!\r\n", status);
    }
    else
    {
        /* Install the UART TX-RX Callback */
        status = HAL_UartInstallCallback((hal_uart_handle_t)handle, controller_ir_uart_tr_rx_cb, NULL);
        if (status != kStatus_HAL_UartSuccess)
        {
            PRINTF_E("Failed to install uart callbacks, %x!\r\n", status);
        }
    }

    if (status == kStatus_HAL_UartSuccess)
    {
        /* Initialize the UART RX DS to be used in the Platform API */
        memset(uart_rx_data_buff, 0, sizeof(uart_rx_data_buff));
        uart_rx_bytes = 0;
        uart_rx.data  = &uart_rx_data_buff[uart_rx_bytes];

        OSA_SemaphoreCreate(xUartRxRcvd, 0);
        OSA_SemaphoreCreate(xUartTxSent, 0);
        uart_rx_state = BT_TRUE;

        OSA_TimeDelay(50U);
    }

    return status;
}

static hal_uart_status_t controller_ir_deinit_uart(hal_uart_handle_t handle)
{
    hal_uart_status_t status = kStatus_HAL_UartSuccess;

    if (uart_rx_state == BT_TRUE)
    {
        (void)HAL_UartAbortReceive(handle);
        (void)HAL_UartAbortSend(handle);
        status = HAL_UartDeinit(handle);

        if (status != kStatus_HAL_UartSuccess)
        {
            PRINTF_E("UARTDeinit Failed for UART Instance: %x\n", status);
        }
        else
        {
            OSA_SemaphoreDestroy(xUartRxRcvd);
            OSA_SemaphoreDestroy(xUartTxSent);
            uart_rx_state = BT_FALSE;
        }
        OSA_TimeDelay(100);
    }
    return status;
}

static void controller_ir_uart_tr_rx_cb(hal_uart_handle_t handle, hal_uart_status_t status, void *userData)
{
    /* Data Received? */
    if (NULL != handle)
    {
        int ret;

        if ((hal_uart_status_t)kStatus_HAL_UartRxIdle == status)
        {
            /* Increment the rx Bytes marker */
            uart_rx_bytes += uart_rx.dataSize;

            if (uart_rx.dataSize == uart_read_expected_len)
            {
                PRINTF_D("RX %d\r\n", uart_read_expected_len);

                ret = OSA_SemaphorePost(xUartRxRcvd);
                if (0U != ret)
                {
                    PRINTF_E("rx signal failed with ret %d\r\n", ret);
                }
            }
            else
            {
                /* Point the place holder for next data reception */
                uart_rx.data     = &uart_rx_data_buff[uart_rx_bytes];
                uart_rx.dataSize = uart_rx.dataSize - uart_rx_bytes;
                /* Invoke Receive Non-Blocking over UART again for expected length */
                (BT_IGNORE_RETURN_VALUE)
                    HAL_UartReceiveNonBlocking((hal_uart_handle_t)handle, uart_rx.data, uart_rx.dataSize);
            }
        }
        else if ((hal_uart_status_t)kStatus_HAL_UartTxIdle == status)
        {
            ret = OSA_SemaphorePost(xUartTxSent);
            if (0U != ret)
            {
                PRINTF_E("tx signal failed with ret %d\r\n", ret);
            }
        }
        else
        {
            //
        }
    }
}
#endif /*ENABLE_BT_IND_RESET*/
