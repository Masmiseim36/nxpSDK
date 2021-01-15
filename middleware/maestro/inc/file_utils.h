/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

/*!
 * @file    file_utils.h
 * @brief   This file provides file manipulation functions.
 */

#include "osa_common.h"

/*!
 * @ingroup libcommon
 * @brief file_getsize
 * @details Function to get the device file size.
 * @param fd       File handle
 * @returns size of file in bytes
 */
uint32_t file_getsize(void *fd);

/*!
 * @ingroup libcommon
 * @brief   file_exists
 * @details Returns true if the given file exists on disk and false if not
 * @param   filename Name and path of the file to test for existence
 * @returns true or false
 */
bool file_exists(const char *filename);

/*!
 * @ingroup libcommon
 * @brief   dir_exists
 * @details Returns true if the given directory exists on disk and false if not
 * @param   dirname Name and path of the directory to test for existence
 * @returns true or false
 */
bool dir_exists(const char *dirname);

#endif

