/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

//#include <zephyr.h>
#include "sbl.h"
#include <string.h>
#include "mcuboot_config.h"

//#include "os/os_heap.h"

//#ifdef CONFIG_BOOT_USE_MBEDTLS
#ifdef MCUBOOT_USE_MBED_TLS

#include <mbedtls/platform.h>
#include <mbedtls/memory_buffer_alloc.h>

/*
 * This is the heap for mbed TLS.  The value needed depends on the key
 * size and algorithm used.  For RSA-2048 signing, 8192 bytes seems to be
 * enough. When using RSA-2048-OAEP encryption + RSA-2048 signing, 10240
 * bytes seem to be enough.
 */
//#if !defined(CONFIG_BOOT_ENCRYPT_RSA)
#if !defined(MCUBOOT_ENCRYPT_RSA)
#define CRYPTO_HEAP_SIZE 8192
#else
#define CRYPTO_HEAP_SIZE 10240
#endif

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
static unsigned char mempool[CRYPTO_HEAP_SIZE];
#endif

/*
 * Initialize mbedtls to be able to use the local heap.
 */
void os_heap_init(void)
{
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
    mbedtls_memory_buffer_alloc_init(mempool, sizeof(mempool));
#endif
}
#else
void os_heap_init(void)
{
}
#endif
