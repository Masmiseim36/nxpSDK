/*! *********************************************************************************
 * \addtogroup GATT_DB
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
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

/************************************************************************************
*************************************************************************************
* Default local configuration
*************************************************************************************
************************************************************************************/
#ifndef gGattDbDynamic_d
#define gGattDbDynamic_d    0
#endif

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "gatt_database.h"
#include "gatt_types.h"
#include "gap_types.h"
#include "board.h"

#if gGattDbDynamic_d
#include "gatt_db_dynamic.h"
#else
/*! Macros and X-Macros */
#include "gatt_db_macros.h"
#include "gatt_db_x_macros.h"
#include "gatt_db_handles.h"
#endif



/************************************************************************************
*************************************************************************************
* X-Macro expansions - enums, structs and memory allocations
*************************************************************************************
************************************************************************************/
#if !gGattDbDynamic_d

/*! Allocate custom 128-bit UUIDs, if any */
#include "gatt_uuid_def_x.h"

/*! Allocate the arrays for Attribute Values */
#include "gatt_alloc_x.h"

/*! Declare the Attribute database */
gattDbAttribute_t static_gattDatabase[] = {
#include "gatt_decl_x.h"   
};

gattDbAttribute_t* gattDatabase = static_gattDatabase;

/*! Declare structure to compute the database size */
typedef struct sizeCounterStruct_tag {
#include "gatt_size_x.h"
} sizeCounterStruct_t;

/*! Compute the database size at compile time */
#define localGattDbAttributeCount_d  ((sizeof(sizeCounterStruct_t))/4)
uint16_t gGattDbAttributeCount_c;

#else
gattDbAttribute_t*  gattDatabase;
uint16_t            gGattDbAttributeCount_c;
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Function performing runtime initialization of the GATT database.
*
* \remarks  This function should be called only once at device startup.
*
********************************************************************************** */
bleResult_t GattDb_Init() {
#if !gGattDbDynamic_d
    static bool_t mAlreadyInit = FALSE;
    if (mAlreadyInit)
    {
        return gBleAlreadyInitialized_c;
    }
    mAlreadyInit = TRUE;
    
    /*! Assign the database size to the global */
    gGattDbAttributeCount_c = localGattDbAttributeCount_d;

    /*! Attribute-specific initialization by X-Macro expansion */
#include "gatt_init_x.h"
    
    return gBleSuccess_c;
#else
    return GattDbDynamic_Init();
#endif
}

/*! *********************************************************************************
* \brief    Database searching function, return the index for a given attribute handle.
*
* \param[in] handle  The attribute handle.
*
* \return  The index of the given attribute in the database or gGattDbInvalidHandleIndex_d.
*
********************************************************************************** */
uint16_t GattDb_GetIndexOfHandle(uint16_t handle)
{
    uint16_t init = (handle >= gGattDbAttributeCount_c) ? (gGattDbAttributeCount_c - 1) : handle;
    for (uint16_t j = init; j != 0xFFFF && gattDatabase[j].handle >= handle; j--)
    {
        if (gattDatabase[j].handle == handle)
        {
            return j;
        }
    }
    return gGattDbInvalidHandleIndex_d;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
