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
* NFC Type 4A Card Emulation application.
* $Author: Purnank G (ing05193) $
* $Revision: 6017 $ (v06.01.00)
* $Date: 2016-09-19 19:12:31 +0530 (Mon, 19 Sep 2016) $
*/

/**
* Reader Library Headers
*/
#include <phApp_Init.h>

/* Local headers */
#include "NfcrdlibEx8_HCE_T4T.h"

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
#include "phhalRng.h"
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

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

const phOsal_TimerPeriodObj_t abs_timeout= {OS_TIMER_UNIT_MSEC, PHOSAL_MAX_DELAY};

/* Check if all required reader library layers are enabled. */
#if NXPBUILD_EX8_MANDATORY_LAYERS

/*******************************************************************************
**   Global Defines
*******************************************************************************/
phacDiscLoop_Sw_DataParams_t       * pDiscLoop;       /* Pointer to Discovery loop data-params */
phpalI14443p4mC_Sw_DataParams_t    * ppalI14443p4mC;  /* Pointer to PAL 14443-4mC data-params */
phceT4T_Sw_DataParams_t            * pceT4T;          /* Pointer to HCE data-params */

/**
 * Parameters for L3 activation during AUTOCOLL (used in
 * phhalHw_Rc523_SetListenParameters)
 * */
uint8_t  sens_res[2]     = {0x04, 0x00};        /* ATQ bytes - needed for anti-collision */
uint8_t  nfc_id1[3]      = {0xA1, 0xA2, 0xA3};  /* UID bytes (one byte defined by HW) */
uint8_t  sel_res         = 0x20;                /* SAK (ISO14443P4 Card) */
uint8_t  nfc_id3         = 0xFA;                /* NFCID3 byte */
uint8_t  poll_res[18]    = {0x01, 0xFE, 0xB2, 0xB3, 0xB4, 0xB5,
                            0xB6, 0xB7, 0xC0, 0xC1, 0xC2, 0xC3,
                            0xC4, 0xC5, 0xC6, 0xC7, 0x23, 0x45};

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aRdLibTaskBuffer[RDLIB_TASK_STACK];
uint32_t aHceAppTaskBuffer[APPLICATION_TASK_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define aRdLibTaskBuffer        NULL
#define aHceAppTaskBuffer       NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

/**
 * Parameters for PAL 14443p4mC ATS configuration (Activation).
 * Refer phpalI14443p4mC_Activate for more info.
 * Used in listen mode / card emulation.
 * */
uint8_t aAtsRes[5]  = {0x05, 0x75, 0x00, 0x88, 0x00}; /* ATS Response send for
                                                       * RATS: |TL|T0|TA|TB|TC|
                                                       */
uint16_t wAtsLength = 5;                              /* ATS length */

/**
 * Parameters for T4T CE (card emulation layer)
 * */
/* Mandatory Capability Container (CC) file. It's set using
 * phceT4T_SetElementaryFile. */
uint8_t aCcFile[23] = {0};

/* Mandatory NDEF file. It's set using phceT4T_SetElementaryFile. */
uint8_t aNdefFile[1024] = {0x00, 0x10, 0xD1, 0x01, 0x0C, 0x55, 0x00, 0x77, 0x77,
                          0x77, 0x2E, 0x6E, 0x78, 0x70, 0x2E, 0x63, 0x6F, 0x6D};

#ifdef NXPBUILD__PHCE_T4T_PROPRIETARY
/* Optional proprietary file (not used in this example. Added to demonstrate
 * support). It's set using phceT4T_SetElementaryFile. */
uint8_t aProprietaryFile[100] = {0};
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */

/**
 * RTOS objects.
 * Semaphores for synchronization between application thread and reader library
 * thread.
 * */
phOsal_SemObj_t  appstart; /* Used to synchronize start of one
                                             cycle of application processing */
phOsal_SemObj_t  appexit;  /* Used to synchronize end of one
                                             cycle of application processing */
#if ISO_10373_6_PICC_TEST_BENCH
phOsal_SemObj_t timerwtx; /* Used in PICC ISO 10373-6 test bench
                                             to create WTX */
#endif /* ISO_10373_6_PICC_TEST_BENCH */
#endif /* NXPBUILD_EX8_MANDATORY_LAYERS */

/*******************************************************************************
**   Function Definitions
*******************************************************************************/
void pWtoxCallBck(void);

/*******************************************************************************
**   Main Function
*******************************************************************************/
int main(void)
{
    do
    {
#if NXPBUILD_EX8_MANDATORY_LAYERS
        phStatus_t            status = PH_ERR_INTERNAL_ERROR;
        phNfcLib_Status_t     dwStatus;
        phNfcLib_AppContext_t AppContext = {0};

        phOsal_ThreadObj_t RdLib;

        /* Perform Controller specific initialization. */
        phApp_CPU_Init();

        /* Perform OSAL Initialization. */
        (void)phOsal_Init();

#ifdef PH_PLATFORM_HAS_ICFRONTEND
        status = phbalReg_Init(&sBalParams, sizeof(phbalReg_Type_t));
        CHECK_STATUS(status);

        AppContext.pBalDataparams = &sBalParams;
#endif

        AppContext.pWtxCallback = &pWtoxCallBck;
        dwStatus = phNfcLib_SetContext(&AppContext);
        CHECK_NFCLIB_STATUS(dwStatus);

        /* Initialize library */
        dwStatus = phNfcLib_Init();
        CHECK_NFCLIB_STATUS(dwStatus);
        if(dwStatus != PH_NFCLIB_STATUS_SUCCESS) break;

        /* Set the generic pointer */
        pHal = phNfcLib_GetDataParams(PH_COMP_HAL);
        pDiscLoop = (phacDiscLoop_Sw_DataParams_t *) phNfcLib_GetDataParams(PH_COMP_AC_DISCLOOP);
        ppalI14443p4mC = (phpalI14443p4mC_Sw_DataParams_t *) phNfcLib_GetDataParams(PH_COMP_PAL_I14443P4MC);
        pceT4T = (phceT4T_Sw_DataParams_t *) phNfcLib_GetDataParams(PH_COMP_CE_T4T);

        /* Initialize other components that are not initialized by NFCLIB and configure Discovery Loop. */
        status = phApp_Comp_Init(pDiscLoop);
        CHECK_STATUS(status);
        if(status != PH_ERR_SUCCESS) break;

        /* Perform Platform Init */
        status = phApp_Configure_IRQ();
        CHECK_STATUS(status);
        if(status != PH_ERR_SUCCESS) break;

        /* Initialize/Configure components required by this Example */
        phApp_Initialize();
        DEBUG_PRINTF("\nCard Emulation Application...\n");

        RdLib.pTaskName = (uint8_t *) "RdLib";
        RdLib.pStackBuffer = aRdLibTaskBuffer;
        RdLib.priority = RDLIB_TASK_PRIO;
        RdLib.stackSizeInNum = RDLIB_TASK_STACK;
        phOsal_ThreadCreate(&RdLib.ThreadHandle, &RdLib, &TReaderLibrary, NULL);

        phOsal_ThreadObj_t HceApp;

        /* Create application thread (to handle UPDATE_BINARY and proprietary
         * commands) */
        HceApp.pTaskName = (uint8_t *) "HceApp";
        HceApp.pStackBuffer = aHceAppTaskBuffer;
        HceApp.priority = APPLICATION_TASK_PRIO;
        HceApp.stackSizeInNum = APPLICATION_TASK_STACK;
        phOsal_ThreadCreate(&HceApp.ThreadHandle, &HceApp, &TApplication, NULL);

        phOsal_StartScheduler();

        /* Scheduler exit hook */
        ERROR_CRITICAL("Error...Scheduler exited...\n");

#else /* NXPBUILD_EX8_MANDATORY_LAYERS */
        DEBUG_PRINTF("\n\tERROR: Missing required reader library components in build...");
#endif /* NXPBUILD_EX8_MANDATORY_LAYERS */

    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

#if NXPBUILD_EX8_MANDATORY_LAYERS

/**
 * Reader library thread. This is the main thread which starts discovery loop
 * in listen mode and poll mode (one at a time) and manages switching between
 * both listen and poll modes.
 * */
void TReaderLibrary(
                    void *pParams
                    )
{
    phStatus_t  status;

    PRINT_INFO("Reader Library thread started...\n");

    /* Configure target mode HAL parameters */
    status = phApp_HALConfigAutoColl();
    CHECK_STATUS(status);

    /* Be in card emulation mode */
    while(1)
    {
        /* T4T Card Emulation mode */
        T4TCardEmulation();
    }
}

/**
 * Do card emulation by starting discovery loop in listen mode and activating
 * ISO 14443p4mC.
 * This function calls phceT4T_Activate() to do T4T emulation.
 * Refer phceT4T_Activate() in phceT4T.h for more info.
 * */
void T4TCardEmulation(void)
{
    phStatus_t  status = 0;
    phStatus_t  wDiscLoop_status = 0;
#if ISO_10373_6_PICC_TEST_BENCH
#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    uint16_t wCMAState = 0;
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */
#endif /* ISO_10373_6_PICC_TEST_BENCH */

    /* Start discovery loop in listen mode */
    wDiscLoop_status = phacDiscLoop_Run(
        pDiscLoop,
        PHAC_DISCLOOP_ENTRY_POINT_LISTEN);

    if((wDiscLoop_status & PH_ERR_MASK) == PHAC_DISCLOOP_EXTERNAL_RFOFF)
    {
        /* Reset SLEEP_AF state (Respond to WupA ("52")/ReqA ("26")) as Field
         * Off is observed.*/
        status = phhalHw_SetConfig(
            pHal,
            PHHAL_HW_CONFIG_MFHALTED,
            PH_OFF);
        CHECK_STATUS(status);

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
        phhalRng_GenerateRng(&((phhalHw_PN7462AU_DataParams_t*)pDiscLoop->pHalDataParams)->pAutocoll.pNfcId1[0], 3);
#endif /* NXPBUILD__PHHAL_HW_PN7462AU*/
    }

    if((wDiscLoop_status & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVATED_BY_PEER)
    {
        /* Validate RATS and send ATS */
        status = phpalI14443p4mC_Activate(
            ppalI14443p4mC,
            pDiscLoop->sTargetParams.pRxBuffer,
            (uint8_t)pDiscLoop->sTargetParams.wRxBufferLen,
            aAtsRes,
            wAtsLength);

        if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
        {
            if(status == (PH_COMP_PAL_I14443P4MC | PH_ERR_PROTOCOL_ERROR))
            {
                /* Invalid RATS: Set to SLEEP_AF state if needed. */
#if ISO_10373_6_PICC_TEST_BENCH
#ifdef NXPBUILD__PHHAL_HW_PN7462AU /* This is required for PN640 for the G.11 test case, where the Card state has to be put to MUTE for scenario */
                status = phhalHw_GetConfig(pHal, PHHAL_HW_CONFIG_MFHALTED, &wCMAState);
                CHECK_STATUS(status);

                status = phhalHw_SetConfig (pHal, PHHAL_HW_CONFIG_MFHALTED, wCMAState);
                CHECK_STATUS(status);
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */
#endif /* ISO_10373_6_PICC_TEST_BENCH */
            }
            else if(status == (PH_COMP_PAL_I14443P4MC | PH_ERR_INVALID_PARAMETER))
            {
                /* Invalid ATS */
                ERROR_CRITICAL("ATS configured is invalid...\n");
            }
            else
            {
                DEBUG_ERROR_PRINT(status);
            }
        }
        else
        {
            /* Trigger AppProcessCmd in Application thread to start */
            status = phOsal_SemPost(&appstart.SemHandle, E_OS_SEM_OPT_NONE);
            if(status != PH_ERR_SUCCESS)
            {
                ERROR_CRITICAL("ReaderLibThread: Releasing Semaphore failed...\n");
            }

            /* Activates HCE. This handles all the main functionalities of
             * card emulation...receive request from reader...process
             * received request...pass UPDATE BINARY/proprietary commands to
             * application thread if enabled...and send the response back
             * to reader. */
            status = phceT4T_Activate(pceT4T);
            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_DESELECTED)
            {
                /* Set SLEEP_AF state (Respond only to WupA ("52"), not to
                 * ReqA ("26")), so set MFHalted bit. After this setting is
                 * done "phhalHw_Autocoll" should be called immediately from
                 * Application so that HAL will know if there is any Field
                 * reset in "phhalHw_Autocoll" API and reset this setting. */
                status = phhalHw_SetConfig(
                    pHal,
                    PHHAL_HW_CONFIG_MFHALTED,
                    PH_ON);
                CHECK_STATUS(status);
            }

            /* Wait for AppProcessCmd in Application thread to exit */
            status = phOsal_SemPend(&appexit.SemHandle, abs_timeout);
            if(status != PH_ERR_SUCCESS)
            {
                ERROR_CRITICAL("ReaderLibThread: Getting Semaphore failed...\n");
            }
        }

        /* Reset Reader library layers */
        status = phpalI14443p4mC_ResetProtocol(ppalI14443p4mC);
        DEBUG_ERROR_PRINT(status);

        status = phceT4T_Reset(pceT4T);
        DEBUG_ERROR_PRINT(status);
    }

    status = phOsal_EventClear(
        &pceT4T->T4TEventObj.EventHandle,
        E_OS_EVENT_OPT_NONE,
        (E_PH_OSAL_EVT_RXDATA_AVAILABLE | E_PH_OSAL_EVT_TXDATA_AVAILABLE |
        E_PH_OSAL_EVT_ABORT | E_PH_OSAL_EVT_ERROR | E_PH_OSAL_EVT_RTO_TIMEOUT),
        NULL);
    DEBUG_ERROR_PRINT(status);
}

/**
 * Application thread. This calls phceT4T_AppProcessCmd() to handle update
 * binary and proprietary commands.
 * Refer phceT4T_AppProcessCmd in phceT4T.h for more info.
 * APIs which does PAL or HAL exchange should not be called from application
 * thread.
 * */
void TApplication(
                  void *pParams
                  )
{
    phStatus_t  status;

    PRINT_INFO("Application thread started...\n");

    while(1)
    {
        /* Wait for trigger from reader library thread to start */
        status = phOsal_SemPend(&appstart.SemHandle, abs_timeout);
        if(status != PH_ERR_SUCCESS)
        {
            ERROR_CRITICAL("AppThread: Getting Semaphore failed...\n");
        }

        /* Start Application processing of HCE */
#if USE_APP_CALLBACK
        /* Use application callback to handle UPDATE_BINARY and proprietary
         * commands. */
        status = phceT4T_AppProcessCmd(
            pceT4T,
            AppProcessCmdCallback);
#else /* USE_APP_CALLBACK */
        /* Application callback not used. Proprietary commands are not handled
         * (unsupported command is returned as status) and UPDATE BINARY will be
         * handled internally by AppProcessCmd. */
        status = phceT4T_AppProcessCmd(
            pceT4T,
            NULL);
#endif /* USE_APP_CALLBACK */

        /* Trigger reader library thread about the phceT4T_AppProcessCmd exit */
        status = phOsal_SemPost(&appexit.SemHandle, E_OS_SEM_OPT_NONE);
        if(status != PH_ERR_SUCCESS)
        {
            ERROR_CRITICAL("AppThread: Releasing Semaphore failed...\n");
        }
    }
}

#if USE_APP_CALLBACK
/**
 * Application callback to handle UPDATE BINARY and proprietary commands.
 * Refer phceT4T_AppCallback_t in phceT4T.h for more info.
 * */
phStatus_t AppProcessCmdCallback(
                                 uint8_t bState,
                                 uint16_t wRxOption,
                                 uint8_t *pRxData,
                                 uint16_t wRxDataLen,
                                 uint16_t *pStatusWord,
                                 uint8_t **ppTxData,
                                 uint16_t *pTxDataLen
                                 )
{
    phStatus_t status;
    uint16_t wSelectedFileId;
    uint8_t *pSelectedFile;
    uint32_t dwSelectedFileSize;
    uint8_t bWriteAccess;
    uint32_t dwFileOffset;
#if ISO_10373_6_PICC_TEST_BENCH
    phOsal_TimerPeriodObj_t timePeriodToWait;
    /* Command to create WTX while running 10373-6 PICC test bench. */
    uint8_t aWaitForWtx[4] = {0xDE, 0xAD, 0xFE, 0xED};
#endif /* ISO_10373_6_PICC_TEST_BENCH */

    if(bState == PHCE_T4T_STATE_FILE_UPDATE)
    {
        /* Get Selected file info */
        status = phceT4T_GetSelectedFileInfo(
            pceT4T,
            &wSelectedFileId,
            &pSelectedFile,
            &dwSelectedFileSize,
            &bWriteAccess,
            &dwFileOffset);

        if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Update file */
            (void)memcpy(&pSelectedFile[dwFileOffset], pRxData, wRxDataLen);

            /* File update success */
            *pStatusWord = 0x9000;
            *ppTxData = NULL;
            *pTxDataLen = 0;
        }
        /* This condition shall never come */
        else
        {
            DEBUG_PRINTF("Application Callback: Getting file Info failed...\n");
            /* Could not update file */
            *pStatusWord = 0x6A82;
            *ppTxData = NULL;
            *pTxDataLen = 0;

            /* Return same error back */
            return (status & PH_ERR_MASK);
        }
    }
    else
    {
        /* ---Do the proprietary command handling here--- */

#if ISO_10373_6_PICC_TEST_BENCH
        /* Create WTX while running 10373-6 PICC test bench. Waits for WTX
         * timer expire, if received "DEADFEED..." */
        if(memcmp(&pRxData[1], aWaitForWtx, 4) == 0)
        {
            /* Wait for trigger from WTX callback */
            timePeriodToWait.unitPeriod = OS_TIMER_UNIT_MSEC;
            timePeriodToWait.period = 0xFFFFFFFF;
            status = phOsal_SemPend(&timerwtx.SemHandle, timePeriodToWait);
            if(status != PH_ERR_SUCCESS)
            {
                DEBUG_PRINTF("Application Callback: Getting WTX Semaphore failed...\n");
                while(1);
            }
        }
#endif /* ISO_10373_6_PICC_TEST_BENCH */

        /* No proprietary support */
        *pStatusWord = 0x6D00;
        *ppTxData = NULL;
        *pTxDataLen = 0;
    }

    return PH_ERR_SUCCESS;
}
#endif /* USE_APP_CALLBACK */

/*
 * Initialize/Configure components required by this Example.
 */
void phApp_Initialize(void)
{
    phStatus_t status;
    int ret;

    /* Configure Card Emulation parameters */
    status = ConfigureCardEmulation();
    if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        ERROR_CRITICAL("Configuring card emulation failed...\n");
    }

    /* Create semaphore for sync. between threads */
    ret = phOsal_SemCreate(&appexit.SemHandle, &appexit, E_OS_SEM_OPT_SIGNALLING_SEM);
    if (ret != 0)
    {
        ERROR_CRITICAL("Semaphore creation failed...\n");
    }

    /* Create semaphore for sync. between threads */
    ret = phOsal_SemCreate(&appstart.SemHandle, &appstart, E_OS_SEM_OPT_SIGNALLING_SEM);
         if (ret != 0)
    {
        ERROR_CRITICAL("Semaphore creation failed...\n");
    }

#if ISO_10373_6_PICC_TEST_BENCH
    /* Create semaphore for testing WTX */
    ret = phOsal_SemCreate(&timerwtx.SemHandle, &timerwtx, E_OS_SEM_OPT_SIGNALLING_SEM);
    if(timerwtx.SemHandle == NULL)
    {
        DEBUG_PRINTF("Semaphore creation failed...\n");
        while(1);
    }
#endif /* ISO_10373_6_PICC_TEST_BENCH */
}

/**
 * Configure 14443p4mC and ceT4T parameters.
 * */
phStatus_t ConfigureCardEmulation(void)
{
    phStatus_t status = PH_ERR_SUCCESS;

    /* Set CC File */
    status = phceT4T_SetElementaryFile(
        pceT4T,
        PHCE_T4T_FILE_CC,
        aCcFile,
        0xE103,
        sizeof(aCcFile),
        GET_CCFILE_CONTENT_LEN(aCcFile));
    CHECK_SUCCESS(status);

    /* Set NDEF File */
    status = phceT4T_SetElementaryFile(
        pceT4T,
        PHCE_T4T_FILE_NDEF,
        aNdefFile,
        0xE104,
        sizeof(aNdefFile),
        GET_FILE_CONTENT_LEN(aNdefFile));
    CHECK_SUCCESS(status);

#ifdef NXPBUILD__PHCE_T4T_PROPRIETARY
    /* Set Proprietary File */
    status = phceT4T_SetElementaryFile(
        pceT4T,
        PHCE_T4T_FILE_PROPRIETARY,
        aProprietaryFile,
        0xE105,
        sizeof(aProprietaryFile),
        GET_FILE_CONTENT_LEN(aProprietaryFile));
    CHECK_SUCCESS(status);
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */

#if USE_APP_CALLBACK
    /* Enable support for proprietary command handling by application
     * callback. */
    status = phceT4T_SetConfig(
        pceT4T,
        PHCE_T4T_CONFIG_RECEIVE_PROPRIETARY,
        PH_ON);
    CHECK_SUCCESS(status);
#endif /* USE_APP_CALLBACK */

    /* Configure MLe (This is optional; default MLe is 255) */
    status = phceT4T_SetConfig(
        pceT4T,
        PHCE_T4T_CONFIG_MLE,
        0x3B);
    CHECK_SUCCESS(status);

    /* Configure MLc (This is optional; default MLc is 255) */
    status = phceT4T_SetConfig(
        pceT4T,
        PHCE_T4T_CONFIG_MLC,
        0x34);
    CHECK_SUCCESS(status);

    /* Configure WTX time (WTX will be triggered after this much % of FWT) */
    status = phpalI14443p4mC_SetConfig(
        ppalI14443p4mC,
        PHPAL_I14443P4MC_CONFIG_WT_PERCENTAGE,
        40);
    CHECK_SUCCESS(status);

    return PH_ERR_SUCCESS;
}

/*
 * WTX Callback called from WTX timer of 14443p3mC PAL.
 */
void pWtoxCallBck()
{
    phOsal_EventPost(&pceT4T->T4TEventObj.EventHandle, E_OS_EVENT_OPT_POST_ISR, E_PH_OSAL_EVT_RTO_TIMEOUT, NULL);

#if ISO_10373_6_PICC_TEST_BENCH
    /* Trigger Application callback (AppProcessCmdCallback) about WTX for ISO
     * 10373-6 test bench.*/
    phOsal_SemPost(&timerwtx.SemHandle, E_OS_SEM_OPT_POST_FROM_ISR);
#endif /* ISO_10373_6_PICC_TEST_BENCH */
}

#endif /* NXPBUILD_EX8_MANDATORY_LAYERS */

/*******************************************************************************
**                            End Of File
*******************************************************************************/
