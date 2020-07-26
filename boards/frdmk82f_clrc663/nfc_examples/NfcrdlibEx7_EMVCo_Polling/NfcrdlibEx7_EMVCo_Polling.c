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
* Example Source for NfcrdlibEx6_EMVCo_Loopback.
* This application will configure Reader Library as per Emvco specification and start Emvco polling.
* This loop back application will send SELECT_PPSE command and is used to test Emvco2.3.1a(L1)
* digital compliance.
* Please refer Readme.txt file  for  Hardware Pin Configuration, Software Configuration and steps to build and
* execute the project which is present in the same project directory.
* $Author: Purnank G (ing05193) $
* $Revision: 6114 $ (v06.01.00)
* $Date: 2016-09-22 16:56:53 +0530 (Thu, 22 Sep 2016) $
*
* History:
* PN: Generated 14. May 2015
* BK: Generated 12. Jun 2014
* PC: Generated 25. Nov 2012
*
*/

/**
* Reader Library Headers
*/
#include <phApp_Init.h>

/* Local headers */
#include "NfcrdlibEx7_EMVCo_Polling.h"

/*******************************************************************************
**   Global Defines
*******************************************************************************/

phacDiscLoop_Sw_DataParams_t       * pDiscLoop;       /* Discovery loop component */

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aEmvcoPollingTaskBuffer[EMVCO_POLLING_TASK_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define aEmvcoPollingTaskBuffer     NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

/*******************************************************************************
**   Static Defines
*******************************************************************************/

#ifdef RUN_TEST_SUIT
/* EMVCo: Select PPSE Command */
static uint8_t PPSE_SELECT_APDU[] = { 0x00, 0xA4, 0x04, 0x00, 0x0E, 0x32, 0x50, 0x41, 0x59,
        0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 };
#else
uint8_t PPSE_response_buffer[256];
uint8_t PPSE_respsize;
/*Define Macro for the PPSE command with different P2 value in PPSE command*/
#define PPSE_FCI
//#define PPSE_FMD
//#define PPSE_FCP
//#define PPSE_NO_LE


/* CLA = 0x00
 *  INS = 0xA4
 *  P1 = 0x04
 *  P2 = 0x00
 *  LC = 0x0E,
 *  DF = 0x32, 0x50, 0x41, 0x59, 0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31
 *  LE = 0x00
 */
/*P2 parameter values of
 *  000000xxb = Return FCI template, optional use of FCI tag and length ,
 *  000001xxb = Return FCP template, mandatory use of FCP tag and length,
 *  000010xxb = Return FMD template, mandatory use of FMD tag and length,
 *  000011xxb = No response data if Le field absent, or proprietary if Le field present
 * */
#ifdef PPSE_FCI
/* EMVCo: Select PPSE Command */
static uint8_t PPSE_SELECT_APDU[] = { 0x00, 0xA4, 0x04, 0x00, 0x0E, 0x32, 0x50, 0x41, 0x59,
        0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 };
#endif
#ifdef PPSE_FCP
/* EMVCo: Select PPSE Command */
static uint8_t PPSE_SELECT_APDU[] = { 0x00, 0xA4, 0x04, 0x04, 0x0E, 0x32, 0x50, 0x41, 0x59,
        0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 };
#endif
#ifdef  PPSE_FMD
/* EMVCo: Select PPSE Command */
static uint8_t PPSE_SELECT_APDU[] = { 0x00, 0xA4, 0x04, 0x08, 0x0E, 0x32, 0x50, 0x41, 0x59,
        0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 };
#endif
#ifdef PPSE_NO_LE
/* EMVCo: Select PPSE Command */
static uint8_t PPSE_SELECT_APDU[] = { 0x00, 0xA4, 0x04, 0x0C, 0x0E, 0x32, 0x50, 0x41, 0x59,
        0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31 };
#endif
#endif
uint8_t command_buffer[PHAC_EMVCO_MAX_BUFFSIZE];
uint8_t *response_buffer;

/*******************************************************************************
**   Function Declaration
*******************************************************************************/
void Emvco_Polling(void * pHalParams);
static phStatus_t LoadEmvcoSettings();
static phStatus_t EmvcoRfReset(void);
static phStatus_t EmvcoDataLoopBack(phacDiscLoop_Sw_DataParams_t * pDataParams);
static phStatus_t EmvcoDataExchange(uint8_t * com_buffer, uint8_t cmdsize, uint8_t ** resp_buffer, uint32_t * wRxLength);

/*******************************************************************************
**   Function Definitions
*******************************************************************************/

/*******************************************************************************
**   Main Function
*******************************************************************************/

int main (void)
{
    do
    {
        phStatus_t status = PH_ERR_INTERNAL_ERROR;
        phNfcLib_Status_t     dwStatus;
#ifdef PH_PLATFORM_HAS_ICFRONTEND
        phNfcLib_AppContext_t AppContext = {0};
#endif /* PH_PLATFORM_HAS_ICFRONTEND */

#ifndef PH_OSAL_NULLOS
        phOsal_ThreadObj_t EmvcoPolling;
#endif /* PH_OSAL_NULLOS */

        /* Perform Controller specific initialization. */
        phApp_CPU_Init();

        /* Perform OSAL Initialization. */
        (void)phOsal_Init();

        DEBUG_PRINTF("\n Emvco Example: ");

#ifdef PH_PLATFORM_HAS_ICFRONTEND
        status = phbalReg_Init(&sBalParams, sizeof(phbalReg_Type_t));
        CHECK_STATUS(status);

        AppContext.pBalDataparams = &sBalParams;
        dwStatus = phNfcLib_SetContext(&AppContext);
        CHECK_NFCLIB_STATUS(dwStatus);
#endif
        /* Initialize library */
        dwStatus = phNfcLib_Init();
        CHECK_NFCLIB_STATUS(dwStatus);
        if(dwStatus != PH_NFCLIB_STATUS_SUCCESS) break;

        /* Set the generic pointer */
        pHal = phNfcLib_GetDataParams(PH_COMP_HAL);
        pDiscLoop = phNfcLib_GetDataParams(PH_COMP_AC_DISCLOOP);

        /* Initialize other components that are not initialized by NFCLIB and configure Discovery Loop. */
        status = phApp_Comp_Init(pDiscLoop);
        CHECK_STATUS(status);
        if(status != PH_ERR_SUCCESS) break;

        /* Perform Platform Init */
        status = phApp_Configure_IRQ();
        CHECK_STATUS(status);
        if(status != PH_ERR_SUCCESS) break;

#ifndef PH_OSAL_NULLOS

        EmvcoPolling.pTaskName = (uint8_t *) "EmvcoPolling";
        EmvcoPolling.pStackBuffer = aEmvcoPollingTaskBuffer;
        EmvcoPolling.priority = EMVCO_POLLING_TASK_PRIO;
        EmvcoPolling.stackSizeInNum = EMVCO_POLLING_TASK_STACK;
        phOsal_ThreadCreate(&EmvcoPolling.ThreadHandle, &EmvcoPolling, &Emvco_Polling, pHal);

        phOsal_StartScheduler();

        DEBUG_PRINTF("RTOS Error : Scheduler exited. \n");

#else
        Emvco_Polling(pHal);
#endif
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

void Emvco_Polling(void * pHalParams)
{
    phStatus_t  status;
    uint16_t    wTagsDetected = 0;
    uint8_t     bCidEnabled;
    uint8_t     bCid;
    uint8_t     bNadSupported;
    uint8_t     bFwi;
    uint8_t     bFsdi;
    uint8_t     bFsci;

    phacDiscLoop_Sw_DataParams_t * pDataParams;

    pDataParams = phNfcLib_GetDataParams(PH_COMP_AC_DISCLOOP);

    /* Load Emvco Default setting */
    status = LoadEmvcoSettings();
    CHECK_STATUS(status);

    /* Perform RF Reset */
    status = EmvcoRfReset();
    CHECK_STATUS(status);

    status = phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_SET_EMD, PH_OFF);
    CHECK_STATUS(status);

    status = phacDiscLoop_Sw_SetConfig(pDataParams,PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG,
    		PHAC_DISCLOOP_POS_BIT_MASK_A | PHAC_DISCLOOP_POS_BIT_MASK_B);
    CHECK_STATUS(status);

    status = phacDiscLoop_Sw_SetConfig(pDataParams,PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG, 0U);
    CHECK_STATUS(status);

    status = phacDiscLoop_Sw_SetConfig(pDataParams,PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG, 0U);
    CHECK_STATUS(status);

    status = phacDiscLoop_Sw_SetConfig(pDataParams,PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG, 0U);
    CHECK_STATUS(status);

    status = PHAC_DISCLOOP_NO_TECH_DETECTED;
    while(1)
    {
        do
        {
            if((status & PH_ERR_MASK) != PHAC_DISCLOOP_NO_TECH_DETECTED)
            {
                /* Perform RF Reset */
                status = EmvcoRfReset();
                CHECK_STATUS(status);
            }

            /* Set discovery loop poll state */
            status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_DETECTION);
            CHECK_STATUS(status);

            /* Start Polling, Function will return once card is activated or any other error has occurred */
            status = phacDiscLoop_Run(pDataParams, PHAC_DISCLOOP_ENTRY_POINT_POLL);

        } while((status & PH_ERR_MASK) != PHAC_DISCLOOP_DEVICE_ACTIVATED); /* Exit on Card detection */

        status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
        CHECK_STATUS(status);

        if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
        {
            /* Retrieve 14443-4A protocol parameter */
            status = phpalI14443p4a_GetProtocolParams(
                pDataParams->pPal1443p4aDataParams,
                &bCidEnabled,
                &bCid,
                &bNadSupported,
                &bFwi,
                &bFsdi,
                &bFsci);
            CHECK_STATUS(status);

            /* Set 14443-4 protocol parameter */
            status = phpalI14443p4_SetProtocol(
                pDataParams->pPal14443p4DataParams,
                PH_OFF,
                bCid,
                PH_OFF,
                PH_OFF,
                bFwi,
                bFsdi,
                bFsci);
            CHECK_STATUS(status);
        }

        if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
        {
            /* Retrieve 14443-3b protocol parameter */
            status = phpalI14443p3b_GetProtocolParams(
                pDataParams->pPal1443p3bDataParams,
                &bCidEnabled,
                &bCid,
                &bNadSupported,
                &bFwi,
                &bFsdi,
                &bFsci);
            CHECK_STATUS(status);

            /* Set 14443-4 protocol parameter */
            status = phpalI14443p4_SetProtocol(
                pDataParams->pPal14443p4DataParams,
                PH_OFF,
                bCid,
                PH_OFF,
                PH_OFF,
                bFwi,
                bFsdi,
                bFsci);
            CHECK_STATUS(status);
        }

        /* Send SELECT_PPSE command and start exchange I-PDU */
        status = EmvcoDataLoopBack(pDataParams);
    }
}

/**
* \brief Perform RF Reset as per Emvco Specification
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
static phStatus_t EmvcoRfReset(void)
{
    phStatus_t status = PH_ERR_SUCCESS;

    /*RF Field OFF*/
    status = phhalHw_FieldOff(pDiscLoop->pHalDataParams);
    CHECK_STATUS(status);

    status = phhalHw_Wait(pDiscLoop->pHalDataParams,PHHAL_HW_TIME_MICROSECONDS, 5100);
    CHECK_STATUS(status);

    /*RF Field ON*/
    status = phhalHw_FieldOn(pDiscLoop->pHalDataParams);
    CHECK_STATUS(status);

    return status;
}

/**
* \brief Exchange Data APDU Packets for EMVCO (ISO14443-4 Exchange)
* This function will Exchange APDU data packets provided by Loop-Back Application
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
static phStatus_t EmvcoDataExchange(uint8_t * com_buffer, uint8_t cmdsize, uint8_t ** resp_buffer, uint32_t * wRxLength)
{
    phStatus_t status;
    uint8_t *ppRxBuffer;
    uint16_t wRxLen = 0;

    status = phpalI14443p4_Exchange(phNfcLib_GetDataParams(PH_COMP_PAL_ISO14443P4), PH_EXCHANGE_DEFAULT,
    		com_buffer, cmdsize, &ppRxBuffer, &wRxLen);
    if (PH_ERR_SUCCESS == status)
    {
        /* set the pointer to the start of the R-APDU */
        *resp_buffer = &ppRxBuffer[0];
    }
    else
    {
        /* Exchange not successful, reset the number of rxd bytes */
        wRxLen = 0x00;
    }

    *wRxLength = wRxLen;

    return status;
}

/**
* \brief EMVCo Loop-Back function
* This Loop-Back function converts each received R-APDU into the next C-APDU (by stripping the
* status words), and sends this C-APDU back to the card simulator.
* Also this function send SELECT_PPSE command after card activation.
* Loop-Back Function exist when EOT (End Of Test) Command is received from the card simulator.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
static phStatus_t EmvcoDataLoopBack(phacDiscLoop_Sw_DataParams_t * pDataParams)
{
    uint32_t cmdsize, respsize;
    phStatus_t status;
    uint8_t bEndOfLoopBack = 0;
    uint8_t bRemovalProcedure = PH_OFF;
    cmdsize = sizeof(PPSE_SELECT_APDU);

    status = EmvcoDataExchange(PPSE_SELECT_APDU, cmdsize, &response_buffer, &respsize);

#ifndef RUN_TEST_SUIT

    /*Check if P1 is 0x04 which means that the data field consists of DF name */
    if(PPSE_SELECT_APDU[2] == 0x04)
    {
        DEBUG_PRINTF("\n DF Name: \n");
        /* DF Size = Total Command size - size of(PDU Header + Expected Len(Le))*/
        phApp_Print_Buff(&PPSE_SELECT_APDU[5], PPSE_SELECT_APDU[4]);
    }
    if (respsize > 0)
    {
        memcpy(&PPSE_response_buffer[0],response_buffer,respsize);
        DEBUG_PRINTF("\n SELECT_PPSE Res:\n");
        /* Status word removed */
        phApp_Print_Buff(PPSE_response_buffer, (respsize - 2));
        DEBUG_PRINTF("\nTransaction Done Remove card\n");
    }
    else
    {
        DEBUG_PRINTF("\nFCI not recieved\n");
#ifdef PPSE_NO_LE
        DEBUG_PRINTF("Transaction Done Remove card\n");
#else
        DEBUG_PRINTF("Transaction Failed Replace the card\n");
#endif
    }

#endif

    while (!bEndOfLoopBack)
    {
        if (respsize > 0)
        {
            if (respsize >= MiN_VALID_DATA_SIZE)
            {
                /* EOT (End Of Test) Command. Exit the loop */
                if (eEmdRes_EOT == response_buffer[1])
                {
                    /* Second byte = 0x70, stop the loopback */
                    bEndOfLoopBack = 1;
                    bRemovalProcedure = PH_ON;
                }
                else if (eEmdRes_SW_0 == response_buffer[respsize - 2])
                {
                    /* Format the card response into a new command without the status word 0x90 0x00 */
                    cmdsize = respsize - 2;  /* To Remove two bytes of status word */
                    memcpy(command_buffer, response_buffer, cmdsize);

                    /* Send back(Command) : Received Response - Status_Word */
                    status = EmvcoDataExchange(command_buffer, cmdsize, &response_buffer, &respsize);
                }
                else
                {
                    /* error Abort Loopback */
                    bEndOfLoopBack = 1;
                }
            }
            else/*if (respsize <6)*/
            {
                /* re-send the select appli APDU */
                status = EmvcoDataExchange(PPSE_SELECT_APDU, cmdsize, &response_buffer, &respsize);
                if (respsize == 0)
                {
                    bEndOfLoopBack = 1;
                }
            }
        }/*if(respsize > 0)*/
        else
        {
            bEndOfLoopBack = 1;
        }
    }/*while (!bEndOfLoopBack)*/

    if(bRemovalProcedure == PH_ON)
    {
        /* Set Poll state to perform Tag removal procedure*/
        status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_REMOVAL);
        CHECK_STATUS(status);

        status = phacDiscLoop_Run(pDataParams, PHAC_DISCLOOP_ENTRY_POINT_POLL);
    }
    return status;
}

/**
* This function will load/configure Discovery loop and 14443 PAL with default values to support Emvco2.3.1a(L1) digital compliance.
 * Application can read these values from EEPROM area and load/configure Discovery loop via SetConfig
* \param   bProfile      Reader Library Profile
* \note    Values used below are default and is for demonstration purpose.
*/
static phStatus_t LoadEmvcoSettings()
{
    phStatus_t status = PH_ERR_SUCCESS;

    /* Passive Bailout bitmap config. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_BAIL_OUT, 0x00);
    CHECK_STATUS(status);

    /* Passive CON_DEVICE limit. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_DEVICE_LIMIT, 1);
    CHECK_STATUS(status);

    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_DEVICE_LIMIT, 1);
    CHECK_STATUS(status);

    /* Passive polling Tx Guard times in micro seconds. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_GTA_VALUE_US, 5100);
    CHECK_STATUS(status);

    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_GTB_VALUE_US, 5100);
    CHECK_STATUS(status);

    /* Configure FSDI for the 14443P4A tags */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_FSDI, 0x08);
    CHECK_STATUS(status);

    /* Configure CID for the 14443P4A tags */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_CID, 0x00);
    CHECK_STATUS(status);

    /* Configure DRI for the 14443P4A tags */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DRI, 0x00);
    CHECK_STATUS(status);

    /* Configure DSI for the 14443P4A tags */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DSI, 0x00);
    CHECK_STATUS(status);

    /* Configure AFI for the type B tags */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_AFI_REQ, 0x00);
    CHECK_STATUS(status);

    /* Configure FSDI for the type B tags */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_FSDI, 0x08);
    CHECK_STATUS(status);

    /* Configure CID for the type B tags */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_CID, 0x00);
    CHECK_STATUS(status);

    /* Configure DRI for the type B tags */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_DRI, 0x00);
    CHECK_STATUS(status);

    /* Configure DSI for the type B tags */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_DSI, 0x00);
    CHECK_STATUS(status);

    /* Configure Extended ATQB support for the type B tags */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_EXTATQB, 0x00);
    CHECK_STATUS(status);

    /* Configure reader library mode */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_OPE_MODE, RD_LIB_MODE_EMVCO);
    CHECK_STATUS(status);

    return status;
}

#ifdef NXPBUILD__PHHAL_HW_TARGET
/* Stubbed definitions in case TARGET is enabled */
uint8_t  sens_res[2]     = {0x04, 0x00};
uint8_t  nfc_id1[3]      = {0xA1, 0xA2, 0xA3};
uint8_t  sel_res         = 0x40;
uint8_t  nfc_id3         = 0xFA;
uint8_t  poll_res[18]    = {0x01, 0xFE, 0xB2, 0xB3, 0xB4, 0xB5,
                                   0xB6, 0xB7, 0xC0, 0xC1, 0xC2, 0xC3,
                                   0xC4, 0xC5, 0xC6, 0xC7, 0x23, 0x45 };
#endif /* NXPBUILD__PHHAL_HW_TARGET */
/******************************************************************************
**                            End Of File
******************************************************************************/
