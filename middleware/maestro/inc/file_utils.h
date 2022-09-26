/*
 * Copyright 2018-2022 NXP.
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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief FatFS enabled
 *
 */
#ifdef SD_ENABLED
#define FILE_ENABLE_FATFS
#endif // SD_ENABLED
#ifndef FILE_ENABLE_SEMI
#define FILE_ENABLE_FATFS
#endif // FILE_ENABLE_SEMI

/**
 * @brief No file utils wrapper used
 *
 */
#define FILE_UTILS_NONE 0
/**
 * @brief FatFS file utils wrapper used
 *
 */
#define FILE_UTILS_FATFS 1
/**
 * @brief Semihosting file utils wrapper used
 *
 */
#define FILE_UTILS_SEMI 2

/**
 * @brief Selection of file system
 *
 */
#ifdef FILE_ENABLE_SEMI
#define FILE_UTILS_FS_SEL FILE_UTILS_SEMI
/**
 * @brief Enable use of cache in semihosting mode
 *
 */
#define FILE_UTILS_SEMI_CACHE
#else
#define FILE_UTILS_FS_SEL FILE_UTILS_FATFS
#endif // FILE_ENABLE_SEMI

/**
 * @brief Maximal number of open files
 *
 */
#ifndef FILE_FS_TABLE_SIZE
#define FILE_FS_TABLE_SIZE 1u
#endif // FILE_FS_TABLE_SIZE

/**
 * @brief File access mode read only
 *
 */
#define FILE_RDONLY 0x01u
/**
 * @brief File access mode write only
 *
 */
#define FILE_WRONLY 0x02u
/**
 * @brief File access mode read write
 *
 */
#define FILE_RDWR 0x04u
/**
 * @brief File access mode append
 *
 */
#define FILE_APPEND 0x08u
/**
 * @brief File access mode create
 *
 */
#define FILE_CREAT 0x10u

/**
 * @brief File access mode create and truncates it to 0.
 *
 */
#define FILE_TRUNC 0x20u

/*!
 * @ingroup utils
 * @brief file_open
 * @details Function to get handle of open device
 * @param pathname File path
 * @param mode file mode
 * @returns file descriptor
 */
int32_t file_open(const char *pathname, uint32_t mode);

/*!
 * @ingroup utils
 * @brief file_close
 * @details Function to close file
 * @param fd File descriptor
 * @returns error code of operation
 */
int32_t file_close(int32_t fd);

/*!
 * @ingroup utils
 * @brief file_read
 * @details Function to read data from file given by handle
 * @param fd File descriptor
 * @param buff buffer pointer
 * @param btr bytes to be read
 * @returns  bytes read
 */
int32_t file_read(int32_t fd, void *buff, size_t btr);

/*!
 * @ingroup utils
 * @brief file_write
 * @details Function to write data to file given by handle
 * @param fd File descriptor
 * @param buff buffer pointer
 * @param cnt bytes to be written
 * @returns  bytes writen
 */
int32_t file_write(int32_t fd, void *buff, size_t cnt);

/*!
 * @ingroup utils
 * @brief file_seek
 * @details Function to seek in given by handle
 * @param fd File descriptor
 * @param offset from start of the file
 * @returns error code of operation
 */
int32_t file_seek(int32_t fd, size_t offset);

/*!
 * @ingroup utils
 * @brief file_getsize
 * @details Function to get the device file size.
 * @param fd       File handle
 * @returns size of file in bytes
 */
int32_t file_getsize(int32_t fd);

#ifdef FILE_ENABLE_FATFS
/*!
 * @ingroup utils
 * @brief   file_exists
 * @details Returns true if the given file exists on disk and false if not
 * @param   filename Name and path of the file to test for existence
 * @returns true or false
 */
bool file_exists(const char *filename);
#endif // FILE_ENABLE_FATFS

/*!
 * @ingroup utils
 * @brief   dir_exists
 * @details Returns true if the given directory exists on disk and false if not
 * @param   dirname Name and path of the directory to test for existence
 * @returns true or false
 */
bool dir_exists(const char *dirname);

#endif
