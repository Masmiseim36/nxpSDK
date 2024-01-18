# Copyright 2023 NXP
# All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT CONNFWK_MCUX_SDK_LIB_EXTERNAL)
    add_subdirectory(${CONNFWK_PLATFORM}_sdk)
endif()

if(CONNFWK_FREERTOS)
    # Configure freertos kernel library
    add_subdirectory(freertos-kernel)
endif()

if(CONNFWK_MBEDTLS)
    # Configure mbedtls library
    add_subdirectory(mbedtls)
endif()
