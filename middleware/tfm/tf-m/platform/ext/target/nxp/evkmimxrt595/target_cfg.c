/*
 * Copyright (c) 2018-2022 Arm Limited. All rights reserved.
 * Copyright (c) 2019-2022 NXP. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "target_cfg.h"
#include "Driver_Common.h"
#include "platform_description.h"
#include "device_definition.h"
#include "region_defs.h"
#include "tfm_plat_defs.h"
#include "region.h"
#include "tfm_assert.h"
#include "tfm_spm_log.h"
#include "utilities.h"

/* The section names come from the scatter file */
REGION_DECLARE(Load$$LR$$, LR_NS_PARTITION, $$Base);
REGION_DECLARE(Image$$, ER_VENEER, $$Base);
REGION_DECLARE(Image$$, VENEER_ALIGN, $$Limit);
#ifdef BL2
REGION_DECLARE(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base);
#endif /* BL2 */

const struct memory_region_limits memory_regions = {
    .non_secure_code_start =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        BL2_HEADER_SIZE,

    .non_secure_partition_base =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base),

    .non_secure_partition_limit =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        NS_PARTITION_SIZE - 1,

    .veneer_base =
        (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),

    .veneer_limit =
        (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit),

#ifdef BL2
    .secondary_partition_base =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base),

    .secondary_partition_limit =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base) +
        SECONDARY_PARTITION_SIZE - 1,
#endif /* BL2 */
};

/* Allows software, via SAU, to define the code region as a NSC */
#define NSCCFG_CODENSC  1

/* Define Peripherals NS address range for the platform */
#define PERIPHERALS_BASE_NS_START (0x40000000)
#define PERIPHERALS_BASE_NS_END   (0x4FFFFFFF)

/* Enable system reset request for CPU 0 */
#define ENABLE_CPU0_SYSTEM_RESET_REQUEST (1U << 4U)

/* To write into AIRCR register, 0x5FA value must be write to the VECTKEY field,
 * otherwise the processor ignores the write.
 */
#define SCB_AIRCR_WRITE_MASK ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos))

struct platform_data_t tfm_peripheral_std_uart = {
        USART0_BASE_NS,
        USART0_BASE_NS + 0xFFF,
        0,
        0
};

struct platform_data_t tfm_peripheral_timer0 = {
        CTIMER2_BASE,
        CTIMER2_BASE + 0xFFF,
        &(AHB_SECURE_CTRL->APB_BRIDGE_PER1_RULE1),
                AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE1_CT32B2_SHIFT
};

enum tfm_plat_err_t enable_fault_handlers(void)
{
    /* Explicitly set secure fault priority to the highest */
    NVIC_SetPriority(SecureFault_IRQn, 0);

    /* Enables BUS, MEM, USG and Secure faults */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk
                  | SCB_SHCSR_BUSFAULTENA_Msk
                  | SCB_SHCSR_MEMFAULTENA_Msk
                  | SCB_SHCSR_SECUREFAULTENA_Msk;
    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t system_reset_cfg(void)
{
    uint32_t reg_value = SCB->AIRCR;

    /* Clear SCB_AIRCR_VECTKEY value */
    reg_value &= ~(uint32_t)(SCB_AIRCR_VECTKEY_Msk);

    /* Enable system reset request only to the secure world */
    reg_value |= (uint32_t)(SCB_AIRCR_WRITE_MASK | SCB_AIRCR_SYSRESETREQS_Msk);

    SCB->AIRCR = reg_value;

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t init_debug(void)
{

#if !defined(DAUTH_CHIP_DEFAULT)
#error "Debug features are set during provisioning. Application is not able to change them as the SYSCTRL->DEBUG_LOCK_EN is locked by the MCU secure boot.  "
#endif
    return TFM_PLAT_ERR_SUCCESS;
}

/*----------------- NVIC interrupt target state to NS configuration ----------*/
enum tfm_plat_err_t nvic_interrupt_target_state_cfg(void)
{
    /* Target every interrupt to NS; unimplemented interrupts will be WI */
    for (uint8_t i=0; i<sizeof(NVIC->ITNS)/sizeof(NVIC->ITNS[0]); i++) {
        NVIC->ITNS[i] = 0xFFFFFFFF;
    }

    /* Make sure that MPC and PPC are targeted to S state */
    NVIC_ClearTargetState(SEC_VIO_IRQn);

    return TFM_PLAT_ERR_SUCCESS;
}

/*----------------- NVIC interrupt enabling for S peripherals ----------------*/
enum tfm_plat_err_t nvic_interrupt_enable(void)
{

    /* MPC/PPC interrupt enabling */

    NVIC_EnableIRQ(SEC_VIO_IRQn);

    return TFM_PLAT_ERR_SUCCESS;
}

/*------------------- SAU/IDAU configuration functions -----------------------*/

void sau_and_idau_cfg(void)
{
    /* Enables SAU */
    TZ_SAU_Enable();

    /* Configures SAU regions to be non-secure */
    SAU->RNR  = 0U;
    SAU->RBAR = (memory_regions.non_secure_partition_base
                & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.non_secure_partition_limit
                & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk;

    SAU->RNR  = 1U;
    SAU->RBAR = (NS_DATA_START & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (NS_DATA_LIMIT & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;

    /* Configures veneers region to be non-secure callable */
    SAU->RNR  = 2U;
    SAU->RBAR = (memory_regions.veneer_base  & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.veneer_limit & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk
                | SAU_RLAR_NSC_Msk;

    /* Configure the peripherals space */
    SAU->RNR  = 3U;
    SAU->RBAR = (PERIPHERALS_BASE_NS_START & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (PERIPHERALS_BASE_NS_END & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk;

#ifdef BL2
    /* Secondary image partition */
    SAU->RNR  = 4U;
    SAU->RBAR = (memory_regions.secondary_partition_base  & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.secondary_partition_limit & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk;
#endif /* BL2 */

#if TARGET_DEBUG_LOG
    SPMLOG_DBGMSG("=== [SAU NS] =======\r\n");
    SPMLOG_DBGMSGVAL("NS ROM starts from : ",
                                      memory_regions.non_secure_partition_base);
    SPMLOG_DBGMSGVAL("NS ROM ends at : ",
                                      memory_regions.non_secure_partition_base +
                                     memory_regions.non_secure_partition_limit);
    SPMLOG_DBGMSGVAL("NS DATA start from : ", NS_DATA_START);
    SPMLOG_DBGMSGVAL("NS DATA ends at : ", NS_DATA_START + NS_DATA_LIMIT);
    SPMLOG_DBGMSGVAL("NSC starts with : ", memory_regions.veneer_base);
    SPMLOG_DBGMSGVAL("NSC ends at : ", memory_regions.veneer_base +
                                       memory_regions.veneer_limit);
    SPMLOG_DBGMSGVAL("PERIPHERALS starts with : ", PERIPHERALS_BASE_NS_START);
    SPMLOG_DBGMSGVAL("PERIPHERALS ends at : ", PERIPHERALS_BASE_NS_END);
#endif
}

/*------------------- Memory configuration functions -------------------------*/

int32_t mpc_init_cfg(void)
{
    uint32_t ns_region_id = 0;
    uint32_t ns_region_start_id = 0;
    uint32_t ns_region_end_id = 0;
    int i;

    /*
    * Starts changing actual configuration so issue DMB to ensure every
    * transaction has completed by now
    */
    __DMB();

    /* Configuration of AHB Secure Controller
     * Possible values for every memory sector or peripheral rule:
     *  0    Non-secure, user access allowed.
     *  1    Non-secure, privileged access allowed.
     *  2    Secure, user access allowed.
     *  3    Secure, privileged access allowed. */

    /* == ROM region == */

    /* Security access rules for boot ROM memory sectors. Each sector is 8 KB. Up to 32 sectors are supported. */
    /* Security control ROM memory configuration (0x3 = all regions set to secure and privileged user access). */
    for(i=0; i < 4; i++)
    {
        AHB_SECURE_CTRL->ROM_MEM_RULE[i] = 0x33333333U;
    }
    
    /* == Flash region == */

    /* FlexSPI0 0x0800 0000 to 0x0FFF FFFF 128 MB
            Region 0: 8 MB (32 * 256 KB)
            Region 1: 8 MB (4 * 2 MB)
            Region 2: 16 MB (4 * 4 MB)
            Region 3: 32 MB (4 * 8 MB)
            Region 4: 64 MB (4 * 16 MB) */

    /* 1) Set FLASH memory security access rule configuration to init value (0x3 = all regions set to secure and privileged user access) */
    for(i=0; i < 4; i++)
    {
        AHB_SECURE_CTRL->FLEXSPI0_REGION0_RULE[i] = 0x33333333U;
        AHB_SECURE_CTRL->FLEXSPI0_REGION1_4_RULE[i].FLEXSPI0_REGION_RULE0 = 0x00003333U;
    }
    
    /* 2) Set FLASH memory security access rule configuration (set to non-secure and non-privileged user access allowed).*/
    
    /* == Region 0 == */
    /* The regions have to be alligned to FLASH_REGION0_SUBREGION_SIZE to cover the AHB Flash Region. */
    TFM_ASSERT(memory_regions.non_secure_partition_base >= NS_ROM_ALIAS_BASE);
    TFM_ASSERT(((memory_regions.non_secure_partition_base - NS_ROM_ALIAS_BASE) % FLASH_REGION0_SUBREGION_SIZE) == 0);
    TFM_ASSERT(((memory_regions.non_secure_partition_limit - NS_ROM_ALIAS_BASE +1) % FLASH_REGION0_SUBREGION_SIZE) == 0);

    ns_region_start_id = (memory_regions.non_secure_partition_base - NS_ROM_ALIAS_BASE)/FLASH_REGION0_SUBREGION_SIZE;
    ns_region_end_id = (memory_regions.non_secure_partition_limit - NS_ROM_ALIAS_BASE + 1)/FLASH_REGION0_SUBREGION_SIZE;
   
    /* Set to non-secure and non-privileged user access allowed */
    for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++) /* == Region 0 == */
    {
        if(ns_region_id < 8) 
        {
            /* Set regions in the AHB controller for flash memory 0x0800 0000 - 0x081F FFFF */
            AHB_SECURE_CTRL->FLEXSPI0_REGION0_RULE[0] &= ~(0xF << (ns_region_id*4));
        }
        else if((ns_region_id >= 8) && (ns_region_id < 16))
        {
            /* Set regions in the AHB controller for flash memory 0x0820 0000 - 0x083F FFFF */
            AHB_SECURE_CTRL->FLEXSPI0_REGION0_RULE[1] &= ~(0xF << ((ns_region_id-8)*4));
        }else if((ns_region_id >= 16) && (ns_region_id < 24))
        {
            /* Set regions the AHB controller for flash memory 0x0840 0000 - 0x085F FFFF */
            AHB_SECURE_CTRL->FLEXSPI0_REGION0_RULE[2] &= ~(0xF << ((ns_region_id-16)*4));
        }
        else if((ns_region_id >= 24) && (ns_region_id < 32))
        {
            /* Set regions the AHB controller for flash memory 0x0860 0000 -  0x087 FFFFF */
            AHB_SECURE_CTRL->FLEXSPI0_REGION0_RULE[3] &= ~(0xF << ((ns_region_id-32)*4));
        }
        else /* == Region 1 == */
        {
            if(ns_region_start_id > FLASH_REGION0_SUBREGION_NUMBER)
            {
                ns_region_start_id = (memory_regions.non_secure_partition_base - NS_ROM_ALIAS_BASE - FLASH_REGION0_SIZE )/FLASH_REGION1_SUBREGION_SIZE;
            }
            else
            {
                ns_region_start_id = 0;   
            }
            
            ns_region_end_id = (memory_regions.non_secure_partition_limit - NS_ROM_ALIAS_BASE - FLASH_REGION0_SIZE + 1)/FLASH_REGION1_SUBREGION_SIZE;
            
            for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++) 
            {
                if(ns_region_id < FLASH_REGION1_SUBREGION_NUMBER)
                {
                    /* Set regions in the AHB controller for flash memory 0x880 0000 - 0x8FF FFFF */
                    AHB_SECURE_CTRL->FLEXSPI0_REGION1_4_RULE[0].FLEXSPI0_REGION_RULE0 &= ~(0xF << (ns_region_id*4));
                }
                else /* == Region 2 == */
                {
                    if(ns_region_start_id > FLASH_REGION1_SUBREGION_NUMBER)
                    {
                        ns_region_start_id = (memory_regions.non_secure_partition_base - NS_ROM_ALIAS_BASE - (FLASH_REGION0_SIZE+FLASH_REGION1_SIZE) )/FLASH_REGION2_SUBREGION_SIZE;
                    }
                    else
                    {
                        ns_region_start_id = 0;   
                    }
                    
                    ns_region_end_id = (memory_regions.non_secure_partition_limit - NS_ROM_ALIAS_BASE - (FLASH_REGION0_SIZE+FLASH_REGION1_SIZE) + 1)/FLASH_REGION2_SUBREGION_SIZE;
                    
                    for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++) 
                    {
                        if(ns_region_id < FLASH_REGION2_SUBREGION_NUMBER)
                        {
                            /* Set regions in the AHB controller for flash memory 0x0900 0000 - 0x9FF FFFF */
                            AHB_SECURE_CTRL->FLEXSPI0_REGION1_4_RULE[1].FLEXSPI0_REGION_RULE0 &= ~(0xF << (ns_region_id*4));
                        }
                        else/* == Region 3 == */
                        {
                            if(ns_region_start_id > FLASH_REGION2_SUBREGION_NUMBER)
                            {
                                ns_region_start_id = (memory_regions.non_secure_partition_base - NS_ROM_ALIAS_BASE - (FLASH_REGION0_SIZE+FLASH_REGION1_SIZE+FLASH_REGION2_SIZE) )/FLASH_REGION3_SUBREGION_SIZE;
                            }
                            else
                            {
                                ns_region_start_id = 0;   
                            }
                        
                            ns_region_end_id = (memory_regions.non_secure_partition_limit - NS_ROM_ALIAS_BASE - (FLASH_REGION0_SIZE+FLASH_REGION1_SIZE+FLASH_REGION2_SIZE) + 1)/FLASH_REGION3_SUBREGION_SIZE;

                            for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++) 
                            {
                                if(ns_region_id < FLASH_REGION3_SUBREGION_NUMBER)
                                {
                                    /* Set regions in the AHB controller for flash memory 0x0A00 0000 - 0xBFF FFFF */
                                    AHB_SECURE_CTRL->FLEXSPI0_REGION1_4_RULE[2].FLEXSPI0_REGION_RULE0 &= ~(0xF << (ns_region_id*4));
                                }
                                else /* == Region 4 == */
                                {
                                    if(ns_region_start_id > FLASH_REGION3_SUBREGION_NUMBER)
                                    {
                                        ns_region_start_id = (memory_regions.non_secure_partition_base - NS_ROM_ALIAS_BASE - (FLASH_REGION0_SIZE+FLASH_REGION1_SIZE+FLASH_REGION2_SIZE+FLASH_REGION3_SIZE) )/FLASH_REGION4_SUBREGION_SIZE;
                                    }
                                    else
                                    {
                                        ns_region_start_id = 0;   
                                    }
                                
                                    ns_region_end_id = (memory_regions.non_secure_partition_limit - NS_ROM_ALIAS_BASE - (FLASH_REGION0_SIZE+FLASH_REGION1_SIZE+FLASH_REGION2_SIZE+FLASH_REGION3_SIZE) + 1)/FLASH_REGION4_SUBREGION_SIZE;

                                    for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++) 
                                    {
                                        if(ns_region_id < FLASH_REGION4_SUBREGION_NUMBER)
                                        {
                                            /* Set regions in the AHB controller for flash memory 0x0C00 0000 - 0xFFF FFFF */
                                            AHB_SECURE_CTRL->FLEXSPI0_REGION1_4_RULE[3].FLEXSPI0_REGION_RULE0 &= ~(0xF << (ns_region_id*4));
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
        
    }

    /* == SRAM region == */
    /*  RAM0 to RAM7 256 KB, each 32 KB (32 * 1 KB)
        RAM8 to RAM11 256 KB, each 64 KB (32 * 2 KB)
        RAM12 to RAM15 512 KB, each 128 KB (32 * 4 KB)
        RAM16 to RAM31 4 MB, each 256 KB (32 * 8 KB) */

    /* The regions have to be alligned to 1 kB to cover the AHB RAM Region */
    TFM_ASSERT((S_DATA_SIZE % DATA_REGION0_SUBREGION_SIZE) == 0);
    TFM_ASSERT(((S_DATA_SIZE + NS_DATA_SIZE) % DATA_REGION0_SUBREGION_SIZE) == 0);
    
    /* Security access rules for RAM (0x3 = all regions set to secure and privileged user access) */
    for(i=0; i < 4; i++)
    {
        AHB_SECURE_CTRL->RAM00_RULE[i] = 0x33333333U; /* 0x2000_0000 -  0x2000_7FFF */
        AHB_SECURE_CTRL->RAM01_RULE[i] = 0x33333333U; /* 0x2000_8000 -  0x2000_FFFF */
        AHB_SECURE_CTRL->RAM02_RULE[i] = 0x33333333U; /* 0x2001_0000 -  0x2001_7FFF */
        AHB_SECURE_CTRL->RAM03_RULE[i] = 0x33333333U; /* 0x2001_8000 -  0x2001_FFFF */
        AHB_SECURE_CTRL->RAM04_RULE[i] = 0x33333333U; /* 0x2002_0000 -  0x2002_7FFF */
        AHB_SECURE_CTRL->RAM05_RULE[i] = 0x33333333U; /* 0x2002_8000 -  0x2002_FFFF */
        AHB_SECURE_CTRL->RAM06_RULE[i] = 0x33333333U; /* 0x2003_0000 -  0x2003_7FFF */
        AHB_SECURE_CTRL->RAM07_RULE[i] = 0x33333333U; /* 0x2003_8000 -  0x2003_FFFF */
        AHB_SECURE_CTRL->RAM08_RULE[i] = 0x33333333U; /* 0x2004_0000 -  0x2004_FFFF */
        AHB_SECURE_CTRL->RAM09_RULE[i] = 0x33333333U; /* 0x2005_0000 -  0x2005_FFFF */
        AHB_SECURE_CTRL->RAM10_RULE[i] = 0x33333333U; /* 0x2006_0000 -  0x2006_FFFF */
        AHB_SECURE_CTRL->RAM11_RULE[i] = 0x33333333U; /* 0x2007_0000 -  0x2007_FFFF */
        AHB_SECURE_CTRL->RAM12_RULE[i] = 0x33333333U; /* 0x2008_0000 -  0x2009_FFFF */
        AHB_SECURE_CTRL->RAM13_RULE[i] = 0x33333333U; /* 0x200A_0000 -  0x200B_FFFF */
        AHB_SECURE_CTRL->RAM14_RULE[i] = 0x33333333U; /* 0x200C_0000 -  0x200D_FFFF */
        AHB_SECURE_CTRL->RAM15_RULE[i] = 0x33333333U; /* 0x200E_0000 -  0x200F_FFFF */
        AHB_SECURE_CTRL->RAM16_RULE[i] = 0x33333333U; /* 0x2010_0000 -  0x2013_FFFF */
        AHB_SECURE_CTRL->RAM17_RULE[i] = 0x33333333U; /* 0x2014_0000 -  0x2017_FFFF */
        AHB_SECURE_CTRL->RAM18_RULE[i] = 0x33333333U; /* 0x2018_0000 -  0x201B_FFFF */
        AHB_SECURE_CTRL->RAM19_RULE[i] = 0x33333333U; /* 0x201C_0000 -  0x201F_FFFF */
        AHB_SECURE_CTRL->RAM20_RULE[i] = 0x33333333U; /* 0x2020_0000 -  0x2023_FFFF */
        AHB_SECURE_CTRL->RAM21_RULE[i] = 0x33333333U; /* 0x2024_0000 -  0x2027_FFFF */
        AHB_SECURE_CTRL->RAM22_RULE[i] = 0x33333333U; /* 0x2028_0000 -  0x202B_FFFF */
        AHB_SECURE_CTRL->RAM23_RULE[i] = 0x33333333U; /* 0x202C_0000 -  0x202F_FFFF */
        AHB_SECURE_CTRL->RAM24_RULE[i] = 0x33333333U; /* 0x2030_0000 -  0x2033_FFFF */
        AHB_SECURE_CTRL->RAM25_RULE[i] = 0x33333333U; /* 0x2034_0000 -  0x2037_FFFF */
        AHB_SECURE_CTRL->RAM26_RULE[i] = 0x33333333U; /* 0x2038_0000 -  0x203B_FFFF */
        AHB_SECURE_CTRL->RAM27_RULE[i] = 0x33333333U; /* 0x203C_0000 -  0x203F_FFFF */
        AHB_SECURE_CTRL->RAM28_RULE[i] = 0x33333333U; /* 0x2040_0000 -  0x2043_FFFF */
        AHB_SECURE_CTRL->RAM29_RULE[i] = 0x33333333U; /* 0x2044_0000 -  0x2047_FFFF */
        AHB_SECURE_CTRL->RAM30_RULE[i] = 0x33333333U; /* 0x2048_0000 -  0x204B_FFFF */
        AHB_SECURE_CTRL->RAM31_RULE[i] = 0x33333333U; /* 0x204C_0000 -  0x204F_FFFF */
    }

    /* == Region 0 == */
    /* RAM memory configuration (set according to region_defs.h and flash_layout.h) */
    ns_region_start_id = S_DATA_SIZE/DATA_REGION0_SUBREGION_SIZE; /* NS starts after S */
    ns_region_end_id = (S_DATA_SIZE + NS_DATA_SIZE)/DATA_REGION0_SUBREGION_SIZE;

    for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++)
    {
        /* Set regions the AHB controller for ram memory 0x2000_0000  - 0x2000_7FFF  */
        if(ns_region_id < 8) {
            AHB_SECURE_CTRL->RAM00_RULE[0] &= ~(0xF << (ns_region_id*4));
        }
        else if((ns_region_id >= 8) && (ns_region_id < 16)) {
                AHB_SECURE_CTRL->RAM00_RULE[1] &= ~(0xF << ((ns_region_id-8)*4));
        }
        else if((ns_region_id >= 16) && (ns_region_id < 24)) {
                AHB_SECURE_CTRL->RAM00_RULE[2] &= ~(0xF << ((ns_region_id-16)*4));
        }
        else if((ns_region_id >= 24) && (ns_region_id < 32)) {
                AHB_SECURE_CTRL->RAM00_RULE[3] &= ~(0xF << ((ns_region_id-24)*4));
        }
        /* Set regions the AHB controller for ram memory 0x2000_8000 - 0x2000_FFFF */
        else if((ns_region_id >= 32) && (ns_region_id < 40)) {
                AHB_SECURE_CTRL->RAM01_RULE[0] &= ~(0xF << ((ns_region_id-32)*4));
        }
        else if((ns_region_id >= 40) && (ns_region_id < 48)) {
                AHB_SECURE_CTRL->RAM01_RULE[1] &= ~(0xF << ((ns_region_id-40)*4));
        }
        else if((ns_region_id >= 48) && (ns_region_id < 56)) {
                AHB_SECURE_CTRL->RAM01_RULE[2] &= ~(0xF << ((ns_region_id-48)*4));
        }
        else if((ns_region_id >= 56) && (ns_region_id < 64)) {
                AHB_SECURE_CTRL->RAM01_RULE[3] &= ~(0xF << ((ns_region_id-56)*4));
        }
        
        /* Set regions the AHB controller for ram memory 0x2001_0000 - 0x2001_7FFF  */
        else if((ns_region_id >= 64) && (ns_region_id < 72)) {
                AHB_SECURE_CTRL->RAM02_RULE[0] &= ~(0xF << ((ns_region_id-64)*4));
        }
        else if((ns_region_id >= 72) && (ns_region_id < 80)) {
                AHB_SECURE_CTRL->RAM02_RULE[1] &= ~(0xF << ((ns_region_id-72)*4));
        }
        else if((ns_region_id >= 80) && (ns_region_id < 88)) {
                AHB_SECURE_CTRL->RAM02_RULE[2] &= ~(0xF << ((ns_region_id-80)*4));
        }
        else if((ns_region_id >= 88) && (ns_region_id < 96)) {
                AHB_SECURE_CTRL->RAM02_RULE[3] &= ~(0xF << ((ns_region_id-88)*4));
        }
        
        /* Set regions the AHB controller for ram memory 0x2001_8000 - 0x2001_FFFF  */
        else if((ns_region_id >= 96) && (ns_region_id < 104)) {
                AHB_SECURE_CTRL->RAM03_RULE[0] &= ~(0xF << ((ns_region_id-96)*4));
        }
        else if((ns_region_id >= 104) && (ns_region_id < 112)) {
                AHB_SECURE_CTRL->RAM03_RULE[1] &= ~(0xF << ((ns_region_id-104)*4));
        }
        else if((ns_region_id >= 112) && (ns_region_id < 120)) {
                AHB_SECURE_CTRL->RAM03_RULE[2] &= ~(0xF << ((ns_region_id-112)*4));
        }
        else if((ns_region_id >= 120) && (ns_region_id < 128)) {
                AHB_SECURE_CTRL->RAM03_RULE[3] &= ~(0xF << ((ns_region_id-120)*4));
        }
        
        /* Set regions the AHB controller for ram memory 0x2002_0000 - 0x2002_7FFF  */
        else if((ns_region_id >= 128) && (ns_region_id < 136)) {
                AHB_SECURE_CTRL->RAM04_RULE[0] &= ~(0xF << ((ns_region_id-128)*4));
        }
        else if((ns_region_id >= 136) && (ns_region_id < 144)) {
                AHB_SECURE_CTRL->RAM04_RULE[1] &= ~(0xF << ((ns_region_id-136)*4));
        }
        else if((ns_region_id >= 144) && (ns_region_id < 152)) {
                AHB_SECURE_CTRL->RAM04_RULE[2] &= ~(0xF << ((ns_region_id-144)*4));
        }
        else if((ns_region_id >= 152) && (ns_region_id < 160)) {
                AHB_SECURE_CTRL->RAM04_RULE[3] &= ~(0xF << ((ns_region_id-152)*4));
        }
        
        /* Set regions the AHB controller for ram memory 0x2002_8000 - 0x2002_FFFF  */
        else if((ns_region_id >= 160) && (ns_region_id < 168)) {
                AHB_SECURE_CTRL->RAM05_RULE[0] &= ~(0xF << ((ns_region_id-160)*4));
        }
        else if((ns_region_id >= 168) && (ns_region_id < 176)) {
                AHB_SECURE_CTRL->RAM05_RULE[1] &= ~(0xF << ((ns_region_id-168)*4));
        }
        else if((ns_region_id >= 176) && (ns_region_id < 184)) {
                AHB_SECURE_CTRL->RAM05_RULE[2] &= ~(0xF << ((ns_region_id-176)*4));
        }
        else if((ns_region_id >= 184) && (ns_region_id < 192)) {
                AHB_SECURE_CTRL->RAM05_RULE[3] &= ~(0xF << ((ns_region_id-184)*4));
        }
        
        /* Set regions the AHB controller for ram memory 0x2003_0000 - 0x2003_7FFF  */
        else if((ns_region_id >= 192) && (ns_region_id < 200)) {
                AHB_SECURE_CTRL->RAM06_RULE[0] &= ~(0xF << ((ns_region_id-192)*4));
        }
        else if((ns_region_id >= 200) && (ns_region_id < 208)) {
                AHB_SECURE_CTRL->RAM06_RULE[1] &= ~(0xF << ((ns_region_id-200)*4));
        }
        else if((ns_region_id >= 208) && (ns_region_id < 216)) {
                AHB_SECURE_CTRL->RAM06_RULE[2] &= ~(0xF << ((ns_region_id-208)*4));
        }
        else if((ns_region_id >= 216) && (ns_region_id < 224)) {
                AHB_SECURE_CTRL->RAM06_RULE[3] &= ~(0xF << ((ns_region_id-216)*4));
        }
        
        /* Set regions the AHB controller for ram memory 0x2003_8000 - 0x2003_FFFF  */
        else if((ns_region_id >= 224) && (ns_region_id < 232)) {
                AHB_SECURE_CTRL->RAM07_RULE[0] &= ~(0xF << ((ns_region_id-224)*4));
        }
        else if((ns_region_id >= 232) && (ns_region_id < 240)) {
                AHB_SECURE_CTRL->RAM07_RULE[1] &= ~(0xF << ((ns_region_id-232)*4));
        }
        else if((ns_region_id >= 240) && (ns_region_id < 248)) {
                AHB_SECURE_CTRL->RAM07_RULE[2] &= ~(0xF << ((ns_region_id-240)*4));
        }
        else if((ns_region_id >= 248) && (ns_region_id < 256)) {
                AHB_SECURE_CTRL->RAM07_RULE[3] &= ~(0xF << ((ns_region_id-248)*4));
        }
        else /* == Region 1 == */
        {
            if(ns_region_start_id > DATA_REGION0_SUBREGION_NUMBER)
            {
                ns_region_start_id = (S_DATA_SIZE - DATA_REGION0_SIZE )/DATA_REGION1_SUBREGION_SIZE;
            }
            else
            {
                ns_region_start_id = 0;   
            }
            
            ns_region_end_id = (S_DATA_SIZE + NS_DATA_SIZE - DATA_REGION0_SIZE + 1)/DATA_REGION1_SUBREGION_SIZE;
            
            for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++)
            {
                /* Set regions the AHB controller for ram memory 0x2004_0000 - 0x2004_FFFF  */
                if(ns_region_id < 8) {
                    AHB_SECURE_CTRL->RAM08_RULE[0] &= ~(0xF << (ns_region_id*4));
                }
                else if((ns_region_id >= 8) && (ns_region_id < 16)) {
                        AHB_SECURE_CTRL->RAM08_RULE[1] &= ~(0xF << ((ns_region_id-8)*4));
                }
                else if((ns_region_id >= 16) && (ns_region_id < 24)) {
                        AHB_SECURE_CTRL->RAM08_RULE[2] &= ~(0xF << ((ns_region_id-16)*4));
                }
                else if((ns_region_id >= 24) && (ns_region_id < 32)) {
                        AHB_SECURE_CTRL->RAM08_RULE[3] &= ~(0xF << ((ns_region_id-24)*4));
                }
                /* Set regions the AHB controller for ram memory 0x2005_0000 - 0x2005_FFFF */
                else if((ns_region_id >= 32) && (ns_region_id < 40)) {
                        AHB_SECURE_CTRL->RAM09_RULE[0] &= ~(0xF << ((ns_region_id-32)*4));
                }
                else if((ns_region_id >= 40) && (ns_region_id < 48)) {
                        AHB_SECURE_CTRL->RAM09_RULE[1] &= ~(0xF << ((ns_region_id-40)*4));
                }
                else if((ns_region_id >= 48) && (ns_region_id < 56)) {
                        AHB_SECURE_CTRL->RAM09_RULE[2] &= ~(0xF << ((ns_region_id-48)*4));
                }
                else if((ns_region_id >= 56) && (ns_region_id < 64)) {
                        AHB_SECURE_CTRL->RAM09_RULE[3] &= ~(0xF << ((ns_region_id-56)*4));
                }
                
                /* Set regions the AHB controller for ram memory 0x2006_0000 - 0x2006_FFFF  */
                else if((ns_region_id >= 64) && (ns_region_id < 72)) {
                        AHB_SECURE_CTRL->RAM10_RULE[0] &= ~(0xF << ((ns_region_id-64)*4));
                }
                else if((ns_region_id >= 72) && (ns_region_id < 80)) {
                        AHB_SECURE_CTRL->RAM10_RULE[1] &= ~(0xF << ((ns_region_id-72)*4));
                }
                else if((ns_region_id >= 80) && (ns_region_id < 88)) {
                        AHB_SECURE_CTRL->RAM10_RULE[2] &= ~(0xF << ((ns_region_id-80)*4));
                }
                else if((ns_region_id >= 88) && (ns_region_id < 96)) {
                        AHB_SECURE_CTRL->RAM10_RULE[3] &= ~(0xF << ((ns_region_id-88)*4));
                }
                
                /* Set regions the AHB controller for ram memory 0x2007_0000 - 0x2007_FFFF  */
                else if((ns_region_id >= 96) && (ns_region_id < 104)) {
                        AHB_SECURE_CTRL->RAM11_RULE[0] &= ~(0xF << ((ns_region_id-96)*4));
                }
                else if((ns_region_id >= 104) && (ns_region_id < 112)) {
                        AHB_SECURE_CTRL->RAM11_RULE[1] &= ~(0xF << ((ns_region_id-104)*4));
                }
                else if((ns_region_id >= 112) && (ns_region_id < 120)) {
                        AHB_SECURE_CTRL->RAM11_RULE[2] &= ~(0xF << ((ns_region_id-112)*4));
                }
                else if((ns_region_id >= 120) && (ns_region_id < 128)) {
                        AHB_SECURE_CTRL->RAM11_RULE[3] &= ~(0xF << ((ns_region_id-120)*4));
                }
                else /* == Region 2 == */
                {
                    if(ns_region_start_id > DATA_REGION1_SUBREGION_NUMBER)
                    {
                        ns_region_start_id = (S_DATA_SIZE - (DATA_REGION0_SIZE + DATA_REGION1_SIZE) )/DATA_REGION2_SUBREGION_SIZE;
                    }
                    else
                    {
                        ns_region_start_id = 0;   
                    }
                    
                    ns_region_end_id = (S_DATA_SIZE + NS_DATA_SIZE - (DATA_REGION0_SIZE + DATA_REGION1_SIZE) + 1)/DATA_REGION2_SUBREGION_SIZE;
                    
                    for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++)
                    {
                        /* Set regions the AHB controller for ram memory 0x2008_0000 - 0x2009_FFFF  */
                        if(ns_region_id < 8) {
                            AHB_SECURE_CTRL->RAM12_RULE[0] &= ~(0xF << (ns_region_id*4));
                        }
                        else if((ns_region_id >= 8) && (ns_region_id < 16)) {
                                AHB_SECURE_CTRL->RAM12_RULE[1] &= ~(0xF << ((ns_region_id-8)*4));
                        }
                        else if((ns_region_id >= 16) && (ns_region_id < 24)) {
                                AHB_SECURE_CTRL->RAM12_RULE[2] &= ~(0xF << ((ns_region_id-16)*4));
                        }
                        else if((ns_region_id >= 24) && (ns_region_id < 32)) {
                                AHB_SECURE_CTRL->RAM12_RULE[3] &= ~(0xF << ((ns_region_id-24)*4));
                        }
                        /* Set regions the AHB controller for ram memory 0x200A_0000 - 00x200B_FFFF */
                        else if((ns_region_id >= 32) && (ns_region_id < 40)) {
                                AHB_SECURE_CTRL->RAM13_RULE[0] &= ~(0xF << ((ns_region_id-32)*4));
                        }
                        else if((ns_region_id >= 40) && (ns_region_id < 48)) {
                                AHB_SECURE_CTRL->RAM13_RULE[1] &= ~(0xF << ((ns_region_id-40)*4));
                        }
                        else if((ns_region_id >= 48) && (ns_region_id < 56)) {
                                AHB_SECURE_CTRL->RAM13_RULE[2] &= ~(0xF << ((ns_region_id-48)*4));
                        }
                        else if((ns_region_id >= 56) && (ns_region_id < 64)) {
                                AHB_SECURE_CTRL->RAM13_RULE[3] &= ~(0xF << ((ns_region_id-56)*4));
                        }
                        /* Set regions the AHB controller for ram memory 0x200C_0000 - 0x200D_FFFF  */
                        else if((ns_region_id >= 64) && (ns_region_id < 72)) {
                                AHB_SECURE_CTRL->RAM14_RULE[0] &= ~(0xF << ((ns_region_id-64)*4));
                        }
                        else if((ns_region_id >= 72) && (ns_region_id < 80)) {
                                AHB_SECURE_CTRL->RAM14_RULE[1] &= ~(0xF << ((ns_region_id-72)*4));
                        }
                        else if((ns_region_id >= 80) && (ns_region_id < 88)) {
                                AHB_SECURE_CTRL->RAM14_RULE[2] &= ~(0xF << ((ns_region_id-80)*4));
                        }
                        else if((ns_region_id >= 88) && (ns_region_id < 96)) {
                                AHB_SECURE_CTRL->RAM14_RULE[3] &= ~(0xF << ((ns_region_id-88)*4));
                        }
                        /* Set regions the AHB controller for ram memory 0x200E_0000 - 0x200F_FFFF  */
                        else if((ns_region_id >= 96) && (ns_region_id < 104)) {
                                AHB_SECURE_CTRL->RAM15_RULE[0] &= ~(0xF << ((ns_region_id-96)*4));
                        }
                        else if((ns_region_id >= 104) && (ns_region_id < 112)) {
                                AHB_SECURE_CTRL->RAM15_RULE[1] &= ~(0xF << ((ns_region_id-104)*4));
                        }
                        else if((ns_region_id >= 112) && (ns_region_id < 120)) {
                                AHB_SECURE_CTRL->RAM15_RULE[2] &= ~(0xF << ((ns_region_id-112)*4));
                        }
                        else if((ns_region_id >= 120) && (ns_region_id < 128)) {
                                AHB_SECURE_CTRL->RAM15_RULE[3] &= ~(0xF << ((ns_region_id-120)*4));
                        }
                        else /* == Region 3 == */
                        {
                            if(ns_region_start_id > DATA_REGION2_SUBREGION_NUMBER)
                            {
                                ns_region_start_id = (S_DATA_SIZE - (DATA_REGION0_SIZE + DATA_REGION1_SIZE + DATA_REGION2_SIZE) )/DATA_REGION3_SUBREGION_SIZE;
                            }
                            else
                            {
                                ns_region_start_id = 0;   
                            }
                            
                            ns_region_end_id = (S_DATA_SIZE + NS_DATA_SIZE - (DATA_REGION0_SIZE + DATA_REGION1_SIZE + DATA_REGION2_SIZE) + 1)/DATA_REGION3_SUBREGION_SIZE;
                            
                            for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++)
                            {
                                /* Set regions the AHB controller for ram memory 0x2010_0000 - 0x2013_FFFF  */
                                if(ns_region_id < 8) {
                                    AHB_SECURE_CTRL->RAM16_RULE[0] &= ~(0xF << (ns_region_id*4));
                                }
                                else if((ns_region_id >= 8) && (ns_region_id < 16)) {
                                        AHB_SECURE_CTRL->RAM16_RULE[1] &= ~(0xF << ((ns_region_id-8)*4));
                                }
                                else if((ns_region_id >= 16) && (ns_region_id < 24)) {
                                        AHB_SECURE_CTRL->RAM16_RULE[2] &= ~(0xF << ((ns_region_id-16)*4));
                                }
                                else if((ns_region_id >= 24) && (ns_region_id < 32)) {
                                        AHB_SECURE_CTRL->RAM16_RULE[3] &= ~(0xF << ((ns_region_id-24)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2014_0000 -  0x2017_FFFF */
                                else if((ns_region_id >= 32) && (ns_region_id < 40)) {
                                        AHB_SECURE_CTRL->RAM17_RULE[0] &= ~(0xF << ((ns_region_id-32)*4));
                                }
                                else if((ns_region_id >= 40) && (ns_region_id < 48)) {
                                        AHB_SECURE_CTRL->RAM17_RULE[1] &= ~(0xF << ((ns_region_id-40)*4));
                                }
                                else if((ns_region_id >= 48) && (ns_region_id < 56)) {
                                        AHB_SECURE_CTRL->RAM17_RULE[2] &= ~(0xF << ((ns_region_id-48)*4));
                                }
                                else if((ns_region_id >= 56) && (ns_region_id < 64)) {
                                        AHB_SECURE_CTRL->RAM17_RULE[3] &= ~(0xF << ((ns_region_id-56)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2018_0000 - 0x201B_FFFF  */
                                else if((ns_region_id >= 64) && (ns_region_id < 72)) {
                                        AHB_SECURE_CTRL->RAM18_RULE[0] &= ~(0xF << ((ns_region_id-64)*4));
                                }
                                else if((ns_region_id >= 72) && (ns_region_id < 80)) {
                                        AHB_SECURE_CTRL->RAM18_RULE[1] &= ~(0xF << ((ns_region_id-72)*4));
                                }
                                else if((ns_region_id >= 80) && (ns_region_id < 88)) {
                                        AHB_SECURE_CTRL->RAM18_RULE[2] &= ~(0xF << ((ns_region_id-80)*4));
                                }
                                else if((ns_region_id >= 88) && (ns_region_id < 96)) {
                                        AHB_SECURE_CTRL->RAM18_RULE[3] &= ~(0xF << ((ns_region_id-88)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x201C_0000 - 0x201F_FFFF  */
                                else if((ns_region_id >= 96) && (ns_region_id < 104)) {
                                        AHB_SECURE_CTRL->RAM19_RULE[0] &= ~(0xF << ((ns_region_id-96)*4));
                                }
                                else if((ns_region_id >= 104) && (ns_region_id < 112)) {
                                        AHB_SECURE_CTRL->RAM19_RULE[1] &= ~(0xF << ((ns_region_id-104)*4));
                                }
                                else if((ns_region_id >= 112) && (ns_region_id < 120)) {
                                        AHB_SECURE_CTRL->RAM19_RULE[2] &= ~(0xF << ((ns_region_id-112)*4));
                                }
                                else if((ns_region_id >= 120) && (ns_region_id < 128)) {
                                        AHB_SECURE_CTRL->RAM19_RULE[3] &= ~(0xF << ((ns_region_id-120)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2020_0000 - 0x2023_FFFF  */
                                else if((ns_region_id >= 128) && (ns_region_id < 136)) {
                                        AHB_SECURE_CTRL->RAM20_RULE[0] &= ~(0xF << ((ns_region_id-128)*4));
                                }
                                else if((ns_region_id >= 136) && (ns_region_id < 144)) {
                                        AHB_SECURE_CTRL->RAM20_RULE[1] &= ~(0xF << ((ns_region_id-136)*4));
                                }
                                else if((ns_region_id >= 144) && (ns_region_id < 152)) {
                                        AHB_SECURE_CTRL->RAM20_RULE[2] &= ~(0xF << ((ns_region_id-144)*4));
                                }
                                else if((ns_region_id >= 152) && (ns_region_id < 160)) {
                                        AHB_SECURE_CTRL->RAM20_RULE[3] &= ~(0xF << ((ns_region_id-152)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2024_0000 - 0x2027_FFFF  */
                                else if((ns_region_id >= 160) && (ns_region_id < 168)) {
                                        AHB_SECURE_CTRL->RAM21_RULE[0] &= ~(0xF << ((ns_region_id-160)*4));
                                }
                                else if((ns_region_id >= 168) && (ns_region_id < 176)) {
                                        AHB_SECURE_CTRL->RAM21_RULE[1] &= ~(0xF << ((ns_region_id-168)*4));
                                }
                                else if((ns_region_id >= 176) && (ns_region_id < 184)) {
                                        AHB_SECURE_CTRL->RAM21_RULE[2] &= ~(0xF << ((ns_region_id-176)*4));
                                }
                                else if((ns_region_id >= 184) && (ns_region_id < 192)) {
                                        AHB_SECURE_CTRL->RAM21_RULE[3] &= ~(0xF << ((ns_region_id-184)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2028_0000 -  0x202B_FFFF  */
                                else if((ns_region_id >= 192) && (ns_region_id < 200)) {
                                        AHB_SECURE_CTRL->RAM22_RULE[0] &= ~(0xF << ((ns_region_id-192)*4));
                                }
                                else if((ns_region_id >= 200) && (ns_region_id < 208)) {
                                        AHB_SECURE_CTRL->RAM22_RULE[1] &= ~(0xF << ((ns_region_id-200)*4));
                                }
                                else if((ns_region_id >= 208) && (ns_region_id < 216)) {
                                        AHB_SECURE_CTRL->RAM22_RULE[2] &= ~(0xF << ((ns_region_id-208)*4));
                                }
                                else if((ns_region_id >= 216) && (ns_region_id < 224)) {
                                        AHB_SECURE_CTRL->RAM22_RULE[3] &= ~(0xF << ((ns_region_id-216)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x202C_0000 - 0x202F_FFFF  */
                                else if((ns_region_id >= 224) && (ns_region_id < 232)) {
                                        AHB_SECURE_CTRL->RAM23_RULE[0] &= ~(0xF << ((ns_region_id-224)*4));
                                }
                                else if((ns_region_id >= 232) && (ns_region_id < 240)) {
                                        AHB_SECURE_CTRL->RAM23_RULE[1] &= ~(0xF << ((ns_region_id-232)*4));
                                }
                                else if((ns_region_id >= 240) && (ns_region_id < 248)) {
                                        AHB_SECURE_CTRL->RAM23_RULE[2] &= ~(0xF << ((ns_region_id-240)*4));
                                }
                                else if((ns_region_id >= 248) && (ns_region_id < 256)) {
                                        AHB_SECURE_CTRL->RAM23_RULE[3] &= ~(0xF << ((ns_region_id-248)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2030_0000 - 0x2033_FFFF  */
                                else if((ns_region_id >= 256) && (ns_region_id < 264)) {
                                        AHB_SECURE_CTRL->RAM24_RULE[0] &= ~(0xF << ((ns_region_id-256)*4));
                                }
                                else if((ns_region_id >= 264) && (ns_region_id < 272)) {
                                        AHB_SECURE_CTRL->RAM24_RULE[1] &= ~(0xF << ((ns_region_id-264)*4));
                                }
                                else if((ns_region_id >= 272) && (ns_region_id < 280)) {
                                        AHB_SECURE_CTRL->RAM24_RULE[2] &= ~(0xF << ((ns_region_id-272)*4));
                                }
                                else if((ns_region_id >= 280) && (ns_region_id < 288)) {
                                        AHB_SECURE_CTRL->RAM24_RULE[3] &= ~(0xF << ((ns_region_id-280)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2034_0000 - 0x2037_FFFF  */
                                else if((ns_region_id >= 288) && (ns_region_id < 296)) {
                                        AHB_SECURE_CTRL->RAM25_RULE[0] &= ~(0xF << ((ns_region_id-288)*4));
                                }
                                else if((ns_region_id >= 296) && (ns_region_id < 304)) {
                                        AHB_SECURE_CTRL->RAM25_RULE[1] &= ~(0xF << ((ns_region_id-296)*4));
                                }
                                else if((ns_region_id >= 304) && (ns_region_id < 312)) {
                                        AHB_SECURE_CTRL->RAM25_RULE[2] &= ~(0xF << ((ns_region_id-304)*4));
                                }
                                else if((ns_region_id >= 312) && (ns_region_id < 320)) {
                                        AHB_SECURE_CTRL->RAM25_RULE[3] &= ~(0xF << ((ns_region_id-312)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2038_0000 - 0x203B_FFFF  */
                                else if((ns_region_id >= 320) && (ns_region_id < 328)) {
                                        AHB_SECURE_CTRL->RAM26_RULE[0] &= ~(0xF << ((ns_region_id-320)*4));
                                }
                                else if((ns_region_id >= 328) && (ns_region_id < 336)) {
                                        AHB_SECURE_CTRL->RAM26_RULE[1] &= ~(0xF << ((ns_region_id-328)*4));
                                }
                                else if((ns_region_id >= 336) && (ns_region_id < 344)) {
                                        AHB_SECURE_CTRL->RAM26_RULE[2] &= ~(0xF << ((ns_region_id-336)*4));
                                }
                                else if((ns_region_id >= 344) && (ns_region_id < 352)) {
                                        AHB_SECURE_CTRL->RAM26_RULE[3] &= ~(0xF << ((ns_region_id-344)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x203C_0000 - 0x203F_FFFF  */
                                else if((ns_region_id >= 352) && (ns_region_id < 360)) {
                                        AHB_SECURE_CTRL->RAM27_RULE[0] &= ~(0xF << ((ns_region_id-352)*4));
                                }
                                else if((ns_region_id >= 360) && (ns_region_id < 368)) {
                                        AHB_SECURE_CTRL->RAM27_RULE[1] &= ~(0xF << ((ns_region_id-360)*4));
                                }
                                else if((ns_region_id >= 368) && (ns_region_id < 376)) {
                                        AHB_SECURE_CTRL->RAM27_RULE[2] &= ~(0xF << ((ns_region_id-368)*4));
                                }
                                else if((ns_region_id >= 376) && (ns_region_id < 384)) {
                                        AHB_SECURE_CTRL->RAM27_RULE[3] &= ~(0xF << ((ns_region_id-376)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2040_0000 - 0x2043_FFFF  */
                                else if((ns_region_id >= 384) && (ns_region_id < 392)) {
                                        AHB_SECURE_CTRL->RAM28_RULE[0] &= ~(0xF << ((ns_region_id-384)*4));
                                }
                                else if((ns_region_id >= 392) && (ns_region_id < 400)) {
                                        AHB_SECURE_CTRL->RAM28_RULE[1] &= ~(0xF << ((ns_region_id-392)*4));
                                }
                                else if((ns_region_id >= 400) && (ns_region_id < 408)) {
                                        AHB_SECURE_CTRL->RAM28_RULE[2] &= ~(0xF << ((ns_region_id-400)*4));
                                }
                                else if((ns_region_id >= 408) && (ns_region_id < 416)) {
                                        AHB_SECURE_CTRL->RAM28_RULE[3] &= ~(0xF << ((ns_region_id-408)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2044_0000 - 0x2047_FFFF  */
                                else if((ns_region_id >= 416) && (ns_region_id < 424)) {
                                        AHB_SECURE_CTRL->RAM29_RULE[0] &= ~(0xF << ((ns_region_id-416)*4));
                                }
                                else if((ns_region_id >= 424) && (ns_region_id < 432)) {
                                        AHB_SECURE_CTRL->RAM29_RULE[1] &= ~(0xF << ((ns_region_id-424)*4));
                                }
                                else if((ns_region_id >= 432) && (ns_region_id < 440)) {
                                        AHB_SECURE_CTRL->RAM29_RULE[2] &= ~(0xF << ((ns_region_id-432)*4));
                                }
                                else if((ns_region_id >= 440) && (ns_region_id < 448)) {
                                        AHB_SECURE_CTRL->RAM29_RULE[3] &= ~(0xF << ((ns_region_id-440)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x2048_0000 -  0x204B_FFFF */
                                else if((ns_region_id >= 448) && (ns_region_id < 456)) {
                                        AHB_SECURE_CTRL->RAM30_RULE[0] &= ~(0xF << ((ns_region_id-416)*4));
                                }
                                else if((ns_region_id >= 456) && (ns_region_id < 464)) {
                                        AHB_SECURE_CTRL->RAM30_RULE[1] &= ~(0xF << ((ns_region_id-424)*4));
                                }
                                else if((ns_region_id >= 464) && (ns_region_id < 472)) {
                                        AHB_SECURE_CTRL->RAM30_RULE[2] &= ~(0xF << ((ns_region_id-432)*4));
                                }
                                else if((ns_region_id >= 472) && (ns_region_id < 480)) {
                                        AHB_SECURE_CTRL->RAM30_RULE[3] &= ~(0xF << ((ns_region_id-440)*4));
                                }
                                /* Set regions the AHB controller for ram memory 0x204C_0000 -  0x204F_FFFF*/
                                else if((ns_region_id >= 480) && (ns_region_id < 488)) {
                                        AHB_SECURE_CTRL->RAM31_RULE[0] &= ~(0xF << ((ns_region_id-416)*4));
                                }
                                else if((ns_region_id >= 488) && (ns_region_id < 496)) {
                                        AHB_SECURE_CTRL->RAM31_RULE[1] &= ~(0xF << ((ns_region_id-424)*4));
                                }
                                else if((ns_region_id >= 496) && (ns_region_id < 504)) {
                                        AHB_SECURE_CTRL->RAM31_RULE[2] &= ~(0xF << ((ns_region_id-432)*4));
                                }
                                else if((ns_region_id >= 504) && (ns_region_id < 512)) {
                                        AHB_SECURE_CTRL->RAM31_RULE[3] &= ~(0xF << ((ns_region_id-440)*4));
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }    
    }
    
    /* Smart DMA Controller RAM 32KB (0x24100000--0x24107FFF) */
    for(i=0; i < 4; i++)
    {
        AHB_SECURE_CTRL->SDMA_RAM_RULE[i] = 0x0U; /* Non-secure, user access allowed */
    }

    /* FlexSPI1 External Memory interface 0x2800 0000 to 0x2FFF FFFF 128 MB
            Region 0: 8 MB (32 * 256 KB)
            Region 1: 8 MB (4 * 2 MB)
            Region 2: 16 MB (4 * 4 MB)
            Region 3: 32 MB (4 * 8 MB)
            Region 4: 64 MB (4 * 16 MB) */

    /* Set FLASH memory security access rule configuration (Non-secure, user access allowed) */
    for(i=0; i < 4; i++)
    {
        AHB_SECURE_CTRL->FLEXSPI1_REGION0_RULE[i] = 0x0U;
        AHB_SECURE_CTRL->FLEXSPI1_REGIONN_RULE0[i].FLEXSPI1_REGION_RULE0 = 0x0U;
    }

#if TARGET_DEBUG_LOG
    SPMLOG_DBGMSG("=== [AHB MPC NS] =======\r\n");
    SPMLOG_DBGMSGVAL("NS ROM starts from : ",
                                      memory_regions.non_secure_partition_base);
    SPMLOG_DBGMSGVAL("NS ROM ends at : ",
                                      memory_regions.non_secure_partition_base +
                                     memory_regions.non_secure_partition_limit);
    SPMLOG_DBGMSGVAL("NS DATA start from : ", NS_DATA_START);
    SPMLOG_DBGMSGVAL("NS DATA ends at : ", NS_DATA_START + NS_DATA_LIMIT);
#endif

    /* Add barriers to assure the MPC configuration is done before continue
     * the execution.
     */
    __DSB();
    __ISB();

    return ARM_DRIVER_OK;
}

/*---------------------- PPC configuration functions -------------------------*/

int32_t ppc_init_cfg(void)
{
     /* Configuration of AHB Secure Controller. Grant user access to peripherals.
     * Possible values for every memory sector or peripheral rule:
     *  0    Non-secure, user access allowed.
     *  1    Non-secure, privileged access allowed.
     *  2    Secure, user access allowed.
     *  3    Secure, privileged access allowed. */
    
    /* Write access attributes for AHB_SECURE_CTRL module are tier-4 (secure privileged). */ 
    
    /* Security access rules for APB Bridge 0 peripherals. */
    AHB_SECURE_CTRL->APB_BRIDGE_PER0_RULE0 =
        (0x30300000U) |                                                         /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_APB_BRIDGE_PER0_RULE0_RSTCTL_A(0x0U) |                  /* RSTCTL_A*/
        AHB_SECURE_CTRL_APB_BRIDGE_PER0_RULE0_CLKCTL_A(0x0U) |                  /* CLKCTL_A */ //TBD
        AHB_SECURE_CTRL_APB_BRIDGE_PER0_RULE0_SYSCTL_A(0x0U) |                  /* SYSCTL_A */ //TBD
        AHB_SECURE_CTRL_APB_BRIDGE_PER0_RULE0_PVT(0x0U) |                       /* PVT */
        AHB_SECURE_CTRL_APB_BRIDGE_PER0_RULE0_IOCON(0x0U) |                     /* IOCON */    //TBD 
        AHB_SECURE_CTRL_APB_BRIDGE_PER0_RULE0_PUF(0x3U);                        /* PUF */

    AHB_SECURE_CTRL->APB_BRIDGE_PER0_RULE1 =
        (0x00333333U) |                                                         /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_APB_BRIDGE_PER0_RULE1_WWDT0(0x0U) |                     /* Watchdog timer 0 */
        AHB_SECURE_CTRL_APB_BRIDGE_PER0_RULE1_MICRO_TICK(0x0U);                 /* U-Tick timer */

    AHB_SECURE_CTRL->APB_BRIDGE_PER0_RULE3 =
        (0x00333333U) |                                                         /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_APB_BRIDGE_PER0_RULE3_PROBE_IS_SYNC(0x0U) |             /* PROBE_IS_SYNC */
        AHB_SECURE_CTRL_APB_BRIDGE_PER0_RULE3_PROBE_IS_XVC(0x0U);               /* PROBE_IS_XVC */

    /* Security access rules for APB Bridge 1 peripherals */
    AHB_SECURE_CTRL->APB_BRIDGE_PER1_RULE0 =
        (0x00033000U) |                                                         /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE0_RSTCTL_B(0x0U) |                  /* RSTCTL_B */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE0_CLKCTL_B(0x0U) |                  /* CLKCTL_B */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE0_SYSCTL_B(0x0U) |                  /* SYSCTL_B */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE0_GPIO_INT(0x0U) |                  /* GPIO pin interrupts (PINT) */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE0_PERIPHERAL_MUXES(0x0U) |          /* Input Muxes */               
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE0_SDMA(0x0U);                       /* Smart DMA (SDMA) */ 

    AHB_SECURE_CTRL->APB_BRIDGE_PER1_RULE1 =
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE1_CT32B0(0x0U) |                    /* CTIMER0 */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE1_CT32B1(0x0U) |                    /* CTIMER1 */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE1_CT32B2(0x0U) |                    /* CTIMER2 */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE1_CT32B3(0x0U) |                    /* CTIMER3 */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE1_CT32B4(0x0U) |                    /* CTIMER4 */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE1_MRT0(0x0U) |                      /* MRT */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE1_WWDT1(0x0U) |                     /* Watchdog timer 1 */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE1_FREQMEASURE(0x0U);                /* Frequency measure */

    AHB_SECURE_CTRL->APB_BRIDGE_PER1_RULE2 =
        (0x00000030U) |                                                         /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE2_RTC_WAKEUP(0x0U) |                /* RTC Wakeup*/
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE2_DSI_HOST_CONTROLLER(0x0U) |       /* DSI Host */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE2_FLEXIO_REGISTERS(0x0U) |          /* FLEXIO */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE2_CACHE_CONTROL_0_REGS(0x0U) |      /* Cache Control 0 */ //TBD
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE2_CACHE_CONTROL_1_REGS(0x0U) |      /* Cache Control 1 */ //TBD
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE2_I3C0(0x0U) |                      /* I3C0 */
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE2_I3C1(0x0U);                       /* I3C1 */

    AHB_SECURE_CTRL->APB_BRIDGE_PER1_RULE3 =
        (0x03333333U) |                                                         /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_APB_BRIDGE_PER1_RULE3_MRT1(0x0U);                       /* MRT1*/

    /* Security access rules for AHB peripherals */ 
    AHB_SECURE_CTRL->AHB_PERIPH0_SLAVE_RULE0 =
        AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE0_HSGPIO(0x0U) |                  /* High-speed GPIO */
        AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE0_DMA0(0x0U) |                    /* DMAC0 */
        AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE0_DMA1(0x0U) |                    /* DMAC1 */
        AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE0_FLEXCOMM0(0x0U) |               /* Flexcomm Interface 0 */
        AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE0_FLEXCOMM1(0x0U) |               /* Flexcomm Interface 1 */
        AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE0_FLEXCOMM2(0x0U) |               /* Flexcomm Interface 2 */
        AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE0_FLEXCOMM3(0x0U) |               /* Flexcomm Interface 3 */
        AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE0_DEBUG_MAILBOX(0x0U);            /* Debug mailbox */  //TBD

    /* Security access rules for AIPS peripherals */
    AHB_SECURE_CTRL->AIPS_BRIDGE0_PER_RULE0 =
        (0x33000000U) |                                                         /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_AIPS_BRIDGE0_PER_RULE0_MU0(0x0U) |                      /* Message Unit for CM33 */
        AHB_SECURE_CTRL_AIPS_BRIDGE0_PER_RULE0_MU1(0x0U) |                      /* Message Unit for DSP */
        AHB_SECURE_CTRL_AIPS_BRIDGE0_PER_RULE0_SEMAPHORE(0x0U) |                /* Semaphore */
        AHB_SECURE_CTRL_AIPS_BRIDGE0_PER_RULE0_OS_EVENT_TIMER_M33_PORT(0x0U) |  /* OS Event Timer for CM33 */
        AHB_SECURE_CTRL_AIPS_BRIDGE0_PER_RULE0_OS_EVENT_TIMER_DSP_PORT(0x0U) |  /* OS Event Timer for DSP */
        AHB_SECURE_CTRL_AIPS_BRIDGE0_PER_RULE0_ROM(0x3U);                       /* ROM */

    /* Security access rules for AHB peripherals */
    AHB_SECURE_CTRL->AHB_PERIPH1_SLAVE_RULE0 =
        AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE0_CRC(0x0U) |                     /* CRC engine */
        AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE0_DMIC0(0x0U) |                   /* DMIC and HWVAD */
        AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE0_FLEXCOMM4(0x0U) |               /* Flexcomm Interface 4 */
        AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE0_FLEXCOMM5(0x0U) |               /* Flexcomm Interface 5 */
        AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE0_FLEXCOMM6(0x0U) |               /* Flexcomm Interface 6 */
        AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE0_FLEXCOMM7(0x0U) |               /* Flexcomm Interface 7 */
        AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE0_FLEXCOMM14(0x0U) |              /* Flexcomm Interface 14 */
        AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE0_FLEXCOMM15(0x0U);               /* Flexcomm Interface 15 */

    AHB_SECURE_CTRL->AHB_PERIPH1_SLAVE_RULE1 =
        AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE1_FLEXCOMM16(0x0U);               /* Flexcomm Interface 16 */
        
    /* Security access rules for AIPS peripherals */
    AHB_SECURE_CTRL->AIPS_BRIDGE1_PER_RULE0 =
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE0_OTP_CONTROLLER_0(0x0U) |         /* OTP 0 */
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE0_OTP_CONTROLLER_1(0x0U) |         /* OTP 1 */
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE0_OTP_CONTROLLER_2(0x0U) |         /* OTP 2 */
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE0_OTP_CONTROLLER_3(0x0U) |         /* OTP 3*/
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE0_FLEXSPI0_REGISTERS(0x0U) |       /* FlexSPI 0 */
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE0_PMC_PMU_CONTROL(0x0U) |          /* PMC */
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE0_SDIO0_REGISTERS(0x0U) |          /* SDIO0 */
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE0_SDIO1_REGISTERS(0x0U);           /* SDIO1 */

    AHB_SECURE_CTRL->AIPS_BRIDGE1_PER_RULE1 =
        (0x33300000U) |                                                         /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE1_RNG(0x0U) |                      /* Random Number Generator */
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE1_ACMP0(0x0U) |                    /* Analog Comparator */
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE1_ADC0(0x0U) |                     /* ADC */
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE1_HS_USB_PHY(0x0U)|                /* USB High Speed PHY */   
        AHB_SECURE_CTRL_AIPS_BRIDGE1_PER_RULE1_FLEXSPI1_REGISTERS(0x0U);        /* FlexSPI 1 */

    /* Security access rules for AHB peripherals */ 
    AHB_SECURE_CTRL->AHB_PERIPH2_SLAVE_RULE0 =
        (0x00030000U) |                                                         /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE0_USB_HS_RAM(0x0U) |              /* High Speed USB RAM */
        AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE0_USB_HS_DEV(0x0U) |              /* High Speed USB Device registers */
        AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE0_USB_HS_HOST(0x0U) |             /* High Speed USB Host registers */
        AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE0_SCT(0x0U);                      /* SCTimer/PWM */

    /* Skip AHB_SECURE_CTRL_PERIPH_RULE0, it sets only read attribute.*/

    /* Security access rules for AHB peripherals */
    AHB_SECURE_CTRL->AHB_PERIPH3_SLAVE_RULE0 =
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE0_POWERQUAD(0x0U) |               /* PowerQuad coprocessor registers */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE0_CASPER(0x3U) |                  /* Casper coprocessor registers */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE0_CASPER_RAM(0x3U) |              /* Casper RAM */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE0_SECURE_GPIO(0x0U) |             /* Secure GPIO */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE0_HASH(0x3U) |                    /* Hash-AES */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE0_FLEXCOMM8(0x0U) |               /* FLEXCOMM 8 */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE0_FLEXCOMM9(0x0U) |               /* FLEXCOMM 9 */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE0_FLEXCOMM10(0x0U);               /* FLEXCOMM 10 */
 
    AHB_SECURE_CTRL->AHB_PERIPH3_SLAVE_RULE1 =
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE1_FLEXCOMM11(0x0U) |              /* FLEXCOMM 11 */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE1_FLEXCOMM12(0x0U) |              /* FLEXCOMM 12 */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE1_FLEXCOMM13(0x0U) |              /* FLEXCOMM 13 */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE1_LCDIF(0x0U) |                   /* LCDIF */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE1_GPU(0x0U) |                     /* GPU */
        AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE1_AXI_SWITCH(0x0U);               /* AXI SWITCH */

    /* Secure GPIO mask for pins. 
    This register is used to block leakage of Secure interface (GPIOs, I2C, UART configured as secure peripherals) pin states to non-secure world. 
    0: GPIO can't read PIOn_PIN, 1: GPIO can read PIOn_PIN 0x1 */
    AHB_SECURE_CTRL->SEC_GPIO_MASK0 = 0xFFFFFFFFU; /* Reset value */
    AHB_SECURE_CTRL->SEC_GPIO_MASK1 = 0xFFFFFFFFU;
    AHB_SECURE_CTRL->SEC_GPIO_MASK2 = 0xFFFFFFFFU;
    
    /* Secure Interrupt Mask for DSP */
    AHB_SECURE_CTRL->DSP_INT_MASK0 = 0xFFFFFFE0U; /* Reset value */
   
    /* SEC_MASK_LOCK */
    /* MASTER_SEC_LEVEL          = 0x80000000U; */
    /* MASTER_SEC_LEVEL_ANTI_POL = 0xBFFFFFFFU; */
    
    /* m33 lock control register (0x2: CM33 Non-secure VTOR is not locked.)*/
    AHB_SECURE_CTRL->CM33_LOCK_REG    = 0x800002AAU; /* Reset value */

    /* Enable AHB secure controller check */
    AHB_SECURE_CTRL->MISC_CTRL_REG = (AHB_SECURE_CTRL->MISC_CTRL_REG & ~(AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING_MASK 
                                        | AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_SIMPLE_MASTER_STRICT_MODE_MASK
                                       // | AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_SMART_MASTER_STRICT_MODE_MASK
                                        |0)) 
                                        /* AHB bus matrix enable secure checking (restrictive mode). */
                                        | AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING(0x1U)
                                        /* 0x1 = Simple master in tier mode. Can read and write to memories at same or below level. */
                                        | AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_SIMPLE_MASTER_STRICT_MODE(0x1U)
                                        /* 0x1 = Smart master in tier mode. Can read and write to memories at same or below level. */
                                       // | AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_SMART_MASTER_STRICT_MODE(0x1U)
                                        ;
                                        /* Other default values:
                                            - Write lock - secure_ctrl_group_rule registers and this register itself can be written.
                                            - AHB bus matrix enable secure privilege check - disabled
                                            - AHB bus matrix enable non-secure privilege check - disabled
                                            - The violation detected by the secure checker causes abort.
                                            - Smart masters in strict mode. Can execute, read and write to memories at same level only. (Mode recommended by ARM.).
                                            – IDAU is enabled.
                                        */
    /* Secure control duplicate register */
    AHB_SECURE_CTRL->MISC_CTRL_DP_REG = AHB_SECURE_CTRL->MISC_CTRL_REG;

    return ARM_DRIVER_OK;
}

void ppc_configure_to_secure(volatile uint32_t *bank, uint32_t pos, bool privileged)
{
    /* Clear NS flag for peripheral to prevent NS access */
    if(bank)
    {
        /*  0b00..Non-secure and Non-priviledge user access allowed.
         *  0b01..Non-secure and Privilege access allowed.
         *  0b10..Secure and Non-priviledge user access allowed.
         *  0b11..Secure and Priviledge/Non-priviledge user access allowed.
         */
        /* Set to secure and privileged user access 0x3. */
        *bank = (*bank) | (((privileged == true)?0x3:0x2) << (pos));
    }
}

/* Secure Violation IRQ */
void SECURE_VIOLATION_IRQHandler(void)
{
    SPMLOG_ERRMSG("Oops... Secure Violation!!!");
    
    tfm_core_panic();
}

