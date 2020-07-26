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
* Example Source for Nfcrdlib_SimplifiedAPI_EMVCo.
* This application will configure Reader Library as per Emvco specification and start Emvco polling.
* This loop back application will send SELECT_PPSE command and is used to test Emvco2.3.1a(L1)
* digital compliance.
* Please refer Readme.txt file  for  Hardware Pin Configuration, Software Configuration and steps to build and
* execute the project which is present in the same project directory.
* $Author: Purnank G (ing05193) $
* $Revision: 5993 $ (v06.01.00)
* $Date: 2016-09-19 13:50:28 +0530 (Mon, 19 Sep 2016) $
*
*/

/**
* Reader Library Headers
*/
#include <phApp_Init.h>

/* NFCLIB Header */
#include <phNfcLib.h>

/* Local headers */
#include "Nfcrdlib_SimplifiedAPI_EMVCo.h"

/*******************************************************************************
**   Global Defines
*******************************************************************************/

uint8_t  response_buffer[PHAC_EMVCO_MAX_BUFFSIZE];
uint16_t respsize = sizeof(response_buffer);

phNfcLib_PeerInfo_t                sNfcLib_Info;                                   /* NFC LIB component holder */

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aEmvcoLoopBackTaskBuffer[EMVCO_LOOPBACK_TASK_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define aEmvcoLoopBackTaskBuffer       NULL
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
void Emvco_LoopBack(void * pParams);
static phStatus_t EmvcoRfReset(uint32_t delayInMs);

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
        phOsal_ThreadObj_t EmvcoLoopBack;
#endif /* PH_OSAL_NULLOS */

        /* Perform Controller specific initialization. */
        phApp_CPU_Init();

        /* Perform OSAL Initialization. */
        (void)phOsal_Init();
        (void)phApp_Led_Init(); /* initialize the LEDs on the board */
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

        DEBUG_PRINTF("\n Emvco compliance example: ");

#ifndef PH_OSAL_NULLOS
        EmvcoLoopBack.pTaskName = (uint8_t *) "EmvcoLoopBack";
        EmvcoLoopBack.pStackBuffer = aEmvcoLoopBackTaskBuffer;
        EmvcoLoopBack.priority = EMVCO_LOOPBACK_TASK_PRIO;
        EmvcoLoopBack.stackSizeInNum = EMVCO_LOOPBACK_TASK_STACK;
        phOsal_ThreadCreate(&EmvcoLoopBack.ThreadHandle, &EmvcoLoopBack, &Emvco_LoopBack, NULL);

        phOsal_StartScheduler();

        DEBUG_PRINTF("RTOS Error : Scheduler exited. \n");

#else
        Emvco_LoopBack(NULL);
#endif
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

void Emvco_LoopBack(void * pParams)
{
    phNfcLib_Status_t   dwStatus;
    uint16_t            wRfTechnos = PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_A | PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_B;
    uint16_t            cmdsize;
    uint8_t             bEndOfLoopBack = 0;
    uint8_t             bRemovalProcedure = PH_NFCLIB_DEACTIVATION_MODE_RF_OFF;
    uint8_t             bEndOfTestAnalog = PH_OFF;

    /* Load Emvco Default setting */
    dwStatus = phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_PROFILE, PH_NFCLIB_ACTIVATION_PROFILE_EMVCO);
    CHECK_NFCLIB_STATUS(dwStatus);

    dwStatus = phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_BLOCKING, PH_ON);
    CHECK_NFCLIB_STATUS(dwStatus);

#ifdef PHDRIVER_K82F_PNEV5190B_BOARD
	/* switch ON the Red LED to indicate board is active */
    phApp_Led_On(PHDRIVER_PIN_RLED);
#endif /* PHDRIVER_K82F_PNEV5190B_BOARD */
    while (1)
    {
        dwStatus = phNfcLib_Activate(wRfTechnos, &sNfcLib_Info, NULL);
        if (dwStatus == PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE)
        {
#ifdef PHDRIVER_K82F_PNEV5190B_BOARD
        	/* switch ON the Blue LED to indicate cards detected */
            phApp_Led_On(PHDRIVER_PIN_BLED);
#endif /* PHDRIVER_K82F_PNEV5190B_BOARD */

            dwStatus = phNfcLib_Transmit(PPSE_SELECT_APDU, sizeof(PPSE_SELECT_APDU));
            if (dwStatus == PH_NFCLIB_STATUS_SUCCESS)
            {
                respsize = sizeof(response_buffer);
                dwStatus = phNfcLib_Receive(response_buffer, &respsize, NULL);
                if ((dwStatus == PH_NFCLIB_STATUS_SUCCESS) && (respsize > 0))
                {
#ifdef PHDRIVER_K82F_PNEV5190B_BOARD
                	/* switch ON the Green LED to indicate cards detected */
                    phApp_Led_On(PHDRIVER_PIN_GLED);
#endif /* PHDRIVER_K82F_PNEV5190B_BOARD */
                    while (!bEndOfLoopBack)
                    {
                        if (respsize >= MIN_VALID_DATA_SIZE)
                        {
                            /* EOT (End Of Test) Command. Exit the loop */
                            if (eEmdRes_EOT == response_buffer[1])
                            {
                                /* Second byte = 0x70, stop the loopback */
                                bEndOfLoopBack = 1;
                                bRemovalProcedure = PH_NFCLIB_DEACTIVATION_MODE_RELEASE;
                            }
                            /* EOT (End Of Test) Command used for analog tests. Exit the loop, perform long rf reset. */
                            else if (eEmdRes_EOT_AnalogTests == response_buffer[1])
                            {
                                /* Second byte = 0x72, stop the loopback */
                                bEndOfLoopBack = 1;
                                bEndOfTestAnalog = PH_ON;
                            }
                            else if (eEmdRes_SW_0 == response_buffer[respsize - 2])
                            {
                                /* Format the card response into a new command without the status word 0x90 0x00 */
                                cmdsize = respsize - 2;  /* To Remove two bytes of status word */

                                /* Send back(Command) : Received Response - Status_Word */
                                dwStatus = phNfcLib_Transmit(response_buffer, cmdsize);
                                if (dwStatus == PH_NFCLIB_STATUS_SUCCESS)
                                {
                                    respsize = sizeof(response_buffer);
                                    dwStatus = phNfcLib_Receive(response_buffer, &respsize, NULL);
                                    if (dwStatus != PH_NFCLIB_STATUS_SUCCESS)
                                    {
                                        bEndOfLoopBack = 1;
                                    }
                                }
                                else
                                {
                                    bEndOfLoopBack = 1;
                                }
                            }
                            else
                            {
                                /* error Abort Loopback */
                                bEndOfLoopBack = 1;
                            }
                        }/*if (respsize >= MIN_VALID_DATA_SIZE)*/
                        else
                        {
                            /* re-send the select appli APDU */
                            dwStatus = phNfcLib_Transmit(PPSE_SELECT_APDU, sizeof(PPSE_SELECT_APDU));
                            if (dwStatus == PH_NFCLIB_STATUS_SUCCESS)
                            {
                                respsize = sizeof(response_buffer);
                                dwStatus = phNfcLib_Receive(response_buffer, &respsize, NULL);
                                if (dwStatus != PH_NFCLIB_STATUS_SUCCESS)
                                {
                                    bEndOfLoopBack = 1;
                                }
                            }
                            else
                            {
                                bEndOfLoopBack = 1;
                            }
                        }
                    }/*while (!bEndOfLoopBack)*/
                    bEndOfLoopBack = 0;

                    if (bEndOfTestAnalog == PH_ON)
                    {
                        /* analog tests - end of tests. long rf reset is required. */
                        EmvcoRfReset(25);
                        bEndOfTestAnalog = PH_OFF;
                    }
                }
            }
        }
        dwStatus = phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_DEACTIVATION_BLOCKING, PH_ON);
        CHECK_NFCLIB_STATUS(dwStatus);

        dwStatus = phNfcLib_Deactivate(bRemovalProcedure, NULL);
        CHECK_NFCLIB_STATUS(dwStatus);

        bRemovalProcedure = PH_NFCLIB_DEACTIVATION_MODE_RF_OFF;

#ifdef PHDRIVER_K82F_PNEV5190B_BOARD
        /* switch OFF the Green/Blue LED to indicate cards detected */
        phApp_Led_Off(PHDRIVER_PIN_GLED);
        phApp_Led_Off(PHDRIVER_PIN_BLED);
#endif /* PHDRIVER_K82F_PNEV5190B_BOARD */

    }
}

/**
* \brief Perform RF Reset for the specified milli-seconds duration.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
static phStatus_t EmvcoRfReset(uint32_t delayInMs)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    /*RF Field OFF*/
    wStatus = phhalHw_FieldOff(phNfcLib_GetDataParams(PH_COMP_HAL));
    CHECK_STATUS(wStatus);

    wStatus = phhalHw_Wait(phNfcLib_GetDataParams(PH_COMP_HAL), PHHAL_HW_TIME_MILLISECONDS, delayInMs);
    CHECK_STATUS(wStatus);

    /*RF Field ON*/
    wStatus = phhalHw_FieldOn(phNfcLib_GetDataParams(PH_COMP_HAL));
    CHECK_STATUS(wStatus);

    return wStatus;
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
