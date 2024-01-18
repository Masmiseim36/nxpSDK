/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "PL_InstAlloc.h"

/****************************************************************************************/
/*                                                                                      */
/*  Define code section                                                                 */
/*                                                                                      */
/****************************************************************************************/
#define InitSection
#define LibInstAlloc
// #include    "PL_CodeSectionDef.h"

/****************************************************************************************
*  Name        : PL_InstAlloc_Init()
*  Input       : pms  - Pointer to the INST_ALLOC instance
StartAddr - Base address of the instance memory
*  Returns     : Error code
*  Description : Initializes the instance distribution and memory size calculation function
*  Remarks     :
****************************************************************************************/

void PL_InstAlloc_Init(INST_ALLOC *pms, void *StartAddr)
{
    pms->pNextMember = (void *)(((PL_UINTPTR)StartAddr + INSTALLOC_OFFSET) & INSTALLOC_MASK);
    pms->TotalSize   = INSTALLOC_OFFSET;
}

/****************************************************************************************
*  Name        : PL_InstAlloc_AddMember()
*  Input       : pms  - Pointer to the INST_ALLOC instance
Size - The size in bytes of the new added member
*  Returns     : A pointer to the new added member
*  Description : Allocates space for a new member in the instance memory and returns
a pointer to this new member.  The start address of all members will
be 32 bit alligned.
*  Remarks     :
****************************************************************************************/

void *PL_InstAlloc_AddMember(INST_ALLOC *pms, uintptr_t Size)
{
    void *pNewMemberAddress; /* Variable to temporarily store the return value */

    pNewMemberAddress = pms->pNextMember;

    Size = (uintptr_t)((Size + INSTALLOC_OFFSET) & INSTALLOC_MASK); /* Ceil the size to a multiple of four */

    pms->TotalSize += Size;
    pms->pNextMember = (void *)((PL_UINTPTR)pms->pNextMember + Size);

    return (pNewMemberAddress);
}

/****************************************************************************************
 *  Name        : PL_InstAlloc_GetTotal()
 *  Input       : pms  - Pointer to the INST_ALLOC instance
 *  Returns     : The instance memory size
 *  Description : This functions returns the calculated instance memory size
 *  Remarks     :
 ****************************************************************************************/

uintptr_t PL_InstAlloc_GetTotal(INST_ALLOC *pms)
{
    if (pms->TotalSize > INSTALLOC_OFFSET)
    {
        return (pms->TotalSize);
    }
    else
    {
        return 0; /* No memory added */
    }
}

void PL_InstAlloc_InitAll(INST_ALLOC *pms, PL_MemoryTable_st *pMemoryTable)
{
    PL_UINTPTR StartAddr;

    StartAddr          = (PL_UINTPTR)pMemoryTable->Region[PL_PERSISTENT_SLOW_DATA].pBaseAddress;
    pms[0].pNextMember = (void *)((StartAddr + INSTALLOC_OFFSET) & INSTALLOC_MASK);

    StartAddr          = (PL_UINTPTR)pMemoryTable->Region[PL_PERSISTENT_FAST_DATA].pBaseAddress;
    pms[1].pNextMember = (void *)((StartAddr + INSTALLOC_OFFSET) & INSTALLOC_MASK);

    StartAddr          = (PL_UINTPTR)pMemoryTable->Region[PL_PERSISTENT_FAST_COEF].pBaseAddress;
    pms[2].pNextMember = (void *)((StartAddr + INSTALLOC_OFFSET) & INSTALLOC_MASK);

    StartAddr          = (PL_UINTPTR)pMemoryTable->Region[PL_TEMPORARY_FAST].pBaseAddress;
    pms[3].pNextMember = (void *)((StartAddr + INSTALLOC_OFFSET) & INSTALLOC_MASK);

    pms[0].TotalSize = INSTALLOC_OFFSET;
    pms[1].TotalSize = INSTALLOC_OFFSET;
    pms[2].TotalSize = INSTALLOC_OFFSET;
    pms[3].TotalSize = INSTALLOC_OFFSET;
}

/****************************************************************************************
 *  Name        : PL_InstAlloc_InitAll_NULL()
 *  Input       : pms  - Pointer to array of four INST_ALLOC instances
 *  Returns     : Nothing
 *  Description : This function reserves Size of INSTALLOC_OFFSET bytes for all memory
 *                 regions and intializes pNextMember for all regions to 0
 *  Remarks     :
 ****************************************************************************************/

void PL_InstAlloc_InitAll_NULL(INST_ALLOC *pms)
{
    pms[0].TotalSize   = INSTALLOC_OFFSET;
    pms[0].pNextMember = PL_NULL;

    pms[1].TotalSize   = INSTALLOC_OFFSET;
    pms[1].pNextMember = PL_NULL;

    pms[2].TotalSize   = INSTALLOC_OFFSET;
    pms[2].pNextMember = PL_NULL;

    pms[3].TotalSize   = INSTALLOC_OFFSET;
    pms[3].pNextMember = PL_NULL;
}

// Specific WIN64 case
#ifdef _WIN64
#define CAST_UINTPTRT_TO_INT32 (PL_UINT32)
#else
#define CAST_UINTPTRT_TO_INT32
#endif

void PL_InstAlloc_AddMemberAll(INST_ALLOC *pms, uintptr_t Size[], PL_MemoryTable_st *pMemoryTable)
{
    (void)PL_InstAlloc_AddMember(&pms[PL_PERSISTENT_SLOW_DATA], Size[PL_PERSISTENT_SLOW_DATA]);

    pMemoryTable->Region[PL_PERSISTENT_SLOW_DATA].Size =
        CAST_UINTPTRT_TO_INT32 PL_InstAlloc_GetTotal(&pms[PL_PERSISTENT_SLOW_DATA]);
    pMemoryTable->Region[PL_PERSISTENT_SLOW_DATA].Type         = PL_PERSISTENT_SLOW_DATA;
    pMemoryTable->Region[PL_PERSISTENT_SLOW_DATA].pBaseAddress = PL_NULL;

    (void)PL_InstAlloc_AddMember(&pms[PL_PERSISTENT_FAST_DATA], Size[PL_PERSISTENT_FAST_DATA]);

    pMemoryTable->Region[PL_PERSISTENT_FAST_DATA].Size =
        CAST_UINTPTRT_TO_INT32 PL_InstAlloc_GetTotal(&pms[PL_PERSISTENT_FAST_DATA]);
    pMemoryTable->Region[PL_PERSISTENT_FAST_DATA].Type         = PL_PERSISTENT_FAST_DATA;
    pMemoryTable->Region[PL_PERSISTENT_FAST_DATA].pBaseAddress = PL_NULL;

    (void)PL_InstAlloc_AddMember(&pms[PL_PERSISTENT_FAST_COEF], Size[PL_PERSISTENT_FAST_COEF]);

    pMemoryTable->Region[PL_PERSISTENT_FAST_COEF].Size =
        CAST_UINTPTRT_TO_INT32 PL_InstAlloc_GetTotal(&pms[PL_PERSISTENT_FAST_COEF]);
    pMemoryTable->Region[PL_PERSISTENT_FAST_COEF].Type         = PL_PERSISTENT_FAST_COEF;
    pMemoryTable->Region[PL_PERSISTENT_FAST_COEF].pBaseAddress = PL_NULL;

    (void)PL_InstAlloc_AddMember(&pms[PL_TEMPORARY_FAST], Size[PL_TEMPORARY_FAST]);

    pMemoryTable->Region[PL_TEMPORARY_FAST].Size =
        CAST_UINTPTRT_TO_INT32 PL_InstAlloc_GetTotal(&pms[PL_TEMPORARY_FAST]);
    pMemoryTable->Region[PL_TEMPORARY_FAST].Type         = PL_TEMPORARY_FAST;
    pMemoryTable->Region[PL_TEMPORARY_FAST].pBaseAddress = PL_NULL;
}

void *PL_InstAlloc_AddMemberAllRet(INST_ALLOC *pms, uintptr_t Size[], void **ptr)
{
    ptr[0] = PL_InstAlloc_AddMember(&pms[PL_PERSISTENT_SLOW_DATA], Size[PL_PERSISTENT_SLOW_DATA]);
    ptr[1] = PL_InstAlloc_AddMember(&pms[PL_PERSISTENT_FAST_DATA], Size[PL_PERSISTENT_FAST_DATA]);
    ptr[2] = PL_InstAlloc_AddMember(&pms[PL_PERSISTENT_FAST_COEF], Size[PL_PERSISTENT_FAST_COEF]);
    ptr[3] = PL_InstAlloc_AddMember(&pms[PL_TEMPORARY_FAST], Size[PL_TEMPORARY_FAST]);

    return (ptr[0]);
}

static void PL_InstAlloc_AddSubmoduleMemoryCore(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                PL_MemoryTable_st *pSubModule_MemoryTable)
{
    PL_UINT32 ModuleTemporaryMemorySize;
    PL_UINT32 SubModuleTemporaryMemorySize;
    PL_UINTPTR pOrigBase;

    /* Update the memory requirements of the parent module with added submodule */

    // The persistent memory regions are added together and allocated one after the other
    pSubModule_MemoryTable->Region[PL_PERSISTENT_SLOW_DATA].pBaseAddress = PL_InstAlloc_AddMember(
        &ModuleMemory[PL_PERSISTENT_SLOW_DATA], pSubModule_MemoryTable->Region[PL_PERSISTENT_SLOW_DATA].Size);
    pSubModule_MemoryTable->Region[PL_PERSISTENT_FAST_DATA].pBaseAddress = PL_InstAlloc_AddMember(
        &ModuleMemory[PL_PERSISTENT_FAST_DATA], pSubModule_MemoryTable->Region[PL_PERSISTENT_FAST_DATA].Size);
    pSubModule_MemoryTable->Region[PL_PERSISTENT_FAST_COEF].pBaseAddress = PL_InstAlloc_AddMember(
        &ModuleMemory[PL_PERSISTENT_FAST_COEF], pSubModule_MemoryTable->Region[PL_PERSISTENT_FAST_COEF].Size);

    // The temporary memory regions are overlayed, the size of the total memory is set to
    // the largest temporary memory region of the modules
    pOrigBase = (PL_UINTPTR)ModuleMemory[PL_TEMPORARY_FAST].pNextMember - ModuleMemory[PL_TEMPORARY_FAST].TotalSize;
    pSubModule_MemoryTable->Region[PL_TEMPORARY_FAST].pBaseAddress =
        (void *)((pOrigBase + INSTALLOC_OFFSET) & INSTALLOC_MASK);

    ModuleTemporaryMemorySize = (PL_UINT32)((PL_UINTPTR)ModuleMemory[PL_TEMPORARY_FAST].pNextMember -
                                            (PL_UINTPTR)pSubModule_MemoryTable->Region[PL_TEMPORARY_FAST].pBaseAddress);
    SubModuleTemporaryMemorySize =
        (PL_UINT32)((pSubModule_MemoryTable->Region[PL_TEMPORARY_FAST].Size + INSTALLOC_OFFSET) & INSTALLOC_MASK);

    if (SubModuleTemporaryMemorySize > ModuleTemporaryMemorySize)
    {
        ModuleMemory[PL_TEMPORARY_FAST].pNextMember =
            (void *)((PL_UINTPTR)pSubModule_MemoryTable->Region[PL_TEMPORARY_FAST].pBaseAddress +
                     SubModuleTemporaryMemorySize);
        ModuleMemory[PL_TEMPORARY_FAST].TotalSize += (SubModuleTemporaryMemorySize - ModuleTemporaryMemorySize);
    }
}

PL_ReturnStatus_en PL_InstAlloc_AddSubmoduleMemory(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                   PL_GetMemoryTableInterface pGetMemoryTable,
                                                   void const *const pSubModule_InstParams)
{
    PL_ReturnStatus_en SubModule_Status;
    PL_MemoryTable_st SubModule_MemoryTable;

    SubModule_Status = pGetMemoryTable(PL_NULL, &SubModule_MemoryTable, pSubModule_InstParams);
    if (SubModule_Status != PL_SUCCESS)
    {
        return SubModule_Status;
    }

    /* Update the memory requirements of the parent module with added submodule */
    PL_InstAlloc_AddSubmoduleMemoryCore(ModuleMemory, &SubModule_MemoryTable);

    return PL_SUCCESS;
}

PL_ReturnStatus_en PL_InstAlloc_AddSubmoduleInstance(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                     PL_GetMemoryTableInterface pGetMemoryTable,
                                                     PL_GetInstanceHandleInterface pGetInstanceHandle,
                                                     void **ppSubModuleInstance,
                                                     void const *const pSubModule_InstParams)
{
    PL_ReturnStatus_en SubModule_Status;
    PL_MemoryTable_st SubModule_MemoryTable;

    /* Get memory requirements of the submodule */
    SubModule_Status = pGetMemoryTable(PL_NULL, &SubModule_MemoryTable, pSubModule_InstParams);
    if (SubModule_Status != PL_SUCCESS)
    {
        return SubModule_Status;
    }

    /* Update the memory requirements of the parent module with added submodule */
    PL_InstAlloc_AddSubmoduleMemoryCore(ModuleMemory, &SubModule_MemoryTable);

    /* Create the instance of the submodule */
    *ppSubModuleInstance = PL_NULL;
    SubModule_Status     = pGetInstanceHandle(ppSubModuleInstance, &SubModule_MemoryTable, pSubModule_InstParams);
    if (SubModule_Status != PL_SUCCESS)
    {
        return SubModule_Status;
    }
    return PL_SUCCESS;
}

PL_ReturnStatus_en PL_InstAlloc_AddSimpleSubmoduleMemory(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                         PL_GetMemoryTableSimpleInterface pGetMemoryTable)
{
    PL_ReturnStatus_en SubModule_Status;
    PL_MemoryTable_st SubModule_MemoryTable;

    SubModule_Status = pGetMemoryTable(PL_NULL, &SubModule_MemoryTable);

    if (SubModule_Status != PL_SUCCESS)
    {
        return SubModule_Status;
    }

    /* Update the memory requirements of the parent module with added submodule */
    PL_InstAlloc_AddSubmoduleMemoryCore(ModuleMemory, &SubModule_MemoryTable);

    return PL_SUCCESS;
}

PL_ReturnStatus_en PL_InstAlloc_AddSimpleSubmoduleInstance(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                           PL_GetMemoryTableSimpleInterface pGetMemoryTable,
                                                           PL_GetInstanceHandleSimpleInterface pGetInstanceHandle,
                                                           void **ppSubModuleInstance)
{
    PL_ReturnStatus_en SubModule_Status;
    PL_MemoryTable_st SubModule_MemoryTable;

    /* Get memory requirements of the submodule */
    SubModule_Status = pGetMemoryTable(PL_NULL, &SubModule_MemoryTable);
    if (SubModule_Status != PL_SUCCESS)
    {
        return SubModule_Status;
    }

    /* Update the memory requirements of the parent module with added submodule */
    PL_InstAlloc_AddSubmoduleMemoryCore(ModuleMemory, &SubModule_MemoryTable);

    /* Create the instance of the submodule */
    *ppSubModuleInstance = PL_NULL;
    SubModule_Status     = pGetInstanceHandle(ppSubModuleInstance, &SubModule_MemoryTable);
    if (SubModule_Status != PL_SUCCESS)
    {
        return SubModule_Status;
    }
    return PL_SUCCESS;
}

PL_ReturnStatus_en PL_InstAlloc_CheckMemoryOverFlow(INST_ALLOC ModuleMemory[PL_NR_MEMORY_REGIONS],
                                                    PL_MemoryTable_st *pMemoryTable)
{
    /* check whether Memory allocated during GetInstance() is lower than the the MEM reserved (GetMemoryTable())*/

    if (ModuleMemory[PL_PERSISTENT_SLOW_DATA].TotalSize > pMemoryTable->Region[PL_PERSISTENT_SLOW_DATA].Size)
    {
        return PL_ERROR;
    }
    if (ModuleMemory[PL_PERSISTENT_FAST_DATA].TotalSize > pMemoryTable->Region[PL_PERSISTENT_FAST_DATA].Size)
    {
        return PL_ERROR;
    }
    if (ModuleMemory[PL_PERSISTENT_FAST_COEF].TotalSize > pMemoryTable->Region[PL_PERSISTENT_FAST_COEF].Size)
    {
        return PL_ERROR;
    }
    if (ModuleMemory[PL_TEMPORARY_FAST].TotalSize > pMemoryTable->Region[PL_TEMPORARY_FAST].Size)
    {
        return PL_ERROR;
    }

    return PL_SUCCESS;
}
