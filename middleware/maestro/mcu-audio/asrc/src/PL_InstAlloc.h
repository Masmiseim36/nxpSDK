/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PL_INSTALLOC_H_
#define PL_INSTALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "PL_platformTypesPrivate.h"
#include "PL_ModuleInterface.h"

#define MEMORY_ALIGNMENT 8

#define INSTALLOC_OFFSET ((PL_UINTPTR)(MEMORY_ALIGNMENT - 1))
#define INSTALLOC_MASK   ((PL_UINTPTR)~INSTALLOC_OFFSET)

#define INSTALLOC_ALIGN(ptr) (((PL_UINTPTR)ptr + INSTALLOC_OFFSET) & INSTALLOC_MASK)

/*######################################################################################*/
/*  Type declarations                                                                   */
/*######################################################################################*/
typedef struct
{
    uintptr_t TotalSize; /*  Accumulative total memory size                      */
    void *pNextMember;   /*  Pointer to the next instance member to be allocated */
} INST_ALLOC;

/*######################################################################################*/
/*  Function prototypes                                                          */
/*######################################################################################*/

/****************************************************************************************
*  Name        : InstAlloc_Init()
*  Input       : pms  - Pointer to the INST_ALLOC instance
StartAddr - Base address of the instance memory
*  Returns     : Error code
*  Description : Initializes the instance distribution and memory size calculation function
*  Remarks     :
****************************************************************************************/

void PL_InstAlloc_Init(INST_ALLOC *pms, void *StartAddr);

/****************************************************************************************
*  Name        : InstAlloc_AddMember()
*  Input       : pms  - Pointer to the INST_ALLOC instance
Size - The size in bytes of the new added member
*  Returns     : A pointer to the new added member
*  Description : Allocates space for a new member in the instance memory and returns
a pointer to this new member.  The start address of all members will
be 32 bit alligned.
*  Remarks     :
****************************************************************************************/

void *PL_InstAlloc_AddMember(INST_ALLOC *pms, uintptr_t Size);

/****************************************************************************************
 *  Name        : InstAlloc_GetTotal()
 *  Input       : pms  - Pointer to the INST_ALLOC instance
 *  Returns     : The instance memory size
 *  Description : This functions returns the calculated instance memory size
 *  Remarks     :
 ****************************************************************************************/

uintptr_t PL_InstAlloc_GetTotal(INST_ALLOC *pms);

/****************************************************************************************
 *  Name        : PL_InstAlloc_AddSubmoduleMemory()
 *  Arguments   : ModuleMemory [OUT] - Array of memory regions for the parent module
 *              : pSubmoduleGetMemoryTable [IN] - Function pointer to GetMemoryTable function of submodule
 *              : pSubmodule_InstParams [IN] - Pointer to pre-filled instance parameters of submodule
 *  Returns     : PL_SUCCESS if successfull, otherwise the error code depends on the submodule
 *  Description : This function updates the ModuleMemory table by adding all memories required
 *                by the submodule. The scratch memory (PL_TEMPORARY_FAST) required by the
 *                submodule is not simply added. In contrast, the scratch memory of the
 *                parent module is only extended to make sure that it can contain that
 *                of the submodule.
 *  Remarks     :
 ****************************************************************************************/
PL_ReturnStatus_en PL_InstAlloc_AddSubmoduleMemory(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                   PL_GetMemoryTableInterface pGetMemoryTable,
                                                   void const *const pSubModule_InstParams);

/****************************************************************************************
 *  Name        : PL_InstAlloc_AddSubmoduleInstance()
 *  Arguments   : ModuleMemory [OUT] - Array of memory regions for the parent module
 *              : pSubmoduleGetMemoryTable [IN] - Function pointer to GetMemoryTable function of submodule
 *              : PL_GetInstanceHandleInterface [IN] - Function pointer to GetInstanceHandle function of submodule
 *              : ppSubModuleInstance [OUT] - Pointer to Handle of instance of submodule (is created)
 *              : pSubmodule_InstParams [IN] - Pointer to pre-filled instance parameters of submodule
 *  Returns     : PL_SUCCESS if successfull, otherwise the error code depends on the submodule
 *  Description : This function creates an instance of the submodule using the memory provided
 *                the parent module (via ModuleMemory). The handle to the created handle is
 *                returned in ppSubModuleInstance.
 *                This function also updates the ModuleMemory table by adding all memories required
 *                by the submodule. The scratch memory (PL_TEMPORARY_FAST) required by the
 *                submodule is not simply added. In contrast, the scratch memory of the
 *                parent module is only extended to make sure that it can contain that
 *                of the submodule.
 *  Remarks     :
 ****************************************************************************************/
PL_ReturnStatus_en PL_InstAlloc_AddSubmoduleInstance(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                     PL_GetMemoryTableInterface pGetMemoryTable,
                                                     PL_GetInstanceHandleInterface pGetInstanceHandle,
                                                     void **ppSubModuleInstance,
                                                     void const *const pSubModule_InstParams);

/****************************************************************************************
 *  Name        : PL_InstAlloc_AddSimpleSubmoduleMemory()
 *  Arguments   : ModuleMemory [OUT] - Array of memory regions for the parent module
 *              : pSubmoduleGetMemoryTable [IN] - Function pointer to GetMemoryTable function of submodule
 *  Returns     : PL_SUCCESS if successfull, otherwise the error code depends on the submodule
 *  Description : This function updates the ModuleMemory table by adding all memories required
 *                by the submodule. The scratch memory (PL_TEMPORARY_FAST) required by the
 *                submodule is not simply added. In contrast, the scratch memory of the
 *                parent module is only extended to make sure that it can contain that
 *                of the submodule.
 *  Remarks     : This function is similar to PL_InstAlloc_AddSubmoduleMemory but this
 *                function is intended for submodules without instance parameters
 ****************************************************************************************/
PL_ReturnStatus_en PL_InstAlloc_AddSimpleSubmoduleMemory(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                         PL_GetMemoryTableSimpleInterface pGetMemoryTable);

/****************************************************************************************
 *  Name        : PL_InstAlloc_AddSimpleSubmoduleInstance()
 *  Arguments   : ModuleMemory [OUT] - Array of memory regions for the parent module
 *              : pSubmoduleGetMemoryTable [IN] - Function pointer to GetMemoryTable function of submodule
 *              : PL_GetInstanceHandleInterface [IN] - Function pointer to GetInstanceHandle function of submodule
 *              : ppSubModuleInstance [OUT] - Pointer to Handle of instance of submodule (is created)
 *  Returns     : PL_SUCCESS if successfull, otherwise the error code depends on the submodule
 *  Description : This function creates an instance of the submodule using the memory provided
 *                the parent module (via ModuleMemory). The handle to the created handle is
 *                returned in ppSubModuleInstance.
 *                This function also updates the ModuleMemory table by adding all memories required
 *                by the submodule. The scratch memory (PL_TEMPORARY_FAST) required by the
 *                submodule is not simply added. In contrast, the scratch memory of the
 *                parent module is only extended to make sure that it can contain that
 *                of the submodule.
 *  Remarks     : This function is similar to PL_InstAlloc_AddSubmoduleInstance but this
 *                function is intended for submodules without instance parameters
 ****************************************************************************************/
PL_ReturnStatus_en PL_InstAlloc_AddSimpleSubmoduleInstance(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                           PL_GetMemoryTableSimpleInterface pGetMemoryTable,
                                                           PL_GetInstanceHandleSimpleInterface pGetInstanceHandle,
                                                           void **ppSubModuleInstance);

void *PL_InstAlloc_AddMemberAllRet(INST_ALLOC *pms, uintptr_t Size[], void **ptr);

void PL_InstAlloc_AddMemberAll(INST_ALLOC *pms, uintptr_t Size[], PL_MemoryTable_st *pMemoryTable);

void PL_InstAlloc_InitAll(INST_ALLOC *pms, PL_MemoryTable_st *pMemoryTable);

void PL_InstAlloc_InitAll_NULL(INST_ALLOC *pms);

PL_ReturnStatus_en PL_InstAlloc_CheckMemoryOverFlow(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                    PL_MemoryTable_st *pMemoryTable);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PL_INSTALLOC_H_ */
