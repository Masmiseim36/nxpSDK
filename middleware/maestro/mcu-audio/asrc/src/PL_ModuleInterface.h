/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/****************************************************************************************/
/*                                                                                      */
/*  Header file defining the standard  types for use in the application layer           */
/*  interface of all VIT modules                                                        */
/*                                                                                      */
/****************************************************************************************/

#ifndef PL_MODULE_INTERFACE_H
#define PL_MODULE_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "PL_platformTypesPrivate.h"
/****************************************************************************************/
/*                                                                                      */
/*  General definitions                                                                 */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Standard Enumerated types                                                           */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Standard function interfaces                                                        */
/*                                                                                      */
/****************************************************************************************/
/**
@brief Interface definition for retrieving the memory requirements of the module.

This function returns a table of memory records that describe size, type and memory space of all
buffers required by the instance. The number of initialized memory records is defined by
PL_NR_MEMORY_REGIONS. This function is used for two purposes and is called in two different ways:

@li Memory Allocation: When the PL_GetMemoryTableSimpleInterface functions are called with a
NULL instance handle (hInstance = PL_NULL) the function returns the memory requirements. The
base address pointers in the memory table are set to NULL.

@li Memory Free: When called with a non-NULL instance handle (hInstance = a valid instance handle)
the function returns the memory table used when the instance was created.
The base address pointers returned will be those supplied by the calling application when the
memory was allocated and can now be used for freeing memory.

@param  pInstance              Pointer to the instance handle.
@param  pMemoryTable           Pointer to the memory definition table.

@pre The number of memory records in the array defined by pMemoryTable is equal to
@ref PL_NR_MEMORY_REGIONS.
@pre Exactly @ref PL_NR_MEMORY_REGIONS memory records of pMemoryTable are initialized.

@post When this function is called with hInstance = NULL the memory base address pointers
will be NULL on return.
@post When the function is called for freeing memory, hInstance = Instance Handle the
memory table returns the allocated memory and base addresses used during
initialisation.

@return  PL_SUCCESS           when the function call succeeds and the memory table is filled
correctly
@return  PL_NULLADDRESS       when any argument was NULL

@see PL_GetMemoryTableInterface
*/
typedef PL_ReturnStatus_en (*PL_GetMemoryTableSimpleInterface)(void const *const pInstance,
                                                               PL_MemoryTable_st *pMemoryTable);

/**
@brief Interface definition for retrieving the memory requirements of the module.

This function returns a table of memory records that describe size, type and memory space of all
buffers required by the instance. The number of initialized memory records is defined by
PL_NR_MEMORY_REGIONS. This function is used for two purposes and is called in two different ways:

@li Memory Allocation: When the PL_GetMemoryTableInterface functions are called with a
NULL instance handle (hInstance = PL_NULL) the function returns the memory requirements. The
base address pointers in the memory table are set to NULL. All elements of the instance
parameters structure (pointed to by pInstParams) must contain valid values as the memory
requirements are affected by these settings.

@li Memory Free: When called with a non-NULL instance handle (hInstance = a valid instance handle)
the function returns the memory table used when the instance was created.
The base address pointers returned will be those supplied by the calling application when the
memory was allocated and can now be used for freeing memory. The instance parameters (pointed
to by pInstParams) are ignored and the pInstParams parameter may be set to NULL.

@param  pInstance              Pointer to the instance handle.
@param  pMemoryTable            Pointer to the memory definition table.
@param  pInstanceParams         Pointer to the instance parameters.

@pre The number of memory records in the array defined by pMemoryTable is equal to
@ref PL_NR_MEMORY_REGIONS.
@pre Exactly @ref PL_NR_MEMORY_REGIONS memory records of pMemoryTable are initialized.

@post When this function is called with hInstance = NULL the memory base address pointers
will be NULL on return.
@post When the function is called for freeing memory, hInstance = Instance Handle the
memory table returns the allocated memory and base addresses used during
initialisation.

@return  PL_SUCCESS           when the function call succeeds and the memory table is filled
correctly
@return  PL_NULLADDRESS       when pMemoryTable was NULL
@return  PL_NULLADDRESS       when pInstanceParams was NULL and the call was for memory allocation
@return  PL_OUTOFRANGE        when pInstanceParams contains parameters out of the excepted range

@see PL_GetMemoryTableSimpleInterface
*/
typedef PL_ReturnStatus_en (*PL_GetMemoryTableInterface)(void const *const pInstance,
                                                         PL_MemoryTable_st *pMemoryTable,
                                                         void const *const pInstanceParams);

/**
@brief Interface definition to create an instance of a module

This function is used to create an instance of a module. All control parameters are set to invalid
values. The memory table pointed to by pMemoryTable must be created. If the memory table is not
correct then an error will be returned.

@pre The memory records tables defined by pMemoryTable contains pointers to non-overlapping buffers
with the size as requested via the prior calls to the @ref PL_GetMemoryTableInterface functions.

@post pInstance contains the handle to the newly created instance

@param  pInstance               Pointer to the instance handle.
@param  pMemoryTable            Pointer to the memory definition table.

@return PL_SUCCESS           when creation was successful
@return PL_NULLADDRESS       when any argument is NULL.
@return PL_NULLADDRESS       when one or more of the memory regions with a non-zero size has been
given a NULL base address pointer.

@see PL_GetInstanceHandleInterface
*/
typedef PL_ReturnStatus_en (*PL_GetInstanceHandleSimpleInterface)(void *pInstance,
                                                                  PL_MemoryTable_st const *const pMemoryTable);

/**
@brief Interface definition to create an instance of a module

This function is used to create an instance of a module. All control parameters are set to invalid
values. The memory table pointed to by pMemoryTable must be created. If the memory table is not
correct then an error will be returned. The memory requirements of the module are dependent
on the instance parameters supplied and so the instance parameters provided in this function call
must be the same as those used in the @ref PL_GetMemoryTableInterface function calls used for memory
allocation.

@pre The memory records tables defined by pMemoryTable contains pointers to non-overlapping buffers
with the size as requested via the prior calls to the @ref PL_GetMemoryTableInterface functions.
@pre The initialization parameter structure defined by pInstanceParams is identical to the structure
passed to prior call to @ref PL_GetMemoryTableInterface functions.

@post pInstance contains the handle to the newly created instance

@param  pInstance               Pointer to the instance handle.
@param  pMemoryTable            Pointer to the memory definition table.
@param  pInstanceParams         Pointer to the instance parameters.

@return PL_SUCCESS           when creation was successful
@return PL_NULLADDRESS       when phInstance or pMemoryTable or pInstanceParams is NULL.
@return PL_NULLADDRESS       when one or more of the memory regions with a non-zero size has been
given a NULL base address pointer.
@return PL_OUTOFRANGE        when pInstanceParams contains parameters out of the excepted range

@see PL_GetInstanceHandleSimpleInterface
*/
typedef PL_ReturnStatus_en (*PL_GetInstanceHandleInterface)(void *pInstance,
                                                            PL_MemoryTable_st const *const pMemoryTable,
                                                            void const *const pInstanceParams);

/****************************************************************************************/
/*                                                                                      */
/*  Standard function interface structures                                              */
/*                                                                                      */
/****************************************************************************************/
/**
@brief Interface structure for memory management of a simple module

Structure of function pointers that allow managing the memory of
a simple Module. A simple module is a module without instance
parameters.

@see PL_GetInstanceHandleSimpleInterface and PL_GetMemoryTableSimpleInterface
@see PL_ModuleMemoryInterface_st for the same with instance parameters
*/
typedef struct
{
    PL_GetMemoryTableSimpleInterface GetMemoryTable; ///< Function pointer to the GetMemoryTable function of the module
    PL_GetInstanceHandleSimpleInterface
        GetInstanceHandle;     ///< Function pointer to the GetInstanceHandle function of the module
    PL_UINTPTR InstanceOffset; ///< Offset of the instance of the present module inside static of the parent module
} PL_SimpleModuleMemoryInterface_st;

/**
@brief Interface structure for memory management of a module

Structure of function pointers that allow managing the memory of
a Module. The module is supposed to have instance parameters.

@see PL_GetInstanceHandleInterface and PL_GetMemoryTableInterface
@see PL_SimpleModuleMemoryInterface_st for the same without instance parameters
*/
typedef struct
{
    PL_GetMemoryTableInterface GetMemoryTable; ///< Function pointer to the GetMemoryTable function of the module
    PL_GetInstanceHandleInterface
        GetInstanceHandle;                     ///< Function pointer to the GetInstanceHandle function of the module
    PL_UINTPTR InstanceOffset; ///< Offset of the instance of the present module inside static of the parent module
} PL_ModuleMemoryInterface_st;

/****************************************************************************************/
/*                                                                                      */
/*  End of file                                                                         */
/*                                                                                      */
/****************************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PL_MODULE_INTERFACE_H */
