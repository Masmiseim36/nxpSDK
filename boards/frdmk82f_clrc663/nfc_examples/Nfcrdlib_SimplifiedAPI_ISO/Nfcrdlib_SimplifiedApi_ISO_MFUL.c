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
 * Reference application file for MFUL interface of Simplified API
 * $Author: Ashish Pal (nxp79566) $
 * $Revision: 5458 $ (v06.01.00)
 * $Date: 2016-09-01 19:11:09 +0530 (Thu, 01 Sept 2016) $
 *
 * History:
 *
 *
 */

#include <Nfcrdlib_SimplifiedApi_ISO.h>

extern phNfcLib_Transmit_t phNfcLib_TransmitInput;
extern phNfcLib_PeerInfo_t PeerInfo;

extern uint8_t bMoreDataAvailable;
extern uint16_t wNumberofBytes;
extern uint8_t  bDataBuffer[256];


/*
 * This application is to demonstrate the usage of simplified API related to Mifare Ultralight
 * The application reaches to this point only after the activation of a Mifare Ultralight card
 * with Type A technology is done. Transmit api is used to perform any command exchange with the card and
 * receive api is used to get back the out data.
 */


uint32_t NfcLib_MifareUltralight_Reference_app()
{
    uint32_t  dwStatus;
    uint8_t bData[16] = {0xB0, 0xB0, 0xB0, 0xB0,
                        0xA0, 0xA0, 0xA0, 0xA0,
                        0xA0, 0xA0, 0xA0, 0xA0,
                        0xA0, 0xA0, 0xA0, 0xA0};

    do
    {
    /*************************************************************************************************************************
     *********************************************MFUL READ *****************************************************************
     *************************************************************************************************************************/
    /* MFUL read takes the page number to read, for refer purpose the page is page 5 */
    MFUL_READ(5)

    /* Transmit will execute the command the returned data can be read by Receive command */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length paramter is used only when apart from the command, there is some data to be send*/
                                );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Read for Block 5 failed... \n");
        break;
    }

    /* This parameter has to be reset before every receive */
    wNumberofBytes = 256;

    /* To perform receive operation to get back the read data */
    dwStatus =  phNfcLib_Receive(&bDataBuffer[0],
                                &wNumberofBytes,
                                &bMoreDataAvailable
                                );

    /* The status should be success and the number of bytes received should be 16 for ultra light cards */
    if((dwStatus != PH_NFCLIB_STATUS_SUCCESS) ||(wNumberofBytes != 16))
    {
        break;
    }
    DEBUG_PRINTF("\nRead Data from block 5 is");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);

    /*************************************************************************************************************************
     *********************************************MFUL WRITE******************************************************************
     *************************************************************************************************************************/
    /* MFUL write takes the page number and data to write, for refer purpose the page is page 5 */
    MFUL_WRITE(5, &bData[0])

    /* Transmit will execute the command */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x04  /* For Mifare Ultralight the size of a page to be written is 4 bytes */
                                );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Write to Block 5 failed... \n");
        break;
    }

    /*************************************************************************************************************************
     *********************************************MFUL COMPATIBILITY WRITE ***************************************************
     *************************************************************************************************************************/

    /* MFUL Compatibilily write takes the page number and data to write, for refer purpose the page is page 6 */
    /* the command is just to accomodate the Mifare PCD infrastructure */
    MFUL_WRITE(6, &bData[0])

    /* Transmit will execute the command */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x10 /* But only 4 LSB will be actuallly written, 16 byte is only for compatibility purpose */
                                );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Compatibilty Write to Block 5 failed... \n");
        break;
    }

    /*************************************************************************************************************************/
    }while(0);

    return dwStatus;
}


