/*
 * Copyright (c) 2016 - 2018, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file sf_cmd.h
 *
 * This module implements console interface for OL2385.
 * Specifically functions for command selection and related handlers.
 */

#ifndef SOURCE_SF_CMD_H_
#define SOURCE_SF_CMD_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_common.h"
#include "sf.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Prints error code and internal OL2385 state which is updated
 *        with each ACK frame.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
void PrintDeviceState(sf_drv_data_t *sfDrvData);

/*!
 * @brief Prints message payload of the sent or received frame.
 *
 * @param msgPayload Encapsulation for Sigfox message data.
 *
 * @return None.
 */
void PrintPayload(const sf_msg_payload_t *msgPayload);

/*!
 * @brief Prints device's ID, version and library version.
 *
 * @param devInfo Encapsulation for device info data.
 *
 * @return None.
 */
void PrintDeviceInfo(const sf_device_info_t *devInfo);

/*!
 * @brief Prints available console commands.
 *
 * @return None.
 */
void PrintCommands(void);

/*!
 * @brief Prints OL2385 firmware version.
 *
 * @param sfDrvData Sigfox driver data.
 *
 * @return Status result of the function.
 */
status_t PrintFirmwareVersion(sf_drv_data_t *sfDrvData);

/*!
 * @brief Reads from stdin which is attached to uart.
 *
 * Input is limited by given length and user must press enter to
 * confirm message.
 *
 * @param str     Pointer to the memory where to store input string.
 * @param length  Maximal expected length of the input string.
 *                Without null-terminating character.
 *
 * @return Number of actually read characters.
 */
uint8_t GetUserInput(char *str, uint8_t length);

/*!
 * @brief Parses user's input to get command selection.
 *
 * @return Sigfox OL2385 command selection.
 */
uint8_t GetUserCommand(void);

/*!
 * @brief Prints status message to serial output. The message contains
 * an error and state returned by the Sigfox device and status returned by
 * Sigfox driver.
 *
 * @param sfDrvData Sigfox driver data.
 * @param execStatus Execution status returned by Sigfox driver.
 * @param sfError True when an error was reported by the Sigfox driver.
 */
void PrintStatus(sf_drv_data_t *sfDrvData, status_t execStatus, bool sfError);

/*!
 * @brief Invokes handler function of given command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 * @param cmd       Sigfox OL2385 command.
 * @param sfNetStandard Currently set networking standard.
 * @param sfError   True when an error was reported by the Sigfox driver,
 * otherwise false (for example an error due to user message processing).
 *
 * @return Status result of the function.
 */
status_t ProcessCommand(sf_drv_data_t *sfDrvData, uint8_t cmd,
        sf_net_standard_t *sfNetStandard, bool *sfError);

#endif /* SOURCE_SF_CMD_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
