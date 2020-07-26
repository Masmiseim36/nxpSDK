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
 * Reference application file for MFC interface of Simplified API
 * $Author: Ashish Pal (nxp79566) $
 * $Revision: 5458 $ (v06.01.00)
 * $Date: 2016-09-01 19:11:09 +0530 (Thu, 01 Sept 2016) $
 *
 * History:
 *
 *
 */

#include <Nfcrdlib_SimplifiedApi_ISO.h>

static uint8_t Message[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
extern phNfcLib_Transmit_t phNfcLib_TransmitInput;
extern phNfcLib_PeerInfo_t PeerInfo;

extern uint8_t bMoreDataAvailable;
extern uint16_t wNumberofBytes;
extern uint8_t  bDataBuffer[256];

 #define MFC_KEYA           0x0AU   /**< MIFARE(R) Key A. */
 #define MFC_KEYB           0x0BU   /**< MIFARE(R) Key B. */

void framevalueblock(uint8_t* pValue, uint8_t bAddrData, uint8_t* pBlock);

/*
 * This application is to demonstrate the usage of simplified API related to Mifare Classic 1K
 * The application reaches to this point only after the activation of a Mifare Classic 1k card
 * with Type A technology is done. Transmit api is used to perform any command exchange with the card and
 * receive api is used to get back the out data.
 */
uint32_t NfcLib_MifareClassic_1k_Reference_app()
{
    uint32_t  dwStatus;
    uint8_t pValue[4] = {0x10, 0x10, 0x10, 0x10};
    uint8_t pBlock[16] = {0};

    do
    {
    /*************************************************************************************************************************
     *********************************************AUTHENTICATE ***************************************************************
     *************************************************************************************************************************/
    /* Authenticate takes the block number to authenticate which is 6 alng with the Key type as MFC_KEYA and key number as 1
     * and key version as 0X00, Key no and Key version depends upon the keystore settings, authenticate is valid for one sector
     */
    MFC_AUTHENTICATE(6, MFC_KEYA, 0x01, 0x00)
    /* To Perform the Authentication of Block 6 */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Authenticate Failed Please use the factory default card... \n");
        break;
    }

    /*************************************************************************************************************************
     ***********************************************WRITE*********************************************************************
     *************************************************************************************************************************/

    /* Write takes the block number and data to write, the block number is 6 */
    MFC_WRITE(6, &Message[0])

    /* To perform the write operation of Block 6 */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x10
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Write to the Mifare Classic Card Failed... \n");
        break;
    }

    /*************************************************************************************************************************
     ***********************************************READ**********************************************************************
     *************************************************************************************************************************/
    /* Read takes the block number to Read */
    MFC_READ(6)

    /* To perform the read operation of Block 6 that was written above*/
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Read from the Mifare Classic Card Failed... \n");
        break;
    }
    /* Has to be reset before every receive */
    wNumberofBytes =  256;

    /* To perform receive operation to get back the read data */
    dwStatus =  phNfcLib_Receive(&bDataBuffer[0],
                                &wNumberofBytes,
                                &bMoreDataAvailable
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        break;
    }
    DEBUG_PRINTF("\nRead Data from block 6 is");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);
    /*************************************************************************************************************************
     *****************************************Creating Value Block************************************************************
     *************************************************************************************************************************/

    /* Api to frame the value block output pBlock is the value block */
    framevalueblock(&pValue[0], 0x05, &pBlock[0]);

    /* Making the Block 5 as the vALUE block */
    MFC_WRITE(5, &pBlock[0])

    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x10 /* 16 bytes of data to be written for value block */
                                );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Creating Value block on Mifare Classic Failed... \n");
        break;
    }
    /*************************************************************************************************************************
     ***********************************************INCREMENT*****************************************************************
     *************************************************************************************************************************/

    /* Now increment can be perfomed over this value block */
    /* Value block to increment is 5 and then the data to be incremented */
    MFC_INCREMENT(5, &pValue[0])

    /* To perform the increment operation on block 5 */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x4 /* the value block for increment just takes 4 byte */
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Increment to the value block failed... \n");
        break;
    }

    /*************************************************************************************************************************
     ***********************************************TRANSFER******************************************************************
     *************************************************************************************************************************/
    /* Transfer takes the block number where the transfer buffer will be transmitted, this case it is 5 */
    MFC_TRANSFER(5)

    /* To perform the transfer operation to transfer data from transferred block to the block 5 */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Increment Transfer Operation failed... \n");
        break;
    }


    /*************************************************************************************************************************
     ***********************************************DECREMENT*****************************************************************
     *************************************************************************************************************************/

    /* Value block to decrement is 5 and then the data to be decremented */
    MFC_DECREMENT(5, &pValue[0])

    /* To perform the decrement operation on block 5 */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x4 /* the value block for decrement just takes 4 byte */
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Decrement Operation failed... \n");
        break;
    }

    /* Transfer takes the block number where the transfer buffer will be transmitted, this case it is 5 */
    MFC_TRANSFER(5)

    /* To perform the transfer operation to transfer data from transferred block to the block 5 */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Decrement Transfer Operation failed... \n");
        break;
    }



    /*************************************************************************************************************************
     ***********************************************RESTORE*****************************************************************
     *************************************************************************************************************************/
    /* Restore takes the block number from where the transfer buffer shalee be restored and is 5 here */
    MFC_RESTORE(5)

    /* To perform the restore operation */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0
                                );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Restore Operation failed... \n");
        break;
    }

    /* Transfer takes the block number where the transfer buffer will be transmitted, this case it is 5 */
    MFC_TRANSFER(5)

    /* To perform the transfer operation to transfer data from transferred block to the block 5 */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Restore Transfer Operation failed... \n");
        break;
    }
    }while(0);

    return dwStatus;
}

/**
 * API to generate value block
 */
void framevalueblock(uint8_t* pValue, uint8_t bAddrData, uint8_t* pBlock)
{
    pBlock[0]  = (uint8_t)pValue[0];
    pBlock[1]  = (uint8_t)pValue[1];
    pBlock[2]  = (uint8_t)pValue[2];
    pBlock[3]  = (uint8_t)pValue[3];
    pBlock[4]  = (uint8_t)~(uint8_t)pValue[0];
    pBlock[5]  = (uint8_t)~(uint8_t)pValue[1];
    pBlock[6]  = (uint8_t)~(uint8_t)pValue[2];
    pBlock[7]  = (uint8_t)~(uint8_t)pValue[3];
    pBlock[8]  = (uint8_t)pValue[0];
    pBlock[9]  = (uint8_t)pValue[1];
    pBlock[10] = (uint8_t)pValue[2];
    pBlock[11] = (uint8_t)pValue[3];
    pBlock[12] = (uint8_t)bAddrData;
    pBlock[13] = (uint8_t)~(uint8_t)bAddrData;
    pBlock[14] = (uint8_t)bAddrData;
    pBlock[15] = (uint8_t)~(uint8_t)bAddrData;
}
