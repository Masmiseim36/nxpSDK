/*
 * Copyright (c) 2017-2019 Arm Limited. All rights reserved.
 * Copyright 2019-2020 NXP. All rights reserved.
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

/**
 * \file platform_base_address.h
 * \brief This file defines all the peripheral base addresses for platform.
 */

#ifndef __PLATFORM_BASE_ADDRESS_H__
#define __PLATFORM_BASE_ADDRESS_H__

#include "platform_regs.h"           /* Platform registers */
#include "MIMXRT595S_cm33.h"

/* Internal Flash memory */
#define FLASH0_BASE_S                 (0x18000000) /* till 0x1FFFFFFF.  FlexSPI memory mapped space with cache and on-the-fly AES decryption (secure access) */
#define FLASH0_BASE_NS                (0x08000000) /* till 0x0FFFFFFF.  FlexSPI memory mapped space with cache and on-the-fly AES decryption (non-secure access) */

#define FLASH0_SIZE                   (FLASH_TOTAL_SIZE)  /* 64 MB */
#define FLASH0_SECTOR_SIZE            (4*1024)  /* Erase command is executed on sector (4K-byte), or block (64K-byte), or whole chip */
#define FLASH0_PAGE_SIZE              (256)  /* Page (256 bytes) basis, or word basis. */
#define FLASH0_PROGRAM_UNIT           FLASH0_PAGE_SIZE         /* Minimum write size */

#endif  /* __PLATFORM_BASE_ADDRESS_H__ */
