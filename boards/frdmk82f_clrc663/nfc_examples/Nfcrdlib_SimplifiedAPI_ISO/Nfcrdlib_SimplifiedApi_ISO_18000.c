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
 * Reference application file for ISO18000 interface of Simplified API
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

#define NFCLIB_I18000P3M3_REQRN_USE_HANDLE    0x01U   /**< Use given Handle for ReqRn command. */

#define NFCLIB_I18000P3M3_AC_NO_COVER_CODING  0x00U   /**< Do not use cover coding, send plain passwords. */
#define NFCLIB_I18000P3M3_AC_USE_COVER_CODING 0x01U   /**< Use cover coding to diversify passwords. */

uint32_t NfcLib_ISO18000p3m3_Reference_app()
{
    uint32_t dwStatus;
    uint8_t bWordPtr = 0x00;
    uint8_t bMask[2] = {0x00, 0x00};
    uint8_t bData[8] = {0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A};
    uint8_t bLockMask[2] = {0x03, 0x00};
    uint8_t bPassword[4] = {0x00, 0x00, 0x00, 0x00}; /* Default Paasword */
    uint8_t bLockAction[2] = {0x03, 0x00};
    uint8_t bHandle[2] = {0x00, 0x00}; /* Invalid Handle */

    phNfcLib_TransmitInput.phNfcLib_ISO18000.wUiiMaskLength = PeerInfo.uTi.uInitiator.tIso18000_3_3.TagIndex[0].wUiiLength;
    phNfcLib_TransmitInput.phNfcLib_ISO18000.pUii = PeerInfo.uTi.uInitiator.tIso18000_3_3.TagIndex[0].pUii;

    do
    {
    /*************************************************************************************************************************
     **********************************************************ACK************************************************************
     *************************************************************************************************************************/
    /* ACK takes no parameter and sends the ack with the Handle */
    I18000_ACK()
    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send */
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Performing Ack Operation failed... \n");
        break;
    }
    /* This parameter has to be reset before every receive */
    wNumberofBytes = 256;

    /*The data received over the above command can be retrieved by calling the receive */
    /* wNumberofBytes first as input tells the max supported recieve size and then as out tells the actual number of data bytes received */
    dwStatus = phNfcLib_Receive(&bDataBuffer[0],
                                &wNumberofBytes,
                                &bMoreDataAvailable
                                );
    DEBUG_PRINTF("\nUII Data Received is");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);

    /*************************************************************************************************************************
     ********************************************************WRITE************************************************************
     *************************************************************************************************************************/

    /* Write is performed with Using cover coding on user membank(0x03) from the start of membank */
    I18000_WRITE(NFCLIB_I18000P3M3_AC_USE_COVER_CODING, 0x03, &bWordPtr, 0x00, &bData[0])

    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x2  /* A word is of 2 bytes*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Performing Write Operation to user Membank failed, Membank may be locked.. \n");
        break;
    }
    /*************************************************************************************************************************
     *******************************************************READ**************************************************************
     *************************************************************************************************************************/

    /* READ wants to perform Read on Membank 0x03 which is a user memory, from the start of the user memory thus ptr and ptr length
     * as 0x00 and total no of words as 1
     */
    I18000_READ(0x03, &bWordPtr, 0x00, 0x01)


    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length paramter is used only when apart from the command, there is some data to be send*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Read Operation of User Membank failed, Membank may be locked... \n");
        break;
    }

    /* This parameter has to be reset before every receive */
    wNumberofBytes = 256;

    /*The data received over the above command can be retrieved by calling the receive will be same as above write */
    /* wNumberofBytes first as input tells the max supported recieve size and then as out tells the actual number of data bytes received */
    dwStatus = phNfcLib_Receive(&bDataBuffer[0],
                                &wNumberofBytes,
                                &bMoreDataAvailable
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        break;
    }
    DEBUG_PRINTF("\nData Read from the ISO File is");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);
    /*************************************************************************************************************************
     ******************************************************BLOCK WRITE*********************************************************
     *************************************************************************************************************************/
    /* Block write is a optional command please check the card's spec regarding support */
    /* BLOCK_WRITE is being performed on User Membank(0x03), from the start of the Mmebank and for ICode ILT-M one block is 2 words */
    I18000_BLOCKWRITE(0x03, &bWordPtr, 0x00, 2,  &bData[0])


    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x4  /* Since ICode ILT-M supports maximum 2 word block write*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF("\nCard Does not support Block Write");
        break;
    }

    /*************************************************************************************************************************
     ********************************************************ACCESS************************************************************
     *************************************************************************************************************************/
    /* Access is performed with cover coding and default password */
    I18000_ACCESS(PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING, &bPassword[0])


    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF("\nAccess Operation Failed, use factory default card");
        break;
    }

    /*************************************************************************************************************************
     ********************************************************LOCK*************************************************************
     *************************************************************************************************************************/
    /* Lock is performed based on the configuration of the Mask and Action please refer to ISO18000-3M3 */
    I18000_LOCK(&bLockMask[0], &bLockAction[0])

    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF("\nLock Operation Failed, use factory default card");
        break;
    }
    /*************************************************************************************************************************
     *****************************************************BLOCK PERMALOCK*****************************************************
     *************************************************************************************************************************/
    /* Block Permalock is a optional command please check the card's spec regarding support */
    /* Block permalock is performed with RFU as 0, LOCK as 1 from start of  USER membank
     * with a block range of 1 means (16 * block range) - 1 blocks and {0x00 , 0x00} mask */
    I18000_BLOCKPERMALOCK(0x00, 0x01, 0x03, &bWordPtr, 0x00, 0x01, &bMask[0])

    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF("\nCard does not support Block Permalock");
        break;
    }
    /*************************************************************************************************************************
     *******************************************************BLOCK ERASE*******************************************************
     *************************************************************************************************************************/
    /* Block write is a optional command please check the card's spec regarding support */
    /* BLOCK_WRITE is being performed on User Membank(0x03), from the start of the Mmebank and for ICode ILT-M one block is 2 words */
    I18000_BLOCKERASE(0x03, &bWordPtr, 0x00, 2)


    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF("\nCard Does not support Block Erase");
        break;
    }
    /*************************************************************************************************************************
     ********************************************************KILL*************************************************************
     *************************************************************************************************************************/
    /* Kill is mandatory for bReCom AS 0x04 and rest is optional thus here it is performed with no cover coding and default password */
    I18000_KILL(PH_NFCLIB_18000P3M3_AC_NO_COVER_CODING, &bPassword[0], 0x04)

    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );

    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF("\nKill Operation Failed, Kill password does not match");
        break;
    }
    /*************************************************************************************************************************
     ******************************************************REQ RN*************************************************************
     *************************************************************************************************************************/

    /* REQRN can here be performed only with handle option */
    I18000_REQRN(NFCLIB_I18000P3M3_REQRN_USE_HANDLE)

    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0   /* No data to be send apart from command*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF("\nReqRN failed");
        break;
    }
    /*************************************************************************************************************************
     ******************************************************SET HANDLE**********************************************************
     *************************************************************************************************************************/
    /* Set Handle is to provide the handle to the data params */
    I18000_SETHANDLE(&bHandle[0])

    /* Finally the command has to be sent */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0   /* No data to be send apart from command*/
                                );


    /**************************************************************************************************************************/
    }while(0);

    return dwStatus;
}

