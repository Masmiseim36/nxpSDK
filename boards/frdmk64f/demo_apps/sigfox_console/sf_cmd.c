/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file sf_cmd.c
 *
 * This module implements console interface for OL2385.
 * Specifically functions for command selection and related handlers.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
/* Standard libraries */
#include "stdlib.h"
#include "ctype.h"
#include "errno.h"
#include "stdio.h"
/* SDK */
#include "fsl_common.h"
#include "fsl_debug_console.h"
/* AML */
#include "common_aml.h"
/* Sigfox */
#include "sf.h"
#include "sf_cmd.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
/*! Number of available commands for OL2385. */
#define SF_CMD_NUM 18U
/*! Format of command list (<cmd id>. <cmd name>). */
#define CMD_STR_FORMAT              " %2X. %-30s"
/*! Message for the user to put a number. */
#define USER_MSG_INV_INT_INPUT      "\tEnter valid number in range "
/* 10 characters for digits, 4 characters for spaces (inserted after 2/3/4 digits),
 * 1 character for sign and 1 null character. */
#define INT32_DEC_DIGITS_SPACES    (10U + 4U + 1U + 1U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Prints two command names with their code as one line.
 *
 * @param firstCmdId Code of the first command.
 * @param firstCmd Name of the first command.
 * @param secCmdId Code of the second command.
 * @param secCmd Name of the second command.
 */
static void printTwoCmds(uint8_t firstCmdId, const char* firstCmd,
        uint8_t secCmdId, const char *secCmd);

/*!
 * @brief Prints a command challenge message.
 */
static void PrintCommandPrompt(void);

/*!
 * @brief Reads user's input from serial console, checks and converts it to
 * signed 32 bit number.
 *
 * @param intRes Read number.
 *
 * @return True on success, otherwise false.
 */
static bool GetUserInputInt(int32_t *intRes);

/*!
 * @brief Reads user's input from serial console, checks and converts it to
 * unsigned 32 bit number.
 *
 * @param intRes Read number.
 * @param base   Expected number base (10 and 16 supported).
 *
 * @return True on success, otherwise false.
 */
static bool GetUserInputUnsigned(uint32_t *intRes, uint8_t base);

/*!
 * @brief Prints a message along with a range defined by parameters to serial
 * output.
 *
 * Printed message is a challenge for the user to put a number in defined range.
 * Min. and max. are unsigned.
 *
 * @param msg Message to printed.
 * @param min Min. value.
 * @param max Max. value.
 * @param base Desired numbers base (min, max). Supported values: 10, 16.
 */
static void PrintMsgRangeUint(const char *msg, uint32_t min, uint32_t max,
        uint8_t base);

/*!
 * @brief Prints a message along with a range defined by parameters to serial
 * output.
 *
 * Printed message is a challenge for the user to put a number in defined range.
 * Min. and max. are signed.
 *
 * @param msg Message to printed.
 * @param min Min. value.
 * @param max Max. value.
 */
static void PrintMsgRangeInt(const char *msg, int32_t min, int32_t max);

/*!
 * @brief Moves a character pointed by srcPtr to dstPtr. It inserts a space
 * after specified number of characters (charsBeforeSpace).
 *
 * @param srcPtr Pointer to a character that should be moved.
 * @param dstPtr Pointer to the position where the character determined by srcPtr
 * should be moved. Note that a space can be inserted after the character
 * (dstPtr - 1).
 * @param pos Position of the character (srcPtr) in a string (starting from 0).
 * @param charsBeforeSpace Space is inserted after a specified number of characters.
 */
static void MoveDigitChar(char **srcPtr, char **dstPtr, uint8_t pos, uint8_t numLen,
        uint8_t charsBeforeSpace);

/*!
 * @brief Puts a space after defined number of characters (charsBeforeSpace).
 * It operates in situ (regarding resulting string), so the string (strNumRes)
 * must have enough space (sign + numLen + number of spaces + null character).
 *
 * @param strNumRes String where the number is stored in ASCII. This string is
 * updated and the new string replaces the old one.
 * @param numLen    Number of characters of the strNumRes excluding null character.
 * @param strNumResLen Length of the strNumRes array. It is used for overflow check.
 * @param charsBeforeSpace Space is inserted after a specified number of characters.
 *
 * @return Pointer to resulting string.
 */
static char *GetFormattedNum(char *strNumRes, uint8_t numLen, uint8_t strNumResLen,
        uint8_t charsBeforeSpace);

/*!
 * @brief Converts a signed number to string and insert a space after 3 digits.
 * Appends a null character at the end.
 *
 * @param number Number to be converted.
 * @param strRes Resulting string.
 * @param strResLen Length of an array storing resulting string.
 *
 * @return Pointer to the beginning of resulting string.
 */
static char *GetFormattedInt(int32_t number, char *strRes, uint8_t strResLen);

/*!
 * @brief Converts an unsigned number to string and insert a space after 3
 * or 4 digits (3 for decimal representation, 4 for hexadecimal). Appends a null
 * character at the end.
 *
 * @param number Number to be converted.
 * @param base   Desired base used for conversion. Allowed values: 10, 16.
 * @param strRes Resulting string.
 * @param strResLen Length of an array storing resulting string.
 *
 * @return Pointer to the beginning of resulting string.
 */
static char *GetFormattedUnsigned(uint32_t number, uint8_t base, char *strRes,
        uint8_t strResLen);

/*!
 * @brief Reads user's input from serial input, converts it to signed number
 * and checks if the number is in defined range.
 *
 * It prints a new challenge message when input string is not valid and continues
 * until the user puts a correct value.
 *
 * @param intMin Min. value of the number.
 * @param intMax Max. value of the number.
 *
 * @return Number read from serial input.
 */
static int32_t GetUserInputIntValid(int32_t intMin, int32_t intMax);

/*!
 * @brief Reads user's input from serial input, converts it to unsigned number
 * and checks if the number is in defined range.
 *
 * It prints a new challenge message when input string is not valid and continues
 * until the user puts a correct value.
 *
 * @param intMin Min. value of the number.
 * @param intMax Max. value of the number.
 * @param intBase Expected number base.
 *
 * @return Number read from serial input.
 */
static uint32_t GetUserInputUnsignedValid(uint32_t intMin, uint32_t intMax,
        uint8_t intBase);

/*!
 * @brief Handler function for WakeUp command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleWakeUp(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for Echo command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleEcho(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for Sleep command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleSleep(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for SendPayload command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleSendPayload(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for SendBit command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleSendBit(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for SendOutOfBand command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleSendOoB(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for ReceiveFrame command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleRcvFrame(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for GetInfo command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleGetInfo(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for Set UL/DL frequency command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 * @param getFreqCmd Command code.
 *
 * @return Status result of the function.
 */
static status_t HandleSetFreq(sf_drv_data_t *sfDrvData, sf_spi_cmd_t setFreqCmd);

/*!
 * @brief Handler function for Get UL/DL frequency command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 * @param getFreqCmd Command code.
 *
 * @return Status result of the function.
 */
static status_t HandleGetFreq(sf_drv_data_t *sfDrvData, sf_spi_cmd_t getFreqCmd);

/*!
 * @brief Handler function for ContinuousWave command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleContWave(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for GetDeviceVersion command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleGetDevVer(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for TriggerWd command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleTrigWd(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for TestMode command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleTestMode(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for Change to RCZx commands.
 *
 * @param sfDrvData Sigfox driver run-time data.
 * @param netStandard Desired networking standard.
 *
 * @return Status result of the function.
 */
static status_t HandleChangeNetStandard(sf_drv_data_t *sfDrvData,
        sf_net_standard_t netStandard);

/*!
 * @brief Handler function for PSK ASK zero/AAAA commands.
 *
 * @param sfDrvData Sigfox driver run-time data.
 * @param pskAskSeqCmd Command code.
 *
 * @return Status result of the function.
 */
static status_t HandleTransmitPskAskSeq(sf_drv_data_t *sfDrvData,
        sf_spi_cmd_t pskAskSeqCmd);

/*!
 * @brief Handler function for Set FCC macro channel command.
 * This command is available for RCZ2 only.
 *
 * @param sfDrvData Sigfox driver run-time data.
 * @param netStandard Currently set networking standard. It is used for checking.
 *
 * @return Status result of the function.
 */
static status_t HandleSetFCCMacroChannel(sf_drv_data_t *sfDrvData,
        sf_net_standard_t netStandard);

/*!
 * @brief Handler function for Static frequency calibration command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleStaticFreqCal(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for Temperature frequency calibration command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleTempFreqCal(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for Get last sent RSSI command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleGetLastSentRSSI(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for Update PA curve command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleUpdatePACurve(sf_drv_data_t *sfDrvData);

/*!
 * @brief Handler function for Initial RSSI calibration command.
 *
 * @param sfDrvData Sigfox driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t HandleInitialRSSICal(sf_drv_data_t *sfDrvData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void PrintDeviceState(sf_drv_data_t *sfDrvData)
{
    AML_ASSERT(sfDrvData != NULL);

    PRINTF("\tError code from ACK:   %u\r\n", sfDrvData->errorCode);
    PRINTF("\tDevice state from ACK: %u\r\n", sfDrvData->devState);
}

void PrintDeviceInfo(const sf_device_info_t *devInfo)
{
    AML_ASSERT(devInfo != NULL);

    PRINTF("\tDevice ID:\t0x%08x\r\n", devInfo->devId);

    PRINTF("\tPAC = 0x");
    PRINTF("%02x%02x ", devInfo->devPac[7], devInfo->devPac[6]);
    PRINTF("%02x%02x ", devInfo->devPac[5], devInfo->devPac[4]);
    PRINTF("%02x%02x ", devInfo->devPac[3], devInfo->devPac[2]);
    PRINTF("%02x%02x\r\n", devInfo->devPac[1], devInfo->devPac[0]);

    PRINTF("\tDevice version = 0x");
    PRINTF("%02x%02x ", devInfo->devVersion[6], devInfo->devVersion[5]);
    PRINTF("%02x%02x ", devInfo->devVersion[4], devInfo->devVersion[3]);
    PRINTF("%02x%02x ", devInfo->devVersion[2], devInfo->devVersion[1]);
    PRINTF("%02x\r\n", devInfo->devVersion[0]);

    PRINTF("\tLibrary version = 0x");
    PRINTF("%02x%02x ", devInfo->libVersion[10], devInfo->libVersion[9]);
    PRINTF("%02x%02x ", devInfo->libVersion[8], devInfo->libVersion[7]);
    PRINTF("%02x%02x ", devInfo->libVersion[6], devInfo->libVersion[5]);
    PRINTF("%02x%02x ", devInfo->libVersion[4], devInfo->libVersion[3]);
    PRINTF("%02x%02x ", devInfo->libVersion[2], devInfo->libVersion[1]);
    PRINTF("%02x\r\n", devInfo->libVersion[0]);
}

static void printTwoCmds(uint8_t firstCmdId, const char* firstCmd,
        uint8_t secCmdId, const char *secCmd) {
    PRINTF(CMD_STR_FORMAT " |", firstCmdId, firstCmd);
    PRINTF(CMD_STR_FORMAT "\r\n", secCmdId, secCmd);
}

void PrintCommands(void)
{
    PRINTF("****************************************************************************\r\n");
    printTwoCmds(1U, "Send_Wakeup", 23U, "Change to RCZ3");
    printTwoCmds(2U, "Echo_Command", 24U, "Change to RCZ4");
    printTwoCmds(3U, "Send_To_Sleep", 25U, "Switch_to_Private_Key");
    printTwoCmds(4U, "Send_Payload", 26U, "Switch_to_Public_Key");
    printTwoCmds(5U, "Send_bit", 27U, "PSK_ASK_Zero_Sequence");
    printTwoCmds(6U, "Send_Out_of_Band", 28U, "PSK_ASK_AAAA_Sequence");
    printTwoCmds(7U, "Receive_Frame", 29U, "Set_DL_frequency");
    printTwoCmds(8U, "Get_info", 30U, "Get_DL_frequency");
    printTwoCmds(9U, "Set_UL_frequency", 31U, "Set_FCC_Macro_Channel");
    printTwoCmds(10U, "Get_UL_frequency", 32U, "Get_FCC_Macro_Channel_Usage");
    printTwoCmds(11U, "Continuous_Wave", 33U, "Reset_FCC_Macro_Channel");
    printTwoCmds(13U, "Get_Device_Version", 34U, "Static_Frequency_Calibration");
    printTwoCmds(18U, "Trigger_Watchdog", 35U, "Temperature_Frequency_Calibration");
    printTwoCmds(20U, "Send_Test_Mode", 36U, "Get_Last_Sent_RSSI");
    printTwoCmds(21U, "Change to RCZ1", 37U, "Update_PA_Curve");
    printTwoCmds(22U, "Change to RCZ2", 38U, "Initial_RSSI_Calibration");
    PRINTF("****************************************************************************\r\n");
}

static void PrintCommandPrompt(void)
{
    PRINTF("Choose a command to be sent and press enter ('?' for help): ");
}

uint8_t GetUserInput(char *str, uint8_t length)
{
    char c = 0;
    uint8_t read = 0;

    while ((c != '\r') && (c != '\n') && (read < length))
    {
        c = GETCHAR();
        PUTCHAR(c);
        if ((c != '\r') && (c != '\n'))
        {
            str[read] = c;
            read++;
        }
    }

    if (read == length)
    {   /* Read the rest of input string. */
        while ((c != '\r') && (c != '\n'))
        {
            c = GETCHAR();
        }
    }

    PRINTF("\r\n");

    str[read] = '\0';

    return read;
}

static bool GetUserInputInt(int32_t *intRes)
{
    char userInputStr[12] = {0};
    char *parseEndPtr = NULL;
    uint8_t readLen = 0U;
    bool success = false;

    readLen = GetUserInput(userInputStr, 11);

    if (readLen > 0U)
    {
        errno = 0;
        *intRes = strtol(userInputStr, &parseEndPtr, 10);

        /* The whole string must be parsed successfully. */
        success = (parseEndPtr != userInputStr) && (*parseEndPtr == '\0') &&
                (ERANGE != errno);
    }

    return success;
}

static bool GetUserInputUnsigned(uint32_t *intRes, uint8_t base)
{
    char userInputStr[12] = {0};
    char *parseEndPtr = NULL;
    uint8_t readLen = 0U;
    bool success = false;

    readLen = GetUserInput(userInputStr, 11);

    if (readLen > 0U)
    {
        errno = 0;
        *intRes = strtoul(userInputStr, &parseEndPtr, base);

        /* The whole string must be parsed successfully. */
        success = (parseEndPtr != userInputStr) && (*parseEndPtr == '\0') &&
                (ERANGE != errno);
    }

    return success;
}

static void PrintMsgRangeUint(const char *msg, uint32_t min, uint32_t max,
        uint8_t base)
{
    char intMinWithSpaces[INT32_DEC_DIGITS_SPACES] = {0};
    char intMaxWithSpaces[INT32_DEC_DIGITS_SPACES] = {0};

    PRINTF("%s <%s; %s>: ", msg,
            GetFormattedUnsigned(min, base, intMinWithSpaces, INT32_DEC_DIGITS_SPACES),
            GetFormattedUnsigned(max, base, intMaxWithSpaces, INT32_DEC_DIGITS_SPACES));
}

static void PrintMsgRangeInt(const char *msg, int32_t min, int32_t max)
{
    char intMinWithSpaces[INT32_DEC_DIGITS_SPACES] = {0};
    char intMaxWithSpaces[INT32_DEC_DIGITS_SPACES] = {0};

    PRINTF("%s <%s; %s>: ", msg,
            GetFormattedInt(min, intMinWithSpaces, INT32_DEC_DIGITS_SPACES),
            GetFormattedInt(max, intMaxWithSpaces, INT32_DEC_DIGITS_SPACES));
}

static void MoveDigitChar(char **srcPtr, char **dstPtr, uint8_t pos, uint8_t numLen,
        uint8_t charsBeforeSpace)
{
    /* Move a character to new position. */
    **dstPtr = **srcPtr;
    /* Set pointers to next position. */
    (*dstPtr)--;
    (*srcPtr)--;

    /* Insert space after specified number of digits. */
    if (((pos % charsBeforeSpace) == 0U) && (pos < numLen))
    {
        **dstPtr = ' ';
        (*dstPtr)--;
    }
}

static char *GetFormattedNum(char *strNumRes, uint8_t numLen, uint8_t strNumResLen,
        uint8_t charsBeforeSpace)
{
    uint8_t i = 0U;
    uint8_t numSpaces = 0U;
    char *dstPtr = NULL;
    char *srcPtr = NULL;

    AML_ASSERT((charsBeforeSpace >= 2U) && (charsBeforeSpace <= 4U));
    AML_ASSERT(0U < numLen);
    AML_ASSERT(0U < strNumResLen);
    AML_ASSERT(NULL != strNumRes);

    numSpaces = numLen / charsBeforeSpace;
    numSpaces = ((numLen % charsBeforeSpace) == 0U) ? numSpaces - 1U : numSpaces;
    AML_ASSERT((numSpaces + numLen) < strNumResLen);

    if (((numSpaces + numLen) < strNumResLen) && (0U < numSpaces))
    {
        dstPtr = strNumRes + (numSpaces + (numLen - 1U));
        srcPtr = strNumRes + (numLen - 1U);

        for (i = 1U; i <= numLen; i++)
        {
            MoveDigitChar(&srcPtr, &dstPtr, i, numLen, charsBeforeSpace);
        }
    }

    return strNumRes;
}

static char *GetFormattedInt(int32_t number, char *strRes, uint8_t strResLen)
{
    int numLen = 0;
    char *strResTemp = strRes;

    numLen = snprintf(strRes, strResLen, "%ld", number);

    if ((numLen >= 0) && (numLen < strResLen))
    {
        if (number < 0)
        {
            /* Skip the sign. */
            numLen--;
            strResTemp++;
        }
        /* Note: strRes is updated in this function. */
        GetFormattedNum(strResTemp, numLen, strResLen, 3U);
    }

    return strRes;
}

static char *GetFormattedUnsigned(uint32_t number, uint8_t base, char *strRes,
        uint8_t strResLen)
{
    int numLen = 0;
    char *strResTemp = strRes;
    char *format = (base == 16U) ? "%x" : "%u";
    uint8_t charsBeforeSpace = (base == 16U) ? 4U : 3U;

    numLen = snprintf(strRes, strResLen, format, number, charsBeforeSpace);

    if ((numLen >= 0) && (numLen < strResLen))
    {
        /* Note: strRes is updated in this function. */
        GetFormattedNum(strResTemp, numLen, strResLen, charsBeforeSpace);
    }

    return strRes;
}

static int32_t GetUserInputIntValid(int32_t intMin, int32_t intMax)
{
    bool success = false;
    int32_t userInputInt = 0;
    char intMinWithSpaces[INT32_DEC_DIGITS_SPACES] = {0};
    char intMaxWithSpaces[INT32_DEC_DIGITS_SPACES] = {0};

    do {
        success = GetUserInputInt(&userInputInt);
        success = success && ((intMin <= userInputInt) && (intMax >= userInputInt));

        if (!success)
        {
            PRINTF(USER_MSG_INV_INT_INPUT "<%s; %s>: ",
                    GetFormattedInt(intMin, intMinWithSpaces, INT32_DEC_DIGITS_SPACES),
                    GetFormattedInt(intMax, intMaxWithSpaces, INT32_DEC_DIGITS_SPACES));
        }
    } while (!success);

    return userInputInt;
}

static uint32_t GetUserInputUnsignedValid(uint32_t intMin, uint32_t intMax,
        uint8_t intBase) {
    bool success = false;
    uint32_t userInputInt = 0;
    char intMinWithSpaces[INT32_DEC_DIGITS_SPACES] = {0};
    char intMaxWithSpaces[INT32_DEC_DIGITS_SPACES] = {0};
    const char *strPrefix = (intBase == 16U) ? "0x" : "";

    do {
        success = GetUserInputUnsigned(&userInputInt, intBase);
        success = success && ((intMin <= userInputInt) && (intMax >= userInputInt));

        if (!success)
        {
            PRINTF(USER_MSG_INV_INT_INPUT "<%s%s; %s%s>: ",
                    strPrefix,
                    GetFormattedUnsigned(intMin, intBase, intMinWithSpaces, INT32_DEC_DIGITS_SPACES),
                    strPrefix,
                    GetFormattedUnsigned(intMax, intBase, intMaxWithSpaces, INT32_DEC_DIGITS_SPACES));
        }
    } while (!success);

    return userInputInt;
}

uint8_t GetUserCommand(void)
{
    char cmdStr[3];
    char *parseEndPtr = NULL;
    uint8_t cmd = 0;
    uint8_t read = 0U;
    bool success = false;

    PrintCommandPrompt();

    while (1)
    {
        read = GetUserInput(cmdStr, 2U);

        if (cmdStr[0] == '?')
        {
            PrintCommands();
            PrintCommandPrompt();
        }
        else
        {
            if (read > 0U)
            {
                errno = 0;
                cmd = (uint8_t)strtoul(cmdStr, &parseEndPtr, 16);

                /* The whole string must be parsed successfully. */
                success = (parseEndPtr != cmdStr) && (*parseEndPtr == '\0') &&
                        (ERANGE != errno);
            }

            if (success && (cmd > SF_CMD_LAST_ID))
            {
                success = false;
            }

            if (!success)
            {
                PrintMsgRangeUint("\tEnter valid hexadecimal number in range",
                        1U, SF_CMD_LAST_ID, 16U);
            }
            else
            {
                break;
            }
        }
    }

    return cmd;
}

void PrintPayload(const sf_msg_payload_t *msgPayload)
{
    uint8_t i;

    AML_ASSERT(msgPayload != NULL);

    PRINTF("\tLength:\t%u bytes\r\n", msgPayload->payloadLen);
    PRINTF("\tPayload:\t0x", msgPayload->payloadLen);
    for (i = 0U; i < msgPayload->payloadLen; i++)
    {
        PRINTF("%02x ", *(msgPayload->payload + i));
    }
    PRINTF("\r\n");
}

void PrintStatus(sf_drv_data_t *sfDrvData, status_t execStatus, bool sfError)
{
    if (sfError) {
        if (execStatus != kStatus_Success)
        {
            PrintDeviceState(sfDrvData); /* Prints OL2385 firmware status. */
            PRINTF("\tCommand failed with error code %d.\r\n", execStatus);
        }
        else
        {
            PRINTF("\tCommand passed.\r\n");
        }
    }
    else
    {
        PRINTF("\tUnknown command code\r\n");
    }
}

status_t ProcessCommand(sf_drv_data_t *sfDrvData, uint8_t cmd,
        sf_net_standard_t *sfNetStandard, bool *sfError)
{
    status_t status = kStatus_Success;
    uint8_t dataUint8 = 0U;

    AML_ASSERT(cmd <= SF_CMD_LAST_ID);

    *sfError = true;

    switch (cmd)
    {
        case sfSpiCmdWakeup:
            status = HandleWakeUp(sfDrvData);
            break;

        case sfSpiCmdSendEcho:
            status = HandleEcho(sfDrvData);
            break;

        case sfSpiCmdSleep:
            status = HandleSleep(sfDrvData);
            break;

        case sfSpiCmdSendPayload:
            status = HandleSendPayload(sfDrvData);
            break;

        case sfSpiCmdSendBit:
            status = HandleSendBit(sfDrvData);
            break;

        case sfSpiCmdOutOfBand:
            status = HandleSendOoB(sfDrvData);
            break;

        case sfSpiCmdReceive:
            status = HandleRcvFrame(sfDrvData);
            break;

        case sfSpiCmdGetInfo:
            status = HandleGetInfo(sfDrvData);
            break;

        case sfSpiCmdSetUlFreq:
            status = HandleSetFreq(sfDrvData, sfSpiCmdSetUlFreq);
            break;

        case sfSpiCmdGetUlFreq:
            status = HandleGetFreq(sfDrvData, sfSpiCmdGetUlFreq);
            break;

        case sfSpiCmdContWave:
            status = HandleContWave(sfDrvData);
            break;

        case sfSpiCmdGetDevVer:
            status = HandleGetDevVer(sfDrvData);
            break;

        case sfSpiCmdTriggerWd:
            status = HandleTrigWd(sfDrvData);
            break;

        case sfSpiCmdSendTestMode:
            status = HandleTestMode(sfDrvData);
            break;

        case sfSpiCmdChangeToRCZ1:
            status = HandleChangeNetStandard(sfDrvData, sfNetStandardETSI);
            *sfNetStandard = sfNetStandardETSI;
            break;

        case sfSpiCmdChangeToRCZ2:
            status = HandleChangeNetStandard(sfDrvData, sfNetStandardFCC_USA);
            *sfNetStandard = sfNetStandardFCC_USA;
            break;

        case sfSpiCmdChangeToRCZ3:
            status = HandleChangeNetStandard(sfDrvData, sfNetStandardARIB);
            *sfNetStandard = sfNetStandardARIB;
            break;

        case sfSpiCmdChangeToRCZ4:
            status = HandleChangeNetStandard(sfDrvData, sfNetStandardFCC_SouthAmerica);
            *sfNetStandard = sfNetStandardFCC_USA;
            break;

        case sfSpiCmdSwitchToPrivateKey:
            PRINTF("Switch to private key command\r\n");
            status = SF_SwitchToPrivateKey(sfDrvData);
            break;

        case sfSpiCmdSwitchToPublicKey:
            PRINTF("Switch to public key command\r\n");
            status = SF_SwitchToPublicKey(sfDrvData);
            break;

        case sfSpiCmdPskAskZeroSeq:
            status = HandleTransmitPskAskSeq(sfDrvData, sfSpiCmdPskAskZeroSeq);
            break;

        case sfSpiCmdPskAskAAAASeq:
            status = HandleTransmitPskAskSeq(sfDrvData, sfSpiCmdPskAskAAAASeq);
            break;

        case sfSpiCmdSetDlFreq:
            status = HandleSetFreq(sfDrvData, sfSpiCmdSetDlFreq);
            break;

        case sfSpiCmdGetDlFreq:
            status = HandleGetFreq(sfDrvData, sfSpiCmdGetDlFreq);
            break;

        case sfSpiCmdSetFCCMacroCh:
            status = HandleSetFCCMacroChannel(sfDrvData, *sfNetStandard);
            break;

        case sfSpiCmdGetFCCMacroChUsage:
            PRINTF("Get FCC macro channel usage command\r\n");
            if ((status = SF_GetFCCMacroChUsage(sfDrvData, &dataUint8)) == kStatus_Success)
            {
                PRINTF("\tFCC macro channel usage data: 0x%02x\r\n", dataUint8);
            }
            break;

        case sfSpiCmdResetFCCMacroCh:
            PRINTF("Reset FCC macro channel command\r\n");
            status = SF_ResetFCCMacroChannel(sfDrvData);
            break;

        case sfSpiCmdStaticFreqCal:
            status = HandleStaticFreqCal(sfDrvData);
            break;

        case sfSpiCmdTempFreqCal:
            status = HandleTempFreqCal(sfDrvData);
            break;

        case sfSpiCmdGetLastSentRSSI:
            status = HandleGetLastSentRSSI(sfDrvData);
            break;

        case sfSpiCmdUpdatePACurve:
            status = HandleUpdatePACurve(sfDrvData);
            break;

        case sfSpiCmdInitRSSICal:
            status = HandleInitialRSSICal(sfDrvData);
            break;

        default:
            *sfError = false;
            status = kStatus_Fail;
            break;
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleWakeUp
 * Description : Handler function for 'Wake up' command.
 *
 *END**************************************************************************/
static status_t HandleWakeUp(sf_drv_data_t *sfDrvData)
{
    status_t status = kStatus_Success;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Wake-up command\r\n");
    status = SF_WakeUp(sfDrvData);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleEcho
 * Description : Handler function for 'Echo' command.
 *
 *END**************************************************************************/
static status_t HandleEcho(sf_drv_data_t *sfDrvData)
{
    status_t status = kStatus_Success;
    bool success = true;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Echo command\r\n");
    PRINTF("\tSending '0x0102 0304 05' via SPI\r\n");

    status = SF_TestSpiCon(sfDrvData, &success);
    if (status == kStatus_Success)
    {
        if (!success) {
            PRINTF("\tReceived echo is wrong.\r\n");
        }
        else
        {
            PRINTF("\tReceived echo is correct: '0x0102 0304 05'\r\n");
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleSleep
 * Description : Handler function for 'Sleep' command.
 *
 *END**************************************************************************/
static status_t HandleSleep(sf_drv_data_t *sfDrvData)
{
    status_t status = kStatus_Success;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Sleep command\r\n");
    status = SF_Sleep(sfDrvData);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleSendPayload
 * Description : Handler function for 'Send payload' command.
 *
 *END**************************************************************************/
static status_t HandleSendPayload(sf_drv_data_t *sfDrvData)
{
    status_t status = kStatus_Success;
    char msg[13];
    uint8_t readLen = 0U;
    sf_msg_payload_t data;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Send payload command\r\n");

    do {
        PRINTF("\tEnter up to 12 characters: ");

        if ((readLen = GetUserInput((char*)msg, 12)) == 0U)
        {
            PRINTF("\tEmpty string passed.\r\n");
        }
    } while (0U == readLen);

    PRINTF("\tSending payload message: %s\r\n", msg);

    data.payload = (uint8_t*)msg;
    data.payloadLen = readLen;

    status = SF_SendPayload(sfDrvData, &data);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleSendBit
 * Description : Handler function for 'Send bit' command.
 *
 *END**************************************************************************/
static status_t HandleSendBit(sf_drv_data_t *sfDrvData)
{
    uint8_t bitVal = 0;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Send bit command\r\n");

    PRINTF("\tSelect bit value to be sent (0 or 1): ");
    bitVal = (uint8_t)GetUserInputUnsignedValid(0U, 1U, 10U);

    return SF_SendBit(sfDrvData, bitVal);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleSendOoB
 * Description : Handler function for 'Send out of band' command.
 *
 *END**************************************************************************/
static status_t HandleSendOoB(sf_drv_data_t *sfDrvData)
{
    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Send out of band command\r\n");

    return SF_SendOutOfBand(sfDrvData);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleRcvFrame
 * Description : Handler function for 'Receive frame' command.
 *
 *END**************************************************************************/
static status_t HandleRcvFrame(sf_drv_data_t *sfDrvData)
{
    uint8_t sendData[12] = {0};
    uint8_t recvData[12] = {0};
    sf_msg_payload_t sendMsg = {12U, sendData};
    sf_msg_payload_t recvMsg = {12U, recvData};
    uint8_t readLen = 0U;
    status_t status = kStatus_Fail;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Receive frame command\r\n");

    do {
        PRINTF("\tEnter up to 12 characters: ");

        if ((readLen = GetUserInput((char*)sendData, 12)) == 0U)
        {
            PRINTF("\tEmpty string passed.\r\n");
        }
    } while (0U == readLen);

    PRINTF("\tWaiting for data from SIGFOX network\r\n");

    sendMsg.payloadLen = readLen;
    status = SF_ReceiveMessage(sfDrvData, &sendMsg, &recvMsg);

    if (kStatus_Success == status)
    {
        PrintPayload(&recvMsg);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleGetInfo
 * Description : Handler function for 'Get info' command.
 *
 *END**************************************************************************/
static status_t HandleGetInfo(sf_drv_data_t *sfDrvData)
{
    status_t status = kStatus_Fail;
    uint8_t rcvPayloadData[SF_ACK_SPI_MSG_MAX_B];  /* Buffer containing received payload. */
    sf_msg_payload_t sndPayload = {0};             /* Dummy data to be sent. */
    sf_msg_payload_t rcvPayload =                  /* Received payload. */
    {
            SF_ACK_SPI_MSG_MAX_B,
            rcvPayloadData
    };
    uint8_t pldOf = 0U;                            /* Offset in the payload. */

    PRINTF("Get info command\r\n");

    status = SF_SendCommand(sfDrvData, sfSpiCmdGetInfo, &sndPayload, &rcvPayload,
            SF_ACK_SPI_MSG_MAX_B);

    if (rcvPayload.payloadLen != SF_GET_INFO_ACK_PLD_B)
    {
        status = kStatus_Fail;
    }

    if (kStatus_Success == status)
    {
        pldOf = SF_DEV_ID_OF;
        PRINTF("\tDevice ID = 0x");
        PRINTF("%02x %02x ", rcvPayloadData[pldOf + 3], rcvPayloadData[pldOf + 2]);
        PRINTF("%02x %02x\r\n", rcvPayloadData[pldOf + 1], rcvPayloadData[pldOf + 0]);

        pldOf = SF_DEV_PAC_OF;
        PRINTF("\tPAC = 0x");
        PRINTF("%02x %02x ", rcvPayloadData[pldOf], rcvPayloadData[pldOf + 1]);
        PRINTF("%02x %02x ", rcvPayloadData[pldOf + 2], rcvPayloadData[pldOf + 3]);
        PRINTF("%02x %02x ", rcvPayloadData[pldOf + 4], rcvPayloadData[pldOf + 5]);
        PRINTF("%02x %02x\r\n", rcvPayloadData[pldOf + 6], rcvPayloadData[pldOf + 7]);

        pldOf = SF_LIB_VER_OF;
        PRINTF("\tLibrary version = 0x");
        PRINTF("%02x ", rcvPayloadData[pldOf + 10]);
        PRINTF("%02x %02x ", rcvPayloadData[pldOf + 9], rcvPayloadData[pldOf + 8]);
        PRINTF("%02x %02x ", rcvPayloadData[pldOf + 7], rcvPayloadData[pldOf + 6]);
        PRINTF("%02x %02x ", rcvPayloadData[pldOf + 5], rcvPayloadData[pldOf + 4]);
        PRINTF("%02x %02x ", rcvPayloadData[pldOf + 3], rcvPayloadData[pldOf + 2]);
        PRINTF("%02x %02x\r\n", rcvPayloadData[pldOf + 1], rcvPayloadData[pldOf]);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleSetFreq
 * Description : Handler function for 'Set UL/DL frequency' command.
 *
 *END**************************************************************************/
static status_t HandleSetFreq(sf_drv_data_t *sfDrvData, sf_spi_cmd_t setFreqCmd)
{
    uint32_t freq = 0U;
    status_t status = kStatus_Fail;
    status_t (*setFreqFunc)(sf_drv_data_t *, uint32_t) = NULL;

    AML_ASSERT(sfDrvData != NULL);

    if (sfSpiCmdSetUlFreq == setFreqCmd) {
        PRINTF("Set up-link frequency command\r\n");
        setFreqFunc = &SF_SetUlFrequency;
    } else if (sfSpiCmdSetDlFreq == setFreqCmd) {
        PRINTF("Set down-link frequency command\r\n");
        setFreqFunc = &SF_SetDlFrequency;
    }

    if (NULL != setFreqFunc) {
        PrintMsgRangeUint("\tEnter frequency in Hz", SF_FREQ_MIN_HZ,
                SF_FREQ_MAX_HZ, 10U);
        freq = GetUserInputUnsignedValid(SF_FREQ_MIN_HZ, SF_FREQ_MAX_HZ, 10U);
        status = (*setFreqFunc)(sfDrvData, freq);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleGetFreq
 * Description : Handler function for 'Get UL/DL frequency' command.
 *
 *END**************************************************************************/
static status_t HandleGetFreq(sf_drv_data_t *sfDrvData, sf_spi_cmd_t getFreqCmd)
{
    uint32_t freq = 0U;
    status_t status = kStatus_Fail;
    status_t (*getFreqFunc)(sf_drv_data_t *, uint32_t *) = NULL;

    AML_ASSERT(sfDrvData != NULL);

    if (sfSpiCmdGetUlFreq == getFreqCmd) {
        PRINTF("Get up-link frequency command\r\n");
        getFreqFunc = &SF_GetUlFrequency;
    } else if (sfSpiCmdGetDlFreq == getFreqCmd) {
        PRINTF("Get down-link frequency command\r\n");
        getFreqFunc = &SF_GetDlFrequency;
    }

    if (NULL != getFreqFunc) {
        status = (*getFreqFunc)(sfDrvData, &freq);
        if (status == kStatus_Success)
        {
            PRINTF("\tFrequency is %d Hz.\r\n", freq);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleContWave
 * Description : Handler function for 'Continuous wave' command.
 *
 *END**************************************************************************/
static status_t HandleContWave(sf_drv_data_t *sfDrvData)
{
    uint8_t userInputInt = 0;
    bool waveEnabled = false;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Continuous wave command\r\n");

    PRINTF("\tFunction switch (0 - disabled, 1 - enabled): ");
    userInputInt = (uint8_t)GetUserInputUnsignedValid(0, 1, 10U);

    waveEnabled = (userInputInt) ? true : false;
    return SF_GenerateContWave(sfDrvData, waveEnabled);
}

status_t PrintFirmwareVersion(sf_drv_data_t *sfDrvData)
{
    status_t status = kStatus_Success;
    sf_msg_payload_t sendPayload;
    sf_msg_payload_t rcvPayload;
    uint8_t rcvData[SF_GET_DEV_VER_ACK_PLD_B];

    AML_ASSERT(sfDrvData != NULL);

    sendPayload.payload = NULL;
    sendPayload.payloadLen = 0U;

    rcvPayload.payload = rcvData;
    rcvPayload.payloadLen = SF_GET_DEV_VER_ACK_PLD_B;

    status = SF_SendCommand(sfDrvData, sfSpiCmdGetDevVer, &sendPayload, \
            &rcvPayload, SF_GET_DEV_VER_ACK_PLD_B);

    if (status == kStatus_Success)
    {
        if (rcvPayload.payloadLen != SF_GET_DEV_VER_ACK_PLD_B)
        {
            status = kStatus_SF_SpiAckLength;
        }
        else
        {
            /* Format: <major>.<minor>.<debug>.<day>.<month>.<year>.<year>. */
            PRINTF("%u.%u.%u.%u.%u.%u.%u",
                    rcvPayload.payload[SF_DEV_VER_OF],
                    rcvPayload.payload[SF_DEV_VER_OF + 1U],
                    rcvPayload.payload[SF_DEV_VER_OF + 2U],
                    rcvPayload.payload[SF_DEV_VER_OF + 3U],
                    rcvPayload.payload[SF_DEV_VER_OF + 4U],
                    rcvPayload.payload[SF_DEV_VER_OF + 5U],
                    rcvPayload.payload[SF_DEV_VER_OF + 6U]);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleGetDevVer
 * Description : Handler function for 'Get device version' command.
 *
 *END**************************************************************************/
static status_t HandleGetDevVer(sf_drv_data_t *sfDrvData)
{
    status_t status = kStatus_Success;

    PRINTF("Get device version command\r\n");

    PRINTF("\tOL2385 firmware version: ");
    status = PrintFirmwareVersion(sfDrvData);
    PRINTF("\r\n");

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleTrigWd
 * Description : Handler function for 'Trigger watchdog' command.
 *
 *END**************************************************************************/
static status_t HandleTrigWd(sf_drv_data_t *sfDrvData)
{
    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Trigger watchdog command\r\n");

    return SF_TriggerWatchdog(sfDrvData);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HandleTestMode
 * Description : Handler function for 'Test mode' command.
 *
 *END**************************************************************************/
static status_t HandleTestMode(sf_drv_data_t *sfDrvData)
{
    const char *testModes[6] = {"TxBpsk", "TxProtocol", "RxProtocol", "RxGfsk", "RxSensi", "TxSynth"};
    uint8_t testModeInt = 0;
    uint8_t modeConfig = 0;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Test mode command\r\n");
    PRINTF("\tAvailable test modes:\r\n");
    PRINTF("\t[0] TxBpsk - Sending PRBS data in a 26 Bytes frame at constant frequency.\r\n");
    PRINTF("\t[1] TxProtocol - Test of the complete protocol in up-link.\r\n");
    PRINTF("\t[2] RxProtocol - Test of the complete protocol in down-link.\r\n");
    PRINTF("\t[3] RxGfsk - Receiving constant GFSK frames at constant frequency.\r\n");
    PRINTF("\t[4] RxSensi - Measurement of the real sensitivity of device.\r\n");
    PRINTF("\t[5] TxSynth - Sending SigFox frames with 4 Bytes payload at forced frequency.\r\n");
    PRINTF("\tSelect a test mode and press enter <0; 5>: ");

    testModeInt = (uint8_t)GetUserInputUnsignedValid(0U, 5U, 10U);

    PRINTF("\tTest mode [%s] was selected.\r\n", testModes[testModeInt]);

    PrintMsgRangeUint("\tEnter test mode configuration", 0U, UINT8_MAX, 10U);
    modeConfig = (uint8_t)GetUserInputUnsignedValid(0U, UINT8_MAX, 10U);

    return SF_TestDevice(sfDrvData, (sf_test_mode_t)testModeInt, modeConfig);
}

static status_t HandleChangeNetStandard(sf_drv_data_t *sfDrvData, sf_net_standard_t netStandard)
{
    uint8_t rczNumber = (uint8_t)netStandard + 1U;
    uint8_t txAttSteps = 0U;
    sf_xtal_type_t xtal = sfXtal552;
    sf_tx_repeat_t txRepeat = sfTxRepeat1;
    sf_pa_type_t paType = sfPa14;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Change to RCZ%u\r\n", rczNumber);

    PrintMsgRangeUint("\tEnter amount of 0.25dB steps that needs to be subtracted from TX power (default value is 0)", 0U, UINT8_MAX, 10U);
    txAttSteps = (uint8_t)GetUserInputUnsignedValid(0U, UINT8_MAX, 10U);

    PRINTF("\tEnter used type of XTAL (0 - 55.2 MHz, 1 - 27.6 MHz TCXO): ");
    xtal = (sf_xtal_type_t) GetUserInputUnsignedValid(sfXtal552, sfXtal276, 10U);

    PRINTF("\tEnter amount of transmissions that will happen on one time invoking of SF_SendPayload function (0 - 1 TX, 1 - 3 TX): ");
    txRepeat = (sf_tx_repeat_t) GetUserInputUnsignedValid(sfTxRepeat1, sfTxRepeat3, 10U);

    PRINTF("\tEnter type of internal PA (0 - 14 dBm PA, 1 - 0 dBm PA): ");
    paType = (sf_pa_type_t) GetUserInputUnsignedValid(sfPa14, sfPa0, 10U);

    return SF_ChangeNetworkStandard(sfDrvData, netStandard, txAttSteps, xtal, txRepeat, paType);
}


static status_t HandleTransmitPskAskSeq(sf_drv_data_t *sfDrvData,
        sf_spi_cmd_t pskAskSeqCmd)
{
    int32_t userInputInt = 0;
    status_t (*pskAskSeqFunc)(sf_drv_data_t *, int16_t) = NULL;
    status_t status = kStatus_Fail;

    if (sfSpiCmdPskAskZeroSeq == pskAskSeqCmd) {
        PRINTF("PSK ASK zero sequence command\r\n");
        pskAskSeqFunc = &SF_TransmitZeroSeq;
    } else if (sfSpiCmdPskAskAAAASeq == pskAskSeqCmd) {
        PRINTF("PSK ASK AAAA sequence command\r\n");
        pskAskSeqFunc = &SF_TransmitAAAASeq;
    }

    if (NULL != pskAskSeqFunc) {
        PRINTF("\tSelect number of repetitions (<-1; 255>, -1 for infinite loop): ");
        userInputInt = GetUserInputIntValid(-1, 255);
        status = (*pskAskSeqFunc)(sfDrvData, (int16_t)userInputInt);
    }

    return status;
}

static status_t HandleSetFCCMacroChannel(sf_drv_data_t *sfDrvData,
        sf_net_standard_t netStandard)
{
    uint32_t channelCfg[3] = {0U};
    uint8_t defaultCh = 0U;
    status_t status = kStatus_Fail;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Set FCC macro channel command\r\n");

    if (sfNetStandardFCC_USA == netStandard) {
        PRINTF("\tA 32 bit mask for enabling/disabling channels is required.\r\n");
        PrintMsgRangeUint("\tEnter mask for ch. 1-32 (ch. 1: bit 0)", 0U, UINT32_MAX, 16U);
        channelCfg[0] = (uint32_t)GetUserInputUnsignedValid(0U, UINT32_MAX, 16U);

        PrintMsgRangeUint("\tEnter mask for ch. 33-64 (ch. 33: bit 0)", 0U, UINT32_MAX, 16U);
        channelCfg[1] = (uint32_t)GetUserInputUnsignedValid(0U, UINT32_MAX, 16U);

        PrintMsgRangeUint("\tEnter mask for ch. 65-86 (ch. 65: bit 0)", 0U, 0x3FFFFFU, 16U);
        channelCfg[2] = (uint32_t)GetUserInputUnsignedValid(0U, 0x003FFFFFU, 16U);

        PrintMsgRangeUint("\tEnter default channel (decimal)", 0U, UINT8_MAX, 10U);
        defaultCh = (uint8_t)GetUserInputUnsignedValid(0U, 0xFFU, 10U);

        status = SF_SetFCCMacroChannel(sfDrvData, channelCfg, defaultCh);
    }
    else
    {
        PRINTF("\tThis command is available for RCZ2 only\r\n");
        status = kStatus_Success;
    }

    return status;
}

static status_t HandleStaticFreqCal(sf_drv_data_t *sfDrvData)
{
    int64_t freqDriftHz = 0;
    uint32_t freqDriftAbsHz = 0U;
    bool signPlus = false;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Static frequency calibration command\r\n");

    PrintMsgRangeInt("\tEnter frequency drift", -(int32_t)SF_FREQ_MAX_HZ, SF_FREQ_MAX_HZ);
    freqDriftHz = GetUserInputIntValid(-(int32_t)SF_FREQ_MAX_HZ, SF_FREQ_MAX_HZ);

    signPlus = freqDriftHz >= 0;
    /* Get absolute value of frequency drift. */
    freqDriftAbsHz = (signPlus) ? (uint32_t)freqDriftHz : (uint32_t)(-freqDriftHz);

    return SF_CalibrateFreqStatic(sfDrvData, signPlus, freqDriftAbsHz);
}

static status_t HandleTempFreqCal(sf_drv_data_t *sfDrvData)
{
    uint8_t subCmd = 0U;
    uint8_t calibTable[SF_TEMP_FREQ_CAL_TBL_SIZE] = {0};
    uint8_t i = 0U;
    status_t status = kStatus_Fail;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Static frequency calibration command\r\n");
    PRINTF("\tEnter sub-command (0 - read, 1 - update, 2 - set default table): ");

    subCmd = GetUserInputUnsignedValid(SF_SUBCMD_READ_TBL, SF_SUBCMD_SET_DEFAULT_TBL, 10U);

    if (SF_SUBCMD_UPDATE_TBL == subCmd)
    {
        PRINTF("\tEnter 26 decimal values:\r\n");
        for (i = 0U; i < SF_TEMP_FREQ_CAL_TBL_SIZE; i++)
        {
            PRINTF("\t\t%2u: ", i + 1U);
            calibTable[i] = GetUserInputUnsignedValid(0U, UINT8_MAX, 10U);
        }
    }

    switch (subCmd)
    {
        case SF_SUBCMD_READ_TBL:
            /* Read values are stored into calibTable. */
            status = SF_ReadTempFreqCalTable(sfDrvData, calibTable);

            if (kStatus_Success == status)
            {
                PRINTF("\tTemp. table: \r\n");
                for (i = 0U; i < SF_TEMP_FREQ_CAL_TBL_SIZE; i++)
                {
                    PRINTF("\t\t%2u: %u\r\n",
                            i + 1U, calibTable[i]);
                }
            }
            break;

        case SF_SUBCMD_UPDATE_TBL:
            /* calibTable values are written into the device. */
            status = SF_UpdateTempFreqCalTable(sfDrvData, calibTable);
            break;

        case SF_SUBCMD_SET_DEFAULT_TBL:
            /* calibTable values are written into the device. */
            status = SF_SetDefaultTempFreqCalTable(sfDrvData);
            break;

        default:
            AML_ASSERT(false);
            status = kStatus_Fail;
            break;
    }

    return status;
}

static status_t HandleGetLastSentRSSI(sf_drv_data_t *sfDrvData)
{
    uint8_t subCmd = 0U;
    uint8_t i = 0U;
    uint8_t rssi = 0U;
    uint8_t rssiBuffer[SF_SENT_RSSI_TBL_SIZE] = {0};
    status_t status = kStatus_Fail;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Get last sent RSSI command\r\n");
    PRINTF("\tEnter sub-command:\r\n");
    PRINTF("\t[0] Get recently computed RSSI.\r\n");
    PRINTF("\t[1] Get recently computed RSSI for 30 frames during RX GFSK test.\r\n");
    PRINTF("\t[2] Reset buffer for 30 frames used by RX GFSK test.\r\n");
    PRINTF("\tSelect a sub-command and press enter <0; 2>: ");
    subCmd = GetUserInputUnsignedValid(SF_SUBCMD_GET_LAST_RSSI, SF_SUBCMD_RESET_RSSI_BUFFER, 10U);

    switch (subCmd)
    {
        case SF_SUBCMD_GET_LAST_RSSI:
            /* Read recently computed RSSI. */
            status = SF_GetLastSentRSSI(sfDrvData, &rssi);
            if (kStatus_Success == status)
            {
                PRINTF("\tRSSI = %u\r\n", rssi);
            }
            break;

        case SF_SUBCMD_GET_LAST_30_RSSI:
            /* Gets recently computed RSSI for 30 frames during RX GFSK test. */
            status = SF_GetLast30SentRSSI(sfDrvData, rssiBuffer);
            if (kStatus_Success == status)
            {
                PRINTF("\tRecently computed RSSI for 30 frames during RX GFSK test:\r\n");
                for (i = 0U; i < SF_SENT_RSSI_TBL_SIZE; i++)
                {
                    PRINTF("\t\t%2u: %u\r\n", i + 1U, rssiBuffer[i]);
                }
            }
            break;

        case SF_SUBCMD_RESET_RSSI_BUFFER:
            /* Resets the buffer. */
            status = SF_ResetLastSentRSSIBuffer(sfDrvData);
            break;

        default:
            AML_ASSERT(false);
            status = kStatus_Fail;
            break;
    }

    return status;
}

static status_t HandleUpdatePACurve(sf_drv_data_t *sfDrvData)
{
    uint8_t subCmd = 0U;
    uint8_t paTable[SF_PA_TBL_SIZE] = {0};
    uint8_t paTableSize;
    sf_net_standard_t netStd;
    uint8_t i = 0U;
    status_t status = kStatus_Fail;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Update PA curve command\r\n");
    PRINTF("\tEnter sub-command:\r\n");
    PRINTF("\t[0] Read current ETSI PA table.\r\n");
    PRINTF("\t[1] Update ETSI PA table.\r\n");
    PRINTF("\t[2] Set default ETSI PA table.\r\n");
    PRINTF("\t[3] Read current FCC PA table.\r\n");
    PRINTF("\t[4] Update FCC PA table.\r\n");
    PRINTF("\t[5] Set default FCC PA table.\r\n");
    PRINTF("\tSelect a sub-command and press enter <0; 5>: ");
    subCmd = GetUserInputUnsignedValid(SF_SUBCMD_READ_ETSI_PA_TABLE, SF_SUBCMD_DEFAULT_FCC_PA_TABLE, 10U);

    netStd = (subCmd < SF_SUBCMD_READ_FCC_PA_TABLE) ? sfNetStandardETSI : sfNetStandardFCC_USA;
    paTableSize = (subCmd < SF_SUBCMD_READ_FCC_PA_TABLE) ? SF_PA_TBL_ETSI_SIZE : SF_PA_TBL_FCC_SIZE;

    if ((SF_SUBCMD_UPDATE_ETSI_PA_TABLE == subCmd) || (SF_SUBCMD_UPDATE_FCC_PA_TABLE == subCmd))
    {
        PRINTF("\tEnter %u decimal values:\r\n", paTableSize);
        for (i = 0U; i < paTableSize; i++)
        {
            PRINTF("\t\t%3u: ", i + 1U);
            paTable[i] = GetUserInputUnsignedValid(0U, UINT8_MAX, 10U);
        }
    }

    switch (subCmd)
    {
        case SF_SUBCMD_READ_ETSI_PA_TABLE:
        case SF_SUBCMD_READ_FCC_PA_TABLE:
            /* Read values are stored into paTable. */
            status = SF_ReadCurrentPATable(sfDrvData, netStd, paTable);

            if (kStatus_Success == status)
            {
                PRINTF("\tPA table: \r\n");
                for (i = 0U; i < paTableSize; i++)
                {
                    PRINTF("\t\t%3u: %u\r\n", i + 1U, paTable[i]);
                }
            }
            break;

        case SF_SUBCMD_UPDATE_ETSI_PA_TABLE:
        case SF_SUBCMD_UPDATE_FCC_PA_TABLE:
            /* paTable values are written into the device. */
            status = SF_UpdatePATable(sfDrvData, netStd, paTable);
            break;

        case SF_SUBCMD_DEFAULT_ETSI_PA_TABLE:
        case SF_SUBCMD_DEFAULT_FCC_PA_TABLE:
            status = SF_SetDefaultPATable(sfDrvData, netStd);
            break;

        default:
            AML_ASSERT(false);
            status = kStatus_Fail;
            break;
    }

    return status;
}

static status_t HandleInitialRSSICal(sf_drv_data_t *sfDrvData)
{
    int64_t driftOffset = 0;
    uint32_t driftOffsetAbs = 0U;
    bool signPlus = false;

    AML_ASSERT(sfDrvData != NULL);

    PRINTF("Initial RSSI calibration command\r\n");

    PrintMsgRangeInt("\tEnter drift offset", -(int32_t)(0xFFFF), (int32_t)(0xFFFF));
    driftOffset = GetUserInputIntValid(-(int32_t)(0xFFFF), (int32_t)(0xFFFF));

    signPlus = driftOffset >= 0;
    /* Get absolute value of frequency drift. */
    driftOffsetAbs = (signPlus) ? (uint32_t)driftOffset : (uint32_t)(-driftOffset);

    return SF_CalibrateInitialRSSI(sfDrvData, signPlus, driftOffsetAbs);
}


/*******************************************************************************
 * EOF
 ******************************************************************************/
