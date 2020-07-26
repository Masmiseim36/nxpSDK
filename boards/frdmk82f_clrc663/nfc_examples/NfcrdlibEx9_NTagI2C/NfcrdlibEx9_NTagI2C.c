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
 * Example Source for NfcrdlibEx9_NTagI2c that uses the NTag I2C implementation.
 * This example will load/configure Discovery loop for NTag I2C, uses POLL mode to detect the NTag I2C card.
 * Displays detected NTag I2C card information like UID, ATQA, SAK, Product, Version Info, Page Read and Write status.
 * Example will activate the device at index zero whenever multiple NTag I2C cards are detected.

 * Please refer Readme.txt file for Hardware Pin Configuration, Software Configuration and steps to build and
 * execute the project which is present in the same project directory.
 * $Author: Purnank G (ing05193) $
 * $Revision: 6114 $ (v06.01.00)
 * $Date: 2016-09-22 16:56:53 +0530 (Thu, 22 Sep 2016) $
 *
 * History:
 *
 *
 */

/**
* Reader Library Headers
*/
#include <phApp_Init.h>

/* Local headers */
#include "NfcrdlibEx9_NTagI2C.h"

/*******************************************************************************
**   Global Variable Declaration
*******************************************************************************/
phacDiscLoop_Sw_DataParams_t       * pDiscLoop;     /* Pointer to Discovery loop data-params */
phalMful_Sw_DataParams_t           * palMful;       /* Pointer to AL MFUL data-params */

uint8_t                            bDataBuffer[DATA_BUFFER_LEN];  /* universal data buffer */

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aNTagI2CTaskBuffer[NTAGI2C_TASK_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define aNTagI2CTaskBuffer      NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

/*******************************************************************************
**   Function Declarations
*******************************************************************************/
void NfcrdlibEx9_NTagI2C(void *pParams);
phStatus_t phExample_Init(void);

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
        phStatus_t            status = PH_ERR_INTERNAL_ERROR;
        phNfcLib_Status_t     dwStatus;
#ifdef PH_PLATFORM_HAS_ICFRONTEND
        phNfcLib_AppContext_t AppContext = {0};
#endif /* PH_PLATFORM_HAS_ICFRONTEND */

#ifndef PH_OSAL_NULLOS
        phOsal_ThreadObj_t NTagI2C;
#endif /* PH_OSAL_NULLOS */

        /* Perform Controller specific initialization. */
        phApp_CPU_Init();

        /* Perform OSAL Initialization. */
        (void)phOsal_Init();

        /* Print Example application name */
        DEBUG_PRINTF("\n *** NTag I2C Example *** \n");

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
        pDiscLoop = (phacDiscLoop_Sw_DataParams_t *) phNfcLib_GetDataParams(PH_COMP_AC_DISCLOOP);
        palMful = (phalMful_Sw_DataParams_t *) phNfcLib_GetDataParams(PH_COMP_AL_MFUL);

        /* Configure the IRQ */
        status = phApp_Configure_IRQ();
        CHECK_STATUS(status);
        if(status != PH_ERR_SUCCESS) break;

#ifndef PH_OSAL_NULLOS

        NTagI2C.pTaskName = (uint8_t *) "NTagI2C";
        NTagI2C.pStackBuffer = aNTagI2CTaskBuffer;
        NTagI2C.priority = NTAGI2C_TASK_PRIO;
        NTagI2C.stackSizeInNum = NTAGI2C_TASK_STACK;

        phOsal_ThreadCreate(&NTagI2C.ThreadHandle, &NTagI2C, &NfcrdlibEx9_NTagI2C, pDiscLoop);

        phOsal_StartScheduler();

        /* Print Error info */
        DEBUG_PRINTF("RTOS Error : Scheduler exited. \n");

#else
        (void)NfcrdlibEx9_NTagI2C(pDiscLoop);
#endif
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

/***********************************************************************************************
 * \brief   This function demonstrates the NTag I2C application as mentioned
 *          Performs Type A Card detection and Activation and displays Tag UID, ATQA and SAK info.
 *          Performs Read Tag Version info to confirm that the detected Tag is NTag I2C.
 *          Performs Read Session Registers to check the Tag Mode. We can perform Sector/Page Read Write only in non pass-through mode.
 *          Performs Read operation and Displays content of Sector 0 Pages 5 to 8 Data.
 *          Performs Write operation i.e. writes back the sector 0 Pages 5 to 8 Data.
 * \param   *pParams
 * \return  This function will never return
 **********************************************************************************************/
void NfcrdlibEx9_NTagI2C(void *pParams)
{
    phStatus_t  status = 0;
    uint16_t    wTagsDetected = 0;
    uint8_t     bTagType;
    uint8_t     *pBuff;
    uint16_t    numOfBytes;
    uint8_t     bSectorNum;
    uint8_t     bMoreCardsAvailable;

    /* Initialize library */
    status = phExample_Init();
    CHECK_STATUS(status);

    while(1)    /* Continuous loop */
    {
        DEBUG_PRINTF("\nReady to detect");
        DEBUG_PRINTF("\n");

        do
        {
            /* Field OFF */
            status = phhalHw_FieldOff(pHal);
            CHECK_STATUS(status);

            status = phhalHw_Wait(pHal, PHHAL_HW_TIME_MICROSECONDS, 5100);
            CHECK_STATUS(status);

            /* Configure Discovery loop for Poll Mode */
            status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_DETECTION);
            CHECK_STATUS(status);

            /* Run Discovery loop */
            status = phacDiscLoop_Run(pDiscLoop, PHAC_DISCLOOP_ENTRY_POINT_POLL);

        }while((status & PH_ERR_MASK) != PHAC_DISCLOOP_DEVICE_ACTIVATED); /* Exit on Card detection */

        /* Card detected */
        /* Get the tag types detected info */
        status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);

        /* Check for Status */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Check for Type A tag detection */
            if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
            {
                /* Bit b3 is set to zero, [Digital] 4.8.2 */
                 /* Mask out all other bits except for b7 and b6 */
                 bTagType = (pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aSak & 0x60);
                 bTagType = bTagType >> 5;

                 /* Check for Type 2 Tag */
                 if (bTagType == PHAC_DISCLOOP_TYPEA_TYPE2_TAG_CONFIG_MASK)
                 {
                    /* Check for NTAG I2C */
                    if ((pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa[0] == 0x44)&&
                        (pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa[1] == 0x00)&&
                        (pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aSak == 0x00))
                    {
                        do
                        {
                            /* Print UID */
                            DEBUG_PRINTF ("\nUID: ");
                            phApp_Print_Buff(pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                                    pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

                            /* Print ATQA  and SAK */
                            DEBUG_PRINTF("\nATQA:");
                            phApp_Print_Buff(pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa, 2);
                            DEBUG_PRINTF ("\nSAK: 0x%x",pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aSak);

                            /* Print Tag info */
                            DEBUG_PRINTF ("\n\nType A T2-tag detected ");

                            /* Read Tag Version
                             * Byte 0: Fixed Header(0x00)
                             * Byte 1: Vendor ID(NXP Semiconductor: 0x04)
                             * Byte 2: Product Type(NTag: 0x04)
                             * Byte 6: Storage size(1k: 0x13 and 2k: 0x15)
                             */
                            status = phalMful_GetVersion(palMful, &bDataBuffer[0]);

                            /* Check for Error */
                            if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                            {
                                DEBUG_PRINTF ("\nUnable to Read Tag Version information");
                                DEBUG_PRINTF("\n\nExecution aborted!!!\n");
                                break;
                            }

                            /* Check for Tag Info */
                            if((bDataBuffer[0] == 0x00) && (bDataBuffer[1] == 0x04) && (bDataBuffer[2] == 0x04))
                            {
                                DEBUG_PRINTF ("\nProduct: NTag I2C ");
                                if(bDataBuffer[6] == NTAG_I2C_1K_VERSION)
                                {
                                    DEBUG_PRINTF ("1k Version ");
                                }
                                else if(bDataBuffer[6] == NTAG_I2C_2K_VERSION)
                                {
                                    DEBUG_PRINTF ("2k Version ");
                                }
                                else
                                {
                                    DEBUG_PRINTF ("\nInvalid Tag Version");
                                    DEBUG_PRINTF("\n\nExecution aborted!!!\n");
                                    break;
                                }
                            }

                            /* Read Tag Mode */
                            /* Select Sector 3 to Read Session Registers */
                            status = phalMful_SectorSelect(palMful, 3);
                            if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                            {
                                DEBUG_PRINTF ("\nSector 3 selection Failed");
                                DEBUG_PRINTF ("\nUnable to Read Tag Mode");
                                DEBUG_PRINTF("\n\nExecution aborted!!!\n");
                                break;
                            }

                            /* Read Session registers */
                            status = phalMful_Read(palMful, 0xF8, bDataBuffer);

                            /* check for Error */
                            if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                            {
                                DEBUG_PRINTF ("\nSession Registers Read Failed");
                                DEBUG_PRINTF ("\nUnable to Read Tag Mode");
                                DEBUG_PRINTF("\n\nExecution aborted!!!\n");
                                break;
                            }

                            /* check for Pass-through mode */
                            if ((bDataBuffer[0] & PTHRU_ON_OFF))
                            {
                                DEBUG_PRINTF ("\nTag is Configured in Pass-through Mode");
                                DEBUG_PRINTF ("\nCan not perform Sector/Page Read Write Operation");
                                DEBUG_PRINTF("\n\nExecution aborted!!!\n");
                                break;
                            }

                            /* --- Read Operation --- */
                            /* Select Sector 0 */
                            status = phalMful_SectorSelect(palMful, 0);

                            /* check for Success */
                            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                            {
                                /* Read Sector 0 Page 5 to 8 Data */
                                status = phalMful_FastRead(palMful, 5, 8, &pBuff, &numOfBytes);

                                /* check for Success */
                                if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                                {
                                    DEBUG_PRINTF ("\n\nRead Success");

                                    /* Copy Sector0 Page 5 to 6 Data */
                                    memcpy(bDataBuffer, pBuff, numOfBytes);

                                    /* Display contents of Sector 0 Pages 5 to 6 Data */
                                    for (bSectorNum = 5; bSectorNum <= 8; bSectorNum++)
                                    {
                                        DEBUG_PRINTF ("\nSector 0 Page %d Data: ", bSectorNum);
                                        phApp_Print_Buff(&bDataBuffer[0 + ((bSectorNum - 5) * 4)], 4);
                                    }
                                }
                            }

                            /* Check for Error */
                            if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                            {
                                DEBUG_PRINTF ("\nRead Operation Failed!!!");
                                DEBUG_PRINTF("\n\nExecution aborted!!!\n");
                                break;
                            }

                            /* End of Read Operation */
                            DEBUG_PRINTF("\n\n --- End of Read Operation --- \n");

                            /* --- Write Operation --- */
                            DEBUG_PRINTF("\nInitiation of Write operation to write back the Sector 0 Pages 5 to 8 Data \n");

                            /* Select Sector 0 */
                            status = phalMful_SectorSelect(palMful, 0);

                            /* check for Success */
                            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                            {
                                /* Write data to Sector 0 Page 5 to 8 */
                                for (bSectorNum = 5; bSectorNum <= 8; bSectorNum++)
                                {
                                    /* Page Write */
                                    status = phalMful_Write(palMful, bSectorNum, &bDataBuffer[0 + ((bSectorNum - 5) * 4)]);

                                    /* Check for Success */
                                    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                                    {
                                        DEBUG_PRINTF("\nWrite Success: Sector 0 Page %d", bSectorNum);
                                    }
                                    else
                                    {
                                        DEBUG_PRINTF ("\nWrite Failed: Sector 0 Page %d ", bSectorNum);
                                        break;
                                    }
                                }
                            }

                            /* Check for Error */
                            if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                            {
                                DEBUG_PRINTF ("\nWrite Operation Failed!!! ");
                                DEBUG_PRINTF("\n\nExecution aborted!!!\n");
                                break;
                            }

                            /* End of Write Operation */
                            DEBUG_PRINTF("\n\n --- End of Write Operation --- \n");

                            /* End of example */
                            DEBUG_PRINTF("\n\n --- End of Example --- \n\n");

                            DEBUG_PRINTF("\nPlease Remove the Card\n\n");

                        }while(0);

                        /* Field RESET */
                        status = phhalHw_FieldReset(pHal);
                        CHECK_STATUS(status);

                        /* Make sure that example application is not detecting the same card continuously */
                        do
                        {
                            /* Activate Card */
                            status = phpalI14443p3a_ActivateCard(
                                pDiscLoop->pPal1443p3aDataParams,
                                pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                                pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize,
                                pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                                &pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize,
                                &pDiscLoop->sTypeATargetInfo.aTypeA_I3P3[0].aSak,
                                &bMoreCardsAvailable);

                            /* Check for Status */
                            if (status != PH_ERR_SUCCESS)
                            {
                                break; /* Card Removed, break from the loop */
                            }

                            /* Send HaltA */
                            phpalI14443p3a_HaltA(pDiscLoop->pPal1443p3aDataParams);

                            /* Delay - 5 milli seconds*/
                            status = phhalHw_Wait(pHal, PHHAL_HW_TIME_MILLISECONDS, 5);
                            CHECK_STATUS(status);

                        }while(1);
                    }
                }
            }
        }
    }
}

/***********************************************************************************************
 * \brief   Application initialization
 * \param   none
 * \return  status  Returns the function status
 **********************************************************************************************/
phStatus_t phExample_Init(void)
{
    phStatus_t  status = 0;

    /* Bailout on Type A detect */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_BAIL_OUT, PHAC_DISCLOOP_POS_BIT_MASK_A);
    CHECK_STATUS(status);

    /* Read the version of the reader IC */
#if defined NXPBUILD__PHHAL_HW_RC663
    status = phhalHw_Rc663_ReadRegister(pHal, PHHAL_HW_RC663_REG_VERSION, &bDataBuffer[0]);
    CHECK_STATUS(status);
    DEBUG_PRINTF("\nReader chip RC663 Version: 0x%02x\n", bDataBuffer[0]);
#endif

    /* Return Success */
    return PH_ERR_SUCCESS;
}

/* Stubs, in case the phApp_Init.c expects these implementations */
#ifdef NXPBUILD__PHPAL_I14443P4MC_SW
/*
 * WTX Callback called from WTX timer of 14443p3mC PAL.
 */
void pWtoxCallBck()
{
  /* Dummy */
}

uint8_t aAppHCEBuf[32];
uint16_t wAppHCEBuffSize = sizeof(aAppHCEBuf);
#endif /* NXPBUILD__PHPAL_I14443P4MC_SW */

#ifdef NXPBUILD__PHPAL_I18092MT_SW
void pRtoxCallBck()
{
  /* Dummy */
}
#endif /* NXPBUILD__PHPAL_I18092MT_SW */

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

/***********************************************************************************************
 *                            End Of File
 **********************************************************************************************/
