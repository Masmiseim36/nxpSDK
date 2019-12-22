/**
 * @file app_boot.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017-2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Implementation of the App booting time initilization functions
 */

#include "sm_types.h"
#include "app_boot.h"
#include "sm_printf.h"
#include "smCom.h"
#include <nxLog.h>
#include <time.h>
#include "sm_api.h"
#include "global_platf.h"
#include "sm_const.h"
#include "PlugAndTrust_HostLib_Ver.h"
#include "nxLog_App.h"
#include "inttypes.h"

#if AX_EMBEDDED
#include <ax_reset.h>
#include <se_reset_config.h>
#include "fsl_gpio.h"
#endif

#ifdef CPU_MIMXRT1052DVL6B
#include "fsl_trng.h"
#include "fsl_dcp.h"
#endif

//TODO: have all the ex included
//#include "a71ch_ex.h"
#include "sm_printf.h"
#include "sm_timer.h"

#if AX_EMBEDDED
#include "board.h"
#include "pin_mux.h"
#if defined(MBEDTLS)
#include "ksdk_mbedtls.h"
#endif
#endif /* FREEDOM */

#if defined(CPU_LPC54018)
#include "fsl_clock.h"
#include "fsl_reset.h"
#endif

#if defined(FRDM_K64F)
#define TARGET_PLATFORM "FRDM-K64F"
#elif defined(FRDM_KW41Z)
#define TARGET_PLATFORM "FRDM-KW41Z"
#elif defined(FRDM_K82F)
#define TARGET_PLATFORM "FRDM-K82F"
#elif defined(CPU_MIMXRT1052DVL6B)
#define TARGET_PLATFORM "i.MX RT 1050"
#endif

#ifdef CPU_MIMXRT1052DVL6B
#define TRNG0 TRNG
#endif

#if defined(CPU_LPC54018)
volatile uint32_t gDummy = 1; /* Put this is .data. Probably at 0x000 address */
#endif

static U16 establishConnnection(SmCommState_t *pCommState, const char *pConnectionParam);
static int translateCommunicationStatus(U16 connectStatus);

#ifdef USE_SERGER_RTT
extern void nInit_segger_Log(void);
#endif

int app_boot_Init()
{
#if defined(_WIN32) && defined(WIN32) && defined(DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

#if defined(FRDM_KW41Z) || defined(FRDM_K64F)
    BOARD_BootClockRUN();
#endif
#if defined(FRDM_K82F)
    BOARD_BootClockHSRUN();
#endif

#ifdef FREEDOM
    BOARD_InitPins();
    BOARD_InitDebugConsole();

    LED_BLUE_INIT(1);
    LED_GREEN_INIT(1);
    LED_RED_INIT(1);
#endif

#ifdef CPU_MIMXRT1052DVL6B
    dcp_config_t dcpConfig;
    trng_config_t trngConfig;

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

#if defined(IMX_RT)
    /* Data cache must be temporarily disabled to be able to use sdram */
    SCB_DisableDCache();
#endif

    /* Initialize DCP */
    DCP_GetDefaultConfig(&dcpConfig);
    DCP_Init(DCP, &dcpConfig);

    /* Initialize TRNG */
    TRNG_GetDefaultConfig(&trngConfig);
    /* Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.
    * It is optional.*/
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;

    /* Initialize TRNG */
    TRNG_Init(TRNG0, &trngConfig);
#endif

#if defined(CPU_LPC54018)

    while (gDummy--) {
        /* Forcefully use gDummy so that linker does not kick it out */
    }

    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_USART_CLK_ATTACH);

    /* Enable Clock for RIT */
    CLOCK_EnableClock(kCLOCK_Gpio3);

    /* attach 12 MHz clock to FLEXCOMM2 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC2_RST_SHIFT_RSTn);

    BOARD_InitBootPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();

#endif /* CPU_LPC54018 */

#ifdef FREEDOM
    LED_BLUE_ON();
#endif
#ifdef FRDM_K64F
    {
        /* For DHCP Ethernet */
        SYSMPU_Type *base = SYSMPU;
        base->CESR &= ~SYSMPU_CESR_VLD_MASK;
    }
    axReset_HostConfigure();
    axReset_PowerUp();
#endif

#ifdef USE_SERGER_RTT
    nInit_segger_Log();
#endif

#if !defined(USE_RTOS) || USE_RTOS == 0
    app_boot_Init_RTOS();
#endif
    LOG_I(PLUGANDTRUST_HOSTLIB_PROD_NAME_VER_FULL);
    return 0;
}

int app_boot_Init_RTOS()
{
#if (AX_EMBEDDED) && defined(MBEDTLS)
    CRYPTO_InitHardware();
#if defined(FSL_FEATURE_SOC_SHA_COUNT) && (FSL_FEATURE_SOC_SHA_COUNT > 0)
    CLOCK_EnableClock(kCLOCK_Sha0);
    RESET_PeripheralReset(kSHA_RST_SHIFT_RSTn);
#endif /* SHA */
#endif /* (AX_EMBEDDED) && defined(MBEDTLS) */

#if (AX_EMBEDDED)
    PRINTF("\r\nWarning: Running this example will issue a debug reset of the attached " SE_NAME "\r\n");
    PRINTF("  The content of the " SE_NAME " will be erased.\r\n");
    PRINTF("  ****************************************\r\n");
    PRINTF("Press any character to continue.\r\n");
    //GETCHAR();
#endif

    sm_initSleep();

    return 0;
}

int app_boot_Connect(SmCommState_t *pCommState, const char *pConnectionParam)
{
    int retVal;

    U16 comStatus = establishConnnection(pCommState, pConnectionParam);
    retVal = translateCommunicationStatus(comStatus);

    if (retVal == 0) {
#if defined(SCI2C)
        PRINTF("SCI2C_"); // To highlight the ATR format for SCI2C deviates from ISO7816-3
#elif defined(SPI)
        PRINTF("SCSPI_");
#elif defined(T1oI2C)
        PRINTF("SCT1oI2C_");
#endif

#if defined(TDA8029_UART)
        PRINTF("UART Baudrate Idx: 0x%02X\r\n", pCommState->param2);
        PRINTF("T=1           TA1: 0x%02X\r\n", pCommState->param1);
#endif
        PRINTF(PLUGANDTRUST_HOSTLIB_PROD_NAME_VER_FULL "\r\n");

        PRINTF("Applet Version   : 0x%" PRIX32 "X\r\n", pCommState->appletVersion);
        PRINTF("SecureBox Version: 0x%" PRIX16 "X\r\n", pCommState->sbVersion);

        PRINTF("\r\n");
        PRINTF("==========SELECT-DONE=========\r\n");
    }
    else {
        PRINTF("\r\n");
        PRINTF("==========SELECT-FAILED=========\r\n");
    }

#ifdef TARGET_PLATFORM
    PRINTF("Platform: %s\r\n", TARGET_PLATFORM);
#endif

#ifdef FREEDOM
    if (retVal != 0) {
        LED_RED_ON();
    }
#endif
    return retVal;
}

/**
 * Local static function to handle communication status
 * @param[in] connectStatus The status of the communication.
 * @return returns the legacy error code
 */

static int translateCommunicationStatus(U16 connectStatus)
{
    if ((connectStatus == ERR_CONNECT_LINK_FAILED) || (connectStatus == ERR_CONNECT_SELECT_FAILED)) {
        PRINTF("SM_Connect failed with status 0x%" PRIX16 "X\n", connectStatus);
        return 2;
    }
    else if (connectStatus == SMCOM_COM_FAILED) {
        PRINTF("SM_Connect failed with status 0x%" PRIX16 "X (Could not open communication channel)\n", connectStatus);
        return 4;
    }
    else if (connectStatus == SMCOM_PROTOCOL_FAILED) {
        PRINTF("SM_Connect failed with status 0x%" PRIX16 "X (Could not establish communication protocol)\n",
            connectStatus);
        return 5;
    }
    else if (connectStatus == ERR_NO_VALID_IP_PORT_PATTERN) {
        PRINTF("Pass the IP address and port number as arguments, e.g. \"127.0.0.1:8050\"!\n");
        return 3;
    }
    else if (connectStatus != SW_OK) {
        // return error code asis to handle AX layer errors
        return connectStatus;
    }
    else {
        return 0;
    }
}
/**
 * Local static function to establish a connection based on the communication port selected
 * @param[in] pCommState pointer where the communication state is updated.
 * @param[in] argc varibale length arg count based on the communication channel used.
 * @param[in] argv variable arguments based on the communication channel used.
 * @return returns the status of connection establishment
 */
static U16 establishConnnection(SmCommState_t *pCommState, const char *pConnectionParam)
{
    U16 connectStatus = 0;

    U8 Atr[64] = {0};
    U16 AtrLen = sizeof(Atr);

#if defined(TDA8029_UART) || defined(SCI2C) || defined(PCSC) || defined(SPI) || defined(IPC) || defined(T1oI2C) || \
    defined(SMCOM_PN7150) || defined(SMCOM_THREAD)
    connectStatus = SM_Connect(pCommState, Atr, &AtrLen);
#elif defined(SMCOM_JRCP_V1) || defined(SMCOM_JRCP_V2)

#if defined SMCOM_JRCP_V1 && !defined(SMCOM_JRCP_V2)
    pCommState->connType = kType_SE_Conn_Type_JRCP_V1;
#endif
#if defined SMCOM_JRCP_V2 && !defined(SMCOM_JRCP_V1)
    pCommState->connType = kType_SE_Conn_Type_JRCP_V2;
#endif
    if (pConnectionParam == NULL) {
        PRINTF("Pass the IP address and port number as arguments, e.g. \"127.0.0.1:8050\"!\n");
        return 4;
    }
    else {
        connectStatus = SM_RjctConnect(pConnectionParam, pCommState, Atr, &AtrLen);
    }
#elif defined(RJCT_VCOM)
    if (pConnectionParam == NULL) {
        PRINTF("Pass the COM Port as arguments, e.g. \"COM3\" or \"\\\\.\\COM18\" !\n");
        return 4;
    }
    else {
        connectStatus = SM_RjctConnect(pConnectionParam, pCommState, Atr, &AtrLen);
    }
#elif defined(SMCOM_PCSC)
    if (pConnectionParam == NULL) {
        PRINTF("Pass the COM Port as arguments, e.g. \"TBU\"\n");
        return 4;
    }
    else {
        connectStatus = SM_RjctConnect(pConnectionParam, pCommState, Atr, &AtrLen);
    }
#else
#error "No communication channel defined"
#endif // SMCOM_JRCP_V1

    if (AtrLen > 0) {
        int i = 0;
#if defined(T1oI2C_UM1225_SE050)
        PRINTF("ATR=0x");
#elif defined(T1oI2C_GP)
        /* for GP ATR is noting but CIP response*/
        PRINTF("CIP=0x");
#endif
        for (i = 0; i < AtrLen; i++) {
            PRINTF("%02X.", Atr[i]);
        }
        PRINTF("\n");
    }

#if defined(TDA8029_UART)
    PRINTF("UART Baudrate Idx: 0x%02X\n", pCommState->param2);
    PRINTF("T=1           TA1: 0x%02X\n", pCommState->param1);
#endif

    return connectStatus;
}

void app_test_status(U8 result)
{
#if FREEDOM
    LED_BLUE_OFF();

    if (result == 0) {
        LED_RED_ON();
    }
    else {
        LED_GREEN_ON();
    }
#endif
    PRINTF("END. Status %s (Compiled on %s %s)\r\n", ((result == 1) ? "OK" : "FAILED"), __DATE__, __TIME__);
}
