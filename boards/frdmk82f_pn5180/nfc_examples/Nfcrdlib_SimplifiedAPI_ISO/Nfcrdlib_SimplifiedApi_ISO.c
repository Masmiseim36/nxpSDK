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
 * $Author: Ashish Pal (nxp79566) $
 * $Revision: 5458 $ (v06.01.00)
 * $Date: 2016-09-01 19:11:09 +0530 (Thu, 01 Sept 2016) $
 *
 * History:
 *
 *
 */

/**
* Reader Library Headers
*/

#include <Nfcrdlib_SimplifiedApi_ISO.h>


/*******************************************************************************
**   Global Defines
*******************************************************************************/
#define DATA_BUFFER_LEN             256 /* Buffer length */

uint8_t bMoreDataAvailable = 0;
uint16_t wNumberofBytes = 256;
phNfcLib_Transmit_t phNfcLib_TransmitInput;
phNfcLib_PeerInfo_t PeerInfo = {0};
uint8_t             bDataBuffer[DATA_BUFFER_LEN];  /* universal data buffer */

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aSimplifiedTaskBuffer[SIMPLIFIED_ISO_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define aSimplifiedTaskBuffer       NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
void SimplifiedApiDemo (void* pParams);

/*******************************************************************************
**   Function Declarations
*******************************************************************************/


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
        phOsal_ThreadObj_t SimplifiedApi;
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

        DEBUG_PRINTF("\n Simplified API ISO example: ");

#ifndef PH_OSAL_NULLOS
        SimplifiedApi.pTaskName = (uint8_t *) "SimplifiedApi";
        SimplifiedApi.pStackBuffer = aSimplifiedTaskBuffer;
        SimplifiedApi.priority = SIMPLIFIED_ISO_PRIO;
        SimplifiedApi.stackSizeInNum = SIMPLIFIED_ISO_STACK;
        phOsal_ThreadCreate(&SimplifiedApi.ThreadHandle, &SimplifiedApi, &SimplifiedApiDemo, NULL);
        phOsal_StartScheduler();

        DEBUG_PRINTF("RTOS Error : Scheduler exited. \n");

#else
        SimplifiedApiDemo(NULL);
#endif
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

void SimplifiedApiDemo (void* pParams)
{
    uint32_t dwStatus = 0;
    uint16_t wStatus = 0;
    uint16_t wTechnologyMask = 0x1B;
    uint8_t bAtqb[13] = {0x00};
    uint8_t bAtqbLen = 0;
    uint8_t bGetParamDemo = 0;
    while(1)
    {
        do
        {

            /* Configuring the activation profile as ISO */
            phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_PROFILE, PH_NFCLIB_ACTIVATION_PROFILE_ISO);
            if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
            {
                break;
            }
            /* Activating the card with blocking activation mode */
            dwStatus = phNfcLib_Activate(wTechnologyMask, &PeerInfo, NULL);
            if(dwStatus != PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE)
            {
                break;
            }

            bGetParamDemo = 0;
            switch(PeerInfo.dwActivatedType)
            {
                /* Reference application for the particular tech type will divert form here */
            case E_PH_NFCLIB_MIFARE_CLASSIC_1K:
                DEBUG_PRINTF (" \nMifare Classic 1K detected... \n");
                dwStatus = NfcLib_MifareClassic_1k_Reference_app();
                break;

            case E_PH_NFCLIB_MIFARE_CLASSIC_4K:
                DEBUG_PRINTF (" \nMifare Classic 4K detected... \n");
                dwStatus = NfcLib_MifareClassic_1k_Reference_app();
                break;

            case E_PH_NFCLIB_MIFARE_ULTRALIGHT:
                DEBUG_PRINTF (" \nMifare UltraLight detected... \n");
                dwStatus = NfcLib_MifareUltralight_Reference_app();
                break;

            case E_PH_NFCLIB_MIFARE_DESFIRE:
                DEBUG_PRINTF (" \nMifare Desfire detected... \n");
                DEBUG_PRINTF (" \nMifare Desfire not supported in simplified api... \n");
                break;

            case E_PH_NFCLIB_TYPEB_LAYER3:
                DEBUG_PRINTF (" \nType B Layer 3 card detected... \n");
                DEBUG_PRINTF (" \nThis application contains nothing for only layer 3 Type B card... \n");
                break;

            case E_PH_NFCLIB_TYPEA_LAYER3:
                DEBUG_PRINTF (" \nType A Layer 3 card or Mifare Classic/ultralight with merged atqa detected... \n");
                DEBUG_PRINTF (" \nThis application contains nothing for only layer 3 Type A card... \n");
                break;

            case E_PH_NFCLIB_TYPEA_LAYER4:
                DEBUG_PRINTF (" \nType A Layer 4 card or Mifare desfire with merged atqa detected... \n");
                dwStatus = NfcLib_Layer4TypeA_Reference_app();
                break;

            case E_PH_NFCLIB_TYPEB_LAYER4:
                DEBUG_PRINTF (" \nType B Layer 4 card detected... \n");
                DEBUG_PRINTF (" \n Do not remove the card ..\n");
                DEBUG_PRINTF (" \n Wait till operation completes... \n");
                dwStatus = NfcLib_TypeB_Reference_app();
                bGetParamDemo = 1;
                break;

            case E_PH_NFCLIB_ISO15693:
                DEBUG_PRINTF (" \nType ISO 15693 compatible card detected... \n");
                dwStatus = NfcLib_ISO15693_Reference_app();
                break;

            case E_PH_NFCLIB_ISO18000p3m3:
                DEBUG_PRINTF (" \nType ISO 18000 compatible card detected... \n");
                dwStatus = NfcLib_ISO18000p3m3_Reference_app();
                break;

            default:
                break;
            }

            dwStatus = phNfcLib_Deactivate(PH_NFCLIB_DEACTIVATION_MODE_RELEASE,
                                            &PeerInfo
                                            );
            DEBUG_PRINTF("\n Please remove the card\n");

            if(bGetParamDemo)
            {
                (void)phhalHw_SetConfig(phNfcLib_GetDataParams(PH_COMP_HAL),
                        PHHAL_HW_CONFIG_SET_EMD,
                              PH_OFF);
                (void)phhalHw_Wait(phNfcLib_GetDataParams(PH_COMP_HAL),
                        PHHAL_HW_TIME_MILLISECONDS,
                              5);
                wStatus = phpalI14443p3b_WakeUpB(phNfcLib_GetDataParams(PH_COMP_PAL_ISO14443P3B),
                        0x00, 0x00,
                        0x00, &bAtqb[0], &bAtqbLen);
                wStatus = wStatus & PH_ERR_MASK;
                while(((wStatus  == PH_ERR_SUCCESS) || (wStatus == PH_ERR_COLLISION_ERROR)))
                {
                    (void)phpalI14443p3b_HaltB(phNfcLib_GetDataParams(PH_COMP_PAL_ISO14443P3B));
                    (void)phhalHw_Wait(phNfcLib_GetDataParams(PH_COMP_HAL),
                            PHHAL_HW_TIME_MILLISECONDS,
                                  5);
                    wStatus = phpalI14443p3b_WakeUpB(phNfcLib_GetDataParams(PH_COMP_PAL_ISO14443P3B), 0x00, 0x00,
                            0x00, &bAtqb[0], &bAtqbLen);
                    wStatus = wStatus & PH_ERR_MASK;
                }
            }

            if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
            {
                DEBUG_PRINTF (" \nDeactivate with Release Mode failed, card was removed from vicinity... \n");
                DEBUG_PRINTF (" \n Thus Performing Deactivate with RF OFF mode... \n");
                dwStatus = phNfcLib_Deactivate(PH_NFCLIB_DEACTIVATION_MODE_RF_OFF,
                                                &PeerInfo
                                                );
            }

            if(bGetParamDemo)
            {
                (void)phhalHw_Wait(phNfcLib_GetDataParams(PH_COMP_HAL), PHHAL_HW_TIME_MILLISECONDS, 1000);
            }

        } while(0);
    }

}


/***********************************************************************************************
 *                            End Of File
 **********************************************************************************************/
