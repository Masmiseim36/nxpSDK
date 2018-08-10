/**
 * @file app_boot.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2017
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
 *
 * @par Description
 * Implementation of the App booting time initilization functions
 */

#include "app_boot.h"

#include <time.h>
#include "ax_api.h"
#include "global_platf.h"

#ifdef CPU_MIMXRT1052DVL6B
#include "fsl_trng.h"
#include "fsl_dcp.h"
#endif

//TODO: have all the ex included
//#include "a71ch_ex.h"
#include "sm_printf.h"
#include "sm_timer.h"

#if defined(CPU_MIMXRT1052DVL6B)
#include "fsl_trng.h"
#include "fsl_dcp.h"
#endif

#if defined(FREEDOM) || defined(IMX_RT)
#   include "board.h"
#   include "pin_mux.h"
#endif /* FREEDOM */

#if defined(FREEDOM) && defined(MBEDTLS)
#   include "ksdk_mbedtls.h"
#endif /* FREEDOM && MBEDTLS */

#if defined(FRDM_K64F)
#    define TARGET_PLATFORM             "FRDM-K64F"
#elif defined(FRDM_KW41Z)
#    define TARGET_PLATFORM             "FRDM-KW41Z"
#elif defined(FRDM_K82F)
#    define TARGET_PLATFORM             "FRDM-K82F"
#elif defined(CPU_MIMXRT1052DVL6B)
#    define TARGET_PLATFORM             "i.MX RT 1050"
#endif

#ifdef CPU_MIMXRT1052DVL6B
#define TRNG0 TRNG
#endif

static U16 establishConnnection(
    SmCommState_t* pCommState, const char* pConnectionParam);
static int translateCommunicationStatus(
    U16 connectStatus);

int app_boot_Init()
{
#if defined(_WIN32) && defined(WIN32) && defined(DEBUG)
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

#if defined (FRDM_KW41Z)|| defined (FRDM_K64F)
    BOARD_BootClockRUN();
#endif
#if defined( FRDM_K82F)
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

    /*TODO: Check if it can be enabled later on*/
    /* Data cache must be temporarily disabled to be able to use sdram */
 //   SCB_DisableDCache();



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


#if defined(FREEDOM) && defined(MBEDTLS)
    CRYPTO_InitHardware();
#endif  /* FREEDOM && MBEDTLS */


#ifdef FREEDOM
    LED_BLUE_ON();
#endif

    sm_initSleep();

    return 0;
}

int app_boot_Connect(
    SmCommState_t* pCommState, const char* pConnectionParam)
{

    int retVal;

    U16 comStatus = establishConnnection(pCommState, pConnectionParam);
    retVal = translateCommunicationStatus(comStatus);

    if (retVal == 0) {
#if defined(I2C)
        sm_printf(CONSOLE, "SCI2C_"); // To highlight the ATR format for SCI2C deviates from ISO7816-3
#elif defined(SPI)
        sm_printf(CONSOLE, "SCSPI_");
#endif

#if defined(TDA8029_UART)
        sm_printf(CONSOLE, "UART Baudrate Idx: 0x%02X\r\n", pCommState->param2);
        sm_printf(CONSOLE, "T=1           TA1: 0x%02X\r\n", pCommState->param1);
#endif
        sm_printf(CONSOLE, "HostLib Version  : 0x%04X\r\n", pCommState->hostLibVersion);

        sm_printf(CONSOLE, "Applet Version   : 0x%04X\r\n", pCommState->appletVersion);
        sm_printf(CONSOLE, "SecureBox Version: 0x%04X\r\n", pCommState->sbVersion);

        sm_printf(DBGOUT, "\r\n");
        sm_printf(DBGOUT, "==========SELECT-DONE=========\r\n");
    }
    else
    {
        sm_printf(DBGOUT, "\r\n");
        sm_printf(DBGOUT, "==========SELECT-FAILED=========\r\n");
    }

#ifdef TARGET_PLATFORM
    sm_printf(CONSOLE, "Platform: %s\r\n",TARGET_PLATFORM);
#endif

#ifdef FREEDOM
    if ( retVal != 0 ) {
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

static int translateCommunicationStatus(
    U16 connectStatus)
{
    if ((connectStatus == ERR_CONNECT_LINK_FAILED) || (connectStatus == ERR_CONNECT_SELECT_FAILED))
    {
        sm_printf(CONSOLE, "SM_Connect failed with status 0x%04X\n", connectStatus);
        return 2;
    }
    else if (connectStatus == SMCOM_COM_FAILED)
    {
        sm_printf(CONSOLE, "SM_Connect failed with status 0x%04X (Could not open communication channel)\n",
            connectStatus);
        return 4;
    }
    else if (connectStatus == SMCOM_PROTOCOL_FAILED)
    {
        sm_printf(CONSOLE, "SM_Connect failed with status 0x%04X (Could not establish communication protocol)\n",
            connectStatus);
        return 5;
    }
    else if (connectStatus == ERR_NO_VALID_IP_PORT_PATTERN)
    {
        sm_printf(DBGOUT, "Pass the IP address and port number as arguments, e.g. \"127.0.0.1:8050\"!\n");
        return 3;
    }
    else if (connectStatus != SW_OK)
    {
        // return error code asis to handle AX layer errors
        return connectStatus;
    }
    else
    {
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
static U16 establishConnnection(
    SmCommState_t* pCommState, const char * pConnectionParam)
{
    U16 connectStatus = 0;

    U8 Atr[64];
    U16 AtrLen = sizeof(Atr);

#if defined(TDA8029_UART) || defined(I2C) || defined(PCSC) || defined(SPI) || defined(IPC)
    connectStatus = SM_Connect(pCommState, Atr, &AtrLen);
#elif defined(RJCT_SOCKET)
    if (pConnectionParam == NULL)
    {
        sm_printf(DBGOUT, "Pass the IP address and port number as arguments, e.g. \"127.0.0.1:8050\"!\n");
        return 4;
    }
    else
    {
        connectStatus = SM_RjctConnect(pConnectionParam, pCommState, Atr, &AtrLen);
    }
#elif defined(RJCT_VCOM)
    if ( pConnectionParam == NULL )
    {
        sm_printf(DBGOUT, "Pass the COM Port as arguments, e.g. \"COM3\" or \"\\\\.\\COM18\" !\n");
        return 4;
    }
    else
    {
        connectStatus = SM_RjctConnect(pConnectionParam,pCommState, Atr, &AtrLen);
    }
#else
#error "No communication channel defined"
#endif // RJCT_SOCKET

    if (AtrLen > 0)
    {
        int i = 0;
        sm_printf(CONSOLE, "ATR=0x");
        for (i = 0; i < AtrLen; i++)
        {
            sm_printf(CONSOLE, "%02X.", Atr[i]);
        }
        sm_printf(CONSOLE, "\n");
    }

#if defined(TDA8029_UART)
    sm_printf(CONSOLE, "UART Baudrate Idx: 0x%02X\n", pCommState->param2);
    sm_printf(CONSOLE, "T=1           TA1: 0x%02X\n", pCommState->param1);
#endif

    return connectStatus;
}

void app_test_status(U8 result) {
#if FREEDOM
	LED_BLUE_OFF();

	if (result == 0) {
		LED_RED_ON();
	}
	else
	{
		LED_GREEN_ON();
	}
#endif
	sm_printf(CONSOLE, "END. Status %s (Compiled on %s %s)\r\n",
			((result == 1) ? "OK" : "FAILED"),
			__DATE__, __TIME__);
}
