# Copyright 2023 NXP
# All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

# Each platform must set FreeRTOS CMake config flags in order to build freertos-kernel library
# FREERTOS_PORT
# FREERTOS_CONFIG_FILE_DIRECTORY
# FREERTOS_HEAP
add_subdirectory(${CONNFWK_SDK_ROOT}/rtos/freertos/freertos-kernel ${PROJECT_BINARY_DIR}/freertos-kernel)

target_link_libraries(connfwk-config INTERFACE freertos_kernel)
