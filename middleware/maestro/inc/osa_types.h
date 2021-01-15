/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_TYPES_H
#define OSA_TYPES_H

/*!
 * @file    osa_types.h
 * @brief   Contains base data types for NXP libosa.
 */

/* Include POSIX type definitions to be used. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*! Define a version for the NXP OSA API. */
#define OSA_VERSION 2.0

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef bool
    #define bool boolean
#endif

#endif

