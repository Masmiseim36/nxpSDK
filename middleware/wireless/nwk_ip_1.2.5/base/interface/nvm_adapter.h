/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
