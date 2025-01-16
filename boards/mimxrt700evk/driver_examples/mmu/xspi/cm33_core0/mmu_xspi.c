/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_mmu.h"
#include "fsl_debug_console.h"

#include "app.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_UpdateTestBuffer(uint32_t muxFactor);
static void DEMO_PrintTableHeader(void);
static void DEMO_PrintResult(uint32_t addr, int32_t readRet, int32_t writeRet);
static void DEMO_MMU_SimpleMap(void);
static void DEMO_MMU_RegionMap(void);
static void DEMO_MMU_ArrayMap(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t testBuffer[DEMO_TEST_BUFFER_LEN];
/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_MMU_IRQ_HANDLER(void)
{
    uint32_t status = MMU_GetStatusFlags(DEMO_MMU_INSTANCE);

    if ((status & kMMU_LUTInvalidFlag) != 0UL)
    {
        /* Access to an invalid item in LUT SRAM. */
        PRINTF("The word of selected pointer of LUT is marked as invalid!\r\n");
    }

    if ((status & kMMU_AddressInvalidFlag) != 0UL)
    {
        /* The AHB access is out of valid address range. */
        PRINTF("Input Address Out Of Range!!!\r\n");
        mmu_ahb_error_record_t ahbErrorRecord;

        MMU_GetAHBErrorRecord(DEMO_MMU_INSTANCE, &ahbErrorRecord);
        PRINTF("AHB Address: %x\r\n", ahbErrorRecord.failAddr);
        PRINTF("Master Id: %d\r\n", ahbErrorRecord.masterId);
        if (ahbErrorRecord.accessType == kMMU_ReadAccess)
        {
            PRINTF("Access Type: AHB Read\r\n");
        }
        else
        {
            PRINTF("Access Type: AHB Write\r\n");
        }

        if (ahbErrorRecord.errorType == kMMU_LUTInvalidError)
        {
            PRINTF("LUT Invalid Error\r\n");
        }
        else if (ahbErrorRecord.errorType == kMMU_AddrInvalidError)
        {
            PRINTF("Invalid Address Error\r\n");
        }
        else
        {
            PRINTF("Parity Error\r\n");
        }
    }

    if ((status & kMMU_ParityErrorFlag) != 0UL)
    {
        PRINTF("A parity error detected in the access address!\r\n");
    }

    if ((status & kMMU_LockErrorFlag) != 0UL)
    {
        /* A lock error detected. */
        PRINTF("Attemp to update the locked LUT SRAM!");
    }

    if ((status & kMMU_IpsReadErrorFlag) != 0UL)
    {
        /* IPS read error detected. */
        PRINTF("Read Error detected in %d\r\n", MMU_GetIPSReadErrorAddress(DEMO_MMU_INSTANCE));
    }

    MMU_ClearStatusFlags(DEMO_MMU_INSTANCE, status);
}

int main(void)
{
    mmu_config_t mmuConfig;

    BOARD_InitHardware();

    PRINTF("MMU XSPI EXAMPLE\r\n");
    MMU_GetDefaultConfig(&mmuConfig);

    mmuConfig.unitSize           = DEMO_MMU_UNITE_SIZE;
    mmuConfig.pageOpt           = kMMU_PageOpt_1Page;
    mmuConfig.invalidAddrMode    = kMMU_InvalidAddrAllowed;
    mmuConfig.baseAddr           = DEMO_MMU_BASE_ADDR;
    mmuConfig.maxAddr            = DEMO_MMU_MAX_ADDR;
    mmuConfig.ptrPhyAddrArray    = NULL;
    mmuConfig.ptrVirtualAddrArray = NULL;
    mmuConfig.enableMMP = false;
         
    MMU_Init(DEMO_MMU_INSTANCE, &mmuConfig);

    MMU_EnableInterrupts(DEMO_MMU_INSTANCE, kMMU_AllInterruptsEnable);
    EnableIRQ(DEMO_IRQN);
    
    DEMO_MMU_SimpleMap();
    DEMO_MMU_RegionMap();
    DEMO_MMU_ArrayMap();
    while (1)
    {
    }
}

static void DEMO_UpdateTestBuffer(uint32_t muxFactor)
{
    for (uint32_t i = 0UL; i < DEMO_TEST_BUFFER_LEN; i++)
    {
        testBuffer[i] = i * muxFactor;
    }
}

static void DEMO_PrintTableHeader(void)
{
    PRINTF("\t|----------------------------------|\r\n");
    PRINTF("\t|%-12s|%-10s|%-10s|\r\n", "Virtual Addr", " Read", " Write");
    PRINTF("\t|----------------------------------|\r\n");
}

static void DEMO_PrintResult(uint32_t addr, int32_t readRet, int32_t writeRet)
{
    
  PRINTF("\t| 0x%08x |%-10s|%-10s|\r\n", addr, (readRet == 0) ? "Success" : "Fail", (writeRet == 0UL) ? "Success" : "Fail");
  PRINTF("\t|%12s|%10s|%10s|\r\n", "------------", "----------", "----------");
}
static void DEMO_MMU_SimpleMap(void)
{
    uint32_t virtualAddr = DEMO_SIMPLE_MAP_VIRT_ADDR;
    uint32_t physicalAddr = DEMO_SIMPLE_MAP_PHYS_ADDR;
    PRINTF("Map virtual address 0x%x to physical address 0x%x\r\n", virtualAddr, physicalAddr);
    DEMO_PrintTableHeader();
    DEMO_CLEAN_INVALIDA_CACHE;
    /* Need to valid original mapping, otherwise, AHB access to orginal physical address cause busfault. */
    MMU_SetupSimpleMap(DEMO_MMU_INSTANCE, physicalAddr, physicalAddr);
    /* Setup mapping from virtual address to physical address. */
    MMU_SetupSimpleMap(DEMO_MMU_INSTANCE, virtualAddr, physicalAddr);
    MMU_EnableMemoryMap(DEMO_MMU_INSTANCE, true);

    DEMO_UpdateTestBuffer(1UL);
    memcpy((void*)physicalAddr, testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
    DEMO_CLEAN_INVALIDA_CACHE;
    int32_t readRet = memcmp((void *)virtualAddr, testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);

    DEMO_UpdateTestBuffer(2UL);
    memcpy((void *)virtualAddr, testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
    DEMO_CLEAN_INVALIDA_CACHE;
    int32_t writeRet = memcmp((void *)physicalAddr, testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
    DEMO_PrintResult(virtualAddr, readRet, writeRet);
    
    MMU_TearDownSimpleMap(DEMO_MMU_INSTANCE, virtualAddr);
    MMU_EnableMemoryMap(DEMO_MMU_INSTANCE, false);
}


static void DEMO_MMU_RegionMap(void)
{
    uint32_t virtualRegionStartAddr = DEMO_REGION_MAP_VIRT_ADDR;
    uint32_t physicalRegionStartAddr = DEMO_REGION_MAP_PHYS_ADDR;
    
    PRINTF("Map virtual region [0x%x ~ 0x%x] to physical region [0x%x ~ 0x%x]\r\n",
           virtualRegionStartAddr, virtualRegionStartAddr + DEMO_REGION_SIZE, 
           physicalRegionStartAddr, physicalRegionStartAddr + DEMO_REGION_SIZE);
    DEMO_PrintTableHeader();
    
    DEMO_CLEAN_INVALIDA_CACHE;
    /* Need to valid original mapping, otherwise, AHB access to orginal physical address cause busfault. */
    MMU_SetupRegionMap(DEMO_MMU_INSTANCE, physicalRegionStartAddr, physicalRegionStartAddr, DEMO_REGION_SIZE);
    MMU_SetupRegionMap(DEMO_MMU_INSTANCE, virtualRegionStartAddr, physicalRegionStartAddr, DEMO_REGION_SIZE);
    MMU_EnableMemoryMap(DEMO_MMU_INSTANCE, true);
    
    int32_t readRet = 0;
    int32_t writeRet = 0;
    for (uint32_t i = 0UL; i < (DEMO_REGION_SIZE / DEMO_UNIT_BYTE_SIZE); i++)
    {
      DEMO_UpdateTestBuffer(1UL);
      memcpy((void *)physicalRegionStartAddr, testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
      DEMO_CLEAN_INVALIDA_CACHE;
      readRet = memcmp((void *)virtualRegionStartAddr, testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
 
      DEMO_UpdateTestBuffer(i);
      memcpy((void *)virtualRegionStartAddr, testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
      DEMO_CLEAN_INVALIDA_CACHE;
      writeRet = memcmp((void *)physicalRegionStartAddr, testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
      
      DEMO_PrintResult(virtualRegionStartAddr, readRet, writeRet);
      
      physicalRegionStartAddr += DEMO_UNIT_BYTE_SIZE;
      virtualRegionStartAddr  += DEMO_UNIT_BYTE_SIZE;
    }
    
    MMU_TearDownRegionMap(DEMO_MMU_INSTANCE, DEMO_REGION_MAP_VIRT_ADDR, DEMO_REGION_SIZE);
    MMU_EnableMemoryMap(DEMO_MMU_INSTANCE, false);
}

static void DEMO_MMU_ArrayMap(void)
{
    uint32_t virtArray[DEMO_MMU_ADDR_ARRAY_SIZE] = DEMO_MMU_VIRTUAL_ADDR_ARRAY;
    uint32_t physArray[DEMO_MMU_ADDR_ARRAY_SIZE] = DEMO_MMU_PHY_ADDR_ARRAY;
    
    PRINTF("Map virtual array %s to physical array %s\r\n",DEMO_MMU_VIRTUAL_ADDR_STR, DEMO_MMU_PHY_ADDR_STR);
    DEMO_PrintTableHeader();
    DEMO_CLEAN_INVALIDA_CACHE;
    MMU_SetupArrayMap(DEMO_MMU_INSTANCE, virtArray, physArray, DEMO_MMU_ADDR_ARRAY_SIZE);
    MMU_EnableMemoryMap(DEMO_MMU_INSTANCE, true);
    
    int32_t readRet = 0;
    int32_t writeRet = 0;
    for (uint32_t i = 0UL; i < DEMO_MMU_ADDR_ARRAY_SIZE; i++)
    {
        DEMO_UpdateTestBuffer(10UL);
        memcpy((void *)(physArray[i]), testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
        DEMO_CLEAN_INVALIDA_CACHE;
        readRet = memcmp((void *)(virtArray[i]), testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
        
        DEMO_UpdateTestBuffer(i);
        memcpy((void *)(virtArray[i]), testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
        DEMO_CLEAN_INVALIDA_CACHE;
        writeRet = memcmp((void *)(physArray[i]), testBuffer, 4UL * DEMO_TEST_BUFFER_LEN);
        
        DEMO_PrintResult(virtArray[i], readRet, writeRet);
    }
    MMU_TearDownArrayMap(DEMO_MMU_INSTANCE, virtArray, DEMO_MMU_ADDR_ARRAY_SIZE);
    MMU_EnableMemoryMap(DEMO_MMU_INSTANCE, false);
}
