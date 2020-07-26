/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Example Source for Nfcrdlib_SimplifiedAPI_EMVCo_Interop.
* This application will configure Reader Library as per Emvco specification and start Emvco Interoperability Loopback.
* This loop back application will send SELECT_PPSE command and is used to test Emvco2.6b(L1) with
* add-on(TTA Bulletin No.195) compliance.
* Please refer Readme.txt file for Hardware Pin Configuration, Software Configuration and steps to build and
* execute the project which is present in the same project directory.
* $Author: Jaganmohan Reddy (nxp95127) $
* $Revision: 1 $ (v06.01.00)
* $Date: 2017-11-08 11:30:00 +0530 (Wed, 08 Nov 2017) $
*
*/

/**
* Reader Library Headers
*/
#include <phApp_Init.h>

/* NFCLIB Header */
#include <phNfcLib.h>

/* Local headers */
#include "Nfcrdlib_SimplifiedAPI_EMVCo_Interop.h"
#include "Nfcrdlib_SimplifiedAPI_EMVCo_Interop_status.h"

/*******************************************************************************
**   Global Defines
*******************************************************************************/

uint8_t  response_buffer[PHAC_EMVCO_MAX_BUFFSIZE];
uint16_t respsize = sizeof(response_buffer);

phNfcLib_PeerInfo_t    sNfcLib_Info;    /* NFC LIB component holder */

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aEmvcoInteropTaskBuffer[EMVCO_INTEROP_TASK_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define aEmvcoInteropTaskBuffer       NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

/*******************************************************************************
**   Static Defines
*******************************************************************************/
/* EMVCo: Select PPSE Command */
static uint8_t PPSE_SELECT_APDU[] = { 0x00, 0xA4, 0x04, 0x00, 0x0E, 0x32, 0x50, 0x41, 0x59,
        0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 };

/*******************************************************************************
**   Function Declaration
*******************************************************************************/
void Emvco_Interop(void * pParams);

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
        phStatus_t        status;
        phNfcLib_Status_t dwStatus;
#ifdef PH_PLATFORM_HAS_ICFRONTEND
        phNfcLib_AppContext_t AppContext = {0};
#endif /* PH_PLATFORM_HAS_ICFRONTEND */

#ifndef PH_OSAL_NULLOS
        phOsal_ThreadObj_t EmvcoInterop;
#endif /* PH_OSAL_NULLOS */

        /* Perform Controller specific initialization. */
        phApp_CPU_Init();

        /* Perform OSAL Initialization. */
        (void)phOsal_Init();

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

        /* IRQ configuration for different HOST environments */
        status = phApp_Configure_IRQ();
        CHECK_STATUS(status);
        if(status != PH_ERR_SUCCESS) break;

        DEBUG_PRINTF("\n Emvco Interoperability Loopback example: ");

#ifndef PH_OSAL_NULLOS
        EmvcoInterop.pTaskName = (uint8_t *) "EmvcoInterop";
        EmvcoInterop.pStackBuffer = aEmvcoInteropTaskBuffer;
        EmvcoInterop.priority = EMVCO_INTEROP_TASK_PRIO;
        EmvcoInterop.stackSizeInNum = EMVCO_INTEROP_TASK_STACK;
        phOsal_ThreadCreate(&EmvcoInterop.ThreadHandle, &EmvcoInterop, &Emvco_Interop, NULL);

        phOsal_StartScheduler();

        DEBUG_PRINTF("RTOS Error : Scheduler exited. \n");

#else
        Emvco_Interop(NULL);
#endif
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

/***********************************************************************************************
 * \brief   This function performs EMVCo Interoperability Loopback operation.
 * \param   *pParams
 * \return  This function will never return
 **********************************************************************************************/
void Emvco_Interop(void * pParams)
{
    phNfcLib_Status_t   dwStatus;
    uint16_t            wRfTechnos = PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_A | PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_B;
    uint16_t            cmdsize;
    uint8_t             bEndOfLoopBack = 0U;
    uint8_t             bRemovalProcedure = PH_NFCLIB_DEACTIVATION_MODE_RF_OFF;
    uint8_t             bAbortLoopback;

    /* Load Emvco Default setting */
    dwStatus = phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_PROFILE, PH_NFCLIB_ACTIVATION_PROFILE_EMVCO);
    CHECK_NFCLIB_STATUS(dwStatus);

    dwStatus = phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_BLOCKING, PH_ON);
    CHECK_NFCLIB_STATUS(dwStatus);

    /* Initialization of GPIO's to indicate Interoperability test status */
    dwStatus = Emvco_Interop_Init();
    CHECK_NFCLIB_STATUS(dwStatus);

    while (1)
    {
        dwStatus = phNfcLib_Activate(wRfTechnos, &sNfcLib_Info, NULL);
        if (dwStatus == PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE)
        {
            dwStatus = phNfcLib_Transmit(PPSE_SELECT_APDU, sizeof(PPSE_SELECT_APDU));
            if (dwStatus == PH_NFCLIB_STATUS_SUCCESS)
            {
                bAbortLoopback = 0U;
                while (!bEndOfLoopBack)
                {
                    bEndOfLoopBack = 1U;
                    respsize = sizeof(response_buffer);
                    dwStatus = phNfcLib_Receive(response_buffer, &respsize, NULL);
                    if ((dwStatus == PH_NFCLIB_STATUS_SUCCESS) && (respsize > 0U))
                    {
                        if (respsize >= MIN_VALID_DATA_SIZE)
                        {
                            /* EOT (End Of Test) Command. Exit the loop */
                            if (eEmdRes_EOT == response_buffer[1U])
                            {
                                /* Second byte = 0x70, indicate success and stop the loopback */
                                Emvco_Interop_Status(EMVCO_INTEROP_ERR_SUCCESS);
                                bRemovalProcedure = PH_NFCLIB_DEACTIVATION_MODE_RELEASE;
                            }
                            else if ((eEmdRes_SW_0 == response_buffer[respsize - 2U]) &&
                                     (eEmdRes_SW_1 == response_buffer[respsize - 1U]))
                            {
                                /* Format the card response into a new command without the status word 0x90 0x00 */
                                cmdsize = respsize - 2;  /* To Remove two bytes of status word */

                                /* Send back(Command) : Received Response - Status_Word */
                                dwStatus = phNfcLib_Transmit(response_buffer, cmdsize);
                                if (dwStatus == PH_NFCLIB_STATUS_SUCCESS)
                                {
                                    /* continue loopback */
                                    bEndOfLoopBack = 0U;
                                }
                            }
                            else
                            {
                                /* Error. Abort Loopback */
                                bAbortLoopback = 1U;
                            }
                        }/* (respsize >= MIN_VALID_DATA_SIZE) */
                        else
                        {
                            /* re-send the select appli APDU */
                            dwStatus = phNfcLib_Transmit(PPSE_SELECT_APDU, sizeof(PPSE_SELECT_APDU));
                            if (dwStatus == PH_NFCLIB_STATUS_SUCCESS)
                            {
                                /* continue loopback */
                                bEndOfLoopBack = 0U;
                            }
                        }
                    }
                }/* while (!bEndOfLoopBack) */
                bEndOfLoopBack = 0U;
            }

            if ((dwStatus != PH_NFCLIB_STATUS_SUCCESS) || (bAbortLoopback != 0U))
            {
                Emvco_Interop_Status(EMVCO_INTEROP_ERR_FAIL);
                bRemovalProcedure = PH_NFCLIB_DEACTIVATION_MODE_RELEASE;
            }
        }

        dwStatus = phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_DEACTIVATION_BLOCKING, PH_ON);
        CHECK_NFCLIB_STATUS(dwStatus);

        dwStatus = phNfcLib_Deactivate(bRemovalProcedure, NULL);
        CHECK_NFCLIB_STATUS(dwStatus);

        bRemovalProcedure = PH_NFCLIB_DEACTIVATION_MODE_RF_OFF;
    }
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
