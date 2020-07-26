/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.d
 */

#include "sln_shell.h"
#include "mbedtls/base64.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
#define SLN_SERIAL_MANAGER_RECEIVE_BUFFER_LEN 2048U
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void *pvPortCalloc(size_t nmemb, size_t xSize);

static shell_status_t sln_reset_handler(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/
SHELL_COMMAND_DEFINE(reset, "\r\n\"reset\": Resets the MCU\r\n", sln_reset_handler, 0);

static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE];
static shell_handle_t s_shellHandle;

static uint8_t s_serialHandleBuffer[SERIAL_MANAGER_HANDLE_SIZE];
static serial_handle_t s_serialHandle = &s_serialHandleBuffer[0];

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
__attribute__((section(".ocram_data"))) __attribute__((aligned(8)))
uint8_t readRingBuffer[SLN_SERIAL_MANAGER_RECEIVE_BUFFER_LEN];
#endif

static EventGroupHandle_t s_ShellEventGroup;
#if USE_WIFI_CONNECTION
static wifi_cred_t s_wifi_cred = {0};
#endif
static TaskHandle_t s_appInitTask      = NULL;
static shell_heap_trace_t s_heap_trace = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    if (CONTROLLER_ID == kSerialManager_UsbControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, 480000000U);
    }
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
}

static shell_status_t sln_reset_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, RESET_EVENT, &xHigherPriorityTaskWoken);

    return kStatus_SHELL_Success;
}

int log_shell_printf(const char *formatString, ...)
{
    va_list ap;
    char logbuf[configLOGGING_MAX_MESSAGE_LENGTH] = {0};

    va_start(ap, formatString);
    vsnprintf(logbuf, configLOGGING_MAX_MESSAGE_LENGTH, formatString, ap);

    va_end(ap);

    SHELL_Write(s_shellHandle, logbuf, strlen(logbuf));

    return 0;
}

int sln_shell_init(void)
{
    status_t status = 0;
    serial_manager_config_t serialConfig;

    serial_port_usb_cdc_config_t usbCdcConfig = {
        .controllerIndex = (serial_port_usb_cdc_controller_index_t)CONTROLLER_ID,
    };

    s_ShellEventGroup = xEventGroupCreate();

    static volatile uint8_t usb_clock_initialized = 0;
    if (!usb_clock_initialized)
    {
        usb_clock_initialized = 1;
        USB_DeviceClockInit();
    }

    /* Init Serial Manager for USB CDC */
    serialConfig.type = kSerialPort_UsbCdc;
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    serialConfig.ringBuffer     = &readRingBuffer[0];
    serialConfig.ringBufferSize = SLN_SERIAL_MANAGER_RECEIVE_BUFFER_LEN;
#endif
    serialConfig.portConfig = &usbCdcConfig;

    status = SerialManager_Init(s_serialHandle, &serialConfig);
    if (status != kStatus_SerialManager_Success)
    {
        return (int32_t)status;
    }

    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];
    SHELL_Init(s_shellHandle, s_serialHandle, "SHELL>> ");

    /* Add the SLN commands to the commands list */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(reset));

    return status;
}

void sln_shell_task(void *arg)
{
    volatile EventBits_t shellEvents = 0U;

    SHELL_Printf(s_shellHandle, "Howdy! Type \"help\" to see what this shell can do!\r\n");
    SHELL_Printf(s_shellHandle, "SHELL>> ");

    while (1)
    {
        shellEvents = xEventGroupWaitBits(s_ShellEventGroup, 0x00FFFFFF, pdTRUE, pdFALSE, portMAX_DELAY);

        if (shellEvents & RESET_EVENT)
        {
            /* this rather drastic approach is used for when one wants to use another
             * wifi network after successfully connecting to another one previously */
            NVIC_SystemReset();
        }
    }

    while (SerialManager_Deinit(s_serialHandle) == kStatus_SerialManager_Busy)
    {
        /* should happen pretty quickly after the call of g_shellCommandexit.pFuncCallBack,
         * just need to wait till the read and write handles are closed */
    }

    /* clean event group */
    vEventGroupDelete(s_ShellEventGroup);

    vTaskDelete(NULL);
}

void sln_shell_set_app_init_task_handle(TaskHandle_t *handle)
{
    if (NULL != handle)
    {
        s_appInitTask = *handle;
    }
}

void sln_shell_trace_malloc(void *ptr, size_t size)
{
    if (s_shellHandle)
    {
        if (s_heap_trace.enable)
        {
            if (size >= s_heap_trace.threshold)
            {
                SHELL_Printf(s_shellHandle, "[TRACE] Allocated %d bytes to 0x%X\r\n", size, (int)ptr);
                SHELL_Printf(s_shellHandle, "SHELL>> ");
            }
        }
    }
}

void sln_shell_trace_free(void *ptr, size_t size)
{
    if (s_shellHandle)
    {
        if (s_heap_trace.enable)
        {
            if (size >= s_heap_trace.threshold)
            {
                SHELL_Printf(s_shellHandle, "[TRACE] De-allocated %d bytes from 0x%X\r\n", size, (int)ptr);
                SHELL_Printf(s_shellHandle, "SHELL>> ");
            }
        }
    }
}
#if !(SDK_DEBUGCONSOLE)
int DbgConsole_Printf(const char *formatString, ...)
{
    va_list ap;
    char logbuf[configLOGGING_MAX_MESSAGE_LENGTH] = {0};

    va_start(ap, formatString);
    vsnprintf(logbuf, configLOGGING_MAX_MESSAGE_LENGTH, formatString, ap);

    va_end(ap);

    return 0;
}
#endif

/* Need to generate a unique ID for the client ID */
void APP_GetUniqueID(char **uniqueID)
{
    uint64_t u64UniqueIDRaw = (uint64_t)((uint64_t)OCOTP->CFG1 << 32ULL) | OCOTP->CFG0;

    uint32_t cIdLen = 0;
    mbedtls_base64_encode(NULL, 0, &cIdLen, (const unsigned char *)&u64UniqueIDRaw, sizeof(uint64_t));
    *uniqueID = (char *)pvPortMalloc(cIdLen + 1);
    memset(*uniqueID, 0, cIdLen + 1);
    uint32_t outputLen = 0;
    mbedtls_base64_encode((unsigned char *)*uniqueID, cIdLen, &outputLen, (const unsigned char *)&u64UniqueIDRaw,
                          sizeof(uint64_t));
}
