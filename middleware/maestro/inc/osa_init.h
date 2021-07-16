/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_INIT_H
#define OSA_INIT_H

/*!
 * @file    osa_init.h
 * @brief   Contains initialization function prototypes for libosa.
 */

/*!
 * @ingroup libosa
 * @brief   Initialize the OSA library
 * @details Some parts of the OSA need to be initialized before first use.
 *          This function performs the necessary initialization and
 *          allocations.
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_GENERAL_ERROR   Error initializing OSA. Memory allocations
 *                                  failed.
*/
int osa_init(void);

/*!
 * @ingroup libosa
 * @brief   Deinitialize the OSA library
 * @details Free the OSA structures
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_GENERAL_ERROR   Error deinitializing OSA. Memory free failed.
*/
int osa_deinit(void);

#endif

