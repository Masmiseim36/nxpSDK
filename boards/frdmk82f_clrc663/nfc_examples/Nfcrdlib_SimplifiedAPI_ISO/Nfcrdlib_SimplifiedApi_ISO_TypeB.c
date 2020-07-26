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
 * Reference application file for Type B interface of Simplified API
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
 * This application is to demonstrate the usage of simplified API related to Type B
 * The application reaches to this point only after the activation of a card of Type B technology with
 * or without ISO 14443-4 compatibilty is done. Transmit api is used to perform any command exchange with the card and
 * receive api is used to get back the out data.
 */
uint32_t NfcLib_TypeB_Reference_app()
{
    uint32_t dwStatus;
    uint8_t bData[6] = {0};
    do
    {

    /*************************************************************************************************************************
     ****************************************ISO 14443-4 EXCHANGE ************************************************************
     *************************************************************************************************************************/

        /*
         * To perform ISO14443-4 layer exchange first form the command  in bData
         * this is get challenge command of ISO7816-4
         */
        bData[0] = 0x00; /* INF1 */
        bData[1] = 0x84; /* INF2 */
        bData[2] = 0x00; /* INF3 */
        bData[3] = 0x00; /* INF4 */
        bData[4] = 0x08; /* INF4 */

        phNfcLib_TransmitInput.phNfcLib_RawTransmit.pBuffer = &bData[0];

        /* Finally the command has to be sent, the command for Type B with layer 4 compatibility will be send with ISO 7816 - 4*/
        dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                    0x05  /* Length of this exchange is 5 bytes*/
                                    );


        /* The status should be success, if not break from the application */
        if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
        {
            DEBUG_PRINTF("\nGet Challenge failed");
            break;
        }

        /* This parameter has to be reset before every receive */
        wNumberofBytes = 256;

        /*The data received over the above command can be retrieved by calling the receive */
        /* wNumberofBytes first as input tells the max supported recieve size and then as out tells the actual number of data bytes received */
        /* wNumberofBytes will vary depending upon the card layout*/
        dwStatus = phNfcLib_Receive(&bDataBuffer[0],
                                    &wNumberofBytes,
                                    &bMoreDataAvailable
                                    );

        DEBUG_PRINTF("\nReply to get challenge is\n");
        phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);
        /*************************************************************************************************************************/

    }while(0);

    return dwStatus;

}
