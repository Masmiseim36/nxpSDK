/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _NVM_ADAPTER_H
#define _NVM_ADAPTER_H

/*!
\file       nvm_adapter.h
\brief      This is the header file for the  stack adapter of non volatile flash 
            storage module. 
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/

typedef enum nvngErrCode_tag{
    gNVNGSuccess_c,
    gNVNGFail_c
}nvngErrCode_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn      nvngErrCode_t NVNG_Init(void)
\brief   Interface function for the NVNG module. It performes the initialization of the module.

\return  nvngErrCode_t  Status: gNVNGSuccess_c or gNVNGFail_c
***************************************************************************************************/
nvngErrCode_t NVNG_Init(void);

/*!*************************************************************************************************
\fn     nvngErrCode_t NVNG_ThreadRestore(bool_t startWithNVM)
\brief  Interface function for the NVNG module. It performs the restore of NVM data.

\param [in]     startWithNVM TRUE start with NVM, false do not restore from nvm

\return         nvngErrCode_t  Status: gNVNGSuccess_c or gNVNGFail_c
***************************************************************************************************/
nvngErrCode_t NVNG_ThreadRestore(bool_t startWithNVM);

/*!*************************************************************************************************
\fn void NVNG_Save(void ** ppRam)
\brief  Saves a structure in NVM in idle mode.

\param [in]  ppRam     double pointer to the entity to be saved
***************************************************************************************************/
void NVNG_Save(void ** ppRam);

/*!*************************************************************************************************
\fn void NVNG_SyncSave(void ** ppRam)
\brief  Save synchronously a structure in NVM.

\param [in]  ppRam     double pointer to the entity to be saved
***************************************************************************************************/
void NVNG_SyncSave(void ** ppRam);

/*!*************************************************************************************************
\fn void NVNG_SaveOnInterval(void ** ppRam)
\brief  Save on interval a structure in NVM.

\param [in]  ppRam     double pointer to the entity to be saved
***************************************************************************************************/
void NVNG_SaveOnInterval(void ** ppRam);

/*!*************************************************************************************************
\fn void NVNG_RestoreDataset(void *pDataset, bool_t restoreAll)
\brief  Restore table from NVM.

\param [in]  pDataset     Pointer to dataset
\param [in]  restoreAll   TRUE - restores entire table
                          FALSE - restores one entry
***************************************************************************************************/
void NVNG_RestoreDataset(void *pDataset, bool_t restoreAll);

/*!*************************************************************************************************
\fn     void NVNG_Erase(void ** ppRam)
\brief  Erases an unmirrord dataset.

\param [in]  ppRam     double pointer to the entity to be erased
***************************************************************************************************/
void NVNG_Erase(void ** ppRam);
/*!*************************************************************************************************
\fn      void NVNG_MoveToRam(void ** ppRam)
\brief   Moves in ram an unmirrored dataset.

\param [in]  ppRam     double pointer to the entity to be moved from flash to RAM
***************************************************************************************************/
void NVNG_MoveToRam(void ** ppRam);


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _NVM_ADAPTER_H */
