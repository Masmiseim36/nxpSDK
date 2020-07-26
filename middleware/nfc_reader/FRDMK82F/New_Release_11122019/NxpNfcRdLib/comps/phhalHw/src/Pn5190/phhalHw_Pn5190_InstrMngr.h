/*
*                     Copyright 2019, NXP
*
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
* Instruction Manager routines definitions for Pn5190 HAL.
*
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHHALHW_PN5190_INSTRMNGR_H_
#define PHHALHW_PN5190_INSTRMNGR_H_


/**
 * \brief Instruction Manager's command parameter structure
 */
typedef struct
{
    uint8_t bCmd;               /**< In_Param - Command code*/
    uint8_t bQueue;             /**< In_Param - Flag to denote that Command is queuable*/
    uint8_t * pTxDataBuff;      /**< In_Param - Pointer to the Tx-Buffer*/
    uint16_t wTxDataLength;     /**< In_Param - No of Bytes in Tx-Buffer */
    uint8_t * pAddnData;        /**< In_Param - Points to additional data to be appended to Tx_Buffer;
                                     applicable for some commands like write eeprom, Exchange_RF_Data*/
    uint16_t wAddnDataLen;      /**< In_Param - Length of the additional data */
    uint8_t ** ppRxBuffer;      /**< InOut_Param - Double pointer to send/retrieve, pointer to RxData in RxBuffer.
                                     For RF data and Read_EEprom response, the pointer to buffer in which data is
                                     to be sent is sent. */
    uint16_t * pRxLength;       /**< Out_Param - Pointer to Rx-length*/
}phhalHw_InstMngr_CmdParams_t;


/**
 * \brief Performs send and receive of TLV pkts over SPI in Command and response format
 *
 * This is a blocking call and Is a single interface to Instruction Manager. It converts
 * incoming PN5190 Instructions into TLV format and queues in buffer. If queuing is not ON,
 * it sends individual TLV frames over SPI to PN5190 and waits for RF Event. Part of
 * response is read in IRQ and remaining part of response is read, after RF event is received.
 *
 * If Queuing is ON, depending upon whether Instruction is queue-able Or not, it does handling
 * of Instruction, as below:
 * Exchange_RF_Data and TransmitRFData are the individual TLV instructions i.e.
 * cannot be concatenated with any other instruction. if any instructions are queued, they
 * are sent before start queuing these.
 * If in Tx-Buffer, Instruction are already Queued, and latest instruction is not queue-able
 * then, incoming non-queue-able Instruction is appended to the Tx-buffer and all the
 * buffered Instructions are sent.
 * If multiple commands are cascaded, then response of only last transmitter TLV is returned back
 * to upper layers.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_BUFFER_OVERFLOW Internal receive buffer is too small for transmission or smaller than the response.
 * \retval #PH_ERR_ABORTED Returned when this API exits after \ref phhalHw_AsyncAbort API is called from another thread.
 * \retval #PH_ERR_INTERNAL_ERROR unexpected code flow
 * \retval #PH_ERR_INVALID_PARAMETER wrong parameter
 * \retval Instructions specific return value received from PN5190 as per Pn5190 Instruction layer doc
 *
 */
phStatus_t phhalHw_Pn5190_InstMngr_HandleCmd(
    phhalHw_Pn5190_DataParams_t *pDataParams,       /**< [In] Pointer to Pn5190 Hal layer's parameter structure. */
    phhalHw_InstMngr_CmdParams_t *pCmdParams,       /**< [In] Pointer to Instruction Manager's Cmd parameter structure. */
    uint16_t wOptions                               /**< [In] Buffering Options applicable for the RF Commands
                                                               like Exchange_RF_Data */
    );

/**
 * \brief Reads the given number of bytes into the buffer
 *
 *  IRQ line must be high, before calling this api
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_Pn5190_Read(
    phhalHw_Pn5190_DataParams_t *pDataParams,       /**< [In] Pointer to Pn5190 Hal layer's parameter structure. */
    uint8_t * pRxBuffer,                            /**< [InOut] Pointer to Buffer to which data to be read into. */
    uint16_t wRxLength                              /**< [In] Number of Bytes to be read into the Buffer. */
    );

/**
 * \brief Sends the given number of bytes over SPI to PN5190
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_Pn5190_Send(
    phhalHw_Pn5190_DataParams_t *pDataParams,       /**< [In] Pointer to Pn5190 Hal layer's parameter structure. */
    uint8_t * pTxBuffer,                            /**< [In] Pointer to Buffer containing data to be sent. */
    uint16_t wTxLength,                             /**< [In] Number of Byte to be sent from the Buffer. */
    uint8_t bCheckIRQ                               /**< [In] Check IRQ status before sending Command to Reader. */
    );

/**
 * \brief Parses the Partial Response read in ISR and reads
 *  the remaining Bytes from Front end.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_Pn5190_InstMngr_ReadRsp(
    phhalHw_Pn5190_DataParams_t *pDataParams,       /**< [In] Pointer to Pn5190 Hal layer's parameter structure. */
    phhalHw_InstMngr_CmdParams_t *pCmdParams        /**< [In] Pointer to Current Command Under-Process. */
    );

#endif /* PHHALHW_PN5190_INSTRMNGR_H_ */
