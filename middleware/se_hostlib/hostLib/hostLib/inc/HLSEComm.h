/**
 * @file HLSEComm.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Host Lib wrapper API: Communication and Secure Channel functions
 *
 * @par HISTORY
 *
 */
#ifndef _HLSE_COMM_H_
#define _HLSE_COMM_H_

#include "HLSETypes.h"

/**
* Establishes the communication with the Secure Element accroding to the requested \p type.
* Additional parameters required for establishing the communication are passed in \p params.
*
* The physical communication layer used (e.g. SCI2C) is determined at compilation time.
*
* \param[in,out] params Additional parameters for opening the commuication
* \param[in,out] commState Points to a HLSE_COMMUNICATION_STATE which returns the communication state (e.g. ATR)
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE HLSE_Connect(HLSE_CONNECTION_PARAMS* params, HLSE_COMMUNICATION_STATE *commState);

/**
* Closes the communication with the Secure Element.
*
* \param[in] mode Specific information that may be required on the link layer
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE HLSE_CloseConnection(HLSE_CLOSE_CONNECTION_MODE mode);

/**
* Resumes the communication with the Secure Element including the secure channel from the previously
* retrieved communication state and secure channel session state.
*
* @param[in] commState   communication state
* @param[in] smState  secure channel session state
*
* @retval ::HLSE_SW_OK Upon successful execution
* @retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE HLSE_ResumeConnection(HLSE_COMMUNICATION_STATE *commState, HLSE_SECURE_CHANNEL_SESSION_STATE *smState);

/**
* Sends the command APDU to the Secure Element and retrieves the response APDU.
* The latter consists of the concatenation of the response data (possibly none) and the status word (2 bytes).
*
* The command APDU and response APDU are not interpreted by the host library.
*
* The command/response APDU sizes must lay within the APDU size limitations
*
* \param[in] cmd   command APDU
* \param[in] cmdLen length (in byte) of \p cmd
* \param[in,out] resp  response APDU (response data || response status word)
* \param[in,out] respLen IN: Length of resp buffer (\p resp) provided; OUT: effective length of response retrieved.
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE HLSE_SendAPDU(U8 *cmd, U16 cmdLen, U8 *resp, U16 *respLen);


/**
* Subscribe a HLSE_SCP_SignalFunction function to receive messages from the Secure Channel.
*
* \param[in] callback   The function
* \param[in] context Optional context information that the function is subsrcibed with and called with
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE HLSE_SCP_Subscribe(HLSE_SCP_SignalFunction callback, void *context);

/**
* Establishes a Secure Channel with the Secure Element, and when successful initializes the current Session Channel state.
*
* \param[in] params Data required to establish the Secure Channel
* \param[in, out] channelState  Returns the Secure Channel state
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE HLSE_SMChannelAuthenticate(HLSE_SECURE_CHANNEL_ESTABLISH_PARAMS* params, HLSE_SECURE_CHANNEL_STATE* channelState);

/**
* Retrieve the Secure Channel Session state from the Host Library.
*
* \param[in,out] channelSessionState IN: pointer to allocated structure; OUT: contains the session state
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE HLSE_SMChannelGetScpSessionState(HLSE_SECURE_CHANNEL_SESSION_STATE *channelSessionState);

/**
* Sets the Secure Channel Session state of the Host Library.
* Can be used in a scenario where e.g. the bootloader has established the Secure Channel link between
* host and secure element and the Host OS must re-establish the communication with the
* secure element without breaking the session.
*
* \param[in] channelSessionState Contains the session state information
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE HLSE_SMChannelSetScpSessionState(HLSE_SECURE_CHANNEL_SESSION_STATE *channelSessionState);


#endif // _HLSE_COMM_H_
