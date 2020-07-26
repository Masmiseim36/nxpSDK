/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Example Source for NfcrdlibEx3_NFCForum.c that uses the Discovery loop for polling and performs Tag Operations and SNEP PUT operation.
* Discovery loop is configured for both POLL and LISTEN modes. This example displays detected tag information(like UID, SAK, Product Type)
* and prints information when it detects NFC Forum Tags and gets activated as a target by an external initiator/reader.
* Whenever multiple technologies are detected, example will select last detected technology to resolve.
* Example will activate device at last index whenever multiple devices are detected.
*
* If a P2P device is detected then this example performs SNEP PUT operation if both SNEP client and server are enabled.
* By default SNEP PUT of URI message (www.nxp.com) will be performed.
*
* If remote P2P device wants to perform SNEP PUT that should be received by this example then SNEP client should be disabled, in this case
* phone acts as client and on screen (Touch to Beam) pops up, when user taps the screen then SNEP PUT will be done by the phone and this
* application will receive this SNEP PUT message and displays on console only in Debug mode (Maximum SNEP PUT
* message that can be received is 1024Bytes as per default SNEP server specification).
*
* Please refer Readme.txt file for hardware pin configuration, software configuration and steps to build and
* execute the project. This file is present in the same project directory.
* $Author: Ashish Pal (nxp79566) $
* $Revision: 6185 $ (v06.01.00)
* $Date: 2016-09-27 12:07:34 +0530 (Tue, 27 Sep 2016) $
*
*/

/**
* Reader Library Headers
*/
#include <phApp_Init.h>

/* Local headers */
#include <cards.h>
#include "NfcrdlibEx3_NFCForum.h"

#ifdef NXPBUILD__PHHAL_HW_RC663
#   warning "This example is not supported for RC663"
int main() {
    DEBUG_PRINTF("This example is not supported for RC663.");
    while (1){

    }
    return 0;
}
/* Actual main defined below has no purpose in this example and hence over-riding it. */
#   define main dummy_main
#endif

/*******************************************************************************
**   Global Defines
*******************************************************************************/
phacDiscLoop_Sw_DataParams_t       * pDiscLoop;       /* Discovery loop component */
void * ppalI18092mPI;
void * ppalI18092mT;
void * palTop;

#ifdef SNEP_SERVER
phnpSnep_Sw_DataParams_t    snpSnepServer;         /* SNEP component holder */

phlnLlcp_Transport_Socket_t ServerSocket;          /* SNEP Server socket and buffers. */
uint8_t                     bServerRxBuffer[260];  /* SNEP Server socket application buffer to copy data from HAL Rx Buff. */
uint8_t                     baSnepAppBuffer[1033]; /* SNEP Server application buffer to store received PUT Message */
#endif /* SNEP_SERVER */

#ifdef SNEP_CLIENT
phnpSnep_Sw_DataParams_t    snpSnepClient;         /* SNEP component holder */

phlnLlcp_Transport_Socket_t ClientSocket;          /* SNEP Client socket */
uint8_t                     bClientRxBuffer[260];  /* SNEP Client socket application buffer to copy data from HAL Rx Buff. */
uint32_t                    dwClientRxBuffLength = sizeof(bClientRxBuffer);
#endif /* SNEP_CLIENT */

#if ! defined(SNEP_SERVER)
uint8_t                     baSnepAppBuffer[500];            /* Buffer used to store NDEF data (poll mode) */
#endif /* SNEP_SERVER */

uint32_t                    baSnepRxLen = 0;

uint8_t   bDid = 0;    /* DID is not used by default. */
uint8_t   bBst = 0x00;
uint8_t   bBrt = 0x00;
uint8_t   bTo = 8;     /* Set TO (time out) value in ATR_RES which will be used to set FDT/FWT on P2P initiator. */
uint8_t   bLrt = 3;    /* Set LRT value to 3. AS LLCP mandates that LRT value to be 3. */

uint16_t  wAtrResLength;

/*The below variables are been initialized according to target mode requirements by this application */
uint8_t  sens_res[2]     = {0x04, 0x00};                          /* ATQ bytes - needed for anti-collision */
uint8_t  nfc_id1[3]      = {0xA1, 0xA2, 0xA3};                    /* user defined bytes of the UID (one is hardcoded) - needed for anti-collision */
uint8_t  sel_res         = 0x40;                                  /* SAK (ISO18092mT) - needed for anti-collision */
uint8_t  nfc_id3         = 0xFA;                                  /* NFC3 byte - required for anti-collision */
uint8_t  poll_res[18]    = {0x01, 0xFE, 0xB2, 0xB3, 0xB4, 0xB5,
                            0xB6, 0xB7, 0xC0, 0xC1, 0xC2, 0xC3,
                            0xC4, 0xC5, 0xC6, 0xC7, 0x23, 0x45 }; /* felica - needed for anti-collision */

/*
 * Internal variables used in this application.
 */
uint8_t bSNEPServer;
uint8_t bSNEPClient;

/*******************************************************************************
**   Extern Defines
*******************************************************************************/

/* Defined in phApp_Init.c */
extern uint8_t   aLLCPGeneralBytes[];
extern uint8_t   bLLCPGBLength;

/* ATR response holder */
extern uint8_t  aResponseHolder[64];

/*******************************************************************************
**   Const Defines
*******************************************************************************/

/* Timeout specification structure */
const phOsal_TimerPeriodObj_t abs_timeout= {OS_TIMER_UNIT_MSEC, 1000};

/*******************************************************************************
**   Static Defines
*******************************************************************************/
/* This is used to save and restore poll configuration if in case application
 * has updated/changed polling configuration to resolve a particular technology
 * when multiple technologies were detected
*/
static uint16_t bSavePollTechCfg  = 0;

static uint32_t dwDataLen = sizeof(baSnepAppBuf);

/* Semaphore used in this example application to synchronize tasks to print the received SNEP message by SNEP server. */
#ifdef SNEP_SERVER
phOsal_SemObj_t xTaskSema;
#endif /* SNEP_SERVER */

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t RdLibTaskBuffer[RDLIB_TASK_STACK];
uint32_t SnepServerTaskBuffer[SNEP_SERVER_TASK_STACK];
uint32_t SnepClientTaskBuffer[SNEP_CLIENT_TASK_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define RdLibTaskBuffer         NULL
#define SnepServerTaskBuffer    NULL
#define SnepClientTaskBuffer    NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */


/*******************************************************************************
**   Function Declarations
*******************************************************************************/
static void GetTagInfo(phacDiscLoop_Sw_DataParams_t  *pDataParams, uint16_t wNumberOfTags, uint16_t wTagsDetected);
static phStatus_t LoadDiscoveryConfiguration();

/*******************************************************************************
**   Extern function for RTOX
**   This example will not need RTOX and the function implementation is empty.
*******************************************************************************/
void pRtoxCallBck(void)
{
}

/*******************************************************************************
**   Main Function
*******************************************************************************/
int main (void)
{
    do
    {
        phStatus_t status = PH_ERR_INTERNAL_ERROR;
        phOsal_ThreadObj_t RdLib;
        phNfcLib_Status_t     dwStatus;
        phNfcLib_AppContext_t AppContext = {0};

        /* Perform Controller specific initialization. */
        phApp_CPU_Init();

        /* Perform OSAL Initialization. */
        (void)phOsal_Init();

        DEBUG_PRINTF("\n NFC Forum Example: \n");

#ifdef PH_PLATFORM_HAS_ICFRONTEND
        status = phbalReg_Init(&sBalParams, sizeof(phbalReg_Type_t));
        CHECK_STATUS(status);

        AppContext.pBalDataparams = &sBalParams;
#endif

        AppContext.pRtoxCallback = pRtoxCallBck;
        dwStatus = phNfcLib_SetContext(&AppContext);
        CHECK_NFCLIB_STATUS(dwStatus);

        /* Initialize library */
        dwStatus = phNfcLib_Init();
        CHECK_NFCLIB_STATUS(dwStatus);
        if(dwStatus != PH_NFCLIB_STATUS_SUCCESS) break;

        /* Set the generic pointer */
        pHal = phNfcLib_GetDataParams(PH_COMP_HAL);
        ppalI18092mPI = phNfcLib_GetDataParams(PH_COMP_PAL_I18092MPI);
        ppalI18092mT = phNfcLib_GetDataParams(PH_COMP_PAL_I18092MT);
        palTop = phNfcLib_GetDataParams(PH_COMP_AL_TOP);

        pDiscLoop = phNfcLib_GetDataParams(PH_COMP_AC_DISCLOOP);

        /* Initialize other components that are not initialized by NFCLIB and configure Discovery Loop. */
        status = phApp_Comp_Init(pDiscLoop);
        CHECK_STATUS(status);
        if(status != PH_ERR_SUCCESS) break;

        /* Perform Platform Init */
        status = phApp_Configure_IRQ();
        CHECK_STATUS(status);
        if(status != PH_ERR_SUCCESS) break;

        RdLib.pTaskName = (uint8_t *) "RdLib";
        RdLib.pStackBuffer = RdLibTaskBuffer;
        RdLib.priority = RDLIB_TASK_PRIO;
        RdLib.stackSizeInNum = RDLIB_TASK_STACK;
        phOsal_ThreadCreate(&RdLib.ThreadHandle, &RdLib, &TReaderLibrary, pDiscLoop);


#ifdef SNEP_SERVER

        phOsal_ThreadObj_t SnepServer;

        SnepServer.pTaskName = (uint8_t *) "SnepServer";
        SnepServer.pStackBuffer = SnepServerTaskBuffer;
        SnepServer.priority = SNEP_SERVER_TASK_PRIO;
        SnepServer.stackSizeInNum = SNEP_SERVER_TASK_STACK;
        phOsal_ThreadCreate(&SnepServer.ThreadHandle, &SnepServer, &TSNEPServer, &slnLlcp);

#endif /* SNEP_SERVER */

#ifdef SNEP_CLIENT

        phOsal_ThreadObj_t SnepClient;

        SnepClient.pTaskName = (uint8_t *) "SnepClient";
        SnepClient.pStackBuffer = SnepClientTaskBuffer;
        SnepClient.priority = SNEP_CLIENT_TASK_PRIO;
        SnepClient.stackSizeInNum = SNEP_CLIENT_TASK_STACK;
        phOsal_ThreadCreate(&SnepClient.ThreadHandle, &SnepClient, &TSNEPClient, &slnLlcp);

#endif /* SNEP_CLIENT */

        phOsal_StartScheduler();

        DEBUG_PRINTF("RTOS error : Scheduler exited. \n");
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

/**
* This Task demonstrates the usage of discovery loop.
* It detects and reports the NFC technology type and performs read NDEF in case any NFC forum tags are detected.
* If a P2P device is detected then LLCP activate will be called to perform Client/Server operation from another Task.
* \param   pDataParams      The discovery loop data parameters
* \note    This Task will never return and Discovery loop run will be called in a infinite while loop.
*/
void TReaderLibrary(void  *pDataParams)
{
    phStatus_t    status;
    phStatus_t    statustmp;
    uint16_t      wTagsDetected = 0;
    uint16_t      wNumberOfTags = 0;
    uint16_t      wEntryPoint;
    uint16_t      wValue;
#ifdef NXPBUILD__PHAC_DISCLOOP_TARGET
    uint16_t      wGeneralBytesLength;
    uint8_t     * pGeneralBytes;
#endif
    uint16_t      wGtLength;
    uint8_t       bIndex;
    uint8_t       bGBLen = 0;
#ifdef SNEP_SERVER
    uint16_t      i = 0;
#endif

    /* Configure HAL target parameters */
    status = phApp_HALConfigAutoColl();
    CHECK_STATUS(status);

    /* Configure discover loop based on the enabled macros i.e. ACTIVE_MODE, PASSIVE_MODE, INITIATOR_MODE, TARGET_MODE. */
    LoadDiscoveryConfiguration();

    /* Set max retry count of 1 in PAL 18092 Initiator to allow only one MAC recovery cycle. */
    status = phpalI18092mPI_SetConfig(ppalI18092mPI, PHPAL_I18092MPI_CONFIG_MAXRETRYCOUNT, 0x01);
    CHECK_STATUS(status);

#if defined INITIATOR_MODE
    /* Start in poll mode */
    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
    status = PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED;
#elif defined TARGET_MODE
    /* Start in listen mode */
    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;
#endif

    while(1)
    {
#ifdef PH_EXAMPLE3_LPCD_ENABLE
        /* Configure LPCD */
        if((status & PH_ERR_MASK) == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
        {
            status = phApp_ConfigureLPCD();
            CHECK_STATUS(status);
        }

        /* Bool to enable LPCD feature. */
        status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, PH_ON);
        CHECK_STATUS(status);
#endif /* PH_EXAMPLE3_LPCD_ENABLE */
        /* Reset local variables used. */
        bGBLen = 0;

        /* Set Discovery poll state to detection */
        status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_DETECTION);
        CHECK_STATUS(status);

        /* Switch off RF field */
        status = phhalHw_FieldOff(pHal);
        CHECK_STATUS(status);

        /* Start discovery loop */
        status = phacDiscLoop_Run(pDataParams, wEntryPoint);
        if(wEntryPoint == PHAC_DISCLOOP_ENTRY_POINT_POLL)
        {
            if((status & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_TECH_DETECTED)
            {
                DEBUG_PRINTF (" \n Multiple technology detected: \n");

                status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                CHECK_STATUS(status);

                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
                {
                    DEBUG_PRINTF (" \tType A detected... \n");
                }
                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
                {
                    DEBUG_PRINTF (" \tType B detected... \n");
                }
                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F212))
                {
                    DEBUG_PRINTF (" \tType F detected with baud rate 212... \n");
                }
                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F424))
                {
                    DEBUG_PRINTF (" \tType F detected with baud rate 424... \n");
                }
                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_V))
                {
                    DEBUG_PRINTF(" \tType V / ISO 15693 / T5T detected... \n");
                }

                /* Store user configured poll configuration. */
                status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, &bSavePollTechCfg);
                CHECK_STATUS(status);

                /* Select last detected technology to resolve. */
                for(bIndex = 0; bIndex < PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED; bIndex++)
                {
                    if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, (1 << bIndex)))
                    {
                        /* Configure for one of the detected technology. */
                        status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, (1 << bIndex));
                        CHECK_STATUS(status);
                    }
                }

                DEBUG_PRINTF ("\tResolving selected technology \n");

                /* Set Discovery loop poll state to collision resolution. */
                status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_COLLISION_RESOLUTION);
                CHECK_STATUS(status);

                /* Start Discovery loop in pool mode from collision resolution phase */
                status = phacDiscLoop_Run(pDataParams, wEntryPoint);

                if((status & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED)
                {
                    /* Get detected technology type */
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                    CHECK_STATUS(status);

                    /* Get number of tags detected */
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
                    CHECK_STATUS(status);

                    DEBUG_PRINTF (" \n Multiple cards resolved: %d cards \n",wNumberOfTags);
                    GetTagInfo(pDataParams, wNumberOfTags, wTagsDetected);

                    if(wNumberOfTags > 1)
                    {
                        /* Get 1st Detected Tag and Activate device at index 0*/
                        for(bIndex = 0; bIndex < PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED; bIndex++)
                        {
                            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, (1 << bIndex)))
                            {
                                DEBUG_PRINTF("\t Activating one card...\n");
                                status = phacDiscLoop_ActivateCard(pDataParams, bIndex, 0);
                                break;
                            }
                        }

                        if(((status & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED) ||
                            ((status & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED))
                        {
                            /* Get Activated Technology Type */
                            status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                            CHECK_STATUS(status);

                            /* Print card details */
                            GetTagInfo(pDataParams, 0x01, wTagsDetected);
                            DEBUG_PRINTF("\tActivation successful\n");
                        }
                        else
                        {
                            DEBUG_PRINTF("\tCard activation failed\n");
                        }
                    }
                }
                else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED)
                {
                    DEBUG_PRINTF (" \tCard detected and activated successfully. \n");

                    /* Get activated technology type */
                    status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                    CHECK_STATUS(status);

                    /*Print card info*/
                    GetTagInfo(pDataParams, 0x01, wTagsDetected);
                }
                else if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED)
                {
                    DEBUG_PRINTF (" \nPassive P2P target detected and activated successfully after collision resolution: \n");

                    /* Assign the PAL 18092 initiator parameters once LLPC should be activated in initiator mode. */
                    slnLlcp.pPalI18092DataParams = ppalI18092mPI;

                    /* Get the ATR_RES length. */
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEF_P2P_ATR_RES_LEN, &wGtLength);
                    CHECK_STATUS(status);

                    bGBLen = (uint8_t)wGtLength;

                    if (bGBLen >= PHLN_LLCP_ATR_RES_MIN_LEN + 3)
                    {
                        /* Activate LLCP with the received ATR_RES in initiator mode. */
                        status = phlnLlcp_Activate(&slnLlcp,
                            &aResponseHolder[PHLN_LLCP_ATR_RES_MIN_LEN],
                            (bGBLen - PHLN_LLCP_ATR_RES_MIN_LEN),
                            PHLN_LLCP_INITIATOR
                            );

                        phOsal_EventClear(&slnLlcp.LlcpEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_LLCP_ACTIVATED, NULL);

                        if (!(((status & PH_ERR_MASK) == PH_ERR_LLCP_DEACTIVATED) ||
                            ((status & PH_ERR_MASK) == PH_ERR_PEER_DISCONNECTED)))
                        {
                            if ((status & PH_ERR_MASK) == PH_ERR_ABORTED)
                            {
                                PRINT_INFO("LLCP exited because of LTO timeout. \n");
                            }
                            else if ((status & PH_ERR_MASK) == PH_ERR_RF_ERROR)
                            {
                                PRINT_INFO("LLCP exited because of RF error. \n");
                            }
                            else
                            {
                                PRINT_INFO("\n LLCP exited unexpectedly:");
                                PrintErrorInfo(status);
                            }
                        }

                        /* Perform LLCP DeInit procedure to release acquired resources. */
                        status = phlnLlcp_DeInit(&slnLlcp);
                        CHECK_STATUS(status);

                        if (bSNEPClient == SNEP_PUT)
                        {
                            DEBUG_PRINTF("\t SNEP Client : Performed SNEP PUT\n\t PUT data length is:%d\n\n", dwDataLen);
                        }

#ifdef SNEP_SERVER
                        /*
                         * Waits on SNEP server to receive SNEP message from a client.
                         * If the server gives this semaphore, then it indicates that
                         * the message has been received and gets printed.
                         */
                        status = phOsal_SemPend(&xTaskSema.SemHandle, abs_timeout);

                        if ((bSNEPServer == SNEP_RECEIVED_PUT) && (status == PH_ERR_SUCCESS))
                        {
                            DEBUG_PRINTF("\t SNEP Server : Received PUT message of length %d bytes\n", baSnepRxLen);

                            for(i = 0; i < baSnepRxLen; i++)
                            {
                                if((baSnepAppBuffer[i] < 0x20) || (baSnepAppBuffer[i] > 0x7e))
                                {
                                    baSnepAppBuffer[i] = '.';
                                }
                            }
                            baSnepAppBuffer[baSnepRxLen] = '\0';
                            DEBUG_PRINTF("\t PUT message : %s\n", baSnepAppBuffer);
                            baSnepRxLen = 0;
                        }
#endif /* SNEP_SERVER */
                        bSNEPClient = 0;
                        bSNEPServer = 0;
                    }
                    else
                    {
                        PRINT_INFO("Received ATR_RES length is wrong. \n");
                    }
                }
                else
                {
                    PRINT_INFO("\tFailed to resolve selected technology.\n");
                }

                /* Re-Store user configured poll configuration. */
                status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, bSavePollTechCfg);
                CHECK_STATUS(status);

                /* Switch to LISTEN mode if supported after POLL mode */
            }/* PHAC_DISCLOOP_MULTI_TECH_DETECTED - End */
            else if (((status & PH_ERR_MASK) == PHAC_DISCLOOP_NO_TECH_DETECTED) ||
                    ((status & PH_ERR_MASK) == PHAC_DISCLOOP_NO_DEVICE_RESOLVED))
            {
                /* Switch to LISTEN mode if supported after POLL mode */
            }
            else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_EXTERNAL_RFON)
            {
                /*
                 * If external RF is detected during POLL, return back so that the application
                 * can restart the loop in LISTEN mode if supported.
                 */
            }
            else if(((status & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED)
                    ||((status & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED)
                    ||((status & PH_ERR_MASK) == PHAC_DISCLOOP_MERGED_SEL_RES_FOUND))

            {
                /* Get detected technology type */
                statustmp = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                CHECK_STATUS(statustmp);

                if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED)
                {
                    if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_ACT_POS_BIT_MASK_106))
                    {
                        DEBUG_PRINTF ("\nActive P2P target detected and activated successfully at 106kbps. \n");

                        /* Get the ATR_RES length. */
                        status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_P2P_ATR_RES_LEN, &wGtLength);
                        CHECK_STATUS(status);
                    }
                    else if((PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_ACT_POS_BIT_MASK_212)) ||
                            (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_ACT_POS_BIT_MASK_424)))
                    {
                        DEBUG_PRINTF ("\nActive P2P target detected and activated successfully at 212/424kbps. \n");

                        /* Get the ATR_RES length. */
                        status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEF_P2P_ATR_RES_LEN, &wGtLength);
                        CHECK_STATUS(status);
                    }
                    else
                    {
                        DEBUG_PRINTF("\nUnknown Active P2P target detected. \n");
                    }

                    /* Save the ATR_RES length. */
                    bGBLen = (uint8_t)wGtLength;
                }
                else if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_MERGED_SEL_RES_FOUND)
                {
                    DEBUG_PRINTF (" \n Merged SAK: Device having T4T and NFC-DEP support detected.\n");

                    /* Send ATR_REQ to activate device in P2P mode. */
                    status = phpalI18092mPI_Atr(ppalI18092mPI,
                        pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                        0x00,
                        0x03,
                        0x00,
                        0x00,
                        aLLCPGeneralBytes,
						pDiscLoop->sTypeATargetInfo.sTypeA_P2P.bGiLength,
                        aResponseHolder,
                        &bGBLen
                        );
                    CHECK_STATUS(status);
                }
                else
                {
                    if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
                    {
                        DEBUG_PRINTF ("\nPassive P2P target detected and activated successfully at 106kbps. \n");

                        /* Get the ATR_RES Length. */
                        status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_P2P_ATR_RES_LEN, &wGtLength);
                        CHECK_STATUS(status);
                    }
                    else if((PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F212)) ||
                            (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F424)))
                    {
                        DEBUG_PRINTF ("\nPassive P2P target detected and activated successfully at 212/424kbps. \n");

                        /* Get the ATR_RES Length. */
                        status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEF_P2P_ATR_RES_LEN, &wGtLength);
                        CHECK_STATUS(status);
                    }
                    else
                    {
                        DEBUG_PRINTF("\nUnknown passive P2P target detected. \n");
                    }

                    /* Save the ATR_RES length. */
                    bGBLen = (uint8_t)wGtLength;
                }

                /* Assign the PAL 18092 Initiator Parameters once LLPC should be activated in Initiator Mode. */
                slnLlcp.pPalI18092DataParams = ppalI18092mPI;

                if (bGBLen >= (PHLN_LLCP_ATR_RES_MIN_LEN + 3))
                {
                    /* Activate LLCP with the received ATR_RES in initiator mode. */
                    status = phlnLlcp_Activate(&slnLlcp,
                        &aResponseHolder[PHLN_LLCP_ATR_RES_MIN_LEN],
                        (bGBLen - PHLN_LLCP_ATR_RES_MIN_LEN),
                        PHLN_LLCP_INITIATOR
                        );

                    phOsal_EventClear(&slnLlcp.LlcpEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_LLCP_ACTIVATED, NULL);

                    if (!(((status & PH_ERR_MASK) == PH_ERR_LLCP_DEACTIVATED) ||
                        ((status & PH_ERR_MASK) == PH_ERR_PEER_DISCONNECTED)))
                    {
                        if ((status & PH_ERR_MASK) == PH_ERR_ABORTED)
                        {
                            PRINT_INFO("LLCP exited because of LTO timeout. \n");
                        }
                        else if ((status & PH_ERR_MASK) == PH_ERR_RF_ERROR)
                        {
                            PRINT_INFO("LLCP exited because of active RF error. \n");
                        }
                        else
                        {
                            PRINT_INFO("\n LLCP exited unexpectedly:");
                            PrintErrorInfo(status);
                        }
                    }

                    /* Perform LLCP DeInit procedure to release acquired resources. */
                    status = phlnLlcp_DeInit(&slnLlcp);
                    CHECK_STATUS(status);

                    if (bSNEPClient == SNEP_PUT)
                    {
                        DEBUG_PRINTF("\t SNEP Client : Performed SNEP PUT\n\t PUT data length is:%d\n\n", dwDataLen);
                    }

#ifdef SNEP_SERVER
                    /*
                     * Waits on SNEP server to receive SNEP message from a client.
                     * If the server gives this semaphore, then it indicates that
                     * the message has been received and gets printed.
                     */
                    status = phOsal_SemPend(&xTaskSema.SemHandle, abs_timeout);
                    if ((bSNEPServer == SNEP_RECEIVED_PUT) && (status == PH_ERR_SUCCESS))
                    {
                        DEBUG_PRINTF("\t SNEP Server : Received PUT message of length %d bytes\n", baSnepRxLen);

                        for(i = 0; i < baSnepRxLen; i++)
                        {
                            if((baSnepAppBuffer[i] < 0x20) || (baSnepAppBuffer[i] > 0x7e))
                            {
                                baSnepAppBuffer[i] = '.';
                            }
                        }
                        baSnepAppBuffer[baSnepRxLen] = '\0';
                        DEBUG_PRINTF("\t PUT Message : %s\n", baSnepAppBuffer);
                        baSnepRxLen = 0;
                    }
#endif /* SNEP_SERVER */

                    bSNEPClient = 0;
                    bSNEPServer = 0;
                }
                else
                {
                    PRINT_INFO("Received ATR_RES doesn't contain LLCP magic bytes. Raw P2P is possible. \n");
                }
                /* Switch to LISTEN mode if supported after POLL mode */
            }
            else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED)
            {
                /*
                 * Multiple cards resolved. It enters here if DEVICE LIMIT > 1 and more than one devices are
                 * detected and resolved.
                 */
                DEBUG_PRINTF (" \n Multiple cards resolved: \n");

                /* Get detected technology type */
                status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                CHECK_STATUS(status);

                /* Get number of tags detected */
                status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
                CHECK_STATUS(status);

                DEBUG_PRINTF ("\tNumber of tags: %d \n",wNumberOfTags);
                GetTagInfo(pDataParams, wNumberOfTags, wTagsDetected);

                if(wNumberOfTags > 1)
                {
                    /* Get 1st detected tag and activate device at index 0 */
                    for(bIndex = 0; bIndex < PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED; bIndex++)
                    {
                        if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, (1 << bIndex)))
                        {
                            DEBUG_PRINTF("\t Activating device @ index 0\n");
                            status = phacDiscLoop_ActivateCard(pDataParams, bIndex, 0);
                            break;
                        }
                    }

                    if( ((status & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED) ||
                            ((status & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED))
                    {
                        /* Get detected technology type */
                        status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                        CHECK_STATUS(status);

                        GetTagInfo(pDataParams, 0x01, wTagsDetected);
                        DEBUG_PRINTF("\t\t Activation successful\n");
                    }
                    else
                    {
                        DEBUG_PRINTF("\t\tCard activation failed\n");
                    }
                }

                /* Switch to LISTEN mode if supported after POLL mode */
            }
            else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED)
            {
                DEBUG_PRINTF (" \n Card detected and activated successfully \n");
                status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
                CHECK_STATUS(status);

                /* Get detected technology type */
                status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                CHECK_STATUS(status);

                DEBUG_PRINTF ("\tNumber of tags: %d \n",wNumberOfTags);
                GetTagInfo(pDataParams, wNumberOfTags, wTagsDetected);

                /* Switch to LISTEN mode if supported after POLL mode */
            }
            else
            {
                if((status & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
                {
                    status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                    CHECK_STATUS(status);
                    PRINT_INFO("\n Discovery loop:");
                    DEBUG_ERROR_PRINT(PrintErrorInfo(wValue));
                }
                else
                {
                    PRINT_INFO("\n Discovery loop:");
                    DEBUG_ERROR_PRINT(PrintErrorInfo(wValue));
                }
            }
#ifdef TARGET_MODE
                /* Start in listen mode */
                wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;
#endif /* TARGET_MODE */
        }
        else
        {
            if((status & PH_ERR_MASK) == PHAC_DISCLOOP_EXTERNAL_RFOFF)
            {
                /*
                 * Enters here if in the target/card mode and external RF is not available
                 * Wait for LISTEN timeout till an external RF is detected.
                 * Application may choose to go into standby at this point.
                 */
                status = phhalHw_EventConsume(pHal);
                CHECK_STATUS(status);

                status = phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_RFON_INTERRUPT, PH_ON);
                CHECK_STATUS(status);

                status = phhalHw_EventWait(pHal, LISTEN_PHASE_TIME_MS);
                if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
                {
                    /* Switch to poll mode */
                    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
                }
                else
                {
                    /* Switch to listen mode */
                    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;
                }
            }
            else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVATED_BY_PEER)
            {
                DEBUG_PRINTF ("\nDevice activated in listen mode. \n");
#ifdef NXPBUILD__PHAC_DISCLOOP_TARGET

                /* Assign the PAL 18092 target parameters once LLPC should be activated in listen mode. */
                slnLlcp.pPalI18092DataParams = ppalI18092mT;

                memcpy(aResponseHolder, poll_res,10);

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
                aResponseHolder[8] = 0xC0;
                aResponseHolder[9] = 0xC1;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU*/

                aResponseHolder[10] = bBst;
                aResponseHolder[11] = bBrt;
                aResponseHolder[12] = bTo;
                aResponseHolder[13] = bLrt << 4 | 0x02; /* Frame Size is 254 and ATR_RES contains General Bytes. */
                memcpy(&aResponseHolder[14], aLLCPGeneralBytes, bLLCPGBLength);
                wAtrResLength = 14 + bLLCPGBLength;

                status = phpalI18092mT_Activate(ppalI18092mT,
                    pDiscLoop->sTargetParams.pRxBuffer,
                    pDiscLoop->sTargetParams.wRxBufferLen,
                    aResponseHolder,
                    wAtrResLength,
                    &pGeneralBytes,
                    &wGeneralBytesLength
                    );
                if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                {
                    /* Activate LLCP with the received ATR_REQ in target mode. */
                    status = phlnLlcp_Activate(&slnLlcp,
                        pGeneralBytes,
                        wGeneralBytesLength,
                        PHLN_LLCP_TARGET
                        );

                    phOsal_EventClear(&slnLlcp.LlcpEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_LLCP_ACTIVATED, NULL);

                    if (!(((status & PH_ERR_MASK) == PH_ERR_LLCP_DEACTIVATED) ||
                        ((status & PH_ERR_MASK) == PH_ERR_PEER_DISCONNECTED)))
                    {
                        if ((status & PH_ERR_MASK) == PH_ERR_ABORTED)
                        {
                            PRINT_INFO("LLCP exited because of LTO timeout. \n");
                        }
                        else if ((status & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR)
                        {
                            PRINT_INFO("LLCP exited because of external RF Off. \n");
                        }
                        else if ((status & PH_ERR_MASK) == PH_ERR_RF_ERROR)
                        {
                            PRINT_INFO("LLCP exited because of active RF error. \n");
                        }
                        else
                        {
                            PRINT_INFO("\n LLCP exited unexpectedly:");
                            PrintErrorInfo(status);
                        }
                    }

                    if (bSNEPClient == SNEP_PUT)
                    {
                        DEBUG_PRINTF("\t SNEP Client : Performed SNEP PUT\n\t PUT data length is:%d\n\n", dwDataLen);
                    }

#ifdef SNEP_SERVER
                    /*
                     * Waits on SNEP server to receive SNEP message from a client.
                     * If the server gives this semaphore, then it indicates that
                     * the message has been received and gets printed.
                     */
                    status = phOsal_SemPend(&xTaskSema.SemHandle, abs_timeout);
                    if ((bSNEPServer == SNEP_RECEIVED_PUT) && (status == PH_ERR_SUCCESS))
                    {
                        DEBUG_PRINTF("\t SNEP Server : Received PUT message of length %d bytes\n", baSnepRxLen);

                        for(i = 0; i < baSnepRxLen; i++)
                        {
                            if((baSnepAppBuffer[i] < 0x20) || (baSnepAppBuffer[i] > 0x7e))
                            {
                                baSnepAppBuffer[i] = '.';
                            }
                        }
                        baSnepAppBuffer[baSnepRxLen] = '\0';
                        DEBUG_PRINTF("\t PUT Message : %s\n", baSnepAppBuffer);
                        baSnepRxLen = 0;
                    }
#endif /* SNEP_SERVER */
                    bSNEPClient = 0;
                    bSNEPServer = 0;

                    /* Perform LLCP DeInit procedure to release acquired resources. */
                    status = phlnLlcp_DeInit(&slnLlcp);
                    CHECK_STATUS(status);
                }
                else
                {
                    PRINT_INFO("\n phpalI18092mT_Activate failed :");
                    PrintErrorInfo(status);
                }
#endif /* NXPBUILD__PHAC_DISCLOOP_TARGET */
            }
            else
            {
                if((status & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
                {
                    status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                    CHECK_STATUS(status);
                    PRINT_INFO("\n Discovery loop:");
                    DEBUG_ERROR_PRINT(PrintErrorInfo(wValue));
                }
                else
                {
                    PRINT_INFO("\n Discovery loop:");
                    DEBUG_ERROR_PRINT(PrintErrorInfo(wValue));
                }
            }

            /* On successful activated by Peer, switch to POLL mode */
            wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
        }
        /* Reset state of layers. */
        (void)phalTop_Reset(palTop);
        (void)phpalI14443p4_ResetProtocol(phNfcLib_GetDataParams(PH_COMP_PAL_ISO14443P4));
        (void)phpalI18092mPI_ResetProtocol(ppalI18092mPI);
#ifdef NXPBUILD__PHPAL_I18092MT_SW
		/* This component is disabled for Rc663. But the example have to build for Rc663 in Linux. */
        (void)phpalI18092mT_ResetProtocol(ppalI18092mT);
#endif
    }
}

#ifdef SNEP_SERVER
/**
* Task 2 : SNEP server task
* This task will first block on LLCP activated event from reader library thread, once
* event is obtained this task will register SNEP server socket on LLCP running in reader
* library thread and will wait for remote client to get connected and perform SNEP PUT operation.
* \note : SNEP server task will run in a infinite while loop.
*/
void TSNEPServer(void * pLlcp)
{
    phStatus_t                  status = 0;
    uint32_t                    dwServerRxBuffLength = sizeof(bServerRxBuffer);
    uint32_t                    baSnepAppBufSize = sizeof(baSnepAppBuffer) - 1;
    uint8_t                     bClientReq;
    int                         ret;

    ret = phOsal_SemCreate(&xTaskSema.SemHandle, &xTaskSema, E_OS_SEM_OPT_SIGNALLING_SEM);

    if (ret != 0)
    {
        DEBUG_PRINTF ("Server task application semaphore could not be created.\n");
    }

    do{
        /* Wait until LLCP activation is complete. */
        status = phlnLlcp_WaitForActivation(pLlcp);
        CHECK_STATUS(status);

        baSnepRxLen = 0;
        bSNEPServer = 0;

        /* Initialize SNEP server component. */
        status = phnpSnep_Sw_Init(&snpSnepServer, sizeof(phnpSnep_Sw_DataParams_t), &slnLlcp, &ServerSocket);
        CHECK_STATUS(status);

        /* Perform server initialization to handle remote SNEP client connection. */
        status = phnpSnep_ServerInit(&snpSnepServer, phnpSnep_Default_Server, NULL, bServerRxBuffer, dwServerRxBuffLength);
        if (status == PH_ERR_SUCCESS)
        {
            do
            {
                /* Handle client PUT request. */
                status = phnpSnep_ServerListen(&snpSnepServer, 0, NULL, NULL, &bClientReq);

                if (status == PH_ERR_SUCCESS)
                {
                    status = phnpSnep_ServerSendResponse(&snpSnepServer, bClientReq, NULL, 0, baSnepAppBufSize, baSnepAppBuffer, &baSnepRxLen);
                }
            }while(!status);

            if (!(((status & PH_ERR_MASK) == PH_ERR_PEER_DISCONNECTED) ||
                ((status & PH_ERR_MASK) == PH_ERR_LLCP_DEACTIVATED)) ||
                ((status & PH_ERR_MASK) == PH_ERR_SUCCESS))
            {
                CHECK_STATUS(status);
            }
            else
            {
                if (baSnepRxLen > 0)
                {
                    /* Print only if server received PUT message of length greater than 0 bytes. */
                    bSNEPServer = SNEP_RECEIVED_PUT;
                }
            }
            status = phOsal_SemPost(&xTaskSema.SemHandle, E_OS_SEM_OPT_NONE);
        }
        else
        {
            /* Server initialization is un-successful as peer did not send CONNECT PDU. */
        }
        phOsal_EventClear(&slnLlcp.LlcpEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_LLCP_ACTIVATED, NULL);

        /* Perform server de-init. */
        status = phnpSnep_ServerDeInit(&snpSnepServer);
        CHECK_STATUS(status);
    }while(1);

    phOsal_ThreadDelete(NULL);
}
#endif /* SNEP_SERVER */

#ifdef SNEP_CLIENT
/**
* Task 3 : SNEP client task.
* This task will first block on LLCP activated event from reader library thread, once
* event is obtained this task will register client socket on LLCP running in reader
* library thread and will connect to remote SNEP server and performs SNEP PUT operation.
* \note : SNEP client task will run in a infinite while loop.
*/
void TSNEPClient(void * pLlcp)
{
    phStatus_t status    = 0;

    while (1)
    {
        do{
            /* Wait until LLCP activation is complete. */
            status = phlnLlcp_WaitForActivation(&slnLlcp);
            CHECK_STATUS(status);

            /* Initialize SNEP client component */
            status = phnpSnep_Sw_Init(&snpSnepClient, sizeof(snpSnepClient), &slnLlcp, &ClientSocket);
            CHECK_STATUS(status);

            /* Perform SNEP client Initialization and connect to remote SNEP server. */
            status = phnpSnep_ClientInit(&snpSnepClient, phnpSnep_Default_Server, NULL, bClientRxBuffer, dwClientRxBuffLength);
            if (status == PH_ERR_SUCCESS)
            {
                status = phnpSnep_Put(&snpSnepClient, (uint8_t *)baSnepAppBuf, dwDataLen);
                ClientSocket.fReady = true;

                phOsal_EventClear(&slnLlcp.LlcpEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_LLCP_ACTIVATED, NULL);

                if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                {
                    status = phnpSnep_ClientDeInit(&snpSnepClient);
                    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                    {
                        status = phlnLlcp_Deactivate(&slnLlcp);
                    }
                    bSNEPClient = SNEP_PUT;
                }
                else
                {
                    status = phlnLlcp_Transport_Socket_Unregister(snpSnepClient.plnLlcpDataParams, snpSnepClient.psSocket);
                    CHECK_STATUS(status);
                }
            }
            else
            {
                phOsal_EventClear(&slnLlcp.LlcpEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_LLCP_ACTIVATED, NULL);

                /* Client initialization is un-successful as failed to connect to remote server.
                 * Release RTOS memory by performing socket unregister. */
                status = phlnLlcp_Transport_Socket_Unregister(snpSnepClient.plnLlcpDataParams, snpSnepClient.psSocket);
                CHECK_STATUS(status);
            }
        } while ((status == PH_ERR_PEER_DISCONNECTED) ||
            (status == PH_ERR_LLCP_DEACTIVATED) ||
            (status == PH_ERR_SUCCESS));
    }

    phOsal_ThreadDelete(NULL);
}
#endif /* SNEP_CLIENT */

/**
* This function will load/configure Discovery loop with default values based on interested profiles.
* Application can read these values from EEPROM area and load/configure Discovery loop via SetConfig
* \param   bProfile      Reader Library Profile
* \note    Values used below are default and is for demonstration purpose.
*/
static phStatus_t LoadDiscoveryConfiguration()
{
    phStatus_t status = PH_ERR_SUCCESS;
    uint16_t   wPasPollConfig = 0;
    uint16_t   wActPollConfig = 0;
    uint16_t   wPasLisConfig = 0;
    uint16_t   wActLisConfig = 0;

#ifdef INITIATOR_MODE

#ifdef PASSIVE_MODE
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
    wPasPollConfig |= PHAC_DISCLOOP_POS_BIT_MASK_A;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    wPasPollConfig |= PHAC_DISCLOOP_POS_BIT_MASK_B;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    wPasPollConfig |= (PHAC_DISCLOOP_POS_BIT_MASK_F212 | PHAC_DISCLOOP_POS_BIT_MASK_F424);
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    wPasPollConfig |= PHAC_DISCLOOP_POS_BIT_MASK_V;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    wPasPollConfig |= PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3;
#endif
#endif /* PASSIVE_MODE */

#ifdef ACTIVE_MODE
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE
    wActPollConfig |= PHAC_DISCLOOP_ACT_POS_BIT_MASK_106;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF212_P2P_ACTIVE
    wActPollConfig |= PHAC_DISCLOOP_ACT_POS_BIT_MASK_212;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF424_P2P_ACTIVE
    wActPollConfig |= PHAC_DISCLOOP_ACT_POS_BIT_MASK_424;
#endif
#endif /* ACTIVE_MODE */

#endif /* INITIATOR_MODE */

#ifdef TARGET_MODE

#ifdef PASSIVE_MODE
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TARGET_PASSIVE
    wPasLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_A;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF212_TARGET_PASSIVE
    wPasLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_F212;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF424_TARGET_PASSIVE
    wPasLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_F424;
#endif
#endif /* PASSIVE_MODE */

#ifdef ACTIVE_MODE
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TARGET_ACTIVE
    wActLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_A;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF212_TARGET_ACTIVE
    wActLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_F212;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF424_TARGET_ACTIVE
    wActLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_F424;
#endif
#endif /* ACTIVE_MODE */

#endif /* TARGET_MODE */

    /* Passive poll bitmap configuration. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, wPasPollConfig);
    CHECK_STATUS(status);

    /* Active poll bitmap configuration. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG, wActPollConfig);
    CHECK_STATUS(status);

    /* Passive listen bitmap configuration. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG, wPasLisConfig);
    CHECK_STATUS(status);

    /* Active listen bitmap configuration. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG, wActLisConfig);
    CHECK_STATUS(status);

    /* Passive Bailout bitmap configuration. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_BAIL_OUT, 0x00);
    CHECK_STATUS(status);

    /* Set LRI value for Type-A polling to 3. LLCP mandates that LRI value to be 3. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_P2P_LRI, 3);
    CHECK_STATUS(status);

    /* Set LRI value for Type-F polling to 3. LLCP mandates that LRI value to be 3. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEF_P2P_LRI, 3);
    CHECK_STATUS(status);

    /* Reset collision pending */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_COLLISION_PENDING, PH_OFF);
    CHECK_STATUS(status);

    /* Set Discovery loop mode to NFC mode. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_OPE_MODE, RD_LIB_MODE_NFC);
    CHECK_STATUS(status);

    return status;
}

/**
* This function will retrieve and print tag information like UID, NFC type etc..
* \param   pDataParams      The discovery loop data parameters
* \param   wNumberOfTags    Total number of tags detected
* \param   wTagsDetected    Technology detected
*/
static void GetTagInfo(phacDiscLoop_Sw_DataParams_t  *pDataParams, uint16_t wNumberOfTags, uint16_t wTagsDetected)
{
    uint8_t bIndex;
    uint8_t bTagType;
    uint8_t bCidEnabled = 0;
    uint8_t bCid = 0;
    uint8_t bNadSuported = 0;
    uint8_t bFwi = 0;
    uint8_t bFsdi = 0;
    uint8_t bFsci = 0;
    uint8_t bNad = 0;
    phStatus_t status = 0;

    /* Required if DETECT_ERROR is not set. DETECT_ERROR is required for debugging purpose only */
    PH_UNUSED_VARIABLE(status);

#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE)
    /* Check for Type A tag */
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
    {
        /* Check for T1T */
        if(pDataParams->sTypeATargetInfo.bT1TFlag)
        {
            DEBUG_PRINTF("\tType A : T1T detected \n");

            /* Check for NDEF and Read */
            status = ReadNdefMessage(PHAL_TOP_TAG_TYPE_T1T_TAG);
            if (status != PH_ERR_SUCCESS)
            {
                DEBUG_ERROR_PRINT(status);
            }
        }
        else
        {
            DEBUG_PRINTF("\tTechnology : Type A");
            /* Loop through all the detected tags (if multiple tags are
            * detected) */
            for(bIndex = 0; bIndex < wNumberOfTags; bIndex++)
            {
                DEBUG_PRINTF ("\n\t\tCard : %d",bIndex + 1);
                DEBUG_PRINTF ("\n\t\tUID  :");
                phApp_Print_Buff( pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aUid,
                    pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].bUidSize);
                DEBUG_PRINTF ("\n\t\tSAK: 0x%x",pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak);

                if ((pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak & (uint8_t) ~0xFB) == 0)
                {
                    /* Bit b3 is set to zero, [Digital] 4.8.2 */
                    /* Mask out all other bits except for b7 and b6 */
                    bTagType = (pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak & 0x60);
                    bTagType = bTagType >> 5;

                    /* Switch to tag type */
                    switch(bTagType)
                    {
                    case PHAC_DISCLOOP_TYPEA_TYPE2_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType : Type 2 tag\n");

                        if (!DetectClassic(
                            pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aAtqa,
                            &pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak))
                        {
                            /* Check for NDEF and Read NDEF if mifare Ultralite card is detected. */
                            status = ReadNdefMessage(PHAL_TOP_TAG_TYPE_T2T_TAG);
                            DEBUG_ERROR_PRINT(status);
                        }
                        break;

                    case PHAC_DISCLOOP_TYPEA_TYPE4A_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType : Type 4A tag\n");
                        (void)phpalI14443p4a_GetProtocolParams(pDataParams->pPal1443p4aDataParams,
                            &bCidEnabled,
                            &bCid,
                            &bNadSuported,
                            &bFwi,
                            &bFsdi,
                            &bFsci
                            );

                        (void)phpalI14443p4_SetProtocol(pDataParams->pPal14443p4DataParams,
                            bCidEnabled,
                            bCid,
                            bNadSuported,
                            bNad,
                            bFwi,
                            bFsdi,
                            bFsci
                            );

                        /* Check for NDEF and Read NDEF */
                        status = ReadNdefMessage(PHAL_TOP_TAG_TYPE_T4T_TAG);
                        DEBUG_ERROR_PRINT(status);
                        break;

                    case PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType : P2P\n");
                        break;

                    case PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TYPE4A_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType : Type NFC_DEP and 4A tag\n");
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    /* Check for Type B tag */
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
    {
        DEBUG_PRINTF("\tTechnology: Type B");
        /* Loop through all the Type B tags detected and print the PUPI */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF ("\n\t\tCard : %d",bIndex + 1);
            DEBUG_PRINTF ("\n\t\tUID  :");
            /* PUPI Length is always 4 bytes */
            phApp_Print_Buff( pDataParams->sTypeBTargetInfo.aTypeB_I3P3[bIndex].aPupi, 0x04);
        }
        DEBUG_PRINTF("\n");
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    /* Check for Type F tag */
    if( PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F212) ||
        PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F424))
    {
        DEBUG_PRINTF("\tTechnology: Type F");

        /* Loop through all the type F tags and print the IDm */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF ("\n\t\tCard : %d",bIndex + 1);
            DEBUG_PRINTF ("\n\t\tUID  :");
            phApp_Print_Buff( pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm,
                PHAC_DISCLOOP_FELICA_IDM_LENGTH );
            /* Check data rate  */
            if(pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].bBaud != PHAC_DISCLOOP_CON_BITR_212)
            {
                DEBUG_PRINTF ("\n\t\tBit Rate: 424 kbps");
            }
            else
            {
                DEBUG_PRINTF ("\n\t\tBit Rate: 212 kbps");
            }
            if ((pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm[0] == 0x01) &&
                (pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm[1] == 0xFE))
            {
                /* This is type F tag with P2P capabilities */
                DEBUG_PRINTF ("\n\t\tType : P2P\n");
            }
            else
            {
                /* This is Type F T3T tag */
                DEBUG_PRINTF ("\n\t\tType : Type 3 tag\n");

                /* Check for NDEF and read NDEF */
                status = ReadNdefMessage(PHAL_TOP_TAG_TYPE_T3T_TAG);
                DEBUG_ERROR_PRINT(status);
            }
        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_V))
    {
        DEBUG_PRINTF("\tTechnology  : Type V / ISO 15693 / T5T");
        /* Loop through all the Type V tags detected and print the UIDs */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF ("\n\t\tCard: %d",bIndex + 1);
            DEBUG_PRINTF ("\n\t\tUID :");
            phApp_Print_Buff( pDataParams->sTypeVTargetInfo.aTypeV[bIndex].aUid, 0x08);

            /* Check for NDEF and Read */
            status = ReadNdefMessage(PHAL_TOP_TAG_TYPE_T5T_TAG);
            if (status != PH_ERR_SUCCESS)
            {
                DEBUG_ERROR_PRINT(status);
            }
        }
        DEBUG_PRINTF("\n");
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3))
    {
        DEBUG_PRINTF("\tTechnology  : ISO 18000p3m3 / EPC Gen2");
        /* Loop through all the 18000p3m3 tags detected and print the UII */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF("\n\t\tCard: %d",bIndex + 1);
            DEBUG_PRINTF("\n\t\tUII :");
            phApp_Print_Buff(
                pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bIndex].aUii,
                (pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bIndex].wUiiLength / 8));
        }
        DEBUG_PRINTF("\n");
    }
#endif
}

/**
* Reads NDEF Message if the detected tag has a valid NDEF message
*/
phStatus_t ReadNdefMessage(uint8_t TopTagType)
{
    phStatus_t status;
    uint8_t bTagState;
    uint16_t wDataLength = 0;

    /* Configure Top layer for specified tag type */
    status = phalTop_SetConfig(palTop, PHAL_TOP_CONFIG_TAG_TYPE, TopTagType);
    DEBUG_ERROR_PRINT(status);

    /* Check for NDEF presence */
    status = phalTop_CheckNdef(palTop, &bTagState);
    DEBUG_ERROR_PRINT(status);

    if((bTagState == PHAL_TOP_STATE_READONLY) || (bTagState == PHAL_TOP_STATE_READWRITE))
    {
        /* Read NDEF message */
        status = phalTop_ReadNdef(palTop, baSnepAppBuffer, &wDataLength);
        DEBUG_ERROR_PRINT(status);

        /* Print NDEF message, if not NULL NDEF */
        if(wDataLength)
        {
            DEBUG_PRINTF("\tNDEF detected...\n");
            DEBUG_PRINTF("\tNDEF length: %d\n", wDataLength);
            DEBUG_PRINTF("\tNDEF message:\n");
            //DumpBuffer(aData, wDataLength);
            DumpBuffer(baSnepAppBuffer, 50);
        }
        else
        {
            DEBUG_PRINTF("\tNDEF content is NULL...\n");
        }
    }
    else
    {
        DEBUG_PRINTF("\tNo NDEF content detected...\n");
    }

    return status;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
