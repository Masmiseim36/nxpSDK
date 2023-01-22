/*
 * Copyright (c) 2017-2022 Arm Limited. All rights reserved.
 * Copyright 2019-2022 NXP. All rights reserved.
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

#ifndef __REGION_DEFS_H__
#define __REGION_DEFS_H__

#include "flash_layout.h"

#ifdef ENABLE_HEAP
    #define S_HEAP_SIZE             (0x0000200)
#endif
#define S_MSP_STACK_SIZE        (0x0000800)
#define S_PSP_STACK_SIZE        (0x0000800)

#define NS_HEAP_SIZE            (0x0001000)
#define NS_MSP_STACK_SIZE       (0x00000A0)
#define NS_PSP_STACK_SIZE       (0x0000140)

/* This size of buffer is big enough to store an attestation
 * token produced by initial attestation service
 */
#define PSA_INITIAL_ATTEST_TOKEN_MAX_SIZE   (0x250)

/* [RT600] The boot image should be program at offset 0x08001000. 
XIP Image layout (FlexSPI):
Offset      Width (Bytes) Field Description
0x0000_0000 256           KeyBlob for OTFAD Optional, programmed with all 0x00s if OTFAD is not enabled?
0x0000_0400 512           Flash Config Block The OSPI FLASH configuration block. This block is required if the FLEXSPI_AUTO_PROBE_EN is not blown on the OTP.
0x0000_0600 4             Boot image version When using dual image ping-pong boot, this field is used to store the boot image version, More details please refer to FlexSPI boot
0x0000_0800 2048          KeyStore Fixed KeyStore field. This field is required if the KeyStore feature is enabled.
0x0000_1000 Image size    Bootable Image The boot image, starts with valid image header.
*/
#define S_IMAGE_PRIMARY_PARTITION_OFFSET (0x1000)

/* [RT600] reserved:
 The SRAM region [0x0-0xFFFF], [0x1C000-0x1FFFF] is reserved for app-specific use cases.
 The SRAM region [0x10000-0x1BFFF] is reserved for ROM code. Application should not use this memory area during ROM API calls.
 The SRAM region [0x20000-0x7FFFF] is reserved for Non-cached shared memory between M33 and DSP.
      NOTE: Ignored the SDK workaround region ([0x80000-0x27FFFF] reserved for DSP code and data), as we do not use DSP.
 It adds 0x00080000 offset*/
#define S_DATA_OFFSET    (0x00080000)

#ifndef LINK_TO_SECONDARY_PARTITION
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET (FLASH_AREA_0_OFFSET + S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE)
#else
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET (FLASH_AREA_2_OFFSET \
                                           + FLASH_S_PARTITION_SIZE)
#endif /* !LINK_TO_SECONDARY_PARTITION */

/* Boot partition structure if MCUBoot is used:
 * 0x0_0000 Bootloader header
 * 0x0_0400 Image area
 * 0x1_FC00 Trailer
 */
/* IMAGE_CODE_SIZE is the space available for the software binary image.
 * It is less than the FLASH_S_PARTITION_SIZE + FLASH_NS_PARTITION_SIZE
 * because we reserve space for the image header and trailer introduced
 * by the bootloader.
 */
#ifdef BL2
#define BL2_HEADER_SIZE      (0x400)       /* 1 KB */
#define BL2_TRAILER_SIZE     (0x400)       /* 1 KB */
#else
/* No header if no bootloader, but keep IMAGE_CODE_SIZE the same */
#define BL2_HEADER_SIZE      (0x0)
#define BL2_TRAILER_SIZE     (0x0)
#endif /* BL2 */

#define IMAGE_S_CODE_SIZE \
            (FLASH_S_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)
#define IMAGE_NS_CODE_SIZE \
            (FLASH_NS_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)

#define CMSE_VENEER_REGION_SIZE     (0x340)

/* Alias definitions for secure and non-secure areas*/
#define S_ROM_ALIAS(x)  (S_ROM_ALIAS_BASE + (x))
#define NS_ROM_ALIAS(x) (NS_ROM_ALIAS_BASE + (x))

#define S_RAM_ALIAS(x)  (S_RAM_ALIAS_BASE + (x))
#define NS_RAM_ALIAS(x) (NS_RAM_ALIAS_BASE + (x))

/* Secure regions */
#define S_IMAGE_PRIMARY_AREA_OFFSET \
             (S_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define S_CODE_START    (S_ROM_ALIAS(S_IMAGE_PRIMARY_AREA_OFFSET))
#define S_CODE_SIZE     (IMAGE_S_CODE_SIZE)
#define S_CODE_LIMIT    (S_CODE_START + S_CODE_SIZE - 1)

#define S_DATA_START    (S_RAM_ALIAS(S_DATA_OFFSET))
#define S_DATA_SIZE     ((TOTAL_RAM_SIZE / 2) - S_DATA_OFFSET)
#define S_DATA_LIMIT    (S_DATA_START + S_DATA_SIZE - 1)

/* Size of vector table: 89 interrupt handlers + 4 bytes MPS initial value */
#define S_CODE_VECTOR_TABLE_SIZE    (0x168)

/* Non-secure regions */
#define NS_IMAGE_PRIMARY_AREA_OFFSET \
                        (NS_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define NS_CODE_START   (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_AREA_OFFSET))
#define NS_CODE_SIZE    (IMAGE_NS_CODE_SIZE)
#define NS_CODE_LIMIT   (NS_CODE_START + NS_CODE_SIZE - 1)

#define NS_DATA_START   (NS_RAM_ALIAS(S_DATA_SIZE))
#define NS_DATA_SIZE    (TOTAL_RAM_SIZE - S_DATA_SIZE)
#define NS_DATA_LIMIT   (NS_DATA_START + NS_DATA_SIZE - 1)

/* FlexSPI. Each sub-region can be assigned individual
security tier by programing corresponding registers in secure AHB controller.*/
/* Region 0: 8 MB (32 * 256 KB). */
#define FLASH_REGION0_SUBREGION_NUMBER  (32)  
#define FLASH_REGION0_SUBREGION_SIZE    (1024 * 256)            /* 256 kB */
#define FLASH_REGION0_SIZE              (FLASH_REGION0_SUBREGION_NUMBER * FLASH_REGION0_SUBREGION_SIZE)     /* 8 MB */
/* Region 1: 8 MB (4 * 2 MB) */
#define FLASH_REGION1_SUBREGION_NUMBER  (4)  
#define FLASH_REGION1_SUBREGION_SIZE    (1024 * 1024 * 2)       /* 2 MB */
#define FLASH_REGION1_SIZE              (FLASH_REGION1_SUBREGION_NUMBER * FLASH_REGION1_SUBREGION_SIZE)     /* 8 MB */
/* Region 2: 16 MB (4 * 4 MB) */
#define FLASH_REGION2_SUBREGION_NUMBER  (4)  
#define FLASH_REGION2_SUBREGION_SIZE    (1024 * 1024 * 4)       /* 4 MB */
#define FLASH_REGION2_SIZE              (FLASH_REGION2_SUBREGION_NUMBER * FLASH_REGION2_SUBREGION_SIZE)     /* 16 MB */
/* Region 3: 32 MB (4 * 8 MB) */
#define FLASH_REGION3_SUBREGION_NUMBER  (4)  
#define FLASH_REGION3_SUBREGION_SIZE    (1024 * 1024 * 8)       /* 8 MB */
#define FLASH_REGION3_SIZE              (FLASH_REGION3_SUBREGION_NUMBER * FLASH_REGION3_SUBREGION_SIZE)     /* 32 MB */
/* Region 4: 64 MB (4 * 16 MB) */
#define FLASH_REGION4_SUBREGION_NUMBER  (4)  
#define FLASH_REGION4_SUBREGION_SIZE    (1024 * 1024 * 16)      /* 16 MB */
#define FLASH_REGION4_SIZE              (FLASH_REGION4_SUBREGION_NUMBER * FLASH_REGION4_SUBREGION_SIZE)     /* 64 MB */


/* RAM. Each sub-region can be assigned individual
security tier by programing corresponding registers in secure AHB controller. */

/* Region 0: RAM0 to RAM7 256 KB, (256 * 1 KB) */
#define DATA_REGION0_SUBREGION_NUMBER   (256)      
#define DATA_REGION0_SUBREGION_SIZE     (1024)      /* 1 KB*/
#define DATA_REGION0_SIZE               (DATA_REGION0_SUBREGION_NUMBER * DATA_REGION0_SUBREGION_SIZE)      /* 256 KB */
/* Region 1: RAM8 to RAM11 256 KB, (128 * 2 KB) */
#define DATA_REGION1_SUBREGION_NUMBER   (128)      
#define DATA_REGION1_SUBREGION_SIZE     (1024 * 2)  /* 2 KB*/
#define DATA_REGION1_SIZE               (DATA_REGION1_SUBREGION_NUMBER * DATA_REGION1_SUBREGION_SIZE)      /* 256 KB */
/* Region 2: RAM12 to RAM15 512 KB, (128 * 4 KB) */
#define DATA_REGION2_SUBREGION_NUMBER   (128)      
#define DATA_REGION2_SUBREGION_SIZE     (1024 * 4)  /* 4 KB*/
#define DATA_REGION2_SIZE               (DATA_REGION2_SUBREGION_NUMBER * DATA_REGION2_SUBREGION_SIZE)      /* 512 KB */
/* Region 3: RAM16 to RAM31 4 MB, (512 * 8 KB) */
#define DATA_REGION3_SUBREGION_NUMBER   (512)      
#define DATA_REGION3_SUBREGION_SIZE     (1024 * 8)  /* 8 KB*/
#define DATA_REGION3_SIZE               (DATA_REGION3_SUBREGION_NUMBER * DATA_REGION3_SUBREGION_SIZE)      /* 4 MB */

/* NS partition information is used for MPC and SAU configuration */
#define NS_PARTITION_START \
            (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_PARTITION_OFFSET))
#define NS_PARTITION_SIZE (FLASH_NS_PARTITION_SIZE)

/* Secondary partition for new images in case of firmware upgrade */
#define SECONDARY_PARTITION_START \
            (NS_ROM_ALIAS(S_IMAGE_SECONDARY_PARTITION_OFFSET))
#define SECONDARY_PARTITION_SIZE (FLASH_S_PARTITION_SIZE + \
                                  FLASH_NS_PARTITION_SIZE)

/* Code SRAM area */
#define TOTAL_CODE_SRAM_SIZE     (0x8000) /* SRAM X region */
#define S_CODE_SRAM_ALIAS_BASE   (0x14000000)
#define NS_CODE_SRAM_ALIAS_BASE  (0x04000000)

#ifdef BL2
/* Bootloader regions */
#define BL2_CODE_START    (S_ROM_ALIAS(FLASH_AREA_BL2_OFFSET))
#define BL2_CODE_SIZE     (FLASH_AREA_BL2_SIZE)
#define BL2_CODE_LIMIT    (BL2_CODE_START + BL2_CODE_SIZE - 1)

#define BL2_DATA_START    (S_RAM_ALIAS(0x0))
#define BL2_DATA_SIZE     (TOTAL_RAM_SIZE)
#define BL2_DATA_LIMIT    (BL2_DATA_START + BL2_DATA_SIZE - 1)
#endif /* BL2 */

/* Shared data area between bootloader and runtime firmware.
 * Shared data area is allocated at the beginning of the RAM, it is overlapping
 * with TF-M Secure code's MSP stack
 */
#define BOOT_TFM_SHARED_DATA_BASE S_RAM_ALIAS_BASE
#define BOOT_TFM_SHARED_DATA_SIZE (0x400)
#define BOOT_TFM_SHARED_DATA_LIMIT (BOOT_TFM_SHARED_DATA_BASE + \
                                    BOOT_TFM_SHARED_DATA_SIZE - 1)

/* RT6xx ROM supports boot from different type NOR flash such as Quad Flash, Octal
Flash, Hyper Flash which connects to FlexSPI pins used by ROM.
For FlexSPI NOR boot, the FCB should be programmed at offset 0x08000400, and the
boot image should be program at offset 0x08001000.
Flash configuration block(FCB) is used to configure the FlexSPI interface during the boot process. */
#define  M_BOOT_FLASH_CONF_START       0x18000400
#define  M_BOOT_FLASH_CONF_SIZE        0x00000200 /* 512-Bytes */

#endif /* __REGION_DEFS_H__ */
