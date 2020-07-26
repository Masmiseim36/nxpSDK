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
 * Reference application file for ISO15693 interface of Simplified API
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
 * This application is to demonstrate the usage of simplified API related to ISO15693
 * The application reaches to this point only after the activation of a card with ISO 15693
 * interface is done. Transmit api is used to perform any command exchange with the card and
 * receive api is used to get back the out data.
 * In case the user already knows the UID of the card he wants to talk to he can directly call these api
 */
uint32_t NfcLib_ISO15693_Reference_app()
{
    uint32_t  dwStatus;
    uint8_t bData[16] = {0xA0, 0xA0, 0xA0, 0xA0,
                        0xA0, 0xA0, 0xA0, 0xA0,
                        0xA0, 0xA0, 0xA0, 0xA0,
                        0xA0, 0xA0, 0xA0, 0xA0};

    /*
     * Copying the UID of the card on which the user wish to perform the operation.
     * For the reference purpose we will be talking to the card at index value 0
     */
    memcpy(phNfcLib_TransmitInput.phNfcLib_ISO15693.bUid, PeerInfo.uTi.uInitiator.tIso15693.TagIndex[0].pUid, 8);


    do
    {

    /*************************************************************************************************************************
     ****************************************READ SINGLE BLOCK ***************************************************************
     *************************************************************************************************************************/

    /* The parameters for Read Single Block are the option and the Block number user wants to read */
    /* Option is kept #PH_OFF for refer purpose and the block number to Read is Block - 5 */
    READ_SINGLEBLOCK(PH_OFF, 5)

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length paramter is used only when apart from the command, there is some data to be send*/
                                );


    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support Read single block... \n");
        break;
    }

    /* wNumberofBytes has to be reset before every receive */
    wNumberofBytes = 256;

    /*The data received over the above command can be retrieved by calling the receive */
    /* wNumberofBytes first as input tells the max supported recieve size and then as out tells the actual number of data bytes received */
    /* wNumberofBytes will vary depending upon the card layout*/
    dwStatus = phNfcLib_Receive(&bDataBuffer[0],
                                &wNumberofBytes,
                                &bMoreDataAvailable
                                );

    /* The status should be success and the number of bytes received should be 4 for Most NXP cards
     * The expectation of the number of bytes can be modified for different cards
     */
    if((dwStatus != PH_NFCLIB_STATUS_SUCCESS) ||(wNumberofBytes != 4))
    {
        break;
    }
    DEBUG_PRINTF("\nRead Data from Block 5 is");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);

    /*************************************************************************************************************************
     ****************************************WRITE SINGLE BLOCK **************************************************************
     *************************************************************************************************************************/

    /* The parameters for Write Single Block are the option, the Block number user wants to write and the data */
    /* Option is kept #PH_OFF for refer purpose and the block number to write is Block - 5 */

    WRITE_SINGLEBLOCK(PH_OFF, 5, &bData[0])

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x04  /* While writing a block we will be writing 4 bytes of data*/
                                );


    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support Write single block... \n");
        break;
    }

    /*************************************************************************************************************************
     ****************************************READ MULTIPLE BLOCK **************************************************************
     *************************************************************************************************************************/

    /* The parameters for Read Multiple Block are the option, the starting Block number and the no of blocks to read*/
    /* Option is kept #PH_OFF for refer purpose, the starting block  number is 5 and the no of blocks is 4*/
    READ_MULTIPLEBLOCK(PH_OFF, 5, 4)

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length paramter is used only when apart from the command, there is some data to be send*/
                                );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support Read Multiple block... \n");
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

    /* The status should be success and the number of bytes received should be 4 * numofblocks for Most NXP cards
     * The expectation of the number of bytes can be modified for different cards
     */
    if((dwStatus != PH_NFCLIB_STATUS_SUCCESS) ||(wNumberofBytes != 16))
    {
        break;
    }
    DEBUG_PRINTF("\nRead Data from Block 5  to 8 is");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);

    /*************************************************************************************************************************
     ****************************************LOCK BLOCK **********************************************************************
     *************************************************************************************************************************/

    /* The parameters for Lock Block are the option and blocks to lock*/
    /* Option is kept #PH_ON for refer purpose, and the block number is 9*/

    LOCK_BLOCK(PH_ON, 9)

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                 );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support Lock block or block is already locked... \n");
        break;
    }

    /*************************************************************************************************************************
     ***************************************WRITE AFI *************************************************************
     *************************************************************************************************************************/

    /* The parameters for Write AFI are the option and AFI to write*/
    /* Option is kept #PH_ON for refer purpose, and the AFI is for Identification*/

    WRITE_AFI(PH_ON, 0x70)

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                 );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support Write AFI or AFI is locked... \n");
        break;
    }

    /*************************************************************************************************************************
     ***********************************************LOCK AFI *************************************************************
     *************************************************************************************************************************/

    /* The parameters for Lock AFI is the option with which to Lock the AFI*/
    /* Option is kept #PH_ON for refer purpose to get back a reply*/

    LOCK_AFI(PH_ON)

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                 );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support Lock AFI or AFI is already locked... \n");
        break;
    }

    /*************************************************************************************************************************
     ***********************************************WRITE DSFID *************************************************************
     *************************************************************************************************************************/

    /* The parameters for Write DSFID are the option and DSFID to write*/
    /* Option is kept #PH_ON for refer purpose to get back a reply and DSFID is 4*/

    WRITE_DSFID(PH_ON, 4)

    /* Thus this command can be called just after the  user has the UID of the card he wishes to talk to  */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                 );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support Write DSFID or DSFID is locked... \n");
        break;
    }

    /*************************************************************************************************************************
     ***********************************************LOCK DSFID *************************************************************
     *************************************************************************************************************************/

    /* The parameters for Lock DSFID is the option with which to Lock the Dsfid*/
    /* Option is kept #PH_ON for refer purpose to get back a reply*/

    LOCK_DSFID(PH_ON)

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                 );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support Lock DSFID or DSFID is already locked... \n");
        break;
    }

    /*************************************************************************************************************************
     *****************************************GET SYSTEM INFORMATION *********************************************************
     *************************************************************************************************************************/

    /* Get System Information takes no parameter and returns back the System Information */
    GET_SYSTEMINFORMATION()

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                 );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support GET SYSTEM INFORMATION... \n");
        break;
    }

    /* This parameter has to be reset before every receive */
    wNumberofBytes = 256;

    /*The System Information  received over the above command can be retrieved by calling the receive */
    /* wNumberofBytes first as input tells the max supported recieve size and then as out tells the actual number of data bytes received */
    dwStatus = phNfcLib_Receive(&bDataBuffer[0],
                &wNumberofBytes,
                &bMoreDataAvailable
                );

    /* The status should be success */

    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        break;
    }
    DEBUG_PRINTF("\nSystem Information is ");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);

    /*************************************************************************************************************************
     ************************************GET MULTIPLE BLOCK SECURITY STATUS **************************************************
     *************************************************************************************************************************/

    /* The parameters for Get Multiple Block security Status are the starting block nad the no of blocks whose status to read*/
    /* Starting block is 9 which is already locked and no of blocks is 4 */

    GET_MULTIPLEBLOCKSECURITYSTATUS(9, 4)

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                 );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support GET MULTIPLE BLOCK SECURITY STATUS... \n");
        break;
    }

    /* This parameter has to be reset before every receive */
    wNumberofBytes = 256;

    /*The System Information received over the above command can be retrieved by calling the receive */
    /* wNumberofBytes first as input tells the max supported recieve size and then as out tells the actual number of data bytes received */
    dwStatus = phNfcLib_Receive(&bDataBuffer[0],
                &wNumberofBytes,
                &bMoreDataAvailable
                );

    /* The status should be success */

    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        break;
    }
    DEBUG_PRINTF("\nSecurity status for block 9 to 12  is ");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);

    /*************************************************************************************************************************
     ****************************************WRITE MULTIPLE BLOCK *************************************************************
     *************************************************************************************************************************/

    /* The parameters for Write Multiple Block are the option, the starting Block number and the no of blocks to write*/
    /* Option is kept #PH_OFF for refer purpose, the startting block  number is 5 and the no of blocks is 4*/

    WRITE_MULTIPLEBLOCK(PH_OFF, 5, 4, &bData[0])

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x10  /* Since we are writing 4 blocks thus we are sending 4*4 = 16 bytes of data*/
                                );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support Write Multiple block... \n");
        break;
    }

    /** From here onwards are the extended commands, the extended commands are supported by the tags with more than 0xFF blocks
     * since we do not have such cards thus we will not be able to test these commands
     */

    /*************************************************************************************************************************
     ************************************EXTENDED GET SYSTEM INFORMATION *****************************************************
     *************************************************************************************************************************/

    /* Extended Get System Information takes no parameter and returns back the System Information for Extended Cards*/

    EXTENDEDGET_SYSTEMINFORMATION()

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                 );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support EXTENDED GET SYSTEM INFORMATION... \n");
        break;
    }

    /* This parameter has to be reset before every receive */
    wNumberofBytes = 256;

    /*The System Information received over the above command can be retrieved by calling the receive */
    /* wNumberofBytes first as input tells the max supported recieve size and then as out tells the actual number of data bytes received */
    dwStatus = phNfcLib_Receive(&bDataBuffer[0],
                &wNumberofBytes,
                &bMoreDataAvailable
                );

    /* The status should be success */

    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        break;
    }
    DEBUG_PRINTF("\nSystem Information for extended support card is ");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);

    /*************************************************************************************************************************
     ********************************EXTENDED READ SINGLE BLOCK *************************************************************
     *************************************************************************************************************************/

    /* The parameters for Extended Read Single Block are the option and the Block number user wants to read */
    /* Option is kept #PH_OFF for refer purpose and the block number to Read is Block - 0x105 */

    EXTENDEDREAD_SINGLEBLOCK(PH_OFF, 0x105)

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                        0x0  /* This length paramter is used only when apart from the command, there is some data to be send*/
                        );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support EXTENDED READ SINGLE BLOCK... \n");
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

    /* The status should be success and the number of bytes received should be 4 for Most NXP cards
    * The expectation of the number of bytes can be modified for different cards
    */
    if((dwStatus != PH_NFCLIB_STATUS_SUCCESS) ||(wNumberofBytes != 4))
    {
        break;
    }
    DEBUG_PRINTF("\nRead Data from Block 261 is");
    phApp_Print_Buff(&bDataBuffer[0], 4);

    /*************************************************************************************************************************
     ********************************EXTENDED WRITE SINGLE BLOCK *************************************************************
     *************************************************************************************************************************/

    /* The parameters for Extended Write Single Block are the option, the Block number user wants to write and the data */
    /* Option is kept #PH_OFF for refer purpose and the block number to write is Block - 0x105 */
    EXTENDEDWRITE_SINGLEBLOCK(PH_OFF, 0x105, &bData[0])

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                        0x04  /* For NXP Cards we will write only 4 bytes at a time, for different vendors length may change*/
                        );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support EXTENDED WRITE SINGLE BLOCK... \n");
        break;
    }


    /*************************************************************************************************************************
     ********************************EXTENDED READ MULTIPLE BLOCK *************************************************************
     *************************************************************************************************************************/

    /* The parameters for Extended Read Multiple Block are the option, the starting Block number and the no of blocks to read*/
    /* Option is kept #PH_OFF for refer purpose, the startting block  number is 0x105 and the no of blocks is 4*/

    EXTENDEDREAD_MULTIPLEBLOCK(PH_OFF, 0x105, 4)

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                               0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support EXTENDED READ MULTIPLE BLOCK... \n");
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

    /* The status should be success and the number of bytes received should be 4 for Most NXP cards
    * The expectation of the number of bytes can be modified for different cards
    */
    if((dwStatus != PH_NFCLIB_STATUS_SUCCESS) ||(wNumberofBytes != 16))
    {
        break;
    }
    DEBUG_PRINTF("\nRead Data from Block 261 to 264 is");
    phApp_Print_Buff(&bDataBuffer[0], 16);
    /*************************************************************************************************************************
     ********************************EXTENDED WRITE MULTIPLE BLOCK *************************************************************
     *************************************************************************************************************************/

    /* The parameters for Extended Write Multiple Block are the option, the starting Block number and the no of blocks to write*/
    /* Option is kept #PH_OFF for refer purpose, the startting block  number is 5 and the no of blocks is 4*/

    EXTENDEDWRITE_MULTIPLEBLOCK(PH_OFF, 0x105, 4,  &bData[0])

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                        0x10  /* For NXP Cards we will write only 4 bytes at a time, for different vendors length may change*/
                        );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support EXTENDED WRITE MULTIPLE BLOCK... \n");
        break;
    }

    /*************************************************************************************************************************
     ****************************************EXTENDED LOCK BLOCK *************************************************************
     *************************************************************************************************************************/

    /* The parameters for Extended Lock Block are the option and blocks to lock*/
    /* Option is kept #PH_ON for refer purpose, and the block number is 0x109*/

    EXTENDEDLOCK_BLOCK(PH_ON, 0x109)

    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                 );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support EXTENDED LOCK BLOCK or block is already locked... \n");
        break;
    }

    /*************************************************************************************************************************
     *********************************EXTENDED GET MULTIPLE BLOCK SECURITY STATUS *********************************************
     *************************************************************************************************************************/

    /* The parameters for Extended Get Multiple Block security Status are the starting block nad the no of blocks whose status to read*/
    /* Starting block is 0x109 which is already locked and no of blocks is 4 */

    EXTENDEDGET_MULTIPLEBLOCKSECURITYSTATUS(0x109, 0x04)
    /* Finally the command has to be sent, the command for 15693 will be send in addressed mode */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length parameter is used only when apart from the command, there is some data to be send*/
                                 );

    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Card does not support EXTENDED GET MULTIPLE BLOCK SECURITY STATUS... \n");
        break;
    }

    /* This parameter has to be reset before every receive */
    wNumberofBytes = 256;

    /*The Security Information received over the above command can be retrieved by calling the receive */
    /* wNumberofBytes first as input tells the max supported recieve size and then as out tells the actual number of data bytes received */
    dwStatus = phNfcLib_Receive(&bDataBuffer[0],
                &wNumberofBytes,
                &bMoreDataAvailable
                );

    /* The status should be success */

    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        break;
    }
    DEBUG_PRINTF("\nSecurity status for multiple block is");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);
    /*************************************************************************************************************************/

    }while(0);

    return dwStatus;
}
