/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __ERROR_H_
#define __ERROR_H_

/*!
 * @file    error.h
 * @brief   Contains common error codes and definitions.
 */

/** @brief  Error code no error */
#define ERRCODE_NO_ERROR 0x0
/** @brief  Error code try again */
#define ERRCODE_TRY_AGAIN 0x1
/** @brief  Error code invalid argument */
#define ERRCODE_INVALID_ARGUMENT 0x02
/** @brief  Error code out of memory */
#define ERRCODE_OUT_OF_MEMORY 0x03
/** @brief  Error code out of space */
#define ERRCODE_OUT_OF_SPACE 0x04
/** @brief  Error code general error */
#define ERRCODE_GENERAL_ERROR 0x05
/** @brief  Error code deadlock */
#define ERRCODE_DEADLOCK 0x06
/** @brief  Error code not supported */
#define ERRCODE_NOT_SUPPORTED 0x07
/** @brief  Error code busy */
#define ERRCODE_BUSY 0x08
/** @brief  Error code permission denied */
#define ERRCODE_PERMISSION_DENIED 0x09
/** @brief  Error code timed out */
#define ERRCODE_TIMED_OUT 0x0A
/** @brief  Error code system limit */
#define ERRCODE_SYSTEM_LIMIT 0x0B
/** @brief  Error code parameter exists */
#define ERRCODE_PARAMETER_EXISTS 0x0C
/** @brief  Error code end of directory */
#define ERRCODE_END_OF_DIRECTORY 0x0D
/** @brief  Error code not found */
#define ERRCODE_NOT_FOUND 0x0E
/** @brief  Error code unknown */
#define ERRCODE_UNKNOWN 0x0F
/** @brief  Error code internal */
#define ERRCODE_INTERNAL 0x10
/** @brief  Error code overflow */
#define ERRCODE_OVERFLOW 0x11
/** @brief  Error code message failure */
#define ERRCODE_MSG_FAILURE 0x12
/** @brief  Error code thread failure */
#define ERRCODE_THREAD_FAILURE 0x13
/** @brief  Error code mutex failure */
#define ERRCODE_MUTEX_FAILURE 0x14
/** @brief  Error code device element exists */
#define ERRCODE_DEVICE_ELEMENT_EXISTS 0x15
/** @brief  Error code not seekable */
#define ERRCODE_NOT_SEEKABLE 0x16

#endif
